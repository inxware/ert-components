#
# Makefile fragment to build target specific component and support code.

# Called by ../../platform/<platform-type>
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
#  EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)

# include sourcecode from this dir in build
export EHS_SKIP_GNULIBRARIES=1
ifeq ($(HEATROD_CONTROLLER_PROJECT),1)
	include $(EHS_TARGETS_ROOT_PATH)/os-arch/nxp_arm/heatrod_controller.mk
	LD_SWITCHES += -T "heatrod_controller.ld"
else
	include $(EHS_TARGETS_ROOT_PATH)/os-arch/nxp_arm/ehs.mk
	LD_SWITCHES += -T "nxp_ehs.ld"
endif
ASFLAGS += -c -x assembler-with-cpp -D__REDLIB__

#CC_SWITCHES += -std=gnu99 -O0 -g3 -fno-common -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -specs=redlib.specs
CC_SWITCHES += -Wall -Werror -std=gnu99 -g0 -Os -c -fno-common -fno-builtin -ffunction-sections -fdata-sections -ffreestanding -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -specs=redlib.specs
LD_SWITCHES += -nostdlib
#LD_SWITCHES += -Xlinker -Map="heatrod_controller.map"

LD_SWITCHES += -Xlinker --defsym=ram_test_backup_size=0x20 -Xlinker --defsym=stack_test_block_size=0x20 -Xlinker --defsym=__size_cstack__=0x800
LD_SWITCHES += -Xlinker --defsym=m_ram_test_backup=__top_SRAM_UPPER-ram_test_backup_size+0x1
LD_SWITCHES += -Xlinker --defsym=m_wd_test_backup=m_ram_test_backup-0x10 -Xlinker --defsym=m_pc_test_flag=m_wd_test_backup-0x4
LD_SWITCHES += -Xlinker --defsym=m_safety_error_code=m_pc_test_flag-0x4 -Xlinker --defsym=m_stack_test_p_4=m_safety_error_code-0x4
LD_SWITCHES += -Xlinker --defsym=m_stack_test_p_3=m_stack_test_p_4-stack_test_block_size+0x4
LD_SWITCHES += -Xlinker --defsym=__BOOT_STACK_ADDRESS=m_stack_test_p_3-0x4 -Xlinker --defsym=m_stack_test_p_2=__BOOT_STACK_ADDRESS-__size_cstack__-0x4
LD_SWITCHES += -Xlinker --defsym=__ram_vector_table__=1 -Xlinker --gc-sections -Xlinker -print-memory-usage
LD_SWITCHES += -Xlinker --sort-section=alignment -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb

DEFS += __REDLIB__
DEFS += PRINTF_FLOAT_ENABLE=1
DEFS += CPU_MK64FN1M0VDC12
DEFS += CPU_MK64FN1M0VDC12_cm4
DEFS += _POSIX_SOURCE
DEFS += CPU_MK64FN1M0VLL12
DEFS += USE_RTOS=1
DEFS += PRINTF_ADVANCED_ENABLE=1
DEFS += FRDM_K64F
DEFS += FREEDOM
DEFS += FSL_RTOS_FREE_RTOS
DEFS += HTTPSRV_CFG_WEBSOCKET_ENABLED=1
DEFS += __MCUXPRESSO
DEFS += __USE_CMSIS
DEFS += NDEBUG
DEFS += MBEDTLS_CONFIG_FILE
DEFS += SDK_DEBUGCONSOLE=0