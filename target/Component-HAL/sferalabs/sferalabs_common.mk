# sferalabs_common.mk — shared build rules for all Sfera Labs HAL modules.
#
# Include this from any sferalabs peripheral target_*.mk instead of
# repeating the INC_DIRS / VPATH / OBJECTS entries by hand.
#
# Guard prevents double-inclusion when multiple sferalabs peripherals
# are built in the same target (e.g. UPS + GPIO + ADC).
#
# Provides:
#   sferalabs_poll    — periodic read-and-compare thread (slow; UPS, ADC, etc.)
#   sferalabs_fd_poll — POSIX poll() fd thread (fast; GPIO inputs if supported)

ifndef SFERALABS_COMMON_INCLUDED
SFERALABS_COMMON_INCLUDED := 1

EHS_SFERALABS_COMMON_PATH ?= $(EHS_TARGET_COMPONENT_HAL_PATH)/sferalabs

INC_DIRS += $(EHS_SFERALABS_COMMON_PATH)
VPATH    += $(EHS_SFERALABS_COMMON_PATH)
OBJECTS  += sferalabs_poll.$(OBJ)
OBJECTS  += sferalabs_fd_poll.$(OBJ)

endif
