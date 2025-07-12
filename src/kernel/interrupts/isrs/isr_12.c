#include <stdint.h>
#include "log.h"
#include "isrs.h"

void isr_12() {
    print("\nInterrupt 12 was triggered and served successfully");
    return;
}
