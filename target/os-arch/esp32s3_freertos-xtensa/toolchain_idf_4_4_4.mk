#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
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

#The following are only needed for buildig the contributed esp32 libraries in the ert-contrib-middleware context. Presume we don't need them here for now.
# TODO remove this when we know that everything works
CFLAGS+= -std=gnu99 -Og -ggdb -Wno-frame-address -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -mlongcalls -nostdlib -fno-jump-tables -fno-tree-switch-conversion -Wall 
#CFLAGS+= -Werror=all -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare 
#CFLAGA+= -Wno-old-style-declaration 
CFLAGS+= -DESP_PLATFORM -D IDF_VER=\"v4.4.4\" -MMD -MP 
#CFLAGS+= -D_GNU_SOURCE 
CFLAGS+= -DUNITY_INCLUDE_CONFIG_H -DMBEDTLS_CONFIG_FILE='"mbedtls/esp_config.h"' -DHAVE_CONFIG_H -DUNITY_INCLUDE_CONFIG_H

# Temporary test, may not need it later.
#CFLAGS+= -std=gnu99 -I /home/inx/fast-workspace/INX/inxware/ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/components/newlib/platform_include -I /home/inx/fast-workspace/INX/inxware/ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/build/include

#LNKFLAGS+= -nostdlib -Wl,--gc-sections -Wl,-static -Wl,--start-group -T esp32.rom.ld -T esp32.rom.libgcc.ld -T esp32.rom.syscalls.ld -T esp32.rom.newlib-data.ld -T esp32.rom.api.ld -T esp32.rom.newlib-funcs.ld -T esp32.rom.newlib-time.ld -Wl,--wrap=longjmp -lesp_rom  -lnewlib -lm -u newlib_include_heap_impl -u newlib_include_syscalls_impl -u newlib_include_pthread_impl -u newlib_include_assert_impl -lgcc -lstdc++ -lgcov -Wl,--end-group -Wl,-EL
#todo2022 the following link  command uses start group to avoid using -Wl on each link. 
#We may want ocosiderthis approach generally for handling libraries included via the normal LIB+= method, but for now we'll do it explcitly here and remove the LIB+ stuff that duplicates this in the target.mk file.
# TODO find a better way to do the linking stage with the groups.
LNKFLAGS+= -nostdlib -Wl,--gc-sections -Wl,-static -Wl,--start-group -lapp_update -u esp_app_desc -Wl,--wrap=_Unwind_SetEnableExceptionFdeSorting -Wl,--wrap=__register_frame_info_bases -Wl,--wrap=__register_frame_info -Wl,--wrap=__register_frame -Wl,--wrap=__register_frame_info_table_bases -Wl,--wrap=__register_frame_info_table -Wl,--wrap=__register_frame_table -Wl,--wrap=__deregister_frame_info_bases -Wl,--wrap=__deregister_frame_info -Wl,--wrap=_Unwind_Find_FDE -Wl,--wrap=_Unwind_GetGR -Wl,--wrap=_Unwind_GetCFA -Wl,--wrap=_Unwind_GetIP -Wl,--wrap=_Unwind_GetIPInfo -Wl,--wrap=_Unwind_GetRegionStart -Wl,--wrap=_Unwind_GetDataRelBase -Wl,--wrap=_Unwind_GetTextRelBase -Wl,--wrap=_Unwind_SetIP -Wl,--wrap=_Unwind_SetGR -Wl,--wrap=_Unwind_GetLanguageSpecificData -Wl,--wrap=_Unwind_FindEnclosingFunction -Wl,--wrap=_Unwind_Resume -Wl,--wrap=_Unwind_RaiseException -Wl,--wrap=_Unwind_DeleteException -Wl,--wrap=_Unwind_ForcedUnwind -Wl,--wrap=_Unwind_Resume_or_Rethrow -Wl,--wrap=_Unwind_Backtrace -Wl,--wrap=__cxa_call_unexpected -Wl,--wrap=__gxx_personality_v0 -u __cxx_fatal_exception 
LNKFLAGS+= -T esp32s3.rom.ld -T esp32s3.rom.libgcc.ld -T esp32s3.rom.newlib.ld -T esp32s3.rom.newlib-nano.ld -T esp32s3.rom.api.ld -T esp32s3.rom.version.ld -T esp32s3.rom.newlib-time.ld -lesp_rom -Wl,--wrap=longjmp -lesp_system -u __ubsan_include -u ld_include_highint_hdl -T memory.ld -T sections.ld -lfreertos -Wl,--undefined=uxTopUsedPriority -lpthread -u pthread_include_pthread_impl -u pthread_include_pthread_cond_impl -u pthread_include_pthread_local_storage_impl -u pthread_include_pthread_rwlock_impl -lnewlib -lc -lm -u newlib_include_heap_impl -u newlib_include_syscalls_impl -u newlib_include_pthread_impl -u newlib_include_assert_impl -lxt_hal -lcore -lnet80211 -lpp -lsmartconfig -lcoexist -lespnow -lmesh -lgcc -lstdc++ -lgcov -T esp32s3.peripherals.ld  
# todo2022 These shoulld be done properly using the LINK+ vriable but this seems to slightly broken: 
#LNKFLAGS+= -lcoexist -lespnow -lmesh -lnet80211 -lphy -lpp -lrtc -lsmartconfig -lwapi -lxt_hal -lapp_trace -lapp_update -lbootloader_support -lbt -lcoap -lconsole -ldriver -lefuse -lesp32s3 -lesp_adc_cal -lesp_common 
LNKFLAGS+= -lcoexist -lespnow -lmesh -lnet80211 -lphy -lpp -lsmartconfig -lwapi -lxt_hal -lapp_trace -lapp_update -lbootloader_support -lbt -lcoap -lconsole -ldriver -lefuse -lesp_adc_cal -lesp_common 
LNKFLAGS+= -lesp_eth -lesp_event -lesp_gdbstub -lesp_hid -lesp_http_client -lesp_http_server -lesp_https_ota -lesp_hw_support -lesp_ipc -lesp_lcd -lesp_local_ctrl -lesp_netif -lesp_phy -lesp_pm -lesp_ringbuf -lesp_rom 
#LNKFLAGS+= -lesp_serial_slave_link -lesp_system -lesp_timer -lesp-tls -lesp_wifi -lexpat -lfatfs -lfreemodbus -lfreertos -lhal -lheap -lidf_test -ljsmn -ljson -llibsodium 
LNKFLAGS+= -lesp_serial_slave_link -lesp_system -lesp_timer -lesp-tls -lesp_wifi -lexpat -lfatfs -lfreemodbus -lfreertos -lhal -lheap -ljsmn -ljson -llibsodium 
LNKFLAGS+= -lnvs_flash -lopenssl -lopenthread -lperfmon -lprotobuf-c -lprotocomm -lpthread -lsdmmc -lsoc -lspiffs -lspi_flash -ltcpip_adapter -ltcp_transport -llog -llwip -lmbedtls -lmdns -lmqtt -lnewlib -lnghttp 
#LNKFLAGS+= -llvgl -llvgl_esp32_drivers
LNKFLAGS+= -lulp -lunity -lvfs -lwear_levelling -lwifi_provisioning -lwpa_supplicant -lxtensa -lhal -lesp_littlefs -Wl,--end-group -Wl,-EL -fno-rtti
LIB+=c
LIB+=cxx
LIB_DIRS += $(EHS_ROOT_PATH)

LDFLAGS = -static
EXE=elf

# use the usual gcc/clang tool chain config
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk # toolchain is gnu

#LNKFLAGS+=-L$(EHS_ROOT_PATH)/../ert-build-support/toolchains/x86_64/xtensa-esp32s3-elf-4.4.4/xtensa-esp32s3-elf/lib -lc
LNKFLAGS+= -Wl,-lc

