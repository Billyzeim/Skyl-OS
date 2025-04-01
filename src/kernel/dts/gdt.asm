bits 32

section .data

gdt_start:          ; Start of the GDT
    dq 0            ; Null descriptor (first entry must be zero)

gdt_code:           ; Code segment descriptor
    dw 0xFFFF       ; Limit (low 16 bits)
    dw 0x0000       ; Base (low 16 bits)
    db 0x00         ; Base (middle 8 bits)
    db 0x9A         ; Flags: Present, Ring 0, Executable, Readable
    db 0xCF         ; Flags: 32-bit, Limit (high 4 bits)
    db 0x00         ; Base (high 8 bits)

gdt_data:           ; Data segment descriptor
    dw 0xFFFF       ; Limit (low 16 bits)
    dw 0x0000       ; Base (low 16 bits)
    db 0x00         ; Base (middle 8 bits)
    db 0x92         ; Flags: Present, Ring 0, Readable/Writable
    db 0xCF         ; Flags: 32-bit, Limit (high 4 bits)
    db 0x00         ; Base (high 8 bits)

gdt_end:            ; End of GDT

gdt_descriptor:     ; GDT descriptor
    dw gdt_end - gdt_start - 1  ; GDT size (limit)
    dd gdt_start                ; GDT address (base)

global load_gdt
load_gdt:
    lgdt [gdt_descriptor]   ; Load the GDT
    ret
