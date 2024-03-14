#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/targetenv_make_apk_hacks/linux_android_arm_p64_h6_player-sandbox_hacks.sh "ci"

# overrides for the sandbox server
export SSHPORT=8822
export DEVMAN_UNAME="root"
export DEVMAN_SERVER_NAME="testing"
if [ "${DEVMAN_SERVER_DOMAIN}" = "" ]; then
    export DEVMAN_SERVER_DOMAIN="devman.inx-systems.com"
else
    export DEVMAN_SERVER_DOMAIN
fi

if [ "${DEVMAN_SERVER_PROTOCOL}" = "" ]; then
    export DEVMAN_SERVER_PROTOCOL="https"
else
    export DEVMAN_SERVER_PROTOCOL
fi

export DEVMAN_SERVER_URL="${DEVMAN_SERVER_PROTOCOL}://${DEVMAN_SERVER_DOMAIN}"

SetupTargetEnv_Certs(){
    # override with certificates
    echo "We need a CA root bundle so copy that in"
    CERTS_DIR=${EHS_ROOT}/../DevmanSecurity/$DEVMAN_SERVER_DOMAIN
    cp -f ${CERTS_DIR}/devman-ca.crt ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-ca.crt || CancelFailed
#   we're not supposed to need this for this server cp -f ${CERTS_DIR}/devman-client-crt-key.pem ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-client-crt-key.pem || CancelFailed
}