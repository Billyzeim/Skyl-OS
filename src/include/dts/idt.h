#ifndef IDT_H
#define IDT_H

#include "../types/ints.h"

// IDT entry structure
struct IDTEntry {
    uint16_t offset_low;   // Lower 16 bits of the ISR address
    uint16_t selector;     // Code segment selector
    uint8_t  zero;         // Reserved, set to 0
    uint8_t  type_attr;    // Type and attributes
    uint16_t offset_high;  // Higher 16 bits of the ISR address
} __attribute__((packed));

// IDT pointer structure
struct IDTPointer {
    uint16_t limit;  // Size of the IDT
    uint32_t base;   // Address of the IDT
} __attribute__((packed));

#define IDT_SIZE 256

// Functions
void idt_install();
void set_idt_gate(int n, uint32_t handler);

#endif
