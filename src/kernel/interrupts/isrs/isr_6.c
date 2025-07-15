#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_6() {
    print("\nInterrupt 6 was triggered and served successfully");
    return;
}
