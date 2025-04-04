bits 32

section .text
    align 4
    dd 0x1BADB002
    dd 0x00
    dd - (0x1BADB002 + 0x00)

extern kernel_main
; extern load_gdt      ; Declare load_gdt function

global start

start:
    cli               ; Disable interrupts

    ; Load the GDT (Global Descriptor Table)
    ; call load_gdt     ; Load GDT

    ; Set segment registers to new GDT values (set DS, ES, FS, GS, SS to data segment)
    mov ax, 0x10      ; Data segment selector (GDT entry 2)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Enable protected mode
    mov eax, cr0      ; Load CR0 register
    or eax, 0x1       ; Set the PE (Protected Mode Enable) bit (bit 0)
    mov cr0, eax      ; Store back the modified CR0 register
    
    ; Far jump to reload CS (code segment)
    jmp 0x08:start32  ; Jump to protected mode entry point (segment 0x08)

start32:
    ; Now we are in protected mode
    ; mov esp, stack_space  ; Set up stack
    ; and esp, 0xFFFFFFF0   ; Align stack to 16 bytes
    call kernel_main      ; Call kernel_main (C function)

section .bss
    align 16
    resb 8192             ; Reserve 8KB for the stack
stack_space:
