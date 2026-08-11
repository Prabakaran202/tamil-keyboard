
#ifndef TTY_FB_H
#define TTY_FB_H

// Framebuffer-ஐ துவக்குவதற்கான ஃபங்ஷன்
int fb_init(void);

// ஒரு குறிப்பிட்ட இடத்தில் 30x30 பாக்ஸை (Bounding Box) வரைவதற்கான ஃபங்ஷன்
void fb_draw_box(int start_x, int start_y, int width, int height);

// மெமரியை க்ளீன் செய்து Framebuffer-ஐ மூடுவதற்கான ஃபங்ஷன்
void fb_cleanup(void);

// தற்காலிகமாக வேலை செய்கிறதா என சோதிக்க ஒரு டெஸ்ட் ஃபங்ஷன்
int fb_run_test(void);

#endif // TTY_FB_H
