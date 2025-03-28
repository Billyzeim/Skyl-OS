;; kernel.asm

;nasm directive - 32 bit
bits 32

section .text
    ; Multiboot header
    align 4
    dd 0x1BADB002            ; magic
    dd 0x00                  ; flags
    dd - (0x1BADB002 + 0x00) ; checksum (magic + flags + checksum = 0)

global start
extern kernel_main          ; kernel_main is defined in the C file

start:
    cli                     ; Disable interrupts
    mov esp, stack_space    ; Set stack pointer
    and esp, 0xFFFFFFF0     ; Align stack to 16 bytes
    call kernel_main        ; Call the C kernel main function
halt:
    hlt                     ; Halt the CPU
    jmp halt                ; Infinite loop to ensure CPU halts

section .bss
    align 16                ; Align the stack to 16 bytes
    resb 8192               ; Reserve 8KB for the stack
stack_space: