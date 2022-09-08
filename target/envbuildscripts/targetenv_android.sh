#!/bin/bash

export SPECIFIC_TARGET=$1
export EHS_ROOT=`pwd` # assuming we're in the ehs project root
pushd ${EHS_ROOT}/..
export REPOSITORY_ROOT=`pwd`
popd
echo ""
echo "***** targetenv for ANDROID ($SPECIFIC_TARGET) *****"
echo ""
if [ -z "$SPECIFIC_TARGET" ]; then
    echo "SPECIFIC_TARGET is not specified."
    exit 1
fi
pushd ../TARGET_TREES || exit 1
TARGET_TREES=$(pwd)
popd
if ! [ -d "$TARGET_TREES" ]; then
    echo "TARGET_TREES directory is not available!"
    exit 1
fi
TARGET_PATH=${TARGET_TREES}/ehs_env-${SPECIFIC_TARGET}
if [ -d $TARGET_PATH ]; then
    echo "Clear TARGET directory ($TARGET_PATH)"
    rm -r "$TARGET_PATH"
fi
echo "Create TARGET directory ($TARGET_PATH)"
mkdir ${TARGET_PATH}
# check if target only intends to build ehs library plugin
if [ "$EHS_PLUGIN_LIBRARY_BUILD" == "yes" ]; then
    echo "Target "
    echo "Copy plugin library to ===> $TARGET_PATH/lib"
    mkdir ${TARGET_PATH}/lib || exit 1
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
fi
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

export TARGET_SYSPATCH=${TARGET_PATH}/syspatch
if [ -d "$TARGET_SYSPATCH" ]; then
    rm -rf ${TARGET_SYSPATCH} || exit 1
fi
mkdir ${TARGET_SYSPATCH} || exit 1

TARGET_ENV_APK_HACKS=${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk_hacks/"${SPECIFIC_TARGET}"_hacks.sh"
if ! [ -f "$TARGET_ENV_APK_HACKS" ]; then
    echo "Cannot do targetenv for ANDROID ($SPECIFIC_TARGET). Hack script doesn't exist for this target."
    rm -rf ${TARGET_PATH}
	exit 1
fi
echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
source $TARGET_ENV_APK_HACKS
# run setup targetenv function sourced from the hack file
SetupTargetEnv
echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
# copy ehs app sourced from the hack file
SetupTargetEnv_CopyEHSTools
# setup devman url
SetupTargetEnv_Url
# setup devman certs
SetupTargetEnv_Certs
# setup bin folder
SetupTargetEnv_BinFolder

if [ -z "$BUILD_WITHOUT_SUPERVISOR" ]; then
    # create supervisor
    export SUPERVISOR_PACKAGE_PATH=${TARGET_PATH}/supervisor
    if [ -d "$SUPERVISOR_PACKAGE_PATH" ]; then
        rm -rf ${SUPERVISOR_PACKAGE_PATH} || exit 1
    fi
    mkdir ${SUPERVISOR_PACKAGE_PATH} || exit 1

#todo2022 - see coment for deconflating the instal and the prperation of target platforms. This is very messy!
# we just want targetenv to do what it usually does  - stage the apps etc . in the  ../TARGET_TREE/ staging areas rady for the  package builder
    # use script to pack supervisor
    ${EHS_ROOT}/target/envbuildscripts/upload_ehs_via_adb.sh ${SPECIFIC_TARGET}
    # create syspatch data
    CreateDevmanSupervisorUpdatesData
else
    echo "Building for an unmanaged target without supervisor."
fi

echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
