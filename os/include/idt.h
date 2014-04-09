#ifndef IDT_H
#define IDT_H

#include<type.h>
typedef struct _idt_desc{

	u16bit base_low:16;
	u16bit selector:16;
	u8bit always0:8;
	u8bit type:4;
	u8bit sys:1;	//sys is 0 system desc ,1 is D/B desc so the sys is always 0
	u8bit dpl:2;
	u8bit p:1;
	u16bit base_high:16;
}idt_desc;


typedef struct _regs{
	u32bit gs,fs,es,ds;
	u32bit bp,di,si,edx,ecx,ebx,eax;
	u32bit eip,cs,eflag,esp,ss;		//items of stack just after interrupt
}regs;


#endif

void set_trap_gate(int n,void * addr);
void set_sys_gate(int n,void * addr);
void set_intr_gate(int n,void * addr);
void do_irq_handle(u32bit n);
void install_irq_handle(u32bit n,void* handle);