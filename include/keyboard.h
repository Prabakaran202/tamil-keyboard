#ifndef TAMIZHI_KEYBOARD_H
#define TAMIZHI_KEYBOARD_H

// Raw Mode-ஐ ஆன்/ஆஃப் செய்யும் ஃபங்ஷன்கள்
void enable_raw_mode();
void disable_raw_mode();

// யூசர் அழுத்தும் ஒரு எழுத்தைப் படிக்கும் ஃபங்ஷன்
char get_keypress();

#endif // TAMIZHI_KEYBOARD_H
