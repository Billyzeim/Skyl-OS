#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGE_SIZE 4096
#define PAGE_DIR_ENTRIES 1024
#define PAGE_TABLE_ENTRIES 1024

#define KERNEL_VIRTUAL_BASE 0xC0000000
#define KERNEL_PHYS_BASE    0x00100000

extern uint32_t page_directory[PAGE_DIR_ENTRIES];
extern uint32_t first_page_table[PAGE_TABLE_ENTRIES];

void init_paging();
void enable_paging(uint32_t* pd);
void unmap_page(uint32_t* pd, uint32_t index);
void reload_cr3();


#endif