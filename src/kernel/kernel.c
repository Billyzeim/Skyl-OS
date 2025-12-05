#include <terminal_io.h>
#include <paging.h>
#include <allocators.h>

#define PIC1_DATA_PORT 0x21

#define PRINT_EAX() ({ \
    uint32_t _val; \
    __asm__ __volatile__("mov %%eax, %0" : "=r"(_val)); \
    printf("Current EAX: 0x%x\n", _val); \
})

void mask_timer() {
    // 1. Read the current mask
    uint8_t cur_mask = inb(PIC1_DATA_PORT);
    
    // 2. Set the 0th bit (IRQ 0 - Timer) to 1
    // 0x01 is binary 00000001
    cur_mask = cur_mask | 0x01; 
    
    // 3. Write the new mask back
    outb(PIC1_DATA_PORT, cur_mask);
}

void unmask_timer() {
    uint8_t cur_mask = inb(PIC1_DATA_PORT);
    
    // Clear the 0th bit to 0 using AND and NOT
    cur_mask = cur_mask & ~(0x01);
    
    outb(PIC1_DATA_PORT, cur_mask);
}

void syscall_echo(char c) {
    __asm__ volatile("int $0x80" : : "a"(c));
}

// Import the IDT initialization function
extern void idt_init(); 

extern char __hh_lma[];      // LMA of .text.hh (address value)
extern char __hh_phys[];     // if you defined it in the linker (ABS value)
extern char __hh_vma[];      // ADDR(.text.hh) if you exported it
extern void hh_kernel(void); // the function (VMA ~ 0xC0……)

void kernel_low() {
    clear_screen();
    kprint("[LOW KERNEL] Starting...\n");

    // 1. Initialize Paging (CRITICAL: Must be first)
    // We must map memory before loading the IDT, otherwise the CPU 
    // might try to read the IDT from an invalid physical address.
    init_paging();
    enable_paging(page_directory);
    kprint("[LOW KERNEL] Paging enabled.\n");

    // 2. Initialize IDT
    // This loads the table and remaps the PIC.
    // Interrupts are currently DISABLED (cli) by idt_init().
    idt_init();
    kprint("[LOW KERNEL] IDT initialized.\n");

    // 3. Enable Interrupts (Tests IRQ 0 - Timer)
    // Only do this AFTER everything else is set up.
    // If your timer_isr prints to the screen, you should see output now.
    kprint("[LOW KERNEL] Enabling interrupts (STI)...\n");
    __asm__ volatile("sti");
    // mask_timer();   // Disable timer IRQ for cleaner output

    // =======================================================================
    // INTERRUPT TESTS
    // Uncomment ONE of the following blocks at a time to test exceptions.
    // These will likely HALT the system (Kernel Panic), so we can't run all.
    // =======================================================================

    // --- TEST 1: Division By Zero (ISR 0) ---
    /*
    kprint("[TEST] Triggering Division by Zero...\n");
    int b = 0;
    __asm__ volatile("div %0" :: "r"(b)); // Force hardware exception
    */

    // --- TEST 2: Page Fault (ISR 14) ---
    /*
    kprint("[TEST] Triggering Page Fault...\n");
    volatile uint32_t *bad_ptr = (volatile uint32_t*)0xA0000000; // Unmapped address
    *bad_ptr = 0xDEADBEEF;
    */

    // --- TEST 3: Software Interrupt (Manual) ---
    // __asm__ volatile("int $0x03"); 

    PRINT_EAX();
    
    // syscall_echo('k');
    
    kprint("[LOW KERNEL] System active. Press keys to test Keyboard (IRQ 1).\n");

    // Jump to Higher Half Kernel (Optional, or stay here for testing)
    // asm volatile("jmp *%0" :: "r"(hh_kernel));

    while(1) {
        // CPU halt loop to save energy while waiting for interrupts
        __asm__ volatile("hlt");
    }
}