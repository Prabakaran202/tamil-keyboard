#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <stdint.h>

// =========================================================
// 1. விர்ச்சுவல் கீபோர்டு லாஜிக் (உங்க கோட்)
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
    if (c >= 'a' && c <= 'z') return KEY_A + (c - 'a'); // Simplified
    return KEY_SPACE;
}

void type_unicode(int fd, const char* hex_code) {
    emit_key(fd, KEY_LEFTCTRL, 1); emit_key(fd, KEY_LEFTSHIFT, 1);
    emit_key(fd, KEY_U, 1); emit_key(fd, KEY_U, 0);
    emit_key(fd, KEY_LEFTSHIFT, 0); emit_key(fd, KEY_LEFTCTRL, 0);

    for(int i = 0; i < strlen(hex_code); i++) {
        int k = char_to_key(hex_code[i]);
        emit_key(fd, k, 1); emit_key(fd, k, 0);
    }
    emit_key(fd, KEY_SPACE, 1); emit_key(fd, KEY_SPACE, 0);
}

// =========================================================
// 2. ட்ரான்ஸ்லேட்டர் லாஜிக் (OS-க்குத் தேவையான குறியீடுகள்)
// =========================================================
const uint32_t MEI_BASE_UNICODE[] = {
    0x0B95, 0x0B99, 0x0B9A, 0x0B9E, 0x0B9F, 0x0BA3, 
    0x0BA4, 0x0BA8, 0x0BAA, 0x0BAE, 0x0BAF, 0x0BB0, 
    0x0BB2, 0x0BB5, 0x0BB4, 0x0BB3, 0x0BB1, 0x0BA9
};

const uint32_t MATRA_UNICODE[] = {
    0x0000, 0x0BBE, 0x0BBF, 0x0BC0, 0x0BC1, 0x0BC2,
    0x0BC6, 0x0BC7, 0x0BC8, 0x0BCA, 0x0BCB, 0x0BCC
};

int translate_tamizhi_to_unicode(unsigned char tz_char, uint32_t* out_codes) {
    // உயிர்மெய் எழுத்துக்கள் (The Reverse Magic!)
    if (tz_char >= 0x20) {
        int offset = tz_char - 0x20;
        int mei_idx = offset / 12;
        int uyir_idx = offset % 12;

        out_codes[0] = MEI_BASE_UNICODE[mei_idx];

        if (uyir_idx == 0) return 1; // மாத்திரை இல்லை (உ-ம்: க)
        
        out_codes[1] = MATRA_UNICODE[uyir_idx]; // மாத்திரை (உ-ம்: ா)
        return 2;
    }
    return 0; 
}

// =========================================================
// 3. The Bridge Function (பாலம்) 🚀
// =========================================================
void type_tamizhi_char(int fd, unsigned char tz_char) {
    uint32_t unicodes[2]; // அதிகபட்சம் 2 யூனிகோடுகள் வரும் (உ-ம்: க + ா)
    int count = translate_tamizhi_to_unicode(tz_char, unicodes);
    
    for (int i = 0; i < count; i++) {
        char hex_str[10];
        // யூனிகோடை "0b95" போன்ற Hex String ஆக மாற்றுதல்
        sprintf(hex_str, "%04x", unicodes[i]); 
        
        // உங்க type_unicode ஃபங்ஷனுக்கு அனுப்புதல்
        type_unicode(fd, hex_str);
    }
}
