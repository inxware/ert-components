#!/bin/bash
set -e
BUILD_MODE="$1"

# This script is the generic script for building arm packages and deploying them to devma
# it should be ncluded by the apk build script. 
# It does some staging directory mangling with SetupTargetEnv_CopyStaging
# then builds the apk with a call to TargetEnvMakeApk_Build

echo "DEBUG_BUILD: In linux_android_arm_hacks.sh"




#The following is required only if publishing APKs to private servers.

#todo The mode for build packages such as debian is to copy any packaing build strucutres per target 
#     in ../TARGET_TREES/ and run the builds from there (I think we are using a single directroy tree 
#     for all here?

#

#This copies a selected android studio project tree from the android_ALL (todo move this!)
#It then copies the pre-prepared staging directory content into it.  
SetupTargetEnv_CopyStaging(){
    
 
}

# This does the build!
TargetEnvMakeApk_Build(){
    
}
