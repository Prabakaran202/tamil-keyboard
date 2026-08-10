#include <stdio.h>
#include <stdlib.h>
#include "keyboard.h"
#include "mapping.h"
#include "unicode.h"

int main() {
    printf("\033[2J\033[H"); // திரையை க்ளீன் செய்ய
    printf("\033[1;36m=============================================\n");
    printf(" 🚀 TAMIZHI 1-BYTE KEYBOARD TESTER \n");
    printf("=============================================\033[0m\n");
    printf("Type in English (QWERTY) to see Tamizhi output.\n");
    printf("Press 'Backspace' to delete, 'Ctrl+C' to quit.\n\n> ");
    
    enable_raw_mode();
    
    while (1) {
        char key = get_keypress();
        
        // 1. Ctrl+C அழுத்தினால் வெளியேற (ASCII 3)
        if (key == 3) { 
            break;
        }

        // 2. 🔥 BACKSPACE / DELETE பட்டன் லாஜிக் (ASCII 127 or 8)
        if (key == 127 || key == 8) {
            // State-ஐ Reset செய்கிறோம் (Pending-ல் உள்ள மெய் எழுத்தை மறக்கச் செய்கிறோம்)
            current_mei_state = -1; 
            
            // டெர்மினலில் முந்தைய எழுத்தை அழிக்கும் மேஜிக் கமாண்ட் (\b = Backspace)
            printf("\b\b  \b\b"); 
            fflush(stdout);
            continue; // இதை அழித்துவிட்டு அடுத்த கீ-க்காக காத்திருக்க லூப்பைத் தொடர்கிறோம்
        }

        // State மாறுவதற்கு முன், பழைய State என்ன என்று சேமித்து வைக்கிறோம்
        int previous_state = current_mei_state;
        
        // State Machine-க்கு பட்டனை அனுப்பி 1-byte தமிழ் கோடை வாங்குகிறோம்
        unsigned char tz_code = process_key(key);
        
        // --- The Internal Backspace Magic ---
        // ஏற்கனவே ஒரு மெய் எழுத்து (உ.ம்: 'க்') திரையில் இருந்து, 
        // இப்போது வந்த கோடு உயிர்மெய் (0x20+) ஆக இருந்தால், 
        // திரையில் உள்ள பழைய 'க்'-ஐ அழிக்க வேண்டும்.
        if (previous_state != -1 && tz_code >= 0x20) {
            printf("\b\b  \b\b"); 
        }
        
        // 1-byte கோடை 3-byte UTF-8 ஆக மாற்றி ஸ்கிரீனில் பிரிண்ட் செய்கிறோம்
        print_tz_char(tz_code);
        
        // உடனுக்குடன் ஸ்கிரீனில் தெரியவைக்க Buffer-ஐ Flush செய்கிறோம்
        fflush(stdout);
    }
    
    disable_raw_mode();
    printf("\n\n\033[1;32mExiting Tamizhi Tester... Nandri!\033[0m\n");
    return 0;
}
