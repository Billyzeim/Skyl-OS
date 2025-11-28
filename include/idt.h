#ifndef IDT_H
#define IDT_H#

#include <stdint.h>

// 1. The IDT Entry Structure
struct idt_entry_struct {
   uint16_t base_low;     // The lower 16 bits of the address to jump to
   uint16_t sel;          // Kernel segment selector
   uint8_t  always0;      // This must always be 0
   uint8_t  flags;        // More flags (Present, DPL, etc)
   uint16_t base_high;    // The upper 16 bits of the address to jump to
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

// 2. The IDT Pointer Structure (passed to lidt)
struct idt_ptr_struct {
   uint16_t limit;
   uint32_t base;         // The address of the first element in our idt_entry_t array
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;

// Function prototypes
void init_idt();
#endif