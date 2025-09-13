#include <terminal_io.h>
#include <paging.h>
#include <allocators.h>

void kernel_main() {
    kprint("Jump to higher half kernel completed\n");
    printf("Value: %X\n", *(volatile unsigned int*)0xC0000000);
    unmap_page(first_page_table, 0); // Reset the first page directory entry
    reload_cr3(); // Reload CR3 to apply changes

    // printf("Value: %X\n", *(volatile unsigned int*)0xC0000000);
    void* page1 = alloc_page();
    void* page2 = alloc_page();
    printf("Allocated pages at: %d and %d\n", page1, page2);

    free_page(page1);
    void* page3 = alloc_page();
    printf("Reused page: %d\n", page3);

    int a;
    while (1) {
        printf("Enter a number: ");
        scanf("%d", &a);
        printf("\nYou entered: %d\n", a);
    }

    while(1);
}

void kernel_low() {
    clear_screen();
    kprint("Interrupts have been set up\n");
    init_paging();
    kprint("Paging has been initialized\n");
    enable_paging(page_directory);
    kprint("Paging has been enabled\n");
    kprint("Jumped to high half kernel\n");
    void *jmp_hh_kernel = (void*)(0xC0000000 + kernel_main); // Address of high half kernel
    asm volatile (
        "jmp *%0"
        :
        : "r"(jmp_hh_kernel)
    );
    while(1);
    return;
}