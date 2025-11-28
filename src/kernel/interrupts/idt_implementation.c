#include <idt.h>

// 1. Declare the IDT Table and Pointer
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

// 2. Import Assembly Functions
extern void idt_load(uint32_t ptr);
extern void init_pic();

// 3. Import Exception Handlers (ASM wrappers)
extern void _______(); // Div Zero
extern void _______(); // Page Fault
extern void _______();  // Debug Breakpoint

// 4. Import IRQ Handlers (ASM wrappers)
// IMPORTANT: These match the 'irq_stub_X' names in idt.asm
extern void irq_stub_0();  extern void irq_stub_1();  extern void irq_stub_2();
extern void irq_stub_3();  extern void irq_stub_4();  extern void irq_stub_5();
extern void irq_stub_6();  extern void irq_stub_7();  extern void irq_stub_8();
extern void irq_stub_9();  extern void irq_stub_10(); extern void irq_stub_11();
extern void irq_stub_12(); extern void irq_stub_13(); extern void irq_stub_14();
extern void irq_stub_15();

// What does this helper do?
void helper_whaaaaat(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low  = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel       = sel;
    idt_entries[num].always0   = 0;
    idt_entries[num].flags     = flags;
}

// Renamed from init_idt to idt_init to match kernel_init call
void idt_init() {
    // CRITICAL FIX: Disable interrupts!
    // The init_pic() function unmasks IRQ0 (Timer). If interrupts are not 
    // strictly disabled here, the timer ISR will fire immediately. If paging
    // or the stack isn't ready for a context switch, this causes a Page Fault.
    __asm__ volatile("cli");

    // A. Initialize the PIC (Remap to 0x20 - 0x2F)
    init_pic(); // why is this necessary?

    // B. Setup the IDT Pointer
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    // C. Set Exception Gates
    // INT 0: Division by Zero
    // helper_whaaaaat(0, (uint32_t)isr_32_asm, 0x08, 0x8E);
    helper_whaaaaat(0, (uint32_t) _________, 0x08, 0x8E); // div by zero
    
    // INT 14: Page Fault
    helper_whaaaaat(14, (uint32_t) __________, 0x08, 0x8E); // page fault

    // Gate 3 is reserved for Debugging/Breakpoints
    helper_whaaaaat(3, (uint32_t) _________, 0x08, 0x8E); // poor mans debugger

    // D. Set IRQ Gates (Mapped to 0x20 - 0x2F)
    // We use the irq_stub_X addresses here
    helper_whaaaaat(0x20, (uint32_t)irq_stub_0, 0x08, 0x8E);
    helper_whaaaaat(0x21, (uint32_t)irq_stub_1, 0x08, 0x8E);
    helper_whaaaaat(0x22, (uint32_t)irq_stub_2, 0x08, 0x8E);
    helper_whaaaaat(0x23, (uint32_t)irq_stub_3, 0x08, 0x8E);
    helper_whaaaaat(0x24, (uint32_t)irq_stub_4, 0x08, 0x8E);
    helper_whaaaaat(0x25, (uint32_t)irq_stub_5, 0x08, 0x8E);
    helper_whaaaaat(0x26, (uint32_t)irq_stub_6, 0x08, 0x8E);
    helper_whaaaaat(0x27, (uint32_t)irq_stub_7, 0x08, 0x8E);
    helper_whaaaaat(0x28, (uint32_t)irq_stub_8, 0x08, 0x8E);
    helper_whaaaaat(0x29, (uint32_t)irq_stub_9, 0x08, 0x8E);
    helper_whaaaaat(0x2A, (uint32_t)irq_stub_10, 0x08, 0x8E);
    helper_whaaaaat(0x2B, (uint32_t)irq_stub_11, 0x08, 0x8E);
    helper_whaaaaat(0x2C, (uint32_t)irq_stub_12, 0x08, 0x8E);
    helper_whaaaaat(0x2D, (uint32_t)irq_stub_13, 0x08, 0x8E);
    helper_whaaaaat(0x2E, (uint32_t)irq_stub_14, 0x08, 0x8E);
    helper_whaaaaat(0x2F, (uint32_t)irq_stub_15, 0x08, 0x8E);

    // E. Load the IDT
    idt_load((uint32_t)&idt_ptr);
}

// Drastiriotita questions:
//  What does the helper what do?
//  It sets up an individual entry in the IDT (Interrupt Descriptor Table) for div by zero, page fault, and poor mans debugger
//  Why is it necessary to disable interrupts at the start of idt_init()?
//  What is pic and why do we need to remap it? Did intel engineers try to cover a stupid mistake of theirs?
//  What happens if we don't remap the PIC?
//  What is the significance of the values 0x20 - 0x2F?
//  What is the difference between an ISR and an IRQ? (we check that when we ask them to set up the gates)