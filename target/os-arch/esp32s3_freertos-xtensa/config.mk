# Add default configuration files for this platform here

EHS_FILESYSTEM_SUPPORT              ?= posix
EHS_MQTT_SUPPORT                    ?= esp_mqtt
EHS_PERIPHERALS_GPIO_SUPPORT        ?= ESP32_IDF
EHS_PERIPHERALS_UART_SUPPORT        ?= esp32s3

#We always support continuous on ESP32s3
EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT ?= yes

# Network config should be default on
# Always have the config function block as a default
EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT ?= yes

#Use NVRAM (for WiFi credentials to reduce clobbering risk of flash updates)
EHS_NVS_SUPPORT                     ?= ESP32S3

# default to allowing SSID for WiFi to be configured using serial tty interface
EHS_HAL_INTERFACE_CONFIG_SUPPORT    ?= EHS_HAL_INTERFACE_CONFIG_ESP32

# default to allowing TCPIP to be configured using serial tty interface
EHS_HAL_NETWORK_CONFIG_SUPPORT      ?= EHS_HAL_NETWORK_CONFIG_ESP32

# New peripheral HAL defaults — stub on ESP32-S3 unless overridden by platform config.mk
EHS_WATCHDOG_SUPPORT      ?= stubbed
EHS_UPS_SUPPORT           ?= stubbed
EHS_BUZZER_SUPPORT        ?= stubbed
EHS_SD_SELECT_SUPPORT     ?= stubbed
EHS_USB_POWER_SUPPORT     ?= stubbed
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT ?= stubbed
EHS_RS485_CONFIG_SUPPORT  ?= stubbed

# Default packager for this os-arch. A platform config.mk may override it with a
# plain '=' -- this file is included after the platform's own config (see
# target/platform/platform.mk, "include $(EHS_TARGET_OS_HW_PATH)/config.mk"),
# so '?=' here yields to the platform. 'make targetenv_package' dispatches on it.
EHS_PACKAGER_TYPE ?= esp32
