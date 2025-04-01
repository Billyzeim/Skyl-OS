extern idt_ptr
global idt_load

idt_load:
    lidt [idt_ptr]  ; Load IDT
    sti             ; Enable interrupts
    ret
