#include<stdarg.h>
#include<string.h>
#include<tty.h>

/*asm divide,the result is that n = qoutient  _res = remiander
 *eax = qou ,edx = rem
*/
#define do_div(n,base) ({\
	int _res;\
	asm("divl %4":"=a"(n),"=d"(_res):"0"(n),"1"(0),"r"(base));\
	_res;})


static char buf[1024]={0,};
static int pos = 0;
//pos = position of buf


//base = x_adecimal 
int printn(int num, int base){
	int top,i;
	char tmp[32];
	//32 bit is enough to express the usigned longlong type
	const char* digtials= "0123456789ABCDEF";
	//only suppose hex_adecimal most you can add sth,if you want
	
	if(base < 2 || base > 16)
		return 0;
	top = 0;

	
	if(num < 0 && base == 10){
		buf[pos++] = '-';
		num = -num;
		}

	
	while(num){
		tmp[top++] = digtials[do_div(num,base)];
		}

	
	if(top){
		
		do{
			buf[pos++] = tmp[--top];
			}while(top);
			
		}else{
		buf[pos++] = '0';
		}
	
}


printk(const char* fmt,...){

	va_list args;
	va_start(args,fmt);
	char *str;
	int t,i;
	pos = 0;
	
	while(*fmt){
		
		if(*fmt != '%'){
			buf[pos++] = *fmt++;
			continue;
			}
		fmt++;
		
		switch(*fmt){
			case'c':
				
				buf[pos++] = (unsigned char) va_arg(args,char);
				fmt++;break;

			case's':
				str =  va_arg(args,char *);
				i = strlen(str);
				
				for(t = 0; i > t; t++){
					buf[pos++] = *str++;
					}
				fmt++;break;

			case'o':
			case'O':
				printn(va_arg(args,int), 8);
				fmt++;break;
				
			case'x':
			case'X':
				printn(va_arg(args,int), 16);
				fmt++;break;

			case'd':
				printn(va_arg(args,int), 10);
				fmt++;break;
				
			default:
				if(*fmt != '%')
					buf[pos++] = '%';
				if(*fmt)
					buf[pos++] = *fmt;
				else
					pos--;//Don't imitate Linux,yours is different!!!
				break;
				
			}
		
		}
	buf[pos] = '\0';

	puts(buf);
	
	va_end(args);

	return pos;
}


