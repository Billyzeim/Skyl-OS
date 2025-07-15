#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_11() {
    print("\nInterrupt 11 was triggered and served successfully");
    return;
}
