#include <stdint.h>
#include "log.h"
#include "isrs.h"

void isr_5() {
    print("\nInterrupt 5 was triggered and served successfully");
    return;
}
