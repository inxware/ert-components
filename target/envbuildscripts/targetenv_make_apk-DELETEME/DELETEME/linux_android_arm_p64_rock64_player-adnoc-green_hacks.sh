#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/targetenv_make_apk_hacks/linux_android_arm_p64_h6_player-ambifier_hacks.sh $1

#todo the parameters in here should be moved to config.mk for the platfrom and and the hack code should be generic for all android. 

# overrides for the sandbox server
export DEVMAN_UNAME="vagrant"
export SSHPORT="22"

# THe following id for some installer magic, not the config itself.
if [ "${DEVMAN_SERVER_NAME}" = "" ]; then
    export DEVMAN_SERVER_NAME="adnoc-green"
else 
    export DEVMAN_SERVER_NAME
fi

#todo2023 - as far Ican tell DEVMAN_SERVER_NAME is not used anyhere - lets remove it so it's less confusing.
#             .... it seems to somehoe get used in adb-install and seems to a new magical name for the server we are taretting in ..../install_scripts/server
#             ..... lets just use the dmain name (i.e. as used in the Devman Security and unstring this from the code.) 
if [ "${DEVMAN_SERVER_DOMAIN}" = "" ]; then
    export DEVMAN_SERVER_DOMAIN="addbgm1.adnoc-dist.co.ae"
else
    export DEVMAN_SERVER_DOMAIN
fi

export DEVMAN_SERVER_URL="https://$DEVMAN_SERVER_DOMAIN"

# Copy certificates to staging directory's android project tree.
SetupTargetEnv_Certs(){
    # override with certificates
    #use the same certs as the previous certs.
    #todo we should just set the certs from the conanical paths in the DevmanSecurity i.e. DevmanSecurity/<DEVMAN URL>/certs/*
    # rather than all this conoluted stuff. Ther ./certs folder could be split into different OS's (optionally) if the formats vary between different devices 
    # connecting to the same server. 
    # something to refactor when we get the regression system working again!
    
    CERTS_DIR=${EHS_ROOT}/../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs
    echo "Setup adnoc server certificates ($CERTS_DIR)"
    test -f ${CERTS_DIR}/devman-ca.crt || CancelFailed
    test -f ${CERTS_DIR}/devman-client-crt-key.pem || CancelFailed
    echo "Using single server certificate for this device (not full CA bundle)"
    cp -f ${CERTS_DIR}/devman-ca.crt ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-ca.crt || CancelFailed
    cp -f ${CERTS_DIR}/devman-client-crt-key.pem ${ANDROID_STUDIO_DEVMAN_PATH}/certs/devman-client-crt-key.pem || CancelFailed
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