#
# Makefile fragment to build target specific component and support code.

# Called by INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/platform/<platform-type>
#

# @author: inx limited
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
#
# Copyright (c) inx limited, 2007. All rights reserved.
#
#
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code

# include sourcecode from this dir in build

target_math.$(OBJ) : target.h

target_process.$(OBJ) : target.h

target_time.$(OBJ) : target.h

target_file.$(OBJ) : target.h

target_gpio.$(OBJ) : target.h

OBJECTS += target_math.$(OBJ) 
OBJECTS += target_process.$(OBJ)
OBJECTS += target_time.$(OBJ)
OBJECTS += target_file.$(OBJ)
OBJECTS += target_gpio.$(OBJ)

export HEATROD_CONTROLLER_DIR = $(EHS_TARGETS_ROOT_PATH)/os-arch/nxp_arm/ehs
INCLUDE_DIRECTORIES += $(EHS_TARGETS_ROOT_PATH)/os-arch/nxp_arm
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/board
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/modbus/ascii
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/modbus/rtu
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/IEC60730_Kinetis_CM4_CM7_Class_B
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/source
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/modbus/include
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src/apps/httpsrv
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/port/arch
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src/include/lwip
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src/include/lwip/priv
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src/include/lwip/prot
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src/include/netif
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src/include/netif/ppp
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src/include/netif/ppp/polarssl
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src/include/posix
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src/include/posix/sys
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src/include/lwip/apps
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/port
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/drivers
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/device
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/CMSIS
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/amazon-freertos/include
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/amazon-freertos/FreeRTOS/portable
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/utilities
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/component/serial_manager
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/component/uart
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/component/lists
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/lwip/src/include
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/mbedtls/include
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/mmcau_common
INCLUDE_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/Generated_Code
INCLUDE_DIRECTORIES += $(EHS_PLATFORM_PATH)
CFLAGS += $(addprefix -I ,$(INCLUDE_DIRECTORIES))

ASFLAGS += $(addprefix -I ,$(INCLUDE_DIRECTORIES))

LIBRARY_DIRECTORIES += $(HEATROD_CONTROLLER_DIR)/libs
LD_SWITCHES += $(addprefix -L ,$(LIBRARY_DIRECTORIES))

LNKFLAGS += -l_mmcau -l_iec60730

LNKFLAGS += -o $(TARGET_NAME).$(EXE)

include $(HEATROD_CONTROLLER_DIR)/utilities/utilities.mk
include $(HEATROD_CONTROLLER_DIR)/startup/startup.mk
include $(HEATROD_CONTROLLER_DIR)/source/utilities/src/src.mk
include $(HEATROD_CONTROLLER_DIR)/source/property/src/src.mk
include $(HEATROD_CONTROLLER_DIR)/source/memory/src/src.mk
include $(HEATROD_CONTROLLER_DIR)/source/crc/crc.mk
include $(HEATROD_CONTROLLER_DIR)/source/bootloader/src/src.mk
include $(HEATROD_CONTROLLER_DIR)/source/source.mk

include $(HEATROD_CONTROLLER_DIR)/modbus/tcp/tcp.mk
include $(HEATROD_CONTROLLER_DIR)/modbus/rtu/rtu.mk
include $(HEATROD_CONTROLLER_DIR)/modbus/port/port.mk
include $(HEATROD_CONTROLLER_DIR)/modbus/functions/functions.mk
include $(HEATROD_CONTROLLER_DIR)/modbus/ascii/ascii.mk
include $(HEATROD_CONTROLLER_DIR)/modbus/modbus.mk

include $(HEATROD_CONTROLLER_DIR)/mmcau_common/mmcau_common.mk
include $(EHS_TARGETS_ROOT_PATH)/Component-HAL/crypto/mbedtls/library/library.mk
include $(HEATROD_CONTROLLER_DIR)/lwip/src/netif/ppp/ppp.mk
include $(HEATROD_CONTROLLER_DIR)/lwip/src/netif/netif.mk
include $(HEATROD_CONTROLLER_DIR)/lwip/src/core/ipv6/ipv6.mk
include $(HEATROD_CONTROLLER_DIR)/lwip/src/core/ipv4/ipv4.mk
include $(HEATROD_CONTROLLER_DIR)/lwip/src/core/core.mk
include $(HEATROD_CONTROLLER_DIR)/lwip/src/apps/mdns/mdns.mk
include $(HEATROD_CONTROLLER_DIR)/lwip/src/apps/httpsrv/httpsrv.mk
include $(HEATROD_CONTROLLER_DIR)/lwip/src/api/api.mk
include $(HEATROD_CONTROLLER_DIR)/lwip/port/port.mk

include $(HEATROD_CONTROLLER_DIR)/drivers/drivers.mk
include $(HEATROD_CONTROLLER_DIR)/device/device.mk
include $(HEATROD_CONTROLLER_DIR)/component/uart/uart.mk
include $(HEATROD_CONTROLLER_DIR)/component/serial_manager/serial_manager.mk
include $(HEATROD_CONTROLLER_DIR)/component/lists/lists.mk
include $(HEATROD_CONTROLLER_DIR)/board/board.mk
include $(HEATROD_CONTROLLER_DIR)/amazon-freertos/FreeRTOS/portable/portable.mk
include $(HEATROD_CONTROLLER_DIR)/amazon-freertos/FreeRTOS/FreeRTOS.mk
include $(HEATROD_CONTROLLER_DIR)/IEC60730_Kinetis_CM4_CM7_Class_B/IEC60730_Kinetis_CM4_CM7_Class_B.mk
include $(HEATROD_CONTROLLER_DIR)/Generated_Code/Generated_Code.mk
