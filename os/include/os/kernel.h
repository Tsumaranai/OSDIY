#ifndef KERNEL_H
#define KERNEL_H

void printk(const char * fmt,...);

void cln_scrn();

void puts(const char * buf);

void trap_init(void);

void clock_initial(void);

void time_handle(void);

void kb_initial(void);

void do_kb(void);

#endif