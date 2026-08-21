##########################################################
####### Arduino mbednano connect configuration ###########
##########################################################

EHS_COMPONENT_NETWORKING_SUPPORT ?= no-curl

# exclude components that depend on curl
EHS_COMPONENTS_NETWORK_URL_GET ?= none

EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER ?= none

# Cross-dependency guard: if CONFIG_SUPPORT is absent, force DEVMAN_PLAYER off.
# (ifneq/ifndef kept intentionally — outer and inner guards test different variables)
ifneq ($(EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER),none)
ifndef EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT
EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER=none
endif
endif

EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT ?= none

EHS_COMMS_API_SUPPORT ?= arduino_nina

EHS_MQTT_SUPPORT ?= arduino

# Use LEDs from RP2040 WiFiNINA
EHS_PERIPHERALS_LED_SUPPORT ?= arduino_nina

# Use accelometer and gyroscope library (Arduino_LSM6DS3@1.0.3)
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT ?= arduino

# Default packager for this os-arch. A platform config.mk may override it with a
# plain '=' -- this file is included after the platform's own config (see
# target/platform/platform.mk, "include $(EHS_TARGET_OS_HW_PATH)/config.mk"),
# so '?=' here yields to the platform. 'make targetenv_package' dispatches on it.
EHS_PACKAGER_TYPE ?= arduino
