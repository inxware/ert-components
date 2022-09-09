#!/system/bin/sh

MAC_ADDRESS=$( cat /system/etc/ehs_supervisor/.id )
ifconfig eth0 hw ether "$MAC_ADDRESS"