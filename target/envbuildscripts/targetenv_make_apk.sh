#!/bin/bash
# inx limited 2020
# Creates an android .apk


echo "**************************************************************************************"
echo "**  Making Android APK - _* option - not standalone **"
echo "**************************************************************************************"

#For badly configured JAVA environments
unset JAVA_HOME

export SPECIFIC_TARGET=$1
export EHS_ROOT=`pwd` # assuming we're in the ehs project root
pushd ${EHS_ROOT}/..
export REPOSITORY_ROOT=`pwd`
popd
if [ -z "$SPECIFIC_TARGET" ]; then
    echo "TARGET is not specified."
    exit 1
fi
pushd ../TARGET_TREES || exit 1
TARGET_TREES=$(pwd)
popd
if ! [ -d "$TARGET_TREES" ]; then
    echo "TARGET_TREES directory is not available!"
    exit 1
fi
export TARGET_PATH=${TARGET_TREES}/ehs_env-${SPECIFIC_TARGET}
export ANDROID_STUDIO_ROOT=${TARGET_PATH}/android_studio_project
if ! [ -d $ANDROID_STUDIO_ROOT ]; then
    echo "Android Studio project folder ($ANDROID_STUDIO_ROOT) is not present in TARGET_TREES."
    echo "Please run 'make targetenv' before doing this."
    exit 1
fi
TARGET_ENV_APK_HACKS=${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk_hacks/"${SPECIFIC_TARGET}"_hacks.sh"
if ! [ -f "$TARGET_ENV_APK_HACKS" ]; then
    echo "Cannot do targetenv_apk for ANDROID ($SPECIFIC_TARGET). Hack script doesn't exist for this target."
	exit 1
fi

export TARGET_SYSPATCH=${TARGET_PATH}/syspatch

source $TARGET_ENV_APK_HACKS
echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

# override version
VERSION_DIR=${TARGET_PATH}/sysdata
if [ -d "$VERSION_DIR" ]; then
    cp -r ${VERSION_DIR} ${ANDROID_STUDIO_USERDATA_PATH} || exit 1
    NEW_VERSION=$( cat $ANDROID_STUDIO_USERDATA_PATH/sysdata/version.nfo )
    echo "Uploading EHS version : $NEW_VERSION"
else
    echo "EHS version not specified."
fi

TargetEnvMakeApk_Build

# create syspatch data
CreateDevmanAppUpdatesData

echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
