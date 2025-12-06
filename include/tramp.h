// filepath: /home/deleted/Skyl-OS/include/tramp.h
// Header file for tramp.c

#ifndef TRAMP_H
#define TRAMP_H

#include <stdint.h>

// Clears the screen and resets the cursor position
void tramp_clear(void);

// Outputs a single character to the screen
void tramp_putc(char ch);

// Outputs a null-terminated string to the screen
void tramp_puts(const char* s);

// Outputs a 32-bit hexadecimal value to the screen
void tramp_puthex32(uint32_t x);

// Outputs an unsigned decimal value to the screen
void tramp_putudec(uint32_t v);

// Entry point for the tramp print test
void tramp(void);

#endif // TRAMP_H