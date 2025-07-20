#include <terminal_io.h>
#include <paging.h>

extern void jmp_hh_kernel();

void kernel_main() {
    clear_screen();
    kprint("Interrupts have been set up\n");
    init_paging();
    kprint("Paging has been initialized\n");
    enable_paging(page_directory);
    kprint("Paging has been enabled\n");
    kprint("Jumped to high half kernel\n");
    while(1);
    return;
}