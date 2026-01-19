section .text.start

global kernel_init
extern kernel_main
extern __stack_top
extern idt_init

kernel_init:
    ; Set up stack
    mov esp, __stack_top
    
    ; Initialize IDT and PIC
    call idt_init
    sti
    call kernel_main
    hlt