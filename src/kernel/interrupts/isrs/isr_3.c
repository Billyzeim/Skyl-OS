#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_3() {
    print("\nInterrupt 3 was triggered and served successfully");
    return;
}
