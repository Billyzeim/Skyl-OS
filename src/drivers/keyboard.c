#include "../include/drivers/keyboard.h"  // Include keyboard header if needed for definitions

// Interrupt handler for keyboard IRQ1
// Interrupt serivice routine (ISR) for keyboard
// This function is called when a keyboard interrupt occurs
void keyboard_isr() {
    unsigned char scancode;
    
    // Read the scancode from the keyboard data port (0x60)
    scancode = (unsigned char)inb(KEYBOARD_DATA_PORT);
    
    // Process the scancode here (you might want to map scancodes to ASCII)
    // Example: Print the scancode or process it further.
    handle_scancode(scancode);
}

// Function to handle scancode and print the pressed key (basic example)
void handle_scancode(unsigned char scancode) {
    // For simplicity, just print the scancode. You'd want to map this to actual characters.
    print((const char *)&scancode);
}

unsigned char read_keyboard() {
    unsigned char status;
    
    // Wait until the keyboard controller has data available
    while (1) {
        status = (unsigned char)inb(KEYBOARD_STATUS_PORT);
        if (status & 0x01) { // 0x01 means data is available in the keyboard buffer
            return (unsigned char)inb(KEYBOARD_DATA_PORT);  // Read the data (scancode)
        }
    }
    return 0; // Should never get here
}

char scancode_to_ascii(unsigned char scancode) {
    static unsigned char shift_pressed = 0;
    static unsigned char caps_lock = 0;

    unsigned char ascii_map[128] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 
        0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 0, 0, '\\', 
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', ' ',
    };

    unsigned char shift_map[128] = {
        0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 
        0, 0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0, 0,
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', 0, 0, '|', 
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, ' ', 0, 
    };

    unsigned char caps_map[128] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 
        0, 0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 0, 
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 0, 0, '\\', 
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', ' ',
    };

    unsigned char shift_caps_map[128] = {
        0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 
        0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', 0, 0,
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', 0, 0, '|', 
        'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0, ' ', 0, 
    };

    if (scancode == 0x2A || scancode == 0x36) {  // Shift pressed
        shift_pressed = 1;
        return 0;
    } else if (scancode == 0xAA || scancode == 0xB6) {  // Shift released
        shift_pressed = 0;
        return 0;
    } else if (scancode == 0x3A) {  // Caps Lock
        caps_lock = !caps_lock;
        return 0;
    }

    if (scancode < 128) {
        if (!shift_pressed && !caps_lock) {
            return ascii_map[scancode];
        } else if (caps_lock && !shift_pressed) {
            return caps_map[scancode];
        } else if (shift_pressed && !caps_lock) {
            return shift_map[scancode];
        } else {
            return shift_caps_map[scancode];
        }
    }
    return 0;  // Return 0 for undefined scancodes
}
