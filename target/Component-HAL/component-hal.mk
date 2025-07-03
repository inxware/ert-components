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
## MCU Peripherals (IO) Labels
########################################################################################################

#Set up some preprocessor enums for different types of peripherals 
#Options for EHS_PERIPHERALS_GPIO_SUPPORT (C code DEFS)

# Options for EHS_OTA_SUPPORT (C code DEFS)
DEFS+=EHS_OTA_SUPPORT_STUBBED=0
DEFS+=EHS_OTA_SUPPORT_SUPPORT=1

# GENERAL POLICY: for HAL Makefiles
# The following components' source is actually in the target_os/*/ directories, 
# because we don't have a VPATH set for selecting the a shared component-HAL directory.  
# todo2022 The sourve should be moved to a the component-HAL when this is done so it can be 
# shared across relevant os-arches are appropriate. 
# Choose the appropriate target support for GPIO

########################################################################################################
## GPIO & PWM
########################################################################################################

ifdef EHS_PERIPHERALS_GPIO_SUPPORT
ifneq ($(EHS_PERIPHERALS_GPIO_SUPPORT),none)
include $(EHS_TARGET_COMPONENT_HAL_PATH)/gpio/gpio_common.mk
endif
endif


########################################################################################################
## ADC & DAC
########################################################################################################

ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
ifneq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),none)
include $(EHS_TARGET_COMPONENT_HAL_PATH)/adc_dac/adc_dac_common.mk
endif
endif


########################################################################################################
## LoRaWAN
########################################################################################################
ifdef EHS_LORAWAN_SUPPORT
ifneq ($(EHS_LORAWAN_SUPPORT),none)
DEFS += EHS_LORAWAN_SUPPORT
include $(EHS_TARGET_COMPONENT_HAL_PATH)/lorawan/lorawan.mk
endif
endif

#TODO2025 - make the ifneq (..., none) consistent here for everything

########################################################################################################
## Wi-Fi
########################################################################################################
ifdef EHS_WIFI_SUPPORT
include $(EHS_TARGET_COMPONENT_HAL_PATH)/wifi/wifi.mk
endif

########################################################################################################
## Machine Vision
########################################################################################################
ifdef EHS_MV_SUPPORT
include $(EHS_TARGET_COMPONENT_HAL_PATH)/mv/mv_common.mk
endif

########################################################################################################
## Machine Learning and AI
########################################################################################################
ifdef EHS_ML_SUPPORT
include $(EHS_TARGET_COMPONENT_HAL_PATH)/ml/ml_common.mk
endif

########################################################################################################
## Multi-Channel PID Controller
########################################################################################################

##todo 2025 all thisshould go in a PID make file not here.
ifdef EHS_PID_SUPPORT
DEFS+=EHS_PID_SUPPORT=$(EHS_PID_SUPPORT)
ifeq ($(EHS_PID_SUPPORT),stubbed)
OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/inx-PID_stub.$(OBJ)
else ifeq ($(EHS_PID_SUPPORT),gnu)
## Thermocouple circuit gain
##  EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM has the bitlength of EHS_THERMOCOUPLE_GAIN_SHIFT_BITS
##  The calculation result involving the multiplier would be shifted right (or divided by one shifted left by) the shift bits.
DEFS += EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM=748
DEFS += EHS_THERMOCOUPLE_GAIN_SHIFT_BITS=17
## Maximum ADC reading in 10-bit fixed point. This is only used to check the thermocouple parameters during build time
DEFS += EHS_THERMOCOUPLE_ADC_MAX_FP=1843200
DEFS += EHS_PT100_AMP_GAIN_SCALE=7652
DEFS += EHS_PT100_AMP_GAIN_DIVIDER=137
OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/inx-PID_gnu.$(OBJ)
else ifeq ($(EHS_PID_SUPPORT),esp32)
#VPATH += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller
INC_DIRS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller
OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/inx-PID_isr.$(OBJ)
OBJECTS += target_pid.$(OBJ)
else
# No objects added - might leave a hole in the toolbox and cause a build error.
endif
endif

########################################################################################################
## OTA
########################################################################################################
ifdef EHS_MQTT_SUPPORT
  ifeq ($(EHS_OTA_SUPPORT),stubbed)
    DEFS+=EHS_OTA_SUPPORT=EHS_OTA_SUPPORT_STUBBED
  else
    DEFS+=EHS_OTA_SUPPORT=EHS_OTA_SUPPORT_SUPPORT
    include $(EHS_TARGET_COMPONENT_HAL_PATH)/system/ota/target_hal_ota.mk
  endif
endif

########################################################################################################
## MQTT
########################################################################################################
ifdef EHS_MQTT_SUPPORT
include $(EHS_TARGET_COMPONENT_HAL_PATH)/mqtt/mqtt_common.mk
endif

########################################################################################################
## HTTP Server
########################################################################################################
ifdef EHS_HTTPSERVER_SUPPORT
ifneq ($(EHS_HTTPSERVER_SUPPORT),none)
	include $(EHS_TARGET_COMPONENT_HAL_PATH)/httpserver/httpserver_common.mk
endif
endif

########################################################################################################
## SYSTEM STATICS
########################################################################################################
include $(EHS_TARGET_COMPONENT_HAL_PATH)/system/sys_stat/target_hal_sys_stat.mk

########################################################################################################
## MODBUS
########################################################################################################
ifdef EHS_MODBUS_SUPPORT
  include $(EHS_TARGET_COMPONENT_HAL_PATH)/modbus/modbus_common.mk
endif

########################################################################################################
## RTC
########################################################################################################
ifdef EHS_RTC_SUPPORT
DEFS += EHS_RTC_SUPPORT
  include $(EHS_TARGET_COMPONENT_HAL_PATH)/system/rtc/target_hal_rtc.mk
endif


########################################################################################################
## UART Stub
########################################################################################################
#todo2025 this needs to be conditional on this beinavavilable (or GPIO toolbox being enabled)
include $(EHS_TARGET_COMPONENT_HAL_PATH)/uart/stubbed/target_stubbed_uart.mk

########################################################################################################
## JSON Common
########################################################################################################
#todo2025 this needs to be conditional on this being available (or toolbox being enabled)
include $(EHS_TARGET_COMPONENT_HAL_PATH)/json/json_thal.mk

########################################################################################################
## HASH Algorithms Common
########################################################################################################
#include $(EHS_TARGET_COMPONENT_HAL_PATH)/hash_alg/hash_alg.mk

