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
#
# This file builds paths to target specific support libraries and toolchains:#
#
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_PLATFORM_PATH - path to the current directory (set by platform makefile)

################## Get the platform parameters from the platform config.mk file #######


$(info )
$(info $(TXT_FG_BLUE)Starting platform.mk with platform:)
$(info $(TXT_FG_WHITE)  '$(EHS_PLATFORM_PATH)')
$(info )
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

#if the platform doesn't specify a specific libc/middleware version with EHS_GNU_OS_VERSION then set it to the same as the toolchain
ifdef EHS_GNU_OS_VERSION
    EHS_GNU_OS_ARCH=$(TOOLCHAIN_ARCH)-$(TOOLCHAIN_OS)$(EHS_GNU_OS_VERSION)
else
    EHS_GNU_OS_ARCH=$(TOOLCHAIN_ARCH)-$(TOOLCHAIN_OS)
endif
export EHS_GNU_OS_ARCH

## Set upvariables first 
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

ifdef TOOLCHAIN_NAME
    ifeq ($(TOOLCHAIN_NAME),HOST)
        export TOOLCHAIN_PATH=HOST
    else
        ifndef TOOLCHAIN_PATH
            export TOOLCHAIN_PATH=$(EHS_BUILD_MAC_ARCH)/$(TOOLCHAIN_NAME)
        endif
    endif
else
    ifndef TOOLCHAIN_PATH
        ifneq ($(TOOLCHAIN_PATH),HOST)
            # check for an arch and OS specific one first. Otherwise try an arch only (which is rarely/never used so far):
            ifneq ($wildcard $($(EHS_CORE_SUPPORT_BASE)/toolchains/$(EHS_BUILD_MAC_ARCH)/$(EHS_GNU_OS_ARCH)),)
                export TOOLCHAIN_PATH=$(EHS_BUILD_MAC_ARCH)/$(EHS_GNU_OS_ARCH)
            else
                export TOOLCHAIN_PATH=$(EHS_BUILD_MAC_ARCH)/$(TOOLCHAIN_ARCH)
            endif
        endif
    else
    # There is an explicit Toolchain path (relative to ert-build-support/toolchains/) specificed, so let this be used.
    endif
endif

# Shall we remove the following CC, CPP, LINK and AS related lines? They seems like dup of the toolchain.mk
# This sgould be done in toochain.mk
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

ifdef LINK_OVERRIDE
    LINK:=$(LINK_OVERRIDE)
endif

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

ifdef COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH
    # if you want component support from a path defined by the toolchain name or something else specific then use this override method 
    export COMPONENT_BASE_TECHNOLOGIES:=$(COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH)
else
    ifdef COMPONENT_VARIANT
        ifdef TOOLCHAIN_NAME
            ifeq ($(TOOLCHAIN_NAME),HOST)
                export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)
            else
                export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME)
            endif
        else
            export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)
        endif
    else
        # If no overrdies to the component base technology we use the toolchain path that the libs are built wtih
        ifdef TOOLCHAIN_NAME
            export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)-$(TOOLCHAIN_NAME)
        else
            export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)
        endif
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
# todo EHS_SPECIAL_CLIB_EXT seems to not be used any more and can probably be removed....

#This doesn't seem to be useful (build OK without) but seems to be more target specific than should be done here? 
ifneq ($(EHS_HOST_DEBIAN_BUILD),)
    # $(info HOST_BUILD is set, using the host's /usr/ directory for core and component dependencies")
    _KNL = $(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/kernel/
    LIB_DIRS += $(_KNL)
    # We might still hav some middleware dependecies for a host build 
    INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)
    LIB_DIRS += $(EHS_COMPONENT_SUPPORT_LIBS)
    $(info $(TXT_FG_BLUE)Using EHS kernel from:)
    $(info $(TXT_FG_WHITE)  [$(_KNL)])
    $(info )
    $(info $(TXT_FG_BLUE)Also including the default ert-contrib middleware include path:)
    $(info $(TXT_FG_WHITE)  [$(EHS_COMPONENT_SUPPORT_INCLUDE)])
    $(info )
    $(info $(TXT_FG_BLUE)Also including the default ert-contrib middleware library path:)
    $(info $(TXT_FG_WHITE)  [$(EHS_COMPONENT_SUPPORT_LIBS)])
    $(info )
else
    # Add paths the ert-build-support's LIBC
    ifdef EHS_CLIB_OVERRIDE_PATH
        $(info $(TXT_FG_BLUE)EHS_CLIB_OVERRIDE_PATH is set, using the override path:)
        $(info $(TXT_FG_WHITE)  [../ert-build-support/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/])
        $(info )
        INC_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/include/
        LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/lib/
        LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/kernel/
        # done properly gnu toolchain.mk export LD_LIBRARY_PATH+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/lib/
    else
        # Note the following is usally handled with the gcc --sysroot, but we'll add INC and LIB paths explicitly too.
        $(info $(TXT_FG_BLUE)Using the default ert-build-support path:)
        $(info $(TXT_FG_WHITE)  [../ert-build-support/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/build/])
        $(info )
        INC_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/build/include/
        LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/build/lib/
        LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/kernel/
        # done properly gnu toolchain.mk export LD_LIBRARY_PATH+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/lib/
    endif
    $(info $(TXT_FG_BLUE)Using the default ert-contrib middleware include path:)
    $(info $(TXT_FG_WHITE)  [$(EHS_COMPONENT_SUPPORT_INCLUDE)])
    $(info )
    $(info $(TXT_FG_BLUE)Using the default ert-contrib middleware library path:)
    $(info $(TXT_FG_WHITE)  [$(EHS_COMPONENT_SUPPORT_LIBS)])
    # Add the component paths (Names distilled above) 
    INC_DIRS+=$(EHS_COMPONENT_SUPPORT_INCLUDE)
    LIB_DIRS+=$(EHS_COMPONENT_SUPPORT_LIBS)
endif

################ Choose which type of EHS kernel to link to #########################
ifeq ($(ERT_SODL_VERSION),1)
    LIB+=:libehs_ehrt1.a
else
    LIB+=:libehs.a
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

#TODO 2025. - this should go in the target component HAL make files
################ Select between render mode A and B ###############################
ifdef EHS_GUI_SUPPORT
# at the moment only mode B is only used for lvgl
ifeq ($(EHS_GUI_SUPPORT),lvgl)
export EHS_RENDER_MODE=B
export EHS_DONT_USE_BASIC_FONTS=yes
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
# We some times uese this build system to build totall irrelevant code to eRT
ifndef EXCLUDE_EHS_COMMON
    ifdef EHS_BUILD_MONOLITHIC_KERNEL
        include $(EHS_COMMON_KERNEL_PATH)/kernel.mk
    endif

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

#TODO all of these should be idefed as they will be set to empty if previously unset

# don't need this in bash: export EHS_MQTT_SUPPORT
#we need to set this for cases where it needs to override an inheritted server config
export DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE
export DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED

# The following will install the additional platform processes (e.g. android downloader or linux cron). 
export EHS_INSTALL_SUPERVISOR

# The following is used mostly for supervisor and downloader configuration. 
export EHS_ANDROID_INSTALL_VERSION

## This is used for the uploader to a package server
export SSHPORT 

## This is used for exporting of Unity 3D IDE (C#) based project to eRT compatible project/exe 
# e.g. eRT Android Studio project or Windows app with eRT plugin.
export EHS_UNITY_PROJECT_EXPORT_SUPPORT

# Path to a direcory with items used for signing android apk and aab files
export EHS_ANDROID_PACKAGE_SIGNING_PATH

# Used to indicate wherther android traget is built with supervisor
export BUILD_WITH_ANDROID_SUPERVISOR

# Used for specifying any android supplementary apps that needs to be added to target e.g. Amabifier.apk
export ANDROID_SUPPLEMENTARY_APP_REPO
export ANDROID_SUPPLEMENTARY_APP_PATH

# Used for specifing name of the eRT package/executable
export ERT_PACKAGE_NAME
# Used for specifing user facing name of installed application (windows installer)
export ERT_NSIS_EXE_NAME

# Used for the appland deployment
export EHS_APPLAND_INST_SUPPORT
export EHS_APPLAND_INST_DEPLOY_NAME
export EHS_APPLAND_INST_OS_NAME

#####################################################################################
# Display the config - please keep this up to date with all the platform options.
#####################################################################################
$(info )
$(info $(TXT_FG_GREY)--------------------------------------------------------------------)
$(info )
$(info $(TXT_FG_BLUE)Build host architecture:)
$(info $(TXT_FG_GREEN)EHS_ARCH                         = '$(TXT_FG_WHITE)$(EHS_ARCH)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_GNU_ARCH                     = '$(TXT_FG_WHITE)$(EHS_GNU_ARCH)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_OS                           = '$(TXT_FG_WHITE)$(EHS_OS)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_GNU_OS                       = '$(TXT_FG_WHITE)$(EHS_GNU_OS)$(TXT_FG_GREEN)')
$(info )
$(info $(TXT_FG_BLUE)Toolboxes:)
$(info $(TXT_FG_GREEN)EHS_PERIPHERALS_GPIO_SUPPORT     = '$(TXT_FG_WHITE)$(EHS_PERIPHERALS_GPIO_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_PERIPHERAL_DEVICE_SUPPORT    = '$(TXT_FG_WHITE)$(EHS_PERIPHERAL_DEVICE_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_PERIPHERALS_ADC_DAC_SUPPORT  = '$(TXT_FG_WHITE)$(EHS_PERIPHERALS_ADC_DAC_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_COMPONENT_NETWORKING_SUPPORT = '$(TXT_FG_WHITE)$(EHS_COMPONENT_NETWORKING_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_PID_SUPPORT                  = '$(TXT_FG_WHITE)$(EHS_PID_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_SCHEDULER_SUPPORT            = '$(TXT_FG_WHITE)$(EHS_SCHEDULER_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_MODBUS_SUPPORT               = '$(TXT_FG_WHITE)$(EHS_MODBUS_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_GUI_SUPPORT                  = '$(TXT_FG_WHITE)$(EHS_GUI_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_AV_SUPPORT                   = '$(TXT_FG_WHITE)$(EHS_AV_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_VIDEO_SUPPORT                = '$(TXT_FG_WHITE)$(EHS_VIDEO_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_MEDIA_SUPPORT                = '$(TXT_FG_WHITE)$(EHS_MEDIA_SUPPORT)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_TOOLKIT_DEPRECATED           = '$(TXT_FG_WHITE)$(EHS_TOOLKIT_DEPRECATED)$(TXT_FG_GREEN)')
$(info )
$(info $(TXT_FG_BLUE)Debug:)
$(info $(TXT_FG_GREEN)EHS_DEBUGALL                     = '$(TXT_FG_WHITE)$(EHS_DEBUGALL)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_DEBUG_AV                     = '$(TXT_FG_WHITE)$(EHS_DEBUG_AV)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_DEBUG_TCPIP_CONSOLE          = '$(TXT_FG_WHITE)$(EHS_DEBUG_TCPIP_CONSOLE)$(TXT_FG_GREEN)')
$(info $(TXT_FG_GREEN)EHS_DEBUG_TRACE                  = '$(TXT_FG_WHITE)$(EHS_DEBUG_TRACE)$(TXT_FG_GREEN)')
$(info )
$(info $(TXT_FG_GREY)--------------------------------------------------------------------)
