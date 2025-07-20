section .text.start

global kernel_init
extern kernel_low
extern __stack_top
extern idt_init
global jmp_hh_kernel

kernel_init:
    ; Set up stack
    mov esp, __stack_top
    
    ; Initialize IDT and PIC
    call idt_init
    sti
    call kernel_low
    hlt

jmp_hh_kernel:
    jmp 0xC0010000