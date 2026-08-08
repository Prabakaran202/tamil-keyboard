#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>

// 1. விர்ச்சுவல் கீபோர்டை உருவாக்குதல்
int create_virtual_keyboard() {
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("[Error] Unable to open /dev/uinput");
        return -1;
    }

    // கீபோர்டு ஈவென்ட்களை அனுமதித்தல்
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_SYN);

    // நாம் பயன்படுத்தப் போகும் பட்டன்களை ரிஜிஸ்டர் செய்தல் (Ctrl, Shift, U, Space)
    ioctl(fd, UI_SET_KEYBIT, KEY_LEFTCTRL);
    ioctl(fd, UI_SET_KEYBIT, KEY_LEFTSHIFT);
    ioctl(fd, UI_SET_KEYBIT, KEY_U);
    ioctl(fd, UI_SET_KEYBIT, KEY_SPACE);
    
    // Hex எண்களுக்கான பட்டன்கள் (0-9, a-f)
    for(int i = KEY_1; i <= KEY_9; i++) ioctl(fd, UI_SET_KEYBIT, i);
    ioctl(fd, UI_SET_KEYBIT, KEY_0);
    for(int i = KEY_Q; i <= KEY_P; i++) ioctl(fd, UI_SET_KEYBIT, i);
    for(int i = KEY_A; i <= KEY_L; i++) ioctl(fd, UI_SET_KEYBIT, i);
    for(int i = KEY_Z; i <= KEY_M; i++) ioctl(fd, UI_SET_KEYBIT, i);

    struct uinput_setup usetup;
    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234;
    usetup.id.product = 0x5678;
    strcpy(usetup.name, "BDH Tamizhi Virtual Keyboard");

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);
    usleep(100000); // சிஸ்டம் கீபோர்டை அடையாளம் காண சிறு இடைவெளி

    return fd;
}

// 2. பட்டன்களை அழுத்தி எடுக்கும் லாஜிக்
void emit_key(int fd, int keycode, int value) {
    struct input_event ie;
    memset(&ie, 0, sizeof(ie));
    ie.type = EV_KEY;
    ie.code = keycode;
    ie.value = value; // 1 = Press, 0 = Release
    write(fd, &ie, sizeof(ie));

    memset(&ie, 0, sizeof(ie));
    ie.type = EV_SYN;
    ie.code = SYN_REPORT;
    ie.value = 0;
    write(fd, &ie, sizeof(ie));
}

// 3. கேரக்டரை Keycode-ஆக மாற்றுதல் (உ-ம்: 'b' -> KEY_B)
int char_to_key(char c) {
    if (c >= '1' && c <= '9') return KEY_1 + (c - '1');
    if (c == '0') return KEY_0;
    if (c == 'a') return KEY_A; if (c == 'b') return KEY_B;
    if (c == 'c') return KEY_C; if (c == 'd') return KEY_D;
    if (c == 'e') return KEY_E; if (c == 'f') return KEY_F;
    return KEY_SPACE;
}

// 4. மேஜிக்! Ctrl+Shift+U + Hex + Space மூலம் தமிழ் டைப் செய்தல்
void type_unicode(int fd, const char* hex_code) {
    // Press Ctrl + Shift
    emit_key(fd, KEY_LEFTCTRL, 1);
    emit_key(fd, KEY_LEFTSHIFT, 1);
    
    // Press & Release 'U'
    emit_key(fd, KEY_U, 1); emit_key(fd, KEY_U, 0);

    // Release Ctrl + Shift
    emit_key(fd, KEY_LEFTSHIFT, 0);
    emit_key(fd, KEY_LEFTCTRL, 0);

    // Type the Hex Code (e.g., "0b85")
    for(int i = 0; i < strlen(hex_code); i++) {
        int k = char_to_key(hex_code[i]);
        emit_key(fd, k, 1); emit_key(fd, k, 0);
    }

    // Press Space to confirm Unicode
    emit_key(fd, KEY_SPACE, 1); emit_key(fd, KEY_SPACE, 0);
}
