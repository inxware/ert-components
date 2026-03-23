
#Default dependencies and feature support for MinGW x86 targets.

EHS_COMMS_API_SUPPORT                ?= winsock

# Default network features for Windows targets
EHS_COMPONENTS_NETWORK_URL_GET       ?= enabled

EHS_IMAGEPROCESSING_APRILTAG_SUPPORT ?= enabled

EHS_PERIPHERALS_UART_SUPPORT         ?= win32

# New peripheral HAL defaults — stub on MinGW/Windows targets
EHS_WATCHDOG_SUPPORT      ?= stubbed
EHS_UPS_SUPPORT           ?= stubbed
EHS_BUZZER_SUPPORT        ?= stubbed
EHS_SD_SELECT_SUPPORT     ?= stubbed
EHS_USB_POWER_SUPPORT     ?= stubbed
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT ?= stubbed
EHS_RS485_CONFIG_SUPPORT  ?= stubbed
