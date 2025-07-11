#include <stdint.h>
#include "log.h"
#include "isrs.h"

void isr_15() {
    print("\nInterrupt 15 was triggered and served successfully");
    return;
}
