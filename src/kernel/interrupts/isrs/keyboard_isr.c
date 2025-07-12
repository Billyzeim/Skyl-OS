#include <stdint.h>
#include "log.h"
#include "isrs.h"
#include "drivers.h"

void isr_1() {
    uint8_t scancode = inb(0x60);

    char c = scancode_to_ascii(scancode);
    if (c) {
        char buf[2] = { c, '\0' };  // Null-terminated
        print(buf);
    }
}
