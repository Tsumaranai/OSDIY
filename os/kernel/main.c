#include<os/kernel.h>
#include<type.h>
#include<asm.h>
#include<idt.h>
extern void cln_scrn();

long user_stack[1024] = {0,};

struct {
	long *a;
	short b;
}stack_start = { &user_stack[1024], 0x10};

u32bit ticks=0;


void main(){
	
	int i;	

	
	trap_init();
	clock_initial();
	kb_initial();
	install_irq_handle(0,&time_handle);
	install_irq_handle(1,&do_kb);
	sti();
	cln_scrn();
	for(i = 0 ; i < 70 ; i++)
	printk("Hello world!!%s\t","hope it success~");
}
