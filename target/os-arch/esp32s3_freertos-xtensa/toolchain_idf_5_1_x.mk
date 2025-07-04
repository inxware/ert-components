#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#  Uses $(INC_DIRS), $(TARGET_NAME), $(DEFS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)
#

#
# Toolset specific 


#Setup the toolchain path
#defaulting to HOST gcc
ifndef CC_OVERRIDE
   CC_OVERRIDE:=xtensa-esp32s3-elf-gcc
endif

ifndef AS_OVERRIDE
   AS_OVERRIDE:=xtensa-esp32s3-elf-gcc
endif

ifndef LINK_OVERRIDE
    LINK_OVERRIDE:=xtensa-esp32s3-elf-gcc
endif

CFLAGS += -I$(EHS_ROOT_PATH)/../ert-contrib-middleware/target_libs/xtensa-esp32s3_freertos-xtensa-esp32s3-elf-5.1/build/include/deprecated
ifndef ESP32S3_DEBUG_BUILD # non-debug - optimise for size
CFLAGS += -std=gnu17 -Os -ggdb -Wno-frame-address -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -mlongcalls -nostdlib -fno-jump-tables -fno-tree-switch-conversion -Wall
CFLAGS += -DNDEBUG
else
CFLAGS += -std=gnu17 -Og -ggdb -Wno-frame-address -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -mlongcalls -nostdlib -fno-jump-tables -fno-tree-switch-conversion -Wall
endif
CFLAGS += -DESP_PLATFORM -D IDF_VER=\"v5.1.2\" -MMD -MP
CFLAGS += -DUNITY_INCLUDE_CONFIG_H -DMBEDTLS_CONFIG_FILE='"mbedtls/esp_config.h"' -DHAVE_CONFIG_H -DUNITY_INCLUDE_CONFIG_H

CFLAGS += -DSOC_MMU_PAGE_SIZE=CONFIG_MMU_PAGE_SIZE -DUNITY_INCLUDE_CONFIG_H -D_GNU_SOURCE -D_POSIX_READER_WRITER_LOCKS

LNKFLAGS += -mlongcalls  -Wl,--cref -Wl,--defsym=IDF_TARGET_ESP32S3=0 -Wl,--Map=$(EHS_ROOT_PATH)/main.map -Wl,--no-warn-rwx-segments -fno-rtti -fno-lto -Wl,--gc-sections -Wl,--warn-common -T esp32s3.peripherals.ld -T esp32s3.rom.ld -T esp32s3.rom.api.ld -T esp32s3.rom.libgcc.ld -T esp32s3.rom.newlib.ld -T esp32s3.rom.version.ld 
LNKFLAGS += -T memory.ld -T sections.ld -lxtensa -lesp_ringbuf -lefuse -ldriver -lesp_pm -lmbedtls -lesp_app_format -lbootloader_support -lesp_partition -lapp_update -lesp_mm -lspi_flash  
LNKFLAGS += -lpthread -lesp_system -lesp_rom -lhal -llog -lheap -lsoc -lesp_hw_support -lfreertos -lnewlib -lcxx -lesp_common -lesp_timer -lapp_trace -lesp_event  
LNKFLAGS += -lnvs_flash -lesp_phy -lvfs -llwip -lesp_netif -lwpa_supplicant -lesp_coex -lesp_wifi -lunity -lcmock -lconsole -lhttp_parser -lesp-tls -lesp_adc -lesp_eth  
LNKFLAGS += -lesp_gdbstub -lesp_hid -ltcp_transport -lesp_http_client -lesp_http_server -lesp_https_ota
ifeq ($(COMPONENT_VARIANT), n8r2)
LNKFLAGS += -lesp_psram
endif
LNKFLAGS += -lesp_lcd -lprotobuf-c -lprotocomm -lesp_local_ctrl -lespcoredump -lwear_levelling
LNKFLAGS += -lsdmmc -lfatfs -ljson -lmqtt -lperfmon -lspiffs -ltouch_element -lulp -lusb -lwifi_provisioning -lexpat -llittlefs
ifeq ($(COMPONENT_VARIANT), n8r2)
LNKFLAGS += -lesp_psram
endif
LNKFLAGS += -lapp_trace -lapp_trace -lcmock -lunity -lesp_hid -lesp_lcd -lesp_local_ctrl -lespcoredump -lfatfs -lwear_levelling -lsdmmc -lmqtt -lperfmon -lspiffs  
LNKFLAGS += -ltouch_element -lusb -lwifi_provisioning -lprotocomm -lconsole -lprotobuf-c -ljson -lexpat -llittlefs -lxtensa -lesp_ringbuf -lefuse -ldriver -lesp_pm -lmbedtls  
LNKFLAGS += -lesp_app_format -lbootloader_support -lesp_partition -lapp_update -lesp_mm -lspi_flash -lpthread -lesp_system -lesp_rom -lhal -llog -lheap -lsoc -lesp_hw_support -lfreertos -lnewlib -lcxx -lesp_common -lesp_timer -lesp_event
LNKFLAGS += -lnvs_flash -lesp_phy -lvfs -llwip -lesp_netif -lwpa_supplicant -lesp_coex -lesp_wifi -lhttp_parser -lesp-tls -lesp_adc -lesp_eth -lesp_gdbstub -ltcp_transport -lesp_http_client -lesp_http_server -lesp_https_ota
ifeq ($(COMPONENT_VARIANT), n8r2)
LNKFLAGS += -lesp_psram
endif
LNKFLAGS += -lulp  

#LNKFLAGS += mbedtls/library/libmbedtls  mbedtls/library/libmbedcrypto  mbedtls/library/libmbedx509  mbedtls/3rdparty/everest/libeverest  mbedtls/3rdparty/p256-m/libp256m  
LNKFLAGS += -lmbedtls #-leverest -lp256m

LNKFLAGS += -lcore -lespnow -lmesh -lnet80211 -lpp -lsmartconfig -lwapi -lxtensa -lesp_ringbuf -lefuse -ldriver -lesp_pm -lmbedtls -lesp_app_format -lbootloader_support -lesp_partition -lapp_update -lesp_mm -lspi_flash -lpthread -lesp_system
LNKFLAGS += -lesp_rom -lhal -llog -lheap -lsoc -lesp_hw_support -lfreertos -lnewlib -lcxx -lesp_common -lesp_timer -lesp_event -lnvs_flash -lesp_phy -lvfs -llwip -lesp_netif -lwpa_supplicant -lesp_coex -lesp_wifi -lhttp_parser -lesp-tls  
LNKFLAGS += -lesp_adc -lesp_eth -lesp_gdbstub -ltcp_transport -lesp_http_client -lesp_http_server -lesp_https_ota
ifeq ($(COMPONENT_VARIANT), n8r2)
LNKFLAGS += -lesp_psram
endif
LNKFLAGS += -lulp  

#LNKFLAGS += mbedtls/library/libmbedtls  mbedtls/library/libmbedcrypto  mbedtls/library/libmbedx509  mbedtls/3rdparty/everest/libeverest  mbedtls/3rdparty/p256-m/libp256m  
LNKFLAGS += -lmbedtls #-leverest -lp256m

LNKFLAGS += -lcore -lespnow -lmesh -lnet80211 -lpp -lsmartconfig -lwapi -lxtensa -lesp_ringbuf -lefuse -ldriver -lesp_pm -lmbedtls -lesp_app_format -lbootloader_support -lesp_partition -lapp_update -lesp_mm -lspi_flash -lpthread -lesp_system  
LNKFLAGS += -lesp_rom -lhal -llog -lheap -lsoc -lesp_hw_support -lfreertos -lnewlib -lcxx -lesp_common -lesp_timer -lesp_event -lnvs_flash -lesp_phy -lvfs -llwip -lesp_netif -lwpa_supplicant -lesp_coex -lesp_wifi -lhttp_parser -lesp-tls  
LNKFLAGS += -lesp_adc -lesp_eth -lesp_gdbstub -ltcp_transport -lesp_http_client -lesp_http_server -lesp_https_ota
LNKFLAGS += -lulp  

#mbedtls/library/libmbedtls  mbedtls/library/libmbedcrypto  mbedtls/library/libmbedx509  mbedtls/3rdparty/everest/libeverest  mbedtls/3rdparty/p256-m/libp256m 
LNKFLAGS += -lmbedtls #-leverest -lp256m

ifeq ($(COMPONENT_VARIANT), n8r2)
LNKFLAGS += -lesp_psram -lesp_psram -lesp_psram
endif

LNKFLAGS += -lcore -lespnow -lmesh -lnet80211 -lpp -lsmartconfig -lwapi  
LNKFLAGS += -lxt_hal  -u esp_app_desc  -u pthread_include_pthread_impl  -u pthread_include_pthread_cond_var_impl  -u pthread_include_pthread_local_storage_impl  -u pthread_include_pthread_rwlock_impl  -u pthread_include_pthread_semaphore_impl  -u ld_include_highint_hdl  -u start_app  -u start_app_other_cores  -u __ubsan_include  -Wl,--wrap=longjmp -u abort  -u __assert_func  -Wl,--undefined=uxTopUsedPriority  -Wl,--undefined=FreeRTOS_openocd_params  -u app_main  -lc  -lm
LNKFLAGS += -lnewlib  -u newlib_include_heap_impl  -u newlib_include_syscalls_impl  -u newlib_include_pthread_impl  -u newlib_include_assert_impl  -Wl,--wrap=_Unwind_SetEnableExceptionFdeSorting  -Wl,--wrap=__register_frame_info_bases  -Wl,--wrap=__register_frame_info  -Wl,--wrap=__register_frame  -Wl,--wrap=__register_frame_info_table_bases  -Wl,--wrap=__register_frame_info_table  -Wl,--wrap=__register_frame_table  -Wl,--wrap=__deregister_frame_info_bases  -Wl,--wrap=__deregister_frame_info  -Wl,--wrap=_Unwind_Find_FDE  -Wl,--wrap=_Unwind_GetGR  -Wl,--wrap=_Unwind_GetCFA  -Wl,--wrap=_Unwind_GetIP  -Wl,--wrap=_Unwind_GetIPInfo  -Wl,--wrap=_Unwind_GetRegionStart  -Wl,--wrap=_Unwind_GetDataRelBase  -Wl,--wrap=_Unwind_GetTextRelBase  -Wl,--wrap=_Unwind_SetIP  -Wl,--wrap=_Unwind_SetGR  -Wl,--wrap=_Unwind_GetLanguageSpecificData  -Wl,--wrap=_Unwind_FindEnclosingFunction  -Wl,--wrap=_Unwind_Resume  -Wl,--wrap=_Unwind_RaiseException  -Wl,--wrap=_Unwind_DeleteException  -Wl,--wrap=_Unwind_ForcedUnwind  -Wl,--wrap=_Unwind_Resume_or_Rethrow  -Wl,--wrap=_Unwind_Backtrace  -Wl,--wrap=__cxa_call_unexpected  -Wl,--wrap=__gxx_personality_v0  -u __cxa_guard_dummy  -lstdc++ -lpthread  -lgcc -lcxx  -u __cxx_fatal_exception  -u include_esp_phy_override  -lphy  -lbtbb -lesp_phy  -lphy  -lbtbb -lesp_phy  -lphy  -lbtbb  -u vfs_include_syscalls_impl

LIB_DIRS += $(EHS_ROOT_PATH)

LDFLAGS = -static
EXE=elf

# use the usual gcc/clang tool chain config
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk # toolchain is gnu

LNKFLAGS+= -Wl,-lc

