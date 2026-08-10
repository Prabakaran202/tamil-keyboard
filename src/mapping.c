#include "mapping.h"

// ஆரம்ப நிலை: -1 என்றால் எந்த மெய் எழுத்தும் Pending-ல் இல்லை என்று அர்த்தம்
int current_mei_state = -1; 

// 1. உயிர் எழுத்துக்கான Index-ஐ கண்டுபிடிக்கும் ஃபங்ஷன்
int get_uyir_index(char key) {
    switch(key) {
        case 'q': return U_A;  case 'w': return U_AA;
        case 'e': return U_I;  case 'r': return U_II;
        case 't': return U_U;  case 'y': return U_UU;
        case 'u': return U_E;  case 'i': return U_EE;
        case 'o': return U_AI; case 'p': return U_O;
        case 'a': return U_OO; case 's': return U_AU;
        default: return -1;
    }
}

// 2. மெய் எழுத்துக்கான Index-ஐ கண்டுபிடிக்கும் ஃபங்ஷன்
// (உதாரணத்திற்கு சில ஆங்கில எழுத்துக்களை மேப் செய்துள்ளோம். இதை உங்கள் வசதிக்கேற்ப மாற்றிக்கொள்ளலாம்)
int get_mei_index(char key) {
    switch(key) {
        case 'k': return M_K;   case 'g': return M_NG;
        case 'c': return M_CH;  case 'j': return M_NJ;
        case 't': return M_T;   case 'N': return M_N;
        case 'h': return M_TH;  case 'n': return M_ND;
        case 'p': return M_P;   case 'm': return M_M;
        case 'y': return M_Y;   case 'r': return M_R;
        case 'l': return M_L;   case 'v': return M_V;
        case 'z': return M_ZH;  case 'L': return M_LL;
        case 'R': return M_TR;  case 'x': return M_NN;
        default: return -1;
    }
}

// 3. The State Machine - Key Processor
unsigned char process_key(char key) {
    int uyir_idx = get_uyir_index(key);
    int mei_idx = get_mei_index(key);

    // நிலை 1: ஏற்கனவே ஒரு மெய் எழுத்து (உ.ம்: 'க்') Pending-ல் இருந்தால்...
    if (current_mei_state != -1) {
        if (uyir_idx != -1) {
            // மேஜிக்: மெய் + உயிர் = உயிர்மெய் (க் + அ = க)
            unsigned char combined = TZ_UYIRMEI(current_mei_state, uyir_idx);
            current_mei_state = -1; // State-ஐ Reset செய்கிறோம்
            
            // குறிப்பு: Editor-ல் இந்த Code-ஐ அனுப்பும்முன், ஒரு Backspace-ஐ அனுப்பி 
            // பழைய 'க்'-ஐ அழித்துவிட்டு இதை பிரிண்ட் செய்ய வேண்டும்.
            return combined; 
        } 
        else if (mei_idx != -1) {
            // அடுத்ததும் ஒரு மெய் எழுத்தாகவே இருந்தால் (உ.ம்: 'க்' அழுத்திவிட்டு 'ப்' அழுத்துவது)
            current_mei_state = mei_idx;
            return (unsigned char)(0x0E + mei_idx); // புதிய மெய் எழுத்தை ரிட்டர்ன் செய்கிறோம்
        } 
        else {
            // Space அல்லது வேறு ஏதேனும் பட்டன் அழுத்தினால்
            current_mei_state = -1;
            return key; // ASCII-ஐ அப்படியே ரிட்டர்ன் செய்கிறோம்
        }
    } 
    // நிலை 2: Idle (எந்த மெய் எழுத்தும் Pending-ல் இல்லை)
    else {
        if (mei_idx != -1) {
            // யூசர் மெய் எழுத்தை ('k') அழுத்துகிறார்
            current_mei_state = mei_idx;
            return (unsigned char)(0x0E + mei_idx); // 0x0E என்பது 'க்'. இதனுடன் Index-ஐ கூட்டுகிறோம். (O(1) Math!)
        } 
        else if (uyir_idx != -1) {
            // யூசர் நேரடியாக உயிர் எழுத்தை ('q' -> 'அ') அழுத்துகிறார்
            return (unsigned char)(0x02 + uyir_idx);
        } 
        else if (key == 'd') {
            // ஆயுத எழுத்து
            return TZ_AK;
        }
    }
    
    // எந்த மேப்பிங்கும் இல்லை என்றால் (உ.ம்: Space, Enter, Numbers)
    return key; 
}
