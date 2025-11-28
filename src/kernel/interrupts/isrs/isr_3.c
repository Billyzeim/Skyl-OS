#include <stdint.h>
#include <terminal_io.h>
#include <isrs.h>

typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pushad
    uint32_t eip, cs, eflags;                        // Pushed by CPU
} registers_t;

void isr_3(registers_t regs) {
    printf("\n=== DEBUG BREAKPOINT ===\n");
    kprint("EAX: "); printf("%x\n", regs.eax);
    kprint(" EBX: "); printf("%x\n", regs.ebx);
    kprint(" EIP: "); printf("%x\n", regs.eip);
    kprint("System Halted for inspection.\n");
    printf("=== DEBUG BREAKPOINT END ===\n");
}

// we should let them define the struct and use it as args to check if they understand how parameters are passed thrrought the stack
// Maybe use code that causes certain known values in the registers and then check if they are printed correctly and why the values are the way they are

