#include "log.h"
#include "isrs.h"

void isr_32() {
    print("Division by zero error occurred!\n");
    halt();
    return;
}