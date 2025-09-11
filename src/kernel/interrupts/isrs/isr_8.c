#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_8() {
    printf("Interrupt 8 was triggered and served successfully\n");
    return;
}
