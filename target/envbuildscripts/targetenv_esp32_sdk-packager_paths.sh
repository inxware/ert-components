
# source this in your scripts to set 
# TOOLCHAIN_FLASHING_VERSION
# CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION

# This provides path variables for various esp32 packing and flashing utilities.
# It pick up configuration from the ert- build system or can be given a command line swit5che to choose the right SDKs.

# Find the path of the relevant tools for the SDK versions. Note: this is not very good!
# - we would ideally just be using the standard $TOOLCHAIN_PATH to find all the tool chanin bits, 
# - but these are flashing rather than building tools so lets atttempt to find the ones we know.
# todo all the utilities in here are a munge of versions and all should use t=something like the below (as a library function) 
# to find the right versions.



# If we have this set then we know we are running ffrom the make sytem. Otherwise we can set the envivariables from a command line switch
if [ "$TOOLCHAIN_NAME" = "" ]; then

    if   [ "$1" = "esp32s3-5.1" ]; then
        TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1
    elif [ "$1" = "esp32s3-4.4.4" ]; then
        TOOLCHAIN_NAME=xtensa-esp32s3-elf-4.4.4
    elif [ "$1" = "esp32-4.4.4" ]; then
        TOOLCHAIN_NAME=xtensa-esp32-elf-4.4.4
    elif [ "$1" = "esp32-4.4.1" ]; then
        TOOLCHAIN_NAME=xtensa-esp32-elf-4.4.1
    fi

    if [ "$TOOLCHAIN_NAME" = "" ]; then
        echo "ERROR: TOOLCHAIN_NAME is not set by the ert-components make system."
        echo "However You can use the following command line switches to choose a target:"
        echo "esp32s3-5.1, esp32s3-4.4.4, esp32-5.1, esp32-4.4.1"
        echo "e.g. ./scripts/build-deploy/esp32/esp32_flash.sh esp32s3-5.1"
        exit 1
    fi
fi

if [ "$TOOLCHAIN_NAME" = "xtensa-esp32s3-elf-5.1" ]; then
   #This doesn't seem to exist as we might expect? TOOLCHAIN_FLASHING_VERSION=espressif-5.1
   TOOLCHAIN_FLASHING_VERSION=espressif-4.4.4
   CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION=esp-idf-5.1
   ESP32_CHIP_VERSION=esp32s3
   
elif [ "$TOOLCHAIN_NAME" = "xtensa-esp32s3-elf-4.4.4" ]; then
    TOOLCHAIN_FLASHING_VERSION=espressif-4.4.4
    CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION=esp-idf-4.4.4
     ESP32_CHIP_VERSION=esp32s3
elif [ "$TOOLCHAIN_NAME" = "xtensa-esp32s3-elf-4.4.1" ]; then
    TOOLCHAIN_FLASHING_VERSION=espressif-4.4.1
    CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION=esp-idf-4.4.1
     ESP32_CHIP_VERSION=esp32s3
elif [ "$TOOLCHAIN_NAME" = "xtensa-esp32-elf-4.4.1" ]; then
    TOOLCHAIN_FLASHING_VERSION=espressif-4.4.1
    CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION=esp-idf-4.4.1
    ESP32_CHIP_VERSION=esp32
else
    echo "TOOLCHAIN_NAME is not set to a recognised tools chain."
    echo "e.g. these are for setting related flashing tool paths found in "
    echo "../ert-ert-build-support/...TOOLCHAIN_PATH.../XXXXXXXXX/tools/ and for"
    echo "../ert-contrib-middleware/contrib/esp-idf/XXXXXXXXX/components/esptool_py/esptool/esptool.py"
    exit 
fi