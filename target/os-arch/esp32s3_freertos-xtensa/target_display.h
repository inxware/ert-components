#ifndef _TARGET_DISPLAY_H_ 
#define _TARGET_DISPLAY_H_ 

#include "target_types.h"

#ifdef EHS_GUI_SUPPORT_MODE_B_LVGL
void FT81x_backlight(int intensity);
#endif//EHS_GUI_SUPPORT_MODE_B_LVGL

void target_setBacklight(ehs_sint32 intensity);

#endif//_TARGET_DISPLAY_H_ 