#!/bin/bash

###setup a factory default in case there are problems with parameters
#DEVMANURL="http://devman.inx-systems.com"
#if [ $# -gt 0 ] # This scripts may be called with other
#then
#	DEVMANURL=$1
#fi

#touch /root/ehs/devman/plugins/X/working.flag # set busy flag for sys download directory 1

#if [ ${#INXDEVICEID} -gt 1 ] # want at least two chars for a valid ID
#then
#  echo Device ID Set: INXDEVICEID = $INXDEVICEID
#else # The device ID has not been set at boot time so set to the MAC address
#  set -- `/root/ehs/download/getHWID-NETIP.sh`
#  INXDEVICEID=$1
#  INXDEVICEIP=$2
#fi

### create the patamters
#paramater="some data"

###send to devman
#wget -q -O - --post-data 'DeviceID='$INXDEVICEID'&'\
#'cgi_parameter='"$parameter" $DEVMANURL/cgi-bin/plugin_X.cgi

### check return value for commands and do things with it
# rm /root/ehs/devman/plugins/X/working.flag # set busy flag for sys download directory 1

