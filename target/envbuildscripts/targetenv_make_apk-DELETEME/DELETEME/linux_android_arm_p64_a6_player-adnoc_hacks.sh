#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/targetenv_make_apk_hacks/linux_android_arm_p64_a6_player-ambifier_hacks.sh $1

#todo the parameters in here should be moved to config.mk for the platfrom and and the hack code should be generic for all android. 

# overrides for the sandbox server
if [ "${DEVMAN_SERVER_NAME}" = "" ]; then
    export DEVMAN_SERVER_NAME="adnoc"
else 
    export DEVMAN_SERVER_NAME
fi

export DEVMAN_UNAME="vagrant"
if [ "${DEVMAN_SERVER_DOMAIN}" = "" ]; then
    export DEVMAN_SERVER_DOMAIN="adnocdevman.inx-systems.net"
else
    export DEVMAN_SERVER_DOMAIN
fi

if [ "${DEVMAN_SERVER_PROTOCOL}" = "" ]; then
    export DEVMAN_SERVER_PROTOCOL="https"
else
    export DEVMAN_SERVER_PROTOCOL
fi

export DEVMAN_SERVER_URL="${DEVMAN_SERVER_PROTOCOL}://${DEVMAN_SERVER_DOMAIN}"
export SSHPORT="22"

# Copy certificates to staging directory's android project tree.
SetupTargetEnv_Certs(){
    # override with certificates
    CERTS_DIR=${EHS_ROOT}/../DevmanSecurity/adnocdevman.inx-systems.net/certs
    echo "Setup adnoc server certificates ($CERTS_DIR)"
    cp -f ${CERTS_DIR}/devman-ca.crt ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-ca.crt || CancelFailed
    cp -f ${CERTS_DIR}/devman-client-crt-key.pem ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-client-crt-key.pem || CancelFailed
    echo "Setup adnoc server certificates - OK"
}

#Copy audio player app from the app repo to the staging directory's android project tree.
SetupTargetEnv_CopyEHSTools(){
    echo "INFO: copying app from RetailMusic/RetailAudio-v0.1.0/export/"
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