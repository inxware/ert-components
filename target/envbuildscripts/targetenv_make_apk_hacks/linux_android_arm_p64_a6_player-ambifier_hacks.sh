#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/targetenv_make_apk_hacks/linux_android_arm_p64_a6_ambifier_hacks.sh $1

export EHS_PRODUCT_NAME="player"

SetupTargetEnv_BinFolder(){
    # do nothing
    echo "setting up - bin folder for player"
}

TargetEnvMakeApk_Build(){
    echo "Building APK for player"
    $EHS_ROOT/target/envbuildscripts/targetenv_make_apk_hacks/targetenv_make_apk_gradlew_build.sh "$SPECIFIC_TARGET" "$ANDROID_STUDIO_ROOT"
    EHS_APK=${TARGET_PATH}/release/app-release.apk
    cp ${EHS_APK} ${TARGET_PATH}/bin/player.apk || CancelFailed
}
