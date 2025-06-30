#!/bin/bash
export PATH=$PATH:/usr/sbin
apt-get -y install firmware-realtek
apt-get -y install iw &>> /var/log/ambifier/prompted_apt_update.log

