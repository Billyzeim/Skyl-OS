ENTRY(kernel_low)                 /* <-- use your real low entry symbol */

KERNEL_LMA = 0x0010000;           /* where BIOS loader puts the file */
KERNEL_VMA = 0xC0000000;          /* higher-half virtual base */

SECTIONS
{
  /* ---------- Low identity trampoline (phys=virt) ---------- */
  . = KERNEL_LMA;
  __tramp_start = .;
  .tramp : AT(ADDR(.tramp))
  {
    KEEP(*(.tramp.entry))         /* low entry goes here */
    *(.tramp .tramp.*)
    *(.text.tramp .text.tramp.*)
    *(.rodata.tramp .rodata.tramp.*)
    *(.data.tramp .data.tramp.*)
    *(.bss.tramp .bss.tramp.*)
  }
  __tramp_end = .;

  /* Next free PHYSICAL address for the high kernel payload */
  . = ALIGN(0x1000);
  __kernel_lma_start = .;

  /* ---------- Switch to high virtual address ---------- */
  . = KERNEL_VMA;
  __kernel_vma_start = .;

  /* text: VMA high, LMA chained after .tramp */
  __text_lma = __kernel_lma_start;
  .text : AT(__text_lma)
  {
    KEEP(*(.text.start))          /* tiny 32-bit stub (if you have one) */
    *(.text .text.*)
  }

  /* rodata: LMA after text's actual load */
  . = ALIGN(0x1000);
  __rodata_lma = LOADADDR(.text) + SIZEOF(.text);
  .rodata : AT(__rodata_lma)
  {
    *(.rodata .rodata.*)
  }

  /* data: LMA after rodata's actual load */
  . = ALIGN(0x1000);
  __data_lma = LOADADDR(.rodata) + SIZEOF(.rodata);
  .data : AT(__data_lma)
  {
    *(.data .data.*)
  }

  /* bss: NOLOAD (still compute end for convenience) */
  . = ALIGN(0x1000);
  __bss_lma = LOADADDR(.data) + SIZEOF(.data);
  .bss (NOLOAD) :
  {
    __bss_start = .;
    *(.bss .bss.* COMMON)
    __bss_end = .;
  }

  . = ALIGN(0x1000);
  __kernel_vma_end = .;
  __kernel_lma_end = __bss_lma + SIZEOF(.bss);

  /* Optional: if you insist on linker-made stack, make it NOLOAD */
  . = ALIGN(0x1000);
  .stack (NOLOAD) :
  {
    __stack_bottom = .;
    . = . + 0x8000;               /* 32 KiB */
    __stack_top = .;
  }

  /* Drop unwind noise to avoid surprises */
  /DISCARD/ : {
    *(.eh_frame) *(.eh_frame_hdr) *(.gcc_except_table*)
    *(.gnu.extab*) *(.gnu.linkonce.eh_frame*)
  }
}
