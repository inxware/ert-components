#!/bin/bash

set -e

source $ANDROID_VERSION_DIR/install_utils.sh

echo "Installing ambifier certificates"

# obtain certificates 

TEMP_CERT_DIR=$INSTALL_DIR/temp_cert

if [ -d "$TEMP_CERT_DIR" ]; then
    rm -rf $TEMP_CERT_DIR
fi

mkdir $TEMP_CERT_DIR || exit 1

cp "$EHS_ROOT/../DevmanSecurity/devman.ambifier.com/devman-ca.crt" "$TEMP_CERT_DIR/devman-ca.crt" || exit 1
cp "$EHS_ROOT/../DevmanSecurity/devman.ambifier.com/devman-client-crt-key.pem" "$TEMP_CERT_DIR/devman-client-crt-key.pem" || exit 1

echo "Converting ambifier .pem to .p12"

openssl pkcs12 -export -out "$TEMP_CERT_DIR/devman-client-crt-key.p12" -in "$TEMP_CERT_DIR/devman-client-crt-key.pem" -passin pass: -passout pass: || exit 1

ls "$TEMP_CERT_DIR/"

echo "Adding certificates to the downloader"
mktemp
cptemp "$TEMP_CERT_DIR/"
CERT_DIR=$( temp )"/temp_cert"
startdownloader
sleep 6 # give some time for the download to start
CA_CERT_DIR="$CERT_DIR/devman-ca.crt"
echo "Installing ca certificate ($CA_CERT_DIR)"
downloader_cert "https://devman.ambifier.com" "ca_cert" "$CA_CERT_DIR"
sleep 2
CLIENT_CERT_DIR="$CERT_DIR/devman-client-crt-key.p12"
echo "Installing clinet certificate ($CLIENT_CERT_DIR)"
downloader_cert "https://devman.ambifier.com" "client_cert" "$CLIENT_CERT_DIR"
sleep 2
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