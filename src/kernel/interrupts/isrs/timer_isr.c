#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

volatile uint32_t ticks = 0;

void isr_0() {
    ticks++;
    if (ticks % 100 == 0) {
        // print("Tick 100\n"); // Uncomment to print every 100 ticks
    }
}