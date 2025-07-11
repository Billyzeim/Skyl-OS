ENTRY(kernel_init)

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
}