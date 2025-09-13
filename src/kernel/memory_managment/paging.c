#include <paging.h>

uint32_t __attribute__((aligned(PAGE_SIZE))) page_directory[PAGE_DIR_ENTRIES];
uint32_t __attribute__((aligned(PAGE_SIZE))) first_page_table[PAGE_TABLE_ENTRIES];

void init_paging() {
    // Zero the directory and table
    for (int i = 0; i < PAGE_DIR_ENTRIES; i++) page_directory[i] = 0x00000002; // Supervisor, read/write, not present
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) first_page_table[i] = 0;

    // Identity map the first 4MB (optional, for early code/data access)
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | 3; // present, rw
    }

    // Map page directory entry 0 to the first page table (identity map)
    page_directory[0] = ((uint32_t)first_page_table) | 3;

    // Map kernel high-half: 0xC0000000 → 0x00000000
    // page_directory index = 0xC0000000 / 4MB = 768
    page_directory[768] = ((uint32_t)first_page_table) | 3;
}

void enable_paging(uint32_t* pd) {
    asm volatile("mov %0, %%cr3" :: "r"(pd));  // Load page directory
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;  // Set paging bit
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

void unmap_page(uint32_t* pd, uint32_t index) {
    if (index < PAGE_DIR_ENTRIES) {
        pd[index] = 0x00000002; // Reset the page directory entry
    }
}

void reload_cr3() {
    asm volatile("mov %0, %%cr3" :: "r"(page_directory) : "memory");
}