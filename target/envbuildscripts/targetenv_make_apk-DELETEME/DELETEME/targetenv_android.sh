#!/bin/bash

# This script Adds some magical things to the staging directory, including another hidden one for the build
# Generally it does something like this:
# 1. Creates the TARGET_TREES staging directory - Not any more
# 2. Deletes anything that was in it if was there before - Not any more
# 3. Checks if a plugin only build - if it is -> exits nicely and do no more
# 4. Creates an android studio project in the staging directory just created.
# 5. Creates a Devman syspatch directory with the Devman syspatch style installer scripts copied in.
# 6. 

export SPECIFIC_TARGET=$1
export EHS_ROOT=`pwd` # assuming we're in the ehs project root
pushd ${EHS_ROOT}/..
export REPOSITORY_ROOT=`pwd`
popd
echo "*************************************************************"
echo "***** BUild target environment for ANDROID ($SPECIFIC_TARGET)"
echo "*************************************************************"

if [ -z "$SPECIFIC_TARGET" ]; then
    echo "SPECIFIC_TARGET is not specified."
    exit 1
fi
pushd ../TARGET_TREES || exit 1
TARGET_TREES=$(pwd)
popd

if ! [ -d "$TARGET_TREES" ]; then
    echo "TARGET_TREES directory could not be created"
    exit 1
fi

TARGET_PATH=${TARGET_TREES}/ehs_env-${SPECIFIC_TARGET}

#if [ -d $TARGET_PATH ]; then
#    echo "Clear TARGET directory ($TARGET_PATH)"
#    rm -r "$TARGET_PATH"
#fi

echo "Create TARGET directory ($TARGET_PATH)"
#todo 2023 this needs to be changed to use the .so from the TARGET_TREES directory
#todo2023 - should this be done in the targetenv stage (i.e. not an this apk build)
#mkdir -p ${TARGET_PATH}
# check if target only intends to build ehs library plugin
if [ "$EHS_PLUGIN_LIBRARY_DEPENDENCY" = "yes" ]; then
    echo "Target"
    echo "Copy plugin library to ===> $TARGET_PATH/lib"
    mkdir -p ${TARGET_PATH}/lib || exit 1
    LIBRARY_FILE=ehs_${SPECIFIC_TARGET}.so
    if [ -f "$LIBRARY_FILE" ]; then
        cp ${LIBRARY_FILE} ${TARGET_PATH}/lib || exit 1
        make clean
        echo "SUCCESS!"
        exit 0
    else
        echo "FAILED! Plugin file $LIBRARY_FILE doesn't exists!"
        rm -r "$TARGET_PATH"
        exit 1
    fi
else
    echo "This target is NOT a plugin Library build..."
fi

# Todo2022 0 the following should be set to some relative path of the platform in ../TARGET_TREES

export ANDROID_STUDIO_ROOT=${TARGET_PATH}/android_studio_project

if ! [ -d $ANDROID_STUDIO_ROOT ]; then
    echo "Create Android Studio project folder ($ANDROID_STUDIO_ROOT)"
    mkdir ${ANDROID_STUDIO_ROOT}
fi

if ! [ -d $ANDROID_STUDIO_ROOT ]; then
    echo "Failed to create Android Studio project folder ($ANDROID_STUDIO_ROOT)"
    exit 1
fi
mkdir ${TARGET_PATH}/bin

#todo what is this for?
export TARGET_SYSPATCH=${TARGET_PATH}/syspatch
if [ -d "$TARGET_SYSPATCH" ]; then
    rm -rf ${TARGET_SYSPATCH} || exit 1
fi
mkdir ${TARGET_SYSPATCH} || exit 1

# ToDo - why do we source the specific hack file here (before we build the apk?
TARGET_ENV_APK_HACKS=${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk/"${SPECIFIC_TARGET}"_hacks.sh"
if ! [ -f "$TARGET_ENV_APK_HACKS" ]; then
    echo "Cannot do targetenv for ANDROID ($SPECIFIC_TARGET). Hack script doesn't exist for this target."
    rm -rf ${TARGET_PATH}
	exit 1
fi

echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
echo " Running target specific hack file : ${TARGET_ENV_APK_HACKS}"
source $TARGET_ENV_APK_HACKS
# run setup targetenv function sourced from the hack file
SetupTargetEnv
echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
# copy the already assembled runtime from the staging directory 
echo "Making the Android Staging directory"

#SetupTargetEnv_Certs
# setup bin folder
# How does this work? What does it depend on and what does it output?
SetupTargetEnv_AddSuplimentaryApps <AppName>

if [ "$BUILD_WITH_SUPERVISOR" !="" ]; then
    # create supervisor
    export SUPERVISOR_PACKAGE_PATH=${TARGET_PATH}/supervisor
    if [ -d "$SUPERVISOR_PACKAGE_PATH" ]; then
        rm -rf ${SUPERVISOR_PACKAGE_PATH} || exit 1
    fi
    mkdir ${SUPERVISOR_PACKAGE_PATH} || exit 1

#TODO2024 - WHat does this script actually do? Why we are calling an upload script during a targetenv operation?

    # use script to pack supervisor
    ${EHS_ROOT}/target/envbuildscripts/upload_ehs_via_adb.sh ${SPECIFIC_TARGET}
    # create syspatch data
    CreateDevmanSupervisorUpdatesData
else
    echo "WARNING!!!: Building for an unmanaged target without supervisor."
fi

echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

