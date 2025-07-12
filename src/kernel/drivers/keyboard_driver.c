#include "drivers.h"

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