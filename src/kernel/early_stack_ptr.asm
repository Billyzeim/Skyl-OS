section .text.start

global stack_init
extern kernel_main
extern __stack_top
extern idt_init

stack_init:
    ; Set up stack
    mov esp, __stack_top
    
    ; Initialize IDT and PIC
    call idt_init
    call kernel_main
    
    ; Test interrupt - if we see '!' on screen, it works
    int 0x21
    
    ; Halt if kernel_main returns
    cli
    hlt