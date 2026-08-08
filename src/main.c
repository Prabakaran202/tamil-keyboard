#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "mapping.h"

#define KEYBOARD_DEVICE "/dev/input/event5" // உங்கள் SEMICO Keyboard

// நாம் uinput_kbd.c ல் உருவாக்கிய ஃபங்ஷன்களை அறிவித்தல்
int create_virtual_keyboard();
void type_unicode(int fd, const char* hex_code);
void emit_key(int fd, int keycode, int value);

int main() {
    struct input_event ev;
    
    // 1. விர்ச்சுவல் கீபோர்டை உருவாக்குதல்
    int v_kbd = create_virtual_keyboard();
    if (v_kbd < 0) return EXIT_FAILURE;

    // 2. நிஜ கீபோர்டைத் திறத்தல்
    int real_kbd = open(KEYBOARD_DEVICE, O_RDONLY);
    if (real_kbd < 0 || ioctl(real_kbd, EVIOCGRAB, 1) < 0) {
        perror("Failed to open or grab real keyboard");
        return EXIT_FAILURE;
    }

    printf("[BDH Tamizhi] Daemon Running! Go to any Text Editor and type!\n");

    while (1) {
        if (read(real_kbd, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_KEY && ev.value == 1) { // Key Press Only
                const char* hex_code = get_tamizhi_char(ev.code);
                
                if (hex_code != NULL) {
                    // மேப் செய்யப்பட்டிருந்தால், uinput மூலம் தமிழை டைப் செய்!
                    type_unicode(v_kbd, hex_code);
                } else {
                    // மேப் செய்யப்படாத பட்டன்களை (எ.கா: Enter, Backspace) அப்படியே அனுப்புதல்
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
