#include<asm/config.h>
#include<os/kernel.h>
#include<type.h>
#include<asm.h>
#include<idt.h>
extern u32bit ticks;
void clock_initial(){

		unsigned int LATCH = 1193180/HZ;//(the time interruput 1193 ticks every second)
		outb(0x36,0x43);	//damn,I typed the wrony port!
		iodelay();
		outb(LATCH&0xff,0x40);
		iodelay();
		outb(LATCH>>8,0x40);
		iodelay();
		outb(inb(0x21)&(~0x01),0x21);
		
}

void time_handle(void){
	
	ticks++;
	//printk("%d\t",ticks);
	
	}
