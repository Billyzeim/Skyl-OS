section .text
global inb
global outb

inb:
    mov dx, [esp + 4]   ; get port number from stack
    xor eax, eax        ; clear eax to zero upper bits
    in al, dx           ; read byte from port into al
    ret

outb:
    mov dx, [esp + 4]   ; Load port number into dx
    mov al, [esp + 8]   ; Load value to output into al
    out dx, al          ; Send the byte to the I/O port
    ret
