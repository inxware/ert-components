#!/bin/bash
######################################################################
# Copyright : inx limited, Sheffield, UK, 2008
# License   : wwww.inx-systems.com/inxware_license.html
# Type      : Target deployed unencrypted code
# Support   : support.inx-systems.com
# Description:
# This script returns the MAC address of the device
# This should be called by the OS once at init time  
######################################################################

#try to get MAC address of eth0
  local_ifconfigZ=`/sbin/ip address show dev enp2s0`
  if [ -z "$local_ifconfigZ" ]; then
    local_ifconfigZ=`/sbin/ip address show dev eth0`
  if [ -z "$local_ifconfigZ" ]; then
    local_ifconfigZ=`/sbin/ip address show dev enp4s0`
  if [ -z "$local_ifconfigZ" ]; then
    local_ifconfigZ=`/sbin/ip address show dev enp3s0`
  if [ -z "$local_ifconfigZ" ]; then
    local_ifconfigZ=`/sbin/ip address show dev enp1s0`
  if [ -z "$local_ifconfigZ" ]; then
    local_ifconfigZ=`/sbin/ip address show dev enp0s0`
  if [ -z "$local_ifconfigZ" ]; then
    local_ifconfigZ=`/sbin/ip address show dev eno0`
  if [ -z "$local_ifconfigZ" ]; then
    local_ifconfigZ=`/sbin/ip address show dev eno1`
  if [ -z "$local_ifconfigZ" ]; then
    local_ifconfigZ=`/sbin/ip address show dev wlan0`
  fi
  fi
  fi
  fi
  fi
  fi
  fi
  fi
  if [ -n "$local_ifconfigZ" ]; then

  local_INXDEVICEIP=`echo "$local_ifconfigZ" | grep -Po 'inet \K[\d.]+'`
  local_INXDEVICEID=`echo "$local_ifconfigZ" | grep -Po 'link/ether \K[\S]+' | tr [:lower:] [:upper:] `
  #@todo should check this regular expression - but expr was not working in ubuntu when run from cron for some reason. perhaps use sed
  if [ ${#local_INXDEVICEID} -gt 5 ] 
  then
    echo -n "$local_INXDEVICEID\n"
  else
    echo -n "IDX\n" 
# don't return anything
  fi

  if [ ${#local_INXDEVICEIP} -gt 6 ]
  then
    echo "$local_INXDEVICEIP"
  else
    echo "IPX" 
# don't return anything
  fi
  else
  	  echo -n "IFX" 
  fi


