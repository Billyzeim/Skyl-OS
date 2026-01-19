#include <terminal_io.h>
#include <paging.h>

void debug_verify_map(uint32_t virt) {
    uint32_t pd_idx = virt >> 22;
    uint32_t pt_idx = (virt >> 12) & 0x3FF;

    // Use recursive addresses
    uint32_t* pd = (uint32_t*)0xFFFFF000;
    uint32_t* pt = (uint32_t*)(0xFFC00000 + (pd_idx * 4096));

    if (pd[pd_idx] & 0x01) {
        printf("PDE is present.\n");
        if (pt[pt_idx] & 0x01) {
            printf("PTE is present. Physical address matches!\n");
        } else {
            printf("Error: PTE not present!\n");
        }
    } else {
        printf("Error: PDE not present!\n");
    }
}

void verify_recursive_math(uint32_t virt_addr) {
    uint32_t pd_idx = virt_addr >> 22;
    uint32_t pt_idx = (virt_addr >> 12) & 0x3FF;

    // Access the Page Table via the recursive magic address
    uint32_t* pt = (uint32_t*)(0xFFC00000 + (pd_idx * 4096));
    
    uint32_t pte_value = pt[pt_idx];
    uint32_t phys_extracted = pte_value & 0xFFFFF000;

    // This should print the same physical address your PMM gave you
    printf("Verification: Virtual %x maps to Physical %x\n", virt_addr, phys_extracted);
}

__attribute__((noreturn))
void kernel_main() {
    clear_screen();
    init_pmm();
    verify_recursive_math(0xFFFFF000);
    while(1);
}