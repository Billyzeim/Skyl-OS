#include <stdint.h>
#include "log.h"
#include "isrs.h"

void isr_9() {
    print("\nInterrupt 9 was triggered and served successfully");
    return;
}
