#!/bin/bash

# inx limited 2020
# Uploads android supervisor bundle and APKs to device via adb

set -e

export SPECIFIC_TARGET=$1
export EHS_ROOT=`pwd` # assuming we're in the ehs project root

echo ""
echo "***** ADB UPLOADER FOR ANDROID ($SPECIFIC_TARGET) *****"
echo ""

if [ "$EHS_ANDROID_SIMPLE_APK" != "yes" ]; then  
echo "Installing magical things in magical ways"
# uses this to export env vars
source ${EHS_ROOT}/target/envbuildscripts/targetenv_android_dep_pack.sh $SPECIFIC_TARGET "no-run"
export SUPERVISOR_PACKAGE_PATH="" # we're uploading to the device, not making bundling supervisor
# run script used for uploading to adb with all of eRT target env vars
${EHS_ROOT}/target/envbuildscripts/installers/android-adb/adb_targetenv.sh ${SPECIFIC_TARGET} ${EHS_ROOT}

else

echo "Installing APK with adb install "
adb install ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.apk

fi

