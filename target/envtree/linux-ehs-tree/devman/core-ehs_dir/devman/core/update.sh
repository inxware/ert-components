#!/bin/bash

######################################################################
# Copyright : inx limited, Sheffield, UK, 2008
# License   : wwww.inx-systems.com/inxware_license.html
# Type      : Target deployed unencrypted code
# Support   : support.inx-systems.com
# Description:
# This script is specific to OS level software updates. 
# Assumptions: The usual search path is set up.
######################################################################

export PATH=$PATH:/sbin/:/bin/:/usr/bin/

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

#Get the devman core - hardware fall-back
DEVMANURL="http://devman.inx-systems.com"
if [ -e "${INXWAREDEVMANROOT}${DEVMANCOREDIR}${DEVICE_CONFIG}DEVMANURL.000" ]; then # This scripts may be called with other devman server URLs
   DEVMANURL_TRY=`cat "${INXWAREDEVMANROOT}${DEVMANCOREDIR}${DEVICE_CONFIG}DEVMANURL.000"`
   if [ ${#DEVMANURL_TRY} -gt 8 ]; then
	DEVMANURL=$DEVMANURL_TRY
   else
	DEVMANURL="http://devman.inx-systems.com"
   fi	
fi


# touch ${INXWAREDEVMANROOT}${DEVMANCOREDIR}working.flag # Leave mutex to the caller

if [ ${#INXDEVICEID} -gt 5 ] # want at least five chars for a valid ID
then
  echo Device ID Set: INXDEVICEID = $INXDEVICEID
else # The device ID has not been set at boot time so set to the MAC address
  # use bash builtin to read the device ID if it exists 
  if [  -f ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${VARIABLE_CACHE}HWID_NETIP ];then
     read -r INXDEVICEID < ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${VARIABLE_CACHE}HWID_NETIP 
  else
  	INXDEVICEID=""
  fi
  if [ ${#INXDEVICEID} -lt 6 ]; then # not got one, so try to get the MAC address again
    set -- `${INXWAREDEVMANROOT}${DEVMANCOREDIR}getHWID-NETIP.sh` # @todo this should use the variables
    INXDEVICEID="$1"
    INXDEVICEIP="$2"
  fi
  #write to file for other process to read
  mkdir -p ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${VARIABLE_CACHE}
  echo -e "$INXDEVICEID\n$INXDEVICEIP\n" > ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${VARIABLE_CACHE}HWID_NETIP
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

# Check to see if there is anything on the server
dl_dataready=`wget $SSLCERTS -q -O - --post-data 'Group=DEFAULT&DeviceType=DEFAULT&DeviceID='$INXDEVICEID $DEVMANURL/devmanIsUpdatedCGI.php`

if [ "$dl_dataready" == "data_ready" ]; then 
	# Server has data available #
	# check to see the directories are all in tact

       

	if [ ! -e ${INXWAREDEVMANROOT}${DEVMANCOREDIR}download/ ] 
	then
	    mkdir ${INXWAREDEVMANROOT}${DEVMANCOREDIR}download
	fi 
	# Make decman more robust when / is readonly: try using tmpfs /run if we can't download scripts to /opt/...
	if touch ${INXWAREDEVMANROOT}${DEVMANCOREDIR}/download/icanwrite ; then
	     export DEVMANDOWNLOADDIR="${INXWAREDEVMANROOT}${DEVMANCOREDIR}/download" 
	else
	     export DEVMANDOWNLOADDIR=/run/ehs/download
	     mkdir -p ${DEVMANDOWNLOADDIR}
	fi
	
	rm -Rf ${DEVMANDOWNLOADDIR}/../download/*   # Careful with rm -rf! hence "../download/"- This is in case the previous write failed and there is debris in the folder that wold cause renaming of files
	cd ${DEVMANDOWNLOADDIR} # all data files in this directory
	# get the data if it exists.

	# Try using nice (default adjustment=10)
	#test -e /usr/bin/nice  && export NICE_RUNNER="/usr/bin/nice"
	#test -e /bin/nice      && export NICE_RUNNER="/bin/nice"
	export NICE_RUNNER=nice
	
	$NICE_RUNNER wget $SSLCERTS -q -O - --post-data 'Group=DEFAULT&DeviceType=DEFAULT&DeviceID='$INXDEVICEID $DEVMANURL/cgi-bin/downloading.cgi || :
	$NICE_RUNNER wget $SSLCERTS -q  --cache=off $DEVMANURL/DeviceGroup/DEFAULT/DEFAULT/$INXDEVICEID/dldata.tgz || :
	$NICE_RUNNER wget $SSLCERTS -q  --cache=off $DEVMANURL/DeviceGroup/DEFAULT/DEFAULT/$INXDEVICEID/dldata.sh  || :

	if [ -e ${DEVMANDOWNLOADDIR}/dldata.sh ];then
		chmod +x ${DEVMANDOWNLOADDIR}/dldata.sh	
	  	./dldata.sh || :
	fi
	# echo "Acknowledging receipt"
	$NICE_RUNNER 	wget  $SSLCERTS -q -O - --post-data 'Group=DEFAULT&DeviceType=DEFAULT&DeviceID='$INXDEVICEID "$DEVMANURL/cgi-bin/updated.cgi" || :
	# clean up when done
	rm -Rf ${DEVMANDOWNLOADDIR}/../download/* || :

#else   # there was nothing to do 
else # if we got something else it might be for someone else so we'll kindly we'll create a flag file any strings returned. (first element will be flag name, the second string will be written into the file)

# Legacy features for setting flag files in devman - this will be deprecated ahead of next full QA cycle.
RET_STRING=$(echo "$dl_dataready" | tr "[:blank:]" "\n")
#iterate to make sure blanks are ignored
INDEX=0;
for x in $RET_STRING; do 
   if [ -n $x ];then
	X[$INDEX]=$x
	INDEX=`expr $INDEX + 1`
   fi
done
if [ $INDEX -gt 0 ];then # write a flag named by the first string and containing the second
   touch "${INXWAREDEVMANROOT}${DEVMANPUBLICDATA}${X[0]}.flag"
   test -n ${X[1]} echo ${X[1]} >> "${INXWAREDEVMANROOT}${DEVMANPUBLICDATA}${X[0]}"
fi

fi
#rm -rf ${INXWAREDEVMANROOT}${DEVMANCOREDIR}working.flag

