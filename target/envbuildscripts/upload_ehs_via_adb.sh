#!/bin/bash
if [ -z "$SUPERVISOR_PACKAGE_PATH" ]; then
echo "**************************************************************************************"
echo "**  Uploading Android APK - ONLY RUN ME AS A make targetenv_* option - not standalone **"
echo "**************************************************************************************"
fi

export SPECIFIC_TARGET=$1
export EHS_ROOT=`pwd` # assuming we're in the ehs project root

if [ -z "$SPECIFIC_TARGET" ]; then
    echo "TARGET is not specified."
    exit 1
fi    
TARGET_TREES=../TARGET_TREES
if ! [ -d "$TARGET_TREES" ]; then
    echo "TARGET_TREES directory is not available!"
    exit 1
fi    
export TARGET_PATH=${TARGET_TREES}/ehs_env-${SPECIFIC_TARGET}
export ANDROID_APK=${TARGET_PATH}/bin
if ! [ -d $ANDROID_APK ]; then
    echo "Android bin folder ($ANDROID_APK) is not present in TARGET_TREES."
    echo "Please do a proper build before doing this (run 'make help' for more details)."
    exit 1
fi
TARGET_ENV_APK_HACKS=${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk_hacks/"${SPECIFIC_TARGET}"_hacks.sh"
if ! [ -f "$TARGET_ENV_APK_HACKS" ]; then
    echo "Cannot do adb upload for ANDROID ($SPECIFIC_TARGET). Hack script doesn't exist for this target."
	exit 1
fi
echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
source $TARGET_ENV_APK_HACKS
SUPERVISOR_PACKAGE=
ADB_ADDRESS=
APK_DIRECTORY=${TARGET_PATH}/bin
if [ -n "$SUPERVISOR_PACKAGE_PATH" ]; then
    echo "Setting-up supervisor package."
    SUPERVISOR_PACKAGE="-pck $SUPERVISOR_PACKAGE_PATH"
fi
if [ -n "$ADB_IP" ]; then
    echo "Uploading via network ADB (ip=$ADB_IP)"
    ADB_ADDRESS="-ip $ADB_IP"
else
    if [ -z "$SUPERVISOR_PACKAGE" ]; then
        echo "Uploading via USB ADB"
    else
        echo "Creating a supervisor package for Devman upload."    
    fi
fi 
if [ -z "$EHS_PRODUCT_NAME" ]; then
    echo "EHS product name e.g ambifier, adnoc, tellisign etc. must be specified in the hack file."
    exit 1
fi
if [ -z "$SYSTEM_VARIANT" ]; then
    echo "EHS android platform system variant (e.g pine64_a6, pine64_h6) must be specified in the platform confing.mk."
    exit 1
fi 
if [ -z "$ANDROID_VERSION_NUMBER" ]; then
    echo "Android version number must be specified in the platform confing.mk."
    exit 1
fi
if [ -z "$DEVMAN_SERVER_URL" ]; then
    echo "Devman server URL wasn't specified."
    exit 1
fi 
if [ -z "$DEVMAN_SERVER_NAME" ]; then
    echo "Devman server name (DEVMAN_SERVER_NAME) wasn't specified."
    exit 1
fi
if [ -z "$APK_DIRECTORY" ]; then
    echo "APK directory is not available in the TARGET_TREES for this target."
    exit 1
fi

export INSTALL_DIR=${EHS_ROOT}/target/envtree/android-ehs-tree

echo "======================================================================================================="
echo "PRODUCT=${EHS_PRODUCT_NAME}, PLATFORM=${SYSTEM_VARIANT}, ANDROID=${ANDROID_VERSION_NUMBER}, SERVER=${DEVMAN_SERVER_NAME}"
echo "======================================================================================================="
sleep 2
$INSTALL_DIR/adb_install.sh -r -pr ${EHS_PRODUCT_NAME} -pl ${SYSTEM_VARIANT} -av ${ANDROID_VERSION_NUMBER} -sn ${DEVMAN_SERVER_NAME} -ad ${APK_DIRECTORY} ${ADB_ADDRESS} ${SUPERVISOR_PACKAGE}

echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
