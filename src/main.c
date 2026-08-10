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
    printf("Press 'Ctrl+C' or type 'EXIT' to quit.\n\n> ");
    
    enable_raw_mode();
    
    while (1) {
        char key = get_keypress();
        
        // Ctrl+C அல்லது 'Q' (Exit condition for testing) அழுத்தினால் வெளியேற
        if (key == 3) { 
            break;
        }

        // State மாறுவதற்கு முன், பழைய State என்ன என்று சேமித்து வைக்கிறோம்
        int previous_state = current_mei_state;
        
        // State Machine-க்கு பட்டனை அனுப்பி 1-byte தமிழ் கோடை வாங்குகிறோம்
        unsigned char tz_code = process_key(key);
        
        // --- The Backspace Magic ---
        // ஏற்கனவே ஒரு மெய் எழுத்து (உ.ம்: 'க்') திரையில் இருந்து, 
        // இப்போது வந்த கோடு உயிர்மெய் (0x20+) ஆக இருந்தால், 
        // திரையில் உள்ள பழைய 'க்'-ஐ அழிக்க வேண்டும்.
        if (previous_state != -1 && tz_code >= 0x20) {
            // லினக்ஸ் டெர்மினலில் முந்தைய UTF-8 எழுத்தை அழிக்க Backspace அனுப்புகிறோம்
            printf("\b\b  \b\b"); // Terminal-ஐ பொறுத்து இதை அட்ஜஸ்ட் செய்ய வேண்டி வரலாம்
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
