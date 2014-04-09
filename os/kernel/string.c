#include<string.h>
#include<type.h>
int strlen(const char* sptr){
	char *s;
	int len;
	s = sptr;
	for(len = 0; *s; s++)
		len++;
	return len;
}

void *memcpy(void * dest,const void * src,unsigned int num){

	asm("cld;"
		"rep;"
		"movsb;"
		::"D"(dest),"S"(src),"c"(num)
		);
	return dest;
}

void *memsetw(void * src,u16bit comment,unsigned int num){

	asm("cld;"
		"rep;"
		"stosw;"
		:"=D"(src):"a"(comment),"D"(src),"c"(num) 	//the stosw = mov es:[di],eax...
		);
	return src;
	
}

void *memsetb(void * src,u8bit comment,unsigned int num){

	asm("cld;"
		"rep;"
		"stosb;"
		:"=D"(src):"a"(comment),"D"(src),"c"(num)
		);
	return src;
}

