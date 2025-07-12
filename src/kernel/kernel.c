#include <terminal_io.h>

void kernel_main() {
    volatile unsigned short *video = (unsigned short*)0xB8000;
    // Clear the screen (80x25 text mode: 80 columns, 25 rows)
    for (int i = 0; i < 80 * 25; i++) {
        video[i] = 0x0720;  // 0x07 (white on black) + 0x20 (space character)
    }

    printf ("Characters: %c %c \n", 'a', 65);
   printf ("Decimals: %d %ld\n", 1977, 650000L);
   printf ("Preceding with blanks: %10d \n", 1977);
   printf ("Preceding with zeros: %010d \n", 1977);
   printf ("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
   printf ("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
   printf ("Width trick: %*d \n", 5, 10);
   printf ("%s \n", "A string");


    while(1);
    return;
}