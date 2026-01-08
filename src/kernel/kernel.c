#include <terminal_io.h>
#include <paging.h>
#include <allocators.h>

extern char __hh_lma[];      // LMA of .text.hh (address value)
extern char __hh_phys[];     // if you defined it in the linker (ABS value)
extern char __hh_vma[];      // ADDR(.text.hh) if you exported it
extern void hh_kernel(void); // the function (VMA ~ 0xC0……)

void kernel_low() {
    clear_screen();
    kprint("Interrupts have been set up\n");
    // init_paging();
    // kprint("Paging has been initialized\n");
    // enable_paging(page_directory);
    // kprint("Paging has been enabled\n");
    // void *jmp_hh_kernel = (void*)(0xC0000000 + kernel_main); // Address of high half kernel
    asm volatile("jmp *%0" :: "r"(hh_kernel));
    while(1);
    return;
}