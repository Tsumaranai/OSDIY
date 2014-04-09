
#/os/Makefile	(C)Tyousan 2013


AS =nasm

ASE =nasm -felf

LD =ld -s -M

GCC =gcc

CFLAGS =-Wall -O -fno-ident -I include  -fno-builtin -fno-stack-protector
#-oformat=a.out-i386-linux

OBJ =objcopy -R .eh_frame -R .comment

all: Image

Image:boot/boot.o boot/setup.o boot/head tool/build
	tool/build boot/boot.o \
	 boot/setup.o boot/head > Image
    
tool/bulid: /os/tool/build.c
	gcc -o tool/build tool/build.c


boot/head: boot/head.o kernel/kernel.o
	$(LD) -o boot/head -T tool/head.ld \
	boot/head.o kernel/kernel.o
boot/setup.o: boot/setup.s
	$(AS) -o boot/setup.o boot/setup.s

boot/boot.o: boot/boot.s
	$(AS) -o boot/boot.o boot/boot.s
boot/head.o:boot/head.s
	$(ASE) -o boot/head.o boot/head.s
	$(OBJ) boot/head.o boot/head.o 	
kernel/kernel.o:
	(cd kernel; make)

clean:
	rm -f boot/*.o boot/head
	(cd kernel;make clean)
