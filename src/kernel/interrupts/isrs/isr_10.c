#include <stdint.h>
#include "log.h"
#include "isrs.h"

void isr_10() {
    print("\nInterrupt 10 was triggered and served successfully");
    return;
}
