
SETUPLEN equ 4

SYSSIZE equ 0x8000   ;now I'm sure that my kernel can't bigger
		;than the 16B * 0x8000 (512KB),and you may have question
		;that why the sysszie is set to be 0x8000
		;Here is the principle:I load the boot on
		;the 0x90000 and the syskernel on the
		;0x10000 ( as what Linus did :-) ),
		;so it means that the syskernel size
		;is 0x90000 - 0x10000 at most,and the 0x8000 is the num of 
		;segment.   PS: yep ,if you're curious guy, pay attention to 
		;the 0x1000,when the BOIS initialized,the interrupts info
		;like vectors and the int programmes 're all set on the 
		;below ,if I load the kernel on the 0x00 at beginning,
		;the info will be ,yeah ,overwrite!!!How to use the 
		;int 0x13 boy~  

BOOTSEG equ 0x07c0

INITSEG equ 0x9000

SETUPSEG equ 0x9020

SYSSEG equ 0x1000

ENDSEG equ SYSSEG + SYSSIZE

start :
	mov ax,BOOTSEG
	mov ds,ax
	
	mov ax,INITSEG
	mov es,ax
	
	mov cx,256
	xor si,si
	xor di,di

	rep movsw	;now I move the boot to the 0x9000
			;256 * 1 word = 512B ,It's first sector
	jmp INITSEG:go

go:	mov ax,cs
	mov ds,ax
	mov es,ax
	
	mov ss,ax
	mov dx,0xf000	;the 0xf000 is an arbitrary value
	mov sp,dx	;just big enough to be a stack~

load_setup:
	xor dx,dx	;drive = 0,head =0
			;if you have on clue on what I'am 
			;doing ,search the int 0x13 on the
			;Internet,you will get all you want
	    		;I'm setting the parameters for int 0x13
	mov cx,0x0002 	;sector 2(first is the boot),track 0
	mov bx,0x0200	;destination  address INITSEG:0x0200
	mov ax,0x0200+SETUPLEN	;ah=2 for service 2,al=SETUPLEN
				;for SETUPLEN sectors
	int 0x13
	
	jnc ok_load_setup
die:	jmp die
	
ok_load_setup:
	
;the following code is that getting the disk
;drive infomation(Linus use the word'parameters',maybe his word
;are more formal,howere I just think my word is more understandable
;ok,ok,whatever...),especially the num of sectors and tracks

	xor dl,dl
	mov ah,0x08	;the 0x08 type service
	int 0x13
	xor ch,ch
	
	
	mov [sectors],cx	;it should be writen as
			;'mov word[sectors],cx' explicitly
			;God may care...I am a lazy boy	

;now I gonna write the code of "moving the code to where it should be..."
;loading the system_kernel (at 0x10000)

	mov ax,SYSSEG
	mov es,ax
	xor bx,bx
	call read_it
	call kill_motor

	jmp SETUPSEG:0	;!!!here the boot is over,and jmp to the setup!!!






	
sectors:DW 0
sread:	DW 1+SETUPLEN	;this parameter is for sectors read of current track
;1 stands for boot the and now you're ready know the SETUPLEN aren't you~:-)

head:	DW 0	;current head
track:	DW 0	;current track

;hey guy listen!The next coming is so complicated and sophisticated literally
;I just wanna say "Linus is the fucking genius!!"

;here we go~
read_it:
	
	mov ax,es
	test ax,0x0fff	;es must be the 64kb boundry
	jne die
	xor bx,bx 	
	
r_read:
	mov ax,es
	cmp ax,ENDSEG	;Have we loaded all yet??
	jb ok1_read
	ret

ok1_read:
	;seg cs
	mov ax,[sectors]
	sub ax,[sread]
	mov cx,ax
	shl cx,9	;cx*512 + bx(the offset in segment)
	add cx,bx
	jnc ok2_read
	je ok2_read	;no carry or no zero, jump and go 
	
	xor ax,ax	;if the result is over,do what it can
			;be loaded at most;
	sub ax,bx	;get the 64KB's complement
	shr ax,9


ok2_read:
	
	call read_track
	
	mov cx,ax
	add ax,[sread]
	;seg cs
	cmp ax,[sectors]	;Have you read the all sectors
				;in this(current) track
	jne ok3_read		;if no goto ok3
;if you have aread done change the head
	mov ax,1
	sub ax,[head]
	jne ok4_read
	inc word[track]
;the above 4 steps,I had to say"touche! touche!!!"

ok4_read:
	mov [head],ax
	xor ax,ax	
;'xor' ax,ax just clean the ax to be zero,In the book of jiongzhao(one of the men
;I admired)his comment on this sentence,is ..sort of ambiguous.I tought what I comment
;on is more rational:-).

ok3_read:
	mov [sread],ax
	shl cx,9
	add bx,cx
	jnc r_read	;if bx > 64Kb ,add the segment 0x10
			;and zero the bx
	
	mov ax,es
	add ah,0x10
	mov es,ax
	xor bx,bx
	jmp r_read

read_track:
	pusha
	mov dx,[track]
	mov cx,[sread]
	inc cx		;the sread is current,so cx++
	
	mov ch,dl
	mov dx,[head]
	mov dh,dl
	and dx,0x0100	;if the dh > 1,that's incorrect

	mov ah,2	;type 2 service

	int 0x13
	popa
	ret
	
kill_motor:		
;Don't ask me why I did like this,I don't know either...
	push dx
	mov dx,0x3f2
	xor al,al
	outsb
	pop dx
	ret
;almostly forget this job
times 510 -($-$$) db 0
	dw 0xAA55	;magic number
