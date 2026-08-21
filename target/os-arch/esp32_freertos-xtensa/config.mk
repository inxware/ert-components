# Add default configuration files for this platform here

# esp32 dependency and default configuration

#"Currently not building as the stubbed HAL doesn't seem to work

EHS_FILESYSTEM_SUPPORT              ?= posix
EHS_PERIPHERALS_GPIO_SUPPORT        ?= ESP32_IDF
EHS_PERIPHERALS_UART_SUPPORT        ?= esp32s3
EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT ?= yes

#Use NVRAM (for WiFi credentials to reduce clobbering risk of flash updates)
#EHS_NVS_SUPPORT=ESP32S3

# default to allowing SSID for WiFi to be configured using serial tty interface
EHS_HAL_INTERFACE_CONFIG_SUPPORT    ?= EHS_HAL_INTERFACE_CONFIG_STUBBED

# default to stubbed TCPIP configuration
EHS_HAL_NETWORK_CONFIG_SUPPORT      ?= EHS_HAL_NETWORK_CONFIG_STUBBED

# New peripheral HAL defaults — stub on ESP32 unless overridden by platform config.mk
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
