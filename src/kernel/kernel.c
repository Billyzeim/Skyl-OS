#include <terminal_io.h>

void kernel_main() {
    volatile unsigned short *video = (unsigned short*)0xB8000;
    // Clear the screen (80x25 text mode: 80 columns, 25 rows)
    for (int i = 0; i < 80 * 25; i++) {
        video[i] = 0x0720;  // 0x07 (white on black) + 0x20 (space character)
    }
    int i  = 0;
    int b = 0;
    char * buf;
    while (i < 10) {
        int a = scanf("%d", &b);
        printf("You entered: %d\n", b);
        switch (b) {
            case 2: __asm__ __volatile__("int $0x22"); break;
            case 3: __asm__ __volatile__("int $0x23"); break;
            case 4: __asm__ __volatile__("int $0x24"); break;
            case 5: __asm__ __volatile__("int $0x25"); break;
            // add more if needed
            default: printf("whoops\n"); break;
        }
        i++;
    }
    while(1);
    return;
}