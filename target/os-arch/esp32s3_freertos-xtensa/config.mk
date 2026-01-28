# Add default configuration files for this platform here

ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifndef EHS_FILESYSTEM_SUPPORT
	EHS_FILESYSTEM_SUPPORT=posix
endif
endif

# We don't want to enable Ethernet support by default if no other networking is enabled usually, but leaving this in for now untill all ethernet targets are checked without it.

ifneq ($(EHS_NETWORK_ETHERNET_SUPPORT),none)
ifndef EHS_NETWORK_ETHERNET_SUPPORT
ifndef EHS_NETWORK_WIFI_SUPPORT
	EHS_NETWORK_ETHERNET_SUPPORT=yes
	DEFS += EHS_NETWORK_ETHERNET_SUPPORT
endif
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
ifndef EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT
	EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT=yes
endif

EHS_NVS_SUPPORT=ESP32S3
