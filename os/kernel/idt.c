#include<idt.h>
#include<os/head.h>
#include<asm.h>

extern void *irq_handle_addr[16];

void set_idt_gate(idt_desc* desc, int type, int dpl, void* addr){

	desc->base_low = (u16bit)addr & 0xffff;
	desc->selector = 0x08;	//kernel code segment
	desc->dpl = dpl & 0x3;
	desc->always0 = 0;
	desc->sys = 0;
	desc->p = 1;
	desc->base_high = ((u16bit)addr & 0xffff0000) >> 16;
	desc->type = type & 0x0f;
}

void set_intr_gate(int n, void* addr){
	
	set_idt_gate((idt_desc*)&idt[n], 14, 0, addr);
}

void set_trap_gate(int n, void* addr){

		set_idt_gate((idt_desc*)&idt[n], 15, 0, addr);
}

void set_sys_gate(int n, void* addr){

		set_idt_gate((idt_desc*)&idt[n], 15, 3, addr);
}
void do_irq_handle(u32bit n){

	void (*handle)(void);
	
	handle = irq_handle_addr[n];
	if(handle){
		
		handle();
	}

	if(n >= 8){

		outb(0x20, 0xa0);
		}
	
	outb(0x20, 0x20);
}

void install_irq_handle(u32bit n, void* handle){
	
	irq_handle_addr[n] = handle;
	}

