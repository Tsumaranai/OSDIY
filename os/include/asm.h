#ifndef ASM_H
#define ASM_H

#define sti() asm("sti"::)
#define cli() asm("cli"::)
#define nop() asm ("nop"::)
#define iret() asm ("iret"::)


#define iodelay() asm("jmp 1f;""1:jmp 1f;""1:")

#define outb(value,port) \
	asm volatile("outb %%al,%%dx;"::"a" (value),"d" (port))

#define inb(port) ({\
	unsigned char _v;\
	asm volatile ("inb %%dx,%%al":"=a" (_v):"d" (port));\
	_v;\
	})

/*#define outw(value,port) \
	asm("outw")*/




#endif

