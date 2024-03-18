#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# Makefile fragment to build target specific component and support code.
# Called by ../../platform/<platform-type>

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)

# include sourcecode from this dir in build
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/freertos_esp32-xtensa/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/freertos_esp32-xtensa/

# Always on parameters
EHS_COMMS_TASK=tcp_server_common
EHS_COMMS_API_SUPPORT=lwip
EHS_ESP32_SUPPORT=1

#os-arch-wide platform component-HAL settings:
EHS_PERIPHERALS_GPIO_SUPPORT=ESP32_IDF
EHS_PERIPHERALS_ADC_DAC_SUPPORT=ESP32_IDF

#We always have MQTT support for ESP32, lets set it here for all esp32
EHS_MQTT_SUPPORT=esp32

#Enable gdb debugging by default
ENABLE_GDB=1 

# Use lwip
DEFS += EHS_LWIP=1
#INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/lwip/
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/Component-HAL/comms/lwip
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/hal/

#IDF build has som specific subdirectories:
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/esp_additions/freertos
#some things reference with freertos/ and others don't ...
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/esp_additions/
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/components/mqtt/esp-mqtt
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/apps/
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/apps/ping/
INC_DIRS +=$(EHS_TARGETS_ROOT_PATH)/os-arch/freertos_esp32-xtensa/examples/wifi_test

# The following will probably need to be ported eventually to support ert components and main build
OBJECTS += target_file.$(OBJ)
OBJECTS += target_math.$(OBJ)
OBJECTS += targetos_init.$(OBJ)
OBJECTS += target_process.$(OBJ)
OBJECTS += target_main.$(OBJ)
OBJECTS += target_time.$(OBJ)
OBJECTS += ping.$(OBJ)
#OBJECTS += wifi_test.$(OBJ)
#OBJECTS += target_math.$(OBJ) 
#OBJECTS += esp_main_example.$(OBJ) 
#OBJECTS += certificate.$(OBJ)
OBJECTS += mqtt.$(OBJ)

#expect we will need all of the lib*.a from ert-contrib-middleware/target_libs/..esp32 .. /build/lib/ here 
# LIB += ....



#LNKFLAGS+= -lcoexist -lespnow -lmesh -lnet80211 -lphy -lpp -lrtc -lsmartconfig -lwapi -lxt_hal -lapp_trace -lapp_update -lbootloader_support -lbt -lcoap -lconsole -ldriver 
#LNKFLAGS+= -lefuse -lesp32 -lesp_adc_cal -lesp_common -lesp_eth 

#LNKFLAGS+= -lulp -lunity -lvfs -lwear_levelling -lwifi_provisioning -lwpa_supplicant -lxtensa

#LNKFLAGS+= -lcoexist -lespnow -lmesh -lnet80211 -lphy -lpp -lrtc -lsmartconfig -lwapi -lxt_hal -lapp_trace -lapp_update -lbootloader_support -lbt -lcoap -lconsole -ldriver -lefuse -lesp32 -lesp_adc_cal -lesp_common 
#LNKFLAGS+= -lesp_eth -lesp_event -lesp_gdbstub -lesp_hid -lesp_http_client -lesp_http_server -lesp_https_ota -lesp_hw_support -lesp_ipc -lesp_lcd -lesp_local_ctrl -lesp_netif -lesp_phy -lesp_pm -lesp_ringbuf -lesp_rom 

# This is of course done for every platform LIB+=:libehs.a
ifdef FIXED_ESP32_LINKLINE_USING_STARTGROUP
LIB += coexist
LIB += espnow
#LIB += mesh
LIB += net80211
LIB += phy
LIB += pp
LIB += rtc
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
LIB += esp32
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
LIB += idf_test
LIB += jsmn
LIB += json
LIB += libsodium

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

