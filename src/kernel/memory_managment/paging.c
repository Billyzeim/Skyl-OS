#include <stdint.h>
#include <paging.h>
#include <terminal_io.h>

uint32_t __attribute__((aligned(PAGE_SIZE))) page_directory[PAGE_DIR_ENTRIES];
uint32_t __attribute__((aligned(PAGE_SIZE))) first_page_table[PAGE_TABLE_ENTRIES];
uint32_t __attribute__((aligned(PAGE_SIZE))) hh_page_table[PAGE_TABLE_ENTRIES];

extern char __hh_lma[]; /* LMA of .text.hh from linker script */

static inline uint32_t align_down(uint32_t x, uint32_t a) { return x & ~(a - 1); }

void init_paging() {
    /* Clear directory and both tables */
    for (int i = 0; i < PAGE_DIR_ENTRIES; i++) page_directory[i]   = 0x00000002; /* rw, not present */
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) first_page_table[i] = 0;
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) hh_page_table[i]    = 0;

    /* ---- Identity map first 4 MiB (PDE 0) ---- */
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | 3; /* present | rw */
    }
    page_directory[0] = ((uint32_t)first_page_table) | 3;

    /* ---- Map higher-half 0xC0000000..0xC0000000+4MiB to the LMA of .text.hh (PDE 768) ---- */
    uint32_t phys_base = align_down((uint32_t)(uintptr_t)__hh_lma, PAGE_SIZE);
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        hh_page_table[i] = (phys_base + (i * PAGE_SIZE)) | 3; /* present | rw */
    }
    page_directory[768] = ((uint32_t)hh_page_table) | 3;
}

void enable_paging(uint32_t* pd) {
    asm volatile("mov %0, %%cr3" :: "r"(pd) : "memory");  /* Load page directory */
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;  /* Set PG bit */
    asm volatile("mov %0, %%cr0" :: "r"(cr0) : "memory");
    asm volatile("jmp 1f\n1:"); /* serialize */
}

void unmap_page(uint32_t* pd, uint32_t index) {
    if (index < PAGE_DIR_ENTRIES) {
        pd[index] = 0x00000002; /* rw, not present */
    }
}

void reload_cr3() {
    asm volatile("mov %0, %%cr3" :: "r"(page_directory) : "memory");
}
