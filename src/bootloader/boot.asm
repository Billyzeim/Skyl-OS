[ORG 0x7C00] ; This is where the bootloader is loaded in memory
[BITS 16]    ; Bootloader code starts in 16-bit mode

start:
    cli
    mov ax, 0x0700
    mov ss, ax       ; Set stack segment to 0x0700
    mov sp, 0x0000   ; Set stack pointer below of bootloader
    xor ax, ax       ; Zero-out ax
    mov ds, ax       ; Set data segment to 0
    mov es, ax       ; Set extra segment to 0

    ; Load x sectors from disk to 0x10000
    mov ah, 0x02
    mov al, 29        ; Number of sectors x
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x1000    ; Segment
    mov es, bx
    xor bx, bx        ; Offset
    int 0x13
    jc disk_error

    ; Setup GDT
    lgdt [gdt_desc]

    ; Enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to clear pipeline and load CS
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
    ; Set up data segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Jump to kernel entry point
    jmp 0x08:0x10000

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