#!/bin/bash

set -e

TARGET=$1

# todo - should this be specifed in config ?
PRE_BUILT_UNITY_EXE_PATH=../ert-contrib-middleware/contrib/Unity3D/SignageWindowsBuild
TARGET_ROOT=../TARGET_TREES/ehs_env-${TARGET}

echo "###############################################################"
echo "################## UNITY 3d WIN32 HACK ########################"
echo "###############################################################"

# remove unnecessary files and dir
[ -f "$TARGET_ROOT/bin/ehs.exe" ] && rm ${TARGET_ROOT}/bin/ehs.exe
[ -d "$TARGET_ROOT/appdata/fallbacks" ] && rm -rf ${TARGET_ROOT}/appdata/fallbacks
[ -d "$TARGET_ROOT/appdata/temp" ] && rm -rf ${TARGET_ROOT}/appdata/temp

cp -Rf ${PRE_BUILT_UNITY_EXE_PATH}/* ${TARGET_ROOT}/bin/

# move plugin to a correct location
mv ${TARGET_ROOT}/bin/ehs.dll ${TARGET_ROOT}/bin/TELLISIGN_Data/Plugins/x86/libnative-activity.dll

echo "DONE!"

echo "###############################################################"