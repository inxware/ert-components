#include "target_display.h"

//TODO2024 set brightness in sysfs
// The `intel_backlight` can be replaced with `*`
// echo 58800 | sudo tee /sys/class/backlight/intel_backlight/brightness    # To set the brightness
// cat /sys/class/backlight/intel_backlight/max_brightness                  # To get the max brightness
void target_setBacklight(ehs_sint32 intensity)
{
    ;
}