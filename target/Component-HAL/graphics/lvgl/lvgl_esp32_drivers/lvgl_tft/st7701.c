/**
 * @file st7701.c
 *
 * Basic ST7701 driver for LVGL on ESP32 (minimal init + flush).
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "esp_log.h"

#include "st7701.h"

#include "disp_spi.h"
#include "driver/gpio.h"
#include "esp_idf_version.h"
#if ESP_IDF_VERSION <= ESP_IDF_VERSION_VAL(5,0,0)
#include "rom/gpio.h"
#endif

static const char *TAG = "st7701";

typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; // number of data; 0xFF = end
} lcd_init_cmd_t;

static void st7701_set_orientation(uint8_t orientation);
static void st7701_send_color(void *data, size_t length);

void st7701_init(void)
{
    lcd_init_cmd_t init_cmds[] = {
        {ST7701_SWRESET, {0}, 0x80},
        {ST7701_SLPOUT, {0}, 0x80},
        {ST7701_COLMOD, {0x55}, 1}, /* 16-bit/pixel */
#if ST7701_INVERT_COLORS == 1
        {ST7701_INVON, {0}, 0},
#else
        {ST7701_INVOFF, {0}, 0},
#endif
        {ST7701_MADCTL, {0x00}, 1},
        {ST7701_CASET, {0x00, 0x00, 0x00, 0xEF}, 4},
        {ST7701_RASET, {0x00, 0x00, 0x01, 0x3F}, 4},
        {ST7701_RAMWR, {0}, 0},
        {ST7701_DISPON, {0}, 0x80},
        {0, {0}, 0xff},
    };

    /* Initialize non-SPI GPIOs */
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,0,0)
    gpio_pad_select_gpio(ST7701_DC);
#else
    esp_rom_gpio_pad_select_gpio(ST7701_DC);
#endif
    gpio_set_direction(ST7701_DC, GPIO_MODE_OUTPUT);

#if !defined(ST7701_SOFT_RST)
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,0,0)
    gpio_pad_select_gpio(ST7701_RST);
#else
    esp_rom_gpio_pad_select_gpio(ST7701_RST);
#endif
    gpio_set_direction(ST7701_RST, GPIO_MODE_OUTPUT);
#endif

    /* Reset */
#if !defined(ST7701_SOFT_RST)
    gpio_set_level(ST7701_RST, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(ST7701_RST, 1);
    vTaskDelay(100 / portTICK_PERIOD_MS);
#else
    st7701_send_cmd(ST7701_SWRESET);
#endif

    /* Send initialization commands */
    uint16_t idx = 0;
    while (init_cmds[idx].databytes != 0xff) {
        st7701_send_cmd(init_cmds[idx].cmd);
        st7701_send_data(init_cmds[idx].data, init_cmds[idx].databytes & 0x1F);
        if (init_cmds[idx].databytes & 0x80) {
            vTaskDelay(120 / portTICK_PERIOD_MS);
        }
        idx++;
    }

    st7701_set_orientation(CONFIG_LV_DISPLAY_ORIENTATION);
}

void st7701_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
    uint8_t data[4];

    uint16_t offsetx1 = area->x1;
    uint16_t offsetx2 = area->x2;
    uint16_t offsety1 = area->y1;
    uint16_t offsety2 = area->y2;

#if (CONFIG_LV_TFT_DISPLAY_OFFSETS)
    offsetx1 += CONFIG_LV_TFT_DISPLAY_X_OFFSET;
    offsetx2 += CONFIG_LV_TFT_DISPLAY_X_OFFSET;
    offsety1 += CONFIG_LV_TFT_DISPLAY_Y_OFFSET;
    offsety2 += CONFIG_LV_TFT_DISPLAY_Y_OFFSET;
#endif

    /* Column address */
    st7701_send_cmd(ST7701_CASET);
    data[0] = (offsetx1 >> 8) & 0xFF;
    data[1] = offsetx1 & 0xFF;
    data[2] = (offsetx2 >> 8) & 0xFF;
    data[3] = offsetx2 & 0xFF;
    st7701_send_data(data, 4);

    /* Page address */
    st7701_send_cmd(ST7701_RASET);
    data[0] = (offsety1 >> 8) & 0xFF;
    data[1] = offsety1 & 0xFF;
    data[2] = (offsety2 >> 8) & 0xFF;
    data[3] = offsety2 & 0xFF;
    st7701_send_data(data, 4);

    /* Memory write */
    st7701_send_cmd(ST7701_RAMWR);

    size_t size = (size_t)lv_area_get_width(area) * (size_t)lv_area_get_height(area);

    st7701_send_color((void*)color_map, size * 2);
}

void st7701_send_cmd(uint8_t cmd)
{
    disp_wait_for_pending_transactions();
    gpio_set_level(ST7701_DC, 0);
    disp_spi_send_data(&cmd, 1);
}

void st7701_send_data(void * data, uint16_t length)
{
    disp_wait_for_pending_transactions();
    gpio_set_level(ST7701_DC, 1);
    disp_spi_send_data(data, length);
}

static void st7701_send_color(void * data, size_t length)
{
    disp_wait_for_pending_transactions();
    gpio_set_level(ST7701_DC, 1);
    disp_spi_send_colors(data, length);
}

static void st7701_set_orientation(uint8_t orientation)
{
    const char *orientation_str[] = {
        "PORTRAIT", "PORTRAIT_INVERTED", "LANDSCAPE", "LANDSCAPE_INVERTED"
    };

    ESP_LOGI(TAG, "Display orientation: %s", orientation_str[orientation]);

    uint8_t data[] = { 0x00, 0x00, 0x60, 0xA0 };

    st7701_send_cmd(ST7701_MADCTL);
    st7701_send_data((void *)&data[orientation], 1);
}
