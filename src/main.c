#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/ioctl.h> // TIOCSTI-க்காக
#include "mapping.h"

#define KEYBOARD_DEVICE "/dev/input/event5"

int create_virtual_keyboard();
void emit_key(int fd, int keycode, int value);

// 🔥 The TTY Hacker: டெர்மினலின் உள்ளே பைட்டுகளை உட்செலுத்தும் வித்தை!
void inject_tty_string(const char* str) {
    // தற்போது ஆக்டிவாக உள்ள TTY-ஐத் திறக்கிறோம்
    int fd = open("/dev/tty", O_WRONLY);
    if (fd < 0) return;

    // ஒவ்வொரு UTF-8 பைட்டையும் தனித்தனியாக டெர்மினலுக்குள் செலுத்துகிறோம்
    while (*str) {
        ioctl(fd, TIOCSTI, str);
        str++;
    }
    close(fd);
}

int main() {
    struct input_event ev;
    
    int v_kbd = create_virtual_keyboard(); // Backspace, Enter வேலை செய்ய இது தேவை
    if (v_kbd < 0) return EXIT_FAILURE;

    int real_kbd = open(KEYBOARD_DEVICE, O_RDONLY);
    if (real_kbd < 0 || ioctl(real_kbd, EVIOCGRAB, 1) < 0) {
        perror("Failed to open or grab real keyboard");
        return EXIT_FAILURE;
    }

    printf("[BDH Tamizhi] TTY Injector Running! Type anywhere in the terminal!\n");

    while (1) {
        if (read(real_kbd, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_KEY && ev.value == 1) { // Key Press
                const char* tamil_char = get_tamizhi_char(ev.code);
                
                if (tamil_char != NULL) {
                    // 🎯 தமிழ் பட்டன் என்றால், TTY-க்குள் நேரடியாக இன்ஜெக்ட் செய்!
                    inject_tty_string(tamil_char);
                } else {
                    // Enter, Backspace, Arrow keys என்றால் விர்ச்சுவல் கீபோர்டு மூலம் அனுப்பு
                    emit_key(v_kbd, ev.code, 1);
                    emit_key(v_kbd, ev.code, 0);
                }
            }
        }
    }

    ioctl(real_kbd, EVIOCGRAB, 0);
    close(real_kbd);
    close(v_kbd);
    return EXIT_SUCCESS;
}
