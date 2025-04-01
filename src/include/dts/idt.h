#ifndef IDT_H
#define IDT_H
#include "../types/defs.h"

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

__attribute__((noreturn))
void exception_handler(void);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);

typedef uint8_t bool;

#define IDT_MAX_DESCRIPTORS 256

static bool vectors[IDT_MAX_DESCRIPTORS];
void idt_init(void);
extern void* isr_stub_table[];

#define uintptr_t (unsigned int *)
#define true 1
#define false 0

#endif
