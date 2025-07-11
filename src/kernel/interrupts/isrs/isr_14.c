#include <stdint.h>
#include "log.h"
#include "isrs.h"

void isr_14() {
    print("\nInterrupt 14 was triggered and served successfully");
    return;
}
