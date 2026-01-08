#include <stdint.h>

#define VGA_MEM   ((volatile uint16_t*)0xB8000)
#define VGA_COLS  80
#define VGA_ROWS  25
#define VGA_ATTR  0x0F

#define TRAMP_TEXT   __attribute__((section(".tramp.text"), noclone))
#define TRAMP_DATA   __attribute__((section(".tramp.data")))
#define TRAMP_BSS    __attribute__((section(".tramp.bss")))
#define TRAMP_RODATA __attribute__((section(".tramp.rodata")))

TRAMP_BSS static uint8_t tramp_cx;
TRAMP_BSS static uint8_t tramp_cy;

TRAMP_RODATA static const char MSG_TITLE[] = "[TRMP]: Setting up Paging...\n";

TRAMP_TEXT static inline uint16_t vga_cell(char ch, uint8_t attr) {
    return (uint16_t)ch | ((uint16_t)attr << 8);
}

TRAMP_TEXT static void tramp_hwcursor_update(void) {
    uint16_t pos = (uint16_t)(tramp_cy * VGA_COLS + tramp_cx);
    __asm__ __volatile__(
        "mov $0x3D4, %%dx; mov $0x0F, %%al; out %%al, %%dx;"
        "mov $0x3D5, %%dx; mov %0, %%al; out %%al, %%dx;"
        "mov $0x3D4, %%dx; mov $0x0E, %%al; out %%al, %%dx;"
        "mov $0x3D5, %%dx; mov %1, %%al; out %%al, %%dx;"
        : : "r" ((uint8_t)(pos & 0xFF)), "r" ((uint8_t)(pos >> 8)) : "al","dx"
    );
}

TRAMP_TEXT void tramp_putc(char ch) {
    if (ch == '\n') { tramp_cx = 0; tramp_cy++; }
    else {
        VGA_MEM[tramp_cy * VGA_COLS + tramp_cx] = vga_cell(ch, VGA_ATTR);
        if (++tramp_cx >= VGA_COLS) { tramp_cx = 0; tramp_cy++; }
    }
    tramp_hwcursor_update();
}

TRAMP_TEXT void tramp_puts(const char* s) {
    while (*s) tramp_putc(*s++);
}

TRAMP_TEXT void tramp_clear_screen(void) {
    for (uint8_t y = 0; y < VGA_ROWS; y++) {
        for (uint8_t x = 0; x < VGA_COLS; x++) {
            VGA_MEM[y * VGA_COLS + x] = vga_cell(' ', VGA_ATTR);
        }
    }
    tramp_cx = 0;
    tramp_cy = 0;
    tramp_hwcursor_update();
}

TRAMP_TEXT void tramp_puthex(uint32_t val) {
    const char* hex_chars = "0123456789ABCDEF";
    tramp_puts("0x");
    for (int i = 28; i >= 0; i -= 4) {
        tramp_putc(hex_chars[(val >> i) & 0xF]);
    }
}

TRAMP_DATA uint32_t boot_page_directory[1024] __attribute__((aligned(4096)));

extern void enable_paging(uint32_t pd_phys_addr);
extern void kernel_init(void);

TRAMP_TEXT void tramp(void) {
    tramp_cx = 0; tramp_cy = 0;
    tramp_clear_screen();
    tramp_puts(MSG_TITLE);

    for(int i = 0; i < 1024; i++) boot_page_directory[i] = 0;
    uint32_t entry_flags = 0x83;

    boot_page_directory[0] = (0x00000000 | entry_flags);
    boot_page_directory[768] = (0x00000000 | entry_flags);

    // tramp_puts("Enabling Paging Hardware...\n");
    enable_paging((uint32_t)boot_page_directory);
    tramp_puts("[TRMP]: SUCCESS.\n");
    kernel_init();
    while(1);
}