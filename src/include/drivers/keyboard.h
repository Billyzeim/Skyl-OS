#ifndef KEYBOARD_H
#define KEYBOARD_H
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_isr();
void handle_scancode(unsigned char scancode);
unsigned char read_keyboard();
void idt_install();
void set_idt_gate(unsigned char num, unsigned int base);
void enable_irq(unsigned char irq);
static inline unsigned char inb(unsigned short port);
static inline void outb(unsigned short port, unsigned char data);
char scancode_to_ascii(unsigned char scancode);

#endif
