#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_21() {
    // uint8_t scancode = inb(0x60);  // Read the keyboard scancode

    // For demonstration: just print the scancode as a hex value
    print("\nInterrupt 21 was triggered and served successfully");
    return;
}
