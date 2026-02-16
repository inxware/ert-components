#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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

########################################################################################################
## Wi-Fi
########################################################################################################
ifdef EHS_NETWORK_WIFI_SUPPORT
ifneq ($(EHS_NETWORK_WIFI_SUPPORT),none)
include $(EHS_TARGET_COMPONENT_HAL_PATH)/wifi/wifi.mk
endif
endif

ifdef EHS_NETWORK_ETHERNET_SUPPORT
ifneq ($(EHS_NETWORK_ETHERNET_SUPPORT),none)
DEFS += EHS_NETWORK_ETHERNET_SUPPORT=$(EHS_NETWORK_ETHERNET_SUPPORT)
endif
endif

########################################################################################################
## Serial Console (Used to configure networks etc)
########################################################################################################
ifdef EHS_SERIAL_CONSOLE_SUPPORT
ifneq ($(EHS_SERIAL_CONSOLE_SUPPORT),none)
DEFS += EHS_SERIAL_CONSOLE_SUPPORT
endif
endif

########################################################################################################
## BLE (Bluetooth Low Energy)
########################################################################################################
ifdef EHS_NETWORK_BLE_SUPPORT
ifneq ($(EHS_NETWORK_BLE_SUPPORT),none)
DEFS += EHS_NETWORK_BLE_SUPPORT
include $(EHS_TARGET_COMPONENT_HAL_PATH)/ble/ble.mk
endif
endif

########################################################################################################
## Machine Vision
########################################################################################################
ifdef EHS_MV_SUPPORT
ifneq ($(EHS_MV_SUPPORT),none)
include $(EHS_TARGET_COMPONENT_HAL_PATH)/mv/mv_common.mk
endif
endif

########################################################################################################
## Machine Learning and AI
########################################################################################################
ifdef EHS_ML_SUPPORT
ifneq ($(EHS_ML_SUPPORT),none)
include $(EHS_TARGET_COMPONENT_HAL_PATH)/ml/ml_common.mk
endif
endif

########################################################################################################
## Multi-Channel PID Controller
########################################################################################################

##todo 2025 This shouldn't be done here - i should be done in the PID make files #
## all this should go in a PID make file not here.
ifdef EHS_PID_SUPPORT
ifneq ($(EHS_PID_SUPPORT),none)
include $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/pid_common.mk
endif
endif

########################################################################################################
## OTA
########################################################################################################
# This is usally only support for chunked OTA (e.g. MQTT or LoRaWAN) 
ifdef EHS_OTA_SUPPORT
ifneq ($(EHS_OTA_SUPPORT),none)
  include $(EHS_TARGET_COMPONENT_HAL_PATH)/system/ota/target_hal_ota.mk
endif
endif

########################################################################################################
## MQTT
########################################################################################################
ifdef EHS_MQTT_SUPPORT
ifneq ($(EHS_MQTT_SUPPORT),none)  
include $(EHS_TARGET_COMPONENT_HAL_PATH)/mqtt/mqtt_common.mk
endif
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
ifneq ($(EHS_MODBUS_SUPPORT),none)
  include $(EHS_TARGET_COMPONENT_HAL_PATH)/modbus/modbus_common.mk
endif
endif

########################################################################################################
## RTC
########################################################################################################
ifdef EHS_RTC_SUPPORT
ifneq ($(EHS_RTC_SUPPORT),none)
DEFS += EHS_RTC_SUPPORT
  include $(EHS_TARGET_COMPONENT_HAL_PATH)/system/rtc/target_hal_rtc.mk
endif
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

########################################################################################################
## Image Processing
########################################################################################################
include $(EHS_TARGET_COMPONENT_HAL_PATH)/image_processing/image_processing.mk

########################################################################################################
## Watchdog Timer
########################################################################################################
include $(EHS_TARGET_COMPONENT_HAL_PATH)/watchdog/target_watchdog.mk

########################################################################################################
## Non-Volatile Storage (NVS)
########################################################################################################
include $(EHS_TARGET_COMPONENT_HAL_PATH)/nvs/hal_nvs.mk

