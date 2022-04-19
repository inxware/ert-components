#!/bin/bash
# inx limited 2020
# Setup gradlew tools and build the APK

export SPECIFIC_TARGET=$1
ANDROID_STUDIO_ROOT=$2
EHS_ROOT=`pwd` # assuming we're in the ehs project root
ANDROID_ROOT="$EHS_ROOT/../inx_android_build"
ANDROID_SDK=${ANDROID_ROOT}"/SDK"
JDK_PATH="/usr/lib/jvm/java-8-openjdk-amd64"
ANDROID_SDK_URL="https://dl.google.com/android/repository/commandlinetools-linux-6200805_latest.zip"

ANDROID_PROJRCT_ROOT=$ANDROID_STUDIO_ROOT

if ! [ -d "$ANDROID_PROJRCT_ROOT" ] ; then
    echo "Pass a valid path to android studio project as a first argument of this script."
    exit 1
fi    

if ! [ -f "$ANDROID_PROJRCT_ROOT/gradlew" ]; then
    echo "Pass a valid path to android studio project as a first argument of this script."
    exit 1
fi    

if ! [ -d "$ANDROID_ROOT" ]; then
    mkdir $ANDROID_ROOT
fi

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
if [[ -z "$JAVA_HOME" ]]; then
    echo "Setting JAVA_HOME env."
    export JAVA_HOME="$JDK_PATH"
    echo $JAVA_HOME
fi    

if ! [ -d "$ANDROID_SDK" ]; then
    # Download SDK
    echo "Downloading SDK"
    if ! curl -k -s -o "$ANDROID_ROOT/android-sdk.zip" $ANDROID_SDK_URL ;
    then
        echo "Failed to download SDK"
        "$ANDROID_ROOT/android-sdk.zip"
        exit 1
    fi
    echo "Unpacking SDK to $ANDROID_SDK"
    if ! unzip -qq "$ANDROID_ROOT/android-sdk.zip" -d "$ANDROID_SDK";
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
if [[ -z "$ANDROID_SDK_ROOT" ]]; then
    echo "Setting ANDROID_SDK_ROOT env."
    export ANDROID_SDK_ROOT="$ANDROID_SDK"
    echo $ANDROID_SDK_ROOT
fi

echo "Signing and building EHS APK"
pushd ${EHS_ROOT}/..
SIGN_RELEASE_DATA_PATH=$(pwd)/DevmanSecurity/ehs.google.play
popd
echo "Using key ($SIGN_RELEASE_DATA_PATH)"
# Write all essential app signing things to gradle.properties
GRADLE_PROPS_FILE=${ANDROID_PROJRCT_ROOT}/gradle.properties
echo "RELEASE_STORE_FILE=$SIGN_RELEASE_DATA_PATH/ehs.jks" >> $GRADLE_PROPS_FILE
while read line; do
    echo "$line" >> $GRADLE_PROPS_FILE
done < "$SIGN_RELEASE_DATA_PATH/.release"
#cat $GRADLE_PROPS_FILE
pushd $ANDROID_PROJRCT_ROOT
chmod +x ./gradlew
#./gradlew build
./gradlew clean
./gradlew assembleRelease
popd

echo "Copying .apk to the target tree"

EHS_ANDROID_APK_ROOT="$ANDROID_STUDIO_ROOT/app/build/outputs/apk"

#test -d $EHS_ANDROID_APK_ROOT/debug   && cp -R "$EHS_ANDROID_APK_ROOT/debug" "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}"
test -d $EHS_ANDROID_APK_ROOT/release && cp -R "$EHS_ANDROID_APK_ROOT/release" "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}"
