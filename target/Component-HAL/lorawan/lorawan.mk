# LoRaWAN HAL makefile
#
# EHS_LORAWAN_SUPPORT selects the modem module subdirectory:
#   wio_e5   — Seeed WIO-E5 AT-command modem (UART)
#   rak3112  — RAK3112 (ESP32-S3 + SX1262) via SX126x-Arduino LoRaWAN stack
#   stubbed  — no-op stub for platforms without a physical modem
#
# Module subdirectories each provide:
#   lorawan_module.h                    — common LoRaWAN_module_* API
#   lorawan-<module>.c / .cpp           — module-specific implementation
#   lorawan_module.mk                   — adds module objects to OBJECTS
#                                         (and any module-specific link flags)

EHS_TARGET_LORAWAN_PATH        := $(EHS_TARGET_COMPONENT_HAL_PATH)/lorawan
EHS_TARGET_LORAWAN_MODULE_PATH := $(EHS_TARGET_LORAWAN_PATH)/$(EHS_LORAWAN_SUPPORT)

VPATH    +=: $(EHS_TARGET_LORAWAN_PATH)
VPATH    +=: $(EHS_TARGET_LORAWAN_MODULE_PATH)
INC_DIRS += $(EHS_TARGET_LORAWAN_PATH)
INC_DIRS += $(EHS_TARGET_LORAWAN_MODULE_PATH)

OBJECTS += lorawan_helper.$(OBJ)
OBJECTS += lorawan.$(OBJ)

include $(EHS_TARGET_LORAWAN_MODULE_PATH)/lorawan_module.mk
