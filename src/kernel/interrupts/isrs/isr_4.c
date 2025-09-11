#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_4() {
    printf("Interrupt 4 was triggered and served successfully\n");
    return;
}
