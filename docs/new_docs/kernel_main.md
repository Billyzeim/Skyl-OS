# kernel.c

This is the main C function of the kernel.

## Responsibilities

- **Clears the screen**:
  ```c
  for (int i = 0; i < 80 * 25; i++) {
      video[i] = 0x0720;
  }
  ```
  Fills the screen with spaces (`0x20`) and attribute byte `0x07` (white on black).

- **Loops Forever**:
  Prevents the kernel from exiting:
  ```c
  while(1);
  ```
