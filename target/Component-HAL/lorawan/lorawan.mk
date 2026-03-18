# LoRaWAN HAL makefile
#
# EHS_LORAWAN_SUPPORT selects the modem module subdirectory:
#   wio_e5   — Seeed WIO-E5 AT-command modem (UART)
#   stubbed  — no-op stub for platforms without a physical modem
#
# Module subdirectories each provide:
#   lorawan-wio_e5.h / lorawan-wio_e5.c  (implementation or stub)
#   lorawan_module.mk                     (adds module objects to OBJECTS)
#
# TODO: when a second real module is added, lorawan.c should be refactored
# to call common LoRaWAN_module_* function names instead of LoRaWAN_wioe5_*
# directly, and each module directory will implement that common interface.
# At that point the switch-on-target dispatch in lorawan.c can be removed.

EHS_TARGET_LORAWAN_PATH        := $(EHS_TARGET_COMPONENT_HAL_PATH)/lorawan
EHS_TARGET_LORAWAN_MODULE_PATH := $(EHS_TARGET_LORAWAN_PATH)/$(EHS_LORAWAN_SUPPORT)

VPATH    +=: $(EHS_TARGET_LORAWAN_PATH)
VPATH    +=: $(EHS_TARGET_LORAWAN_MODULE_PATH)
INC_DIRS += $(EHS_TARGET_LORAWAN_PATH)
INC_DIRS += $(EHS_TARGET_LORAWAN_MODULE_PATH)

OBJECTS += lorawan_helper.$(OBJ)
OBJECTS += lorawan.$(OBJ)

include $(EHS_TARGET_LORAWAN_MODULE_PATH)/lorawan_module.mk
