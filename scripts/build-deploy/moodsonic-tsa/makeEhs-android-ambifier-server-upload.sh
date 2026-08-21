#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#This script does a full build and dependency build for all Android targets for Moodsonic

set -e

if [ "$1" = "-no-upload" ]; then
    echo "Not Uploading builds"
else
    DO_UPLOAD="TRUE"
fi

if [ -f configure ] ; then 
    source ./scripts/build-function-library/android-utilities.sh
else
    source ../../build-function-library/android-utilities.sh || exit 1
    cd ../../../
fi

echo "==================================================="
echo "WARNING: NOT CLEANING THE UNITY BUILD."
echo "IF YOU NEED TO DO THIS HE RUN"
echo "make targetenv_cleancfg"
echo "---------------------------------------------------"


echo "Making android a6 (MSG200 & MSG300) packages"
# build all ambifier server android pine-64 A6 platforms
./configure linux_android_arm_p64_a6_ambifier
make prepdeps # this needs to be done only once
test -n "$DO_UPLOAD" && make targetenv_version

BuildAndroid
test -n "$DO_UPLOAD" && UploadApk-ambifier

./configure linux_android_arm_p64_a6_ambifier-debug
BuildAndroid
test -n "$DO_UPLOAD" && UploadApk-ambifier

# build all ambifier server android pine-64 H6 platforms
./configure linux_android_arm_p64_h6_ambifier
BuildAndroid
test -n "$DO_UPLOAD" && UploadApk-ambifier

# TODO - no debug build fo this platform atm
# Does this have a different package name to ambifier or just clobber the ambifier build?
##./configure linux_android_arm_p64_h6_player_ambifier
##BuildAndroid
##test -n "$DO_UPLOAD" && UploadApk-ambifier

# Ethernet only Android Ambifier device usually
./configure linux_android_arm_p64_rock64_ambifier
BuildAndroid
test -n "$DO_UPLOAD" && UploadApk-ambifier

#WiFI enabled Android device (Signage Hardware)
./configure linux_android_arm_rk3399_rockpi4_ambifier
BuildAndroid
test -n "$DO_UPLOAD" && UploadApk-ambifier
# todo report the version number here... It's not an env variable usually though...
echo "==================== SUCCESS ====================="
