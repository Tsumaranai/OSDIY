#include<asm.h>
#include<string.h>
#include<tty.h>

#define ORIG_X 0
#define ORIG_Y 0

#define VIDEO_BASE 0xb8000

#define v_column 80
#define v_row 25
#define num_char v_column * v_row

#define null_char 0x0700
//wihte charactor black backgroud
#define wGb 0x07
#define SCRN_POS ((con_x + con_y*v_column)<<1)
//offset of memory

unsigned int con_x = 0;
unsigned int con_y = 0;


static void set_cursor(){
	cli();
	outb(14,0x3D4);
	iodelay();
	outb(SCRN_POS>>9,0x3D5);
	iodelay();
	outb(15,0x3D4);
	iodelay();
	outb(SCRN_POS>>1,0x3D5);
	sti();
	}
static void scrup(){

	memcpy((void *)(VIDEO_BASE), (const void *)(VIDEO_BASE + (v_column << 1)),
		v_column * (v_row - 1) * 2);
	memsetw((void *)(VIDEO_BASE + SCRN_POS), 0, v_column);
}

void cln_scrn(){
	asm("cld;"
	"rep;"
	"stosw;"
	::"a" (null_char),
	"c" (num_char),
	"D" (VIDEO_BASE)
		);
	con_x = 0;
	con_y = 0;
	set_cursor();
}



void lead_column(){
	
	con_x = 0;
	
}

void next_row(){

	if(con_y + 1 < v_row){
		
		con_y++;
		
		}else{
		
		scrup();
		
		}
}

void pt_table(){

	int t;
	t = con_x % 4;
	if((con_x + 4 - t) >= (v_column)){
		
		memsetw((void *)(VIDEO_BASE + SCRN_POS),(0x20|wGb << 8), (v_column - con_x));
		lead_column();
		next_row();
		
		}else{
	
		memsetw((void *)(VIDEO_BASE + SCRN_POS), (0x20|wGb << 8),(4 - t));
		con_x += (4 - t);
		
		}
}

void puts(const char* buf){
	int t = 0;
	char c;
	while(c = buf[t++]){
		
		if(c == '\n'){
			
			lead_column();
			next_row();
			continue;
			}
		if(c == '\r'){

			lead_column();
			continue;
			}
		if(c == '\t'){

			pt_table();
			continue;
			}
		if(con_x  >= v_column ){
			lead_column();
			next_row();
			}
		asm("movw %%ax,%0"
			:"=m"(*(short *)(VIDEO_BASE + SCRN_POS))
			:"a"(c|wGb << 8)
			
			);
		con_x++;
		}
	set_cursor();
}




