# log.c

Implements text output to VGA text memory.

## `print(const char *str)`
- Writes a string to the screen using `print_char`.

## `print_char(char c)`
- Outputs a character at the current cursor location.
- Handles newline by moving cursor to the next line.
- VGA text mode used (0xB8000, 80x25 screen, each char = 2 bytes).

## `print_int(int num)`
- Converts integer to ASCII and prints using `print_char`.
- Handles 0 and negative numbers.
