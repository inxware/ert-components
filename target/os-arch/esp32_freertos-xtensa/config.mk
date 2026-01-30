# Add default configuration files for this platform here

# esp32 dependency and default configuration

#"Currently not building as the stubbed HAL doesn't seem to work

ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifndef EHS_FILESYSTEM_SUPPORT
	EHS_FILESYSTEM_SUPPORT=posix
endif
endif

ifndef EHS_PERIPHERALS_GPIO_SUPPORT
	EHS_PERIPHERALS_GPIO_SUPPORT=ESP32_IDF
endif

#Use NVRAM (for WiFi crenedtials to reduce clobbering risk of flash updates)
#EHS_NVS_SUPPORT=ESP32S3

#default to allowing SSID for WiFi to be configured using serial tty interface
EHS_HAL_INTERFACE_CONFIG_SUPPORT=EHS_HAL_INTERFACE_CONFIG_ESP32
#default to allowing TCPIP to be configured using serial tty interface
EHS_HAL_NETWORK_CONFIG_SUPPORT=EHS_HAL_NETWORK_CONFIG_ESP32
#Always have the configu function block as a default
EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT=yes
