#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/targetenv_make_apk_hacks/linux_android_arm_p64_a6_player-ambifier_hacks.sh $1

# overrides for the sandbox server
export DEVMAN_SERVER_NAME="sandbox"
export DEVMAN_SERVER_DOMAIN="devman-sandbox.inx-systems.net"
export DEVMAN_SERVER_URL="http://$DEVMAN_SERVER_DOMAIN"

SetupTargetEnv_Certs(){
    # override with certificates
    echo "player-sandbox server - no certificates required"
}
