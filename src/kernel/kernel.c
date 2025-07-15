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
        printf("%d\n", a);
        i++;
    }
    while(1);
    return;
}