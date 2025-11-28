
section .text
global idt_load
global init_pic

; ---------------------------------------------
; Import C Handlers (The logic you requested)
; ---------------------------------------------
; This imports isr_0, isr_1, ... isr_15
%assign i 0
%rep 16
    extern isr_%+i
%assign i i+1
%endrep

extern isr_32              ; Division by Zero C handler
extern isr_14              ; Page Fault C handler (Note: also used by IRQ 14 loop above)
extern isr_3               ; Debug Breakpoint C handler

; ---------------------------------------------
; Export Assembly Stubs
; ---------------------------------------------
; We export these so the C code can get their addresses
global isr_32_asm          ; Div Zero wrapper
global isr_14_asm          ; Page Fault wrapper
global isr_3_asm           ; Debug Breakpoint wrapper

%assign i 0
%rep 16
    global irq_stub_%+i    ; IRQ wrappers (renamed to avoid isr_14_asm collision)
%assign i i+1
%endrep

; ---------------------------------------------
; Function: init_pic
; Description: Remaps PIC to 0x20-0x2F. Callable from C.
; ---------------------------------------------
init_pic:
    ; Master PIC
    mov al, 0x11
    out 0x20, al
    mov al, 0x20    ; IRQ0-7 -> INT 0x20-0x27
    out 0x21, al
    mov al, 0x04
    out 0x21, al
    mov al, 0x01
    out 0x21, al

    ; Slave PIC
    mov al, 0x11
    out 0xA0, al
    mov al, 0x28    ; IRQ8-15 -> INT 0x28-0x2F
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

; ---------------------------------------------
; Function: idt_load
; Prototype: void idt_load(uint32_t *idt_ptr);
; Description: Loads the IDT pointer.
; ---------------------------------------------
idt_load:
    mov eax, [esp + 4]  ; Get pointer argument from stack
    lidt [eax]          ; Load IDT
    ret

; ---------------------------------------------
; IRQ Stubs (Mapped to INT 0x20 - 0x2F)
; ---------------------------------------------
%assign i 0
%rep 16
    irq_stub_%+i:
        cli
        pushad
        call isr_%+i      ; Calls C function isr_0, isr_1, etc.
        
        mov al, 0x20      ; Send EOI to Master
    %if i >= 8
        out 0xA0, al      ; Send EOI to Slave if needed
    %endif
        out 0x20, al
        
        popad
        iret
%assign i i+1
%endrep

; ---------------------------------------------
; Exception Stub: Division by Zero (INT 0x00)
; ---------------------------------------------
isr_32_asm:
    cli
    pushad
    call isr_32
    popad
    iret

; ---------------------------------------------
; Exception Stub: Page Fault (INT 0x0E)
; ---------------------------------------------
isr_14_asm:
    cli
    pushad
    mov eax, [esp + 32]   ; Get error code pushed by CPU
    push eax              ; Push error code as argument to C
    call isr_14           ; Call C handler
    add esp, 4            ; Clean up argument
    popad
    jmp $                 ; Infinite loop (Page faults are usually fatal here)

isr_3_asm:
    cli
    pushad          ; Pushes EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
    mov eax, esp    
    push eax        ; Push pointer as argument
    call isr_3      ; Call C handler
    add esp, 4      ; Pop the argument
    popad           ; Restore registers
    iret            ; Resume execution
