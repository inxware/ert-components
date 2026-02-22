#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
# default is using library built with 32k instruction cache

# This is already done for all GNU/Clang based toolchains CFLAGS += -c

ifeq ($(TOOLCHAIN_NAME),xtensa-esp32s3-elf-5.1)
    ESP32_TOOLCHAIN_MATCH = 1
endif
# this is using library built with 16k instruction cache
ifeq ($(TOOLCHAIN_NAME),xtensa-esp32s3-elf-5.1-16k)
    ESP32_TOOLCHAIN_MATCH = 1
endif

ifeq ($(ESP32_TOOLCHAIN_MATCH),1)
# add v5.1 specific configuration
include $(EHS_TARGETS_ROOT_PATH)/os-arch/esp32s3_freertos-xtensa/toolchain_idf_5_1_x.mk
else ifeq ($(TOOLCHAIN_NAME),xtensa-esp32s3-elf-4.4.4)
# add v4.4.4 specific configuration
CFLAGS += -DUSE_ESP32S3_LEGACY_API=1 -fshort-enums
# v4.4.4 doesn't support a proper continious adc api, so we just stub the pid
ifdef EHS_PID_SUPPORT
EHS_PID_SUPPORT=stubbed
endif
include $(EHS_TARGETS_ROOT_PATH)/os-arch/esp32s3_freertos-xtensa/toolchain_idf_4_4_4.mk
endif

