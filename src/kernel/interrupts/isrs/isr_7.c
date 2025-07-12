#include <stdint.h>
#include "log.h"
#include "isrs.h"

void isr_7() {
    print("\nInterrupt 7 was triggered and served successfully");
    return;
}
