#include <terminal_io.h>
#include <isrs.h>

extern void halt();

void isr_32() {
    print("Division by zero error occurred!\n");
    halt();
    return;
}