#!/bin/bash
# P Drezet 2008

#stop any crontabs on this machine that will create guff. Too brutal? ... Sorry.
crontab -r

if [[ ! -n ${1} ]]
	then
	echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	echo No target path is defined ${1} - exiting
	echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	exit
fi

SPECIFIC_TARGET=$1;
#echo Creating Target path same as source target path =$SPECIFIC_TARGET

if [ -d ../TARGET_TREES/ehs_env-$TARGET ]; then

#Install the default apps we want

mkdir -p ../TARGET_TREES/ehs_env-$TARGET/appdata/default

rm -f ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/sysdata/ehslog.csv
rm -f ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ehs_tcpip.log
rm -f ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/sysdata/*.flag
rm -f ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/sysdata/var/*

rm -f ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/sysdata/app2run.nfo

# remove any logs from debug versions etc.
rm -f ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/sysdata/ehslog.csv
rm -f ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/sysdata/ehs_tcp.log

# remove any unique to customer data
# DeleteDevman configuration @todo the configuration data should be moved to a new single config folder.
# Depricated  

rm -f    ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/core/HWID_NETIP*
rm -f    ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/core/var/*
rm -f    ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/core/*.flag
rm -f    ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/core/reporting.flag
# temprary data
rm -f -r ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/core/download/*
rm -f -r ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/core/var/*

# configuration

if [[ ! -n ${KEEP_DEVMANCONFIG} ]];then
	rm -f -r ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/core/config/*
	rm -f    ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/plugins/*/DEVMAN*URL*
fi

# temporary files - always remove - all plugins 
rm -f    ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/plugins/0/*.flag
rm -f -r ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/plugins/0/download/*
rm -f    ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/plugins/1/*.flag
rm -f -r ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/plugins/1/download/*
rm -f    ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/plugins/2/*.flag
rm -f -r ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman/plugins/2/download/*

# DEFAULT IS TO REMOVE THE APPLICATION
if [[ ! -n ${KEEP_APPLICATION} ]]; then
	# clean application data
	rm -f -r ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/appdata/*
	mkdir  ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/appdata/default
	mkdir ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/appdata/temp
else 
	echo "NOTE: Keeping the application as requested"
fi

#clean userdata - includes any unique to unit configuration
if [[  -n ${KEEP_USERCONFIG} ]];then
	# clean application data
	mv ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/userdata/configs ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/userdata-configs-temp 
	rm -f -r ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/userdata/*
	mv ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/userdata-configs-temp ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/userdata/configs
	echo "NOTE: Keeping the userconfiguration information as requested"
else
	rm -f -r ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/userdata/*
fi

echo "######################################################################################################"
echo "# Done - Cleaning ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET of configuration"
echo "# This is now suitable for sys updates without changing existing configuration data"
echo "######################################################################################################"

else 
  echo "Target env directory $1 does not exist!! - Exiting"
  exit 1
fi
