; idt.asm - IDT initialization with ISR for INT 21h
; Compile with: nasm -f elf32 idt.asm -o idt.o

section .text
global idt_init
global isr_21
global init_pic
extern isr_21_handler

; Initialize PIC (Programmable Interrupt Controller)
init_pic:
    ; Initialize master PIC
    mov al, 0x11         ; ICW1: initialization
    out 0x20, al
    mov al, 0x20         ; ICW2: master PIC vector offset (32)
    out 0x21, al
    mov al, 0x04         ; ICW3: tell master about slave at IRQ2
    out 0x21, al
    mov al, 0x01         ; ICW4: 8086 mode
    out 0x21, al

    ; Initialize slave PIC
    mov al, 0x11
    out 0xA0, al
    mov al, 0x28         ; ICW2: slave PIC vector offset (40)
    out 0xA1, al
    mov al, 0x02         ; ICW3: slave identity
    out 0xA1, al
    mov al, 0x01
    out 0xA1, al

    ; Mask all interrupts for now
    mov al, 0xFF
    out 0x21, al
    out 0xA1, al
    ret

idt_init:
    cli
    ; Initialize PIC first
    call init_pic
    
    ; Initialize IDT
    call init_idt
    
    ; Load IDT
    lidt [idt_ptr]
    ret

; IDT Initialization
init_idt:
    ; Clear IDT entries
    mov ecx, 256
    mov edi, idt
    xor eax, eax
    rep stosd

    ; Set up INT 0x21 handler
    mov eax, isr_21           ; Get ISR address
    mov word [idt + 0x21 * 8], ax        ; Low 16 bits of offset
    mov word [idt + 0x21 * 8 + 2], 0x08  ; Code segment selector
    mov word [idt + 0x21 * 8 + 4], 0x8E00 ; Present, DPL=0, 32-bit interrupt gate
    shr eax, 16
    mov word [idt + 0x21 * 8 + 6], ax    ; High 16 bits of offset

    ret

; ISR for INT 0x21
isr_21:
    cli
    push eax
    push ebx
    push ecx
    push edx

    ; Set up pointer to video memory
    ; mov ebx, 0xB8000          ; Start of video memory
    ; mov byte [ebx], 'K'       ; Character to print
    ; mov byte [ebx + 1], 0x07  ; Attribute byte: white on black
    call isr_21_handler

    ; Send End of Interrupt to PIC
    mov al, 0x20
    out 0x20, al

    pop edx
    pop ecx
    pop ebx
    pop eax
    hlt
    ; sti
    iret

section .data
align 8
; IDT (256 entries)
idt:
    times 256 * 8 db 0      ; 256 entries, 8 bytes each

; IDT pointer
idt_ptr:
    dw 256 * 8 - 1          ; Limit (size of IDT - 1)
    dd idt                  ; Base address of IDT