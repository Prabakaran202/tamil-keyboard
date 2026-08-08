#include "mapping.h"
#include <stddef.h>

// Map Linux Keycodes to Tamizhi Unicode Characters
const char* get_tamizhi_char(int keycode) {
    switch (keycode) {
        case KEY_Q: return "அ"; 
        case KEY_W: return "ஆ"; 
        case KEY_E: return "இ"; 
        case KEY_R: return "ஈ"; 
        case KEY_T: return "உ"; 
        case KEY_Y: return "ஊ"; 
        
        // Consonants mapping examples
        case KEY_K: return "க்"; 
        case KEY_S: return "ச்"; 
        
        default: return NULL;
    }
}

