#!/bin/bash
# inx limited 2020
# Creates an android .apk

set -e

echo "**************************************************************************************"
echo "**  Making Android APK - _* option - not standalone **"
echo "**************************************************************************************"
echo " Using Android Studio Project ${ANDROID_STUDIO_EHS_PROJECT}"

#For badly configured JAVA environments
unset JAVA_HOME

export SPECIFIC_TARGET=$1

export EHS_ROOT=`pwd` # assuming we're in the ehs project root
pushd ${EHS_ROOT}/..
export REPOSITORY_ROOT=`pwd`
popd
if [ "${SPECIFIC_TARGET}" = "" ]; then
    echo "TARGET is not specified."
    exit 1
fi
if [ "$EHS_ANDROID_PACKAGE_SIGNING_PATH" = "" ]; then
   echo "You need to set a signing path EHS_ANDROID_PACKAGE_SIGNING_PATH in your config "
   echo " to point to something in ../DevmanSecurity currently.... Sorry!"
   exit
fi


pushd ../TARGET_TREES || exit 1
export TARGET_TREES=$(pwd)
popd
if ! [ -d "$TARGET_TREES" ]; then
    echo "TARGET_TREES directory is not available!"
    exit 1
fi

# todo this isn't needed here any more..
#if [ "${EHS_PRODUCT_NAME}" = "" ]; then
#    echo "WARNING: EHS_PRODUCT_NAME is not set"
#else
#    export EHS_PRODUCT_NAME
#fi

if [ "${BUILD_WITH_ANDROID_SUPERVISOR}" = "" ]; then
    echo "BUILD_WITH_SUPERVISOR is not set (Not the OS supervisor package with this build)" 
else
    export BUILD_WITH_ANDROID_SUPERVISOR
fi

export TARGET_PATH=${TARGET_TREES}/ehs_env-${SPECIFIC_TARGET}
# This is the path where this script will download the Android SDK to an adjascent directory to this repo.
export ANDROID_ROOT="$EHS_ROOT/../inx_android_build"
export ANDROID_SDK=${ANDROID_ROOT}"/SDK"
# Do we still need this for android studio?
export JDK_PATH="/usr/lib/jvm/java-8-openjdk-amd64"
# Location in the staging directory where the selected reference project is copied to for running the build in
export ANDROID_STUDIO_ROOT="${TARGET_PATH}/android_studio_project"
export ANDROID_STUDIO_BUILD_RELEASE_APK_NAME="app-release.apk"
export ANDROID_STUDIO_BUILD_BUNDLE_OUTPUT=""
export ANDROID_STUDIO_SRC_ROOT="${ANDROID_STUDIO_ROOT}/app"
export ANDROID_STUDIO_BUILD_APK_OUTPUT="${ANDROID_STUDIO_SRC_ROOT}/build/outputs/apk"

# If a specific android version is not specified then use this default. Careful changing this for legacy platforms!!
if [ "${ANDROID_STUDIO_EHS_PROJECT}" = "" ]; then
    ANDROID_STUDIO_EHS_PROJECT="android_studio_ehs"
fi

# This is the apk name that the Android studio projects will generate and shouldn't be changed.
export ANDROID_STUDIO_BUILD_RELEASE_APK_NAME="app-release.apk"
#If we want to have different deployed package names - then this where you get your chance
if [ ${ANDROID_TARGET_APK_NAME}="" ]; then
export ANDROID_TARGET_APK_NAME="ehs.apk" # @TODO - this name should probably be changed to eRT? Just beware that renaming will afect supervisor and updates scripts !
fi

export REPOSITORY_ANDROID_STUDIO_ROOT="${EHS_ROOT}/target/os-arch/android_ALL/${ANDROID_STUDIO_EHS_PROJECT}"

if [ "${EHS_ARCH}" = "arm64" ] || [ "${EHS_ANDROID_NATIVE_ABI}" = "arm64" ]; then
    echo "Using 64 bit eRT Android plug-in for this target."
    export ANDROID_STUDIO_JNILIBS_PATH="$ANDROID_STUDIO_SRC_ROOT/src/main/jniLibs/arm64-v8a"
else
    # assume its 32 bit
    echo "Using 32 bit eRT Android plug-in for this target."
    export ANDROID_STUDIO_JNILIBS_PATH="$ANDROID_STUDIO_SRC_ROOT/src/main/jniLibs/armeabi-v7a"
fi

echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo "EHS_UNITY_PROJECT_EXPORT_SUPPORT=${EHS_UNITY_PROJECT_EXPORT_SUPPORT}"
echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++"
################# SET-UP UNITY ENV if we going to copy Native libs to the relevant Unity Project#################
## TODO THis following block should be followed by an else of all the above, rather than messily overwriting some (possibly all?) of the above.
if [ "${EHS_UNITY_PROJECT_EXPORT_SUPPORT}" != "" ]; then
    echo "* Building apk from exported Unity project *"
    # set Unity android env defined in export script
    # This willl override the paths to default "normal" android APK SDK & template project paths.
    source ${EHS_ROOT}/target/envbuildscripts/targetenv_unity_export.sh $1 "no-run"

    # @TODO - seems like main difference is name 'app' for eRT project and 'launcher' for Unity.
    # Could rename 'app' -> 'launcher' in eRT project template to simplyfy things
    # "launcher" is the Unity Android Studio name that then gets used as the final apk name - this is not easy to change in Unity.
    export ANDROID_STUDIO_BUILD_RELEASE_APK_NAME="launcher-release.apk"
    export ANDROID_STUDIO_BUILD_BUNDLE_OUTPUT="${ANDROID_STUDIO_ROOT}/launcher/build/outputs/bundle"
    export ANDROID_STUDIO_SRC_ROOT="${ANDROID_STUDIO_ROOT}/launcher"
    export ANDROID_STUDIO_BUILD_APK_OUTPUT="${ANDROID_STUDIO_SRC_ROOT}/build/outputs/apk"
    #This is the final name of the package (copied from )
    export ANDROID_TARGET_APK_NAME="tellisign.apk"
    export REPOSITORY_ANDROID_STUDIO_ROOT=""
    # For Unity targets the jniLibs path inside the exported unityLibrary project is
    # the canonical location Gradle picks up native libs from.  We set it here so the
    # copy block below updates it whenever make targetenv_apk is run — no need to
    # re-run the slow make targetenv_unity_export just because ehs.so changed.
    if [ "${EHS_ARCH}" = "arm64" ] || [ "${EHS_ANDROID_NATIVE_ABI}" = "arm64" ]; then
        export ANDROID_STUDIO_JNILIBS_PATH="${ANDROID_STUDIO_ROOT}/unityLibrary/src/main/jniLibs/arm64-v8a"
    else
        export ANDROID_STUDIO_JNILIBS_PATH="${ANDROID_STUDIO_ROOT}/unityLibrary/src/main/jniLibs/armeabi-v7a"
    fi
fi
####################################################

export ANDROID_STUDIO_USERDATA_PATH="${ANDROID_STUDIO_SRC_ROOT}/src/main/assets/userdata"
export ANDROID_STUDIO_DEVMAN_PATH="${ANDROID_STUDIO_USERDATA_PATH}/devman"
export ANDROID_STUDIO_APP_PATH="${ANDROID_STUDIO_USERDATA_PATH}/appdata"

mkdir -p ${ANDROID_STUDIO_USERDATA_PATH}
mkdir -p ${ANDROID_STUDIO_DEVMAN_PATH}
mkdir -p ${ANDROID_STUDIO_APP_PATH}
echo "Android studio project at ${ANDROID_STUDIO_USERDATA_PATH}"

echo "====================================================================================="
echo "Copying the staging directory content to the Android studio project."
if [ "${REPOSITORY_ANDROID_STUDIO_ROOT}" = "" ]; then
    echo "Android Studio project is not being sateged for this target as part of building apk."
    echo "e.g. Unity does it while exporting project. 'make targetenv_unity_export'"
else
    echo "Copying Android Studio project from repository ($REPOSITORY_ANDROID_STUDIO_ROOT) to a staging directory."
   if [ -d "${REPOSITORY_ANDROID_STUDIO_ROOT}" ]; then
        cp -Rf $REPOSITORY_ANDROID_STUDIO_ROOT/* $ANDROID_STUDIO_ROOT/
    else
        echo "Failed to copy the default eRT App!"
        echo "Have you ran make targetenv? Does the staging directory exist?"
        exit 1   
    fi
fi
echo "Copying latest eRT plugin to Android Studio project."
if [ "${ANDROID_STUDIO_JNILIBS_PATH}" = "" ]; then
    echo "ERROR: ANDROID_STUDIO_JNILIBS_PATH is not set — cannot update jniLibs."
    exit 1
else
    echo "cp ${TARGET_PATH}/bin/ehs.${EXE} ${ANDROID_STUDIO_JNILIBS_PATH}/libnative-activity.${EXE}"
    cp ${TARGET_PATH}/bin/ehs.${EXE} ${ANDROID_STUDIO_JNILIBS_PATH}/libnative-activity.${EXE}
    # Copy libc++_shared.so from the contrib target_libs directory.  This is the version
    # built/extracted from the same NDK as liblitert_c.a (NDK r27c) and must be version-
    # coherent with it.  It is placed alongside libnative-activity.so so the Android
    # dynamic linker finds it inside the APK rather than falling back to a missing system lib.
    if [ -n "${EHS_COMPONENT_SUPPORT_LIBS}" ] && [ -f "${EHS_COMPONENT_SUPPORT_LIBS}libc++_shared.so" ]; then
        echo "cp ${EHS_COMPONENT_SUPPORT_LIBS}libc++_shared.so ${ANDROID_STUDIO_JNILIBS_PATH}/libc++_shared.so"
        cp "${EHS_COMPONENT_SUPPORT_LIBS}libc++_shared.so" "${ANDROID_STUDIO_JNILIBS_PATH}/libc++_shared.so"
    else
        echo "WARNING: libc++_shared.so not found at ${EHS_COMPONENT_SUPPORT_LIBS} — APK may fail to load on device."
        echo "         Run: make all_docker first to ensure libc++_shared.so is present."
    fi
fi
echo "Copying 'devman' data"
cp -Rf ${TARGET_PATH}/devman/* ${ANDROID_STUDIO_DEVMAN_PATH}
echo "Copying 'userdata' data"
[ "$(ls ${TARGET_PATH}/userdata)" ] && cp -Rf ${TARGET_PATH}/userdata/* ${ANDROID_STUDIO_USERDATA_PATH}/
echo "Copying 'appdata' data"
[ "$(ls ${TARGET_PATH}/appdata)" ] && cp -Rf ${TARGET_PATH}/appdata/* ${ANDROID_STUDIO_APP_PATH}/
echo "Done!"
echo "====================================================================================="


echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

# override version
VERSION_DIR=${TARGET_PATH}/sysdata
if [ -d "$VERSION_DIR" ]; then
    cp -r ${VERSION_DIR} ${ANDROID_STUDIO_USERDATA_PATH}/ || echo "Couldn't copy sysdata/versioning info... Exiting " || exit 1
    NEW_VERSION=$( cat $ANDROID_STUDIO_USERDATA_PATH/sysdata/version.nfo )
    echo "Packaging EHS version : $NEW_VERSION"
else
    echo "EHS version not specified."
fi

echo "====================================================================================="
echo "Building eRT APK"
echo ""
########## download and install sdk if not present ##########

ANDROID_SDK_URL="https://dl.google.com/android/repository/commandlinetools-linux-6200805_latest.zip"
ANDROID_PROJECT_ROOT=$ANDROID_STUDIO_ROOT

if ! [ -d "$ANDROID_PROJECT_ROOT" ] ; then
    echo "Pass a valid path to android studio project : $ANDROID_PROJECT_ROOT"
    echo "as a first argument of this script."
    exit 1
fi    

if ! [ -f "$ANDROID_PROJECT_ROOT/gradlew" ]; then
    echo "Pass a valid path to gradlew android studio project as a first argument of this script."
    echo "[$ANDROID_PROJECT_ROOT/gradlew]"
    exit 1
fi    

if ! [ -d "$ANDROID_ROOT" ]; then
    mkdir -p $ANDROID_ROOT
fi

#todo2022 - this isn't needed with the docker (or does it detect Java OK in docker and skip it?)
if ! [ -d "$JDK_PATH" ]; then
    # Download JDK using debian package
    echo "Installing JDK ..."
    sudo apt install openjdk-8-jdk
    if ! [ -d "$JDK_PATH" ]; then
        echo "Failed to install JAVA into JAVA_HOME location ($JDK_PATH)."
        echo "set JDK_PATH env of this script to a valid openjdk-8-jdk location."
        exit 1
    fi    
else
    echo "Android JDK =============> OK"    
fi

# Set JDK env var
if [ "${JAVA_HOME}" = "" ]; then
    echo "Setting JAVA_HOME env."
    export JAVA_HOME="$JDK_PATH"
    echo $JAVA_HOME
fi    

#todo  should we look for the SDK in opt (including in the unity SDK) if we are building on the host. 
# or should we do this in a docker image in any case?
if ! [ -d "$ANDROID_SDK" ]; then
    # Download SDK
    echo "Downloading SDK"
    if ! curl -k -o "$ANDROID_ROOT/android-sdk.zip" $ANDROID_SDK_URL ;
    then
        echo "Failed to download SDK"
        "$ANDROID_ROOT/android-sdk.zip"
        exit 1
    fi
    echo "Unpacking SDK to $ANDROID_SDK"
    if ! unzip "$ANDROID_ROOT/android-sdk.zip" -d "$ANDROID_SDK";
    then
        echo "Failed to unpack SDK"
        rm -f "$ANDROID_ROOT/android-sdk.zip"
        rm -rf $ANDROID_SDK
        exit 1
    fi
    rm -f "$ANDROID_ROOT/android-sdk.zip"
    # Accept android SDK licenses
    export REPO_OS_OVERRIDE="linux"
    echo "Accepting licence for the SDK"
    yes | $ANDROID_SDK/tools/bin/sdkmanager --sdk_root=$ANDROID_SDK --licenses
else
    echo "Android SDK =============> OK"    
fi

# Set SDK env var
if [ "${ANDROID_SDK_ROOT}" = "" ]; then
    echo "Setting ANDROID_SDK_ROOT env."
    export ANDROID_SDK_ROOT="$ANDROID_SDK"
    echo $ANDROID_SDK_ROOT
fi


#################### build and sign apk #####################
if [ "$EHS_ANDROID_PACKAGE_SIGNING_PATH" != "" ]; then
# @TODO - signing data should get copied as part of tragetenv ?
echo "Signing and building EHS APK"
pushd ${EHS_ROOT}/..
SIGN_RELEASE_DATA_PATH=$(pwd)/DevmanSecurity/${EHS_ANDROID_PACKAGE_SIGNING_PATH}
popd
SIGNING_KEY=$SIGN_RELEASE_DATA_PATH/signing_key.jks
echo "Using key ($SIGNING_KEY)"
# Write all essential app signing things to gradle.properties
GRADLE_PROPS_FILE=${ANDROID_PROJECT_ROOT}/gradle.properties
echo "" >> $GRADLE_PROPS_FILE
echo "RELEASE_STORE_FILE=$SIGNING_KEY" >> $GRADLE_PROPS_FILE
while read line; do
    echo "$line" >> $GRADLE_PROPS_FILE
done < "$SIGN_RELEASE_DATA_PATH/release"

else 
echo "WARNING: Not Using a specific signing key"
fi
 

# Inject EHS_ANDROID_MIN_SDK into gradle.properties so build.gradle modules can read it.
# Platforms targeting modern hardware / Play Store leave this unset (defaults to 23 in build.gradle).
# Legacy embedded platforms that must run on very old Android set EHS_ANDROID_MIN_SDK=16 in config.mk.
_MIN_SDK=${EHS_ANDROID_MIN_SDK:-23}
echo "EHS_ANDROID_MIN_SDK=${_MIN_SDK}" >> ${ANDROID_PROJECT_ROOT}/gradle.properties
echo "Using Android minSdkVersion=${_MIN_SDK}"

pushd $ANDROID_PROJECT_ROOT
chmod +x ./gradlew

# Kill any stale il2cpp/bee_backend processes from previous interrupted builds
# to avoid them interfering with the new build or leaving pipes open in Gradle.
echo "Killing any stale il2cpp/bee_backend processes..."
pkill -KILL -f "Il2CppOutputProject/IL2CPP/build/deploy/il2cpp" 2>/dev/null || true
pkill -KILL -f "bee_backend/linux-x64/bee_backend" 2>/dev/null || true

# Stop any lingering Gradle daemon — long-running daemons accumulate state from
# previous builds and can hang at arbitrary points. A fresh daemon is safer.
./gradlew --stop 2>/dev/null || true

./gradlew assembleRelease
if [ "${ANDROID_STUDIO_BUILD_BUNDLE_OUTPUT}" = "" ]; then
    echo "Android bundle (.aab) is not built for this target."
else
    echo "Building android bundle."
if [ "$EHS_ANDROID_PACKAGE_SIGNING_PATH" != "" ]; then
    ./gradlew signReleaseBundle
fi
    echo "Copying .aab from ($ANDROID_STUDIO_BUILD_BUNDLE_OUTPUT) to the target tree"
    cp -R "${ANDROID_STUDIO_BUILD_BUNDLE_OUTPUT}" "${TARGET_PATH}"
fi
popd

EXPECTED_APK="${ANDROID_STUDIO_BUILD_APK_OUTPUT}/release/${ANDROID_STUDIO_BUILD_RELEASE_APK_NAME}"
if [ ! -f "${EXPECTED_APK}" ]; then
    echo "ERROR: Gradle build did not produce expected APK at:"
    echo "  ${EXPECTED_APK}"
    echo "The build may have failed silently. Check Gradle output above."
    exit 1
fi

echo "Copying .apk from ($ANDROID_STUDIO_BUILD_APK_OUTPUT) to the target tree"
cp -R "${ANDROID_STUDIO_BUILD_APK_OUTPUT}" "${TARGET_PATH}"

EHS_APK=${TARGET_PATH}/apk/release/${ANDROID_STUDIO_BUILD_RELEASE_APK_NAME}
cp ${EHS_APK} ${TARGET_PATH}/bin/${ANDROID_TARGET_APK_NAME}

if [ ! -f "${TARGET_PATH}/bin/${ANDROID_TARGET_APK_NAME}" ]; then
    echo "ERROR: Failed to stage APK to ${TARGET_PATH}/bin/${ANDROID_TARGET_APK_NAME}"
    exit 1
fi
echo "APK staged successfully: ${TARGET_PATH}/bin/${ANDROID_TARGET_APK_NAME}"
echo "Done!"
echo "====================================================================================="

echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
