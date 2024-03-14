#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/installers/android-adb/install_scripts/install_utils.sh

echo "Installing tellisign certificates"

# obtain certificates

URL="devman.inx-systems.com"

TEMP_CERT_DIR=$INSTALL_DIR/temp_cert

if [ -d "$TEMP_CERT_DIR" ]; then
    rm -rf $TEMP_CERT_DIR
fi

mkdir $TEMP_CERT_DIR || exit 1

# WARNING - openssl seems to create different .p12 keys on Ubuntu 20.04 and 22.04 (DevmanSecurity is using key created in Ubuntu 20.04)
#echo "Converting ambifier .pem to .p12"
#openssl pkcs12 -export -out "$TEMP_CERT_DIR/devman-client-crt-key.p12" -in "$TEMP_CERT_DIR/devman-client-crt-key.pem" -passin pass: -passout pass: || exit 1

#todo2023 - all this should be copying from the staging directory as we ahve already done the magic to find them and don't want to do it again here/
cp "$EHS_ROOT/../DevmanSecurity/$URL/certs/client/devman-ca.crt" "$TEMP_CERT_DIR/devman-ca.crt" || exit 1
cp "$EHS_ROOT/../DevmanSecurity/$URL/certs/client/devman-client-crt-key.pem" "$TEMP_CERT_DIR/devman-client-crt-key.pem" || exit 1
cp "$EHS_ROOT/../DevmanSecurity/$URL/certs/client/devman-client-crt-key.p12" "$TEMP_CERT_DIR/devman-client-crt-key.p12" || exit 1


ls "$TEMP_CERT_DIR/"

echo "Adding certificates to the downloader"
mktemp
cptemp "$TEMP_CERT_DIR/"
CERT_DIR=$( temp )"/temp_cert"
startdownloader
sleep 6 # give some time for the download to start
CA_CERT_DIR="$CERT_DIR/devman-ca.crt"
echo "Installing ca certificate ($CA_CERT_DIR)"
downloader_cert "https://$URL" "ca_cert" "$CA_CERT_DIR"
sleep 4
CLIENT_CERT_DIR="$CERT_DIR/devman-client-crt-key.p12"
echo "Installing clinet certificate ($CLIENT_CERT_DIR)"
downloader_cert "https://$URL" "client_cert" "$CLIENT_CERT_DIR"
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
