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

export UNITY_BUILD_TOOLS_VERSION="${EHS_UNITY_VERSION:-2022.3.62f3}"
export UNITY_BUILD_TOOLS_ROOT="$REPOSITORY_ROOT/unity3d-build-support"
export UNITY_BUILD_TOOLS="$UNITY_BUILD_TOOLS_ROOT/$UNITY_BUILD_TOOLS_VERSION/Editor"
export ANDROID_SDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/SDK"
export ANDROID_NDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/NDK"
export ANDROID_JDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/OpenJDK"
export JAVA_HOME=$ANDROID_JDK

export ANDROID_STUDIO_ROOT=${TARGET_PATH}/android_studio_project
# Derive the Android Studio template directory from the Unity major.minor version, e.g.
# 2022.3.62f3 -> android_studio_unity_ehs_2022.3
# 6000.4.1f1  -> android_studio_unity_ehs_6000.4
# Override by setting EHS_UNITY_ANDROID_STUDIO_TEMPLATE explicitly in config.mk.
_UNITY_MAJOR_MINOR=$(echo "$UNITY_BUILD_TOOLS_VERSION" | cut -d. -f1,2)
export REPOSITORY_ANDROID_STUDIO_ROOT="$EHS_ROOT/target/os-arch/android_ALL/${EHS_UNITY_ANDROID_STUDIO_TEMPLATE:-android_studio_unity_ehs_${_UNITY_MAJOR_MINOR}}"
export REPOSITORY_ANDROID_STUDIO_LIBS="$EHS_ROOT/target/os-arch/android_ALL/android_studio_ehs"

export EHS_UNITY_PROJECT_ROOT="$EHS_ROOT/target/os-arch/android_ALL/Unity_EHS"
export EHS_UNITY_PROJECT_PATH="$TARGET_PATH/Unity_EHS"

function CancelFailed(){
    echo "FAILED to export Unity target=($SPECIFIC_TARGET) !!!"
    # rm -rf ${TARGET_PATH}
	exit 1
}

function PrepareUnityToolchain(){
    # --- Path 1: host-installed Unity ---------------------------------------
    # Override by setting UNITY_HOST_EDITOR_PATH in your environment, e.g.:
    #   export UNITY_HOST_EDITOR_PATH=~/TOOLS/Unity3D/6000.4.1f1/Editor
    # Otherwise searched in order: /opt/unity3d, then anywhere under /opt or ~/TOOLS.
    # Install via: ./scripts/build-deploy/unity/install-unity-host.sh
    # Use with:    make targetenv_unity_export  (no Docker)
    if [ -n "$UNITY_HOST_EDITOR_PATH" ]; then
        HOST_UNITY_PATH="$UNITY_HOST_EDITOR_PATH"
    else
        HOST_UNITY_PATH="/opt/unity3d/${UNITY_BUILD_TOOLS_VERSION}/Editor"
        if ! [ -x "$HOST_UNITY_PATH/Unity" ]; then
            FOUND=$(find /opt "$HOME/TOOLS" -name "Unity" -type f \
                -path "*${UNITY_BUILD_TOOLS_VERSION}*" 2>/dev/null | head -1)
            if [ -n "$FOUND" ]; then
                HOST_UNITY_PATH=$(dirname "$FOUND")
                echo "Unity3d found at $HOST_UNITY_PATH"
            fi
        fi
    fi
    if [ -x "$HOST_UNITY_PATH/Unity" ]; then
        echo "Unity3d found at host path ($HOST_UNITY_PATH)."
        export UNITY_BUILD_TOOLS="$HOST_UNITY_PATH"
        export ANDROID_SDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/SDK"
        export ANDROID_NDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/NDK"
        export ANDROID_JDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/OpenJDK"
        export JAVA_HOME=$ANDROID_JDK
        return
    fi

    # --- Path 2: GameCI unityci/editor Docker image --------------------------
    # Unity lives at /opt/unity/Editor inside those containers.
    # Use with:    make targetenv_unity_export_docker
    # Setup via:   ./scripts/build-deploy/unity/setup-unity-license.sh
    GAMECI_UNITY_PATH="/opt/unity/Editor"
    if [ -x "$GAMECI_UNITY_PATH/Unity" ]; then
        echo "Unity3d found at GameCI path ($GAMECI_UNITY_PATH)."
        export UNITY_BUILD_TOOLS="$GAMECI_UNITY_PATH"
        export ANDROID_SDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/SDK"
        export ANDROID_NDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/NDK"
        export ANDROID_JDK="$UNITY_BUILD_TOOLS/Data/PlaybackEngines/AndroidPlayer/OpenJDK"
        export JAVA_HOME=$ANDROID_JDK
        return
    fi


    if ! [ -d "$UNITY_BUILD_TOOLS" ]; then
        echo ""
        echo "========================================================================="
        echo "ERROR: Unity ${UNITY_BUILD_TOOLS_VERSION} not found."
        echo ""
        echo "Option A — install Unity on this host (recommended, no Docker license"
        echo "           issues), then use 'make targetenv_unity_export':"
        echo ""
        echo "  ./scripts/build-deploy/unity/install-unity-host.sh"
        echo ""
        echo "  Unity will be installed to: /opt/unity3d/${UNITY_BUILD_TOOLS_VERSION}/"
        echo "  After install, activate a free Personal license via Unity Hub."
        echo ""
        echo "Option B — run via GameCI Docker image (requires UNITY_LICENSE env var),"
        echo "           then use 'make targetenv_unity_export_docker':"
        echo ""
        echo "  ./scripts/build-deploy/unity/setup-unity-license.sh"
        echo "  export UNITY_LICENSE=\$(cat /tmp/unity-activation/Unity_v*.ulf)"
        echo "========================================================================="
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

    # Determine ABI layout.
    # arm64-only targets (Android 12+ 64-bit boards, e.g. Radxa): set EHS_ANDROID_NATIVE_ABI=arm64
    # in config.mk — the app .so goes straight into arm64/; no 32-bit companion is needed.
    # Legacy dual-ABI targets (32-bit ARM Android): leave EHS_ANDROID_NATIVE_ABI unset — the
    # 32-bit app .so goes into arm/ and a pre-built 64-bit dependency goes into arm64/.
    NATIVE_ABI="${EHS_ANDROID_NATIVE_ABI:-arm}"

    if [ "$NATIVE_ABI" = "arm64" ]; then
        echo "arm64-only target: skipping 32-bit dependency check"
    else
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
    fi

    echo "Cleaning stale Unity Android Studio export directory ($ANDROID_STUDIO_ROOT)"
    rm -rf "$ANDROID_STUDIO_ROOT"
    mkdir -p "$ANDROID_STUDIO_ROOT" || CancelFailed

    echo "Copying EHS Unity project ($EHS_UNITY_PROJECT_ROOT) to $EHS_UNITY_PROJECT_PATH"
    # Replace the entire destination directory so stale files from previous runs
    # (Library/, packages-lock.json) do not carry over. Using rm+cp rather than
    # rsync --delete to avoid any dependency on rsync being present.
    rm -rf "$EHS_UNITY_PROJECT_PATH"
    cp -r "$EHS_UNITY_PROJECT_ROOT" "$EHS_UNITY_PROJECT_PATH" || CancelFailed

    # Unity 2022.3 requires a custom gradleTemplate.properties with unityTemplateVersion=7 to
    # pass its template-version check during Android export; the file is committed to Unity_EHS.
    # Unity 6000+ manages template versioning internally (unityTemplateVersion=21+); if the
    # Unity-2022.3-era file is present it would cause a mismatch, so remove it.
    # Unity 6000+ has incompatible template formats; remove Unity-2022-era custom templates
    # so Unity 6 uses its built-in templates (which handle versioning and IL2CPP differently).
    if [[ "$UNITY_BUILD_TOOLS_VERSION" == 6000* ]]; then
        for _TPL in gradleTemplate.properties mainTemplate.gradle; do
            _F="$EHS_UNITY_PROJECT_PATH/Assets/Plugins/Android/$_TPL"
            if [ -f "$_F" ]; then
                rm -f "$_F" "${_F}.meta"
                echo "Removed Unity-2022 $_TPL for Unity 6000 compatibility"
            fi
        done
    fi

    # overwrite with latest eRT plugins
    # libc++_shared.so must be bundled alongside libnative-activity.so — it is the C++
    # runtime required by liblitert_c.a (built with NDK r27c) and is not guaranteed to
    # be present on the device.  It is stored in EHS_COMPONENT_SUPPORT_LIBS by the
    # contrib build (extracted from the same NDK r27c Docker image used to build TFLite).
    EHS_LIBCPP="${EHS_COMPONENT_SUPPORT_LIBS:-}libc++_shared.so"
    if [ ! -f "$EHS_LIBCPP" ]; then
        # Fallback: derive from the known contrib path relative to EHS_ROOT
        EHS_LIBCPP="$EHS_ROOT/../ert-contrib-middleware/target_libs/armv7a-linux-android/build/lib/libc++_shared.so"
    fi
    if [ ! -f "$EHS_LIBCPP" ]; then
        echo "WARNING: libc++_shared.so not found — APK may crash on device."
        echo "         Run: make all_docker to ensure it is extracted from NDK r27c."
    fi

    if [ "$NATIVE_ABI" = "arm64" ]; then
        echo "Copying 64-bit ehs plugin to Unity project (arm64-only)"
        cp ${EHS_ANDROID_LIB_FILE} "$EHS_UNITY_PROJECT_PATH/Assets/Libs/arm64/libnative-activity.so" || CancelFailed
    else
        echo "Copying 32-bit ehs plugin to Unity project"
        cp ${EHS_ANDROID_LIB_FILE} "$EHS_UNITY_PROJECT_PATH/Assets/Libs/arm/libnative-activity.so" || CancelFailed
        if [ -f "$EHS_LIBCPP" ]; then
            echo "Copying libc++_shared.so (NDK r27c) to Unity project"
            cp "$EHS_LIBCPP" "$EHS_UNITY_PROJECT_PATH/Assets/Libs/arm/libc++_shared.so" || CancelFailed
        fi
        echo "Copying 64-bit ehs plugin to Unity project"
        cp ${DEPENDENCY_FILE} "$EHS_UNITY_PROJECT_PATH/Assets/Libs/arm64/libnative-activity.so" || CancelFailed
    fi

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

    # Limit IL2CPP parallel C++ compilation to avoid memory exhaustion.
    # bee_backend (the Bee build system invoked by il2cpp) defaults to --threads=<cpu_count>.
    # On machines with many cores, compiling ~2500 large IL2CPP-generated C++ files in
    # parallel exhausts available RAM and deadlocks both il2cpp and bee_backend.
    # We patch the Unity-generated unityLibrary/build.gradle to pass --jobs=N to il2cpp.
    # Override via: export EHS_IL2CPP_JOBS=8  (default: 4)
    _IL2CPP_JOBS="${EHS_IL2CPP_JOBS:-4}"
    _UNITY_BUILD_GRADLE="$ANDROID_STUDIO_ROOT/unityLibrary/build.gradle"
    if grep -q 'commandLineArgs.add("--compile-cpp")' "$_UNITY_BUILD_GRADLE" 2>/dev/null; then
        sed -i 's/commandLineArgs\.add("--compile-cpp")/commandLineArgs.add("--compile-cpp")\n        commandLineArgs.add("--jobs='"${_IL2CPP_JOBS}"'")/' "$_UNITY_BUILD_GRADLE"
        echo "IL2CPP parallel jobs limited to ${_IL2CPP_JOBS} (patched unityLibrary/build.gradle)"
    else
        echo "WARNING: Could not patch IL2CPP --jobs in $ANDROID_STUDIO_ROOT/unityLibrary/build.gradle (pattern not found)"
    fi

    # Patch bee_backend IPC hang: Unity 2022+ runs il2cpp via bee_backend in
    # --beedriver-listener (persistent daemon) mode, which never exits after completing
    # its work in a one-shot Gradle build. Replace the blocking exec{} with a watchdog
    # that runs il2cpp in the background and kills it once libil2cpp.so is produced and
    # tundra's build log confirms FinalProgram (the completion sentinel) has been reached.
    python3 - "$_UNITY_BUILD_GRADLE" <<'PYEOF'
import sys, re

path = sys.argv[1]
text = open(path).read()

old = r"""    exec \{
        executable workingDir \+ "/src/main/Il2CppOutputProject/IL2CPP/build/deploy/il2cpp" \+ executableExtension
        args commandLineArgs
        environment "ANDROID_SDK_ROOT", getSdkDir\(\)
    \}"""

new = '''    // Workaround: bee_backend --beedriver-listener never exits after completing work in a
    // one-shot Gradle build (Unity 2022+ regression). Run il2cpp in the background and
    // kill it (plus its bee_backend child) once outputs are ready.
    def il2cppBin = workingDir + "/src/main/Il2CppOutputProject/IL2CPP/build/deploy/il2cpp" + executableExtension
    def outputSo  = new File(workingDir + "/src/main/jniLibs/" + abi + "/libil2cpp.so")
    def tundraLog = new File(workingDir + "/build/il2cpp_" + abi + "_" + configuration + "/il2cpp_cache/buildstate/tundra.log.json")
    def pb = new ProcessBuilder([il2cppBin] + commandLineArgs)
    pb.environment().put("ANDROID_SDK_ROOT", getSdkDir())
    pb.redirectOutput(ProcessBuilder.Redirect.INHERIT)
    pb.redirectError(ProcessBuilder.Redirect.INHERIT)
    def il2cppProcess = pb.start()
    def deadline = System.currentTimeMillis() + 7200000L
    while (il2cppProcess.alive && System.currentTimeMillis() < deadline) {
        Thread.sleep(5000)
        if (outputSo.exists() && tundraLog.exists() &&
                tundraLog.text.contains('"annotation":"FinalProgram"')) {
            Thread.sleep(3000)
            println "IL2CPP outputs ready for ${abi} — killing il2cpp/bee_backend"
            ['bash', '-c', "pkill -KILL -P ${il2cppProcess.pid()} 2>/dev/null; kill -KILL ${il2cppProcess.pid()} 2>/dev/null; true"].execute().waitFor()
            Thread.sleep(1000)
            il2cppProcess.destroyForcibly()
        }
    }
    il2cppProcess.waitFor()
    if (!outputSo.exists()) {
        throw new GradleException("IL2CPP build failed for ${abi}: output not found at ${outputSo}")
    }'''

result = re.sub(old, new, text)
if result == text:
    print("WARNING: Could not patch bee_backend watchdog in " + path + " (exec{} pattern not found)")
else:
    open(path, 'w').write(result)
    print("bee_backend watchdog patched in " + path)
PYEOF

    # Patch BuildIl2CppTask to declare its outputs so Gradle considers it up-to-date
    # on subsequent invocations (e.g. signReleaseBundle) and doesn't re-run IL2CPP.
    python3 - "$_UNITY_BUILD_GRADLE" <<'PYEOF'
import sys, re

path = sys.argv[1]
text = open(path).read()

old = r'(    task BuildIl2CppTask \{)'
new = r'''\1
        outputs.file("${projectDir}/src/main/jniLibs/armeabi-v7a/libil2cpp.so")
        outputs.file("${projectDir}/src/main/jniLibs/arm64-v8a/libil2cpp.so")'''

result = re.sub(old, new, text)
if result == text:
    print("WARNING: Could not patch BuildIl2CppTask outputs in " + path)
else:
    open(path, 'w').write(result)
    print("BuildIl2CppTask outputs patched in " + path)
PYEOF
}


#########################################################################################

echo ""

if [ "${EHS_UNITY_PROJECT_EXPORT_SUPPORT}" = "" ]; then
    echo "FAILED!"
    echo "This option can only be run for targets that has Unity export support (EHS_UNITY_PROJECT_EXPORT_SUPPORT) set in config.mk"
    exit 1
fi

# Always resolve the Unity toolchain paths (UNITY_BUILD_TOOLS, ANDROID_SDK, ANDROID_NDK,
# ANDROID_JDK, JAVA_HOME) so that scripts sourcing this file with "no-run" (e.g.
# targetenv_make_apk.sh) still get correct SDK paths even when the export is skipped.
PrepareUnityToolchain

# Set Android SDK/NDK root env vars and accept SDK licences.
# Done unconditionally so 'make targetenv_apk' (which sources this with "no-run")
# also has a correctly licensed SDK before invoking Gradle.
export ANDROID_SDK_ROOT=$ANDROID_SDK
export ANDROID_NDK_ROOT=$ANDROID_NDK
# Prefer cmdline-tools/latest; fall back to any versioned cmdline-tools dir
# (Unity bundles a specific version, e.g. 16.0, not a "latest" symlink).
# Only use the legacy tools/bin/sdkmanager as a last resort — it requires Java 8.
if [ -x "$ANDROID_SDK/cmdline-tools/latest/bin/sdkmanager" ]; then
    SDKMANAGER="$ANDROID_SDK/cmdline-tools/latest/bin/sdkmanager"
else
    SDKMANAGER=$(find "$ANDROID_SDK/cmdline-tools" -name "sdkmanager" -maxdepth 3 2>/dev/null | head -1)
    if [ -z "$SDKMANAGER" ]; then
        SDKMANAGER="$ANDROID_SDK/tools/bin/sdkmanager"
    fi
fi
export PATH=$PATH:$(dirname "$SDKMANAGER")
LICENCE_CHECKED="$UNITY_BUILD_TOOLS/licence_checked"
if ! [ -f "$LICENCE_CHECKED" ]; then
    mkdir -p "$HOME/.android" && touch "$HOME/.android/repositories.cfg"
    yes | "$SDKMANAGER" --licenses && echo "checked" > "$LICENCE_CHECKED" || echo "WARNING: sdkmanager --licenses failed — build may fail due to unaccepted SDK licences"
fi

# Write local.properties so Gradle always uses the correct SDK regardless of which Unity
# version last exported the project. Done unconditionally so 'make targetenv_apk' (no-run
# mode) also fixes any stale sdk.dir left by a previous export with a different Unity version.
echo "sdk.dir=$ANDROID_SDK" > "$ANDROID_STUDIO_ROOT/local.properties"

if [ "$WITHOUT_RUNNING_EXPORT" = "" ]; then

    # @TODO - enable exporting projects/apps for othe platforms e.g. windows
    ExportUnityAndroidStudio

    echo "================================================="
    echo "UNITY EXPORT COMPLETED"
    echo "================================================="

fi
