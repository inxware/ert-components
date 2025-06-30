#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

include $(EHS_TARGETS_ROOT_PATH)/os-arch/arduino_ALL/target.mk
# HW independent linux build environment parameters 

#target types are always the same for all arduino targets so just use one file
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/arduino_ALL/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/arduino_ALL/

INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/Component-HAL/comms/arduino

INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/variants/NANO_RP2040_CONNECT
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/api/deprecated
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/api/deprecated-avr-comp

CPPFLAGS += -fpermissive -w -g3 -nostdlib -DARDUINO_ARCH_RP2040 -MMD -mcpu=cortex-m0plus -DARDUINO=10607 -DARDUINO_NANO_RP2040_CONNECT -DARDUINO_ARCH_MBED_NANO -DARDUINO_ARCH_MBED -DARDUINO_LIBRARY_DISCOVERY_PHASE=0
CFLAGS += -fpermissive -w -g3 -nostdlib -DARDUINO_ARCH_RP2040 -MMD -mcpu=cortex-m0plus -DARDUINO=10607 -DARDUINO_NANO_RP2040_CONNECT -DARDUINO_ARCH_MBED_NANO -DARDUINO_ARCH_MBED -DARDUINO_LIBRARY_DISCOVERY_PHASE=0

CFLAGS += @$(EHS_COMPONENT_SUPPORT_INCLUDE)/variants/NANO_RP2040_CONNECT/defines.txt
CPPFLAGS += @$(EHS_COMPONENT_SUPPORT_INCLUDE)/variants/NANO_RP2040_CONNECT/defines.txt

CFLAGS += @$(EHS_COMPONENT_SUPPORT_INCLUDE)/variants/NANO_RP2040_CONNECT/cflags.txt 
CPPFLAGS += @$(EHS_COMPONENT_SUPPORT_INCLUDE)//variants/NANO_RP2040_CONNECT/cxxflags.txt 

CFLAGS += -iprefix$(EHS_COMPONENT_SUPPORT_INCLUDE) @$(EHS_COMPONENT_SUPPORT_INCLUDE)/variants/NANO_RP2040_CONNECT/includes.txt
CPPFLAGS += -iprefix$(EHS_COMPONENT_SUPPORT_INCLUDE) @$(EHS_COMPONENT_SUPPORT_INCLUDE)/variants/NANO_RP2040_CONNECT/includes.txt

# Include Arduino headerfiles installed via docker
ifeq ($(EHS_COMMS_API_SUPPORT),arduino_nina)
# arduino-cli lib install "WiFiNINA@1.9.0"
INC_DIRS += /home/inxware/Arduino/libraries/WiFiNINA/src
endif

#TODO2025 - Some of these should be moved to the Component HAL

ifeq ($(EHS_MQTT_SUPPORT),arduino)
# arduino-cli lib install ArduinoMqttClient
INC_DIRS += /home/inxware/Arduino/libraries/ArduinoMqttClient/src
endif

ifeq ($(EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT),arduino)
# arduino-cli lib install Arduino_LSM6DS3@1.0.3
# these are installed in the docker image
INC_DIRS += /home/inxware/Arduino/libraries/Arduino_LSM6DS3/src
INC_DIRS += /home/inxware/.arduino15/packages/arduino/hardware/mbed_nano/4.2.2/libraries/Wire
INC_DIRS += /home/inxware/.arduino15/packages/arduino/hardware/mbed_nano/4.2.2/libraries/SPI
endif

ifndef EHS_PLUGIN_LIBRARY_DEPENDENCY
# this cannot be used when building as a static library
LNKFLAGS += -Wl,--gc-sections -w -Wl,--as-needed @$(EHS_COMPONENT_SUPPORT_INCLUDE)/variants/NANO_RP2040_CONNECT/ldflags.txt 
LNKFLAGS += -Wl,-Map,main.map --specs=nosys.specs
LNKFLAGS += -Wl,--whole-archive $(EHS_COMPONENT_SUPPORT_LIBS)/libcore.a $(EHS_COMPONENT_SUPPORT_LIBS)/libmisc.a  $(EHS_COMPONENT_SUPPORT_LIBS)/libmbed.a -Wl,--no-whole-archive
LNKFLAGS += -Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys -Wl,--end-group
endif

# Add traget specific support
ifeq ($(EHS_PERIPHERALS_LED_SUPPORT),arduino_nina)
OBJECTS += target_led_nina.$(OBJ)
endif