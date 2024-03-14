#!/bin/bash
#####################################################################################
# arg 1 : TARGET NAME
# arg 2 : Yes to export the Android project (not sure we we wouldn't in this scrip?)
# No other environment variables appear to be needed
######################################################################################
export SPECIFIC_TARGET=$1
export WITHOUT_RUNNING_EXPORT=$2
export EHS_ROOT=`pwd` # assuming we're in the ehs project root
pushd ${EHS_ROOT}/..
export REPOSITORY_ROOT=`pwd`
popd
echo "*************************************************************"
echo "***** Exporting Unity project for ($SPECIFIC_TARGET)"
echo "*************************************************************"

if [ -z "$SPECIFIC_TARGET" ]; then
    echo "SPECIFIC_TARGET is not specified."
    exit 1
fi
pushd ../TARGET_TREES || exit 1
TARGET_TREES=$(pwd)
popd

if ! [ -d "$TARGET_TREES" ]; then
    echo "TARGET_TREES directory could not be created"
    exit 1
fi

export TARGET_PATH=${TARGET_TREES}/ehs_env-${SPECIFIC_TARGET}

export UNITY_BUILD_TOOLS_VERSION="2019.4.40f1"
export UNITY_BUILD_TOOLS_ROOT="$REPOSITORY_ROOT/unity3d-build-support"
export UNITY_BUILD_TOOLS="$UNITY_BUILD_TOOLS_ROOT/$UNITY_BUILD_TOOLS_VERSION/Editor"
export ANDROID_SDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/SDK"
export ANDROID_NDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/NDK"
export ANDROID_JDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/OpenJDK"
export JAVA_HOME=$ANDROID_JDK

export ANDROID_STUDIO_ROOT=${TARGET_PATH}/android_studio_project
export REPOSITORY_ANDROID_STUDIO_ROOT="$EHS_ROOT/target/os-arch/android_ALL/android_studio_unity_ehs"
export REPOSITORY_ANDROID_STUDIO_LIBS="$EHS_ROOT/target/os-arch/android_ALL/android_studio_ehs"

export EHS_UNITY_PROJECT_ROOT="$EHS_ROOT/target/os-arch/android_ALL/Unity_EHS"
export EHS_UNITY_PROJECT_PATH="$TARGET_PATH/Unity_EHS"

function CancelFailed(){
    echo "FAILED to export Unity target=($SPECIFIC_TARGET) !!!"
    # rm -rf ${TARGET_PATH}
	exit 1
}

function PrepareUnityToolchain(){
    # @TODO - move Unity toolchain to ert-contrib-middleware

    # check if the unity build tools are available
    if [ -d "$UNITY_BUILD_TOOLS" ]; then
        echo "Unity3d build support tools are present."
    else
        echo "Unity3d build support tools are NOT present on this machine."
        echo "Downloading Unity3d build support tools ..."
        mkdir "$UNITY_BUILD_TOOLS_ROOT"
        DATA_SERVER="tech-data@dev.inx-systems.net"
        DATA_FILE="unity3d-build-support.tar.gz"
        scp -P 8822 ${DATA_SERVER}":/home/inx-data/data/Unity/unity3d-build-support.tar.gz" ${UNITY_BUILD_TOOLS_ROOT}
        pushd ${UNITY_BUILD_TOOLS_ROOT}
        tar xfvz ${DATA_FILE}
        rm ${DATA_FILE}
        popd
    fi

    if ! [ -d "$UNITY_BUILD_TOOLS" ]; then
        echo "Unable to continue. Unity3d build support tools are NOT present ($UNITY_BUILD_TOOLS)."
        exit 1
    fi
}

function ExportUnityAndroidStudio() {

    # @TODO - should this be copied from TARGET_TREES ?
    EHS_ANDROID_LIB_FILE="$EHS_ROOT/ehs_$SPECIFIC_TARGET.so"
    if ! [ -f "$EHS_ANDROID_LIB_FILE" ] ; then
        echo "EHS lib file ($EHS_ANDROID_LIB_FILE) is not present in the root of EHS repository."
        echo "Make sure you build the EHS project for this traget ($SPECIFIC_TARGET) first."
        exit 1
    fi

    if ! [ -d "$REPOSITORY_ANDROID_STUDIO_ROOT" ] ; then
        echo "Android Studio project template path must be specified in this file."
        exit 1
    fi

    if ! [ -d "$REPOSITORY_ANDROID_STUDIO_LIBS" ] ; then
        echo "Android Studio EHS jni modules project path must be specified in this file."
        exit 1
    fi

    if ! [ -d "$EHS_UNITY_PROJECT_ROOT" ] ; then
        echo "EHS Unity project path must be specified in this file."
        exit 1
    fi
    # check for the 64-bit unity dependency
    DEPENDENCY_FILE=${TARGET_TREES}/ehs_env-${EHS_PLUGIN_LIBRARY_DEPENDENCY}/bin/ehs.so
    if ! [ -f "$DEPENDENCY_FILE" ]; then
        make clean
        echo "================================================="
        echo "Please make sure 64-bit dependecy ($EHS_PLUGIN_LIBRARY_DEPENDENCY) is built before this project."
        echo "================================================="
        echo "./configure $EHS_PLUGIN_LIBRARY_DEPENDENCY"
        echo "make all"
        echo "make targetenv"
        echo "make clean"
        echo " ... then configure and build this project again."
        echo "================================================="
        CancelFailed
    fi

    echo "Create Unity Android Studio project directory ($ANDROID_STUDIO_ROOT)"
    mkdir -p ${ANDROID_STUDIO_ROOT} || CancelFailed

    echo "Copying EHS Unity project ($EHS_UNITY_PROJECT_ROOT) to $EHS_UNITY_PROJECT_PATH"
    cp -r $EHS_UNITY_PROJECT_ROOT $EHS_UNITY_PROJECT_PATH || CancelFailed

    # make sure that Unity toolchain is present
    PrepareUnityToolchain

    echo "***************************************"
    echo "******  Build Unity 3d project   ******"
    echo "***************************************"    

    # using android sdk/ndk that is part of Unity toolchain
    # set enviroment vars
    export UNITY_ANDROID_BATCH_BUILD_PATH="$ANDROID_STUDIO_ROOT"
    export ANDROID_SDK_ROOT=$ANDROID_SDK
    export ANDROID_NDK_ROOT=$ANDROID_NDK
    export PATH=$PATH:$ANDROID_SDK/tools/bin/
    # accept android sdk licence
    LICENCE_CHECKED="$UNITY_BUILD_TOOLS_ROOT/$UNITY_BUILD_TOOLS_VERSION/licence_checked"
    if ! [ -f "$LICENCE_CHECKED" ]; then
        yes | sdkmanager --update
        yes | sdkmanager "platforms;android-29" "build-tools;29.0.3"
        yes | sdkmanager --licenses
        echo "checked" > $LICENCE_CHECKED
    fi

    # build and export android project
    pushd "$UNITY_BUILD_TOOLS" || CancelFailed
        #BUILD_WORKER_COUNT=-job-worker-count 8
        echo "Exporting unity project to the android studio."
        echo "See logs ===> 'tail -f $TARGET_PATH/log'"
        ./Unity ${BUILD_WORKER_COUNT} -quit -batchmode -nographics -logfile "$TARGET_PATH/log" -projectPath "$EHS_UNITY_PROJECT_PATH" -executemethod "BuildScript.Android" || CancelFailed
    popd

    echo "Copying Android Studio project from the template"
    # copy project with libs e.g. media, utils etc
    cp -r $REPOSITORY_ANDROID_STUDIO_LIBS $TARGET_PATH || CancelFailed
    CURRENT_VERSION_NAME=$(grep "versionName" $REPOSITORY_ANDROID_STUDIO_ROOT/launcher/build.gradle | awk '{print $2}')
    CURRENT_VERSION_CODE=$(grep "versionCode" $REPOSITORY_ANDROID_STUDIO_ROOT/launcher/build.gradle | awk '{print $2}')
    NEW_VERSION_NAME=$(grep "versionName" $ANDROID_STUDIO_ROOT/launcher/build.gradle | awk '{print $2}')
    NEW_VERSION_CODE=$(grep "versionCode" $ANDROID_STUDIO_ROOT/launcher/build.gradle | awk '{print $2}')

    cp -r $REPOSITORY_ANDROID_STUDIO_ROOT/* $ANDROID_STUDIO_ROOT || CancelFailed

    echo "Upating version name ($CURRENT_VERSION_NAME) ====> ($NEW_VERSION_NAME)"
    echo "Upating version code ($CURRENT_VERSION_CODE) ====> ($NEW_VERSION_CODE)"

    # replace versions in the files
    sed -i "s/versionName ${CURRENT_VERSION_NAME}/versionName ${NEW_VERSION_NAME}/g" $ANDROID_STUDIO_ROOT/launcher/build.gradle || CancelFailed
    sed -i "s/versionCode $CURRENT_VERSION_CODE/versionCode $NEW_VERSION_CODE/g" $ANDROID_STUDIO_ROOT/launcher/build.gradle || CancelFailed

    # overwrite with latest eRT plugins (make sure both plugins are uptodate)
    echo "Copying 32-bit ehs plugin to Unity project"
    cp ${EHS_ANDROID_LIB_FILE} "$EHS_UNITY_PROJECT_PATH/Assets/Libs/arm/libnative-activity.so" || CancelFailed
    echo "Copying 64-bit ehs plugin to Unity project"
    cp ${DEPENDENCY_FILE} "$EHS_UNITY_PROJECT_PATH/Assets/Libs/arm64/libnative-activity.so" || CancelFailed
}


#########################################################################################

echo ""

if [ "${EHS_UNITY_PROJECT_EXPORT_SUPPORT}" = "" ]; then
    echo "FAILED!"
    echo "This option can only be run for targets that has Unity export support (EHS_UNITY_PROJECT_EXPORT_SUPPORT) set in config.mk"
    exit 1
fi

if [ "$WITHOUT_RUNNING_EXPORT" = "" ]; then

    # @TODO - enable exporting projects/apps for othe platforms e.g. windows
    ExportUnityAndroidStudio

    echo "================================================="
    echo "UNITY EXPORT COMPLETED"
    echo "================================================="

fi