#ifndef PIC_H
#define PIC_H
#define PIC1        0x20    // I/O port for the Master PIC
#define PIC2        0xA0    // I/O port for the Slave PIC
#define PIC1_CMD    PIC1    // Command port for Master PIC
#define PIC1_DATA   (PIC1 + 1)  // Data port for Master PIC
#define PIC2_CMD    PIC2    // Command port for Slave PIC
#define PIC2_DATA   (PIC2 + 1)  // Data port for Slave PIC
#include "../types/ints.h"

void pic_init();  // Function to initialize the PIC
void pic_send_eoi(uint8_t irq);  // Function to send End Of Interrupt (EOI) signal
void outb(uint16_t port, uint8_t data);  // Function to write data to a port
// PIC Initialization Command
#define ICW1_ICW4   0x01    // ICW4 (needed for 8086 mode)
#define ICW1_INIT    0x10    // Initialization command
#define ICW4_8086   0x01    // 8086/88 mode

#endif