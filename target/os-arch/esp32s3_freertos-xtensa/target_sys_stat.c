#include "hal_target_sys_stat.h"
#include "target_types.h"
#include "driver/temperature_sensor.h"

static temperature_sensor_handle_t gTempSensor = NULL;
static temperature_sensor_config_t gTempSensorConfig = TEMPERATURE_SENSOR_CONFIG_DEFAULT(0, 80);

ehs_sint32 EhsTGetCpuTemp()
{
    float temp;
    if (gTempSensor == NULL)
    {
        ESP_ERROR_CHECK(temperature_sensor_install(&gTempSensorConfig, &gTempSensor));
        ESP_ERROR_CHECK(temperature_sensor_enable(gTempSensor));
    }
    ESP_ERROR_CHECK(temperature_sensor_get_celsius(gTempSensor, &temp));
    return (ehs_sint32) temp;
}

ehs_sint32 EhsTGetCpuUsagePercent()
{
    return 0;
}

ehs_sint32 EhsTGetRamUsagePercent()
{
    return 0;
}