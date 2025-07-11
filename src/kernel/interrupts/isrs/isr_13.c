#include <stdint.h>
#include "log.h"
#include "isrs.h"

void isr_13() {
    print("\nInterrupt 13 was triggered and served successfully");
    return;
}
