#include "../../include/pic/pic.h"

static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile ("out %0, %1" : : "a"(data), "Nd"(port));
}

void isr_default() {
    // Default ISR handler
    // This function can be customized to handle specific interrupts
    // For now, we will just print the interrupt number
    print("Default ISR handler called\n");
    outb(0x0020, 0x20);
    outb(0x00A0, 0x20);
}
