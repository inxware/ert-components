#!/bin/bash

set -e

#todo2023 - this needs to be merged into a single script using canonical names 
source ${EHS_ROOT}/target/envbuildscripts/installers/android-adb/install_scripts/install_utils.sh

echo "Installing $DEVMAN_SERVER_DOMAIN certificates"

# obtain certificates
# Noooo - not again!
URL=$DEVMAN_SERVER_DOMAIN

TEMP_CERT_DIR=$INSTALL_DIR/temp_cert

if [ -d "$TEMP_CERT_DIR" ]; then
    echo "Temporary certificate file exists - deleting"
    rm -rf $TEMP_CERT_DIR
fi

mkdir $TEMP_CERT_DIR || exit 1

# This mess spawned out of find a way to provide certificates to the supervisor without including them in the APK.
# Don't this it wil only work as soon as you look away.. Just store them in the certiicate repo like everything else
# WARNING - openssl seems to create different .p12 keys on Ubuntu 20.04 and 22.04 (DevmanSecurity is using key created in Ubuntu 20.04)
#echo "Converting ambifier .pem to .p12"
#openssl pkcs12 -export -out "$TEMP_CERT_DIR/devman-client-crt-key.p12" -in "$TEMP_CERT_DIR/devman-client-crt-key.pem" -passin pass: -passout pass: || exit 1
# and of course there is no point applying certificates at this stage.... as they are still just visible files for the supervisor to use
# Just use the ones that have been staged - all this logic has already been done by make targetenv
cp "$EHS_ROOT/../TARGET_TREES/ehs-env-${TARGET}/devman/core/certs/devman-ca.crt" "$TEMP_CERT_DIR/devman-ca.crt" || exit 1
cp "$EHS_ROOT/../TARGET_TREES/ehs-env-${TARGET}/devman/core/certs/devman-client-crt-key.pem" "$TEMP_CERT_DIR/devman-client-crt-key.pem" || exit 1
cp "$EHS_ROOT/../TARGET_TREES/ehs-env-${TARGET}/devman/core/certs/devman-client-crt-key.p12" "$TEMP_CERT_DIR/devman-client-crt-key.p12" || exit 1

ls "$TEMP_CERT_DIR/"

echo "Adding certificates to the downloader"
# copy certificates to the device using ADB so that can be installed by the downloader
mktemp # can't we just use the adb command, or comment what these do?
cptemp "$TEMP_CERT_DIR/" # and this.
CERT_DIR=$( temp )"/temp_cert"
## use adb to shell to start downloader
startdownloader
sleep 10 # give some time for the download to start
CA_CERT_DIR="$CERT_DIR/devman-ca.crt"
echo "Installing ca certificate ($CA_CERT_DIR)"
downloader_cert "https://$URL" "ca_cert" "$CA_CERT_DIR"
sleep 10
CLIENT_CERT_DIR="$CERT_DIR/devman-client-crt-key.p12"
echo "Installing client certificate ($CLIENT_CERT_DIR)"
downloader_cert "https://$URL" "client_cert" "$CLIENT_CERT_DIR"
sleep 10

CERT_COUNT=$( count_files '/data/data/com.utils.downloader/files/' )
echo "Installed $CERT_COUNT Certs (expecting 2):"
list_files '/data/data/com.utils.downloader/files/'
rmtemp
rm -r "$TEMP_CERT_DIR"
if [ "$CERT_COUNT" == "2" ]; then
    echo "Successfully installed downloader certificates."
else
    echo "Failed to install downloader certificates."
    exit 1
fi
