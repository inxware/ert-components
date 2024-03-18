#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

## The following picks up config options identified in 'toolchain.mk file' and selects the appropriate support code for each option.

########################################################################################################
## Graphics and UI support
########################################################################################################
ifdef EHS_GUI_SUPPORT
ifneq ($(EHS_GUI_SUPPORT), none)
include $(EHS_TARGET_COMPONENT_HAL_PATH)/graphics/graphics_common.mk
endif
endif

########################################################################################################
## Audio Visual Support
########################################################################################################
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

########################################################################################################
## HTML Rendering
########################################################################################################
ifdef EHS_WEBKIT_SUPPORT
ifneq ($(EHS_WEBKIT_SUPPORT), none)
EHS_TARGET_WEBKIT_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/webkit/$(EHS_WEBKIT_SUPPORT)
DEFS+=EHS_WEBKIT_SUPPORT
include $(EHS_TARGET_WEBKIT_PATH)/webkit.mk
endif
endif

########################################################################################################
## Text Displays 
########################################################################################################

ifdef EHS_TEXT_DEVICE_SUPPORT
ifneq ($(EHS_TEXT_DEVICE_SUPPORT), none)
# include files to build the video for this platform slightly OS specfic - but HW independent for supported HW
EHS_TARGET_TEXT_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/peripherals/$(EHS_TEXT_DEVICE_SUPPORT)_$(EHS_OS)
DEFS+=EHS_TEXT_DEVICE_SUPPORT
include $(EHS_TARGET_TEXT_PATH)/text-display.mk #@todo these make files should all have the same name to make life simpler
endif
endif

########################################################################################################
## Comms 
########################################################################################################

ifndef EHS_COMMS_TASK
EHS_COMMS_TASK=tcp_server_common
endif

#include files to use the TCP/IP interface as a server on this platform - Not in common??
EHS_TARGET_SERVER_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/comms/$(EHS_COMMS_TASK)


########################################################################################################
## MCU Peripherals (IO)
########################################################################################################

#Set up some preprocessor enums for different types of peripherals 
#Options for EHS_PERIPHERALS_GPIO_SUPPORT (C code DEFS)
DEFS+=EHS_PERIPHERALS_GPIO_TYPE_STUBBED=0
DEFS+=EHS_PERIPHERALS_GPIO_TYPE_NXP_K64=1
DEFS+=EHS_PERIPHERALS_GPIO_TYPE_SYSFS_LINUX_ARM=2
DEFS+=EHS_PERIPHERALS_GPIO_TYPE_ESP32_IDF=3
DEFS+=EHS_PERIPHERALS_GPIO_TYPE_GUI=4
DEFS+=EHS_PERIPHERALS_GPIO_TYPE_UNKNOWN=1000000
# Options for EHS_PERIPHERALS_ADC_DAC_SUPPORT (C code DEFS)
DEFS+=EHS_PERIPHERALS_ADCDAC_TYPE_STUBBED=0
DEFS+=EHS_PERIPHERALS_ADCDAC_TYPE_NXP_K64=1
DEFS+=EHS_PERIPHERALS_ADCDAC_TYPE_SPI_A6_LTC241X=2
DEFS+=EHS_PERIPHERALS_ADCDAC_TYPE_ESP32_IDF=3
DEFS+=EHS_PERIPHERALS_ADCDAC_TYPE_UNKNOWN=1000000
# Options for EHS_OTA_SUPPORT (C code DEFS)
DEFS+=EHS_OTA_SUPPORT_STUBBED=0
DEFS+=EHS_OTA_SUPPORT_SUPPORT=1

# The following components' source is actually in the target_os/*/ directories, 
# because we don't have a VPATH set for selecting the a shared component-HAL directory.  
# todo2022 The sourve should be moved to a the component-HAL when this is done so it can be 
# shared across relevant os-arches are appropriate. 
# Choose the appropriate target support for GPIO

ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),stubbed)
	OBJECTS += gpio_hal_stubb.$(OBJ)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_STUBBED
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),gui)
	OBJECTS += gpio_hal_gui.$(OBJ)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_GUI
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),NXP_K64)
	OBJECTS += target_gpio.$(OBJ)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_NXP_K64
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),SYSFS_LINUX_ARM)
	OBJECTS += target_gpio.$(OBJ)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_SYSFS_LINUX_ARM
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),ESP32_IDF)
	OBJECTS += target_gpio.$(OBJ)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_ESP32_IDF
else  
#This shold be probably fail as we don't really have an unknown option
# DONT SET 	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_UKNOWN
endif

# Choose appropriate target support ADCDAC
ifeq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),stubbed)
	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_STUBBED
	OBJECTS += target_adcdac.$(OBJ)
# todo2022 we need to create stub code if we don't want to leave a hole in the toolbox.
else ifeq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),SPI_A6_LTC241X)
	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_SPI_A6_LTC241X
	OBJECTS += target_adcdac.$(OBJ)
	OBJECTS += spi.$(OBJ)
	OBJECTS += ltc241x.$(OBJ)
else ifeq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),NXP_K64)
	OBJECTS += target_adcdac.$(OBJ)
	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_NXP_K64
else ifeq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),ESP32_IDF)
	OBJECTS += target_adcdac.$(OBJ)
	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_ESP32_IDF
# The NXP os-arch prbably hardwires ADC support in already included code.
else
# DONT SET DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_UNKNOWN
# No objects added - might leave a hole in the toolbox and cause a build error.
endif

########################################################################################################
## LoRaWAN
########################################################################################################
ifdef EHS_LORAWAN_SUPPORT
include $(EHS_TARGET_COMPONENT_HAL_PATH)/lorawan/lorawan.mk
endif

########################################################################################################
## Wi-Fi
########################################################################################################
ifdef EHS_WIFI_SUPPORT
include $(EHS_TARGET_COMPONENT_HAL_PATH)/wifi/wifi.mk
endif

########################################################################################################
## Multi-Channel PID Controller
########################################################################################################
ifeq ($(EHS_PID_SUPPORT),stubbed)
OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/inx-PID_stub.$(OBJ)
else ifeq ($(EHS_PID_SUPPORT),gnu)
OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/inx-PID_gnu.$(OBJ)
OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/inx-PID_heatrod.$(OBJ)
else ifeq ($(EHS_PID_SUPPORT),esp32)
#VPATH += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller
INC_DIRS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller
OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/inx-PID_heatrod.$(OBJ)
OBJECTS += target_pid.$(OBJ)
else
# No objects added - might leave a hole in the toolbox and cause a build error.
endif

########################################################################################################
## OTA
########################################################################################################
ifeq ($(EHS_OTA_SUPPORT),stubbed)
DEFS+=EHS_OTA_SUPPORT=EHS_OTA_SUPPORT_STUBBED
else
DEFS+=EHS_OTA_SUPPORT=EHS_OTA_SUPPORT_SUPPORT
include $(EHS_TARGET_COMPONENT_HAL_PATH)/system/ota/target_hal_ota.mk
endif
