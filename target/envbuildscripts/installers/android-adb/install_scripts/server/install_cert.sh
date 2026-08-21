#!/bin/bash
###########in########################################################################
## This scriot is for the downloader's scripts only. 
## It does not change the certificates used by eRT (which may be different formats)
######################################################################################
set -e

#todo2023 - this needs to be merged into a single script using canonical names 
echo "###1"
source ${EHS_ROOT}/target/envbuildscripts/installers/android-adb/install_scripts/install_utils.sh

function InstallSupervisorCerts() {

SUPERVISOR_DEVMAN_SERVER_DOMAIN=$1
DEVMAN_SERVER_DOMAIN_DEFAULT=$2


DEVMAN_CERT_DIR=certs
if [ "$DEVMAN_SERVER_DOMAIN_DEFAULT" != "default" ]; then
    DEVMAN_CERT_DIR="certs/$SUPERVISOR_DEVMAN_SERVER_DOMAIN"
fi

echo "Installing $SUPERVISOR_DEVMAN_SERVER_DOMAIN certificates"
echo "from from ../TARGET_TREES/ehs_env-${TARGET}/devman/core/${DEVMAN_CERT_DIR}"

# obtain certificates
URL=$SUPERVISOR_DEVMAN_SERVER_DOMAIN

TEMP_CERT_DIR=$INSTALL_DIR/temp_cert

if [ -d "$TEMP_CERT_DIR" ]; then
    echo "Temporary certificate file exists - deleting"
    rm -rf $TEMP_CERT_DIR
fi

#We probably never wanted this as we already have a staging directory for certs
mkdir $TEMP_CERT_DIR || exit 1

# WARNING - openssl seems to create different .p12 keys on Ubuntu 20.04 and 22.04 (DevmanSecurity is using key created in Ubuntu 20.04)
# The downloader may want a server certificate so we check for the server only case
if [ -f $EHS_ROOT/../TARGET_TREES/ehs_env-${TARGET}/devman/core/${DEVMAN_CERT_DIR}/devman-only-ca.crt ]; then
   # Not sure there is ever a file called devman-only-ar.crt anymore. This should probably b more explicit.
   cp "$EHS_ROOT/../TARGET_TREES/ehs_env-${TARGET}/devman/core/${DEVMAN_CERT_DIR}/devman-only-ca.crt" "$TEMP_CERT_DIR/devman-ca.crt" || exit 1
else
   cp "$EHS_ROOT/../TARGET_TREES/ehs_env-${TARGET}/devman/core/${DEVMAN_CERT_DIR}/devman-ca.crt" "$TEMP_CERT_DIR/devman-ca.crt" || exit 1
fi

# Copy the 
cp "$EHS_ROOT/../TARGET_TREES/ehs_env-${TARGET}/devman/core/${DEVMAN_CERT_DIR}/devman-client-crt-key.pem" "$TEMP_CERT_DIR/devman-client-crt-key.pem" || exit 1
cp "$EHS_ROOT/../TARGET_TREES/ehs_env-${TARGET}/devman/core/${DEVMAN_CERT_DIR}/devman-client-crt-key.p12" "$TEMP_CERT_DIR/devman-client-crt-key.p12" || exit 1

ls "$TEMP_CERT_DIR/"

echo "Adding certificates to the downloader"
# change temporary directory to install certs
export TEMP_DIR="/data/data/com.utils.downloader/cache/.ehs_temp"
# copy certificates to the device using ADB so that can be installed by the downloader
rmtemp
mktemp # can't we just use the adb command, or comment what these do?
cptemp "$TEMP_CERT_DIR/" # and this.
CERT_DIR=$( temp )"/temp_cert"
## use adb to shell to start downloader
startdownloader
sleep 10 # give some time for the download to start
# This is broadcasting the certificate to the downloader to install
CA_CERT_DIR="$CERT_DIR/devman-ca.crt"
echo "Installing ca certificate ($CA_CERT_DIR)"
downloader_cert "https://$URL" "ca_cert" "$CA_CERT_DIR"
sleep 10
CLIENT_CERT_DIR="$CERT_DIR/devman-client-crt-key.p12"
echo "Installing client certificate ($CLIENT_CERT_DIR)"
downloader_cert "https://$URL" "client_cert" "$CLIENT_CERT_DIR"
sleep 10

rmtemp
rm -r "$TEMP_CERT_DIR"

} # InstallSupervisorCerts()

# check the default certificate domain
if [ "$DEVMAN_SERVER_DOMAIN" != "" ]; then

# @TODO - adding to traget config ?
EXPECTED_CERT_COUNT="2"

echo "Setting DEVMAN_SERVER_DOMAIN (default)"
InstallSupervisorCerts ${DEVMAN_SERVER_DOMAIN} "default"

# check the second certificate domain
if [ "$DEVMAN_SERVER_DOMAIN_1" != "" ]; then
echo "Setting DEVMAN_SERVER_DOMAIN_1"
InstallSupervisorCerts ${DEVMAN_SERVER_DOMAIN_1} ""
EXPECTED_CERT_COUNT="4"
else
echo "Skip DEVMAN_SERVER_DOMAIN_1 ..."
fi

# check the third certificate domain
if [ "$DEVMAN_SERVER_DOMAIN_2" != "" ]; then
echo "Setting DEVMAN_SERVER_DOMAIN_2"
InstallSupervisorCerts ${DEVMAN_SERVER_DOMAIN_2} ""
EXPECTED_CERT_COUNT="6"
else
echo "Skip DEVMAN_SERVER_DOMAIN_2 ..."
fi

# We now count the stupid files Java creates for the certificate. Though sometimes they are 0-byte and it has failed anyway
# Perhaps we need to check the logs for 
CERT_COUNT=$( count_files '/data/data/com.utils.downloader/files/' )
echo "Installed $CERT_COUNT Certs (expecting $EXPECTED_CERT_COUNT):"
list_files '/data/data/com.utils.downloader/files/'

if [ "$CERT_COUNT" == "$EXPECTED_CERT_COUNT" ]; then
    echo "Successfully installed downloader certificates."
else
    echo "Failed to install downloader certificates."
    exit 1
fi

else 
	echo "Warning!!!! DEVMAN_SERVER_DOMAIN is not set."
    exit 1
fi