#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/targetenv_make_apk_hacks/linux_android_arm_p64_a6_player-ambifier_hacks.sh $1

# overrides for the sandbox server
export DEVMAN_SERVER_NAME="adnoc"
export DEVMAN_UNAME="vagrant"
export DEVMAN_SERVER_DOMAIN="adnocdevman.inx-systems.net"
export DEVMAN_SERVER_URL="https://$DEVMAN_SERVER_DOMAIN"
export SSHPORT="22"

SetupTargetEnv_Certs(){
    # override with certificates
    echo "Setup adnoc server certificates"
    CERTS_DIR=${EHS_ROOT}/../DevmanSecurity/adnocdevman.inx-systems.net/certs
    cp -f ${CERTS_DIR}/devman-ca.crt ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-ca.crt || CancelFailed
    cp -f ${CERTS_DIR}/devman-client-crt-key.pem ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-client-crt-key.pem || CancelFailed
}

SetupTargetEnv_CopyEHSTools(){
    TOOLS_DIR=${EHS_ROOT}/../apps/
    GetApplicationRepo $TOOLS_DIR
    
    if [ -d "$TOOLS_DIR" ] && [ -d "$ANDROID_STUDIO_TOOLS_PATH" ]; then
        echo "Coping the ADNOC tools app to the project !!!!!!!!!!!!!!!!!!!!!!."
        cp -Rf ${EHS_ROOT}/../apps/customer-apps/RetailMusic/RetailAudio-v0.1.0/export/* ${ANDROID_STUDIO_TOOLS_PATH} || CancelFailed
    else
        echo "Failed to copy the tools app!"
        CancelFailed   
    fi
}