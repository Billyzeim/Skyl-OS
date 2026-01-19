#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGE_SIZE 4096
#define PAGE_DIR_ENTRIES 1024
#define PAGE_TABLE_ENTRIES 1024

#define KERNEL_VIRTUAL_BASE 0xC0000000
#define KERNEL_PHYS_BASE    0x00100000

#define KERNEL_VIRTUAL_BASE 0xC0000000
#define V2P(addr) ((uintptr_t)(addr) - KERNEL_VIRTUAL_BASE)
#define P2V(addr) ((uintptr_t)(addr) + KERNEL_VIRTUAL_BASE)

typedef unsigned int size_t;

extern uint32_t page_directory[PAGE_DIR_ENTRIES];
extern uint32_t first_page_table[PAGE_TABLE_ENTRIES];

void init_paging();
void enable_paging(uint32_t* pd);
void unmap_page(uint32_t* pd, uint32_t index);
void reload_cr3();

void* find_available_page();
void free_page(void* addr);
void init_pmm();

typedef uint32_t pde;
typedef uint32_t pte;
void set_pde(pde* pt);
void get_address(pte* pt);

void *memset(void *dest, int val, size_t len);

#endif
