;
;	setup.s		(c)2013 Tyousan
;Still remember where I put the system_kernel on?(0x10000)
;setup.s is responsible for loading the system_kernel to the right place
;yep,somewhere they belong,0x0,and setting a temporary gdt and ldt,then
;turning the segment mechanism on.

INITSEG equ 0x9000
SYSSEG equ 0x1000
SETUPSEG equ 0x9020

start:
	mov ax,INITSEG
	mov ds,ax
;int 0x15 type 0x88,is the service for getting the size of memory (extended mem KB)
	mov ah,0x88
	int 0x15
	mov [2],ax	;put [ax] into 0x90002


;now we're gonna move to the protected mode...
;
	cli		;no interrupts allowed

;mov the system kernel to the right place(0x0)

	mov ax,0x0
	cld	;'direction' = 0,movs moves forward
;BTY std movs moves backward

do_move:
	mov es,ax
	add ax,0x1000
	cmp ax,INITSEG	;is it beyond the 0x9000?
	jz load_dt
	mov ds,ax
	xor di,di
	xor si,si
	mov cx,0x8000	;0x8000 * 2B = 2^16B = 64KB = 1 seg
	rep movsw
	
	jmp do_move
	
load_dt:
	mov ax,SETUPSEG
	mov ds,ax
	lidt [idt_48]
	lgdt [gdt_48]
	jmp EA20
gdt:
	dw 0,0,0,0	;first is dummy
	
	dw 0x07ff
	dw 0x0000
	dw 0x9a00
	dw 0x00c0
	
	dw 0x07ff
	dw 0x0000
	dw 0x9200
	dw 0x00c0
gdt_48:			;48 stands for 48bits,is the length of gdt register
	dw 0x800	;gdt limit 0x800,can contian 
				;0x800/8B(lenght of descriptor) = 256 items 
	dw 512+gdt,0x9
idt_48:
	dw 0
	dw 0,0

;now we enable the A20(turn the protected mode on)
EA20:	call empty_8042
	mov al,0xd1
	out 0x64,al
	call empty_8042
	mov al,0xdf
	out 0x60,al
	call empty_8042
	jmp RP_8259
empty_8042:
	dw 0x00eb,0x00eb	;just for delay
	in al,0x64	;8042 status port
	test al,2	;is input buffer full?
	jnz empty_8042	;if it's yes,go clean
	ret

;well,well,well,Now we have to reprogram the interrupts :-(
;we put them right after the intel-reserved hardware interrupts
;at the int 0x20-0x2f.there they won't mess up anything
;the above is what the Linus said.I,of cause ,don't have
;any rejections.:->,If you really wanna figure it out ,
;surf the Intelnet,google will tell you anything 

;okay,now start it,reprogram the 8259's,it does isn't fun

RP_8259:
	mov al,0x11
	out 0x20,al
	dw 0x00eb,0x00eb
	
	out 0xa0,al
	dw 0x00eb,0x00eb
	
	mov al,0x20		;reset 0x20 as IBM BIOS(irq0-irq7) 0x20--0x27
	out 0x21,al
	dw 0x00eb,0x00eb
	
	mov al,0x28		;reset 0x28 as IBM BIOS (irq8-irq15)0x28--0x2f
	out 0xa1,al
	dw 0x00eb,0x00eb
	
	mov al,0x04
	out 0x21,al
	dw 0x00eb,0x00eb
	
	mov al,0x02
	out 0xa1,al
	dw 0x00eb,0x00eb
	
	mov al,0x01
	out 0x21,al
	dw 0x00eb,0x00eb
	
	out 0xa1,al
	dw 0x00eb,0x00eb

	mov al,0xff
	out 0x21,al
	dw 0x00eb,0x00eb
	
	out 0xa1,al


;holy crap!!finally...It's certianly wasn't fun :-< .hopefully,it works
;yep,like what Linus said ,This is how REAL programmers do it...

	mov ax,0x0001	;set the PE bit up
	lmsw ax		;load the machine stutas word,up the PE
	jmp 8:0 	;here we go,jmp offset 0 of segment 8(actually it's selector 8)

