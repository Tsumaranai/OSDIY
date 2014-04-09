#ifndef GDT_H
#define GDT_H

#include<type.h>

typedef struct _seg_descriptor{

	u16bit limit_low;
	u16bit base_low;
	u8bit base_mid;
	u8bit type:4;
	u8bit sys:1;
	u8bit dpl:2;
	u8bit p:1;
	u8bit limit_high:4;
	u8bit AVL:1;
	u8bit reserve:1; //constant 0
	u8bit BD:1;
	u8bit granulation:1;
	u8bit base_high;
}seg_descriptor;



#define STA_X       0x8     // Executable segment
#define STA_E       0x4     // Expand down (non-executable segments)
#define STA_C       0x4     // Conforming code segment (executable only)
#define STA_W       0x2     // Writeable (non-executable segments)
#define STA_R       0x2     // Readable (executable segments)
#define STA_A       0x1     // Accessed
// System segment type bits
#define STS_LDT     0x2     // Local Descriptor Table
#define STS_TG      0x5     // Task Gate / Coum Transmitions
#define STS_TA      0x9     // Available 32-bit TSS
#define STS_TB      0xB     // Busy 32-bit TSS
#define STS_CG      0xC     // 32-bit Call Gate
#define STS_IG      0xE     // 32-bit Interrupt Gate
#define STS_TRG     0xF     // 32-bit Trap Gate
/* selectors */
//3 is the dpl
#define KERN_CS (1<<3)
#define KERN_DS (2<<3)
#define USER_CS ((3<<3)|3)
#define USER_DS ((4<<3)|3)

#endif


void set_seg_desc(seg_descriptor* gate_addr,int dpl,int limit,int type,int base);
void set_ldt(int n,int base);
void set_tss(int n,int base);
