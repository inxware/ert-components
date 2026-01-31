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

export EHS_COMPONENT_SUPPORT_BASE=$(EHS_ROOT_PATH)/../ert-contrib-middleware
export EHS_CORE_SUPPORT_BASE=$(EHS_ROOT_PATH)/../ert-build-support

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

.PHONY: chkconfig
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
	@$(ECHO) "The following dependencies are available for this build with the kernel at ${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}"
	@$(ECHO) "meld ./target/platform/$(TARGET)/ ../EHS-kernel/target/platform/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/"
	@meld "./target/platform/$(TARGET)/ ../EHS-kernel/target/platform/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/"
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
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv$(TXT_FG_BRIGHT_GREEN)          - Creates the target runtime file structure in the staging directory ../TAREGET_TREES/"
	@$(ECHO) "  $(TXT_FG_GREEN)                     + Use make targetenv HOST_OS_CONFIG_SCRIPTS_EXTRA=\"XXX-ABCD YYY-EFGH\" to include additional OS config."
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_package$(TXT_FG_BRIGHT_GREEN)  - Creates the target runtime package using the installer method speficied by the platform/config.mk"
	@$(ECHO)
	@$(ECHO) "--------------------------------------------------------------------"
	@$(ECHO)
	@$(ECHO) "$(TXT_FG_BLUE)Build Diagnostics:"
	@$(ECHO)
	@$(ECHO) "  $(TXT_FG_WHITE)chkconfig$(TXT_FG_BRIGHT_GREEN)            - Shows the current key config parameters implied by the platform/<TARGET>config.mk"
	@$(ECHO) "  $(TXT_FG_WHITE)compare_kernelconfig$(TXT_FG_BRIGHT_GREEN) - Compares platform/<TARGET>/config.mk with the one in ../EHS-kernel/targete/platform/<OS ARCH VERSION>/"
	@$(ECHO) "  $(TXT_FG_WHITE)chk_ext_deps$(TXT_FG_BRIGHT_GREEN)         - Shows the external dependencies met or unmet for the platform configuration."
	@$(ECHO_N) "  $(TXT_FG_WHITE)depend$(TXT_FG_BRIGHT_GREEN)               - "
	@$(WARN) "this updates the source level dependencies and will update the deps.mk make files."
	@$(ECHO)
	@$(ECHO) "--------------------------------------------------------------------"
	@$(ECHO)
	@$(ECHO) "$(TXT_FG_BLUE)Other make targets:"
	@$(ECHO)
	@$(ECHO) "  $(TXT_FG_WHITE)all_docker$(TXT_FG_BRIGHT_GREEN)                     - Makes ehs_$(TARGET).exe for host or docker enviorment and copied TARGETENV bin as ehs.exe"
	@$(ECHO) "  $(TXT_FG_WHITE)publish_docker_image$(TXT_FG_BRIGHT_GREEN)           - Build new docker image and publish it to inxware dockerhub organization."
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
	@$(ECHO) "  $(TXT_FG_WHITE)targetenv_run_tests$(TXT_FG_BRIGHT_GREEN)            - Runs all regression tests."
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

targetenv_package: chkconfig
	@./target/envbuildscripts/targetenv_make_package.sh $(TARGET)

targetenv_nsis: chkconfig
	@./target/envbuildscripts/targetenv_make_nsis.sh $(TARGET)

targetenv_nsis_docker: chkconfig
	@./target/envbuildscripts/targetenv_make_nsis_docker.sh $(TARGET)

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

targetenv_run_tests: chkconfig
	@./target/envbuildscripts/targetenv_run_tests.sh $(TARGET)

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

publish_docker_image:
	@./target/envbuildscripts/publish_docker_image.sh

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
