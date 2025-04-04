#include "../../include/isr/isr.h"

void isr_default() {
    // Default ISR handler
    // This function can be customized to handle specific interrupts
    // For now, we will just print the interrupt number
    print("Default ISR handler called\n");
    outb(0x0020, 0x20);
    outb(0x00A0, 0x20);
}
