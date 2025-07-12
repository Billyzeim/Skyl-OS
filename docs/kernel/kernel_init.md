# kernel_init.asm

This assembly file is the kernel's entry point after the bootloader passes control to protected mode. It is responsible for initializing the system stack and the interrupt descriptor table (IDT), enabling interrupts, and finally calling the kernel's C main function.

## Key Steps

- **Stack Setup**:
  ```asm
  mov esp, __stack_top
  ```
  This sets the stack pointer to a predefined memory address where the stack starts.

- **IDT Initialization**:
  ```asm
  call idt_init
  ```
  Prepares the system for interrupt handling.

- **Enable Interrupts**:
  ```asm
  sti
  ```

- **Call Kernel Main**:
  ```asm
  call kernel_main
  ```

- **Halt Execution**:
  ```asm
  hlt
  ```
  Prevents the CPU from executing random instructions if `kernel_main` ever returns.
