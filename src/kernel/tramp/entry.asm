; src/kernel/entry.asm
[BITS 32]

global entry
global debug
extern tramp
global enable_paging

section .tramp.bss
align 16
tramp_stack_bottom:
    resb 4096
tramp_stack_top:

section .tramp.entry
entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, tramp_stack_top
    call tramp

.hang:
    hlt
    jmp .hang

debug:
    mov edi, 0xB8000
    mov ecx, 80*25
    mov ax, 0x0720
    rep stosw
    mov word [0xB8000], 0x0E4B ; Yellow 'K'
    ret

enable_paging:
    mov cr3, eax

    mov ecx, cr4
    or ecx, 0x00000010
    mov cr4, ecx

    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    ret

section .note.GNU-stack noalloc noexec nowrite progbits


