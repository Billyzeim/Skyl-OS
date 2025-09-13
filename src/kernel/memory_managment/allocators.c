#include <stdint.h>
#include <allocators.h>

typedef unsigned int size_t;

#define PAGE_SIZE 4096
#define BITMAP_SIZE_BYTES (64 * 1024 * 1024 / PAGE_SIZE / 8)  // 64MiB / 4KiB / 8 = 2048 bytes

static uint8_t physical_bitmap[BITMAP_SIZE_BYTES];

#define BITMAP_GET(bitmap, index) ((bitmap[(index) / 8] >> ((index) % 8)) & 1)
#define BITMAP_SET(bitmap, index) (bitmap[(index) / 8] |= (1 << ((index) % 8)))
#define BITMAP_CLEAR(bitmap, index) (bitmap[(index) / 8] &= ~(1 << ((index) % 8)))

void* alloc_page() {
    for (uint32_t i = 0; i < BITMAP_SIZE_BYTES * 8; i++) {
        if (!BITMAP_GET(physical_bitmap, i)) {
            BITMAP_SET(physical_bitmap, i);
            return (void*)(i * PAGE_SIZE);
        }
    }
    return 0; // Out of memory
}

void free_page(void* addr) {
    uint32_t index = (uint32_t)addr / PAGE_SIZE;
    BITMAP_CLEAR(physical_bitmap, index);
}

void *memset(void *dest, int val, size_t len) {
    unsigned char *ptr = dest;
    while (len-- > 0)
        *ptr++ = (unsigned char)val;
    return dest;
}

void init_pmm() {
    memset(physical_bitmap, 0, sizeof(physical_bitmap));

    // Mark first N pages as used (bootloader, kernel, bitmap itself)
    uint32_t used = 0x100000 / PAGE_SIZE;  // Example: kernel ends at 1 MiB
    for (uint32_t i = 0; i < used; i++) {
        BITMAP_SET(physical_bitmap, i);
    }

    // Also mark the pages that store the bitmap itself
    uintptr_t bitmap_start = (uintptr_t)physical_bitmap;
    uintptr_t bitmap_end = bitmap_start + BITMAP_SIZE_BYTES;
    for (uintptr_t addr = bitmap_start; addr < bitmap_end; addr += PAGE_SIZE) {
        BITMAP_SET(physical_bitmap, addr / PAGE_SIZE);
    }
}
