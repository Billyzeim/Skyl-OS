# keyboard_driver.c

Maps keyboard scancodes to ASCII characters, handling shift and caps lock.

## Key Functions

- **State Tracking**:
  - `shift_pressed` and `caps_lock` are tracked statically.

- **Four Character Maps**:
  - `ascii_map`: regular layout.
  - `shift_map`: with Shift pressed.
  - `caps_map`: with Caps Lock.
  - `shift_caps_map`: both Shift and Caps Lock.

- **Handles special scancodes**:
  - `0x2A`, `0x36`: Shift down.
  - `0xAA`, `0xB6`: Shift up.
  - `0x3A`: Toggle Caps Lock.
