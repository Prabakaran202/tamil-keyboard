#include "mapping.h"
#include <stddef.h>
#include <linux/input.h>

// யூனிகோடு Hex எண்கள் (எ.கா: அ = 0b85)
const char* get_tamizhi_char(int keycode) {
    switch (keycode) {
        case KEY_Q: return "0b85"; // அ
        case KEY_W: return "0b86"; // ஆ
        case KEY_E: return "0b87"; // இ
        case KEY_R: return "0b88"; // ஈ
        case KEY_T: return "0b89"; // உ
        case KEY_A: return "0b8a"; // ஊ
        case KEY_S: return "0b8e"; // எ
        case KEY_D: return "0b8f"; // ஏ
        case KEY_F: return "0b90"; // ஐ
        case KEY_G: return "0b92"; // ஒ
        case KEY_Z: return "0b93"; // ஓ
        case KEY_X: return "0b94"; // ஔ
        case KEY_P: return "0b95"; // க (மெய்யெழுத்துக்குப் பிறகு மாற்றுவோம்)
        default: return NULL;
    }
}
