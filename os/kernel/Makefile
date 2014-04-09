
#/os/kernel/Makefile	(C)Tyousan 2013


AS =nasm

ASE =nasm -f elf

LD =ld -s -r

GCC =gcc

CFLAGS =-Wall -O -fno-ident -I../include -fno-builtin -fno-stack-protector
#-oformat=a.out-i386-linux
OBJCOPY =objcopy -R .eh_frame -R .comment

OBJ = idt.o main.o print.o seg.o Trap.o vga.o string.o clock.o keyboard.o

all: kernel.o

kernel.o:$(OBJ)
	$(LD) $(OBJ) -o kernel.o 
	$(OBJCOPY) kernel.o kernel.o

main.o:main.c
	$(GCC) $(CFLAGS) -c $< -o $@

vga.o:vga.c
	$(GCC) $(CFLAGS) -c $< -o $@
	
idt.o:idt.c
	$(GCC) $(CFLAGS) -c $< -o $@
	
print.o:print.c
	$(GCC) $(CFLAGS) -c $< -o $@

seg.o:seg.c
	$(GCC) $(CFLAGS) -c $< -o $@

string.o:string.c
	$(GCC) $(CFLAGS) -c $< -o $@
	
Trap.o:trap.o asm.o
	$(LD) trap.o asm.o -o Trap.o

trap.o:trap.c
	$(GCC) $(CFLAGS) -c $< -o $@

clock.o:clock.c
	$(GCC) $(CFLAGS) -c $< -o $@

keyboard.o:keyboard.c
	$(GCC) $(CFLAGS) -c $< -o $@
	
asm.o:asm.s
	$(ASE) $< -o $@

clean:
	rm -f *.o
