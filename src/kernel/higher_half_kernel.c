#include <stdint.h>
#include <terminal_io.h>
#include <paging.h>
#include <allocators.h>

#define PAGE_SIZE       4096
#define KERNEL_VMA      0xC0000000
#define HIGH_STACK_TOP  0xC0400000  /* top of first 4MiB high window; stack grows down */

__attribute__((noreturn))
void move_stack_and_jump(void) {
    kprint("Moving stack to higher half...\n");
    uintptr_t new_stack = HIGH_STACK_TOP;
    __asm__ volatile(
        "mov %0, %%esp\n\t"     /* switch to new stack */
        "xor %%ebp, %%ebp\n\t"  /* clear frame base (optional) */
        "jmp kernel_main\n\t"       /* jump, do not return */
        :
        : "r"(new_stack)
        : "memory"
    );

    __builtin_unreachable();
}

__attribute__((section(".text.hh"), used, noreturn))
void hh_kernel(void) {
    asm volatile("int $0x21");
    printf("In higher half kernel at address: %x\n", hh_kernel);
    kprint("Higher half kernel is running!\n");

    int a = 42;
    printf("Variable a is at address: %x with value: %d\n", &a, a); 

    move_stack_and_jump();
    while (1);
}

__attribute__((noreturn))
void kernel_main(void) {
    int a = 42;
    printf("&a = %x (should be high)\n", (void*)&a);
    kprint("Only kernel_main function and stack should be in higher half now.\n");
    kprint("The rest of the kernel (paging, allocators, etc.) is still in low memory.\n");
    kprint("You can now implement higher-half versions of paging and allocators...\n");

    char c;
    scanf("%c", (char*)&c); // Wait for input before proceeding
    printf("You entered: %c\n", c);

    // Uncomment to test page fault handling
    // printf("Deliberately causing a page fault by accessing unmapped memory...\n");
    // unmap_page(first_page_table, 0);
    // reload_cr3();
    // int * iptr;
    // iptr = (int*)0x00000000; // Accessing null pointer to cause page fault
    // printf("%x", *iptr); // This should trigger a page fault
    for (;;) {}
}