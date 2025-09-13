#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <terminal_io.h>
#include <stdbool.h>

#define WHITE_ON_BLACK 0x0F
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* const vga_buffer = (uint16_t*)VGA_ADDRESS;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void putchar(char c) {
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

static void puts(const char* s) {
    while (*s) putchar(*s++);
}

static void utoa(unsigned long value, char* buf, int base, bool upper) {
    const char* digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    char tmp[32];
    int i = 0;
    if (value == 0) {
        tmp[i++] = '0';
    } else {
        while (value > 0) {
            tmp[i++] = digits[value % base];
            value /= base;
        }
    }
    for (int j = 0; j < i; ++j)
        buf[j] = tmp[i - j - 1];
    buf[i] = 0;
}

static void itoa(long value, char* buf, int base, bool plus_sign) {
    if (value < 0) {
        *buf++ = '-';
        utoa(-value, buf, base, false);
    } else {
        if (plus_sign) *buf++ = '+';
        utoa(value, buf, base, false);
    }
}

static void ftoa(double n, char* buf, int precision) {
    if (n < 0) {
        *buf++ = '-';
        n = -n;
    }

    long ipart = (long)n;
    double fpart = n - ipart;

    utoa(ipart, buf, 10, false);
    while (*buf) buf++;
    *buf++ = '.';

    for (int i = 0; i < precision; i++) {
        fpart *= 10;
        int digit = (int)fpart;
        *buf++ = '0' + digit;
        fpart -= digit;
    }
    *buf = 0;
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[128];

    while (*fmt) {
        if (*fmt != '%') {
            putchar(*fmt++);
            continue;
        }

        fmt++; // skip '%'

        bool zero_pad = false, plus_sign = false, alt_form = false;
        int width = 0, precision = -1;
        if (*fmt == '+') { plus_sign = true; fmt++; }
        if (*fmt == '#') { alt_form = true; fmt++; }
        if (*fmt == '0') { zero_pad = true; fmt++; }

        if (*fmt == '*') {
            width = va_arg(args, int);
            fmt++;
        } else {
            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt - '0');
                fmt++;
            }
        }

        if (*fmt == '.') {
            fmt++;
            precision = 0;
            while (*fmt >= '0' && *fmt <= '9') {
                precision = precision * 10 + (*fmt - '0');
                fmt++;
            }
        }

        bool long_flag = false;
        if (*fmt == 'l') {
            long_flag = true;
            fmt++;
        }

        char ch = *fmt++;
        switch (ch) {
            case 'c':
                putchar((char)va_arg(args, int));
                break;
            case 's': {
                char* s = va_arg(args, char*);
                puts(s);
                break;
            }
            case 'd':
            case 'i': {
                long val = long_flag ? va_arg(args, long) : va_arg(args, int);
                itoa(val, buffer, 10, plus_sign);
                puts(buffer);
                break;
            }
            case 'u': {
                unsigned long val = long_flag ? va_arg(args, unsigned long) : va_arg(args, unsigned int);
                utoa(val, buffer, 10, false);
                puts(buffer);
                break;
            }
            case 'x':
            case 'X': {
                unsigned long val = long_flag ? va_arg(args, unsigned long) : va_arg(args, unsigned int);
                if (alt_form) puts("0x");
                utoa(val, buffer, 16, ch == 'X');
                puts(buffer);
                break;
            }
            case 'o': {
                unsigned long val = long_flag ? va_arg(args, unsigned long) : va_arg(args, unsigned int);
                if (alt_form) puts("0");
                utoa(val, buffer, 8, false);
                puts(buffer);
                break;
            }
            case 'f':
            case 'e':
            case 'E': {
                double val = va_arg(args, double);
                if (precision == -1) precision = 6;
                ftoa(val, buffer, precision);
                puts(buffer);
                break;
            }
            case '%':
                putchar('%');
                break;
            default:
                putchar('?');
                break;
        }
    }

    va_end(args);
}

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
    char buffer[12];  // Enough for -2,147,483,648 + '\0'
    int i = 0;

    if (num == 0) {
        print_char('0');
        return;
    }

    if (num < 0) {
        print_char('-');
        num = -num;
    }

    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Digits are stored in reverse
    while (i--) {
        print_char(buffer[i]);
    }
}

static char skip_whitespace(void) {
    char c;
    do {
        c = getchar();
    } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');
    return c;
}

static void read_word(char* buf, int max_len) {
    int i = 0;
    char c;
    while (i < max_len - 1) {
        c = getchar();
        if (c == '\n' || c == '\r' || c == ' ') break;
        buf[i++] = c;
    }
    buf[i] = '\0';
}

static void read_number(char* buf, int max_len) {
    int i = 0;
    char c;
    while (i < max_len - 1) {
        c = getchar();
        if (c < '0' || c > '9') break;
        buf[i++] = c;
    }
    buf[i] = '\0';
}

static int atoi(const char* s) {
    int result = 0;
    int sign = 1;
    if (*s == '-') {
        sign = -1;
        s++;
    }
    while (*s >= '0' && *s <= '9') {
        result = result * 10 + (*s++ - '0');
    }
    return sign * result;
}

static int atox(const char* s) {
    int result = 0;
    while ((*s >= '0' && *s <= '9') ||
           (*s >= 'a' && *s <= 'f') ||
           (*s >= 'A' && *s <= 'F')) {
        char c = *s++;
        if (c >= '0' && c <= '9') {
            result = result * 16 + (c - '0');
        } else if (c >= 'a' && c <= 'f') {
            result = result * 16 + (c - 'a' + 10);
        } else if (c >= 'A' && c <= 'F') {
            result = result * 16 + (c - 'A' + 10);
        }
    }
    return result;
}

int scanf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int assigned = 0;
    char c, buffer[64];

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd') {
                char* temp = buffer;
                int i = 0;
                char ch = skip_whitespace();
                if (ch == '-') {
                    buffer[i++] = ch;
                    ch = getchar();
                }
                while (ch >= '0' && ch <= '9' && i < 63) {
                    buffer[i++] = ch;
                    ch = getchar();
                }
                buffer[i] = '\0';
                int* out = va_arg(args, int*);
                *out = atoi(buffer);
                assigned++;
            } else if (*fmt == 'x') {
                char* temp = buffer;
                int i = 0;
                char ch = skip_whitespace();
                while (((ch >= '0' && ch <= '9') ||
                        (ch >= 'a' && ch <= 'f') ||
                        (ch >= 'A' && ch <= 'F')) && i < 63) {
                    buffer[i++] = ch;
                    ch = getchar();
                }
                buffer[i] = '\0';
                int* out = va_arg(args, int*);
                *out = atox(buffer);
                assigned++;
            } else if (*fmt == 's') {
                char* out = va_arg(args, char*);
                char ch = skip_whitespace();
                int i = 0;
                while (ch != ' ' && ch != '\n' && ch != '\r' && i < 63) {
                    out[i++] = ch;
                    ch = getchar();
                }
                out[i] = '\0';
                assigned++;
            } else if (*fmt == 'c') {
                char* out = va_arg(args, char*);
                *out = getchar();
                assigned++;
            } else {
                // unsupported format
            }
        } else {
            char ch = getchar();
            if (ch != *fmt) {
                break; // mismatch between input and format
            }
        }
        fmt++;
    }

    va_end(args);
    return assigned;
}

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = ' ' | (WHITE_ON_BLACK << 8);
    }
    cursor_x = 0;
    cursor_y = 0;
}

void kprint(const char* str) {
    printf("[KERNEL]: %s", str);
}