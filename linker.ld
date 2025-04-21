ENTRY(kernel_main)

SECTIONS {
    . = 0x1000;  /* Load kernel at 0x1000 (where bootloader loads it) */
    .text : { *(.text*) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) }
}
