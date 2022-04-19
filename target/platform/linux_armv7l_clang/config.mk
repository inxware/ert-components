#
# config.mk - Configuration properties of the current platform
# 
# Called by ../../../Makefile
#

# @author: inx limited, Pierre Drezet
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
# 
# Copyright (c) inx limited, 2007. All rights reserved.
#
#

#DEBUG OPTIONS
#EHS_DEBUGALL=true
ifdef EHS_DEBUGALL
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
export EHS_DEBUG=yes
endif
DEFS += EHS_DEBUG_TCPIP_CONSOLE
DEFS += EHS_MQTT_SUPPORT
export EHS_MQTT_SUPPORT=yes

################################################################################################################
# Define the specific variant of the architecture and OS - this selects different component support library sets
################################################################################################################

# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
#export SYSTEM_VARIANT=

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
export COMPONENT_VARIANT=base

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
export EHS_GNU_ARCH=arm
export EHS_GNU_OS=linux-gnu
export EHS_GNU_OS_VERSION=-clang10ubuntu18
#use toolchain clib environment
#Optional if different clib build is required - dangerous!
#export EHS_GNU_CLIB_ARCH_OVERRIDE=i686
export KERNEL_VERSION=linux/2.6.35.9

# EHS Section 
# ehs is more generic
export EHS_ARCH=arm
export EHS_OS=linux
export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base
#export TOOLCHAIN_NAME=x86_64-linux-gnu_clang10ubuntu18
export CC_OVERRIDE=clang
CFLAGS+=-Wl,-m,armelf_linux_eabi -v -B ../ert-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/usr/lib/gcc/arm-linux-gnueabihf/8 -B ../ert-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/usr/lib/ --sysroot=../ert-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/ --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard -B../ert-build-support/toolchains/x86_64/x86_64-linux-gnu_clang10ubuntu18/bin
LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v -L../comp-lib-support/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/usr/lib/gcc/arm-linux-gnueabihf/8 --sysroot=../ert-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/ -L../ert-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/lib
# "/home/patrick/ssd/work/inx/ert-build-support/toolchains/x86_64/x86_64-linux-gnu_clang10ubuntu18/bin/ld" -z relro --hash-style=gnu --eh-frame-hdr -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o a.out /usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../x86_64-linux-gnu/crt1.o /usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/7.5.0/crtbegin.o -L/usr/lib/gcc/x86_64-linux-gnu/7.5.0 -L/usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../.. -L/home/patrick/ssd/work/inx/ert-build-support/toolchains/x86_64/x86_64-linux-gnu_clang10ubuntu18/bin/../lib  -L/usr/lib target_time.o targetos_init.o target_file.o target_process.o target_main.o target_math.o console_server.o target_console.o target_tcp.o targetcomms_init.o app_data.o blockref_table.o callback_queue.o console.o console_queue.o ehs_main.o debug.o event_queue.o fid.o fidt.o globals.o group.o group_table.o identify.o initialise.o parser.o parse_sodl.o timer.o trigger_table.o utils.o hal_file.o hal_strptime.o hal_systime.o threads.o hal_linkedlist.o hal_jsonlist.o hal_logger.o hal_mem.o hal_string.o devappget.o appstorage.o hal_xml_minimal.o hal_url.o devman_mon.o hal.o statmodules.o fs_dir_list1.o fs_dir_create_remove.o event_counter1.o SFCBarGroup1.o SFCBarGroup1Event.o arraystring1.o buffer.o calc_i.o const1.o convertor.o core_components.o demux.o file.o fileFunctionsUtils.o ehs_if.o logic.o mux.o operator1.o sfc.o state.o stringfn.o time_clock.o trigger.o system_exec.o rtinfo.o wall_clock.o appinfo.o appget.o usercomponents.o inx-ehs_controller.o inx-application_run.o inx-inx-netsocket.o inx-application_info_getter.o inx-devman_interface.o inx-netsocketrsrvr.o yajl_alloc.o yajl_buf.o yajl.o yajl_encode.o yajl_gen.o yajl_lex.o yajl_parser.o yajl_tree.o yajl_version.o inx-json_stream.o inx-xml_stream.o inx-JSONObjectFunctionBlock.o inx-mqtt_client.o inx-mqtt_publish.o inx-mqtt_subscribe.o devman_player.o networking_components.o url_get.o playManager.o media_components.o -lm -oehs_linux_x86_64_clang.exe -E -L/home/patrick/ssd/work/inx/EHS/../ert-build-support/support_libs/target_libs/x86-linux-gnu-x86_64-linux-gnu_clang10ubuntu18/build/lib/ -L/home/patrick/ssd/work/inx/EHS/../ert-middleware/target_libs/x86_64-linux-gnu_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/lib/ -L/home/patrick/ssd/work/inx/EHS  -lpthread -lidn -larchive -lz -lcurl -lexpat -ldl -laws-c-common -lxml2 -lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc --as-needed -lgcc_s --no-as-needed /usr/lib/gcc/x86_64-linux-gnu/7.5.0/crtend.o /usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../x86_64-linux-gnu/crtn.o --sysroot=/home/patrick/ssd/work/inx/ert-middleware/target_libs/x86_64-linux-gnu_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build -l:libcrypto.a -l:libssl.a -l:libaws-c-compression.a -l:libaws-c-cal.a -l:libaws-c-http.a -l:libs2n.a -l:libaws-c-io.a -l:libaws-c-mqtt.a
#
# uncomment this variable if the platform requires graphics/video support
#EHS_GUI_SUPPORT=yes
#
# Set this to match one of the graphics types in EHS/target/graphics
#EHS_GUI=none
#export EHS_GUI_SUPPORT=gtk
#IS_RGBA=yes - delete this it is not used ..

#
# uncomment this variable if the platform requires audio / video support
export  EHS_AV_SUPPORT=devmanonly
# @todo this should be the same as media support?

#
# uncomment this variable if the platform requires media manager support (e.g. SMIL, DLNA).
#EHS_VIDEO_SUPPORT=yes
#EHS_VIDEO=none
export  EHS_MEDIA_SUPPORT=all

#
# uncomment this variable if the platform requires NETWORKING e.g. devman plugins 

export EHS_NETWORKING_SUPPORT=all
export EHS_COMPONENT_NETWORKING_SUPPORT=all

#
# uncomment this variable if the platform requires devman monitor support
export EHS_DEVMAN_SUPPORT=all
export EHS_DEVMAN_MON_SUPPORT=yes 
#todo there should be a better conversion of 'all' into each devman required - maybe scrap EHS_DEVMAN_SUPPORT?

#
# uncomment this variable if the platform needs to support deprecated toolkit
##export EHS_TOOLKIT_DEPRECATED=yes

#
# uncomment this variable if the platform requires networking support
#
# Set this to reflect the Comms API (e.g. BSD or Winsock)
export  EHS_COMMS_API_SUPPORT=bsdsockets

#
# Set this to reflect the type of communication task (e.g. tcp_server_common, ...)
export EHS_COMMS_TASK=tcp_server_common
#@todo this should just go to the bdcsockets and winsockets .mk files

#This include RCUs, text displays, etc.
##export EHS_PERIPHERAL_DEVICE_SUPPORT=all