#!/bin/bash

set -e

UPLOAD_ASSETS_ONLY=""

if [ "$1" = "--assets-only" ]; then
    UPLOAD_ASSETS_ONLY="yes"
fi

function build_linux_based_target() {
    ./configure $1
    if [ "${UPLOAD_ASSETS_ONLY}" = "yes" ]; then
        make targetenv_upload_appland ASSETS_ONLY=yes
    else
        [ "$2" = "prepdeps" ] && make prepdeps
        make clean ||:
        make all_docker
        make targetenv
        make targetenv_deb_docker
        make targetenv_upload_appland
    fi
}

function build_windows_based_target() {
    ./configure $1
    if [ "${UPLOAD_ASSETS_ONLY}" = "yes" ]; then
        make targetenv_upload_appland ASSETS_ONLY=yes
    else
        [ "$2" = "prepdeps" ] && make prepdeps
        make clean ||:
        make all_docker
        make targetenv
        make targetenv_nsis_docker
        make targetenv_upload_appland
    fi
}

function build_esp32_based_target() {
    ./configure $1
    if [ "${UPLOAD_ASSETS_ONLY}" = "yes" ]; then
        make targetenv_upload_appland ASSETS_ONLY=yes
    else
        [ "$2" = "prepdeps" ] && make prepdeps
        make clean ||:
        make targetenv_prebuild
        make targetenv_littlefs
        make all_docker
        make targetenv_esp32_docker
        make targetenv_upload_appland
    fi
}

function build_android_based_target() {
    ./configure $1
    if [ "${UPLOAD_ASSETS_ONLY}" = "yes" ]; then
        make targetenv_upload_appland ASSETS_ONLY=yes
    else
        [ "$2" = "prepdeps" ] && make prepdeps
        make clean ||:
        make all_docker
        make targetenv
        make targetenv_apk_docker
        make targetenv_upload_appland
    fi
}

function build_arduino_based_target() {
    ./configure $1
    if [ "${UPLOAD_ASSETS_ONLY}" = "yes" ]; then
        make targetenv_upload_appland ASSETS_ONLY=yes
    else
        [ "$2" = "prepdeps" ] && make prepdeps
        make clean ||:
        make all_docker
        make targetenv
        make targetenv_arduino
        make targetenv_upload_appland
    fi
}


# build raspberry pi tragets, and do 'prepdeps'
build_linux_based_target "linux_arm64_lvgl_gg_debian11" "prepdeps"

# build debian tragets
build_linux_based_target "linux_x86_64-lucid-debian11"

# build windows tragets
build_windows_based_target "win_x86_32-lucid-win10"

# build esp32 tragets
build_esp32_based_target "esp32s3_freertos-xtensa-community"
build_esp32_based_target "esp32s3_freertos-xtensa-community-inx-devman"

# build android tragets
build_android_based_target "linux_android_arm_community"

# build arduino targets
build_arduino_based_target "arduino_arduino-mbed-nano-image_community"
build_arduino_based_target "arduino_arduino-mbed-nano-lib_community"
