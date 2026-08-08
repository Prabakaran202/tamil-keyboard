#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "mapping.h" // நாம் உருவாக்கிய புதிய லாஜிக் ஃபைல்!

#define KEYBOARD_DEVICE "/dev/input/event0" 

int main() {
    int fd;
    struct input_event ev;

    printf("[BDH Tamizhi] Starting Keyboard Daemon...\n");

    // Open the keyboard device file (Requires SUDO/Root)
    fd = open(KEYBOARD_DEVICE, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open keyboard device (Are you root?)");
        printf("Hint: Please run this on your Linux machine with sudo!\n");
        return EXIT_FAILURE;
    }

    printf("Successfully connected to %s. Listening for keys...\n", KEYBOARD_DEVICE);
    printf("Press CTRL+C to exit.\n");

    // Infinite loop to read raw physical keypresses
    while (1) {
        if (read(fd, &ev, sizeof(struct input_event)) > 0) {
            // Check only for Key Press (value = 1)
            if (ev.type == EV_KEY && ev.value == 1) {
                
                // Get the mapped Tamizhi character
                const char* tamil_char = get_tamizhi_char(ev.code);
                
                if (tamil_char != NULL) {
                    // Mapped key found!
                    printf("Tamizhi Key Pressed: %s\n", tamil_char);
                } else {
                    // Key not mapped in our switch case yet
                    printf("Unmapped Key Code: %d\n", ev.code);
                }
            }
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}

