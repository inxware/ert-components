#include "target_display.h"
#include "target_types.h"

void FT81x_backlight(int intensity);

void target_setBacklight(ehs_sint32 intensity)
{
#ifdef EHS_GUI_SUPPORT_MODE_B_LVGL
    FT81x_backlight(((intensity > 100 ? 100 : intensity < 0 ? 0 : intensity) << 7) / 100);
#endif//EHS_GUI_SUPPORT_MODE_B_LVGL
}