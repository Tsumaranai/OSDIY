#include<asm.h>
#include<idt.h>
#include<type.h>
#include<os/kernel.h>

static u8bit shift_p;
static u8bit ctrl_p;
static u8bit alt_p;
static u8bit scan_code;

static const char key_print_map[0x3a][2]={
/*00*/{0x0, 0x0}, {0x0, 0x0}, {'1', '!'}, {'2', '@'}, 
/*04*/{'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, 
/*08*/{'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'},
/*0c*/{'-', '_'}, {'=', '+'}, {'\b','\b'},{'\t','\t'},
/*10*/{'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'},
/*14*/{'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'},
/*18*/{'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'},
/*1c*/{'\n','\n'},{0x0, 0x0}, {'a', 'A'}, {'s', 'S'},
/*20*/{'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'},
/*24*/{'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'},
/*28*/{'\'','\"'},{'`', '~'}, {0x0, 0x0}, {'\\','|'}, 
/*2c*/{'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, 
/*30*/{'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'},
/*34*/{'.', '>'}, {'/', '?'}, {0x0, 0x0}, {'*', '*'},
/*38*/{0x0, 0x0}, {' ', ' '} };

void do_print(void){

	/*the code of  releasing action is defferent to pressing action in 8th bit, the
	releasing action code's tth bit is 1, So if we encount this action, don't deal 
	is situation.
	*/
	int capital;
	if((scan_code & 0x80))
		return;

	capital = shift_p;		//just program like that, I'll add the capslk in future.

	printk("%c",key_print_map[scan_code & (~0x80)][capital]);
	//printk("press\n");
	
}

void do_shift(void){

	shift_p &= 0x1;		//just in case 
	shift_p ^= 0x1;
}

void do_alt(void){
	alt_p &= 0x1;
	alt_p ^= 0x1;
}

void do_ctrl(void){
	ctrl_p &= 0x1;
	ctrl_p ^= 0x1;
}

void do_fn(void){
	
}

void do_not(void){
	
}

void *key_way[0x80] = {
                /*00*/&do_not, &do_not, &do_print, &do_print, &do_print, &do_print, &do_print, &do_print,
                /*08*/&do_print, &do_print, &do_print, &do_print, &do_print, &do_print, &do_print, &do_print,
                /*10*/&do_print, &do_print, &do_print, &do_print, &do_print, &do_print, &do_print, &do_print,
                /*18*/&do_print, &do_print, &do_print, &do_print, &do_print, &do_ctrl, &do_print, &do_print,
                /*20*/&do_print, &do_print, &do_print, &do_print, &do_print, &do_print, &do_print, &do_print,
                /*28*/&do_print, &do_print, &do_shift, &do_print, &do_print, &do_print, &do_print, &do_print,
                /*30*/&do_print, &do_print, &do_print, &do_print, &do_print, &do_print, &do_shift, &do_print,
                /*38*/&do_alt, &do_print, &do_not, &do_fn, &do_fn, &do_fn, &do_fn, &do_fn,
                /*40*/&do_fn, &do_fn, &do_fn, &do_fn, &do_fn, &do_not, &do_not, &do_not,
                /*48*/&do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not,
                /*50*/&do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_fn,
                /*58*/&do_fn, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not,
                /*60*/&do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not,
                /*68*/&do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not,
                /*70*/&do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not,
                /*78*/&do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not, &do_not,
        };

void do_kb(void) {

	int com;
	void (*key_fun)(void);
	scan_code = inb(0x60);
	
    key_fun = key_way[scan_code & 0x7f];
	key_fun();

	com = inb(0x61)|0x80;
	outb(com,0x61);
	outb(com & 0x7f,0x61);
}

void kb_initial(void){

	//enable irq2 in PIC1
	outb(inb(0x21)& (~0x2),0x21);
	}

