#----------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#/bin/bash
set -e

#source ../../build-function-library/debian-utilities.sh

#cdBuildTreeRoot || exit 1


echo "================================================"
echo "WARNING - THIS BUILD IS NOT USING A DOCKER IMAGE"
echo "------------------------------------------------"
# Full build of Unity tellisign
ERT_TELLISIGN_TARGET_CONFIG=$1
if [ "$ERT_TELLISIGN_TARGET_CONFIG" = "" ]; then
    . TARGET.cfg
    export ERT_TELLISIGN_TARGET_CONFIG=$TARGET
    #export ERT_TELLISIGN_TARGET_CONFIG=linux_android_arm_unity-tellisign
    #export ERT_TELLISIGN_TARGET_CONFIG=linux_android_arm_p64_h6_unity-tellisign
    echo "Using default Tellisign target config ($ERT_TELLISIGN_TARGET_CONFIG)"
fi

# build 64-bit eRT plugin required by all Unity targets
./configure linux_android_arm64_unity-lib
make clean ||:
make all_docker
make targetenv
make clean ||:

# build arm 32-bit unity ehs library, set up unity and android envs, build apk
./configure ${ERT_TELLISIGN_TARGET_CONFIG}
#make targetenv_cleanall          # needed ONLY when Unity C# project needs updating
make all_docker
make targetenv
echo "+++++++++++++ Make targetenv_apk_docker +++++++++++++++++++++++++++++++++++++++++++"
make targetenv_unity_export
echo "+++++++++++++ Make targetenv_apk_docker +++++++++++++++++++++++++++++++++++++++++++"
make targetenv_apk # todo - fix targetenv_apk_docker

echo "==================================================="

echo "Installing to any connected devices.. (or press CTRL-C to skip)"

#read -n 1
make upload_ehs_via_adb


echo "Uploade to the tellisign server?"
read -n 1
scp -P 8822 ../TARGET_TREES/ehs_env-${ERT_TELLISIGN_TARGET_CONFIG}/bin/tellisign.apk  root@cms.tellisign.com:/home/www/devport-show/docs/downloads/


#adb install -g -r ../TARGET_TREES/ehs_env-linux_android_arm_unity-tellisign/bin/tellisign.apk
#adb install -g -r ../TARGET_TREES/ehs_env-linux_android_arm_unity-tellisign/bin/tellisign.apk

