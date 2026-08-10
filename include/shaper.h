#ifndef SHAPER_H
#define SHAPER_H

#include <stdint.h>

// தமிழ் ஷேப்பிங் ஸ்டேட்ஸ் மற்றும் பஃபர் மேனேஜ்மென்ட்
void shaper_reset_buffer(void);
const char* shaper_process_key(uint32_t unicode_char);

#endif // SHAPER_H
