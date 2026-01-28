# BLE Service HAL Makefile

# Determine which BLE stack to use based on platform
ifdef EHS_NETWORK_BLE_SUPPORT
ifneq ($(EHS_NETWORK_BLE_SUPPORT),none)

    EHS_COMMON_BLE_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/ble
    EHS_TARGET_BLE_HAL_PATH=$(EHS_COMMON_BLE_HAL_PATH)/$(EHS_NETWORK_BLE_SUPPORT)
    include $(EHS_TARGET_BLE_HAL_PATH)/target_ble.mk

    # Add NimBLE include path
    INC_DIRS += $(EHS_TARGET_BLE_HAL_PATH)

    # Add to VPATH so make can find the sources
    VPATH += $(EHS_TARGET_BLE_HAL_PATH)

endif
endif

