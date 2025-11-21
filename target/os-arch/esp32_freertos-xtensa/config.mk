# Add default configuration files for this platform here

# esp32 dependency and default configuration


ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifndef EHS_FILESYSTEM_SUPPORT
	EHS_FILESYSTEM_SUPPORT=posix
endif
endif

ifndef EHS_PERIPHERALS_GPIO_SUPPORT
	EHS_PERIPHERALS_GPIO_SUPPORT=ESP32_IDF
endif
