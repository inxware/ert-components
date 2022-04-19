#!/bin/bash
# inx limited 2008

export SPECIFIC_TARGET=$1

echo "########################################################"
echo "  Build parameters: "
echo "--> $EHS_GNU_OS"
echo "--> $EHS_GNU_ARCH"
echo "--> $EHS_OS"
echo "--> $EHS_ARCH"
echo "########################################################"

########################################################################
# Check if we are building for android platform
if [[ "$SPECIFIC_TARGET" == *"android"* ]]; then
	./target/envbuildscripts/targetenv_android.sh $SPECIFIC_TARGET
	exit 0
fi

########################################################################
## Stop disturbances from cron to the environment if we are mapped to /root/ehs
crontab -r

# export UPLOAD variable to let basj use it for doing uploads (e.g. to dbian servers). 
export UPLOAD

########################################################
#create File system, including target specific parts and version information
./target/envbuildscripts/targetenv_directories.sh $SPECIFIC_TARGET

### EHS ################################################
if test -e ehs_$SPECIFIC_TARGET.$EXE; then 
	printf "ehs_"$SPECIFIC_TARGET.$EXE" present - OK\n"; 
	cp ehs_$SPECIFIC_TARGET.$EXE ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.exe
else 
	echo ehs_$SPECIFIC_TARGET.$EXE not present - please build using make TARGET=$SPECIFIC_TARGET - Exiting ... 
exit
fi

EHS_TOOLCHAIN_PATH_FROM_BASE="ert-build-support/toolchains/${TOOLCHAIN_PATH}"
EHS_TOOLCHAIN_LOCAL_PATH="${LOCAL_BASE}/${EHS_TOOLCHAIN_PATH_FROM_BASE}"
EHS_TOOLCHAIN_SERVER_PATH="${REPOSITORY}/${EHS_TOOLCHAIN_PATH_FROM_BASE}"
echo "XXXXXXXXXXXXXXXXX  Adding Supporting Target Binaries to EHS File System  XXXXXXXXXXXXXXXXX "
#Add user space support for EHS and components:
mkdir -p ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib
# toolchain sysroot shared libs
# note - may not exist for target
test -e ../ert-build-support/toolchains/${TOOLCHAIN_PATH}/sysroot/lib && cp -PR ../ert-build-support/toolchains/${TOOLCHAIN_PATH}/sysroot/lib/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib/ || echo "no toolchain sysroot support libraries found"
rm -f ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib/*.a
rm -Rf ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib/gcc
# additional clib libraries if built separately
if [ ! -n "${EHS_CLIB_OVERRIDE_PATH}" ];then
test -e "../ert-build-support/support_libs/target_libs/${EHS_GNU_ARCH}-${EHS_GNU_OS}${EHS_GNU_OS_VERSION}${EHS_SPECIAL_CLIB_EXT}/target_packages" && cp -PR "../ert-build-support/support_libs/target_libs/${EHS_GNU_ARCH}-${EHS_GNU_OS}${EHS_GNU_OS_VERSION}${EHS_SPECIAL_CLIB_EXT}/target_packages/"* "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin" || echo "no additional core libraries found"
else
echo "WARNING - using CLIB override path =${EHS_CLIB_OVERRIDE_PATH}"
test -e "../ert-build-support/support_libs/target_libsx/${EHS_CLIB_OVERRIDE_PATH}/target_packages" && cp -PR "../ert-build-support/support_libs/target_libs/${EHS_CLIB_OVERRIDE_PATH}/target_packages/"* "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin" || echo "no additional support libraries found"
fi
 
#and the component libraries

test -d "../ert-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/target_packages/" && (cp -PR --remove-destination "../ert-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/target_packages/"* "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/" || exit) || echo "£No component libraries found"


#create version information file - this maybe overwritten when code is submitted to a server
# -- don't update the version number
echo "XXXXXXXXXXXXXXXXX Setting the internal version for the build         XXXXXXXXXXXXXXXXXXXXX"
./target/envbuildscripts/targetenv_create_version_info.sh $SPECIFIC_TARGET

#### Add selected Host OS configuration scripts that should run at install (first install boot more precisely) ####
echo "XXXXXXXXXXXXXXXXX Adding Host OS configuration sripts            XXXXXXXXXXXXXXXXXXXXXXXXXX"
echo "Selected scripts: ${HOST_OS_CONFIG_SCRIPTS}"
echo ""
#Create the script directory if it isn't in the env
mkdir -p ../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/HostOsInit
#remove any directory place holders or residual runtime spam
rm -f ../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/HostOsInit/*
for i in ${HOST_OS_CONFIG_SCRIPTS}; do 
	cp ./target/envtree/${EHS_OS}-ehs-tree/system/HostOsInit/$i.* ../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/HostOsInit/
done 

echo "XXXXXXXXXXXXXXXXXX Looking for platform hacks script             XXXXXXXXXXXXXXXXXXXXXXXXXXX"
# Some devices have varying OSs features that need to be modified:
if  test -e ./target/envbuildscripts/targetenv_hacks_${SYSTEM_VARIANT}.sh 
then
echo "Applying Hack: /target/envbuildscripts/targetenv_hacks_${SYSTEM_VARIANT}.sh $SPECIFIC_TARGET"
	./target/envbuildscripts/targetenv_hacks_${SYSTEM_VARIANT}.sh $SPECIFIC_TARGET
fi



 
