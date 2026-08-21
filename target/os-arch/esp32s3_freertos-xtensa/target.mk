#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Makefile fragment to build target specific component and support code.
# Called by ../../platform/<platform-type>

#  OBJ - File extension for object files

#os-arch-wide platform component-HAL settings:
ifndef EHS_PERIPHERALS_GPIO_SUPPORT
EHS_PERIPHERALS_GPIO_SUPPORT=ESP32S3_IDF
endif
ifndef EHS_PERIPHERALS_ADC_DAC_SUPPORT
EHS_PERIPHERALS_ADC_DAC_SUPPORT=ESP32S3_IDF
endif

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)

# Default OS Features Supported
ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifndef EHS_FILESYSTEM_SUPPORT
	EHS_FILESYSTEM_SUPPORT=posix
endif
endif

# target path
_TARGET_PATH = $(EHS_TARGETS_ROOT_PATH)/os-arch/esp32s3_freertos-xtensa/

# include sourcecode from this dir in build
# We do not need this
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/freertos_esp32s3-xtensa/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/freertos_esp32s3-xtensa/

# Always on parameters
EHS_COMMS_TASK=tcp_server_common
EHS_COMMS_API_SUPPORT=lwip
EHS_ESP32_SUPPORT=1
DEFS += EHS_ESP32_SUPPORT=1

# Export ESP32_* partition variables from config.mk into targetenv_esp32.sh's
# subshell. When ESP32_FLASH_SIZE is set, targetenv_esp32.sh generates
# partitions.csv + partition-table.bin locally from these vars and derives
# merge_bin offsets from the resulting table. When unset, the legacy
# ert-contrib-middleware-supplied partition table is used unchanged.
export ESP32_FLASH_SIZE
export ESP32_PARTITION_TABLE_OFFSET
export ESP32_PART_NVS_SIZE
export ESP32_PART_PHY_INIT_SIZE
export ESP32_PART_OTA_ENABLED
export ESP32_PART_OTA_SIZE
export ESP32_PART_FACTORY_SIZE
export ESP32_PART_STORAGE_SIZE
export ESP32_PART_APPDATA_SIZE

include TARGET.cfg
DEFS += 'TARGET_OS_VERSION_STRING="$(shell head -c -1 ./Releases/version_strings | tr '\n' '.')\x20:$(TARGET)"'

# Enable gdb debugging by default
# TODO2026 do we want this as a default? Does it add memory or CPU overhead?
ENABLE_GDB=1 

# Use lwip
DEFS += EHS_LWIP=1
#INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/lwip/
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/Component-HAL/comms/lwip
ifdef EHS_NETWORK_WIFI_SUPPORT
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/Component-HAL/wifi
INC_DIRS +=$(EHS_TARGETS_ROOT_PATH)/os-arch/freertos_esp32s3-xtensa/examples/wifi_test
endif
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/hal/

#IDF build has som specific subdirectories:
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/esp_additions/freertos
#some things reference with freertos/ and others don't ...
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/esp_additions/
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/components/mqtt/esp-mqtt
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/apps/
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/apps/ping/

# include next folder
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/include_next
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/esp32s3/

# Always needed Objects doe esp32s
OBJECTS += target_file.$(OBJ)
OBJECTS += target_math.$(OBJ)
OBJECTS += targetos_init.$(OBJ)
OBJECTS += target_process.$(OBJ)
OBJECTS += target_main.$(OBJ)
OBJECTS += target_time.$(OBJ)
OBJECTS += ping.$(OBJ)
OBJECTS += target_sys_stat.$(OBJ)

# Optionally needed, depening on platform build
# target_uart.$(OBJ) is now added by uart_common.mk via EHS_PERIPHERALS_UART_SUPPORT.

ifdef EHS_NETWORK_WIFI_SUPPORT
ifneq ($(EHS_NETWORK_WIFI_SUPPORT),none)
OBJECTS += target_wifi.${OBJ}
DEFS += EHS_NETWORK_WIFI_SUPPORT
endif
endif

ifdef EHS_NETWORK_ETHERNET_SUPPORT
ifneq ($(EHS_NETWORK_ETHERNET_SUPPORT),none)
OBJECTS += target_ethernet.${OBJ}
DEFS += EHS_NETWORK_ETHERNET_SUPPORT
endif
endif
# We deal with MODBUS specifically for ESP32 IDF because it has a native implementation.
# otherwise this should have gone in the target/Component-HAL/ under the appropriate directory.
ifdef EHS_MODBUS_SUPPORT
ifneq ($(EHS_MODBUS_SUPPORT),none)
OBJECTS += target_mbport.$(OBJ)
DEFS += EHS_MODBUS_SUPPORT
endif
endif

ifdef EHS_GUI_SUPPORT
ifneq ($(EHS_GUI_SUPPORT),none)
OBJECTS += target_display.$(OBJ)
DEFS += EHS_GUI_SUPPORT
endif
endif

# Per-target serial-console HAL — backs the cross-platform console code in
# Common/Ehs/serial_console.c. Contract in Common/HAL/include/hal_serial.h.
ifdef EHS_SERIAL_CONSOLE_SUPPORT
ifneq ($(EHS_SERIAL_CONSOLE_SUPPORT),none)
OBJECTS += target_serial.$(OBJ)
DEFS += EHS_SERIAL_CONSOLE_SUPPORT
endif
endif

# IF there are some data partition sources present then add them too 
ifneq (,$(wildcard $(_TARGET_PATH)/target_data_bin.c))
OBJECTS += target_data_bin.${OBJ}
else
OBJECTS += target_data_bin_default.$(OBJ)
endif

ifdef EHS_I2C_SUPPORT
ifneq ($(EHS_I2C_SUPPORT),none)	
DEFS += EHS_I2C_SUPPORT
OBJECTS += target_specific.$(OBJ)
endif
endif

#todo2025 - still not sure how we should be doing this:
EHS_PERIPHERALS_BACKLIGHT_SUPPORT=esp32s3

ifdef FIXED_ESP32_LINKLINE_USING_STARTGROUP
LIB += coexist
LIB += espnow
#LIB += mesh
LIB += net80211
LIB += phy
LIB += pp
#LIB += rtc
LIB += smartconfig
LIB += wapi
LIB += xt_hal

LIB += app_trace
LIB += app_update
LIB += bootloader_support
LIB += bt
LIB += coap
LIB += console
LIB += driver
LIB += efuse
#LIB += esp32s3
LIB += esp_adc_cal
LIB += esp_common
LIB += esp_eth
LIB += esp_event
LIB += esp_gdbstub
LIB += esp_hid
LIB += esp_http_client
LIB += esp_http_server
LIB += esp_https_ota
LIB += esp_hw_support
LIB += esp_ipc
LIB += esp_lcd
LIB += esp_local_ctrl
LIB += esp_netif
LIB += esp_phy
LIB += esp_pm
LIB += esp_ringbuf
LIB += esp_rom
LIB += mqtt
#LNKFLAGS+= -lesp_serial_slave_link -lesp_system -lesp_timer -lesp-tls -lesp_wifi -lexpat -lfatfs -lfreemodbus -lfreertos -lhal -lheap -lidf_test -ljsmn -ljson -llibsodium 

LIB += esp_serial_slave_link
LIB += esp_system
LIB += esp_timer
LIB += esp-tls
LIB += esp_wifi
LIB += expat
LIB += fatfs
LIB += freemodbus
LIB += freertos
LIB += hal
LIB += heap
#LIB += idf_test
LIB += jsmn
LIB += json
LIB += libsodium
#LIB += esp_psram

#LNKFLAGS+= -lnvs_flash -lopenssl -lopenthread -lperfmon -lprotobuf-c -lprotocomm -lpthread -lsdmmc -lsoc -lspiffs -lspi_flash -ltcpip_adapter -ltcp_transport -llog -llwip -lmbedtls -lmdns -lmqtt -lnewlib -lnghttp 


LIB += log
LIB += lwip
LIB += mbedtls
LIB += mdns
LIB += newlib
LIB += nghttp
LIB += nvs_flash
LIB += openssl
LIB += openthread
LIB += perfmon
LIB += protobuf-c
LIB += protocomm
LIB += pthread
LIB += sdmmc
LIB += soc
LIB += spiffs
LIB += spi_flash
LIB += tcpip_adapter
LIB += tcp_transport
LIB += ulp
LIB += unity
LIB += vfs
LIB += wear_levelling
LIB += wifi_provisioning
LIB += wpa_supplicant
LIB += xtensa
LIB += cxx
LIB += esp_littlefs
endif

#LNKFLAGS+= -lulp -lunity -lvfs -lwear_levelling -lwifi_provisioning -lwpa_supplicant -lxtensa


# Task watchdog. Off by default: app_main calls esp_task_wdt_deinit() so a blocked or
# spinning task hangs silently. Set EHS_TARGET_TASK_WATCHDOG=yes in config.mk to keep it
# running instead, so a hang panics with a backtrace naming the stuck task. Timeout
# defaults to 8 s in target_main.c; override EHS_TARGET_TASK_WATCHDOG_MS if needed.
ifeq ($(EHS_TARGET_TASK_WATCHDOG),yes)
DEFS += EHS_TARGET_TASK_WATCHDOG
endif
