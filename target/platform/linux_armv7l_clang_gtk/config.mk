#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_armv7l_clang_gtk
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
export EHS_GNU_ARCH=armv7l
export EHS_GNU_OS=linux-gnu
export EHS_GNU_OS_VERSION=-clang10ubuntu18

# EHS Section 
# ehs is more generic
export EHS_ARCH=arm
export EHS_OS=linux_clang
#This is for the idn library version
export EHS_DEBIAN_VERSION=8

################################################################################################################
# Configure debug/production levels
################################################################################################################
#DEBUG OPTIONS
EHS_DEBUGALL=true
#endif

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

export EHS_NETWORKING_SUPPORT=all
export EHS_COMPONENT_NETWORKING_SUPPORT=all
#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
export EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
export EHS_DEVMAN_MON_SUPPORT=yes 
export EHS_MQTT_SUPPORT=greengrass

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
export COMPONENT_VARIANT=aws-iot

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
export EHS_PERIPHERAL_DEVICE_SUPPORT=all
# uncomment this variable if the platform requires graphics/video support
# Set this to match one of the graphics types in EHS/target/graphics
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
export EHS_GUI_SUPPORT=gtk
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
export  EHS_AV_SUPPORT=devmanonly
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
export  EHS_MEDIA_SUPPORT=all

# The following toolbox contains legacy components that are no longer supported in the main toolsboxes and can b relegacted here in case               #
# backward compatability with previous apps  is required. Note this requires the toolbox hash checks to be  disabled                                                        #
export EHS_TOOLKIT_DEPRECATED=yes

################################### END OF TOOLBOX CONFIGURATION ###################################################

ifdef NOT_DEFD
#export CC_OVERRIDE=clang
export INX_SYS_ROOT=../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build
CFLAGS+=-I${INX_SYS_ROOT}/usr/include/atk-1.0 -I${INX_SYS_ROOT}/usr/include/gdk-pixbuf-2.0 -I${INX_SYS_ROOT}/usr/lib/arm-linux-gnueabihf/gtk-2.0/include -I${INX_SYS_ROOT}/usr/include/pango-1.0 -I${INX_SYS_ROOT}/usr/include/cairo -I${INX_SYS_ROOT}/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I${INX_SYS_ROOT}/usr/include/glib-2.0 -I${INX_SYS_ROOT}/usr/include/gtk-2.0 -Wl,-m,armelf_linux_eabi -v -B ${INX_SYS_ROOT}/usr/lib/gcc/arm-linux-gnueabihf/8 -B ${INX_SYS_ROOT}/usr/lib/ --sysroot=${INX_SYS_ROOT} --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard -B..ertS-build-support/toolchains/x86_64/x86_64-linux-gnu_clang10ubuntu18/bin
LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v -L${INX_SYS_ROOT}/usr/lib/gcc/arm-linux-gnueabihf/8 --sysroot=${INX_SYS_ROOT} -L${INX_SYS_ROOT}/lib

#CFLAGS+=-Wl,-m,armelf_linux_eabi -v -B ../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/usr/lib/gcc/arm-linux-gnueabihf/8 -B ../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/usr/lib/ --sysroot=../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/ --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard -B../ert-build-support/toolchains/x86_64/x86_64-linux-gnu_clang10ubuntu18/bin
#CFLAGS+=-Wl,-m,armelf_linux_eabi -v --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard 
CFLAGS+=-Wl,-m -v --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard 
#export LINK_OVERRIDE=clang

#LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v -L../comp-lib-support/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/usr/lib/gcc/arm-linux-gnueabihf/8 --sysroot=../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/ -L../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/lib
LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v 
#the following are to stop clang selecting the host /usr/bin/ld
LNKFLAGS+=-B../ert-build-support/toolchains/x86_64/armv7l-linux-gnu-clang10ubuntu18/bin
LNKFLAGS+=-fuse-ld=lld

# "/home/patrick/ssd/work/inx/ert-build-support/toolchains/x86_64/x86_64-linux-gnu_clang10ubuntu18/bin/ld" -z relro --hash-style=gnu --eh-frame-hdr -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o a.out /usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../x86_64-linux-gnu/crt1.o /usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/7.5.0/crtbegin.o -L/usr/lib/gcc/x86_64-linux-gnu/7.5.0 -L/usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../.. -L/home/patrick/ssd/work/inx/ert-build-support/toolchains/x86_64/x86_64-linux-gnu_clang10ubuntu18/bin/../lib  -L/usr/lib target_time.o targetos_init.o target_file.o target_process.o target_main.o target_math.o console_server.o target_console.o target_tcp.o targetcomms_init.o app_data.o blockref_table.o callback_queue.o console.o console_queue.o ehs_main.o debug.o event_queue.o fid.o fidt.o globals.o group.o group_table.o identify.o initialise.o parser.o parse_sodl.o timer.o trigger_table.o utils.o hal_file.o hal_strptime.o hal_systime.o threads.o hal_linkedlist.o hal_jsonlist.o hal_logger.o hal_mem.o hal_string.o devappget.o appstorage.o hal_xml_minimal.o hal_url.o devman_mon.o hal.o statmodules.o fs_dir_list1.o fs_dir_create_remove.o event_counter1.o SFCBarGroup1.o SFCBarGroup1Event.o arraystring1.o buffer.o calc_i.o const1.o convertor.o core_components.o demux.o file.o fileFunctionsUtils.o ehs_if.o logic.o mux.o operator1.o sfc.o state.o stringfn.o time_clock.o trigger.o system_exec.o rtinfo.o wall_clock.o appinfo.o appget.o usercomponents.o inx-ehs_controller.o inx-application_run.o inx-inx-netsocket.o inx-application_info_getter.o inx-devman_interface.o inx-netsocketrsrvr.o yajl_alloc.o yajl_buf.o yajl.o yajl_encode.o yajl_gen.o yajl_lex.o yajl_parser.o yajl_tree.o yajl_version.o inx-json_stream.o inx-xml_stream.o inx-JSONObjectFunctionBlock.o inx-mqtt_client.o inx-mqtt_publish.o inx-mqtt_subscribe.o devman_player.o networking_components.o url_get.o playManager.o media_components.o -lm -oehs_linux_x86_64_clang.exe -E -L/home/patrick/ssd/work/inx/EHS/../ert-build-support/support_libs/target_libs/x86-linux-gnu-x86_64-linux-gnu_clang10ubuntu18/build/lib/ -L/home/patrick/ssd/work/inx/EHS/../ert-contrib-middleware/target_libs/x86_64-linux-gnu_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/lib/ -L/home/patrick/ssd/work/inx/EHS  -lpthread -lidn -larchive -lz -lcurl -lexpat -ldl -laws-c-common -lxml2 -lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc --as-needed -lgcc_s --no-as-needed /usr/lib/gcc/x86_64-linux-gnu/7.5.0/crtend.o /usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../x86_64-linux-gnu/crtn.o --sysroot=/home/patrick/ssd/work/inx/ert-contrib-middleware/target_libs/x86_64-linux-gnu_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build -l:libcrypto.a -l:libssl.a -l:libaws-c-compression.a -l:libaws-c-cal.a -l:libaws-c-http.a -l:libs2n.a -l:libaws-c-io.a -l:libaws-c-mqtt.a
#
endif

EHS_PNG_LIB := png16


