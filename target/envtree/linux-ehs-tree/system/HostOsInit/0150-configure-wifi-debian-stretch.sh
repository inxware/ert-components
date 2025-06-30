#!/bin/bash
export PATH=$PATH:/usr/sbin

#install package for the wifi, this wont work on its own though
apt-get update
apt-get -y install iw
apt-get -y install wpasupplicant
apt-get -y install firmware-iwlwifi

