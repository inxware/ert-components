#!/bin/bash

BUILD_MODE="$1"

export SSHPORT=8822
export DEVMAN_SERVER_NAME="tellisign"
export DEVMAN_SERVER_DOMAIN="devman.inx-systems.com"
export DEVMAN_SERVER_URL="https://$DEVMAN_SERVER_DOMAIN"
export DEVMAN_UNAME="root"
export EHS_PRODUCT_NAME="tellisign"

export UNITY_BUILD_TOOLS_VERSION="2019.4.8f1"
export UNITY_BUILD_TOOLS_ROOT="$REPOSITORY_ROOT/unity3d-build-support"
export UNITY_BUILD_TOOLS="$UNITY_BUILD_TOOLS_ROOT/$UNITY_BUILD_TOOLS_VERSION/Editor"
export ANDROID_SDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/SDK"
export ANDROID_NDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/NDK"
export ANDROID_JDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/OpenJDK"
export JAVA_HOME=$ANDROID_JDK

export REPOSITORY_ANDROID_STUDIO_ROOT="$EHS_ROOT/target/os-arch/android_ALL/android_studio_unity_ehs"
export REPOSITORY_ANDROID_STUDIO_LIBS="$EHS_ROOT/target/os-arch/android_ALL/android_studio_ehs"
export ANDROID_STUDIO_USERDATA_PATH="$ANDROID_STUDIO_ROOT/launcher/src/main/assets/userdata"
export ANDROID_STUDIO_DEVMAN_PATH="$ANDROID_STUDIO_USERDATA_PATH/devman/core"
export ANDROID_STUDIO_TOOLS_PATH="$ANDROID_STUDIO_USERDATA_PATH/appdata/default"

export EHS_UNITY_PROJECT_ROOT="$EHS_ROOT/target/os-arch/android_ALL/Unity_EHS"
export EHS_UNITY_PROJECT_PATH="$TARGET_PATH/Unity_EHS"

source ${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk_hacks/targetenv_make_apk_utils.sh"
source ${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk_hacks/targetenv_make_apk_setup.sh"

SetupTargetEnv(){
    EHS_ANDROID_LIB_FILE="$EHS_ROOT/ehs_$SPECIFIC_TARGET.so"
    if ! [ -f "$EHS_ANDROID_LIB_FILE" ] ; then
        echo "EHS lib file ($EHS_ANDROID_LIB_FILE) is not present in the root of EHS repository."
        echo "Make sure you build the EHS project for this traget ($SPECIFIC_TARGET) first."
        exit 1
    fi
    if ! [ -d "$ANDROID_STUDIO_ROOT" ]; then
        echo "TARGET_TREES must contain android_studio_project directory."
        exit 1
    fi
    if ! [ -d "$REPOSITORY_ANDROID_STUDIO_ROOT" ] ; then
        echo "Android Studio project path must be specified in the hack file."
        exit 1
    fi
    if ! [ -d "$EHS_UNITY_PROJECT_ROOT" ] ; then
        echo "EHS Unity project path must be specified in the hack file."
        exit 1
    fi
    # check for the 64-bit unity dependency
    DEPENDENCY_FILE=${TARGET_TREES}/ehs_env-${EHS_PLUGIN_LIBRARY_DEPENDENCY}/lib/ehs_${EHS_PLUGIN_LIBRARY_DEPENDENCY}.so
    if ! [ -f "$DEPENDENCY_FILE" ]; then
        make clean
        echo "================================================="
        echo "Please make sure 64-bit dependecy ($EHS_PLUGIN_LIBRARY_DEPENDENCY) is built before this project."
        echo "================================================="
        echo "./configure $EHS_PLUGIN_LIBRARY_DEPENDENCY"
        echo "make all"
        echo "make targetenv"
        echo " ... then configure and build this project again."
        echo "================================================="
        CancelFailed
    fi

    echo "Copying EHS Unity project ($EHS_UNITY_PROJECT_ROOT) to $EHS_UNITY_PROJECT_PATH"
    cp -r $EHS_UNITY_PROJECT_ROOT $EHS_UNITY_PROJECT_PATH || CancelFailed
    echo "Copying 32-bit ehs plugin to Unity project"
    cp ${EHS_ANDROID_LIB_FILE} "$EHS_UNITY_PROJECT_PATH/Assets/Libs/arm/libnative-activity.so" || CancelFailed
    echo "Copying 64-bit ehs plugin to Unity project"
    cp ${DEPENDENCY_FILE} "$EHS_UNITY_PROJECT_PATH/Assets/Libs/arm64/libnative-activity.so" || CancelFailed

    echo "***************************************"
    echo "******  Build Unity 3d project   ******"
    echo "***************************************"

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

    # set enviroment vars
    export UNITY_ANDROID_BATCH_BUILD_PATH="$ANDROID_STUDIO_ROOT"
    export ANDROID_SDK_ROOT=$ANDROID_SDK
    export ANDROID_NDK_ROOT=$ANDROID_NDK
    export JAVA_HOME=$ANDROID_JDK
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
        echo "Exporting unity project to the android studio. This may take Loooooong time (hours?)."
        echo "To see logs do : 'tail -f $TARGET_PATH/log'"
        ./Unity -quit -batchmode -nographics -logfile "$TARGET_PATH/log" -projectPath "$EHS_UNITY_PROJECT_PATH" -executemethod "BuildScript.Android" -exportAndroidStudio "true" || CancelFailed
        #./Unity -job-worker-count 8 -quit -batchmode -logfile "$TARGET_PATH/log" -projectPath "$EHS_UNITY_PROJECT_PATH" -executemethod "BuildScript.Android" -exportAndroidStudio "true" || CancelFailed
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
    mkdir -p ${ANDROID_STUDIO_DEVMAN_PATH}/config || exit 1
}

SetupTargetEnv_CopyEHSTools(){
    TOOLS_DIR=${EHS_ROOT}/../apps/
    GetApplicationRepo $TOOLS_DIR
    echo "INFO: copying app from /customer-apps/SimpleSignOn/sso-unity-v1.0.0/export/"
    if [ -d "$TOOLS_DIR" ] && [ -d "$ANDROID_STUDIO_TOOLS_PATH" ]; then
        echo "Coping the Unity signage tools app to the project."
        cp -Rf ${EHS_ROOT}/../apps/customer-apps/SimpleSignOn/sso-unity-v1.0.0/export/* ${ANDROID_STUDIO_TOOLS_PATH} || CancelFailed
    else
        echo "Failed to copy the tools app!"
        CancelFailed
    fi
}

SetupTargetEnv_Certs(){
    # override with certificates
    CERTS_DIR=${EHS_ROOT}/../DevmanSecurity/$DEVMAN_SERVER_DOMAIN
    cp -f ${CERTS_DIR}/devman-ca.crt ${ANDROID_STUDIO_DEVMAN_PATH}/certs/cacert.pem || CancelFailed
 #todo why no client certificates installed? is tellising insecure?
    #//target/os-arch/android_ALL/android_studio_unity_ehs/launcher/src/main/assets/userdata/devman/core/certs
    #                                     "$ANDROID_STUDIO_ROOT/app/src/main/assets/userdata/devman/core
}

SetupTargetEnv_BinFolder(){
    # do nothing
    echo "setting up - bin folder for unity signage"
}

#todo2022 - we want to untangle the apk signing and the unity build at some point. 
# - we can have a paramter on config.mk that identifies an signing keys in the DevmanSecurity repo that should be used.
TargetEnvMakeApk_Build(){
    echo "Building APK for Unity Tellisign"

    SIGN_RELEASE_DATA_PATH=${EHS_ROOT}/../DevmanSecurity/show.online.google.play
    # Write all essential app signing things to gradle.properties
    GRADLE_PROPS_FILE=${ANDROID_STUDIO_ROOT}/gradle.properties
    if grep -q "RELEASE_STORE_FILE" "$GRADLE_PROPS_FILE"; then
        echo "gradle.properties already stored"
    else
        echo "RELEASE_STORE_FILE=$SIGN_RELEASE_DATA_PATH/show.jks" >> $GRADLE_PROPS_FILE
        while read line; do
            echo "$line" >> $GRADLE_PROPS_FILE
        done < "$SIGN_RELEASE_DATA_PATH/.release"
    fi
    pushd $ANDROID_STUDIO_ROOT
    chmod +x ./gradlew
    #./gradlew build
    ./gradlew clean
    ./gradlew assembleRelease
    ./gradlew signReleaseBundle
    popd
    EHS_ANDROID_APK_ROOT="$ANDROID_STUDIO_ROOT/launcher/build/outputs"
    cp -R "$EHS_ANDROID_APK_ROOT/apk/release" ${TARGET_PATH}
    cp -R "$EHS_ANDROID_APK_ROOT/bundle/release/launcher.aab" ${TARGET_PATH}/release
    SSO_APK=${TARGET_PATH}/release/launcher-release.apk
    cp ${SSO_APK} ${TARGET_PATH}/bin/tellisign.apk
}
