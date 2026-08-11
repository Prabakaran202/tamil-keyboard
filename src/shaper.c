#include <stdint.h>
#include "tamil.h"

// =========================================================
// 🌐 UNICODE MAPPING TABLES (OS-க்குத் தேவையான குறியீடுகள்)
// =========================================================

// 1. உயிர் எழுத்துக்கள் (அ முதல் ஒள வரை)
const uint32_t UYIR_UNICODE[] = {
    0x0B85, 0x0B86, 0x0B87, 0x0B88, 0x0B89, 0x0B8A, 
    0x0B8E, 0x0B8F, 0x0B90, 0x0B92, 0x0B93, 0x0B94
};

// 2. மெய் எழுத்துக்களின் அடிப்படை (க, ங, ச... - புள்ளி இல்லாமல்)
const uint32_t MEI_BASE_UNICODE[] = {
    0x0B95, 0x0B99, 0x0B9A, 0x0B9E, 0x0B9F, 0x0BA3, 
    0x0BA4, 0x0BA8, 0x0BAA, 0x0BAE, 0x0BAF, 0x0BB0, 
    0x0BB2, 0x0BB5, 0x0BB4, 0x0BB3, 0x0BB1, 0x0BA9
};

// 3. உயிர்மெய் மாத்திரைகள் (Vowel Signs/Matras)
// 0-க்கு மாத்திரை இல்லை (அ வரிசை), 1 = ா, 2 = ி, 3 = ீ...
const uint32_t MATRA_UNICODE[] = {
    0x0000, 0x0BBE, 0x0BBF, 0x0BC0, 0x0BC1, 0x0BC2,
    0x0BC6, 0x0BC7, 0x0BC8, 0x0BCA, 0x0BCB, 0x0BCC
};

#define PULLI_UNICODE 0x0BCD // ் (புள்ளி)
#define AYUTHAM_UNICODE 0x0B83 // ஃ

// =========================================================
// 🧠 TRANSLATOR LOGIC (1-Byte to Unicode)
// =========================================================
// குறிப்பு: சில எழுத்துக்களுக்கு 2 யூனிகோடுகள் தேவைப்படும் (உ-ம்: க் = க + ்). 
// அதனால் out_codes என்ற Array-ல் விடையைத் தருகிறது.

int translate_tamizhi_to_unicode(unsigned char tz_char, uint32_t* out_codes) {
    // 1. ஆயுத எழுத்து
    if (tz_char == TZ_AK) {
        out_codes[0] = AYUTHAM_UNICODE; 
        return 1; // 1 Unicode அனுப்ப வேண்டும்
    }
    // 2. உயிர் எழுத்துக்கள்
    else if (tz_char >= TZ_A && tz_char <= TZ_AU) {
        out_codes[0] = UYIR_UNICODE[tz_char - TZ_A]; 
        return 1;
    }
    // 3. மெய் எழுத்துக்கள் (உ-ம்: க் = க + ்)
    else if (tz_char >= TZ_IK && tz_char <= TZ_INN) {
        out_codes[0] = MEI_BASE_UNICODE[tz_char - TZ_IK]; // க
        out_codes[1] = PULLI_UNICODE;                     // ்
        return 2; // 2 Unicodes அனுப்ப வேண்டும்
    }
    // 4. உயிர்மெய் எழுத்துக்கள் (The Reverse Magic!)
    else if (tz_char >= 0x20) {
        int offset = tz_char - 0x20;
        int mei_idx = offset / 12; // மெய் வரிசையைக் கண்டுபிடித்தல்
        int uyir_idx = offset % 12; // மாத்திரையைக் கண்டுபிடித்தல்

        out_codes[0] = MEI_BASE_UNICODE[mei_idx]; // அடிப்படை எழுத்து

        if (uyir_idx == 0) {
            return 1; // 'அ' வரிசைக்கு மாத்திரை இல்லை (உ-ம்: க)
        } else {
            out_codes[1] = MATRA_UNICODE[uyir_idx]; // மாத்திரை (உ-ம்: ா)
            return 2;
        }
    }
    return 0; // Invalid Key
}
