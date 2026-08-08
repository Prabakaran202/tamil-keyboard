#include "mapping.h"
#include <stddef.h>
#include <linux/input.h>

// யூனிகோடு Hex-ஐ எடுத்துவிட்டு, மீண்டும் உண்மையான தமிழைப் போடுங்கள்
const char* get_tamizhi_char(int keycode) {
    switch (keycode) {
        // --- Top Row (QWERTY...) ---
        case KEY_Q: return "அ"; 
        case KEY_W: return "ஆ"; 
        case KEY_E: return "இ"; 
        case KEY_R: return "ஈ"; 
        case KEY_T: return "உ"; 
        case KEY_Y: return "ட்"; 
        case KEY_U: return "ஞ்"; 
        case KEY_I: return "ச்"; 
        case KEY_O: return "ங்"; 
        case KEY_P: return "க்"; 

        // --- Middle Row (ASDFGH...) ---
        case KEY_A: return "ஊ"; 
        case KEY_S: return "எ"; 
        case KEY_D: return "ஏ"; 
        case KEY_F: return "ஐ"; 
        case KEY_G: return "ஒ"; 
        case KEY_H: return "ய்"; 
        case KEY_J: return "ம்"; 
        case KEY_K: return "ப்"; 
        case KEY_L: return "ந்"; 
        case KEY_SEMICOLON: return "த்";
        case KEY_APOSTROPHE: return "ண்";

        // --- Bottom Row (ZXCVBN...) ---
        case KEY_Z: return "ஓ"; 
        case KEY_X: return "ஔ"; 
        case KEY_C: return "ன்"; 
        case KEY_V: return "ற்"; 
        case KEY_B: return "ள்"; 
        case KEY_N: return "ழ்"; 
        case KEY_M: return "வ்"; 
        case KEY_COMMA: return "ல்";
        case KEY_DOT: return "ர்";
        
        default: return NULL;
    }
}
