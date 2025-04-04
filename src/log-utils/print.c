#include "../include/log.h"

#define VGA_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x0F
#define VGA_WIDTH 80

void print(const char *str) {
    while (*str) {
        print_char(*str++);
    }
}

void print_char(char c) {
    static int cursor_x = 0, cursor_y = 0;
    char *vidmem = (char *)VGA_ADDRESS;
    int index = (cursor_y * VGA_WIDTH + cursor_x) * 2;

    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        vidmem[index] = c;
        vidmem[index + 1] = WHITE_ON_BLACK; // Color attribute (white on black)
        cursor_x++;
    }

    // Move to next line if at the end
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
}

void print_int(int num) {
    char buffer[12]; // Enough to hold INT_MIN
    int i = 0;

    if (num < 0) {
        print("-");
        num = -num;
    }

    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    while (i > 0) {
        print_char(buffer[--i]);
    }
}