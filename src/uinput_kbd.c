#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <stdint.h>

// ஷேப்பர் லாஜிக்கை இங்கிருந்து அழைக்கிறோம் (Linker Error தீர்வு)
#include "../include/shaper.h" 

// =========================================================
// 1. விர்ச்சுவல் கீபோர்டு லாஜிக் 
// =========================================================
int create_virtual_keyboard() {
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) { perror("Error"); return -1; }

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_SYN);

    ioctl(fd, UI_SET_KEYBIT, KEY_LEFTCTRL);
    ioctl(fd, UI_SET_KEYBIT, KEY_LEFTSHIFT);
    ioctl(fd, UI_SET_KEYBIT, KEY_U);
    ioctl(fd, UI_SET_KEYBIT, KEY_SPACE);
    
    for(int i = KEY_1; i <= KEY_9; i++) ioctl(fd, UI_SET_KEYBIT, i);
    ioctl(fd, UI_SET_KEYBIT, KEY_0);
    for(int i = KEY_Q; i <= KEY_P; i++) ioctl(fd, UI_SET_KEYBIT, i);
    for(int i = KEY_A; i <= KEY_L; i++) ioctl(fd, UI_SET_KEYBIT, i);
    for(int i = KEY_Z; i <= KEY_M; i++) ioctl(fd, UI_SET_KEYBIT, i);

    struct uinput_setup usetup;
    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    strcpy(usetup.name, "BDH Tamizhi Virtual Keyboard");

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);
    usleep(100000); 

    return fd;
}

void emit_key(int fd, int keycode, int value) {
    struct input_event ie;
    memset(&ie, 0, sizeof(ie));
    ie.type = EV_KEY; ie.code = keycode; ie.value = value;
    write(fd, &ie, sizeof(ie));
    
    memset(&ie, 0, sizeof(ie));
    ie.type = EV_SYN; ie.code = SYN_REPORT; ie.value = 0;
    write(fd, &ie, sizeof(ie));
}

int char_to_key(char c) {
    if (c >= '1' && c <= '9') return KEY_1 + (c - '1');
    if (c == '0') return KEY_0;
    if (c >= 'a' && c <= 'z') return KEY_A + (c - 'a'); 
    return KEY_SPACE;
}

void type_unicode(int fd, const char* hex_code) {
    emit_key(fd, KEY_LEFTCTRL, 1); emit_key(fd, KEY_LEFTSHIFT, 1);
    emit_key(fd, KEY_U, 1); emit_key(fd, KEY_U, 0);
    emit_key(fd, KEY_LEFTSHIFT, 0); emit_key(fd, KEY_LEFTCTRL, 0);

    // Warning தீர்வு: int i என்பதற்குப் பதிலாக size_t i பயன்படுத்தப்பட்டுள்ளது
    for(size_t i = 0; i < strlen(hex_code); i++) {
        int k = char_to_key(hex_code[i]);
        emit_key(fd, k, 1); emit_key(fd, k, 0);
    }
    emit_key(fd, KEY_SPACE, 1); emit_key(fd, KEY_SPACE, 0);
}

// =========================================================
// 2. The Bridge Function (பாலம்) 🚀
// =========================================================
void type_tamizhi_char(int fd, unsigned char tz_char) {
    uint32_t unicodes[2]; 
    
    // translate_tamizhi_to_unicode இப்போது shaper.c-ல் இருந்து வரும்
    int count = translate_tamizhi_to_unicode(tz_char, unicodes);
    
    for (int i = 0; i < count; i++) {
        char hex_str[10];
        sprintf(hex_str, "%04x", unicodes[i]); 
        type_unicode(fd, hex_str);
    }
}
