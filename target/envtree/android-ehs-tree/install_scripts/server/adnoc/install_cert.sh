#!/bin/bash

source $ANDROID_VERSION_DIR/install_utils.sh

echo "Installing adnoc certificates"

# obtain certificates 
TEMP_CERT_DIR=$INSTALL_DIR/temp_cert
if [ -d "$TEMP_CERT_DIR" ]; then
    rm -rf $TEMP_CERT_DIR
fi
mkdir "$INSTALL_DIR/temp_cert" || exit 1

# NOTE: The following ignores the peviously set env variabls for certificate paths etc.
cp "$EHS_ROOT/../DevmanSecurity/adnocdevman.inx-systems.net/certs/devman-ca.crt" "$TEMP_CERT_DIR/adnoc_devman-ca.crt" || exit 1
cp "$EHS_ROOT/../DevmanSecurity/adnocdevman.inx-systems.net/certs/devman-client-crt-key.pem" "$TEMP_CERT_DIR/adnoc_devman-client-crt-key.pem" || exit 1

echo "Converting adnoc .pem to .p12 - Android keys need .p12 format"
openssl pkcs12 -export -out "$TEMP_CERT_DIR/adnoc_devman-client-crt-key.p12" -in "$TEMP_CERT_DIR/adnoc_devman-client-crt-key.pem" -passin pass: -passout pass: || exit 1

ls "$TEMP_CERT_DIR/"

echo "Adding certificates to the downloader"
# copy certificates to the device using ADB so that can be installed by the downloader
mktemp # can't we just use the adb command, or comment what these do?
cptemp "$TEMP_CERT_DIR/" # and this.
CERT_DIR=$( temp )"/temp_cert"
## use adb to shell to start downloader
startdownloader
sleep 6 # give some time for the download to start

CA_CERT_ADNOC_DIR="$CERT_DIR/adnoc_devman-ca.crt"
echo "Installing ca adnoc to certificate ($CA_CERT_ADNOC_DIR)"
downloader_cert "https://adnocdevman.inx-systems.net" "ca_cert" "$CA_CERT_ADNOC_DIR"
sleep 4

CLIENT_CERT_ADNOC_DIR="$CERT_DIR/adnoc_devman-client-crt-key.p12"
echo "Installing clinet adnoc certificate ($CLIENT_CERT_ADNOC_DIR)"
downloader_cert "https://adnocdevman.inx-systems.net" "client_cert" "$CLIENT_CERT_ADNOC_DIR"
sleep 4

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
