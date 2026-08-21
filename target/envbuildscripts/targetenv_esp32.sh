#!/bin/bash

# creating the esp final binaries into the target/envbuildscripts/ directory along with all the others

####################################################################################################
# This file script will build the ert source using the docker file available or if not will use
# the host's make system.
####################################################################################################

# TODO 2026 have esp tools run from both the venv and then sometimes also directly. This is just a mess of course.
# We should check for a python venv and use this preferentially as teh esp tool path and if it's not there we can fall back 
# to the host pythoin environment (assuming this has been setu) and use the tools in the ert-build-support.   

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


#We run from make so assume repo route to find scripts.
source "./target/envbuildscripts/targetenv_esp32_sdk-packager_paths.sh"

# Check to see if we should use a docker image
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"
#todo check for a config.mk variable - either as an argument or set by mkae as an environment variable.

if [ ! -d /opt/python_env/bin ]; then
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo "The python virtual environment should be installed at /opt/python_env/bin/"
    echo "You should run make targetenv_esp33_docker to make sure you use a good environment"
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

PATH_TO_BINARIES=../TARGET_TREES/ehs_env-$TARGET/bin/

export EHS_ROOT=`pwd` # assuming we're in the ehs project root
pushd ${EHS_ROOT}/..
export REPOSITORY_ROOT=`pwd`
popd

# Note we can use the ates esptool for all versions of esp,buyt allowing for an override
if [ "${EHS_ESPTOOL_VERSION_OVERRIDE}" = "5.1.0" ] ; then
#These are probably not necessary for any IDF version, but is not supported in 4.4.1.
    EXTRA_ESPTOOL_ARGS="-min-rev-full 0 --max-rev-full 9999 "
    export TOOLCHAIN_VERSION="xtensa-esp32-elf-5.1.0"
else
    #default for anything else
    EXTRA_ESPTOOL_ARGS=
    export TOOLCHAIN_VERSION="xtensa-esp32-elf-4.4.1"
fi

# These tools haven't changed, so we can just use the 4.4.4 for all versions of IDF apparently

export TEMP_PWD=${PWD}
INX_HOST_ARCH=$(uname -m)
# Absolute, unlike make's exported TOOLCHAIN_PATH which is relative to
# ert-build-support/toolchains/. Named apart so the two are not confused; this
# script runs as its own process (Makefile: targetenv_esp32) so it does not
# clobber the make value for anything else.
ESP_TOOLCHAIN_DIR="${TEMP_PWD}/../ert-build-support/toolchains/${INX_HOST_ARCH}/${TOOLCHAIN_VERSION}"

export IDF_PYTHON_ENV_BASE="../../TARGET_TREES/esp32_venv/"
export IDF_ESPTOOL_BASE="${ESP_TOOLCHAIN_DIR}/${TOOLCHAIN_FLASHING_VERSION}/tools"

#Check if we are in docker sowe don't try to install IDF's grim python tools again
if [ -f /.dockerenv ]; then
    echo "Already running in Docker continuing"
    export PATH="/opt/python_env/bin:$PATH"
else
    # If for some reason we don't have a python venv environment we can try and run the host 
    # TODO see above note on this being an agregation of of mind-farts.

    # The following would be better picked from config.mk rather than hardwired, but would need to passed in for Docker builds.

    _PATH="${ESP_TOOLCHAIN_DIR}/bin"
    
    ## Create python virtual environment, install requirements and export it to PATH
    echo "TOOLCHAIN PATH IS $ESP_TOOLCHAIN_DIR"
    
    ## We don't need this of we are relying on the docker one, but will inclide in we are not (e.g. system for flashing should have this VM.)
    # todo-we should consider if we need a nother temporary directory for tools that are built per host machine (TARHGET_TREES is not very descriptive)
    # Note there is also a venv that might be built into /opt/python_env - this is typically only generated for workstaions that flash devices. 

    python3 -m venv ${IDF_PYTHON_ENV_BASE} > /dev/null
    export IDF_PYTHON_ENV_PATH="${IDF_PYTHON_ENV_BASE}/bin"

    #Set path to the python and toolchain.
    _PATH="${ESP_TOOLCHAIN_DIR}/bin"
    _PATH="${_PATH:+${_PATH}:}${IDF_ESPTOOL_BASE}/openocd-esp32/v0.11.0-esp32-20211220/openocd-esp32/bin"
    _PATH="${_PATH:+${_PATH}:}${IDF_PYTHON_ENV_PATH}"
    _PATH="${_PATH:+${_PATH}:}${IDF_ESPTOOL_BASE}/esptool_py/esptool"
    _PATH="${_PATH:+${_PATH}:}${IDF_ESPTOOL_BASE}/espcoredump"
    _PATH="${_PATH:+${_PATH}:}${IDF_ESPTOOL_BASE}/partition_table"
    _PATH="${_PATH:+${_PATH}:}${IDF_ESPTOOL_BASE}/app_update"
    _PATH="${_PATH:+${_PATH}:}${IDF_ESPTOOL_BASE}/additional_tools"
    export PATH="${_PATH}${PATH:+:${PATH}}"
    # Start the python virutal environment.
    sudo python3 -m venv /opt/python_env
    pip install pyserial > /dev/null
fi

#build and copy supportive binaries
#source ../ert-contrib-middleware/inx_build_scripts/source-scripts/inx-xbuilder-source-me-espidf.sh

#build_bootloader esp-idf -4.4.1 bootloader
#build_partition_table esp-idf -4.4.1 partition_table

# build .bin file from elf file

echo "---------------------------------------------------------------------------------------------------------------------------"
echo "### Building .bin file for esp32 from .elf file        "
echo " Location of the elf file:"
echo " ehs_esp32s3_freertos-xtensa-base.elf # or use this: $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.exe"
echo "---------------------------------------------------------------------------------------------------------------------------"                                              



# Building the bootable binary from ehs.exe elf file 

if python3  ${IDF_ESPTOOL_BASE}/esptool_py/esptool/esptool.py --chip esp32s3 elf2image ${EXTRA_ESPTOOL_ARGS} -ff 80m -fm qio -fs 8MB -o $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.bin $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.exe 
then
    echo "### Copied ehs.bin file generated in $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/"
else
    echo "!!! ERROR: COULD NOT GENERATE ehs.bin                                                                                !!!"
     exit 1
fi

echo "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
echo "Building the full flashable image from bootloader.bin, partition_table and any other partitions (file sytems and OTA etc)"
ls -l "$PWD/../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/build/lib/"
echo "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"

if test -f "$PWD/../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/build/lib/bootloader.bin"
then
    echo "### Copied the pre build bootloader binary (bootloader.bin) into TARGET_TREES ###"
    cp $PWD/../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/build/lib/bootloader.bin $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ || echo "ERROR COULD NOT copy bootloader.bin"
else
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo "!!!  ERROR: COuld not find bootloader.bin in $PWD/../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/build/lib/bootloader.bin         !!!"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    exit 1
fi

# previously know as? "$PWD/../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/build/partitions.bin"
if [ -n "${ESP32_FLASH_SIZE}" ]; then
    # Custom partition table: generate locally from ESP32_PART_* vars in config.mk.
    # Middleware bootloader is reused; CONFIG_ESPTOOLPY_FLASHSIZE is build-system-only
    # and the bootloader reads the actual flash size from the image header at runtime.
    CSV_OUT="$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/partitions.csv"
    PART_ENV="$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/partitions.env"
    PART_BIN="$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/partition-table.bin"
    PT_OFFSET="${ESP32_PARTITION_TABLE_OFFSET:-0x9000}"

    python3 ./target/envbuildscripts/esp32_gen_partitions.py "${CSV_OUT}" "${PART_ENV}" || exit 1
    python3 "${IDF_ESPTOOL_BASE}/partition_table/gen_esp32part.py" --offset "${PT_OFFSET}" "${CSV_OUT}" "${PART_BIN}" || exit 1
    # shellcheck disable=SC1090
    source "${PART_ENV}"
    echo "### Generated partition-table.bin locally from ESP32_PART_* vars (flash ${ESP32_GEN_FLASH_SIZE_HEX}) ###"
elif test -f "$PWD/../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/build/lib/partition-table.bin"
then
    cp "$PWD/../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/build/lib/partition-table.bin" "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/" || echo "ERROR: Could not copy the partition table"
    echo "### Copied the pre built partition table binary (partition-table.bin) into TARGET_TREES                              ###"
else
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo "!!!  ERROR: COuld not find partition table (partition-table.bin)) in $PWD/../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/build/lib/ !!!"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    exit 1
fi

# Convert the application folder to binary image
## Copy all the folders except bin/ to the data_partition/ folder
rm -rf $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/data_partition
mkdir -p $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/data_partition
for d in $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/*
do
if [[ -d "$d" ]]
then
if [[ "$d" != *"bin" ]] && [[ "$d" != *"data_partition" ]] && [[ "$d" != *"appdata" ]]
then
cp -r "$d" $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/data_partition/
fi
fi
done

# TODO See comments in head of this document, why we are using the venv scripts here
if [ -n "${ESP32_FLASH_SIZE}" ]; then
    # Custom-partitions path. Sizes and offsets come from the generated partitions.env.
    # data.bin is sized to ESP32_PART_STORAGE_SIZE so the image fits the partition.
    DATA_FS_SIZE=$(( ESP32_GEN_STORAGE_SIZE ))
    /opt/python_env/bin/littlefs-python create -v --block-size 4096 --fs-size "${DATA_FS_SIZE}" --name-max 64 --image $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/data.bin $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/data_partition/ || exit
else
    # DEFAULT PATH — taken whenever ESP32_FLASH_SIZE is unset, which is nearly
    # every esp32 target. Offsets and flash size are fixed here rather than
    # derived, so a board with a different layout needs the branch above:
    # set ESP32_FLASH_SIZE in its config.mk and let esp32_gen_partitions.py
    # generate the table.
    # /opt/python_env/bin/littlefs-python create -v --block-size 4096 --fs-size 1572864 --name-max 64 --image $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/data.bin $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/data_partition/ || exit
    /opt/python_env/bin/littlefs-python create -v --block-size 4096 --fs-size 512000 --name-max 64 --image $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/data.bin $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/data_partition/ || exit
fi

#Lets build the binary (todo2023 - we should do this in a packer target in the future?? )
echo "Converting the elf file to binary image.... (This is usually done by make targetenv_esp32s3)"
if [ -n "${ESP32_FLASH_SIZE}" ]; then
    # Custom-partitions path. Every offset is derived from the generated partition table,
    # so the merged image is self-consistent with partition-table.bin by construction.
    # --flash_size re-stamps the image header byte so the middleware-built bootloader
    # works across arbitrary flash sizes (the C bootloader reads the header at runtime
    # via update_flash_config()).
    MERGE_ARGS=(
        --chip esp32s3 merge_bin
        -o "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.img"
        --flash_mode dio
        --flash_size "${ESP32_FLASH_SIZE}"
        0x0 "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/bootloader.bin"
        "${ESP32_GEN_PARTITION_TABLE_OFFSET}" "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/partition-table.bin"
        "${ESP32_GEN_FACTORY_OFFSET}" "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.bin"
        "${ESP32_GEN_STORAGE_OFFSET}" "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/data.bin"
    )
    if [ "${ESP32_GEN_HAS_APPDATA}" = "yes" ] && [ -f "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/app_data.bin" ]; then
        MERGE_ARGS+=( "${ESP32_GEN_APPDATA_OFFSET}" "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/app_data.bin" )
    fi
    python3 ${IDF_ESPTOOL_BASE}/esptool_py/esptool/esptool.py "${MERGE_ARGS[@]}" || exit 1
else
    # DEFAULT PATH — taken whenever ESP32_FLASH_SIZE is unset, which is nearly
    # every esp32 target. Offsets and flash size are fixed here rather than
    # derived, so a board with a different layout needs the branch above:
    # set ESP32_FLASH_SIZE in its config.mk and let esp32_gen_partitions.py
    # generate the table.
    #python3 ../ert-contrib-middleware/contrib/esp-idf/CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION/components/esptool_py/esptool/esptool.py --chip esp32s3 merge_bin -o $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.img --flash_mode dio --flash_size 8MB 0x0 $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/bootloader.bin  0x9000 ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/partition-table.bin 0x10000 $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.bin 0x410000 $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/data.bin

    #python3 ${IDF_ESPTOOL_BASE}/esptool_py/esptool/esptool.py --chip esp32s3 merge_bin -o $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.img --flash_mode dio --flash_size 8MB 0x0 $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/bootloader.bin  0x9000 ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/partition-table.bin 0x20000 $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.bin 0x67c000 $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/data.bin 0x5ff000 $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/app_data.bin ||exit 1
    # app_data.bin is produced by `make targetenv_littlefs`, a separate goal, so
    # it is absent unless that has been run. esptool merge_bin fails outright if
    # given a path that does not exist, hence the guard.
    LEGACY_ARGS=(
        --chip esp32s3 merge_bin
        -o "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.img"
        --flash_mode dio
        --flash_size 8MB
        0x0      "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/bootloader.bin"
        0x9000   "../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/partition-table.bin"
        0x20000  "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.bin"
        0x77d000 "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/data.bin"
    )
    if [ -f "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/app_data.bin" ]; then
        LEGACY_ARGS+=( 0x700000 "$PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/app_data.bin" )
    else
        echo "Note: bin/app_data.bin absent - image built without an app-data partition."
        echo "      Run 'make targetenv_littlefs' first if this target needs one."
    fi
    python3 ${IDF_ESPTOOL_BASE}/esptool_py/esptool/esptool.py "${LEGACY_ARGS[@]}" || exit 1
fi

echo "---------------------------------------------------------------------------------------------------------------------------"
echo "All Done!"
