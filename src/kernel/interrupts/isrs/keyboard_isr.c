#include <stdint.h>
#include <stdbool.h>
#include <drivers.h>
#include <terminal_io.h>
#include <isrs.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* const vga_buffer = (uint16_t*)VGA_ADDRESS;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

void putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = (0x07 << 8) | c;
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= VGA_HEIGHT) {
        for (int y = 1; y < VGA_HEIGHT; ++y)
            for (int x = 0; x < VGA_WIDTH; ++x)
                vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];

        for (int x = 0; x < VGA_WIDTH; ++x)
            vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ' ' | (0x07 << 8);

        cursor_y = VGA_HEIGHT - 1;
    }
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

#define KEYBUF_SIZE 128
static char keybuf[KEYBUF_SIZE];
static int keybuf_head = 0;
static int keybuf_tail = 0;

static bool keybuf_is_empty() {
    return keybuf_head == keybuf_tail;
}

static bool keybuf_is_full() {
    return ((keybuf_head + 1) % KEYBUF_SIZE) == keybuf_tail;
}

void keybuf_push(char c) {
    if (!keybuf_is_full()) {
        keybuf[keybuf_head] = c;
        keybuf_head = (keybuf_head + 1) % KEYBUF_SIZE;
    }
}

char keybuf_pop() {
    while (keybuf_is_empty());  // block until key is available
    char c = keybuf[keybuf_tail];
    keybuf_tail = (keybuf_tail + 1) % KEYBUF_SIZE;
    return c;
}

char getchar() {
    return keybuf_pop();  // blocks until key available
}

void keyboard_echo_loop() {
    while (1) {
        char c = getchar();
        putchar(c);
    }
}

void isr_1() {
    uint8_t scancode = inb(0x60);

    if (!(scancode & 0x80)) { // key press only
        char c = scancode_to_ascii(scancode);
        if (c) keybuf_push(c);
    }

    // Send EOI to PIC
    outb(0x20, 0x20);
}
