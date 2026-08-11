
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mmap.h>

#include <linux/fb.h>

// புதிய ஹெடர் ஃபைலை இணைக்கிறோம்
#include "../include/tty_fb.h"

static int fbfd = -1;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static long int screensize = 0;
static char *fbp = 0;

int fb_init(void) {
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        return -1;
    }

    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        return -1;
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        return -1;
    }

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((intptr_t)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        return -1;
    }
    return 0;
}

void fb_draw_box(int start_x, int start_y, int width, int height) {
    if (fbfd == -1 || fbp == 0) return;

    for (int y = start_y; y < start_y + height; y++) {
        for (int x = start_x; x < start_x + width; x++) {
            if (x >= (int)vinfo.xres || y >= (int)vinfo.yres || x < 0 || y < 0) continue;

            long int location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                                (y + vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = 255;        // Blue
                *(fbp + location + 1) = 255;    // Green
                *(fbp + location + 2) = 255;    // Red
                *(fbp + location + 3) = 0;      // Transparency
            }
        }
    }
}

void fb_cleanup(void) {
    if (fbp != 0 && fbp != (char *)-1) {
        munmap(fbp, screensize);
        fbp = 0;
    }
    if (fbfd != -1) {
        close(fbfd);
        fbfd = -1;
    }
}

int fb_run_test(void) {
    printf("[BDH Terminal Engine] Initializing TTY Framebuffer...\n");
    if (fb_init() == -1) return -1;
    
    printf("Drawing 30x30 Bounding Box at X:100, Y:100...\n");
    fb_draw_box(100, 100, 30, 30);
    
    sleep(3); 
    
    fb_cleanup();
    printf("TTY Framebuffer closed cleanly.\n");
    return 0;
}
