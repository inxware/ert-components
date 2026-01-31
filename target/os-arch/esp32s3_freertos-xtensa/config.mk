# Add default configuration files for this platform here

ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifndef EHS_FILESYSTEM_SUPPORT
	EHS_FILESYSTEM_SUPPORT=posix
endif
endif

ifneq ($(EHS_MQTT_SUPPORT),none)
ifndef EHS_MQTT_SUPPORT
    EHS_MQTT_SUPPORT=esp_mqtt
endif
endif	

ifneq ($(EHS_PERIPHERALS_GPIO_SUPPORT),none)
ifndef EHS_PERIPHERALS_GPIO_SUPPORT
	EHS_PERIPHERALS_GPIO_SUPPORT=ESP32_IDF
endif
endif

#We always support continuous on ESP32s3
ifneq ($(EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT),none)
ifndef EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT
	EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT=yes
endif
endif

# Network config should be default on
ifneq ($(EHS_COMPONENT_NETWORKING_SUPPORT),none)
ifndef EHS_COMPONENT_NETWORKING_SUPPORT
#Always have the configu function block as a default
EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT=yes
endif
endif

#Use NVRAM (for WiFi crenedtials to reduce clobbering risk of flash updates)
ifneq ($(EHS_NVS_SUPPORT),none)
ifndef EHS_NVS_SUPPORT
EHS_NVS_SUPPORT=ESP32S3
endif
endif

ifneq ($(EHS_HAL_INTERFACE_CONFIG_SUPPORT),none)
ifndef EHS_HAL_INTERFACE_CONFIG_SUPPORT
#default to allowing SSID for WiFi to be configured using serial tty interface would be nice but needs backporting from s3
EHS_HAL_INTERFACE_CONFIG_SUPPORT=EHS_HAL_INTERFACE_CONFIG_ESP32
#EHS_HAL_INTERFACE_CONFIG_SUPPORT=EHS_HAL_INTERFACE_CONFIG_ESP32
endif
endif

ifneq ($(EHS_HAL_NETWORK_CONFIG_SUPPORT),none)
ifndef EHS_HAL_NETWORK_CONFIG_SUPPORT
#default to allowing TCPIP to be configured using serial tty interface
EHS_HAL_NETWORK_CONFIG_SUPPORT=EHS_HAL_NETWORK_CONFIG_ESP32
endif
endif

