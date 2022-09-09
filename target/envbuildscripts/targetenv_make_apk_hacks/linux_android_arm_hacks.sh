#!/bin/bash
set -e
BUILD_MODE="$1"

echo "DEBUG_BUILD: In linux_android_arm_hacks.sh"

#todo2022 - do we want this here?
export BUILD_WITHOUT_SUPERVISOR="1"

export REPOSITORY_ANDROID_STUDIO_ROOT="$EHS_ROOT/target/os-arch/android_ALL/android_studio_ehs"
export ANDROID_STUDIO_JNILIBS_PATH="$ANDROID_STUDIO_ROOT/app/src/main/jniLibs/armeabi-v7a"
export ANDROID_STUDIO_USERDATA_PATH="$ANDROID_STUDIO_ROOT/app/src/main/assets/userdata"
export ANDROID_STUDIO_DEVMAN_PATH="$ANDROID_STUDIO_USERDATA_PATH/devman/core"
export ANDROID_STUDIO_TOOLS_PATH="$ANDROID_STUDIO_USERDATA_PATH/appdata/default"

#The following is required only if publishing APKs to private servers.
export SSHPORT=8822
export DEVMAN_SERVER_NAME="inx"
export DEVMAN_SERVER_DOMAIN="devman-inx-systems.net"
export DEVMAN_SERVER_URL="https://$DEVMAN_SERVER_DOMAIN"
export DEVMAN_UNAME="inx"
export EHS_PRODUCT_NAME="ehs" # todo - change product to eRT (needs to be done accross all scripts/templates)

source ${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk_hacks/targetenv_make_apk_utils.sh"
source ${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk_hacks/targetenv_make_apk_setup.sh"

#This seem to only install the default brix-style home appplication not the target specific apps.
#todo2022 we should have a single script that downloads the app that is specified in the config.mk rather than hiding config in code in this exotic spot.
# OK seems to have worked ot this function might not be the function that actually gets calles as there are lots of other hack files with this name..
#todo 2022 - let 
SetupTargetEnv_CopyEHSTools(){
    echo "INFO: copying app from systemapps/Home/export/"
    TOOLS_DIR=${EHS_ROOT}/../apps/
    #todo2022: I suppose this is yet another way of downloading the app repo, but we should have one way that does it for all target types:
    GetApplicationRepo $TOOLS_DIR
    echo "INFO: Copying from $ANDROID_STUDIO_TOOLS_PATH to "
    if [ -d "$TOOLS_DIR" ] && [ -d "$ANDROID_STUDIO_TOOLS_PATH" ]; then
        echo "Copying the Default eRT home app to the project."
        cp -Rf ${EHS_ROOT}/../apps/systemapps/Home/export/* ${ANDROID_STUDIO_TOOLS_PATH} || CancelFailed
    else
        echo "Failed to copy the default eRT App app!"
        CancelFailed   
    fi
}

SetupTargetEnv_Certs(){
    # override with certificates
    echo "INFO: Setting up ambifier server certificates"
    CERTS_DIR=${EHS_ROOT}/../DevmanSecurity/devman.inx-systems.com
    cp -f ${CERTS_DIR}/devman-ca.crt ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-ca.crt || CancelFailed
    cp -f ${CERTS_DIR}/devman-client-crt-key.pem ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-client-crt-key.pem || CancelFailed
}

SetupTargetEnv_BinFolder(){
    # nothing to do here
    :
}

#todo - change the targe apk name to ert.apk
TargetEnvMakeApk_Build(){
    echo "Building eRT APK for ambifier"
    $EHS_ROOT/target/envbuildscripts/targetenv_make_apk_hacks/targetenv_make_apk_gradlew_build.sh "$SPECIFIC_TARGET" "$ANDROID_STUDIO_ROOT"
    EHS_APK=${TARGET_PATH}/release/app-release.apk
    cp ${EHS_APK} ${TARGET_PATH}/bin/ehs.apk || CancelFailed
}
