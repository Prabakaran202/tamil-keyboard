#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <errno.h>

// 1. விசைப்பலகையைத் திறத்தல் மற்றும் கைப்பற்றுதல் (Grab)
int keyboard_open(const char *device_path) {
    int fd = open(device_path, O_RDONLY);
    if (fd < 0) {
        perror("[Error] Failed to open keyboard device");
        return -1;
    }

    // 🔥 மிக முக்கியம்: விசைப்பலகையை லினக்ஸிடம் இருந்து பிடுங்குதல் (GRAB)
    // இதுதான் ஆங்கில எழுத்துக்கள் ஸ்க்ரீனில் பிரிண்ட் ஆவதைத் தடுக்கும்!
    if (ioctl(fd, EVIOCGRAB, 1) < 0) {
        perror("[Error] Failed to grab keyboard. Are you root?");
        close(fd);
        return -1;
    }

    printf("[BDH Tamizhi] Successfully grabbed %s. Original English input is now BLOCKED!\n", device_path);
    return fd;
}

// 2. கீபோர்டு ஈவென்ட்களைப் படித்தல் (Listen Loop)
void keyboard_listen(int fd) {
    struct input_event ev;
    ssize_t n;

    printf("[BDH Tamizhi] Listening for keystrokes... Press Ctrl+C to exit.\n");

    while (1) {
        n = read(fd, &ev, sizeof(ev));
        if (n == (ssize_t)-1) {
            perror("[Error] Failed to read event");
            break;
        } else if (n != sizeof(ev)) {
            continue; // முழுமையற்ற ஈவென்ட்டைத் தவிர்த்தல்
        }

        // EV_KEY (1) என்பது விசைப்பலகை ஈவென்ட்.
        // ev.value == 1 (Key Press), ev.value == 2 (Key Hold)
        if (ev.type == EV_KEY && (ev.value == 1 || ev.value == 2)) {
            // இப்போது நாம் எந்த Key அழுத்தப்பட்டது என்ற கோடை மட்டும் அச்சிடுகிறோம்
            printf("Key Code Pressed: %d\n", ev.code);
            
            // எதிர்காலத்தில் இங்குதான் உங்களின் Mapping Function-ஐ அழைக்கப் போகிறோம்:
            // process_and_send_tamil_key(ev.code);
        }
    }
}

// 3. விசைப்பலகையை விடுவித்தல் (Release)
void keyboard_close(int fd) {
    if (fd >= 0) {
        ioctl(fd, EVIOCGRAB, 0); // லினக்ஸிடம் விசைப்பலகையைத் திருப்பிக் கொடுத்தல்
        close(fd);
        printf("\n[BDH Tamizhi] Keyboard released cleanly.\n");
    }
}
