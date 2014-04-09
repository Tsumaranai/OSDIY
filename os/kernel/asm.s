;asm.s is a file contains hardware error functions
;Those functions are declared in the trap.c and defined in
;this files.

;2013/5/6 coded by tyousan
[bits 32]
;extern do_divide_error,do_debug,do_nmi,do_breakpoint,do_overfolw,do_bounds_check,do_invalid_opcode
;extern do_device_not_available,do_double_fault,do_coprocessor_segment_overrun
;extern do_invalid_TSS,do_segment_not_present,do_stack_segment,do_general_protection
;extern do_page_fault,do_reserved,do_coprocessor_error,do_parallel_interrupt,do_irq13

extern do_handle_function

global divide_error,debug,nmi,breakpoint,overflow,bounds_check,invalid_opcode
global device_not_available,double_fault,coprocessor_segment_overrun
global invalid_TSS,segment_not_present,stack_segment,general_protection
global page_fault,reserved,coprocessor_error,parallel_interrupt,irq13

global irq0,irq1,irq2,irq3,irq4,irq5,irq6,irq7
global irq8,irq9,irq10,irq11,irq12,irq13,irq14,irq15
extern do_irq_handle

;do_handle_function(*reg,E_CODE,NO)

divide_error:					;int error
	push 0					;no error's error code is 0
	push do_handle_function
	push 0						;int no

;the followings are with-error-code interruptions
;	the way of push with-error-code interruptions
;	is different. 
;the situation of stack before jumping to error_code
;	|  SS   |
;	|  ESP  |
;	| EFLAG |
;	|  CS	|
;	|  EIP  |
;	| E_CODE|	<- the CPU add the "error_code" in stack when int happen.
;	| DO_FUN|	<-esp1
;	|  N.O  |	<- the number of int
;the situation of stack after calling handle function
;	|  SS   |
;	|  ESP  |
;	| EFLAG |
;	|  CS	|
;	|  EIP  |
;	| eax(E_CODE)|	<- the CPU add the "error_code" in stack when int happen.
;	| ebx(DO_FUN)|	<-esp1
;	| ecx  (N.O) |	<- the number of int
;	| ...|
;	| ...gs |	<- as a parameter for do_handle_function
;	|	NO	|
;	| E_CODE|
;	|  &reg |
;	|  ...  |
;
;	Illustration
;	

error_code:
	xchg [esp+8],eax  ;eax = E_CODE	;esp = do_fun
	xchg [esp+4],ebx		;ebx = do_fun
	xchg [esp],ecx		;ecx = N.O
	push edx
	push esi
	push edi
	push ebp
	push ds
	push es
	push fs
	push gs
	push ecx	;push NO
	push eax	;push E_CODE
	lea edx,[esp+8]  ;&reg 	(NO and E_CODE) * 4 = 8
	push edx			;&reg
	mov edx,0x10
	mov fs,edx
	mov es,edx
	mov ds,edx
	call [ebx]		;jmp to the handle function
	add esp,12
	pop gs
	pop fs
	pop es
	pop ds
	pop ebp
	pop edi
	pop esi
	pop edx
	pop ecx
	pop ebx
	pop eax
	iret
debug:							;int 1 debug
	push 0
	push do_handle_function
	push 1
	jmp error_code
	
nmi:							;int 2
	push 0
	push do_handle_function
	push 2
	jmp error_code
	
breakpoint:						;int 3
	push 0
	push do_handle_function
	push 3
	jmp error_code
	
overflow:						;int 4
	push 0
	push do_handle_function
	push 4
	jmp error_code
	
bounds_check:					;int 5
	push 0
	push do_handle_function
	push 5
	jmp error_code
	
invalid_opcode:					;int 6
	push 0
	push do_handle_function
	push 6
	jmp error_code
	
coprocessor_segment_overrun:	;int 9
	push 0
	push do_handle_function
	push 9
	jmp error_code
	
reserved:						;int 15
	push 0
	push do_handle_function
	push 15
	jmp error_code	
;the following interruption get the error no when interrupt happen
;;int 8
double_fault:
	push do_handle_function
	push 8
;;int 10
invalid_TSS:
	push do_handle_function
	push 10
	jmp error_code

;;int 11
segment_not_present:
	push do_handle_function
	push 11
	jmp error_code
;;int 12
stack_segment:
	push do_handle_function
	push 12
	jmp error_code
;;int 13
general_protection:
	push do_handle_function
	push 13
	jmp error_code

	
;;the followings are for test, just in case
;;int 7
device_not_available:
	push do_handle_function
	push 7
	jmp error_code
;;int 14
page_fault:
	push do_handle_function
	push 14
	jmp error_code

coprocessor_error:			;int 16
	push do_handle_function
	push 16
	jmp error_code

parallel_interrupt:			;int 39
	push do_handle_function
	push 39
	jmp error_code
;;int 7 int 16 int 14  int 39 will be deleted 
;;the following is the irq.
;int no
;Temp := (SP);
;Push(AX)
;Push(CX)
;Push(DX)
;Push(BX)
;Push(Temp)
;Push(BP)
;Push(SI)
;Push(DI)

irq_routine:

	xchg [esp],eax	;eax=no
	push ecx
	push edx
	push ebx
	push 0			;according the sequence of the pusha,it shuold push a temp,
					;I push 0. However,popa will abandon this(Believe or not , check the 
					;Intel mannul if you wanna).So you can push everything you want.
	push ebp
	push esi
	push edi
	push ds
	push es
	push fs
	push gs
	mov bx,0x10
	mov gs,bx
	mov fs,bx
	mov es,bx
	mov ds,bx
	push eax
	call do_irq_handle
	pop eax
	pop gs
	pop fs
	pop es
	pop ds

	popa

;	add esp,4		;abandon the irq_NO
	iret
	
irq0:
	push 0
	jmp irq_routine
irq1:
	push 1
	jmp irq_routine
irq2:
	push 2
	jmp irq_routine
irq3:
	push 3
	jmp irq_routine
irq4:
	push 4
	jmp irq_routine
irq5:
	push 5
	jmp irq_routine
irq6:
	push 6
	jmp irq_routine
irq7:
	push 7
	jmp irq_routine
irq8:
	push 8
	jmp irq_routine
irq9:
	push 9
	jmp irq_routine
irq10:
	push 10
	jmp irq_routine
irq11:
	push 11
	jmp irq_routine
irq12:
	push 12
	jmp irq_routine
irq13:
	push 13
	jmp irq_routine
irq14:
	push 14
	jmp irq_routine
irq15:
	push 15
	jmp irq_routine
