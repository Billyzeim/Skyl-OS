global outba
extern first
extern second

outba:
    mov edx, [second]  ; Load the port (2nd argument) into DX
    mov al, [first]   ; Load the data (1st argument) into AL
    out dx, al          ; Send AL to port DX
    ret                 ; Return
