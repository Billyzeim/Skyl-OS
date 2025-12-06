KERNEL_VMA = 0xC0000000;   /* The virtual base for the higher-half section */

/* ENTRY(kernel_low_asm) */

SECTIONS
{
	. = 0x10000;

	.text : ALIGN(4K)
	{
        *(.text.start)
		*(.text)
	}

	.rodata : ALIGN(4K)
	{
		*(.rodata*)
	}

	.data : ALIGN(4K)
	{
		*(.data)
	}

	.bss : ALIGN(4K)
	{
		*(.bss*)
	}

	.stack : ALIGN(4K)
	{
		__stack_bottom = .;
		. = . + 0x80000; /* 32 KiB should be fine for now, will manage in C later */
		__stack_top = .;
	}

	PROVIDE(__hh_phys = __hh_lma + (hh_kernel - KERNEL_VMA));
	
	/* --- Higher-half kernel entry --- */
	__hh_lma = ALIGN(LOADADDR(.data) + SIZEOF(.data), 0x1000);

	.text.hh KERNEL_VMA : AT(__hh_lma)
	{
		KEEP(*(.text.hh))             /* put hh_kernel here */
		PROVIDE(__hh_kernel = hh_kernel); /* export its virtual address */
	}
}
