#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/targetenv_make_apk_hacks/linux_android_arm_p64_a6_ambifier_hacks.sh $1

if [ "${DEVMAN_SERVER_NAME}" = "" ]; then
    export DEVMAN_SERVER_NAME="sandbox"
else 
    export DEVMAN_SERVER_NAME
fi

if [ "${DEVMAN_SERVER_DOMAIN}" = "" ]; then
    export DEVMAN_SERVER_DOMAIN="devman-sandbox.inx-systems.net"
else
    export DEVMAN_SERVER_DOMAIN
fi

if [ "${DEVMAN_SERVER_PROTOCOL}" = "" ]; then
    export DEVMAN_SERVER_PROTOCOL="http"
else
    export DEVMAN_SERVER_PROTOCOL
fi

export DEVMAN_SERVER_URL="${DEVMAN_SERVER_PROTOCOL}://${DEVMAN_SERVER_DOMAIN}"

SetupTargetEnv_Certs(){
    # override with certificates
    echo "ambifier-sandbox server - no certificates required"
}