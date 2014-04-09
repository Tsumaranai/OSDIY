OUTPUT_FORMAT("a.out-i386-linux")
SECTIONS
{
	.text : { *(.text) *(.rodata) *(.rodata.*) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
