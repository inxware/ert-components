#!/bin/bash
set -e
BUILD_MODE="$1" 

export REPOSITORY_ANDROID_STUDIO_ROOT="$EHS_ROOT/target/os-arch/android_ALL/android_studio_ehs"
export ANDROID_STUDIO_JNILIBS_PATH="$ANDROID_STUDIO_ROOT/app/src/main/jniLibs/armeabi-v7a"
export ANDROID_STUDIO_USERDATA_PATH="$ANDROID_STUDIO_ROOT/app/src/main/assets/userdata"
export ANDROID_STUDIO_DEVMAN_PATH="$ANDROID_STUDIO_USERDATA_PATH/devman/core"
export ANDROID_STUDIO_TOOLS_PATH="$ANDROID_STUDIO_USERDATA_PATH/appdata/default"

export SSHPORT=8822
export DEVMAN_SERVER_NAME="ambifier"
export DEVMAN_SERVER_DOMAIN="devman.ambifier.com"
export DEVMAN_SERVER_URL="https://$DEVMAN_SERVER_DOMAIN"
export DEVMAN_UNAME="ambifier"
export EHS_PRODUCT_NAME="ambifier"

source ${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk_hacks/targetenv_make_apk_utils.sh"
source ${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk_hacks/targetenv_make_apk_setup.sh"

# todo rename CopyEHSTools() to what it actualy does e.g. CopyDefautApp
#todo2022 can't these function be remved from a config file? we just want paramters not code that will likely get broken in stranger ways...
SetupTargetEnv_CopyEHSTools(){
    TOOLS_DIR=${EHS_ROOT}/../apps/
    GetApplicationRepo $TOOLS_DIR
    
    if [ -d "$TOOLS_DIR" ] && [ -d "$ANDROID_STUDIO_TOOLS_PATH" ]; then
        echo "Copying the TSA tools app to the project."
        cp -Rf ${EHS_ROOT}/../apps/customer-apps/TSA/ambifier-v0.6.0-integrated/export/* ${ANDROID_STUDIO_TOOLS_PATH} || CancelFailed
    else
        echo "Failed to copy the TSA app!"
        CancelFailed   
    fi
}

SetupTargetEnv_Certs(){
    # override with certificates
    echo "Setup ambifier server certificates"
    CERTS_DIR=${EHS_ROOT}/../DevmanSecurity/devman.ambifier.com
    cp -f ${CERTS_DIR}/devman-ca.crt ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-ca.crt || CancelFailed
    cp -f ${CERTS_DIR}/devman-client-crt-key.pem ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-client-crt-key.pem || CancelFailed
}

SetupTargetEnv_BinFolder(){
    echo "Setting up bin folder for APKs."
    echo "Enter password for 'Ambifier.git' repository."
    APK_FOLDER=${TARGET_PATH}/bin
    AMBIFIER_APK_PATH=${EHS_ROOT}/../Ambifier
    if ! [ -d "$AMBIFIER_APK_PATH" ]; then
        pushd ${EHS_ROOT}/.. || CancelFailed
        git clone ssh://tech-data@dev.inx-systems.net:8822/home/inx-data/data/Repos/Ambifier.git || CancelFailed
        popd
    else
        pushd ${EHS_ROOT}/../Ambifier || CancelFailed
        git pull
        popd
    fi
    AMBIFIER_APK=${AMBIFIER_APK_PATH}/Ambifier2/builds/Android/ambifier.apk
    if ! [ -f "$AMBIFIER_APK" ]; then
        echo "Failed to obtain Ambifier.git repo which contains ambifier.apk"
        CancelFailed
    fi
    cp ${AMBIFIER_APK} ${APK_FOLDER}/ambifier.apk || CancelFailed
}

TargetEnvMakeApk_Build(){
    echo "Building APK for ambifier"
    $EHS_ROOT/target/envbuildscripts/targetenv_make_apk_hacks/targetenv_make_apk_gradlew_build.sh "$SPECIFIC_TARGET" "$ANDROID_STUDIO_ROOT"
    EHS_APK=${TARGET_PATH}/release/app-release.apk
    cp ${EHS_APK} ${TARGET_PATH}/bin/ehs.apk || CancelFailed
}
