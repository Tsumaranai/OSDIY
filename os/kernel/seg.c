#include<gdt.h>
#include<os/head.h>
void set_seg_desc(seg_descriptor* seg_addr,int dpl,int limit,int type,int base){

	seg_addr->limit_low = limit & 0xffff;
	seg_addr->base_low = base & 0xffff;

	seg_addr->base_mid = ( base & 0x00ff0000 ) >> 16;
	seg_addr->type = type & 0xf;
	seg_addr->sys = 1;
	seg_addr->dpl = dpl & 0x3;
	seg_addr->p = 1;
	seg_addr->limit_high = (limit & 0xf0000) >> 16;
	seg_addr->AVL = 0;
	seg_addr->reserve = 0;
	seg_addr->BD = 1;
	seg_addr->granulation = 1;
	seg_addr->base_high = (base & 0xff000000) >> 24;
	
}
void set_param_ldt(seg_descriptor* seg_addr){

	seg_addr->granulation = 0;
	/*granulation=0 the unit = 1B;
	* 3 descriptor(zero(dummy),Data seg,code seg) in the ldt table ;
	*every desc is 64 bit,so the limit = 64 * 3 /8=24;0x20 just in case;
	*/
	seg_addr->limit_low = 0x20;
	seg_addr->sys = 0;
}
void set_ldt(int n, int base){

	set_seg_desc((seg_descriptor*)&gdt[n] , 0, 0, STS_LDT, base);
	set_param_ldt((seg_descriptor*)&gdt[n]);
}

void set_param_tss(seg_descriptor* seg_addr){

	seg_addr->granulation = 0;
	//the size of tss is 0x68
	seg_addr->limit_low = 0x68;
	seg_addr->sys = 0;
}


void set_tss(int n, int base){

	set_seg_desc((seg_descriptor*)&gdt[n], 0, 0, STS_TA, base);
	set_param_tss((seg_descriptor*)&gdt[n]);
	}

