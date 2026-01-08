ENTRY(entry)

KERNEL_VMA = 0xC0000000;
KERNEL_LMA = 0x10000;

SECTIONS
{
    . = KERNEL_LMA;
    __tramp_phys_start = .;

    .tramp : {
        KEEP(*(.tramp.entry))
        *(.tramp.text)
        *(.tramp.rodata)
        *(.tramp.data)
        *(.tramp.bss)
        . = ALIGN(4K);
    }
    __tramp_phys_end = .;

    __kernel_lma_start = __tramp_phys_end;
    
    . = KERNEL_VMA + __kernel_lma_start;

    .text : AT(__kernel_lma_start) ALIGN(4K)
    {
        *(.text)
        *(.text.*)
    }

    .rodata : AT(ADDR(.rodata) - KERNEL_VMA) ALIGN(4K)
    {
        *(.rodata*)
    }

    .data : AT(ADDR(.data) - KERNEL_VMA) ALIGN(4K)
    {
        *(.data)
    }

    .bss : AT(ADDR(.bss) - KERNEL_VMA) ALIGN(4K)
    {
        *(.bss*)
        *(COMMON)
    }

    .stack : AT(ADDR(.stack) - KERNEL_VMA) ALIGN(4K)
    {
        __stack_bottom = .;
        . = . + 0x8000; 
        __stack_top = .;
    }

    __kernel_phys_end = ADDR(.stack) + SIZEOF(.stack) - KERNEL_VMA;
}