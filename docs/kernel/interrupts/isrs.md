# ISR Files

Each `isr_X.c` file implements a specific interrupt service routine.

## Common Structure

- Uses `print()` to confirm the interrupt occurred and was handled.
- Example:
  ```c
  void isr_4() {
      print("\nInterrupt 4 was triggered and served successfully");
  }
  ```

## `isr_0.c`

Special: Tracks clock ticks using a `volatile uint32_t ticks` variable.

```c
ticks++;
if (ticks % 10 == 0) {
    // Uncomment to print every 10 ticks
}
```

## `isr_1.c`

Handles keyboard input:

- Reads scancode from port `0x60`.
- Converts it using `scancode_to_ascii`.
- Prints the character.
