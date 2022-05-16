



## The following picks up config options identified in 'toolchain.mk file' and selects the appropriate support code for each option.

ifdef EHS_GUI_SUPPORT
ifneq ($(EHS_GUI_SUPPORT), none)
EHS_TARGET_GRAPHICS_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/graphics/$(EHS_GUI_SUPPORT)
DEFS+=EHS_GUI_SUPPORT
include $(EHS_TARGET_GRAPHICS_PATH)/graphics.mk
include $(EHS_TARGET_GRAPHICS_PATH)/Components/components.mk
endif
endif

ifdef EHS_AV_SUPPORT
ifneq ($(EHS_AV_SUPPORT), none)
# include files to build the video for this platform slightly OS specfic - but HW independent for supported HW
EHS_TARGET_AV_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/AV/$(EHS_AV_SUPPORT)_$(EHS_OS)
DEFS+=EHS_AV_SUPPORT
ifdef EHS_VIDEO_SUPPORT # @todo video.mk includes all the video playstuff so need to define VIDEO support for audio also at the moment

VPATH +=: $(EHS_TARGET_AV_PATH)
INC_DIRS += $(EHS_TARGET_AV_PATH)
include $(EHS_TARGET_AV_PATH)/deps.mk

include $(EHS_TARGET_AV_PATH)/video.mk
DEFS+=EHS_VIDEO_SUPPORT
endif
endif
endif

ifdef EHS_WEBKIT_SUPPORT
ifneq ($(EHS_WEBKIT_SUPPORT), none)
EHS_TARGET_WEBKIT_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/webkit/$(EHS_WEBKIT_SUPPORT)
DEFS+=EHS_WEBKIT_SUPPORT
include $(EHS_TARGET_WEBKIT_PATH)/webkit.mk
endif
endif

ifdef EHS_TEXT_DEVICE_SUPPORT
ifneq ($(EHS_TEXT_DEVICE_SUPPORT), none)
# include files to build the video for this platform slightly OS specfic - but HW independent for supported HW
EHS_TARGET_TEXT_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/peripherals/$(EHS_TEXT_DEVICE_SUPPORT)_$(EHS_OS)
DEFS+=EHS_TEXT_DEVICE_SUPPORT
include $(EHS_TARGET_TEXT_PATH)/text-display.mk #@todo these make files should all have the same name to make life simpler
endif
endif


ifndef EHS_COMMS_TASK
EHS_COMMS_TASK=tcp_server_common
endif

#include files to use the TCP/IP interface as a server on this platform - Not in common??
EHS_TARGET_SERVER_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/comms/$(EHS_COMMS_TASK)




