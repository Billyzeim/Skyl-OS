[BITS 32]
global kernel_low_asm
global debug
extern tramp
section .tramp.entry

kernel_low_asm:
    call tramp

debug:
    mov edi, 0xB8000
    mov ecx, 80*25
    mov ax, 0x0720          ; ' ' (space) with attr 0x07
    rep stosw

    ; write 'K' at top-left
    mov word [0xB8000], 0x074B   ; 'K' (0x4B) + attr 0x07
    ret

.hang: hlt
      jmp .hang
