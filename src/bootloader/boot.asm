[ORG 0x7C00]
[BITS 16]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Load 1 sector from disk to 0x1000
    mov ah, 0x02
    mov al, 1
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x1000
    mov es, bx
    xor bx, bx
    int 0x13
    jc disk_error

    ; Setup GDT
    lgdt [gdt_desc]

    ; Enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; ; Far jump to clear instruction queue and load new CS
    jmp 0x08:protected_mode_start

; --------------- GDT ----------------
gdt_start:
    dq 0x0000000000000000     ; null descriptor
    dq 0x00CF9A000000FFFF     ; code segment
    dq 0x00CF92000000FFFF     ; data segment
gdt_end:

gdt_desc:
    dw gdt_end - gdt_start - 1   ; size = total bytes - 1
    dd gdt_start                 ; address of the GDT

; --------------- Protected Mode Code ----------------
[BITS 32]
protected_mode_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; Jump to kernel entry point at 0x1000 (flat address)
    jmp 0x08:0x10000   ; 0x08 = your code segment selector

hang:
    hlt
    jmp hang


; --------------- Error Handling ----------------
[BITS 16]
disk_error:
    mov ah, 0x0E
    mov al, 'E'
    int 0x10
    hlt
    jmp $

; --------------- Boot Signature ----------------
times 510 - ($ - $$) db 0
dw 0xAA55