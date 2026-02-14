/** @file target_config.h
 * Base Target config to be included by target specific configs
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 * @author: inx limited
 *
 * Copyright (c) inx Ltd, 2020. All rights reserved.
 */

#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

/* Overrides go here */
#define EHS_CONFIG_DISPLAY_HEIGHT 240 /**< Height for the OSD part of the display */
#define EHS_CONFIG_DISPLAY_WIDTH  135 /**< Width for the OSD part of the display */
#define EHS_TARGET_DEFAULT_ID "(xtensa/esp32)"

#include "../base_small/base_config.h"

/* LVGL heap size */
#define LV_MEM_SIZE   (32U * 1024U)    /*[bytes]*/

#define PLATFORM_CONFIG_INX_LV_FONT_MONTSERRAT_8                1

/* LVGL target specific config */
#define CONFIG_LV_DPI_DEF 186
#define CONFIG_LV_TFT_DISPLAY_PROTOCOL_SPI 1
#define CONFIG_LV_DISPLAY_ORIENTATION_LANDSCAPE 1
#define CONFIG_LV_DISPLAY_ORIENTATION 0
#define CONFIG_LV_TFT_DISPLAY_SPI3_HOST 1
#define CONFIG_LV_TFT_DISPLAY_SPI_TRANS_MODE_SIO 1
#define CONFIG_LV_TFT_DISPLAY_SPI_HALF_DUPLEX 1
#define CONFIG_LV_TFT_USE_CUSTOM_SPI_CLK_DIVIDER 1
#define CONFIG_LV_TFT_SPI_CLK_DIVIDER_4 1
#define CONFIG_LV_TFT_CUSTOM_SPI_CLK_DIVIDER 4
#define CONFIG_LV_DISP_SPI_MOSI 19
//#define CONFIG_LV_DISPLAY_USE_SPI_MISO 1
//#define CONFIG_LV_DISP_SPI_MISO 10
#define CONFIG_LV_DISP_SPI_INPUT_DELAY_NS 0
#define CONFIG_LV_DISP_SPI_CLK 18
#define CONFIG_LV_DISPLAY_USE_SPI_CS 1
#define CONFIG_LV_DISP_SPI_CS 5
//#define CONFIG_LV_DISP_USE_RST 1
//#define CONFIG_LV_DISP_PIN_RST 14
#define CONFIG_LV_DISP_BACKLIGHT_SWITCH 1
#define CONFIG_LV_DISP_PIN_BCKL -1
//#define CONFIG_LV_I2C_DISPLAY_PORT 0
//#define CONFIG_LV_TOUCH_CONTROLLER 5
//#define CONFIG_LV_TOUCH_CONTROLLER_FT81X 1
//#define CONFIG_LV_TOUCH_DRIVER_PROTOCOL_SPI 1
//#define CONFIG_LV_TOUCH_CONTROLLER_SPI3_HOST 1
//#define CONFIG_LV_I2C_TOUCH_PORT 0



#include "../esp32_freertos-xtensa-base/target_config.h"

#endif /* TARGET_CONFIG_H */
