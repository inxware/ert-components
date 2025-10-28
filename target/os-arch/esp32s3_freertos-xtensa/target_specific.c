

#include <stdio.h>
#include "esp_err.h"
#include "esp_log.h"

#include "globals.h"

#ifdef EHS_RTC_SUPPORT
#include "target_hal_rtc.h"
#include <time.h>
#endif

#ifdef EHS_I2C_SUPPORT
#include "driver/i2c.h"

/*********** I2C configuration macros ***********/
#ifndef INX_TARGET_I2C_MASTER_SCL_IO
#define INX_TARGET_I2C_MASTER_SCL_IO            26
#endif//INX_TARGET_I2C_MASTER_SCL_IO
#ifndef INX_TARGET_I2C_MASTER_SDA_IO
#define INX_TARGET_I2C_MASTER_SDA_IO            21
#endif//INX_TARGET_I2C_MASTER_SDA_IO
#define INX_TARGET_I2C_MASTER_NUM               0
#define INX_TARGET_I2C_MASTER_FREQ_HZ           400000
#define INX_TARGET_I2C_MASTER_TX_BUF_DISABLE    0
#define INX_TARGET_I2C_MASTER_RX_BUF_DISABLE    0

static ehs_bool gbI2cInited = EHS_FALSE;

/*********** I2C Master General Functions ***********/
ehs_bool EhsTI2CMasterInit( void )
{
    int i2c_master_port = INX_TARGET_I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = INX_TARGET_I2C_MASTER_SDA_IO,
        .scl_io_num = INX_TARGET_I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = INX_TARGET_I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    if (i2c_driver_install(i2c_master_port, conf.mode, INX_TARGET_I2C_MASTER_RX_BUF_DISABLE, INX_TARGET_I2C_MASTER_TX_BUF_DISABLE, 0) == ESP_OK)
    {
        gbI2cInited = EHS_TRUE;
        return EHS_TRUE;
    }
    else return EHS_FALSE;
}

#ifdef EHS_MAX31343_SUPPORT
/*********** MAX31343 RTC/Temp I2C macros ***********/
#define INX_TARGET_I2C_MAX31343_SLAVE_ADDR      0xD0
#define MAX31343_RTC_CONFIG_1               0x03
#define MAX31343_RTC_CONFIG_2               0x04
#define MAX31343_SECONDS                    0x06
#define MAX31343_MINUTES                    0x07
#define MAX31343_HOURS                      0x08
#define MAX31343_DAY                        0x09
#define MAX31343_DATE                       0x0A
#define MAX31343_MONTH                      0x0B
#define MAX31343_YEAR                       0x0C
#define MAX31343_PWR_MGMT                   0x18
#define MAX31343_TRICKLE_REG                0x19
#define MAX31343_TEMP_MSB                   0x1A
#define MAX31343_TEMP_LSB                   0x1B
#define MAX31343_TS_CONFIG                  0x1C

/*********** MAX31343 RTC/Temp I2C commands ***********/
static void max31343_read_byte(uint8_t reg, uint8_t *data)
{
    if (gbI2cInited != EHS_TRUE) return;
    i2c_cmd_handle_t i2c_h = i2c_cmd_link_create();
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_start(i2c_h));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_write_byte(i2c_h, 0xD0, true));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_write_byte(i2c_h, reg, true));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_start(i2c_h));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_write_byte(i2c_h, 0xD1, true));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_read_byte(i2c_h, data, I2C_MASTER_NACK));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_stop(i2c_h));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_cmd_begin(INX_TARGET_I2C_MASTER_NUM, i2c_h, 2));
    i2c_cmd_link_delete(i2c_h);
}

static void max31343_write_byte(uint8_t reg, uint8_t data)
{
    if (gbI2cInited != EHS_TRUE) return;
    i2c_cmd_handle_t i2c_h = i2c_cmd_link_create();
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_start(i2c_h));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_write_byte(i2c_h, 0xD0, true));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_write_byte(i2c_h, reg, true));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_write_byte(i2c_h, data, true));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_stop(i2c_h));
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_cmd_begin(INX_TARGET_I2C_MASTER_NUM, i2c_h, 2));
    i2c_cmd_link_delete(i2c_h);
}

void EhsTMax31343Init( void )
{
    if (gbI2cInited != EHS_TRUE) return;
    /* Reg PWR_MGMT
     * Bit      Description
     * 0x3 << 4 Set Power fail voltage to 2.4V
     * 0x0 << 3 Use Vcc as supply
     * (DELETED) ~0x1 << 2 User decides power~
     */
    // max31343_write_byte(MAX31343_PWR_MGMT, (0x3 << 4));

    /* Reg TRICKLE_REG
     * Bit      Description
     * 0x5 << 4 Enable Trickle Charger
     * 0x0      3k Ohms in series with a Schottky diode
     */
    max31343_write_byte(MAX31343_TRICKLE_REG, 0x5 << 4);

    /* Reg TS_CONFIG
     * Bit      Description
     * 0x1 << 7 Enable Temperature Automatic measurement
     * 0x0 << 6 No oneshot temp measurement requested
     * 0x0 << 3 Set temp measurement interval to 1 second
     */
    max31343_write_byte(MAX31343_TS_CONFIG, 0x1 << 7);
}

float EhsTMax31343GetTemperature( void )
{
    if (gbI2cInited != EHS_TRUE) return 0;
    uint8_t data[2] = { 0 };
    max31343_read_byte(MAX31343_TEMP_MSB, data);
    max31343_read_byte(MAX31343_TEMP_LSB, data + 1);

    int fp = (data[1] >> 6) & 0b11;
    float sign = (data[0] >> 7) & 1 ? -1 : 1;
    float whole = (float) (data[0] & 0b1111111);
    float decimal = (fp >> 1) * 0.5 + (fp & 1) * 0.25;

    return sign * (whole + decimal);
}

ehs_sint32 EhsTMax31343GetTemperatureFP( void )
{
    if (gbI2cInited != EHS_TRUE) return 0;
    uint8_t data[2] = { 0 };
    max31343_read_byte(MAX31343_TEMP_MSB, data);
    max31343_read_byte(MAX31343_TEMP_LSB, data + 1);

    int fp = (data[1] >> 6) & 0b11;
    ehs_sint32 sign = (data[0] >> 7) & 1 ? -1 : 1;
    ehs_sint32 whole = (data[0] & 0b1111111) * 1024;
    ehs_sint32 decimal = (fp >> 1) * 512 + (fp & 1) * 256;

    return sign * (whole + decimal);
}

/* Get the RTC value from the chip. All parameters are output.
 * The parameter will be checked against NULL. If the parameter is NULL, there is no output for it.
 */
void EhsTMax31343GetRTC(ehs_uint8 *seconds, ehs_uint8 *minutes, ehs_uint8 *hours, ehs_uint8 *day, ehs_uint8 *date, ehs_uint8 *month, ehs_uint16 *year)
{
    if (gbI2cInited != EHS_TRUE) return;
    uint8_t reading = 0;
    if (year != NULL)
    {
        max31343_read_byte(MAX31343_YEAR, &reading);
        *year = ((reading >> 4) & 0x0F) * 10 + (reading & 0x0F) + 2000;
        max31343_read_byte(MAX31343_MONTH, &reading);
        *year += (reading >> 7) * 100;
    }
    if (month != NULL)
    {
        *month = ((reading >> 4) & 1) * 10 + (reading & 0x0F);
    }
    if (date != NULL)
    {
        max31343_read_byte(MAX31343_DATE, &reading);
        *date = ((reading >> 4) & 0b11) * 10 + (reading & 0x0F);
    }
    if (day != NULL)
    {
        max31343_read_byte(MAX31343_DAY, &reading);
        *day = reading & 0b111;
    }
    if (hours != NULL)
    {
        max31343_read_byte(MAX31343_HOURS, &reading);
        *hours = ((reading >> 4) & 0b11) * 10 + (reading & 0x0F);
    }
    if (minutes != NULL)
    {
        max31343_read_byte(MAX31343_MINUTES, &reading);
        *minutes = ((reading >> 4) & 0b111) * 10 + (reading & 0x0F);
    }
    if (seconds != NULL)
    {
        max31343_read_byte(MAX31343_SECONDS, &reading);
        *seconds = ((reading >> 4) & 0b111) * 10 + (reading & 0x0F);
    }
}

/* Set the RTC year, month, date and day for MAX31343
 * @param year  The real year value of the year between 2000 and 2199
 * @param month The Month between 1 and 12
 * @param date  The Date between 1 and 31
 */
void EhsTMax31343SetYearMonthDateDay(ehs_uint16 year, ehs_uint8 month, ehs_uint8 date, ehs_uint8 day)
{
    if (gbI2cInited != EHS_TRUE) return;
    // Do not assign the value beyond the range
    if (year < 2000 || year >= 2200) return;
    if (month == 0 || month > 12) return;
    if (date == 0 || date > 31) return;
    if (day == 0 || day > 7) return;

    uint8_t century = year / 100 - 20;
    uint8_t ones = year % 10;
    uint8_t tens = (year % 100) / 10;
    max31343_write_byte(MAX31343_YEAR, tens << 4 | ones);

    ones = month % 10;
    tens = month / 10;
    max31343_write_byte(MAX31343_MONTH, century << 7 + tens << 4 | ones);

    ones = date % 10;
    tens = date / 10;
    max31343_write_byte(MAX31343_DATE, tens << 4 | ones);

    max31343_write_byte(MAX31343_DAY, day & 0b111);
}

void EhsTMax31343SetHourMinuteSecond(ehs_uint8 hours, ehs_uint8 minutes, ehs_uint8 seconds)
{
    if (gbI2cInited != EHS_TRUE) return;
    // Do not assign the value beyond the range
    if (hours > 24 || minutes > 60 || seconds > 60) return;

    seconds++; // RTC update 1 second after it's updated
    uint8_t ones = seconds % 10;
    uint8_t tens = seconds / 10;
    max31343_write_byte(MAX31343_SECONDS, tens << 4 | ones);

    ones = minutes % 10;
    tens = minutes / 10;
    max31343_write_byte(MAX31343_MINUTES, tens << 4 | ones);

    ones = hours % 10;
    tens = hours / 10;
    max31343_write_byte(MAX31343_HOURS, tens << 4 | ones);
}

#endif//EHS_MAX31343_SUPPORT

#endif//EHS_I2C_SUPPORT

#ifdef  EHS_RTC_SUPPORT
void EhsTSetRtcSecond(ehs_uint32 time_sec)
{
    if (gbI2cInited != EHS_TRUE) return;
#ifdef  EHS_MAX31343_SUPPORT
    struct tm *tm = localtime(&time_sec);
    EhsTMax31343SetHourMinuteSecond(tm->tm_hour, tm->tm_min, tm->tm_sec);
    EhsTMax31343SetYearMonthDateDay(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_wday ? tm->tm_wday : 7);
#endif//EHS_MAX31343_SUPPORT
}

ehs_uint32 EhsTGetRtcSecond( void )
{
    if (gbI2cInited != EHS_TRUE) return 1;
#ifdef  EHS_MAX31343_SUPPORT
    struct tm tm1;
    EhsTMax31343GetRTC(&tm1.tm_sec, &tm1.tm_min, &tm1.tm_hour, &tm1.tm_wday, &tm1.tm_mday, &tm1.tm_mon, &tm1.tm_year);
    tm1.tm_year -= 1900;
    tm1.tm_mon -= 1;
    tm1.tm_wday = tm1.tm_wday == 7 ? 0 : tm1.tm_wday;
    time_t _t1 = mktime(&tm1);
    return _t1;
#endif//EHS_MAX31343_SUPPORT
    return 0;
}
#endif//EHS_RTC_SUPPORT