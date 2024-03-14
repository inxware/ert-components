#!/bin/bash

######################################################################
# Copyright : inx limited, Sheffield, UK, 2008
# License   : wwww.inx-systems.com/inxware_license.html
# Type      : Target deployed unencrypted code
# Support   : support.inx-systems.com
# Description:
# This script is sto post device logs to a devman server. 
# Assumptions: The usual search path is set up.
######################################################################


# Devman root dir 
if [ -z "${INXWAREDEVMANROOT}" ]
then
	COMLINE="${0}"
	COMLINECHAR1=${COMLINE:(0):(1)}
	if [ "${COMLINECHAR1}" = "/" ]; then
		INXWAREDEVMANROOT="`dirname $0`/"
	else
		INXWAREDEVMANROOT="${PWD}/`dirname $0`/"
	fi
fi

##setup relative path names from the devman core directory we expect to be in
DEVMANCOREDIR="../core/"

DEVMANPLUGINDIR="../plugins/"
DEVMANPUBLICDATA="../pubdata/"
VARIABLE_CACHE="var/"
DEVICE_CONFIG="config/"
CERTIFICATES="certs/"

if [ -n "${DEVMANDOWNLOADDIR}" ]; then
	DEVMANCOREDLDATA="${DEVMANDOWNLOADDIR}"
else
	DEVMANCOREDLDATA="../../core/download"
fi

EHSSYSTEMDATADIR=${INXWAREDEVMANROOT}"/../../sysdata/"

#Get the devman core - hardwire fall-back
DEVMANURL="http://devman.inx-systems.com"
if [ -e "${INXWAREDEVMANROOT}${DEVMANCOREDIR}${DEVICE_CONFIG}DEVMANURL.000" ]; then # This scripts may be called with other devman server URLs
	DEVMANURL_TRY=`cat "${INXWAREDEVMANROOT}${DEVMANCOREDIR}${DEVICE_CONFIG}DEVMANURL.000"`
   if [ ${#DEVMANURL_TRY} -gt 8 ]; then
 #  	echo TRY=${DEVMANURL_TRY}
	DEVMANURL=$DEVMANURL_TRY
   else
	DEVMANURL="http://devman.inx-systems.com"
   fi	
fi
#echo ${DEVMANURL}
# TDOD: trim the URL so it's just a single URL

# touch ${INXWAREDEVMANROOT}${DEVMANCOREDIR}working.flag # Leave mutex to the caller

if [ ${#INXDEVICEID} -gt 5 ] # want at least five chars for a valid ID
then
  echo Device ID Set: INXDEVICEID = $INXDEVICEID
else # The device ID has not been set at boot time so set to the MAC address
  # use bash builtin to read the device ID if it exists 
  read -r INXDEVICEID < ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${VARIABLE_CACHE}HWID_NETIP 
  if [ ${#INXDEVICEID} -lt 6 ]; then # not got one, so try to get the MAC address again
    set -- `${INXWAREDEVMANROOT}${DEVMANCOREDIR}getHWID-NETIP.sh` # @todo this should use the variables
    INXDEVICEID="$1"
    INXDEVICEIP="$2"
  fi
fi

#Set the certificate paths for wget
if [ -e ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${CERTIFICATES}devman-ca.crt ]; then
SSLCERTS="--ca-certificate=${INXWAREDEVMANROOT}${DEVMANCOREDIR}${CERTIFICATES}devman-ca.crt"
fi
if [ -e ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${CERTIFICATES}devman-client-crt-key.pem ]; then
SSLCERTS="${SSLCERTS} --certificate=${INXWAREDEVMANROOT}${DEVMANCOREDIR}${CERTIFICATES}devman-client-crt-key.pem"
fi

#Read any proxy server configurations:
test -e ${SYSDATA}/HTTP_PROXY && export http_proxy=`cat "${SYSDATA}/HTTP_PROXY"`
test -e ${SYSDATA}/HTTPS_PROXY && export https_proxy=`cat "${SYSDATA}/HTTPS_PROXY"`

# Post our stuff to the server
# Create a version header
mkdir -p /run/ehs
echo "**************************************" > /run/ehs/platformstatus.log
cat ${EHSSYSTEMDATADIR}/version.nfo >> /run/ehs/platformstatus.log
echo "**************************************" >> /run/ehs/platformstatus.log

# Run the commands we're sent in the dldata.tgz - which is in plain text not gsipped
test -e  ${DEVMANCOREDLDATA}/dldata.tgz &&  source ${DEVMANCOREDLDATA}/dldata.tgz &>> /run/ehs/platformstatus.log

#Todo2023 - remove this old CGI version: /usr/bin/wget $SSLCERTS -q -O - --post-data 'Group=DEFAULT&DeviceType=DEFAULT&DeviceID='$INXDEVICEID'&rawreport='"`tr -d ';&' < /run/ehs/platformstatus.log`" ${DEVMANURL}/cgi-bin/rawreport.cgi
/usr/bin/wget $SSLCERTS -q -O - --post-data 'Group=DEFAULT&DeviceType=DEFAULT&DeviceID='$INXDEVICEID'&rawreport='"`tr -d ';&' < /run/ehs/platformstatus.log`" ${DEVMANURL}/rawreport.php
