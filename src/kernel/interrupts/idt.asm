; idt.asm - IDT initialization with ISRs for IRQs 0–15
; Compile with: nasm -f elf32 idt.asm -o idt.o

section .text
global idt_init
global init_pic

%assign i 0
%rep 16
    extern isr_ %+ i
    global isr_ %+ i %+ _asm
%assign i i+1
%endrep


; ------------------------
; Initialize the PIC (8259A)
; ------------------------
init_pic:
    ; Master PIC
    mov al, 0x11
    out 0x20, al
    mov al, 0x20    ; IRQ0-7 → INT 0x20–0x27
    out 0x21, al
    mov al, 0x04    ; Slave at IRQ2
    out 0x21, al
    mov al, 0x01    ; 8086/88 mode
    out 0x21, al

    ; Slave PIC
    mov al, 0x11
    out 0xA0, al
    mov al, 0x28    ; IRQ8-15 → INT 0x28–0x2F
    out 0xA1, al
    mov al, 0x02
    out 0xA1, al
    mov al, 0x01
    out 0xA1, al

    ; Unmask IRQ0 and IRQ1
    mov al, 0xFC
    out 0x21, al
    mov al, 0xFF
    out 0xA1, al
    ret

; ------------------------
; Initialize IDT
; ------------------------
idt_init:
    cli
    call init_pic
    call init_idt
    lidt [idt_ptr]
    ret

init_idt:
    ; Clear IDT
    mov ecx, 256
    mov edi, idt
    xor eax, eax
    rep stosd

%assign i 0
%rep 16
    mov eax, isr_ %+ i %+ _asm
    mov ebx, (0x20 + i) * 8         ; Calculate IDT entry offset

    mov word [idt + ebx], ax                ; Offset bits 0..15
    mov word [idt + ebx + 2], 0x08          ; Selector
    mov word [idt + ebx + 4], 0x8E00        ; Type + attributes
    shr eax, 16
    mov word [idt + ebx + 6], ax            ; Offset bits 16..31
%assign i i+1
%endrep


    ret

; ------------------------
; ISR stubs
; ------------------------
%assign i 0
%rep 16
isr_ %+ i %+ _asm:
    cli
    pushad
    call isr_ %+ i
    mov al, 0x20
%if i >= 8
    out 0xA0, al
%endif
    out 0x20, al
    popad
    iret
%assign i i+1
%endrep

; ------------------------
; IDT storage
; ------------------------
section .data
align 8
idt:
    times 256 * 8 db 0

idt_ptr:
    dw 256 * 8 - 1
    dd idt
