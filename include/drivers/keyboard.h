#ifndef KEYBOARD_H
#define KEYBOARD_H
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_isr();
void handle_scancode(unsigned char scancode);
unsigned char read_keyboard();
char scancode_to_ascii(unsigned char scancode);

#endif
