#ifndef MAPPING_H
#define MAPPING_H

#include <linux/input.h>

// Function to get the Tamil Unicode string for a given Linux Keycode
const char* get_tamizhi_char(int keycode);

#endif // MAPPING_H
