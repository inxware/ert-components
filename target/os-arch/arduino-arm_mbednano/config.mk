##########################################################
####### Arduino mbednano connect configuration ###########
##########################################################

ifneq ($(EHS_COMPONENT_NETWORKING_SUPPORT),none)
ifndef EHS_COMPONENT_NETWORKING_SUPPORT
  EHS_COMPONENT_NETWORKING_SUPPORT=no-curl
endif
endif

# exclude components that depend on curl
ifneq ($(EHS_COMPONENTS_NETWORK_URL_GET),none)
ifndef EHS_COMPONENTS_NETWORK_URL_GET
  EHS_COMPONENTS_NETWORK_URL_GET=none
endif
endif

ifneq ($(EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER),none)
ifndef EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER	
  EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER=none
endif
endif

ifneq ($(EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER),none)
ifndef EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT
EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER=none
endif
endif

ifneq ($(EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT),none)
ifndef EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT
EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT=none
endif
endif

ifneq ($(EHS_COMMS_API_SUPPORT),none)
ifndef EHS_COMMS_API_SUPPORT
EHS_COMMS_API_SUPPORT=arduino_nina
endif
endif

ifneq ($(EHS_MQTT_SUPPORT),none)
ifndef EHS_MQTT_SUPPORT
EHS_MQTT_SUPPORT=arduino
endif
endif

# Use LEDs from RP2040 WiFiNINA
ifneq ($(EHS_PERIPHERALS_LED_SUPPORT),none)
ifndef EHS_PERIPHERALS_LED_SUPPORT
EHS_PERIPHERALS_LED_SUPPORT=arduino_nina
endif
endif

# Use accelometer and gyroscope library (Arduino_LSM6DS3@1.0.3)
ifneq ($(EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT),none)
ifndef EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT=arduino
endif
endif
