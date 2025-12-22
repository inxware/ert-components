# NimBLE BLE HAL Makefile
# Used for ESP32 platforms with NimBLE stack

# Add NimBLE source files to build
OBJECTS += ble_service_nimble.$(OBJ)

# Add glue layer
OBJECTS += inx-ble_service_hal_glue.$(OBJ)
