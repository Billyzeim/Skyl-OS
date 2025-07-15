#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_4() {
    print("\nInterrupt 4 was triggered and served successfully");
    return;
}
