#!/bin/bash

set -e

export SPECIFIC_TARGET=$1

EHS_OTA_VERSION=$( head -c -1 ./Releases/version_strings | tr '\n' '.' )

# TODO - we may want this to be set in the config
EHS_OTA_PACKAGE_SRC=../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/ehs.bin
EHS_OTA_PACKAGE_DST=/home/inx-devman/bin/dldata-scripts/firmwares/inx_${EHS_PRODUCT_NAME}_${SPECIFIC_TARGET}_${EHS_OTA_VERSION}_ota.bin

ERROR_FORMAT="!!! ERROR !!! %s is not specified !\n"
test -z "$EHS_OTA_PACKAGE_SRC" && printf "$ERROR_FORMAT" "EHS_OTA_PACKAGE_SRC" && exit 1
test -z "$EHS_OTA_PACKAGE_DST" && printf "$ERROR_FORMAT" "EHS_OTA_PACKAGE_DST" && exit 1
test -z "$DEVMAN_SERVER_DOMAIN" && printf "$ERROR_FORMAT" "DEVMAN_SERVER_DOMAIN" && exit 1
test -z "$DEVMAN_UNAME" && printf "$ERROR_FORMAT" "DEVMAN_UNAME" && exit 1
test -z "$SSHPORT" && printf "$ERROR_FORMAT" "SSHPORT" && exit 1

DEVMAN_SERVER_DST="$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN"

if [ -n "$SERVER_OVERRIDE" ]; then
    echo "Devman server destination override $DEVMAN_SERVER_DST ====> $SERVER_OVERRIDE"
    DEVMAN_SERVER_DST="$SERVER_OVERRIDE"
fi

if [ -f "$EHS_OTA_PACKAGE_SRC" ]; then
    echo "####################################################################"
    echo "################### Uploading OTA package ##########################"
    echo "$EHS_OTA_PACKAGE_SRC"
    echo "########################## TO ######################################"
    echo "$DEVMAN_SERVER_DST:$EHS_OTA_PACKAGE_DST"
    echo "####################################################################"
else
    echo "!!! ERROR !!! EHS_OTA_PACKAGE_SRC doesn't exist ($EHS_OTA_PACKAGE_SRC)"
    exit 1
fi

# upload OTA file to the server
echo "Uploading OTA file to the server ..."
scp -P ${SSHPORT} ${EHS_OTA_PACKAGE_SRC} "$DEVMAN_SERVER_DST:$EHS_OTA_PACKAGE_DST"
echo "Setting-up ownership for the OTA file ..."
# make sure the OTA file has correct ownership
ssh -p ${SSHPORT} "$DEVMAN_SERVER_DST" 'chown www-data:www-data "'${EHS_OTA_PACKAGE_DST}'"'
