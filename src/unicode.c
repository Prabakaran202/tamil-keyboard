#include <stdio.h>
#include "unicode.h"
#include "tamil.h"

// 1. உயிர் எழுத்துக்கள் (12)
const char* uyir_utf8[12] = {
    "அ", "ஆ", "இ", "ஈ", "உ", "ஊ", 
    "எ", "ஏ", "ஐ", "ஒ", "ஓ", "ஔ"
};

// 2. மெய் எழுத்துக்களின் வேர் (Base Consonants - 18)
const char* mei_base_utf8[18] = {
    "க", "ங", "ச", "ஞ", "ட", "ண", 
    "த", "ந", "ப", "ம", "ய", "ர", 
    "ல", "வ", "ழ", "ள", "ற", "ன"
};

// 3. உயிர்மெய் குறியீடுகள் (Vowel Modifiers - 12)
// "அ" க்கு குறியீடு இல்லை (""), "ஆ" க்கு ("ா"), etc.
const char* uyirmei_suffix[12] = {
    "", "ா", "ி", "ீ", "ு", "ூ", 
    "ெ", "ே", "ை", "ொ", "ோ", "ௌ"
};

// புள்ளி (Virama)
const char* virama = "்";

void print_tz_char(unsigned char tz_code) {
    // 1. ஆயுத எழுத்து
    if (tz_code == TZ_AK) {
        printf("ஃ");
    } 
    // 2. உயிர் எழுத்துக்கள் (0x02 - 0x0D)
    else if (tz_code >= TZ_A && tz_code <= TZ_AU) {
        printf("%s", uyir_utf8[tz_code - TZ_A]);
    } 
    // 3. மெய் எழுத்துக்கள் (0x0E - 0x1F)
    else if (tz_code >= TZ_IK && tz_code <= TZ_INN) {
        // வேர் எழுத்து + புள்ளி (எ.கா: க + ் = க்)
        printf("%s%s", mei_base_utf8[tz_code - TZ_IK], virama);
    } 
    // 4. உயிர்மெய் எழுத்துக்கள் (0x20 முதல்)
    else if (tz_code >= 0x20 && tz_code <= 0xF7) {
        // Reverse Engineering the Macro! (O(1) Math)
        int offset = tz_code - 0x20;
        int mei_idx = offset / 12; // எந்த மெய் எழுத்து வரிசை?
        int uyir_idx = offset % 12; // எந்த உயிர் குறியீடு?
        
        if (mei_idx < 18 && uyir_idx < 12) {
            // வேர் எழுத்து + குறியீடு (எ.கா: க + ா = கா)
            printf("%s%s", mei_base_utf8[mei_idx], uyirmei_suffix[uyir_idx]);
        }
    } 
    // 5. ஆங்கில எழுத்துக்கள் மற்றும் எண்கள் (Space, Enter etc.)
    else {
        putchar(tz_code);
    }
}
