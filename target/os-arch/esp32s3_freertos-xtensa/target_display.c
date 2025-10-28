
#include "globals.h"
#include "target_display.h"


#ifdef CONFIG_LV_TFT_DISPLAY_CONTROLLER_FT81X
void FT81x_backlight(int intensity);
#endif//CONFIG_LV_TFT_DISPLAY_CONTROLLER_FT81X

void target_setBacklight(ehs_sint32 intensity)
{
#ifdef CONFIG_LV_TFT_DISPLAY_CONTROLLER_FT81X
    FT81x_backlight(((intensity > 100 ? 100 : intensity < 0 ? 0 : intensity) << 7) / 100);
#endif//CONFIG_LV_TFT_DISPLAY_CONTROLLER_FT81X
}