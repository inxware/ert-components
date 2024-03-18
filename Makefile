#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#


# Makefile for ert-components

# @author: inx limited
# 
# Copyright (c) inx Ltd, 2022. All rights reserved.
#
# Overrides from command line:
# TARGET - specify a different target
# REVOPT=-V - if a build with the revision number is required.
# TK_DEPRECATE=no if the deprecated toolkit is not required in this build
#
# Targets:
#  clean
#  depend - recreate file dependencies


#todo to stop the script bailing out the other includes should be prepended with -, but this maybe a bit obscure for now
-include ./TARGET.cfg   

export TARGET
# if passed on the command ine then export
#export OBJDIRECTORY=./OBJECTS/$(TARGET)#
export TARGET_NAME=ehs_$(TARGET)#

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

#Get OS specfic HAL code

include $(EHS_PLATFORM_PATH)/../platform.mk

.PHONY: chkconfig
.PHONY: help
#.PHONY: $(OBJDIRECTORY)
############################ make targets #####################################################
# First rule: defines what to build
#

all: $(TARGET_NAME).$(FINAL) chkconfig 

#$(OBJDIRECTORY):
#	@mkdir -p $(OBJDIRECTORY)
	

# Can't use predefined implicit rules here - we might need to generate .obj, rather than .o
# therefore create custom implicit rules.
%.$(OBJ): %.s
	@echo $(AS) $(ASFLAGS) $<
	@$(AS) $(ASFLAGS) $< -o $@

%.$(OBJ): %.c
	@echo $(CC) $(CC_SWITCHES) $(CFLAGS) $<
	@$(CC) $(CC_SWITCHES) $(CFLAGS) $< -o $@

%.$(OBJ): %.cpp 
	@echo $(CC) $(CC_SWITCHES) $(CPPFLAGS) $<
	$(CPP) $(CC_SWITCHES) $(CPPFLAGS) $< -o $@

$(TARGET_NAME).$(FINAL) : $(OBJECTS)
#	@echo $(PWD)
#	@echo Linking with Flags: $(LINK) $(LNKFLAGS) $<
	$(LINK) $(LD_SWITCHES) $(OBJECTS) $(LNKFLAGS) 
# -Wl,-Bdynamic -- if dynamic needs forcing?
#    echo will try copying ehs binary to canonical location for debug - Will fail if link from /root/ehs to TARGET env has not been created
#	@mkdir -p ./objects_$(TARGET) 
ifdef EHS_ANDROID
ifdef EHS_ANDROID_JNI
#todo remove this if we no longer support JNI meandroid builds or change it to use the staging directory if we do
	@cp -f $(TARGET_NAME).$(FINAL) ./target/os-arch/android_ALL/android-java-project/libs/armeabi/lib$(TARGET_NAME).$(FINAL)
	@echo copied to ./target/os-arch/android_ALL/android-java-project/libs/armeabi/lib$(TARGET_NAME).$(FINAL)
endif
#	@mkdir -p ../TARGET_TREES/ehs_env-$(TARGET)/ todo - look for paths used in esoteric android targetenv scripts.
#	@cp -f $(TARGET_NAME).$(FINAL) ../TARGET_TREES/ehs_env-$(TARGET)/
#	@echo copied to ../TARGET_TREES/ehs_env-$(TARGET)/
else
	@mkdir -p ../TARGET_TREES/ehs_env-$(TARGET)/bin
	@cp -f $(TARGET_NAME).$(FINAL) ../TARGET_TREES/ehs_env-$(TARGET)/bin/ehs.exe
	@echo copied to ../TARGET_TREES/ehs_env-$(TARGET)/bin/ehs.exe
endif
	

#end of  all:
############################  Environment & Repository Scripts ##############################################
#@todo the following should be tidied so that a single base script is called and that searches for any more specific scripts from the TARGET argumenent
#This would make it easier to come up with new default targets without having to create a new scripts
# rebuild all dependencies

chkconfig:
ifndef TARGET
	@echo "No TARGET specified. Please run ./confgure for a list of available targets"
	@echo "and then run ./configure [selected TARGET]".
	@echo or use
	@echo make TARGET=[selected TARGET] [required TARGET] 
else
	@echo "The following dependencies are available for this build"
	@./target/envbuildscripts/buildenv_checkpaths.sh
endif


compare_kernelconfig:
ifndef TARGET
	@echo "No TARGET specified. Please run ./confgure for a list of available targets"
	@echo "and then run ./configure [selected TARGET]".
	@echo or use
	@echo make TARGET=[selected TARGET] [required TARGET] 
else
	@echo "The following dependencies are available for this build with the kernel at  ${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}"
	@echo "meld ./target/platform/$(TARGET)/ ../EHS-kernel/target/platform/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/"
	@meld ./target/platform/$(TARGET)/ ../EHS-kernel/target/platform/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/
endif



help:
	@echo "******************************************************************************************************************************"
	@echo "*                                 MAKE HELP FOR inxware runtime software" 
	@echo "* Make Targets in order of usual execution:"
	@echo "* "
	@echo "* prepdeps           - Checksout dependencies git (unless SKIP_REPOS=yes)"
	@echo "* all                - makes ehs_$(TARGET).exe and copied TARGETENV bin as ehs.exe "
	@echo "* targetenv          - Creates the target runtime file structure in the staging directory ../TAREGET_TREES/"
	@echo "*                        - use make targetenv HOST_OS_CONFIG_SCRIPTS_EXTRA=\"XXX-ABCD YYY-EFGH\"  to include additional OS config"
	@echo "* targetenv_package  - Creates the target runtime package using the installer method speficied by the platform/config.mk"
	@echo "* ---------------------------------------------------------------------------------------------------------------------------"
	@echo "* BUILD Diagnostics:"
	@echo "* chkconfig    		  - Shows the current key config parameters implied by the platform/<TARGET>config.mk"
	@echo "* compare_kernelconfig - Compares  platform/<TARGET>/config.mk with the one in ../EHS-kernel/targete/platform/<OS ARCH VERSION>/"
	@echo "* chk_ext_deps		  - SHows the external dependencies met or unmet for the platform configuration"
	@echo "* depend			- !!WARNING!! this updated the source level dependencies and update the deps.mk make files"
	@echo "* ---------------------------------------------------------------------------------------------------------------------------"
	@echo "* all_docker	       - makes ehs_$(TARGET).exe for host or docker enviorment and copied TARGETENV bin as ehs.exe "
	@echo "* publish_docker_image - Build new docker image and publish it to inxware dockerhub organization"
	@echo "* target_buildenv      - Start the platform's DOCKER environment shell.  Useful during build system tuning."
	@echo "* targetenv_version    - Create a new version number for the target. Note this will check in all changes and create a tagged commit"
	@echo "* targetenv_cleanall   - Removes ALL data and directories from ../TARGET_TREES/ehs_env-$(TARGET)"
	@echo "* targetenv_cleancfg   - Removes all user data from the TARGETENV tree for deployment."
	@echo "*                      - Set env variable KEEP_USERCONFIG=yes to keep the userdata/configuration data in tact."
	@echo "*                      - Set env variable KEEP_DEVMANCONFIG=yes to keep the devman servers in tact."
	@echo "*                      - Set env variable KEEP_APPLICATION=yes to keep the appdata in tact."
	@echo "* targetenv_makeprod   - Configures the runtime with standard INX apps and devman configuration. Cleans existing config first! "
	@echo "* targetenv_pre_build  - Run everything that is needed before building traget"
	@echo "* targetenv_pre_build_docker - Run make targetenv_pre_build in docker image."
	@echo "* targetenv_deb        - Creates a debian installer for current tree (targetted at /opt/ehs). optional: UPLOAD=<deb repo URL>"
	@echo "* targetenv_apk        - Builds android APK and stores it in ../TARGET_TREES/"
	@echo "* targetenv_apk_docker - Builds android APK and stores it in ../TARGET_TREES/ in an android arm configured docker image."
	@echo "* targetenv_unity_export - Exports Unity 3D IDE (C#) based project to eRT compatible project/exe e.g. eRT Android Studio project or Windows app with eRT plugin."
	@echo "* targetenv_unity_export_docker - Same as above but in docker."
	@echo "* targetenv_esp32      - Builds an esp32 image for subsequent deployment via usb or OTA deployment"
	@echo "* targetenv_esp32_docker    - runs make targetenv_esp32 in an esp32s3 configured docker image."
	@echo "* targetenv_esp32s3     - Builds an esp32s3 image for subsequent deployment via usb or OTA deployment"
	@echo "* targetenv_esp32s3_docker  - runs make targetenv_esp32s3 in an esp32s3 configured docker image."
	@echo "* targetenv_nsis_docker - Builds a windows installer using the NSIS installer"
	@echo "* upload_ehs_via_adb   - Uploads apks to the connected (or IP mapped) android device via adb. optional: ADB_IP=<device ip>"
	@echo "* upload_ehs_deb       - Uploads the debian package created targetenv_deb. Set  UPLOAD=<deb repo URL>"
	@echo "* targetenv_android_dep_pack - Bundles eRT android supplementary apps, supervisor into Devman uploadable packages (no APKs are built)."
	@echo "* upload_ehs_sys_patch - Uploads TARGETENV tree package (Linux and Android only - FREERTOS fimrware images too?) to a Devman server"
	@echo "*                      - Use VERSION_NAME=[your version name] to give the build a special name."
	@echo "*                      - e.g. make DEVMANSERVER=[your.url.com] DEVMANUID=[your username] upload_ehs_sys_patch."
	@echo "*                      - If the patch requires a server reboot (i.e. because it has a new start-upo script) then"
	@echo "*                        set an additional variable SYSPATCH_NEED_REBOOT=yes on the command line."
	@echo "*                        (KEEP_USERCONFIG=yes & KEEP_APPLICATION=yes can also be used here as described above)."
	@echo "*                      - No arguments are required for ANDROID builds. These are deployed to devices using update-to-latest-xxxxx-android" 
	@echo "* upload_server2server_OS_Update - This will install an update to the host server DEVMAN_INTERMEDIATE_SERVER=[your.url.com] that can be deployed to a slave" 
	@echo "*                             - (e.g. fire-walled) devman instance. One deployed from the host server the packages will become the OS "
	@echo "*                             - update patches on the final distation. You may also set DEVMAN_INTERMEDIATE_UNAME & DEVMAN_INTERMEDIATE_SSHPORT"
	@echo "* toolsenv_update      - Updates the dist directory's IDF and CDF directories with this EHS's version component description files"
	@echo "* static_analysis      - runs rhe static analyser suite on the full source code tree for all configurations."
	@echo "*"
	@echo "*"
	@echo "* The usual sequence to build a packge is:"
	@echo "* ./configure [a target]  # targets can be identified with ./configure help"
	@echo "* make prepdeps 	         # checkout the platform dependencies and toolchain for the target"
	@echo "* make all                # makes ehs and will update a runtime tree if one exists (see next)"
	@echo "* make targetenv          # makes a runtime tree with all excutables and assets for the specific target in ../TARGET_TREES/*"
	@echo "* make targetenv_version  # makes a new formal version number and checks all into the EHS repo."
	@echo "* make targetenv_deb      # is an optional step to create a debian package for linux systems."
	@echo "* or"
	@echo "* make targetenv_apk      # is an optional step to create an apk installer for android devices."
	@echo "*"
	@echo "*******************************************************************************************************************************"

depend:
	./makedep.sh $(INC_DIRS)
prepdeps: chkconfig
	@./target/envbuildscripts/prepdeps.sh $(TARGET)
all_docker: chkconfig
	@./target/envbuildscripts/all_docker.sh $(TARGET)
targetenv: chkconfig
	@./target/envbuildscripts/targetenv.sh $(TARGET) 
targetenv_pre_build: chkconfig
	@./target/envbuildscripts/targetenv_pre_build.sh $(TARGET)
targetenv_pre_build_docker: chkconfig
	@./target/envbuildscripts/targetenv_pre_build_docker.sh $(TARGET)
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
targetenv_esp32s3: chkconfig
	@./target/envbuildscripts/targetenv_esp32s3.sh $(TARGET) 
targetenv_esp32s3_docker: chkconfig
	@./target/envbuildscripts/targetenv_esp32s3_docker.sh $(TARGET) 
targetenv_apk_docker: chkconfig
	@./target/envbuildscripts/targetenv_make_apk_docker.sh $(TARGET)
targetenv_version: chkconfig
	@./target/envbuildscripts/targetenv_create_version_info.sh $(TARGET) INC_VERSION
targetenv_makeprod: chkconfig
	@./target/envbuildscripts/targetenv_makeprod.sh $(TARGET) 
targetenv_deb: chkconfig
	@./target/envbuildscripts/targetenv_make_deb.sh $(TARGET)
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
targetenv_cleancfg: chkconfig
	@./target/envbuildscripts/targetenv_clean_config.sh $(TARGET)
targetenv_cleanall: chkconfig
	@./target/envbuildscripts/targetenv_clean_all.sh $(TARGET)
upload_ehs_deb: chkconfig #
	@./target/envbuildscripts/upload_ehs_deb.sh $(TARGET)
upload_ehs_sys_patch: chkconfig #
	@./target/envbuildscripts/EHS_syspatch_generate_upload.sh $(TARGET)
upload_server2server_OS_Update: chkconfig #
	@./target/envbuildscripts/EHS_server2server_update_generate_upload.sh $(TARGET)
upload_ehs_via_adb: chkconfig #
	@./target/envbuildscripts/upload_ehs_via_adb.sh $(TARGET)	
#upload_devmanpatch: chkconfig # this doesn't seem to exist anymore
#	@./target/envbuildscripts/devmanserver_syspatch_devman.sh $(DEVMANSERVER)
toolsenv_update: 
	@./target/envbuildscripts/toolsenv_update_cdf.sh
static_analysis: 
	@./target/envbuildscripts/static_analysis.sh
publish_docker_image: 
	@./target/envbuildscripts/publish_docker_image.sh
target_buildenv: 
	@./target/envbuildscripts/target_buildenv.sh
clean:
	find -name "*.o" -delete
	rm -f *.d
	find -name "*.d" -delete
	rm $(TARGET_NAME).$(FINAL) $(CLEAN_FILES)
	
.DEFAULT_GOAL := all
	
