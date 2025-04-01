#include "../../include/dts/idt.h"
#include "../../include/isr/isr.h"
#include "../../include/log.h"

struct IDTEntry idt[IDT_SIZE];
struct IDTPointer idt_ptr;

extern void idt_load();  // Defined in idt_load.asm

// Function to set an entry in the IDT
void set_idt_gate(int n, uint32_t handler) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08;   // Kernel code segment
    idt[n].zero = 0;
    idt[n].type_attr = 0x8E;  // 32-bit interrupt gate
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

// Function to initialize and load the IDT
void idt_install() {
    idt_ptr.limit = (sizeof(struct IDTEntry) * IDT_SIZE) - 1;
    idt_ptr.base = (uint32_t) &idt;

    // Clear IDT entries
    for (int i = 0; i < IDT_SIZE; i++) {
        set_idt_gate(i, (uint32_t)isr_default);  // Default ISR
    }
    // set_idt_gate(0x64, (uint32_t)isr_default);

    idt_load();  // Load the IDT using the assembly function
    print("IDT loaded\n");
}
