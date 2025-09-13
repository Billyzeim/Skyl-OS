#ifndef LOG_H
#define LOG_H

#include <stdbool.h>

static void putchar(char c);
static void puts(const char* s);
static void utoa(unsigned long value, char* buf, int base, bool upper);
static void itoa(long value, char* buf, int base, bool plus_sign);
static void ftoa(double n, char* buf, int precision);
void printf(const char* fmt, ...);
void print(const char *message);
void print_char(char c);
void print_int(int num);
char getchar(void);
void clear_screen(void);
void kprint(const char* str);
int scanf(const char* fmt, ...);

#endif