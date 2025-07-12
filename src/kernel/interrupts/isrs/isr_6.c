#include <stdint.h>
#include "log.h"
#include "isrs.h"

void isr_6() {
    print("\nInterrupt 6 was triggered and served successfully");
    return;
}
