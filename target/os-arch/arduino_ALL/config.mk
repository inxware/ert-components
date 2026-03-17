#Arduino default dependency and feature configurations

#For now we will stubb all file systems on ardino by default.
EHS_FILESYSTEM_SUPPORT       ?= stubbed
EHS_PERIPHERALS_UART_SUPPORT ?= stubbed

# New peripheral HAL defaults — stub all on Arduino targets
EHS_WATCHDOG_SUPPORT      ?= stubbed
EHS_UPS_SUPPORT           ?= stubbed
EHS_BUZZER_SUPPORT        ?= stubbed
EHS_SD_SELECT_SUPPORT     ?= stubbed
EHS_USB_POWER_SUPPORT     ?= stubbed
EHS_ACCELEROMETER_SUPPORT ?= stubbed
EHS_RS485_CONFIG_SUPPORT  ?= stubbed
