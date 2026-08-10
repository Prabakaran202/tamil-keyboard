#ifndef TAMIZHI_MAPPING_H
#define TAMIZHI_MAPPING_H

#include "tamil.h"

// தற்போதைய State-ஐ சேமிக்க (எ.கா: கடைசியாக 'க்' அழுத்தப்பட்டிருந்தால் அது Pending-ல் இருக்கும்)
extern int current_mei_state; 

// யூசர் அழுத்தும் கீபோர்டு பட்டனை வாங்கி, 1-byte தமிழ் கோடாக மாற்றும் ஃபங்ஷன்
unsigned char process_key(char key);

#endif // TAMIZHI_MAPPING_H
