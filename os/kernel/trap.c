//code modified from Linux-0.11

#include<os/kernel.h>
#include<os/head.h>
#include<idt.h>
#include<asm.h>

void *irq_handle_addr[16]={
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};
static char *trap_str[] = {    
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",    
	"Segment Not Present",    
	"Stack Fault",    
	"General Protection Fault",    
	"Page Fault",    
	"Unknown Interrupt",     
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check", //19
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",		//32
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"parallel_interrupt",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"irq13",		//45
};


void divide_error(void);			//int0

void debug(void);		//int1

void nmi(void);	//non markable interruption	int 2

void breakpoint(void);	//int 3	sys

void overflow(void);		//int 4	sys

void bounds_check(void);	//int 5	sys

void invalid_opcode(void);		//int 6

void device_not_available(void);	//int 7	

void double_fault(void);		//int 8

void coprocessor_segment_overrun(void);//int 9

void invalid_TSS(void); //int 10

void segment_not_present(void);	//int 11

void stack_segment(void);	//int 12

void general_protection(void);	//int 13

void page_fault(void);	//int 14		

void reserved(void);			//int 15

void coprocessor_error(void);		//int 16	

void parallel_interrupt(void);		//int 39

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);	//int 45
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);


//void alignment_check(void);	//int 19

void do_handle_function(regs * r, int error_code, int int_no){

	printk("%s,ecode=0x%x\n",trap_str[int_no],error_code);
	printk("eax=0x%x,ebx=0x%x,ecx=0x%x,edx=0x%x\n",r->eax,r->ebx,r->ecx,r->edx);
	printk("cs=0x%x,eip=0x%x,eflag=0x%x\n",r->cs,r->eip,r->eflag);
	
}
void trap_init(void){

	int i;

	set_trap_gate(0,&divide_error);	
	set_trap_gate(1,&debug);	
	set_trap_gate(2,&nmi);
	set_sys_gate(3,&breakpoint);	/* int3-5 can be called from all */	
	set_sys_gate(4,&overflow);
	set_sys_gate(5,&bounds_check);	
	set_trap_gate(6,&invalid_opcode);	
	set_trap_gate(7,&device_not_available);
	set_trap_gate(8,&double_fault);	
	set_trap_gate(9,&coprocessor_segment_overrun);	
	set_trap_gate(10,&invalid_TSS);	
	set_trap_gate(11,&segment_not_present);	
	set_trap_gate(12,&stack_segment);	
	set_trap_gate(13,&general_protection);	
	set_trap_gate(14,&page_fault);	
	set_trap_gate(15,&reserved);	
	set_trap_gate(16,&coprocessor_error);

	
		for (i=17;i<48;i++)		
			set_trap_gate(i,&reserved);

	set_intr_gate(32,&irq0);
	set_intr_gate(33,&irq1);
	set_intr_gate(34,&irq2);
	set_intr_gate(35,&irq3);
	set_intr_gate(36,&irq4);
	set_intr_gate(37,&irq5);
	set_intr_gate(38,&irq6);
	set_intr_gate(39,&irq7);
	set_intr_gate(40,&irq8);
	set_intr_gate(41,&irq9);
	set_intr_gate(42,&irq10);
	set_intr_gate(43,&irq11);
	set_intr_gate(44,&irq12);
	set_intr_gate(45,&irq13);
	set_intr_gate(46,&irq14);
	set_intr_gate(47,&irq15);
	//set_trap_gate(39,&parallel_interrupt);
	//set_trap_gate(45,&irq13);	//45 - 32 = 13
		
}
