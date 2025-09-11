#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_2() {
    printf("Interrupt 2 was triggered and served successfully\n");
    return;
}
