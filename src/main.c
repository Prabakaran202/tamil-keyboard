#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "keyboard.h"
#include "mapping.h"
#include "unicode.h"
// Framebuffer ஹெடரை இணைக்கிறோம்
#include "../include/tty_fb.h" 

// கர்சரின் இருப்பிடத்தைக் கணக்கிட Global Variables
int cursor_x = 50;  // ஸ்கிரீனில் தொடங்கும் X இடம்
int cursor_y = 50;  // ஸ்கிரீனில் தொடங்கும் Y இடம்
const int FONT_WIDTH = 30; // நம்ம 30-பிக்சல் லாஜிக்

int main() {
    printf("\033[2J\033[H");
    printf("\033[1;36m=============================================\n");
    printf(" 🚀 BDH TAMIZHI TERMINAL ENGINE \n");
    printf("=============================================\033[0m\n");
    
    // 1. Framebuffer-ஐ துவக்குதல்
    if (fb_init() < 0) {
        printf("[Error] BDH Framebuffer failed!\n");
        return -1;
    }
    printf("[System] Framebuffer Initialized for Rendering.\n");
    
    enable_raw_mode();
    
    while (1) {
        char key = get_keypress();
        
        if (key == 3) { // Ctrl+C
            break;
        }

        // BACKSPACE லாஜிக்
        if (key == 127 || key == 8) {
            current_mei_state = -1; 
            
            // கர்சரை ஒரு ஸ்டெப் பின்னாடி கொண்டு வருகிறோம்
            if (cursor_x >= 50 + FONT_WIDTH) {
                cursor_x -= FONT_WIDTH;
                // [TODO]: Framebuffer-ல் அந்த இடத்தை கறுப்பு நிறமாக மாற்றி அழிக்க வேண்டும்
            }
            continue; 
        }

        int previous_state = current_mei_state;
        unsigned char tz_code = process_key(key);
        
        // --- The Internal Backspace Magic ---
        if (previous_state != -1 && tz_code >= 0x20) {
            // கர்சரை பின்னாடி நகர்த்தி பழைய 'க்' பிக்சல்களை அழிக்க வேண்டும்
            if (cursor_x >= 50 + FONT_WIDTH) {
                cursor_x -= FONT_WIDTH;
            }
        }
        
        // 2. 1-byte கோடை Framebuffer-க்கு அனுப்புதல்
        // [குறிப்பு: FreeType இணைத்த பிறகு இது உண்மையான எழுத்தை வரையும்]
        fb_draw_box(cursor_x, cursor_y, FONT_WIDTH, FONT_WIDTH);
        
        // அடுத்த எழுத்துக்காக கர்சரை நகர்த்துகிறோம்
        cursor_x += FONT_WIDTH;
    }
    
    disable_raw_mode();
    fb_cleanup(); // Framebuffer-ஐ க்ளீன் செய்கிறோம்
    printf("\n\n\033[1;32mExiting BDH Terminal Engine... Nandri!\033[0m\n");
    return 0;
}
