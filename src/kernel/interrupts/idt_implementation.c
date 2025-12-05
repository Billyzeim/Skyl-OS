#include <idt.h>

// 1. Declare the IDT Table and Pointer
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

// 2. Import Assembly Functions
extern void idt_load(uint32_t ptr);
extern void init_pic();

// 3. Import Exception Handlers (ASM wrappers)
// find the correct names for these handlers in the isrs folder
extern void __________(); // Div Zero
extern void __________(); // Page Fault
extern void __________();  // Debug Breakpoint

// 4. Import IRQ Handlers (ASM wrappers)
// IMPORTANT: These match the 'irq_stub_X' names in idt.asm
extern void irq_stub_0();  extern void irq_stub_1();  extern void irq_stub_2();
extern void irq_stub_3();  extern void irq_stub_4();  extern void irq_stub_5();
extern void irq_stub_6();  extern void irq_stub_7();  extern void irq_stub_8();
extern void irq_stub_9();  extern void irq_stub_10(); extern void irq_stub_11();
extern void irq_stub_12(); extern void irq_stub_13(); extern void irq_stub_14();
extern void irq_stub_15();

// What does this helper do?
void helper(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low  = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel       = sel;
    idt_entries[num].always0   = 0;
    idt_entries[num].flags     = flags;
}

void idt_init() {
    __asm__ volatile("cli");

    // A. Initialize the PIC (Remap to 0x20 - 0x2F)
    init_pic();

    // B. Setup the IDT Pointer
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    // C. Set Exception Gates
    // INT 0: Division by Zero
    helper(0, (uint32_t) __________, 0x08, 0x8E); // div by zero 
    
    // INT 14: Page Fault
    helper(14, (uint32_t) __________, 0x08, 0x8E); // page fault

    helper(3, (uint32_t) __________, 0x08, 0x8E); // poor mans debugger

    // D. Set IRQ Gates (Mapped to 0x20 - 0x2F)
    // We use the irq_stub_X addresses here
    helper(0x20, (uint32_t)irq_stub_0, 0x08, 0x8E); // Timer IRQ
    helper(_____, (uint32_t)irq_stub_1, 0x08, 0x8E); // add the correct interrupt vector number for the keyboard interrupts

    // E. Load the IDT
    idt_load((uint32_t)&idt_ptr);
    return;
}