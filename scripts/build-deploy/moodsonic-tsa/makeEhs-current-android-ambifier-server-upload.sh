#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# This script builds and dependency build for alljust the currently configure Android 
# targets for Moodsonic and uploads it to Devman

set -e

if [ "$1" = "-no-upload" ]; then
    echo "Not Uploading builds"
else
    DO_UPLOAD="TRUE"
fi

if [ -f configure ] ; then 
    source ./scripts/build-function-library/android-utilities.sh
else
    source ../../build-function-library/android-utilities.sh
    cd ../../../
fi

echo "==================================================="
echo "WARNING: NOT CLEANING THE UNITY BUILD."
echo "IF YOU NEED TO DO THIS HE RUN"
echo "make targetenv_cleancfg"
echo "---------------------------------------------------"


#make prepdeps # this needs to be done only once
make targetenv_version
BuildAndroid
test -n "$DO_UPLOAD" && UploadApk-ambifier
echo "==================== SUCCESS =========================="
