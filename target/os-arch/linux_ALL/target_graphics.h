#ifndef _EHS_TARGET_GRAPHICS_H_
#define _EHS_TARGET_GRAPHICS_H_

#ifdef EHS_GUI_SUPPORT_MODE_B_LVGL

#include "lvgl/lvgl.h"
#include "globals.h"

#ifdef EHS_LVGL_LINUX_DISPLAY_BACKEND_DRM
/*
 * Native DRM/KMS display backend.
 * Uses /dev/dri/cardX directly via libdrm — no SDL2, no compositor required.
 * Select this backend for embedded boards where SDL2 is Wayland-only or absent.
 * Set EHS_LVGL_LINUX_DISPLAY_BACKEND=drm in the platform config.mk.
 */
/* Must be defined before drm.h — all declarations inside are guarded by #if USE_DRM */
#define USE_DRM 1
#include "lv_drivers/display/drm.h"

#define TARGET_LVGL_USE_COMMON_DRIVER 1

#define TARGET_LVGL_HORIZONTAL  EHS_CONFIG_DISPLAY_WIDTH
#define TARGET_LVGL_VERTICAL    EHS_CONFIG_DISPLAY_HEIGHT

#define TARGET_DISPLAY_BUFFER_SIZE  (TARGET_LVGL_HORIZONTAL * 100)

#define TARGET_LVGL_DRIVER_INIT drm_init()

#define TARGET_MEMORY_DYNAMIC   0

#define TARGET_LVGL_DISPLAY_FLUSH_FUNCTION  drm_flush

/* Input: no pointer/keyboard by default for DRM.
 * Add evdev driver support and define these if touch/keyboard is needed. */
#define TARGET_LVGL_POINTER  0
#define TARGET_LVGL_KEYBOARD 0
#define TARGET_LVGL_ENCODER  0

#else /* SDL2 backend (default) */
/*
 * SDL2 display backend.
 * Requires SDL2 compiled with kmsdrm, Wayland, or X11 support.
 * Works out-of-the-box on desktop Linux.  For bare embedded boards
 * whose SDL2 is Wayland-only, use the DRM backend instead.
 */
#include "lv_drivers/sdl/sdl.h"
#include "SDL2/SDL.h"

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

#define TARGET_LVGL_DISPLAY_FLUSH_FUNCTION      sdl_display_flush

#define TARGET_LVGL_POINTER 1
#if TARGET_LVGL_POINTER == 1
#define TARGET_LVGL_POINTER_CALLBACK_FUNCTION   sdl_mouse_read
#endif

#define TARGET_LVGL_KEYBOARD 1
#if TARGET_LVGL_KEYBOARD == 1
#define TARGET_LVGL_KEYBOARD_CALLBACK_FUNCTION  sdl_keyboard_read
#endif

#define TARGET_LVGL_ENCODER 1
#if TARGET_LVGL_ENCODER == 1
#define TARGET_LVGL_ENCODER_CALLBACK_FUNCTION   sdl_mousewheel_read
#endif

#endif /* EHS_LVGL_LINUX_DISPLAY_BACKEND_DRM */

#endif /* EHS_GUI_SUPPORT_MODE_B_LVGL */

#endif /* _EHS_TARGET_GRAPHICS_H_ */
