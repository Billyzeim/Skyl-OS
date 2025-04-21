void kernel_main() {
    volatile unsigned short *video = (unsigned short*)0xB8000;
    // Clear the screen (80x25 text mode: 80 columns, 25 rows)
    for (int i = 0; i < 80 * 25; i++) {
        video[i] = 0x0720;  // 0x07 (white on black) + 0x20 (space character)
    }
    const char msg[] = "Hello world !";
    for (int i = 0; msg[i] != '\0'; i++) {
        video[i] = (0x07 << 8) | msg[i];
    }
    while(1);
}
