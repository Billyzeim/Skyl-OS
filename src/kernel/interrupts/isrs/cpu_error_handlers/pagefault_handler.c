#include <stdint.h>

#include <stdint.h>

// Assume you have a simple VGA text output function like putchar or printf.
// If not, you can use direct memory writes to 0xB8000 (text mode).

void panic(const char* message) {
    // Disable interrupts to avoid further system activity
    asm volatile("cli");

    // Simple way to print the panic message on screen
    // If you have printf available:
    printf("KERNEL PANIC: %s\n", message);

    // Halt CPU in an infinite loop to stop everything safely
    while (1) {
        asm volatile("hlt");
    }
}

void isr_14(uint32_t error_code) {
    uint32_t faulting_address;
    // Read CR2 - the address that caused the page fault
    asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

    // Decode error code bits
    int present   = !(error_code & 0x1);  // 0 = not present, 1 = protection violation
    int rw        = error_code & 0x2;     // 0 = read, 1 = write
    int us        = error_code & 0x4;     // 0 = kernel mode, 1 = user mode
    int reserved  = error_code & 0x8;     // Overwritten CPU-reserved bits?
    int instr_fetch = error_code & 0x10;  // 1 if fault during instruction fetch

    // Print info (you need your own print or debug function)
    printf("Page Fault! Address: 0x%x\n", faulting_address);
    printf("Error Code: 0x%x\n", error_code);
    printf("Details: %s, %s, %s, %s, %s\n",
           present ? "Protection Violation" : "Not Present",
           rw ? "Write" : "Read",
           us ? "User Mode" : "Kernel Mode",
           reserved ? "Reserved Bits Overwritten" : "No Reserved Bits",
           instr_fetch ? "Instruction Fetch" : "Data Access");

    // Here you can try to handle the fault, e.g., map the missing page if possible

    // If you can't recover:
    panic("Page fault - Halting system.");
}
