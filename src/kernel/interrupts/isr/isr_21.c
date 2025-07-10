#include <stdint.h>
#include "log.h"
#include "isrs.h"

void isr_21_handler() {
    // uint8_t scancode = inb(0x60);  // Read the keyboard scancode

    // For demonstration: just print the scancode as a hex value
    print("\nInterrupt 21 was triggered and served successfully");
    return;
}
