#!/bin/bash

#assumes the sources list is already correct (e.g. configure-ambifier-debian)
#echo -e "
# deb http://repo.ambifier.com/mirror jessie main
# deb http://ftp.uk.debian.org/debian jessie main non-free
#" > /etc/apt

apt-get update

#install usual wifi packages
apt-get -y install -t jessie-backports firmware-realtek &>> /var/log/ambifier/prompted_apt_update.log
apt-get -y install -t jessie-backports firmware-iwlwifi &>> /var/log/ambifier/prompted_apt_update.log
apt-get -y install -t jessie-backports firmware-ralink || apt-get -y install firmware-ralink &>> /var/log/ambifier/prompted_apt_update.log

#apt-get install wireless-tools

apt-get -y install iw &>> /var/log/ambifier/prompted_apt_update.log
apt-get -y install wpasupplicant &>> /var/log/ambifier/prompted_apt_update.log

echo -e "network={
    ssid=\"GoldMagnolia\"
    psk=\"wbqrhalk\"
}
" > /etc/wpa_supplicant/wpa_supplicant.conf
#Add the wlan0 interface config
echo -e "
auto wlan0
iface wlan0 inet dhcp
wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
" >> /etc/network/interfaces
fi
