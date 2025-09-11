#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

void isr_7() {
    printf("Interrupt 7 was triggered and served successfully\n");
    return;
}
