global load_gdt
load_gdt:
    lgdt [esp + 4]  ; Load GDT from the pointer argument

    mov ax, 0x10    ; Load Data Segment (GDT entry 2)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:flush_cs  ; Long jump to reload CS (Code Segment, GDT entry 1)
flush_cs:
    ret
