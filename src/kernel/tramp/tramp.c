// tramp_print.c (patched)

#include <stdint.h>

#define VGA_MEM   ((volatile uint16_t*)0xB8000)
#define VGA_COLS  80
#define VGA_ROWS  25
#define VGA_ATTR  0x07

__attribute__((section(".bss.tramp"))) static uint8_t tramp_cx = 0, tramp_cy = 0;

__attribute__((section(".text.tramp")))
static inline uint16_t vga_cell(char ch, uint8_t attr) {
    return (uint16_t)ch | ((uint16_t)attr << 8);
}

__attribute__((section(".text.tramp"))) static void tramp_hwcursor_update(void) {
    uint16_t pos = (uint16_t)(tramp_cy * VGA_COLS + tramp_cx);
    __asm__ __volatile__(
        "mov $0x3D4, %%dx; mov $0x0F, %%al; out %%al, %%dx;"
        "mov $0x3D5, %%dx; mov %0, %%al; out %%al, %%dx;"
        "mov $0x3D4, %%dx; mov $0x0E, %%al; out %%al, %%dx;"
        "mov $0x3D5, %%dx; mov %1, %%al; out %%al, %%dx;"
        :
        : "r" ((uint8_t)(pos & 0xFF)), "r" ((uint8_t)(pos >> 8))
        : "al","dx"
    );
}

__attribute__((section(".text.tramp"))) static void tramp_scroll(void) {
    if (tramp_cy < VGA_ROWS) return;
    for (uint32_t r = 1; r < VGA_ROWS; ++r)
        for (uint32_t c = 0; c < VGA_COLS; ++c)
            VGA_MEM[(r-1)*VGA_COLS + c] = VGA_MEM[r*VGA_COLS + c];
    for (uint32_t c = 0; c < VGA_COLS; ++c)
        VGA_MEM[(VGA_ROWS-1)*VGA_COLS + c] = vga_cell(' ', VGA_ATTR);
    tramp_cy = VGA_ROWS - 1;
}

__attribute__((section(".text.tramp"))) void tramp_clear(void) {
    for (uint32_t i = 0; i < VGA_COLS * VGA_ROWS; ++i)
        VGA_MEM[i] = vga_cell(' ', VGA_ATTR);
    tramp_cx = tramp_cy = 0;
    tramp_hwcursor_update();
}

__attribute__((section(".text.tramp"))) void tramp_putc(char ch) {
    if (ch == '\n') { tramp_cx = 0; ++tramp_cy; tramp_scroll(); tramp_hwcursor_update(); return; }
    if (ch == '\r') { tramp_cx = 0; tramp_hwcursor_update(); return; }
    if (ch == '\t') { tramp_cx = (uint8_t)((tramp_cx + 8) & ~7); if (tramp_cx >= VGA_COLS) { tramp_cx = 0; ++tramp_cy; tramp_scroll(); } tramp_hwcursor_update(); return; }
    VGA_MEM[tramp_cy * VGA_COLS + tramp_cx] = vga_cell(ch, VGA_ATTR);
    if (++tramp_cx >= VGA_COLS) { tramp_cx = 0; ++tramp_cy; tramp_scroll(); }
    tramp_hwcursor_update();
}

__attribute__((section(".text.tramp"))) void tramp_puts(const char* s) {
    for (; *s; ++s) tramp_putc(*s);
}

__attribute__((section(".text.tramp"))) static char hex_nibble(uint8_t v) {
    return (v < 10) ? ('0' + v) : ('A' + (v - 10));
}

__attribute__((section(".text.tramp"))) void tramp_puthex32(uint32_t x) {
    // use local char array to avoid string literal
    char buf[10];
    buf[0] = '0'; buf[1] = 'x'; buf[2] = 0;
    tramp_puts(buf);
    for (int i = 7; i >= 0; --i) {
        uint8_t nib = (x >> (i * 4)) & 0xF;
        tramp_putc(hex_nibble(nib));
    }
}

__attribute__((section(".text.tramp"))) void tramp_putudec(uint32_t v) {
    char buf[10]; int i = 0;
    if (v == 0) { tramp_putc('0'); return; }
    while (v > 0 && i < 10) {
        uint32_t q = 0, r = 0;
        for (int b = 31; b >= 0; --b) { r = (r << 1) | ((v >> b) & 1u); if (r >= 10) { r -= 10; q |= (1u << b); } }
        buf[i++] = (char)('0' + r);
        v = q;
    }
    while (--i >= 0) tramp_putc(buf[i]);
}

/* Put *messages* into .rodata.tramp so they’re safe pre-paging */
__attribute__((section(".rodata.tramp"))) static const char MSG_TITLE[]   = "Tramp Print Test:";
__attribute__((section(".rodata.tramp"))) static const char MSG_HEX[]     = "Hex: ";
__attribute__((section(".rodata.tramp"))) static const char MSG_DEC[]     = "Decimal: ";
__attribute__((section(".rodata.tramp"))) static const char MSG_DONE[]    = "Done.";

extern void debug();

__attribute__((section(".text.tramp"), used))
void tramp(void) {
    volatile uint16_t* v = (volatile uint16_t*)0xB8000;
    // Call your clear
    tramp_clear();
    // Write a post-marker so we know we returned
    v[1] = 0x0743; // 'C' at cell 1
    for(;;) __asm__("hlt");
}
