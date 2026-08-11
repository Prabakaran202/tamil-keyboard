#ifndef SHAPER_H
#define SHAPER_H

#include <stdint.h>

// தமிழ் ஷேப்பிங் ஸ்டேட்ஸ் மற்றும் பஃபர் மேனேஜ்மென்ட்
void shaper_reset_buffer(void);
const char* shaper_process_key(uint32_t unicode_char);

// 1-Byte to 3-Byte Unicode Translator (இதைத்தான் புதிதாகச் சேர்த்துள்ளோம் 🚀)
int translate_tamizhi_to_unicode(unsigned char tz_char, uint32_t* out_codes);

#endif // SHAPER_H
