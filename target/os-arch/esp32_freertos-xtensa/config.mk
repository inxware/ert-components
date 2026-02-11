# Add default configuration files for this platform here

# esp32 dependency and default configuration

#"Currently not building as the stubbed HAL doesn't seem to work

ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifndef EHS_FILESYSTEM_SUPPORT
	EHS_FILESYSTEM_SUPPORT=posix
endif
endif

ifneq ($(EHS_PERIPHERALS_GPIO_SUPPORT),none)	
ifndef EHS_PERIPHERALS_GPIO_SUPPORT
	EHS_PERIPHERALS_GPIO_SUPPORT=ESP32_IDF
endif
endif

ifneq ($(EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT),none)
ifndef EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT
EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT=yes
endif
endif


#Use NVRAM (for WiFi crenedtials to reduce clobbering risk of flash updates)
#EHS_NVS_SUPPORT=ESP32S3

ifneq ($(EHS_HAL_INTERFACE_CONFIG_SUPPORT),none)
ifndef EHS_HAL_INTERFACE_CONFIG_SUPPORT
#default to allowing SSID for WiFi to be configured using serial tty interface
EHS_HAL_INTERFACE_CONFIG_SUPPORT=EHS_HAL_INTERFACE_CONFIG_STUBBED
endif
endif

ifneq ($(EHS_HAL_NETWORK_CONFIG_SUPPORT),none)
ifndef EHS_HAL_NETWORK_CONFIG_SUPPORT
#default to stubbed TCPIP configuration
EHS_HAL_NETWORK_CONFIG_SUPPORT=EHS_HAL_NETWORK_CONFIG_STUBBED
endif
endif

