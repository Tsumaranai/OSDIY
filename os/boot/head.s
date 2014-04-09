;
;		head.s (C) 2013 Tyousan
;head.s cantains the 32-bit startup code.
;head.s will be compiled with the main.c(or other some C language programme)
;and when turned the paging mechanism,it's will be overwriten in the mem
;(It will be covered by the PDE and PTE)

;PS:Linus used the GNU-gas to compile this ,so you can see how weird the GAS syntax is..
[bits 32]

extern main
extern stack_start
extern printk	

global _start
global gdt,idt,pg_dir


pg_dir:
;pay attention to here,here is where the PTE 's supposed to be loaded

_start :	;_start is symbol that can be recognized by ld as a entry
	mov eax,0x10	;0x10 is the selector
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	lss esp,[stack_start]

	call setup_idt
	call setup_gdt

;it's necessary to reset those segment registers,because after setup the gdt
;the limit is changed from 8MB to 32MB.and the CS should be reset too,but Linus
;didn't,that's a bug.But Linux-0.12's kernel is far smaller than 8MB,so this bug
;didn't invoke the mistake,and In the contemporary version change the code
;,they add a 'jmp' command "ljmp $(__KERNEL_CS),$1f" to reset the CS.
	
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	
	lss esp,[stack_start]

;test the and make sure that is the A20 ?
	xor eax,eax
	inc eax
	mov [0x000000],eax
	cmp [0x100000],eax
n:	je n	;loop forever if it's

	
	jmp after_page_table


setup_idt:
	lea edx,[ignore_int]
	mov eax,0x00080000
	mov ax,dx
	mov dx,0x8e00
	lea edi,[idt]
	mov ecx,256
rp_sidt:
	mov [edi],eax
	mov [edi+4],edx
	add edi,8
	dec ecx
	jnz rp_sidt
	
	lidt[idt_descr]
	ret

setup_gdt:
	lgdt [gdt_descr]
	ret
;I put the (PT here),in linux-0.12,he just set 4page table (16MB)
;I changed it ,Cuz ,I got 32MB in my virtual PC,
times 0x1000-($ - $$) db 0

pg0:

times 0x1000 db 0

pg1:
times 0x1000 db 0

pg2:
times 0x1000 db 0

pg3:
times 0x1000 db 0

pg4:
times 0x1000 db 0

pg5:
times 0x1000 db 0

pg6:
times 0x1000 db 0

pg7:
times 0x1000 db 0


after_page_table:
	push 0
	push 0
	push 0
	push L6
	push main

	jmp setup_paging
L6:
	jmp L6 ;main never return here,Just in case
		;I don't konw why Linus called it 'L6'

;int_msg: db 'Unknow interrupt',13,10
align 4
ignore_int:
	push eax
	push ecx
	push edx
	push ds
	push es
	push fs
	push gs
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	;push int_msg
	;call printk
	;pop eax
	pop gs
	pop fs
	pop es
	pop ds
	pop edx
	pop ecx
	pop eax
	
	iret

align 4

setup_paging:
	mov ecx,1024*9	;(5 pages(4kb),pg_dir+8*page table)
	xor eax,eax
	xor edi,edi
	
	cld	;CLean Direction flag
	rep
	stosd	;copy the [eax] to ES:[edi],then dei+=4;

	mov dword[pg_dir],pg0+7	  ;0x7 set hte present bit/user r/w
	mov dword[pg_dir+4],pg1+7
	mov dword[pg_dir+8],pg2+7
	mov dword[pg_dir+12],pg3+7
	mov dword[pg_dir+16],pg4+7
	mov dword[pg_dir+20],pg5+7
	mov dword[pg_dir+24],pg6+7
	mov dword[pg_dir+28],pg7+7
	
	mov edi,pg7+4092
	mov eax,0x1fff007 ;(8pages*1024 -1)*4096+7
	std
n1	stosd
	sub eax,0x1000	;set one item of paging table address - 0x1000
	jge n1
	
	xor eax,eax
	mov cr3,eax	;cr3 is the base of page directory

	mov eax,cr0
	or eax,0x80000000	;set the paging(PG)bit
	mov cr0,eax

	ret	;NOTE this is a phenmenal step,remember the four push
		;after"after_page_table",this command pop the main out
		;and go the main!(WOW!!exciting moment,isn't it),start
		;the C language.now the this program is end here!

align 4
	dw 0
idt_descr:
	dw 256*8-1	;idt contains 256 entries(length of idt)
	dd idt		;32bit base_address

align 4
	dw 0
	
gdt_descr:
	dw 256*8-1
	dd gdt
align 8
idt: times 256 dq 0	;set the idt(or uninitialized the idt)

gdt:
	dw 0,0,0,0	;NULL descriptor

	dw 0xffff	;granularity is up,so limit is (0x1fff+1)*4KB=32MB
	dw 0x0000
	dw 0x9a00
	dw 0x00c0
	
	dw 0xffff	;granularity is up,so limit is (0x1fff+1)*4KB=32MB
	dw 0x0000
	dw 0x9200
	dw 0x00c0
	 
	times 253 dq 0	;(256 - 3)*dq(8B)
