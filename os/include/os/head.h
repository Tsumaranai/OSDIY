#ifndef HEAD_H
#define HEAD_H

#include<type.h>

typedef struct des{
	u32bit a,b;
}desc_table;

extern desc_table idt[256],gdt[256];		//global value in head.s

#endif

