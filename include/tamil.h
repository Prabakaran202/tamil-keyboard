#ifndef TAMIZHI_TAMIL_H
#define TAMIZHI_TAMIL_H

/* =========================================================
 * 🚀 TAMIZHI 1-BYTE NATIVE ENCODING (0x01 - 0xF7)
 * ========================================================= */

// 1. ஆயுத எழுத்து (1)
#define TZ_AK 0x01

// 2. உயிர் எழுத்துக்கள் (12)
#define TZ_A   0x02
#define TZ_AA  0x03
#define TZ_I   0x04
#define TZ_II  0x05
#define TZ_U   0x06
#define TZ_UU  0x07
#define TZ_E   0x08
#define TZ_EE  0x09
#define TZ_AI  0x0A
#define TZ_O   0x0B
#define TZ_OO  0x0C
#define TZ_AU  0x0D

// 3. மெய் எழுத்துக்கள் (18) - The Base Consonants
#define TZ_IK  0x0E // க்
#define TZ_ING 0x0F // ங்
#define TZ_ICH 0x10 // ச்
#define TZ_INJ 0x11 // ஞ்
#define TZ_IT  0x12 // ட்
#define TZ_IN  0x13 // ண்
#define TZ_ITH 0x14 // த்
#define TZ_IND 0x15 // ந்
#define TZ_IP  0x16 // ப்
#define TZ_IM  0x17 // ம்
#define TZ_IY  0x18 // ய்
#define TZ_IR  0x19 // ர்
#define TZ_IL  0x1A // ல்
#define TZ_IV  0x1B // வ்
#define TZ_IZH 0x1C // ழ்
#define TZ_ILL 0x1D // ள்
#define TZ_ITR 0x1E // ற்
#define TZ_INN 0x1F // ன்

/* =========================================================
 * 🧠 1-BYTE UYIRMEI CALCULATOR (O(1) Time Complexity)
 * ========================================================= 
 * உயிர்மெய் எழுத்துக்கள் 0x20 முதல் தொடங்கும். 
 * சூத்திரம்: 0x20 + (மெய் வரிசை * 12) + உயிர் வரிசை
 */

// மெய் எழுத்துக்களின் இண்டெக்ஸ் (0 முதல் 17 வரை)
typedef enum {
    M_K=0, M_NG, M_CH, M_NJ, M_T, M_N, M_TH, M_ND, M_P, M_M, 
    M_Y, M_R, M_L, M_V, M_ZH, M_LL, M_TR, M_NN
} MeiIndex;

// உயிர் எழுத்துக்களின் இண்டெக்ஸ் (0 முதல் 11 வரை)
typedef enum {
    U_A=0, U_AA, U_I, U_II, U_U, U_UU, U_E, U_EE, U_AI, U_O, U_OO, U_AU
} UyirIndex;

// 🚀 The Magic Macro!
#define TZ_UYIRMEI(mei_idx, uyir_idx) (unsigned char)(0x20 + ((mei_idx) * 12) + (uyir_idx))

#endif // TAMIZHI_TAMIL_H
