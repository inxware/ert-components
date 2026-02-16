/**
 * @file cst816t.c
 * 
 * CST816T Capacitive Touch Controller Driver Implementation
 * 
 * This driver supports the CST816T capacitive touch chip commonly found
 * on small ESP32 displays like the Waveshare 1.69" LCD.
 * 
 * Copyright (c) 2024. All rights reserved.
 */

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include <lvgl.h>
#else
#include <lvgl/lvgl.h>
#endif

#include "cst816t.h"
#include "lvgl_i2c/i2c_manager.h"

/*********************
 *      DEFINES
 *********************/

#define TAG "CST816T"

/* I2C port to use (defined in config) */
#ifndef CONFIG_LV_I2C_TOUCH_PORT
#define CONFIG_LV_I2C_TOUCH_PORT I2C_NUM_0
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

static cst816t_status_t cst816t_status = {
    .inited = false,
    .max_x = CST816T_MAX_WIDTH,
    .max_y = CST816T_MAX_HEIGHT,
    .i2c_dev_addr = CST816T_I2C_SLAVE_ADDR,
    .intr_pin = -1
};

/**********************
 *  STATIC FUNCTIONS
 **********************/

/**
 * @brief Read data from CST816T via I2C
 */
static esp_err_t cst816t_i2c_read(uint8_t slave_addr, uint8_t register_addr, uint8_t *data_buf, uint8_t len)
{
    return lvgl_i2c_read(CONFIG_LV_I2C_TOUCH_PORT, slave_addr, register_addr, data_buf, len);
}

/**
 * @brief Write data to CST816T via I2C
 */
static esp_err_t cst816t_i2c_write(uint8_t slave_addr, uint8_t register_addr, uint8_t data)
{
    uint8_t buffer = data;
    return lvgl_i2c_write(CONFIG_LV_I2C_TOUCH_PORT, slave_addr, register_addr, &buffer, 1);
}

/**
 * @brief Read touch coordinates from CST816T
 */
static void cst816t_read_coordinates(uint8_t *x_h, uint8_t *x_l, uint8_t *y_h, uint8_t *y_l, uint8_t point_num)
{
    uint8_t addr_offset = 0;
    
    if (point_num == 0) {
        /* Point 1 */
        addr_offset = 0;
    } else if (point_num == 1) {
        /* Point 2 */
        addr_offset = 6;
    } else {
        return;
    }

    cst816t_i2c_read(cst816t_status.i2c_dev_addr, CST816T_POINT1_XH + addr_offset, x_h, 1);
    cst816t_i2c_read(cst816t_status.i2c_dev_addr, CST816T_POINT1_XL + addr_offset, x_l, 1);
    cst816t_i2c_read(cst816t_status.i2c_dev_addr, CST816T_POINT1_YH + addr_offset, y_h, 1);
    cst816t_i2c_read(cst816t_status.i2c_dev_addr, CST816T_POINT1_YL + addr_offset, y_l, 1);
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * @brief Initialize CST816T touch controller
 */
void cst816t_init(uint8_t dev_addr)
{
    if (!cst816t_status.inited) {
        cst816t_status.i2c_dev_addr = dev_addr;
        
        uint8_t chip_id = 0;
        uint8_t fw_version = 0;
        esp_err_t ret;

        ESP_LOGI(TAG, "Initializing CST816T Touch Controller");
        
        /* Read and verify chip ID */
        ret = cst816t_i2c_read(dev_addr, CST816T_CHIP_ID_REG, &chip_id, 1);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to read chip ID: %s", esp_err_to_name(ret));
            return;
        }

        ESP_LOGI(TAG, "CST816T Chip ID: 0x%02x", chip_id);
        
        /* Read firmware version */
        cst816t_i2c_read(dev_addr, CST816T_FW_VERSION_REG, &fw_version, 1);
        ESP_LOGI(TAG, "Firmware Version: 0x%02x", fw_version);
        
        /* Read max coordinates if available from device */
        ESP_LOGI(TAG, "Max X: %d, Max Y: %d", cst816t_status.max_x, cst816t_status.max_y);

        cst816t_status.inited = true;
        ESP_LOGI(TAG, "CST816T initialized successfully");
    }
}

/**
 * @brief Read touch data from CST816T
 */
#if LVGL_VERSION_MAJOR >= 8
void cst816t_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
#else
bool cst816t_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
#endif
{
    static int16_t last_x = 0;
    static int16_t last_y = 0;
    bool res = false;

    uint8_t status_reg = 0;
    uint8_t num_touch_points = 0;
    
    uint8_t x_h = 0, x_l = 0, y_h = 0, y_l = 0;
    uint16_t x_coord = 0, y_coord = 0;

    /* Read status register to check if touch is detected */
    if (cst816t_i2c_read(cst816t_status.i2c_dev_addr, CST816T_STATUS_REG, &status_reg, 1) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read status register");
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;
#if LVGL_VERSION_MAJOR >= 8
        data->continue_reading = false;
#else
        return false;
#endif
    }

    /* Extract number of touch points */
    num_touch_points = status_reg & CST816T_STATUS_NUM_TOUCH_MASK;

    if (num_touch_points > 0) {
        /* Read first touch point coordinates */
        cst816t_read_coordinates(&x_h, &x_l, &y_h, &y_l, 0);

        /* Combine high and low bytes
         * CST816T uses 12-bit coordinates, with 4 bits in high byte and 8 bits in low byte */
        x_coord = ((uint16_t)(x_h & 0x0F) << 8) | x_l;
        y_coord = ((uint16_t)(y_h & 0x0F) << 8) | y_l;

        last_x = x_coord;
        last_y = y_coord;
        data->state = LV_INDEV_STATE_PR;
        res = true;
        ESP_LOGI(TAG, "Touch detected at (%d, %d)", x_coord, y_coord);
    } else {
        data->state = LV_INDEV_STATE_REL;
        res = false;
    }

    /* Set the coordinates */
    data->point.x = last_x;
    data->point.y = last_y;

#if LVGL_VERSION_MAJOR >= 8
    data->continue_reading = res;
#else
    return res;
#endif
}

/**
 * @brief Enter sleep mode
 */
void cst816t_sleep(void)
{
    if (cst816t_status.inited) {
        ESP_LOGI(TAG, "Entering sleep mode");
        cst816t_i2c_write(cst816t_status.i2c_dev_addr, CST816T_SLEEP_REG, 0x03);
    }
}

/**
 * @brief Wake up from sleep mode
 */
void cst816t_wake(void)
{
    if (cst816t_status.inited) {
        ESP_LOGI(TAG, "Waking up from sleep mode");
        cst816t_i2c_write(cst816t_status.i2c_dev_addr, CST816T_SLEEP_REG, 0x00);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
