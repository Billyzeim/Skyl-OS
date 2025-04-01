#include "../../include/pic/pic.h" 
#include "../../include/types/ints.h"

extern void outba();
// PIC Initialization
void pic_init() {
    // Start the initialization sequence for both PICs (ICW1)
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);  // Tell PIC1 to start initialization
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);  // Tell PIC2 to start initialization

    // Set the vector offsets for the PICs (ICW2)
    outb(PIC1_DATA, 0x20);  // Master PIC will use interrupt vectors 0x20-0x27 (IRQ0-IRQ7)
    outb(PIC2_DATA, 0x28);  // Slave PIC will use interrupt vectors 0x28-0x2F (IRQ8-IRQ15)

    // Tell PIC1 about PIC2 (ICW3)
    outb(PIC1_DATA, 0x04);
}

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8) {
        outb(PIC2_CMD, 0x20);  // Send EOI to Slave PIC
    }
    outb(PIC1_CMD, 0x20);  // Send EOI to Master PIC
}

void outb(uint16_t port, uint8_t data)
{
    uint16_t* first = (uint16_t*)port;
    uint8_t* second = (uint8_t*)data;
    outba();
}