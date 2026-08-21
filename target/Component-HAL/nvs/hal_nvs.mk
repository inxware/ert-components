
INC_DIRS += $(EHS_TARGET_COMPONENT_HAL_PATH)/nvs
ifndef EHS_NVS_SUPPORT
	EHS_NVS_SUPPORT=stubbed
endif
INC_DIRS += $(EHS_TARGET_COMPONENT_HAL_PATH)/nvs/${EHS_NVS_SUPPORT}
# VPATH as well as the path-qualified object below: the CMake-master Zephyr
# build resolves OBJECTS back to sources by looking the bare stem up in VPATH
# (see _ert_find_src in the top-level Makefile), so a path-qualified object
# alone leaves target_nvs.c out of the build and every EhsNvs* call unresolved
# at link time. Only the selected backend's directory is added, so there is no
# ambiguity between the stubbed and ESP32S3 copies.
VPATH += $(EHS_TARGET_COMPONENT_HAL_PATH)/nvs/${EHS_NVS_SUPPORT}
OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/nvs/${EHS_NVS_SUPPORT}/target_nvs.${OBJ}
