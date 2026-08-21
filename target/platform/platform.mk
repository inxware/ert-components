#---------------------------------------------------------------
# Copyright (C) 2008-2025, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
# <https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------
#
#  eRT components platform build configuration
#
# Makefile fragment using the target's config.mk data to find the correct component and core support code.
# Called by ../../Makefile

###############################################################################################################################
# This file builds paths to target specific support libraries and toolchains:
###############################################################################################################################
# The make file include sequence is as follows:
# 1. target/platform/*/config.mk    : most spefic config for a borad/product/platform                          (USER_EDITABLE)
# 2. target/os-arch/*/toolchain.mk  : toolchain switches needed to choose a toolchain path & compile binaries. (ADMIN EDITABLE)
# 3. target/os-arch/*/config.mk     : default config (including components selected) for an os-arch type       (USER_EDITABLE)
# 4. target/os-arch/*/target.mk     : other build configuration magic incantations for an os-arch type         (ADMIN EDITABLE)
# 5. */ all the deps.mk files for dependency                                                                   (AUTO UPDATEABLE)
# 5. target/Component-HAL/component-hal.mk : Component HAL selected in the config.mk files                     (ADMIN EDITABLE)
# 6. Common/components.mk           : Chooses the selected components to build from the condfig.mk             (ADMIN EDITABLE)
# 7. Common/Ehs/ehs.mk              : Kernel runtime configuration a                                           (ADMIN EDITABLE)    
# 8. Common/Hal/HAL.mk              : Hardware (& Middleware) Abstraction layer build configuration            (USER_EDITABLE)
################################################################################################################################

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_PLATFORM_PATH - path to the current directory (set by platform makefile)

################## Get the platform parameters from the platform config.mk file #######
#$(info )
#$(info $(TXT_FG_BLUE)Starting platform.mk with platform:)
#$(info $(TXT_FG_WHITE)  '$(EHS_PLATFORM_PATH)')
#$(info )

include $(EHS_PLATFORM_PATH)/config.mk

ifdef TOOLCHAIN_NAME
    export TOOLCHAIN_NAME
endif

#set the build host's machine's architecture (it is always linux so far...)

export EHS_BUILD_MAC_ARCH=$(shell uname -m)

#If the ./target/os-arch/ config doesn't set a GNU (GCC/Clang) canonical OS then use the basic one
ifndef EHS_GNU_OS
    EHS_GNU_OS=$(EHS_OS)
endif
TOOLCHAIN_OS=$(EHS_GNU_OS)

#If the ./target/os-arch/ build config doesn't set a GNU (GCC/Clang) canonical OS then use the basic one
ifndef EHS_GNU_ARCH
    EHS_GNU_ARCH=$(EHS_ARCH)
endif
TOOLCHAIN_ARCH=$(EHS_GNU_ARCH)

#These paramters are used in the targetenv shell scripts so need to be exported
export EHS_OS
export EHS_ARCH
export EHS_GNU_OS
export EHS_GNU_ARCH
#some slightly target pecific hack paramters we should export. 
#We should probably do this in the more specific target

# default to eRT1 binary SODL 
ifndef ERT_SODL_VERSION
    ERT_SODL_VERSION=1
endif

export ERT_SODL_VERSION

export DEBIAN_PACKAGE_NAME

export EHS_AUTO_START

#Note we have some toolchains in the oposite order e.g. linux-android-armv7a 
#- in which case we can either fix it in the support repo or use the TOOLCHAIN_NAME override

############# Set up paths to  libc and middleware dependencies    ##########

# EHS_TARGET_LIB_VARIANT is a suffix that narrows the pre-built library set identifier, forming:
#   EHS_GNU_OS_ARCH = $(TOOLCHAIN_ARCH)-$(TOOLCHAIN_OS)$(EHS_TARGET_LIB_VARIANT)
# It is not an OS version — that is why it was renamed from EHS_GNU_OS_VERSION.
# On Linux targets it holds a toolchain/OS flavour string (e.g. -clang10_clang10, -debian-9.4).
# On Zephyr MCU targets it holds a CPU architecture sub-variant (e.g. -cortexm4, -cortexm33).
# On esp32s3 it is the intended home for the cache/memory profile (e.g. -16k).
ifdef EHS_TARGET_LIB_VARIANT
    EHS_GNU_OS_ARCH=$(TOOLCHAIN_ARCH)-$(TOOLCHAIN_OS)$(EHS_TARGET_LIB_VARIANT)
else
    EHS_GNU_OS_ARCH=$(TOOLCHAIN_ARCH)-$(TOOLCHAIN_OS)
endif
export EHS_GNU_OS_ARCH

# ── Where the EHS kernel archives live ────────────────────────────────────────
# EHS_KERNEL_DIR resolves to ../ert-kernels/<EHS_GNU_OS_ARCH>/kernel/.
# ert-kernels is a separate repo and must be checked out alongside
# ert-components — see its README.
#
# There is deliberately no fallback path: without one, a missing checkout fails
# the build rather than silently linking a stale archive from elsewhere.
export EHS_KERNEL_BASE ?= $(EHS_ROOT_PATH)/../ert-kernels

_KEY := $(EHS_GNU_OS_ARCH)
EHS_KERNEL_DIR := $(EHS_KERNEL_BASE)/$(_KEY)/kernel/
export EHS_KERNEL_DIR

## Set up variables first 
################# Some components might need kernel headers e.g. sizes.h       ###################
# Deprecated
#export KERNEL_HEADERS_BASE_DIR=$(EHS_CORE_SUPPORT_BASE)/kernel-dependencies/
#export KERNEL_HEADERS_RELPATH 
#export INC_DIRS += ${KERNEL_HEADERS_BASE_DIR}/$(KERNEL_HEADERS_RELPATH)

############## Define the OS_HW PATH for dependencies within the eRT-componnents source tree ######

export EHS_TARGET_OS_HW_PATH=$(EHS_TARGETS_ROOT_PATH)/os-arch/$(EHS_OS)-$(EHS_ARCH)

# Paramters that need exportng to targetenv and other bas scripts
# SYSTEM_VARIANT selects different OS initialisation scripts and other dployed files that cary with specific targets. 
export SYSTEM_VARIANT
# The hacks file i for more unusual target specific requirements, such as adding 3rd-party apps and resoures for a particular product
export INXWARE_TARGETENV_HACKS

# export the targetenv configuration environment variables for bash
export EHS_DEVMAN_SUPERVISOR_REQUIRED

####################   Configure the os-arch independent toolchain paths  ############################## 

# TOOLCHAIN_PATH is relative to $(EHS_CORE_SUPPORT_BASE)/toolchains/.
# HOST means the build environment supplies the compiler and nothing under
# toolchains/ is read.
ifdef TOOLCHAIN_NAME
    ifeq ($(TOOLCHAIN_NAME),HOST)
        export TOOLCHAIN_PATH=HOST
    else
        export TOOLCHAIN_PATH=$(EHS_BUILD_MAC_ARCH)/$(TOOLCHAIN_NAME)
    endif
else
    # Prefer an arch-and-OS specific toolchain; fall back to arch-only.
    ifneq ($(wildcard $(EHS_CORE_SUPPORT_BASE)/toolchains/$(EHS_BUILD_MAC_ARCH)/$(EHS_GNU_OS_ARCH)),)
        export TOOLCHAIN_PATH=$(EHS_BUILD_MAC_ARCH)/$(EHS_GNU_OS_ARCH)
    else
        export TOOLCHAIN_PATH=$(EHS_BUILD_MAC_ARCH)/$(TOOLCHAIN_ARCH)
    endif
endif

# Shall we remove the following CC, CPP, LINK and AS related lines? They seems like dup of the toolchain.mk
# This should be done in toochain.mk
#ifdef CXX_OVERRIDE
#    CPP:=$(CXX_OVERRIDE)
#endif

#ifdef CC_OVERRIDE
#    CC:=$(CC_OVERRIDE)
#    LINK:=$(CC_OVERRIDE)
#endif

#ifdef AS_OVERRIDE
#    AS:=$(AS_OVERRIDE)
#endif

#ifdef LINK_OVERRIDE
#    LINK:=$(LINK_OVERRIDE)
#endif

ifdef CC
export CC
endif
ifdef CPP
export CPP
endif
ifdef
export LINK
endif
ifdef AS
export AS
endif

################# Set up the Component Library Support Paths ####################################
# Note : gnu sysroot might use this so do it before including toolchain.mk

# COMPONENT_BASE_TECHNOLOGIES — the ert-contrib-middleware target_libs key.
#
#   <EHS_GNU_OS_ARCH>[_<COMPONENT_VARIANT>][-<TOOLCHAIN_NAME>]
#
# The -<TOOLCHAIN_NAME> suffix is deliberate and always applied when a non-HOST
# toolchain is named, even where it duplicates part of the arch/OS string. It is
# what lets one os-arch carry libraries built by two different toolchains side by
# side — e.g. xtensa-esp32s3_freertos has both -xtensa-esp32s3-elf-4.4.4 and
# -xtensa-esp32s3-elf-5.1. Dropping it to save characters would let those collide
# silently, linking libraries built by the wrong compiler.
#
# There is deliberately NO override hook. COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH
# was removed: every use of it compensated for a contrib directory named after how
# it was built rather than how it is consumed. Rename the directory to the key this
# rule derives instead. See docs/target-libs-naming-audit.md.
#
# NOTE: the kernel key (below) is EHS_GNU_OS_ARCH alone — it carries neither the
# variant nor the toolchain, so two toolchains for one os-arch currently share one
# kernel archive. Unify when the kernel repo is split out.
# A platform config assigning COMPONENT_BASE_TECHNOLOGIES directly is silently
# discarded -- the := below always wins. xcore_freertos-xcore-base/config.mk:29
# does exactly that and has no effect. Warn rather than error: the assignment is
# harmless, but believing it works is not.
ifdef COMPONENT_BASE_TECHNOLOGIES
    $(warning COMPONENT_BASE_TECHNOLOGIES set in a platform config is ignored -- \
it is derived below. Rename the ert-contrib-middleware directory to the derived \
key instead. See docs/target-libs-naming-audit.md.)
endif

ifdef COMPONENT_VARIANT
    ifdef TOOLCHAIN_NAME
        ifeq ($(TOOLCHAIN_NAME),HOST)
            export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)
        else
            export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME)
        endif
    else
        export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)
    endif
else
    ifdef TOOLCHAIN_NAME
        export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)-$(TOOLCHAIN_NAME)
    else
        export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)
    endif
endif

#################### Configure the arch-os-specific toolchain parameters ##############################
include $(EHS_TARGET_OS_HW_PATH)/toolchain.mk

export EHS_TARGET_COMPONENT_HAL_PATH=$(EHS_TARGETS_ROOT_PATH)/Component-HAL

ifndef COMPONENT_BASE_TECHNOLOGIES
    $(error == COMPONENT_BASE_TECHNOLOGIES is not defined)
else
    $(info $(TXT_FG_BLUE)Your Build target is using ert-contrib_middleware:)
    $(info $(TXT_FG_WHITE)  [$(COMPONENT_BASE_TECHNOLOGIES)])
    $(info )
endif

# and apply to the compiler paths 
export EHS_COMPONENT_SUPPORT_BUILD:=$(EHS_COMPONENT_SUPPORT_BASE)/target_libs/$(COMPONENT_BASE_TECHNOLOGIES)/build/
export EHS_COMPONENT_SUPPORT_INCLUDE:=$(EHS_COMPONENT_SUPPORT_BUILD)/include/
export EHS_COMPONENT_SUPPORT_LIBS:=$(EHS_COMPONENT_SUPPORT_BUILD)lib/

################## Build the core target specific support code #########################
# include files to build the core EHS target specific for this platform + any special OS Compoents.

# Setup default dependency and feature support for os-arch types so this doesn't need to be done for each platform.

ifneq ($(wildcard $(EHS_TARGET_OS_HW_PATH)/config.mk),)
	include $(EHS_TARGET_OS_HW_PATH)/config.mk
else 
#$(info $(TXT_FG_TELLO) Warning, Could not find a os-arch level config.mk file at:)
#$(info $(TXT_FG_YELLOW) '$(EHS_TARGET_OS_HW_PATH)')
endif

# Set up any other build configurations.
include $(EHS_TARGET_OS_HW_PATH)/target.mk
# Check for a default configuration file fir this is-arch 

############## Set up some eRT Source level conditional build macros ##########

# IF WE HAVE A NATIVE BUILD (e.g. docker) THEN MUCH OF THE ABOVE SHOULD PROBABLY BE REMOVED? 
# Though it probably doesn't do any harm having linkes to resources in ert-* support repos if there's nothing in them.
# TODO2022 we want this to be set more generically for platform.mk that wnt to build under a specific docker or vagrant environment.


#This doesn't seem to be useful (build OK without) but seems to be more target specific than should be done here? 
# macOS host builds use the same path conventions as Debian host builds.
# Map EHS_HOST_MACOS_BUILD -> EHS_HOST_DEBIAN_BUILD so the block below handles both.
ifdef EHS_HOST_MACOS_BUILD
    ifndef EHS_HOST_DEBIAN_BUILD
        EHS_HOST_DEBIAN_BUILD=$(EHS_HOST_MACOS_BUILD)
    endif
endif

ifneq ($(EHS_HOST_DEBIAN_BUILD),)
    # $(info HOST_BUILD is set, using the host's /usr/ directory for core and component dependencies")

    _KNL = $(EHS_KERNEL_DIR)
    LIB_DIRS += $(_KNL)
    # We might still hav some middleware dependecies for a host build 
    INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)
    LIB_DIRS += $(EHS_COMPONENT_SUPPORT_LIBS)
    $(info $(TXT_FG_BLUE)Using EHS kernel from:)
    $(info $(TXT_FG_WHITE)  [$(_KNL)])

    $(info $(TXT_FG_BLUE)Also including the default ert-contrib middleware include path:)
    $(info $(TXT_FG_WHITE)  [$(EHS_COMPONENT_SUPPORT_INCLUDE)])

    $(info $(TXT_FG_BLUE)Also including the default ert-contrib middleware library path:)
    $(info $(TXT_FG_WHITE)  [$(EHS_COMPONENT_SUPPORT_LIBS)])

else
    # Add paths the ert-build-support's LIBC
    ifdef EHS_CLIB_OVERRIDE_PATH
        $(info $(TXT_FG_BLUE)EHS_CLIB_OVERRIDE_PATH is set, using the override path:)
        $(info $(TXT_FG_WHITE)  [../ert-build-support/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/])
        INC_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/include/
        LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/lib/
        # done properly gnu toolchain.mk export LD_LIBRARY_PATH+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/lib/
    else
        # Note the following is usally handled with the gcc --sysroot, but we'll add INC and LIB paths explicitly too.
        $(info $(TXT_FG_BLUE)Using the default ert-build-support path:)
        $(info $(TXT_FG_WHITE)  [../ert-build-support/support_libs/target_libs/$(EHS_GNU_OS_ARCH)/build/])

        INC_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)/build/include/
        LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)/build/lib/
        # done properly gnu toolchain.mk export LD_LIBRARY_PATH+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)/lib/
    endif

    # The kernel lives in its own repo and its location does not depend on the
    # Outside the EHS_CLIB_OVERRIDE_PATH branch above: the kernel lives in its
    # own repo (ert-kernels) and its location does not depend on the libc.
    LIB_DIRS+=$(EHS_KERNEL_DIR)
    $(info $(TXT_FG_BLUE)Using the default ert-contrib middleware include path:)
    $(info $(TXT_FG_WHITE)  [$(EHS_COMPONENT_SUPPORT_INCLUDE)])
    $(info $(TXT_FG_BLUE)Using the default ert-contrib middleware library path:)
    $(info $(TXT_FG_WHITE)  [$(EHS_COMPONENT_SUPPORT_LIBS)])
    # Add the component paths (Names distilled above) 
    INC_DIRS+=$(EHS_COMPONENT_SUPPORT_INCLUDE)
    LIB_DIRS+=$(EHS_COMPONENT_SUPPORT_LIBS)
endif

################ Choose which type of EHS kernel to link to #########################
# GNU ld supports -l:filename (exact match); Apple ld only supports -lname.
# Use standard -lname on macOS — the kernel dir is already in LIB_DIRS and
# there is no .dylib counterpart, so -lehs_ehrt1 resolves to libehs_ehrt1.a.
ifeq ($(ERT_SODL_VERSION),1)
    ifeq ($(EHS_OS),macos)
        LIB+=ehs_ehrt1
    else
        LIB+=:libehs_ehrt1.a
    endif
else
    ifeq ($(EHS_OS),macos)
        LIB+=ehs
    else
        LIB+=:libehs.a
    endif
endif
#And just in case the code needs to know what the SODL type is
DEFS += ERT_SODL_VERSION=$(ERT_SODL_VERSION)

###########################################################################################
# Function-specific components mostly
##########################################################################################


# Some components have GNU dependencies, This flag can be set to make sure these aren't built 
# or included in builds.

ifdef EHS_SKIP_GNULIBRARIES
ifneq ($(EHS_SKIP_GNULIBRARIES),none)
    DEFS += EHS_SKIP_GNULIBRARIES
endif
endif

################# Setup the target include paths for core EHS ##########################
INC_DIRS +=$(EHS_PLATFORM_PATH)
VPATH += $(EHS_PLATFORM_PATH)
#VPATH+= $(EHS_TARGET_OS_HW_PATH)
#OBJECTS+=: none - there are no c-files for each platform just config parameters.
# Do the rest in the os make file

include $(EHS_TARGET_OS_HW_PATH)/deps.mk
INC_DIRS += $(EHS_TARGET_OS_HW_PATH)
VPATH+= $(EHS_TARGET_OS_HW_PATH)

# Don't include this yet because it gets configured by the target specific make scripts.
### include $(EHS_COMMON_HAL_PATH)/HAL.mk
# but we do need to do this:
INC_DIRS += $(EHS_COMMON_HAL_PATH)/include

#include sourcecode from components dir in build
include $(EHS_TARGET_OS_HW_PATH)/Components/deps.mk
INC_DIRS += $(EHS_TARGET_OS_HW_PATH)/Components
VPATH+= $(EHS_TARGET_OS_HW_PATH)/Components

#
# add required object files to OBJECTS
#
# THIS SHOULD ALWAYS BE TRUE for and ert-component build.
ifndef EXCLUDE_EHS_COMMON

# EHS_BUILD_MONOLITHIC_KERNEL: the EHS kernel is pre-compiled by the EHS-kernel repo and
# installed to ert-build-support as libehs_ehrt1.a (or libehs.a for SODL v0).
# The CMakeLists.txt generated by zephyr_cmake_gen.py links against that library directly.
# No kernel source needs compiling here — EHS_COMMON_KAPI_PATH (already in INC_DIRS) provides
# the kernel headers for the ert-components C files.
# define the component HAL first because this can affect the components selected for the toolboxes'
include $(EHS_TARGET_COMPONENT_HAL_PATH)/component-hal.mk

# Configure the Components Code used
# build the common Layer (The common components.mk file will conditionally compile depending on Component Options
include $(EHS_COMMON_COMPONENTS_PATH)/components.mk
# All target stuff is done from the platform.mk file (indirectly) relative target specific layer
include $(EHS_COMMON_EHS_PATH)/ehs.mk

endif

# Important to inlcude the HAL last because it's build will depend on what subcomponents are included above.
include $(EHS_COMMON_HAL_PATH)/HAL.mk

############## Agregate OS configuration scripts from config.mk recipe and command line ###########
export HOST_OS_CONFIG_SCRIPTS+=$(HOST_OS_CONFIG_SCRIPTS_EXTRA)

############# Some very specific variables that need to be exported to targetenv scripts:
export EHS_PLUGIN_LIBRARY_DEPENDENCY

############ Export the application config for bash scripts
# This defines the default application - set NONE for none, otherwise the default desktop home app is installed
export EHS_DEFAULT_APP

############ Pick up any Devman URLs and credentials and pass these on to the build and packaing environments #####

# This is the URL the CORE devman services will use to connect
export DEVMAN_SERVER_DOMAIN
export DEVMAN_SERVER_DOMAIN_1
export DEVMAN_SERVER_DOMAIN_2
# export additional domains
export DEVMAN_SERVER_PROTOCOL
export DEVMAN_UNAME
export DEBIAN_INXWARE_SERVER_DOMAIN
# Devman upload packaging (to generalise at some point for each type of packager - perhaps a list of APKs to zip?)
export EHS_PRODUCT_NAME
#todo 2023: We should be able to dump this wen we get rid of the android installer script duplication
export DEVMAN_SERVER_NAME

export NETWORK_NTP_SERVER
export NETWORK_HARDWIRED_HOSTS

# So we don't use any code we shouldnt. We also need to start any service threads for MCUs.
ifdef EHS_MCU_TARGET
    export EHS_MCU_TARGET
    DEFS += EHS_MCU_TARGET
endif

# Allow basic memory management with no clean up. Not recommended for apps that have
# console enabled or can receive new apps.
ifdef EHS_MEMORY_MANAGMENT
    ifeq ($(EHS_MEMORY_MANAGMENT),none)
        DEFS += EHS_MEMORY_MANAGMENT__NOMANAGEMENT
    endif
endif

# Callback-driven main loop (e.g. Qt timer). When set, target_main uses
# EhsMainLoopSingle() instead of the blocking EhsMainLoop().
ifdef EHS_MAIN_LOOP_ITERATIVE
    DEFS += EHS_MAIN_LOOP_ITERATIVE
endif

#TODO all of these should be idefed as they will be set to empty if previously unset

# don't need this in bash: export EHS_MQTT_SUPPORT
#we need to set this for cases where it needs to override an inheritted server config
ifdef DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE
export DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE
endif
ifdef DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED
export DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED
endif

# The following will install the additional platform processes (e.g. android downloader or linux cron). 
ifdef EHS_INSTALL_SUPERVISOR
export EHS_INSTALL_SUPERVISOR
endif

# The following is used mostly for supervisor and downloader configuration. 
ifdef EHS_ANDROID_INSTALL_VERSION
export EHS_ANDROID_INSTALL_VERSION
endif
## This is used for the uploader to a package server
ifdef SSHPORT
export SSHPORT 
endif

## This is used for exporting of Unity 3D IDE (C#) based project to eRT compatible project/exe 
# e.g. eRT Android Studio project or Windows app with eRT plugin.
ifdef EHS_UNITY_PROJECT_EXPORT_SUPPORT
export EHS_UNITY_PROJECT_EXPORT_SUPPORT
endif
ifdef EHS_UNITY_VERSION
export EHS_UNITY_VERSION
endif
ifdef EHS_UNITY_ANDROID_STUDIO_TEMPLATE
export EHS_UNITY_ANDROID_STUDIO_TEMPLATE  # optional override — normally derived from EHS_UNITY_VERSION
endif
ifdef EHS_ANDROID_NATIVE_ABI
export EHS_ANDROID_NATIVE_ABI  # arm64 for 64-bit-only targets (Android 12+); default arm for dual-ABI
endif
# Path to a direcory with items used for signing android apk and aab files
ifdef EHS_ANDROID_PACKAGE_SIGNING_PATH
export EHS_ANDROID_PACKAGE_SIGNING_PATH
endif

# Used to indicate wherther android traget is built with supervisor
ifdef BUILD_WITH_ANDROID_SUPERVISOR
export BUILD_WITH_ANDROID_SUPERVISOR
endif

# Used for specifying any android supplementary apps that needs to be added to target e.g. Amabifier.apk
ifdef ANDROID_SUPPLEMENTARY_APP_REPO
export ANDROID_SUPPLEMENTARY_APP_REPO
endif

ifdef ANDROID_SUPPLEMENTARY_APP_PATH
export ANDROID_SUPPLEMENTARY_APP_PATH
endif

# Used for specifing name of the eRT package/executable
ifdef ERT_PACKAGE_NAME
export ERT_PACKAGE_NAME
endif

# Used for specifing user facing name of installed application (windows installer)

ifdef  ERT_NSIS_EXE_NAME
export ERT_NSIS_EXE_NAME
endif

# Used for the appland deployment
ifdef EHS_APPLAND_INST_SUPPORT
export EHS_APPLAND_INST_SUPPORT
endif

ifdef EHS_APPLAND_INST_DEPLOY_NAME
export EHS_APPLAND_INST_DEPLOY_NAME
endif

ifdef EHS_APPLAND_INST_OS_NAME
export EHS_APPLAND_INST_OS_NAME
endif

#####################################################################################
# Display the config - please keep this up to date with all the platform options.
#####################################################################################
#$(info $(TXT_BG_GREY) MAKE ENVIRONMENT                                              $(TXT_BG_BLACK))
$(info $(TXT_FG_GREY)--$(TXT_FG_CYAN)BUILD HOST ARCHIECTURE$(TXT_FG_GREY)---------------------------------------)
$(info $(TXT_FG_GREEN)EHS_ARCH                         = '$(TXT_FG_WHITE)$(EHS_ARCH)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_GNU_ARCH                     = '$(TXT_FG_WHITE)$(EHS_GNU_ARCH)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_OS                           = '$(TXT_FG_WHITE)$(EHS_OS)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_GNU_OS                       = '$(TXT_FG_WHITE)$(EHS_GNU_OS)$(TXT_FG_GREEN)')
ifdef EHS_DEBIAN_VERSION
$(info $(TXT_FG_GREEN)DEBIAN VERSION                   = '$(TXT_FG_WHITE)$(EHS_DEBIAN_VERSION)$(TXT_FG_GREEN)')
endif
$(info $(TXT_FG_GREY)--$(TXT_FG_CYAN)COMPONENT TOOLBOXES$(TXT_FG_GREY)------------------------------------------)
$(info $(TXT_FG_GREEN)EHS_PERIPHERALS_GPIO_SUPPORT     = '$(TXT_FG_WHITE)$(EHS_PERIPHERALS_GPIO_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_PERIPHERALS_PWM_SUPPORT      = '$(TXT_FG_WHITE)$(EHS_PERIPHERALS_PWM_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_PERIPHERAL_DEVICE_SUPPORT    = '$(TXT_FG_WHITE)$(EHS_PERIPHERAL_DEVICE_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_PERIPHERALS_ADC_DAC_SUPPORT  = '$(TXT_FG_WHITE)$(EHS_PERIPHERALS_ADC_DAC_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_PERIPHERALS_UART_SUPPORT     = '$(TXT_FG_WHITE)$(EHS_PERIPHERALS_UART_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_COMPONENT_NETWORKING_SUPPORT = '$(TXT_FG_WHITE)$(EHS_COMPONENT_NETWORKING_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_PID_SUPPORT                  = '$(TXT_FG_WHITE)$(EHS_PID_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_SCHEDULER_SUPPORT            = '$(TXT_FG_WHITE)$(EHS_SCHEDULER_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_MODBUS_SUPPORT               = '$(TXT_FG_WHITE)$(EHS_MODBUS_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_GUI_SUPPORT                  = '$(TXT_FG_WHITE)$(EHS_GUI_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_AV_SUPPORT                   = '$(TXT_FG_WHITE)$(EHS_AV_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_VIDEO_SUPPORT                = '$(TXT_FG_WHITE)$(EHS_VIDEO_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_MEDIA_SUPPORT                = '$(TXT_FG_WHITE)$(EHS_MEDIA_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_TOOLKIT_DEPRECATED           = '$(TXT_FG_WHITE)$(EHS_TOOLKIT_DEPRECATED)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_MV_SUPPORT                   = '$(TXT_FG_WHITE)$(EHS_MV_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_ML_SUPPORT                   = '$(TXT_FG_WHITE)$(EHS_ML_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_ML_IE_IMAGE_SUPPORT   = '$(TXT_FG_WHITE)$(EHS_ML_IE_IMAGE_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_ML_HARDWARE_ACCELERATION     = '$(TXT_FG_WHITE)$(EHS_ML_HARDWARE_ACCELERATION)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_MQTT_SUPPORT                 = '$(TXT_FG_WHITE)$(EHS_MQTT_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_LORAWAN_SUPPORT              = '$(TXT_FG_WHITE)$(EHS_LORAWAN_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREY)--$(TXT_FG_CYAN)DEBUGGING LEVELS$(TXT_FG_GREY)----------------------------------------------)
$(info $(TXT_FG_GREEN)EHS_DEBUGALL                     = '$(TXT_FG_WHITE)$(EHS_DEBUGALL)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_DEBUG_AV                     = '$(TXT_FG_WHITE)$(EHS_DEBUG_AV)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_DEBUG_TCPIP_CONSOLE          = '$(TXT_FG_WHITE)$(EHS_DEBUG_TCPIP_CONSOLE)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_DEBUG_TRACE                  = '$(TXT_FG_WHITE)$(EHS_DEBUG_TRACE)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREY)---------------------------------------------------------------)
