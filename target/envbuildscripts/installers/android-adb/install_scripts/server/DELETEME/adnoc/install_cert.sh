#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/installers/android-adb/install_scripts/install_utils.sh

echo "Installing adnoc certificates"

# obtain certificates 
TEMP_CERT_DIR=$INSTALL_DIR/temp_cert
if [ -d "$TEMP_CERT_DIR" ]; then
    rm -rf $TEMP_CERT_DIR
fi
mkdir "$INSTALL_DIR/temp_cert" || exit 1

# WARNING - openssl seems to create different .p12 keys on Ubuntu 20.04 and 22.04 (DevmanSecurity is using key created in Ubuntu 20.04)
#echo "Converting ambifier .pem to .p12"
#openssl pkcs12 -export -out "$TEMP_CERT_DIR/devman-client-crt-key.p12" -in "$TEMP_CERT_DIR/devman-client-crt-key.pem" -passin pass: -passout pass: || exit 1

# NOTE: The following ignores the peviously set env variabls for certificate paths etc.
cp "$EHS_ROOT/../DevmanSecurity/adnocdevman.inx-systems.net/certs/devman-ca.crt" "$TEMP_CERT_DIR/adnoc_devman-ca.crt" || exit 1
cp "$EHS_ROOT/../DevmanSecurity/adnocdevman.inx-systems.net/certs/devman-client-crt-key.pem" "$TEMP_CERT_DIR/adnoc_devman-client-crt-key.pem" || exit 1
cp "$EHS_ROOT/../DevmanSecurity/adnocdevman.inx-systems.net/certs/adnoc_devman-client-crt-key.p12" "$TEMP_CERT_DIR/adnoc_devman-client-crt-key.p12" || exit 1


ls "$TEMP_CERT_DIR/"

echo "Adding certificates to the downloader"
# copy certificates to the device using ADB so that can be installed by the downloader
mktemp # can't we just use the adb command, or comment what these do?
cptemp "$TEMP_CERT_DIR/" # and this.
CERT_DIR=$( temp )"/temp_cert"
## use adb to shell to start downloader
startdownloader
sleep 10 # give some time for the download to start

CA_CERT_ADNOC_DIR="$CERT_DIR/adnoc_devman-ca.crt"
echo "Installing ca adnoc to certificate ($CA_CERT_ADNOC_DIR)"
downloader_cert "https://adnocdevman.inx-systems.net" "ca_cert" "$CA_CERT_ADNOC_DIR"
sleep 10

CLIENT_CERT_ADNOC_DIR="$CERT_DIR/adnoc_devman-client-crt-key.p12"
echo "Installing clinet adnoc certificate ($CLIENT_CERT_ADNOC_DIR)"
downloader_cert "https://adnocdevman.inx-systems.net" "client_cert" "$CLIENT_CERT_ADNOC_DIR"
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
