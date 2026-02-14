/**
 * @file cst816t.h
 * 
 * CST816T Capacitive Touch Controller Driver for LVGL
 * Support for I2C interface
 * 
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef __CST816T_H
#define __CST816T_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/

#define CST816T_I2C_SLAVE_ADDR     0x15

/* CST816T Register Addresses */
#define CST816T_STATUS_REG          0x00    /* Touch status register */
#define CST816T_POINT1_XH           0x03    /* Touch point 1 X coordinate high byte */
#define CST816T_POINT1_XL           0x04    /* Touch point 1 X coordinate low byte */
#define CST816T_POINT1_YH           0x05    /* Touch point 1 Y coordinate high byte */
#define CST816T_POINT1_YL           0x06    /* Touch point 1 Y coordinate low byte */
#define CST816T_POINT2_XH           0x09    /* Touch point 2 X coordinate high byte */
#define CST816T_POINT2_XL           0x0A    /* Touch point 2 X coordinate low byte */
#define CST816T_POINT2_YH           0x0B    /* Touch point 2 Y coordinate high byte */
#define CST816T_POINT2_YL           0x0C    /* Touch point 2 Y coordinate low byte */

#define CST816T_CHIP_ID_REG         0xA7    /* Chip ID register */
#define CST816T_FW_VERSION_REG      0xA9    /* Firmware version register */
#define CST816T_SLEEP_REG           0xE5    /* Sleep control register */

/* CST816T Status Register Bits */
#define CST816T_STATUS_NUM_TOUCH_MASK   0x0F    /* Number of touch points mask */
#define CST816T_STATUS_BUF_STAT         0x80    /* Buffer status */

/* Maximum display dimensions */
#define CST816T_MAX_WIDTH           240
#define CST816T_MAX_HEIGHT          280

/* Maximum number of simultaneous touch points */
#define CST816T_MAX_TOUCH_PNTS      2

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    bool inited;
    uint16_t max_x;
    uint16_t max_y;
    uint8_t i2c_dev_addr;
    int intr_pin;
} cst816t_status_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Initialize CST816T touch controller
 * @param dev_addr I2C slave address of CST816T
 */
void cst816t_init(uint8_t dev_addr);

/**
 * @brief Read touch data from CST816T
 * @param drv LVGL input device driver
 * @param data LVGL input device data
 * @return True if touch data is available, false otherwise
 */
#if LVGL_VERSION_MAJOR >= 8
void cst816t_read(lv_indev_drv_t *drv, lv_indev_data_t *data);
#else
bool cst816t_read(lv_indev_drv_t *drv, lv_indev_data_t *data);
#endif

/**
 * @brief Enter sleep mode
 */
void cst816t_sleep(void);

/**
 * @brief Wake up from sleep mode
 */
void cst816t_wake(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CST816T_H */
