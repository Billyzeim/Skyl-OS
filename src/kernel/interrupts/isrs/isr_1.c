#include <stdint.h>
#include "log.h"
#include "isrs.h"
#include "drivers.h"

int inb(uint16_t port) {
    uint8_t value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void isr_1() {
    uint8_t scancode = inb(0x60);

    char c = scancode_to_ascii(scancode);
    if (c) {
        char buf[2] = { c, '\0' };  // Null-terminated
        print(buf);
    }
}
