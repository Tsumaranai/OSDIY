#ifndef STRING_H
#define STRING_H
#include<type.h>

int strlen(const char* sptr);

void *memcpy(void *dest, const void *source, unsigned int num);

void *memsetw(void *source, u16bit comment, unsigned int num);

void *memsetb(void *source, u8bit comment, unsigned int num);
	

#endif
