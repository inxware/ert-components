#!/bin/bash

# creating the esp final binaries into the target/envbuildscripts/ directory along with all the others

####################################################################################################
# This file script will build the ert source using the docker file available or if not will use 
# the host's make system.  
####################################################################################################

set -e

#If this is called as a docker command we wont have the exported environment variables, so read them from the paramaters
if [ "${TARGET}" = "" ]; then 
    echo "Setting TARGET to ${TARGET}"
    export TARGET=$1
fi

if [ "${SPECIFIC_TARGET}" = "" ]; then 
    echo "Setting SPECIFIC_TARGET to ${TARGET}"
    export SPECIFIC_TARGET=${TARGET}
fi

# Check to see if we should use a docker image
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"
#todo check for a config.mk variable - either as an argument or set by mkae as an environment variable.

if [ ! -d /opt/venv/bin ]; then 
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo "The python virtual environment should be installed at /opt/ver/bin/"
    echo "You should run make targetenv_32 in the docker environment that has the "
    echo "python environment installed OR install it using the esp32 scripts in ./ert-contrib-middleware " 
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    read -n 1 -p  "YOU MAY CONTINUE AND THIS SCRIPT WILL TRY TO INSTALL THE ENVIRONMENT ON YOUR HOST MACHINE (y/n)" answer
    if [ "$answer" = "y" ]; then 
       echo "Continuing..."
    else
        echo "Exiting ..."
        exit 0
    fi
else
    echo "Building  deployable esp32 binararies. The results must be subsequently installed with"
    echo "make upload_ehs_via_esp32USB"
    echo "Or deployed to a an OTA server"
    export SPECIFIC_TARGET=$1
fi


export EHS_ROOT=`pwd` # assuming we're in the ehs project root
pushd ${EHS_ROOT}/..
export REPOSITORY_ROOT=`pwd`
popd
#Check if we are alr
if [ -f /.dockerenv ]; then
    echo "Already running in Docker continuing"
    export PATH="/opt/venv/bin:$PATH"
else
    #Presumably this is attempting to create the python environment on the host?

    # The following would be better picked from config.mk rather than hardwired, but would need to passed in for Docker builds.
    export TOOLCHAIN_VERSION="xtensa-esp32-elf-4.4.1"
    export TEMP_PWD=${PWD} 
    INX_HOST_ARCH=$(uname -m)
    TOOLCHAIN_PATH="${TEMP_PWD}/../ert-build-support/toolchains/${INX_HOST_ARCH}/${TOOLCHAIN_VERSION}"
    _PATH="${TOOLCHAIN_PATH}/bin"

    ## Create python virtual environment, install requirements and export it to PATH
    echo "TOOLCHAIN PATH IS $TOOLCHAIN_PATH"

    ## We don't need this of we are relying on the docker one?
    python3 -m venv "${TOOLCHAIN_PATH}/espressif-4.4.1/python_env/venv" > /dev/null

    export IDF_PYTHON_ENV_PATH="${TOOLCHAIN_PATH}/espressif-4.4.1/python_env/venv/bin"

    #Set path to the python and toolchain.
    _PATH="${TOOLCHAIN_PATH}/bin"
    export IDF_PYTHON_ENV_PATH="${TOOLCHAIN_PATH}/espressif-4.4.1/python_env/venv/bin"
    _PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/openocd-esp32/v0.11.0-esp32-20211220/openocd-esp32/bin"
    _PATH="${_PATH:+${_PATH}:}${IDF_PYTHON_ENV_PATH}"
    _PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/esptool_py/esptool"
    _PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/espcoredump"
    _PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/partition_table"
    _PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/app_update"
    _PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/additional_tools"
    export PATH="${_PATH}${PATH:+:${PATH}}"
    #RUN python3 -m venv /opt/venv
fi
#build and copy supportive binaries
#source ../ert-contrib-middleware/inx_build_scripts/source-scripts/inx-xbuilder-source-me-espidf.sh

#build_bootloader esp-idf -4.4.1 bootloader 
#build_partition_table esp-idf -4.4.1 partition_table

# build .bin file from elf file 

echo "---------------------------------------------------------------------------------------------------------------------------"
echo "### Building .bin file for esp32 from .elf file                                                                         ###"
if $python3 ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/components/esptool_py/esptool/esptool.py --chip esp32 elf2image --flash_mode "dio" --flash_freq "40m" --flash_size "4MB" --elf-sha256-offset 0xb0 ehs_esp32_freertos-xtensor-base.elf -o $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.bin 
then
    echo "### Copied ehs.bin file generated in $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/"
else 
    echo "!!! ERROR: COULD NOT GENERATE ehs.bin                                                                               !!!"
fi

if test -f "$PWD/../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/build/bootloader/bootloader.bin" 
then
    echo "### Copied the pre build bootloader binary (bootloader.bin) into TARGET_TREES                                            ###"
    cp $PWD/../ert-contrib-middleware/target_libs/xtensa-esp32_freertos-xtensa-esp32-elf-4.4.1/build/lib/bootloader.bin $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ || echo "ERROR COULD NOT copy bootloader.bin"
else
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo "!!!  ERROR: COuld not find bootloader.bin in ert-contrib-middleware                              !!!"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    exit
fi

if test -f "$PWD/../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/build/partitions_singleapp.bin"
then
    cp $PWD/../ert-contrib-middleware/target_libs/xtensa-esp32_freertos-xtensa-esp32-elf-4.4.1/build/lib/partitions_singleapp.bin $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ || echoe "ERROR: Could not copy the partition table"
    echo "### Copied the pre built partition table binary (partition_singleapp.bin) into TARGET_TREES                              ###"
else
     echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
     echo "!!!  ERROR: COuld not find partition table (partitions_singleapp.bin)) in ert-contrib-middleware  !!!"
     echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
     exit
fi
echo "---------------------------------------------------------------------------------------------------------------------------"
echo "All DOne!"
