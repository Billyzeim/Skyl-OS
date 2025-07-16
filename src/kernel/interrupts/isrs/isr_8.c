#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_8() {
    print("\nInterrupt 8 was triggered and served successfully");
    return;
}
