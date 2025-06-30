#!/bin/bash
export PATH=$PATH:/usr/sbin
#assumes the sources list is already correct (e.g. configure-ambifier-debian)
#echo -e "
#
#deb http://httpredir.debian.org/debian jessie-backports main contrib non-free
#deb-src http://httpredir.debian.org/debian jessie-backports main contrib non-free
#
#" >> /etc/apt/sources.list
count = 100
while ! ping -c 1 -W 1 httpredir.debian.org; do
    echo "Waiting for  httpredir.debian.org" >> /var/log/ambifier/prompted_apt_update.log
    sleep 10
    count = $count -1
    if [ $count -lt 1 ]; then
    	break
    fi
done


apt-get update &>> /var/log/ambifier/prompted_apt_update.log

apt-get -y install -t jessie-backports linux-image-686-pae linux-base &>> /var/log/ambifier/prompted_apt_update.log



