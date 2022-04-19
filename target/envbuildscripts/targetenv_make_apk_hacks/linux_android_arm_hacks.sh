#!/bin/bash

source ${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk_hacks/targetenv_make_apk_setup.sh"

#TOD The following paths shouldbe changed tothe staging directory  (../TARGET_TREES/...) and 
# the template android projects copied there to avoid manipulating code in the repo. 

export ANDROID_STUDIO_ROOT="$EHS_ROOT/target/os-arch/android_ALL/android_studio_ehs"
export ANDROID_STUDIO_JNILIBS_PATH="$ANDROID_STUDIO_ROOT/app/src/main/jniLibs/armeabi-v7a"
export ANDROID_STUDIO_SYSDATA_PATH="$ANDROID_STUDIO_ROOT/app/src/main/assets/userdata/sysdata"
export ANDROID_STUDIO_DEVMAN_PATH="$ANDROID_STUDIO_ROOT/app/src/main/assets/userdata/devman/core"


DEVMAN_URL="devman-inx-systems.net"
DEVMAN_URL_TYPE="https://"
DOWNLOAD_URL="$DEVMAN_URL"
DOWNLOAD_URL_TYPE="$DEVMAN_URL_TYPE"
DEVMAN_UNAME="devman"
DOWNLOAD_UNAME="$DEVMAN_UNAME"

AndroidStudioPath(){
    echo $ANDROID_STUDIO_ROOT
}

ApkDownloadDevmanUrl(){
    echo "$DOWNLOAD_URL"
}

# returns domain without UTL type e.g. for certification generation
ApkDevmanUrl(){
    echo "$DEVMAN_URL"
}

ApkDevmanUsername(){
    echo "$DEVMAN_UNAME"
}

ApkDownloadUsername(){
    echo "$DOWNLOAD_UNAME"
}

ApkBuildSetup(){
    echo "${DEVMAN_URL_TYPE}${DEVMAN_URL}" | tr -d \\n > "$ANDROID_STUDIO_DEVMAN_PATH/config/DEVMANURL.000"
}

ApkBuildCleanUp(){
    echo "Clean up android build env."
    # clean git changes
    git checkout -- "$ANDROID_STUDIO_DEVMAN_PATH/config/DEVMANURL.000"
    if [ -d "$ANDROID_STUDIO_USERDATA_PATH/sysdata/" ]; then
        rm -r $ANDROID_STUDIO_USERDATA_PATH/sysdata/
    fi
}

ApkUpload2Server(){
    echo "@TODO - ApkUpload2Server function needs implementation!"
}
