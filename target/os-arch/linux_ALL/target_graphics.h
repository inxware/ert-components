#ifndef _EHS_TARGET_GRAPHICS_H_
#define _EHS_TARGET_GRAPHICS_H_

#ifdef EHS_GUI_SUPPORT_MODE_B_LVGL

#include "lvgl/lvgl.h"
#include "lv_drivers/sdl/sdl.h"
#include "SDL2/SDL.h"

#include "globals.h"

void sdl_init(void);
void sdl_display_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
void sdl_mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
void sdl_mousewheel_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
void sdl_keyboard_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

#define TARGET_LVGL_USE_COMMON_DRIVER 1

#define USE_SDL 1

#define TARGET_LVGL_HORIZONTAL  EHS_CONFIG_DISPLAY_WIDTH
#define TARGET_LVGL_VERTICAL    EHS_CONFIG_DISPLAY_HEIGHT

#define TARGET_DISPLAY_BUFFER_SIZE  (TARGET_LVGL_HORIZONTAL * 100)

#define TARGET_LVGL_DRIVER_INIT sdl_init()

#define TARGET_MEMORY_DYNAMIC   0
#if TARGET_MEMORY_DYNAMIC == 1
#define TARGET_LVGL_MALLOC(size) 
#endif

#define TARGET_LVGL_DISPLAY_FLUSH_FUNCTION		sdl_display_flush

#define TARGET_LVGL_POINTER 1
#if TARGET_LVGL_POINTER == 1
#define TARGET_LVGL_POINTER_CALLBACK_FUNCTION		sdl_mouse_read
#endif

#define TARGET_LVGL_KEYBOARD 1
#if TARGET_LVGL_KEYBOARD == 1
#define TARGET_LVGL_KEYBOARD_CALLBACK_FUNCTION		sdl_keyboard_read
#endif

#define TARGET_LVGL_ENCODER 1
#if TARGET_LVGL_ENCODER == 1
#define TARGET_LVGL_ENCODER_CALLBACK_FUNCTION		sdl_mousewheel_read
#endif

#endif

#endif
