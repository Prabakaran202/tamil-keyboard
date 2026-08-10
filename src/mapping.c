#include "mapping.h"

// ஆரம்ப நிலை: -1 என்றால் எந்த மெய் எழுத்தும் Pending-ல் இல்லை என்று அர்த்தம்
int current_mei_state = -1; 

// 1. படத்திலுள்ளது போல உயிர் எழுத்துக்கான Index (QWERTY இடது புறம்)
int get_uyir_index(char key) {
    switch(key) {
        case 'q': return U_A;  case 'w': return U_AA;
        case 'e': return U_I;  case 'r': return U_II;
        case 't': return U_U;  case 'y': return U_UU;
        case 'a': return U_E;  case 's': return U_EE;
        case 'd': return U_AI; case 'f': return U_O;
        case 'g': return U_OO; case 'z': return U_AU;
        default: return -1;
    }
}

// 2. படத்திலுள்ளது போல மெய் எழுத்துக்கான Index (QWERTY வலது புறம் மற்றும் கீழ் வரிசை)
int get_mei_index(char key) {
    switch(key) {
        case '[': return M_K;   case 'p': return M_NG;
        case 'o': return M_CH;  case 'i': return M_NJ;
        case 'u': return M_T;   case '\'': return M_N;  // ' = ண்
        case ';': return M_TH;  case 'l': return M_ND;
        case 'k': return M_P;   case 'j': return M_M;
        case 'h': return M_Y;   case '.': return M_R;
        case ',': return M_L;   case 'm': return M_V;
        case 'n': return M_ZH;  case 'b': return M_LL;
        case 'v': return M_TR;  case 'c': return M_NN;
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
            return combined; 
        } 
        else if (mei_idx != -1) {
            // அடுத்ததும் ஒரு மெய் எழுத்தாகவே இருந்தால்
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
            // யூசர் மெய் எழுத்தை அழுத்துகிறார்
            current_mei_state = mei_idx;
            return (unsigned char)(0x0E + mei_idx); 
        } 
        else if (uyir_idx != -1) {
            // யூசர் நேரடியாக உயிர் எழுத்தை அழுத்துகிறார்
            return (unsigned char)(0x02 + uyir_idx);
        } 
        else if (key == 'x') {
            // படத்திலுள்ளது போல 'x' அழுத்தினால் ஆயுத எழுத்து (ஃ)
            return TZ_AK;
        }
    }
    
    // எந்த மேப்பிங்கும் இல்லை என்றால் (உ.ம்: Space, Enter, Numbers)
    return key; 
}
