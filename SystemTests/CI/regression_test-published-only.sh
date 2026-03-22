#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2010-2025, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
# <https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------
#
# 'linux_x86_64-lucid-debian11'

source ./scripts/build-function-library/colour.sh
# Default to cyan text in this script
TXT_FG=${TXT_FG_CYAN}

# Community Tragets exported (2025-10-20)
#arduino_arduino-mbed-nano_base           esp32s3_freertos-xtensa-base_n8r2  linux_arm64_gtk_gst_gg_debian10        linux_armv7l_clang_debian10     linux_x86_64_clang_gg_debian11                   linux_x86_64_lvgl_debian11-debug  nxp_arm_inx_hri_ehs_debug
#arduino_arduino-mbed-nano-image_community  esp32s3_freertos-xtensa-community  linux_arm64_gtk_gst_gg_debian11        linux_armv7l_clang_debian9     linux_x86_64_clang_gtk                       linux_x86_debian11             OS_ARCH_PLATFORM-TEMPLATES
#arduino_arduino-mbed-nano_lib           esp32s3_freertos-xtensa-no-certs   linux_arm64_gtk_gst_gg_debian11-no-certs    linux_armv7l_clang_gtk_debian10  linux_x86_64_clang_gtk_gst_gg_debian11-devman-inxsystems  linux_x86_gtk             win_x86
#arduino_arduino-mbed-nano-lib_community    linux_amd64_debian9              linux_arm64_lvgl_gg_debian10        linux_armv7l_clang_gtk_debian9     linux_x86_64_clang_gtk_gst_gg_debian11-no-certs       linux_x86_gtk_gst_deb         win_x86_32-lucid-win10
#esp32_freertos-xtensor-base           linux_amd64_gtk_gst_debian9          linux_arm64_lvgl_gg_debian11        linux_x86             linux_x86_64_clang_lvgl_debian11               linux_x86_gtk_gst_deb_withtools   win_x86_gtk_gst
#esp32s3_freertos-xtensa-base           linux_android_arm              linux_arm_gtk_gst                linux_x86_64_clang         linux_x86_64_clang_lvgl_debian11-no-certs           linux_x86_gtk_vlc             win_x86_gtk_vlc
#esp32s3_freertos-xtensa-base-debug       linux_android_arm64              linux_arm_gtk_gst_raspberrypi        linux_x86_64_clang_gg_debian10     linux_x86_64_gtk_gst_debian11                   nxp_arm                 win_x86_lvgl
#linux_x86_64_clang_g


target_platforX_small=(
    'linux_x86_64_clang_gtk_gst_gg_debian11-no-certs'
    'linux_x86_64_clang_lvgl_debian11-no-certs'
    'linux_arm64_gtk_gst_gg_debian11'
    'linux_arm64_lvgl_gg_debian11'
    'esp32s3_freertos-xtensa-community'
    'arduino_arduino-mbed-nano-image_community'
    'arduino_arduino-mbed-nano-lib_community'
    'win_x86_32-lucid-win10'
)

target_platform=(
    'linux_amd64_debian9'
    'linux_amd64_gtk_gst_debian9'
    'linux_x86_64_clang'
    'linux_x86_64_clang_gtk_gst_gg_debian11-no-certs'
    'linux_x86_64_gtk_gst_debian11'
    'linux_x86_64_clang_lvgl_debian11'
    'linux_armv7l_clang_debian10'
    'linux_arm64_gtk_gst_gg_debian10'
    'linux_arm64_lvgl_gg_debian10'
    'linux_arm64_lvgl_gg_debian11'
    'linux_arm64_gtk_gst_gg_debian11'
    'linux_android_arm'
    'esp32_freertos-xtensa-base'
    'esp32s3_freertos-xtensa-base'
    'arduino_arduino-mbed-nano-image_community'
    'arduino_arduino-mbed-nano-lib_community'
    'win_x86_gtk_gst'
    'win_x86_32-lucid-win10'
    'linux_arm64_lvgl_jetson_nano_ngc'
)

echo "Testing the following platforms:"
#target_platform=( "${taregt_platform_github_ci[*]}" )
for platform in "${target_platform[@]}"; do
    echo "- ${platform}"
done

#Parked targets
# 'linux_armv7l_clang_debian11'
# 'linux_x86_gtk_vlc'
# 'linux_x86_64_clang_gg_debian11'
# 'linux_armv7l_clang_gtk_gst_debian11'

if [ "$1" = "--include-slow" ]; then
    # Long running ones...
    target_platform=(
        ${target_platform}
        'linux_android_arm64_unity-lib'
    )
elif [ "$1" = "--compile-only" ]; then
    COMPILE_ONLY=yes
fi
#####################################################
#Set up paths
#####################################################

#set -e

if [ -d ../CI ]; then
    ROOT_DIR=$PWD/../../
else
    ROOT_DIR=$PWD
fi

CI_RESULT_DIR=${ROOT_DIR}/SystemTests/CI/results
TARGET_TREE_DIR=${ROOT_DIR}/../TARGET_TREES
SYSTEM_TESTS_DIR=${ROOT_DIR}/SystemTests/LucidTestApps/DefaultTestApp/export/
TOOL_TEST_EHS_VARIANT=linux_x86_gtk

function display_target {
    echo
    echo "--------------------------------------------------------------------"
    echo
    echo -n "Starting regression test for "
    heading "${platform}"
}

function display_log_tail {
    echo ">>>>>> START LOG TAIL ($1) >>>>>>"
    tail -n 20 $1
    echo "<<<<<< END LOG TAIL <<<<<<"
}

function build_target {
    if [ ! -d ../ert-components ]; then
        err "Not in EHS directory, in ${PWD}. press 'y' to continue"
        read -n 1 WHAT
        if [ $WHAT != "y" ] ; then
            echo "Exiting"
            exit 1
        fi
    fi

    local LOG=${CI_RESULT_DIR}/${platform}/build.log
    local FAIL=0

    # Don't do this so we can test branches in different adjascent directories
    #cd ${ROOT_DIR}/../ert-components #just to make sure!
    make clean &> /dev/null ||:
    ./configure ${platform} &> "${LOG}" || { err "Platform not found!"; exit 1; }

    make targetenv_cleanall &>> "${LOG}" ||:
    touch ${CI_RESULT_DIR}/${platform}/build.started

    # Docker build (or not sometimes if not configured) - we probably don't need to check these days as make all_docker will default to a hist build if not image is found
     if [ -f "${ROOT_DIR}/target/platform/${platform}/Dockerimagename" ]; then
        if make all_docker &>> "${LOG}"; then 
            echo "${TXT_FG_BRIGHT_GREEN}${platform} all_docker build success"
            touch ${CI_RESULT_DIR}/${platform}/build.pass
            if make targetenv &>> "${LOG}"; then 
                echo "${TXT_FG_BRIGHT_GREEN}${platform} targetenv build success"
                touch ${CI_RESULT_DIR}/${platform}/build-target.pass
            else
                err "${TXT_FG_BRIGHT_GREEN}${platform} targetenv build failed"
                FAIL=1
                display_log_tail "${LOG}"
                touch ${CI_RESULT_DIR}/${platform}/build-target.fail
            fi
        else
            err "${platform} Docker build failed - targetenv build not run"
            FAIL=1
            display_log_tail "${LOG}"
            touch ${CI_RESULT_DIR}/${platform}/build.fail
            touch ${CI_RESULT_DIR}/${platform}/build-target.notrun
        fi
    else
        # Build on the Host - we don't generally want to do this, but during experimentation stages it is usually easier to build interactively without a prebuilt Docker image.
        if make -j 8 &>> "${LOG}"; then 
            touch ${CI_RESULT_DIR}/${platform}/build.pass
            if make targetenv &>> "${LOG}"; then 
                echo "${TXT_FG_BRIGHT_GREEN}${platform} build success"
                touch ${CI_RESULT_DIR}/${platform}/build-target.pass
            else
                err "${platform} build failed"
                FAIL=1
                display_log_tail "${LOG}"
                touch ${CI_RESULT_DIR}/${platform}/build-target.fail
            fi
        else
            err "${platform} build failed"
            FAIL=1
            display_log_tail "${LOG}"
            touch ${CI_RESULT_DIR}/${platform}/build-target.notrun
            touch ${CI_RESULT_DIR}/${platform}/build.fail
        fi
    fi

    if [ ${FAIL} -eq 0 -a "${COMPILE_ONLY}" != "yes" ]; then
        # Check if we need to build a Unity Android project
        if [[ "${platform}" == *"_unity"* ]] && [ "${platform}" != "linux_android_arm64_unity-lib" ]; then
            echo "========= Building unity app for the target ============="
            if make targetenv_unity_export &>> "${LOG}"; then
                echo "${TXT_FG_BRIGHT_GREEN}${platform} unity build success"
                touch ${CI_RESULT_DIR}/${platform}/build-unity.pass
            else
                err "${platform} unity build failed"
                FAIL=1
                display_log_tail "${LOG}"
                touch ${CI_RESULT_DIR}/${platform}/build-unity.fail
            fi
        fi

        # Check if this is Android
        echo "Checking for android ehs_${platform}.so"
        if [ ${FAIL} -eq 0 -a -f "ehs_${platform}.so" ]; then
            echo "Found ehs_${platform}.so - building the .apk target"
            if make targetenv_apk &>> "${LOG}"; then 
                echo "${TXT_FG_BRIGHT_GREEN}${platform} APK build success"
                touch ${CI_RESULT_DIR}/${platform}/build-apk.pass
            else
                err "${platform} APK build failed"
                FAIL=1
                display_log_tail "${LOG}"
                touch ${CI_RESULT_DIR}/${platform}/build-apk.fail
            fi
        fi
    fi

    # Clean up
    rm -f ${CI_RESULT_DIR}/${platform}/build.started

    return $FAIL
}

function test_build {
    if test -e ${TARGET_TREE_DIR}/ehs_env-${platform}/bin/ehs.exe ; then
        echo "${TXT_FG_BRIGHT_GREEN}'ehs.exe' ran successfully"
        touch ${CI_RESULT_DIR}/${platform}/exe-built.pass
    else
        if [ -f "ehs_${platform}.so" ]; then
            echo "${TXT_FG_BRIGHT_GREEN}'ehs_${platform}.so' built successfully"
            touch ${CI_RESULT_DIR}/${platform}/exe-built.pass
        else
            err "'ehs_${platform}.so' failed to build"
            touch ${CI_RESULT_DIR}/${platform}/exe-built.fail
        fi
    fi
}

function remove_build {
    rm -f ${TARGET_TREE_DIR}/ehs_env-${platform}/bin/ehs.exe &>/dev/null ||:
    rm -f ${TARGET_TREE_DIR}/ehs_env-${platform}/bin/*.so &>/dev/null ||:
}

###################### Main ####################################

test -n "${CI_RESULT_DIR}" && mkdir -p "${CI_RESULT_DIR}" || exit 1
cd ${ROOT_DIR} || exit 1

if [ "${CI_RESULT_DIR}" = "" ]; then
    err "'CI_RESULT_DIR' is unset"
    exit 1
else
    # Clean the results directory
    rm -Rf ${CI_RESULT_DIR}/*
fi

#####################################################
# Pre-flight: validate function block CDF hashes
#####################################################
echo
heading "Checking CDF NameHash_CRC16 consistency across all function blocks..."
if python3 scripts/inxware-id-tool/check_cdf_hashes.py Common/Components; then
    echo "${TXT_FG_BRIGHT_GREEN}CDF hash check passed"
else
    err "CDF hash check FAILED — fix NameHash_CRC16 mismatches before building"
    exit 1
fi

for platform in "${target_platform[@]}"; do
    display_target
    if [  "${platform}" == ".svn" -o "${platform}" == "depricated" -o "`expr "${platform}" : "base.*"`" -gt 0 ]; then
        warn "Ignoring directory ${platform}"
    else
        echo "Building and testing ${platform}"
        mkdir -p "${CI_RESULT_DIR}/${platform}"
        rm -Rf "${CI_RESULT_DIR}/${platform}/*" &> /dev/null
        touch "${CI_RESULT_DIR}/${platform}/building.flag"
        remove_build
        if build_target; then
            if [ "$COMPILE_ONLY" != "yes" ]; then
                test_build
            fi
        fi
        rm -f "${CI_RESULT_DIR}/${platform}/building.flag"
        #tree "${CI_RESULT_DIR}"
    fi
done

# Report summary
LS_COLORS=${LS_COLORS}"*.fail=10;31:*.pass=00;32:" ls -l --color=always ${CI_RESULT_DIR}/*/

# Exit with a suitable return code
ls -- "${CI_RESULT_DIR%/}"/*/*.fail >/dev/null 2>&1 && exit 1 || exit 0

