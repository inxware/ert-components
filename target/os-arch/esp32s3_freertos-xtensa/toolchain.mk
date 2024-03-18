#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

ifeq ($(TOOLCHAIN_NAME),xtensa-esp32s3-elf-5.1)
# add v5.1 specific configuration
include $(EHS_TARGETS_ROOT_PATH)/os-arch/esp32s3_freertos-xtensa/toolchain_idf_5_1_x.mk
else ifeq ($(TOOLCHAIN_NAME),xtensa-esp32s3-elf-4.4.4)
# add v4.4.4 specific configuration
CFLAGS += -DUSE_ESP32S3_LEGACY_API=1
# v4.4.4 doesn't support a proper continious adc api, so we just stub the pid
ifdef EHS_PID_SUPPORT
EHS_PID_SUPPORT=stubbed
endif
include $(EHS_TARGETS_ROOT_PATH)/os-arch/esp32s3_freertos-xtensa/toolchain_idf_4_4_4.mk
endif

