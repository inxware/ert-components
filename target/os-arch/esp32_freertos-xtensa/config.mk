# Add default configuration files for this platform here

# esp32 dependency and default configuration

#"Currently not building as the stubbed HAL doesn't seem to work

EHS_FILESYSTEM_SUPPORT              ?= posix
EHS_PERIPHERALS_GPIO_SUPPORT        ?= ESP32_IDF
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
EHS_ACCELEROMETER_SUPPORT ?= stubbed
EHS_RS485_CONFIG_SUPPORT  ?= stubbed
