#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_3() {
    printf("Interrupt 3 was triggered and served successfully\n");
    return;
}
