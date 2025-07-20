#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_13() {
    print("\nGeneral Protection Fault (GPF) occurred!\n");
    return;
}
