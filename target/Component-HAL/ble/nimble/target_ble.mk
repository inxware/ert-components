# NimBLE BLE HAL Makefile
# Used for ESP32 platforms with NimBLE stack

# Add NimBLE source files to build
OBJECTS += ble_service_nimble.$(OBJ)

# Add glue layer
OBJECTS += inx-ble_service_hal_glue.$(OBJ)

ifeq ($(EHS_ARCH), esp32_freertos-xtensa)
	LIBS += bt 
	LIBS += btdm_app 
else ifeq ($(EHS_ARCH), esp32s3_freertos-xtensa)
	LIBS += bt 
	LIBS += btdm_app 
else ifeq ($(EHS_ARCH), esp32c3_freertos-xtensa)
	LIBS += bt 
	LIBS += btdm_app 
else
	$(error "NimBLE BLE HAL not supported for architecture $(EHS_ARCH)")
endif
