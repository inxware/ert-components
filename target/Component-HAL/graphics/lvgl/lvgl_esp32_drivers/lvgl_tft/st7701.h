/**
 * @file st7701.h
 */

#ifndef ST7701_H
#define ST7701_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif
#include "../lvgl_helpers.h"

#include "sdkconfig.h"

#define ST7701_DC       CONFIG_LV_DISP_PIN_DC
#define ST7701_RST      CONFIG_LV_DISP_PIN_RST

#if CONFIG_LV_DISP_USE_RST
  #if CONFIG_LV_DISP_ST7701_SOFT_RESET
    #define ST7701_SOFT_RST
  #endif
#else
  #define ST7701_SOFT_RST
#endif

#define ST7701_INVERT_COLORS            CONFIG_LV_INVERT_COLORS

/* Common commands used by many controllers (ST7701 compatible set) */
#define ST7701_NOP      0x00
#define ST7701_SWRESET  0x01
#define ST7701_SLPIN    0x10
#define ST7701_SLPOUT   0x11
#define ST7701_INVOFF   0x20
#define ST7701_INVON    0x21
#define ST7701_DISPOFF  0x28
#define ST7701_DISPON   0x29
#define ST7701_CASET    0x2A
#define ST7701_RASET    0x2B
#define ST7701_RAMWR    0x2C
#define ST7701_MADCTL   0x36
#define ST7701_COLMOD   0x3A

void st7701_init(void);
void st7701_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);

void st7701_send_cmd(uint8_t cmd);
void st7701_send_data(void *data, uint16_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ST7701_H */
