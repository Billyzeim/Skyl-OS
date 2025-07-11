# idt.asm

This file initializes the Interrupt Descriptor Table (IDT) and the Programmable Interrupt Controller (PIC).

## Structure

### `init_pic`
- Remaps the master PIC (IRQ0-7 → 0x20–0x27) and slave PIC (IRQ8-15 → 0x28–0x2F).
- Masks all IRQs except IRQ0 (timer) and IRQ1 (keyboard).

### `idt_init`
- Calls `init_pic` and then `init_idt`.
- Loads IDT with `lidt`.

### `init_idt`
- Clears the IDT.
- Sets up entries for IRQs 0–15, pointing them to respective assembly ISR stubs.

### ISR Stubs
Each stub:
- Disables interrupts
- Saves registers
- Calls corresponding `isr_x` C function
- Sends End Of Interrupt (EOI)
- Restores registers
- Executes `iret`

### IDT Data
- Contains 256 entries (8 bytes each).
- A descriptor pointer `idt_ptr` used for `lidt`.
