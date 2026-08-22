#---------------------------------------------------------------
# Copyright (C) 2008-2025, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Makefile for ert-components

# Overrides from command line:
#  TARGET - specify a different target
#  REVOPT=-V - if a build with the revision number is required.
#  TK_DEPRECATE=no if the deprecated toolkit is not required in this build
#
# Targets:
#  clean
#  depend - recreate file dependencies

#todo to stop the script bailing out the other includes should be prepended with -, but this maybe a bit obscure for now
-include ./TARGET.cfg
$(info Configured TARGET loaded as '$(TARGET)')

export TARGET
# if passed on the command line then export
#export OBJDIRECTORY=./OBJECTS/$(TARGET)#
export TARGET_NAME=ehs_$(TARGET)

################################################################################
# Standard directory paths
#
SCRIPT_PATH=../scripts
#ENVIRONMENT_PATH=../environment - no longer used for EHS

#Core EHS paths:
export EHS_ROOT_PATH=$(PWD)
EHS_BUILD_PATH=$(EHS_ROOT_PATH)
EHS_TARGETS_ROOT_PATH=$(EHS_ROOT_PATH)/target
EHS_PLATFORM_PATH=$(EHS_TARGETS_ROOT_PATH)/platform/$(TARGET)
EHS_COMMON_PATH=$(EHS_ROOT_PATH)/Common
EHS_COMMON_HAL_PATH=$(EHS_COMMON_PATH)/HAL
EHS_COMMON_KERNEL_PATH=$(EHS_COMMON_PATH)/Kernel
EHS_COMMON_KAPI_PATH=$(EHS_COMMON_PATH)/KAPI
EHS_COMMON_EHS_PATH=$(EHS_COMMON_PATH)/Ehs
EHS_COMMON_COMPONENTS_PATH=$(EHS_COMMON_PATH)/Components
EHS_COMMON_TOOLKIT_CORE_PATH=$(EHS_COMMON_COMPONENTS_PATH)/core

# Enable colourised output within this makefile
# todo colour.nk should go in the scripts folder or be optional. It is breaking a lot of things as is.
include $(EHS_COMMON_EHS_PATH)/colour.mk

################################################################################
# Standard include paths
#
INC_DIRS+=$(EHS_COMMON_KAPI_PATH) $(EHS_COMMON_EHS_PATH) $(EHS_COMMON_COMPONENTS_PATH)
INC_DIRS+=$(EHS_COMMON_TOOLKIT_CORE_PATH)

# ?= so a build environment (e.g. a Docker image that bakes in the prebuilt
# libs and toolchain) can point these elsewhere. = would discard the inherited
# environment value. Default remains the sibling checkout.
export EHS_COMPONENT_SUPPORT_BASE ?= $(EHS_ROOT_PATH)/../ert-contrib-middleware
export EHS_CORE_SUPPORT_BASE ?= $(EHS_ROOT_PATH)/../ert-build-support

#We need to export this for bash targetenv scripts to use
export EXE
#This is the binary output filename extension
export FINAL
# standard definitions for Csource
DEFS = MAKE_TARGET=$(TARGET)

# Test function override support
ifdef TEST_FUNC
    DEFS += EHS_TEST_FUNC_OVERRIDE
    DEFS += EHS_TEST_FUNC_NAME=$(TEST_FUNC)

    # Optional: Skip all eRT initialization
    ifeq ($(ERT_INIT),none)
        DEFS += EHS_TEST_FUNC_NO_ERT_INIT
    endif
endif

################## Target-specific tool definitions ##############################################################
#
#  Uses $(INC_DIRS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)
#

################### Where to look for source code ####################################################
#
# Linker to look in our object directory
VPATH+=./
#$(OBJDIRECTORY)

# Get OS specfic build configurations and target-specific toolchain settings
ifdef TARGET
include $(EHS_PLATFORM_PATH)/../platform.mk
endif

# ---------------------------------------------------------------------------
# Zephyr: resolve OBJECTS → source file paths using VPATH.
#
# Done here (after all .mk includes) so the full OBJECTS and VPATH lists are
# known.  The result is written to make_vars.env by the zephyr_cmake_gen
# recipe so that scripts/zephyr_cmake_gen.py receives fully-resolved paths
# and does NOT need to re-implement make's VPATH search.
#
# Uses $(wildcard) which returns empty string for unresolvable objects;
# those are silently omitted from ERT_ZEPHYR_SOURCES.
# ---------------------------------------------------------------------------
ifdef EHS_ZEPHYR
  # Normalise VPATH: make accepts both space- and colon-separated entries
  _ERT_VPATH_DIRS := $(subst :, ,$(VPATH))
  # Function: stem → first matching source file found in VPATH
  _ert_find_src = $(firstword \
      $(wildcard $(addsuffix /$(1).c,$(_ERT_VPATH_DIRS)))  \
      $(wildcard $(addsuffix /$(1).cpp,$(_ERT_VPATH_DIRS)))\
      $(wildcard $(addsuffix /$(1).s,$(_ERT_VPATH_DIRS)))  \
      $(wildcard $(addsuffix /$(1).S,$(_ERT_VPATH_DIRS))))
  # Resolve every object to its source file (strip directory prefix and .o)
  ERT_ZEPHYR_SOURCES := $(strip $(foreach obj,$(OBJECTS),\
      $(call _ert_find_src,$(basename $(notdir $(obj))))))
endif

# SBOM (Software Bill of Materials) — dependency usage tracking across platforms.
# Only loaded when 'make sbom' is explicitly requested to avoid parse-time overhead.
ifeq ($(filter sbom,$(MAKECMDGOALS)),sbom)
-include $(EHS_TARGETS_ROOT_PATH)/platform/sbom.mk
endif

.PHONY: chkconfig
.PHONY: configure-host
.PHONY: unit_tests unit_tests_asan stack_report static_analysis
.PHONY: sbom
.PHONY: help
#.PHONY: $(OBJDIRECTORY)

############################ make targets #####################################################
# First rule: defines what to build
#
# Ensure chkconfig is run _before_ building
$(TARGET_NAME).$(FINAL): chkconfig
all: $(TARGET_NAME).$(FINAL)

#$(OBJDIRECTORY):
#	@mkdir -p $(OBJDIRECTORY)

# Can't use predefined implicit rules here - we might need to generate .obj, rather than .o
# therefore create custom implicit rules.
# All object files depend on all included makefiles - changes to .mk files trigger rebuild
%.$(OBJ): %.s $(MAKEFILE_LIST)
	@$(ECHO) $(AS) $<
	@$(AS) $(ASFLAGS) $< -o $@

%.$(OBJ): %.c $(MAKEFILE_LIST)
	@$(ECHO) $(CC) $<
	@$(CC) -v $(CC_SWITCHES) $(CFLAGS) $< -o $@

%.$(OBJ): %.cpp $(MAKEFILE_LIST)
	@$(ECHO) $(CPP) $<
	@$(CPP) $(CC_SWITCHES) $(CPPFLAGS) $< -o $@

$(TARGET_NAME).$(FINAL) : $(OBJECTS)
# @$(ECHO) $(PWD)
# @$(ECHO) Linking with Flags: $(LINK) $(LNKFLAGS) $<
	$(LINK) $(LD_SWITCHES) $(OBJECTS) $(LNKFLAGS)
# -Wl,-Bdynamic -- if dynamic needs forcing?
# echo will try copying ehs binary to canonical location for debug - Will fail if link from /root/ehs to TARGET env has not been created
# @mkdir -p ./objects_$(TARGET)
ifdef EHS_ANDROID
ifdef EHS_ANDROID_JNI
# todo remove this if we no longer support JNI meandroid builds or change it to use the staging directory if we do
	@cp -f $(TARGET_NAME).$(FINAL) ./target/os-arch/android_ALL/android-java-project/libs/armeabi/lib$(TARGET_NAME).$(FINAL)
	@$(ECHO) copied to ./target/os-arch/android_ALL/android-java-project/libs/armeabi/lib$(TARGET_NAME).$(FINAL)
endif
# @mkdir -p ../TARGET_TREES/ehs_env-$(TARGET)/ todo - look for paths used in esoteric android targetenv scripts.
# @cp -f $(TARGET_NAME).$(FINAL) ../TARGET_TREES/ehs_env-$(TARGET)/
# @$(ECHO) copied to ../TARGET_TREES/ehs_env-$(TARGET)/
else ifdef EHS_XCORE
	@$(ECHO) "Phase 1 complete: $(TARGET_NAME).$(FINAL)"
	@$(ECHO) "Run 'make targetenv_xmos_docker' to link the final .xe firmware via xcommon_cmake."
else
	@mkdir -p ../TARGET_TREES/ehs_env-$(TARGET)/bin
	@cp -f $(TARGET_NAME).$(FINAL) ../TARGET_TREES/ehs_env-$(TARGET)/bin/ehs.exe
	@$(ECHO) copied to ../TARGET_TREES/ehs_env-$(TARGET)/bin/ehs.exe
endif
#end of  all:

############################  Environment & Repository Scripts ##############################################
#@todo the following should be tidied so that a single base script is called and that searches for any more specific scripts from the TARGET argumenent
#This would make it easier to come up with new default targets without having to create a new scripts
# rebuild all dependencies

define missing_target
    @$(ERR) "No TARGET specified."
    @$(ECHO)
    @$(ECHO) "Please run ./confgure to list of available platform targets and then run:"
    @$(ECHO) "  $(TXT_FG_WHITE)./configure TARGET  $(TXT_FG)to select a platform target (sticky)."
    @$(ECHO)
    @$(ECHO) "or use:"
    @$(ECHO) "  $(TXT_FG_WHITE)make TARGET=[PLATFORM_TARGET] [MAKE_TARGET]  $(TXT_FG)to make with a named platform target."
    @$(ECHO)
    @exit 1
endef

chkconfig:
ifndef TARGET
	@$(call missing_target)
else
	@$(ECHO) "The following dependencies are available for this build:"
	@./target/envbuildscripts/buildenv_checkpaths.sh
endif

compare_kernelconfig:
ifndef TARGET
	@$(call missing_target)
else
	@$(ECHO) "The following dependencies are available for this build with the kernel at ${EHS_GNU_OS_ARCH}"
	@$(ECHO) "meld ./target/platform/$(TARGET)/ ../EHS-kernel/target/platform/${EHS_GNU_OS_ARCH}/"
	@meld "./target/platform/$(TARGET)/ ../EHS-kernel/target/platform/${EHS_GNU_OS_ARCH}/"
endif

help:
	@$(ECHO)
	@$(HEADING) "Make help for inxware runtime software"
	@$(ECHO)
	@$(ECHO) "--------------------------------------------------------------------"
	@$(ECHO)
	@$(ECHO) "$(TXT_FG_BLUE)The most common make targets, in typical order of execution:"
	@$(ECHO)
	@$(ECHO) "  $(TXT_FG_WHITE)prepdeps$(TXT_FG_BRIGHT_GREEN)           - Checkout all git dependencies (unless SKIP_REPOS=yes)."
	@$(ECHO) "  $(TXT_FG_WHITE)all$(TXT_FG_BRIGHT_GREEN)                - Make ehs_$(TARGET).exe and copy the resulting TARGETENV bin as ehs.exe"
	@$(ECHO) "     - Set the environment variable $(TXT_FG_WHITE)TEST_FUNC=<some function> $(TXT_FG) to run an alternative entrypoint instead of ehs_main() event handler."
	@$(ECHO) "     - Set $(TXT_FG_WHITE)ERT_INIT=none$(TXT_FG) the normal HAL initialisation functions before running TEST_FUNC."

	@$(ECHO) "  $(TXT_FG_GREEN)                     + Use make targetenv HOST_OS_CONFIG_SCRIPTS_EXTRA=\"XXX-ABCD YYY-EFGH\" to include additional OS config."
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_package$(TXT_FG_BRIGHT_GREEN)  - Creates the target runtime package using the installer method speficied by the platform/config.mk"
	@$(ECHO)
	@$(ECHO) "--------------------------------------------------------------------"
	@$(ECHO)
	@$(ECHO) "$(TXT_FG_BLUE)Build Diagnostics:"
	@$(ECHO)
	@$(ECHO) "  $(TXT_FG_WHITE)chkconfig$(TXT_FG_BRIGHT_GREEN)            - Shows the current key config parameters implied by the platform/<TARGET>config.mk"
	@$(ECHO) "  $(TXT_FG_WHITE)sbom$(TXT_FG_BRIGHT_GREEN)                 - Appends a dependency-usage record for TARGET into SBOM.md at each external dep root."
	@$(ECHO) "  $(TXT_FG_WHITE)compare_kernelconfig$(TXT_FG_BRIGHT_GREEN) - Compares platform/<TARGET>/config.mk with the one in ../EHS-kernel/targete/platform/<OS ARCH VERSION>/"
	@$(ECHO) "  $(TXT_FG_WHITE)chk_ext_deps$(TXT_FG_BRIGHT_GREEN)         - Shows the external dependencies met or unmet for the platform configuration."
	@$(ECHO_N) "  $(TXT_FG_WHITE)depend$(TXT_FG_BRIGHT_GREEN)               - "
	@$(WARN) "this updates the source level dependencies and will update the deps.mk make files."
	@$(ECHO)
	@$(ECHO) "--------------------------------------------------------------------"
	@$(ECHO)
	@$(ECHO) "$(TXT_FG_BLUE)Other make targets:"
	@$(ECHO)
	@$(ECHO) "  $(TXT_FG_WHITE)all_docker$(TXT_FG_BRIGHT_GREEN)                     - Makes ehs_$(TARGET).exe for host or docker environment and copied TARGETENV bin as ehs.exe"
	@$(ECHO) "  $(TXT_FG_WHITE)publish_docker_image$(TXT_FG_BRIGHT_GREEN)           - Build new docker image and publish it to inxware dockerhub organization."
	@$(ECHO) "  $(TXT_FG_WHITE)build_docker_local$(TXT_FG_BRIGHT_GREEN)             - Build docker image locally without pushing to registry."
	@$(ECHO) "  $(TXT_FG_WHITE)target_buildenv$(TXT_FG_BRIGHT_GREEN)                - Start the platform's DOCKER environment shell.  Useful during build system tuning."
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_version$(TXT_FG_BRIGHT_GREEN)              - Create a new version number for the target. Note this will check in all changes and create a tagged commit."
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_cleanall$(TXT_FG_BRIGHT_GREEN)             - Removes ALL data and directories from ../TARGET_TREES/ehs_env-$(TARGET)"
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_cleancfg$(TXT_FG_BRIGHT_GREEN)             - Removes all user data from the TARGETENV tree for deployment."
	@$(ECHO) "                                   $(TXT_FG_GREEN)+ Set env variable KEEP_USERCONFIG=yes to keep the userdata/configuration data intact."
	@$(ECHO) "                                   $(TXT_FG_GREEN)+ Set env variable KEEP_DEVMANCONFIG=yes to keep the devman servers intact."
	@$(ECHO) "                                   $(TXT_FG_GREEN)+ Set env variable KEEP_APPLICATION=yes to keep the appdata intact."
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_makeprod$(TXT_FG_BRIGHT_GREEN)             - Configures the runtime with standard INX apps and devman configuration. Cleans existing config first!"
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_deb$(TXT_FG_BRIGHT_GREEN)                  - Creates a debian installer for current tree (targetted at /opt/ehs). optional: UPLOAD=<deb repo URL>"
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_apk$(TXT_FG_BRIGHT_GREEN)                  - Builds android APK and stores it in ../TARGET_TREES/"
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_apk_docker$(TXT_FG_BRIGHT_GREEN)           - Builds android APK and stores it in ../TARGET_TREES/ in an android arm configured docker image."
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_unity_export$(TXT_FG_BRIGHT_GREEN)         - Exports Unity 3D IDE (C#) based project to eRT compatible project/exe e.g. eRT Android Studio project or Windows app with eRT plugin."
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_unity_export_docker$(TXT_FG_BRIGHT_GREEN)  - Same as above but in docker."
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_xmos_docker$(TXT_FG_BRIGHT_GREEN)           - Phase 2 XMOS xcore build: links Phase 1 .a + fwk_rtos SDK via xcommon_cmake → ehs.xe (run after make all_docker)."
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_esp32$(TXT_FG_BRIGHT_GREEN)                - Builds an esp32 (or esp32s3,...) bootable image for deployment via usb or OTA."
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_esp32_docker$(TXT_FG_BRIGHT_GREEN)         - runs make targetenv_esp32X in an esp32X configured docker image."
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_nsis_docker$(TXT_FG_BRIGHT_GREEN)          - Builds a windows installer using the NSIS installer"
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_upload_appland$(TXT_FG_BRIGHT_GREEN)       - Uploads target to the appland alongside all of its documentation. optional: ASSETS_ONLY=yes"
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_upload_ota$(TXT_FG_BRIGHT_GREEN)           - Uploads OTA package to Devman server. optional: SERVER_OVERRIDE=<user@url> server destination override."
	@$(ECHO) "  $(TXT_FG_WHITE)upload_ehs_via_adb$(TXT_FG_BRIGHT_GREEN)             - Uploads apks to the connected (or IP mapped) android device via adb. optional: ADB_IP=<device ip>"
	@$(ECHO) "  $(TXT_FG_WHITE)upload_ehs_deb$(TXT_FG_BRIGHT_GREEN)                 - Uploads the debian package created targetenv_deb. Set  UPLOAD=<deb repo URL>"
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_android_dep_pack$(TXT_FG_BRIGHT_GREEN)     - Bundles eRT android supplementary apps, supervisor into Devman uploadable packages (no APKs are built)."
	@$(ECHO) "  $(TXT_FG_WHITE)upload_ehs_sys_patch$(TXT_FG_BRIGHT_GREEN)           - Uploads TARGETENV tree package (Linux and Android only - FREERTOS fimrware images too?) to a Devman server."
	@$(ECHO) "  $(TXT_FG_GREEN)                                 + Use VERSION_NAME=[your version name] to give the build a special name."
	@$(ECHO) "  $(TXT_FG_GREEN)                                   e.g. make DEVMANSERVER=[your.url.com] DEVMANUID=[your username] upload_ehs_sys_patch."
	@$(ECHO) "  $(TXT_FG_GREEN)                                 + If the patch requires a server reboot (i.e. because it has a new start-upo script) then"
	@$(ECHO) "  $(TXT_FG_GREEN)                                   set an additional variable SYSPATCH_NEED_REBOOT=yes on the command line."
	@$(ECHO) "  $(TXT_FG_GREEN)                                   (KEEP_USERCONFIG=yes & KEEP_APPLICATION=yes can also be used here as described above)."
	@$(ECHO) "  $(TXT_FG_GREEN)                                 + No arguments are required for ANDROID builds. These are deployed to devices using update-to-latest-xxxxx-android"
	@$(ECHO) "  $(TXT_FG_WHITE)upload_server2server_OS_Update$(TXT_FG_BRIGHT_GREEN) - This will install an update to the host server DEVMAN_INTERMEDIATE_SERVER=[your.url.com]"
	@$(ECHO) "  $(TXT_FG_GREEN)                                 + This can be deployed to a slave(e.g. fire-walled) devman instance. One deployed from the host server the packages will become"
	@$(ECHO) "  $(TXT_FG_GREEN)                                   the OS update patches on the final distation. You may also set DEVMAN_INTERMEDIATE_UNAME & DEVMAN_INTERMEDIATE_SSHPORT"
	@$(ECHO) "  $(TXT_FG_WHITE)toolsenv_update$(TXT_FG_BRIGHT_GREEN)                - Updates the dist directory's IDF and CDF directories with this EHS's version component description files."
	@$(ECHO) "  $(TXT_FG_WHITE)components_gendocs$(TXT_FG_BRIGHT_GREEN)             - Generates markdown documentation for all CDF files in component docs directories."
	@$(ECHO) "  $(TXT_FG_WHITE)static_analysis$(TXT_FG_BRIGHT_GREEN)                - Runs rhe static analyser suite on the full source code tree for all configurations."
	@$(ECHO) "  $(TXT_FG_WHITE)configure-host$(TXT_FG_BRIGHT_GREEN)                 - Installs the build tools this machine needs. Run once per machine."
	@$(ECHO) "  $(TXT_FG_WHITE)unit_tests$(TXT_FG_BRIGHT_GREEN)                     - Runs the host unit tests in UnitTest/ (no target or hardware needed)."
	@$(ECHO) "  $(TXT_FG_WHITE)unit_tests_asan$(TXT_FG_BRIGHT_GREEN)                - As unit_tests, under AddressSanitizer + UndefinedBehaviorSanitizer."
	@$(ECHO) "  $(TXT_FG_WHITE)stack_report$(TXT_FG_BRIGHT_GREEN)                   - Reports the largest per-function stack frames. STACK_SRCS=... STACK_THRESHOLD=N"
	@$(ECHO)
	@$(ECHO) "--------------------------------------------------------------------"
	@$(ECHO)
	@$(ECHO) "$(TXT_FG_BLUE)Zephyr RTOS targets (zephyr_arm-*):"
	@$(ECHO)
	@$(WARN) "Zephyr targets do NOT use 'make all' / 'make all_docker'. Use the three steps below instead."
	@$(ECHO)
	@$(ECHO) "  $(TXT_FG_WHITE)zephyr_cmake_gen$(TXT_FG_BRIGHT_GREEN)    - Step 1, CONFIGURE (host-side, no Docker): generates the Zephyr CMake app"
	@$(ECHO) "$(TXT_FG_BRIGHT_GREEN)                          (CMakeLists.txt, prj.conf, board overlays) in ../TARGET_TREES/ehs_env-<TARGET>/zephyr-staging/app/."
	@$(ECHO) "$(TXT_FG_BRIGHT_GREEN)                          Run once per fresh checkout/session, and again after any source, include, define or Kconfig change."
	@$(ECHO) "  $(TXT_FG_WHITE)zephyr_prepdeps$(TXT_FG_BRIGHT_GREEN)     - Step 2, FETCH (needs internet; runs inside Docker, '--network host'): fetches the Zephyr/NCS west"
	@$(ECHO) "$(TXT_FG_BRIGHT_GREEN)                          workspace. Runs automatically before the build targets below; no-op once fetched or if ZEPHYR_BASE is set."
	@$(ECHO) "  $(TXT_FG_WHITE)zephyr_build_docker$(TXT_FG_BRIGHT_GREEN) - Step 3, BUILD (the actual compile+link step — the 'make all_docker' equivalent for Zephyr):"
	@$(ECHO) "$(TXT_FG_BRIGHT_GREEN)                          runs 'west build' inside the Zephyr Docker image → firmware (.hex/.elf/.bin) in ../TARGET_TREES/ehs_env-<TARGET>/bin/."
	@$(ECHO) "$(TXT_FG_BRIGHT_GREEN)                          Never touches the network itself — the fetch already happened in Step 2."
	@$(ECHO) "$(TXT_FG_BRIGHT_GREEN)                          Incremental; safe to re-run as often as you like (only re-run Step 1 when inputs change)."
	@$(ECHO) "  $(TXT_FG_WHITE)zephyr_build$(TXT_FG_BRIGHT_GREEN)        - Step 3, BUILD, native alternative to zephyr_build_docker (requires ZEPHYR_BASE set to a local Zephyr install; no Docker)."
	@$(ECHO)
	@$(ECHO) "$(TXT_FG_BLUE)The usual sequence to build a package is:"
	@$(ECHO)
	@$(ECHO) "  $(TXT_FG_WHITE)./configure TARGET$(TXT_FG_BRIGHT_GREEN)             - Targets can be identified by running ./configure with no parameters."
	@$(ECHO) "  $(TXT_FG_WHITE)make prepdeps$(TXT_FG_BRIGHT_GREEN)                  - Clone  and checkout the platform dependencies and toolchain for the target."
	@$(ECHO) "  $(TXT_FG_WHITE)make all$(TXT_FG_BRIGHT_GREEN)                       - Makes ehs and updates a runtime tree if one exists (see below)."
	@$(ECHO) "  $(TXT_FG_WHITE)make targetenv$(TXT_FG_BRIGHT_GREEN)                 - Makes a runtime tree with all excutables and assets for the specific target in ../TARGET_TREES/*"
	@$(ECHO)
	@$(ECHO) "$(TXT_FG_BLUE)You may also do any of these:"
	@$(ECHO)
	@$(ECHO) "  $(TXT_FG_WHITE)make targetenv_version$(TXT_FG_BRIGHT_GREEN)         - Makes a new formal version number and checks all into the EHS repo."
	@$(ECHO) "  $(TXT_FG_WHITE)make targetenv_deb$(TXT_FG_BRIGHT_GREEN)             - Is an optional step to create a debian package for linux systems."
	@$(ECHO) "  $(TXT_FG_WHITE)make targetenv_apk$(TXT_FG_BRIGHT_GREEN)             - Is an optional step to create an apk installer for android devices."
	@$(ECHO)

depend:
	./makedep.sh $(INC_DIRS)

# Host build tools (Xcode CLT / Homebrew / apt packages / git-lfs / make / curl /
# Docker). Machine scope, not target scope - run once per machine. Split out of
# prepdeps in 2026-08 so a regression sweep stops probing Homebrew and possibly
# running 'sudo apt' once per platform.
configure-host:
	@./target/envbuildscripts/configure_host.sh

# Dependency repos plus the per-target checks. Needs the make-exported environment
# (TARGET, EHS_ARCH, EHS_GNU_OS...), which is why these are here and not in
# ./configure - that is a plain shell script with no make variables.
# Set SKIP_REPOS=1 in the environment to run only the checks and skip the fetch.
prepdeps: chkconfig
	@./target/envbuildscripts/prepdeps.sh $(TARGET)

all_docker: chkconfig
	@./target/envbuildscripts/all_docker.sh $(TARGET)

targetenv: chkconfig
	@./target/envbuildscripts/targetenv.sh $(TARGET)

targetenv_prebuild: chkconfig
	@./target/envbuildscripts/targetenv_prebuild.sh $(TARGET)

targetenv_littlefs: chkconfig
	@./target/envbuildscripts/targetenv_littlefs_docker.sh $(TARGET)

# ── targetenv_package — proxy to the packager this target needs ──────────────
# `make targetenv_package` depends on whichever real packaging goal
# EHS_PACKAGER_TYPE names, so CI does not need to know which one a platform
# uses. A plain prerequisite, not a dispatcher: `make -n targetenv_package`
# shows exactly what will run.
#
# EHS_PACKAGER_TYPE is set per platform in target/platform/<TARGET>/config.mk,
# or defaulted per os-arch in target/os-arch/<os>-<arch>/config.mk. The os-arch
# config is included after the platform's own, so the default uses ?= and a
# platform setting wins. "none" is valid — a firmware image or a library has
# nothing to install.
EHS_PACKAGER_MAP_deb     := targetenv_deb_docker
EHS_PACKAGER_MAP_apk     := targetenv_apk
EHS_PACKAGER_MAP_nsis    := targetenv_nsis_docker
EHS_PACKAGER_MAP_esp32   := targetenv_esp32_docker
EHS_PACKAGER_MAP_xmos    := targetenv_xmos_docker
EHS_PACKAGER_MAP_arduino := targetenv_arduino
EHS_PACKAGER_KNOWN       := deb apk nsis esp32 xmos arduino none

# 'none' is a legitimate answer (firmware image or library -- nothing to
# install), and resolves to no prerequisite at all.
EHS_PACKAGER_GOAL := $(if $(filter-out none,$(EHS_PACKAGER_TYPE)),$(EHS_PACKAGER_MAP_$(EHS_PACKAGER_TYPE)))

# Validate only when this goal was actually asked for. A parse-time $(error)
# guarded on MAKECMDGOALS keeps a bad value from breaking every other target,
# while still failing loudly rather than packaging nothing.
ifneq ($(filter targetenv_package,$(MAKECMDGOALS)),)
    ifeq ($(EHS_PACKAGER_TYPE),)
        $(error EHS_PACKAGER_TYPE is not set and this os-arch declares no default. \
Set it in target/platform/$(TARGET)/config.mk, or add a default (with ?=) to \
target/os-arch/$(EHS_OS)-$(EHS_ARCH)/config.mk. Known types: $(EHS_PACKAGER_KNOWN))
    endif
    ifeq ($(filter $(EHS_PACKAGER_TYPE),$(EHS_PACKAGER_KNOWN)),)
        $(error Unknown EHS_PACKAGER_TYPE '$(EHS_PACKAGER_TYPE)'. \
Known types: $(EHS_PACKAGER_KNOWN). Add an EHS_PACKAGER_MAP_<type> entry in the \
Makefile if this is a new one.)
    endif
endif

targetenv_package: $(EHS_PACKAGER_GOAL)
ifeq ($(EHS_PACKAGER_TYPE),none)
	@$(ECHO) "$(TXT_FG_WHITE)EHS_PACKAGER_TYPE=none$(TXT_FG) - $(TARGET) ships no installable package (firmware image or library)."
else
	@$(ECHO) "$(TXT_FG_BRIGHT_GREEN)Packaged $(TARGET) via $(EHS_PACKAGER_GOAL) (EHS_PACKAGER_TYPE=$(EHS_PACKAGER_TYPE))$(TXT_FG)"
endif

targetenv_nsis: chkconfig
	@./target/envbuildscripts/targetenv_make_nsis.sh $(TARGET)

targetenv_nsis_docker: chkconfig
	@./target/envbuildscripts/targetenv_make_nsis_docker.sh $(TARGET)

targetenv_xmos: chkconfig
	@./target/envbuildscripts/targetenv_xmos.sh $(TARGET)

targetenv_xmos_docker: chkconfig
	@./target/envbuildscripts/targetenv_xmos_docker.sh $(TARGET)

# ---------------------------------------------------------------------------
# Zephyr CMake-master build targets
#
# Workflow:
#   1. make zephyr_cmake_gen    — resolves all source files, include dirs
#                                    and DEFS from the make variable state and
#                                    writes a Zephyr CMake app to
#                                    ../TARGET_TREES/ehs_env-<TARGET>/zephyr-staging/app/
#   2. make zephyr_prepdeps     — fetches the Zephyr/NCS west workspace inside
#                                    the Zephyr Docker image, run with
#                                    '--network host' (needs internet). No-op
#                                    if ZEPHYR_BASE is set or the workspace
#                                    already exists. Also runs automatically
#                                    as a prerequisite of the two build
#                                    targets below.
#   3. make zephyr_build_docker — runs 'west build' inside the Zephyr
#                                    Docker image and copies firmware to
#                                    ../TARGET_TREES/ehs_env-<TARGET>/bin/
#   Or natively (ZEPHYR_BASE must be set):
#   3. make zephyr_build       — same as above without Docker
#
# Future: zephyr_cmake_gen may become a conditional dependency of
#         targetenv_prebuild (and possibly all/all_docker) for Zephyr targets.
# ---------------------------------------------------------------------------

# Staging root for generated Zephyr app and west workspace
ERT_ZEPHYR_STAGING_DIR = ../TARGET_TREES/ehs_env-$(TARGET)/zephyr-staging

########################################################################################
# Default-application bundling
########################################################################################
# Embeds the SODL named by EHS_DEFAULT_APP into a generated header, which
# target_file.c writes to the filesystem on first boot. Needed on boards with no
# live app-upload path at bring-up.
#
# THE GENERATED HEADER IS TEMPORARY - it lands in the staging tree, never in the
# repo. What IS in the repo is target/os-arch/zephyr-arm/default_sodl.h (blinky),
# which stays as the permanent fallback: if the app tree is missing or the named
# app has no ERT1 export, the build silently uses that instead of failing. A
# board that boots blinky is diagnosable; a board that would not build is not.
EHS_APPS_PATH ?= $(EHS_ROOT_PATH)/../apps
EHS_DEFAULT_APP_SDL := $(wildcard $(EHS_APPS_PATH)/$(EHS_DEFAULT_APP)/export-ert1/t.sdl)
EHS_DEFAULT_APP_HDR  = $(ERT_ZEPHYR_STAGING_DIR)/app/default_sodl_generated.h

# The staging dir always holds a header, so target_file.c includes it
# unconditionally and there is NO preprocessor flag to select between two
# sources. That matters: a -D lives in the generated CMakeLists, which only
# zephyr_cmake_gen rewrites, so a flag-based scheme goes stale the moment
# someone runs just the build step - producing an image built from one app with
# the define pointing at the other. Copying the fallback into the same filename
# removes the failure mode rather than documenting it.
INC_DIRS += $(ERT_ZEPHYR_STAGING_DIR)/app


# Regenerate the embedded default-app header. A PHONY prerequisite of BOTH the
# generate and the build steps, deliberately: the header is derived from
# EHS_DEFAULT_APP in a config.mk, and editing that file must not leave a stale
# app baked into the image. Making only zephyr_cmake_gen do it means
# "edit config, make zephyr_build_docker" silently ships the previous app -
# a failure with no error message and a very confusing symptom on the bench.
.PHONY: ert_bundle_default_app
ert_bundle_default_app:
	@mkdir -p $(ERT_ZEPHYR_STAGING_DIR)/app
ifneq ($(EHS_DEFAULT_APP_SDL),)
	@$(ECHO) "$(TXT_FG_BRIGHT_GREEN)Bundling default app '$(EHS_DEFAULT_APP)'$(TXT_RESET)"
	@python3 ./scripts/sodl_to_header.py "$(EHS_DEFAULT_APP_SDL)" "$(EHS_DEFAULT_APP_HDR)" --app "$(EHS_DEFAULT_APP)"
else
	@$(ECHO) "$(TXT_FG_BRIGHT_YELLOW)No ERT1 export for EHS_DEFAULT_APP='$(EHS_DEFAULT_APP)' under $(EHS_APPS_PATH) - using the committed fallback app$(TXT_RESET)"
	@cp -f $(EHS_TARGETS_ROOT_PATH)/os-arch/zephyr-arm/default_sodl.h $(EHS_DEFAULT_APP_HDR)
endif

zephyr_cmake_gen: chkconfig ert_bundle_default_app
	$(file >.zephyr_vars.env.tmp,EHS_ROOT_PATH=$(EHS_ROOT_PATH))
	$(file >>.zephyr_vars.env.tmp,TARGET=$(TARGET))
	$(file >>.zephyr_vars.env.tmp,EHS_PLATFORM_PATH=$(EHS_PLATFORM_PATH))
	$(file >>.zephyr_vars.env.tmp,ERT_ZEPHYR_BASE_PLATFORM=$(ERT_ZEPHYR_BASE_PLATFORM))
	$(file >>.zephyr_vars.env.tmp,INC_DIRS=$(INC_DIRS))
	$(file >>.zephyr_vars.env.tmp,DEFS=$(DEFS))
	$(file >>.zephyr_vars.env.tmp,ERT_ZEPHYR_SOURCES=$(ERT_ZEPHYR_SOURCES))
	$(file >>.zephyr_vars.env.tmp,ERT_ZEPHYR_BOARD=$(ERT_ZEPHYR_BOARD))
	$(file >>.zephyr_vars.env.tmp,ERT_ZEPHYR_VERSION=$(ERT_ZEPHYR_VERSION))
	$(file >>.zephyr_vars.env.tmp,ERT_ZEPHYR_MANIFEST=$(ERT_ZEPHYR_MANIFEST))
	$(file >>.zephyr_vars.env.tmp,ERT_ZEPHYR_SYSBUILD=$(ERT_ZEPHYR_SYSBUILD))
	$(file >>.zephyr_vars.env.tmp,ERT_ZEPHYR_KCONFIG=$(ERT_ZEPHYR_KCONFIG))
	$(file >>.zephyr_vars.env.tmp,ERT_ZEPHYR_SYSBUILD_KCONFIG=$(ERT_ZEPHYR_SYSBUILD_KCONFIG))
	$(file >>.zephyr_vars.env.tmp,EHS_TOOLBOX_HASHES_VALUE=$(EHS_TOOLBOX_HASHES))
	$(file >>.zephyr_vars.env.tmp,EHS_GNU_OS_ARCH=$(EHS_GNU_OS_ARCH))
	$(file >>.zephyr_vars.env.tmp,ERT_SODL_VERSION=$(ERT_SODL_VERSION))
	@mkdir -p $(ERT_ZEPHYR_STAGING_DIR)/app/boards
	@mv .zephyr_vars.env.tmp $(ERT_ZEPHYR_STAGING_DIR)/make_vars.env
	@./target/envbuildscripts/zephyr_cmake_gen.sh $(TARGET)

zephyr_prepdeps: chkconfig
	@./target/envbuildscripts/zephyr_prepdeps_docker.sh $(TARGET)

# Staleness guard for the two-step Zephyr flow.
#
# zephyr_build/_docker deliberately do NOT depend on zephyr_cmake_gen: the gen
# step bakes TEST_FUNC and the whole DEFS/Kconfig set into the staging tree, so
# auto-running it would silently flip a TEST_FUNC build back to production (and
# vice versa) depending on which variables happened to be on the command line.
#
# The cost of that independence is that editing config.mk and then running only
# the build step compiles the PREVIOUS configuration, with no warning and a
# perfectly successful build. That has already cost one debugging session: a
# Wi-Fi SSID/PSK change in the platform config.mk never reached the firmware,
# which kept trying to join the old network. Fail loudly instead.
#
# Escape hatch: ERT_ZEPHYR_SKIP_STALE_CHECK=1 to build the staging tree as-is.
zephyr_check_staging_fresh:
	@if [ "$(ERT_ZEPHYR_SKIP_STALE_CHECK)" = "1" ]; then exit 0; fi; \
	prj="$(ERT_ZEPHYR_STAGING_DIR)/app/prj.conf"; \
	cml="$(ERT_ZEPHYR_STAGING_DIR)/app/CMakeLists.txt"; \
	if [ ! -f "$$prj" ] || [ ! -f "$$cml" ]; then \
	    echo "ERROR: no generated Zephyr app for TARGET=$(TARGET)."; \
	    echo "       Run:  make $(if $(TEST_FUNC),TEST_FUNC=$(TEST_FUNC) )zephyr_cmake_gen"; \
	    exit 1; \
	fi; \
	newer=$$(find TARGET.cfg $(EHS_PLATFORM_PATH) target/os-arch -name '*.mk' -newer "$$prj" -print 2>/dev/null; \
	         find TARGET.cfg -newer "$$prj" -print 2>/dev/null); \
	if [ -n "$$newer" ]; then \
	    echo "ERROR: the generated Zephyr app is older than your build configuration."; \
	    echo "       These inputs changed after the last 'zephyr_cmake_gen':"; \
	    echo "$$newer" | sed 's/^/         /'; \
	    echo "       Building now would compile the PREVIOUS configuration."; \
	    echo "       Run:  make $(if $(TEST_FUNC),TEST_FUNC=$(TEST_FUNC) )zephyr_cmake_gen"; \
	    exit 1; \
	fi; \
	gen_tf=$$(sed -n 's/.*EHS_TEST_FUNC_NAME=\([A-Za-z0-9_]*\).*/\1/p' "$$cml" | head -1); \
	if [ "$$gen_tf" != "$(TEST_FUNC)" ]; then \
	    echo "ERROR: TEST_FUNC mismatch between the generated app and this command."; \
	    echo "       generated with: TEST_FUNC=$${gen_tf:-<none, production build>}"; \
	    echo "       building with : TEST_FUNC=$(if $(TEST_FUNC),$(TEST_FUNC),<none, production build>)"; \
	    echo "       TEST_FUNC is baked in at generation time, so re-run:"; \
	    echo "       Run:  make $(if $(TEST_FUNC),TEST_FUNC=$(TEST_FUNC) )zephyr_cmake_gen"; \
	    exit 1; \
	fi

zephyr_build: chkconfig zephyr_check_staging_fresh zephyr_prepdeps
	@./target/envbuildscripts/zephyr_build.sh $(TARGET)

zephyr_build_docker: chkconfig ert_bundle_default_app zephyr_check_staging_fresh zephyr_prepdeps
	@./target/envbuildscripts/zephyr_build_docker.sh $(TARGET)

targetenv_esp32: chkconfig
	@./target/envbuildscripts/targetenv_esp32.sh $(TARGET)

targetenv_esp32_docker: chkconfig
	@./target/envbuildscripts/targetenv_esp32_docker.sh $(TARGET)

targetenv_arduino: chkconfig
	@./target/envbuildscripts/targetenv_arduino_docker.sh $(TARGET)

targetenv_apk_docker: chkconfig
	@./target/envbuildscripts/targetenv_make_apk_docker.sh $(TARGET)

targetenv_version: chkconfig
	@./target/envbuildscripts/targetenv_create_version_info.sh $(TARGET) INC_VERSION

targetenv_makeprod: chkconfig
	@./target/envbuildscripts/targetenv_makeprod.sh $(TARGET)

targetenv_deb: chkconfig
	@./target/envbuildscripts/targetenv_make_deb.sh $(TARGET) $(AUTO_START)

targetenv_deb_docker: chkconfig
	@./target/envbuildscripts/targetenv_make_deb_docker.sh $(TARGET)

targetenv_apk: chkconfig
	@./target/envbuildscripts/targetenv_make_apk.sh $(TARGET)

targetenv_android_dep_pack: chkconfig
	@./target/envbuildscripts/targetenv_android_dep_pack.sh $(TARGET)

targetenv_unity_export: chkconfig
	@./target/envbuildscripts/targetenv_unity_export.sh $(TARGET)

targetenv_unity_export_docker: chkconfig
	@./target/envbuildscripts/targetenv_unity_export_docker.sh $(TARGET)

targetenv_upload_appland: chkconfig
	@./target/envbuildscripts/targetenv_upload_appland.sh $(TARGET)

targetenv_upload_ota: chkconfig
	@./target/envbuildscripts/targetenv_upload_ota.sh $(TARGET)

# App tests are not make targets: they run across builds x apps, which make
# cannot express while bound to one TARGET.cfg.
#   ./SystemTests/CI/run_lucid_apps.sh --suite unit|system|external
# See SystemTests/apps/README.md.

targetenv_cleancfg: chkconfig
	@./target/envbuildscripts/targetenv_clean_config.sh $(TARGET)

targetenv_cleanall: chkconfig
	@./target/envbuildscripts/targetenv_clean_all.sh $(TARGET)

upload_ehs_deb: chkconfig #
	@./target/envbuildscripts/upload_ehs_deb.sh $(TARGET)

upload_ehs_sys_patch: chkconfig #upload
	@./target/envbuildscripts/EHS_syspatch_generate_upload.sh $(TARGET)

upload_server2server_OS_Update: chkconfig #
	@./target/envbuildscripts/EHS_server2server_update_generate_upload.sh $(TARGET)

upload_ehs_via_adb: chkconfig #
	@./target/envbuildscripts/upload_ehs_via_adb.sh $(TARGET)

#upload_devmanpatch: chkconfig # this doesn't seem to exist anymore
#	@./target/envbuildscripts/devmanserver_syspatch_devman.sh $(DEVMANSERVER)

toolsenv_update:
	@./target/envbuildscripts/toolsenv_update_cdf.sh

components_gendocs:
	@$(ECHO) "$(TXT_FG_CYAN)Generating markdown documentation for all CDF files...$(TXT_RESET)"
	@for cdf in $$(find Common/Components -name "*.cdf" -type f); do \
		dir=$$(dirname "$$cdf"); \
		base=$$(basename "$$cdf" .cdf); \
		docdir="$$dir/$$base/docs"; \
		mdfile="$$docdir/$$base.md"; \
		mkdir -p "$$docdir"; \
		$(ECHO) "  Generating: $$mdfile"; \
		python3 scripts/software-utilities/cdf_to_ascii.py "$$cdf" > "$$mdfile"; \
	done
	@$(ECHO) "$(TXT_FG_GREEN)Component documentation generation complete.$(TXT_RESET)"

# Pattern rule: .md files depend on their corresponding .cdf files
%.md: %.cdf
	@mkdir -p $(dir $@)
	@$(ECHO) "$(TXT_FG_CYAN)Regenerating: $@$(TXT_RESET)"
	@python3 scripts/software-utilities/cdf_to_ascii.py $< > $@

static_analysis: 
	@./target/envbuildscripts/static_analysis.sh

# Host unit tests: every UnitTest/*/Makefile that has a 'run' target. These
# build the real source against the host cc, so they need no target tree.
UNIT_TEST_DIRS := $(dir $(wildcard UnitTest/*/Makefile))

unit_tests:
	@rc=0; for d in $(UNIT_TEST_DIRS); do \
	    $(ECHO) "$(TXT_FG_CYAN)== unit test: $$d$(TXT_RESET)"; \
	    $(MAKE) -s -C $$d run || rc=1; \
	done; exit $$rc

unit_tests_asan:
	@rc=0; for d in $(UNIT_TEST_DIRS); do \
	    $(ECHO) "$(TXT_FG_CYAN)== unit test (asan): $$d$(TXT_RESET)"; \
	    $(MAKE) -s -C $$d asan || rc=1; \
	done; exit $$rc

# Per-function stack frames. Override the sources, and set a threshold to make
# it fail rather than just report:
#   make stack_report STACK_SRCS="Common/Components/core/stringfn.c" STACK_THRESHOLD=8192
STACK_SRCS      ?= $(wildcard Common/Components/core/*.c)
STACK_THRESHOLD ?= 0

stack_report:
	@./scripts/software-utilities/stack_report.sh \
	    --threshold $(STACK_THRESHOLD) $(STACK_SRCS)

publish_docker_image:
	@./target/envbuildscripts/publish_docker_image.sh

build_docker_local:
	@./target/envbuildscripts/build_docker_local.sh

target_buildenv:
	@./target/envbuildscripts/target_buildenv.sh

clean:
	find -name "*.o" -delete
	rm -f *.log
	rm -f *.map
	rm -f *.elf
	rm -f *.bin
	rm -f *.exe
	rm -f *.a
	rm -f *.d
	find -name "*.d" -delete
	rm -f $(TARGET_NAME).$(FINAL) $(CLEAN_FILES)
	@$(ECHO) "$(TXT_FG_WHITE)Target '$(TARGET_NAME)' cleaned"

.DEFAULT_GOAL := all
