
# Bootloader Documentation

A bootloader is a low-level program responsible for initializing the system and loading the operating system kernel into memory. This document provides an overview of the bootloader's function, the architecture-specific details, and how the transition from real mode to protected mode occurs.

---

## What the Bootloader Does

The bootloader is the first program executed by the CPU after the BIOS. Its responsibilities include:

- Running in **real-mode** upon CPU start.
- Interfacing with the BIOS to **load the kernel** from disk.
- Setting up the **GDT (Global Descriptor Table)**.
- Switching the CPU into **protected mode**.
- Jumping to the kernel's entry point.

---

## Real Mode

Real mode is a legacy 16-bit CPU mode used for compatibility with older systems. Here are some constraints and features:

- Memory addresses are computed using a **segment:offset** formula:
  ```
  physical_address = segment * 16 + offset
  ```
- Maximum addressable memory is **1MB**.
- BIOS interrupts are available (e.g., disk I/O via `int 13h`, screen output via `int 10h`).

> [!NOTE]  
> Because segments overlap, there are multiple ways to reference the same physical address. Care must be taken during segment setup.

The `start` label in the assembly code sets up real-mode segments and stack, and uses BIOS interrupt `int 13h` to read the next sectors from the disk to address `0x1000`.

> [!WARNING] 
> The number of sectors loaded by the bootloader depends of the size of the kernel after compilation.
> Currently this is hard-coded for simplicity but the bootloader should be detecting it automatically.

---

## Reading from Drive

Reading from the disk is done using **BIOS interrupt 13h**, which is only available in real mode.

```asm
; Load x sectors from disk to 0x10000
mov ah, 0x02
mov al, 21        ; Number of sectors x
mov ch, 0
mov cl, 2
mov dh, 0
mov dl, 0x80
mov bx, 0x1000    ; Segment
mov es, bx
xor bx, bx        ; Offset
int 0x13
jc disk_error
```

> [!WARNING]
> Always check the **Carry Flag (CF)** after a BIOS disk read to handle failures. In this code, this is correctly done with a `jc disk_error`.

---

## What is a Boot Sector?

The bootloader resides in the **boot sector**, which is the first 512 bytes of a bootable drive. BIOS loads it to memory at `0x7C00` and starts executing from there.

- The last two bytes must be the **boot signature** `0xAA55`:
  ```asm
  times 510 - ($ - $$) db 0
  dw 0xAA55
  ```

---

## Protected Mode

Protected mode enables 32-bit instructions and access to up to **4GB** of memory. It also introduces hardware-level protection features and a more structured memory model.

Transitioning to protected mode requires:

1. Setting up a **GDT**.
2. Enabling the PE bit in **CR0**.
3. Performing a **far jump** to load the new code segment selector.

Your code does this immediately after loading the kernel.

---

## GDT (Global Descriptor Table)

The GDT defines memory segments and their properties (e.g., base, limit, access rights). In your implementation, it includes:

- A null descriptor (mandatory).
- A code segment descriptor.
- A data segment descriptor.

Example:
```asm
gdt_start:
    dq 0x0000000000000000     ; null descriptor
    dq 0x00CF9A000000FFFF     ; code segment
    dq 0x00CF92000000FFFF     ; data segment
```

The `gdt_desc` structure defines the size and address of the GDT, which is then loaded using the `lgdt` instruction.

> [!NOTE]
> The limit in GDT is `0xFFFF` and granularity is set to 4KB, which means the actual segment size is 4GB.

---

## Jumping to the Kernel

Once in protected mode, the bootloader sets up segment registers and the stack. Then, it performs a far jump to the kernel's entry point:

```asm
jmp 0x08:0x10000
```

This jumps to a flat memory address `0x10000`, where the kernel was loaded earlier.

> [!NOTE]  
> `0x08` is the selector for the code segment in the GDT.

---

## Error Handling

If disk reading fails, your code prints `'E'` using BIOS `int 10h` and halts. This is crucial for debugging:

```asm
disk_error:
    mov ah, 0x0E
    mov al, 'E'
    int 0x10
    hlt
```

---

## Bootloader Flow Summary

1. BIOS loads bootloader to `0x7C00`.
2. Bootloader:
   - Sets up segment registers and stack.
   - Reads one sector from disk to `0x1000`.
   - Sets up and loads GDT.
   - Switches to protected mode.
   - Jumps to kernel at `0x10000`.

---

## Resources

- [Intel 80386 Programmer’s Reference Manual](https://pdos.csail.mit.edu/6.828/2018/readings/i386/toc.htm)
- [OSDev Wiki - Bare Bones](https://wiki.osdev.org/Bare_Bones)
- [OSDev Wiki - GDT Tutorial](https://wiki.osdev.org/GDT_Tutorial)
- [Interrupt List (Ralf Brown’s)](http://www.ctyme.com/rbrown.htm)
