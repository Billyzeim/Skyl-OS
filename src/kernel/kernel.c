#include "../include/log.h"
#include "../include/drivers/keyboard.h"

void kernel_main() {
    print("Kernel is running...\n");
    
    char *vidptr = (char*)0xb8000; 	//video mem begins here.
	unsigned int i = 0;
	unsigned int j = 0;

	/* this loops clears the screen
	* there are 25 lines each of 80 columns; each element takes 2 bytes */
	while(j < 80 * 25 * 2) {
		/* blank character */
		vidptr[j] = ' ';
		/* attribute-byte - light grey on black screen */
		vidptr[j+1] = 0x07; 		
		j = j + 2;
	}

    while (1) {
        // Poll for keyboard input
        unsigned char scancode = read_keyboard();
        if (scancode != 0) {
            // Handle the scancode (or convert to ASCII)
            const char ascii = scancode_to_ascii(scancode);
            if (ascii) {
                const char str[2] = {ascii, '\0'};
                print(str);
                // print("\n");
            }
        }
    }
}