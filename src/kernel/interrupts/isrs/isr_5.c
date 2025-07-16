#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_5() {
    print("\nInterrupt 5 was triggered and served successfully");
    return;
}
