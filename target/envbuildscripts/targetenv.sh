#!/bin/bash
# inx limited 2008

set -e
export SPECIFIC_TARGET=$1
echo "########################  Building target environment staging tree ################################"
echo "#  Build parameters: "
echo "#--> GNU OS     : $EHS_GNU_OS"
echo "#--> GNU ARCH   : $EHS_GNU_ARCH"
echo "#--> COMMON OS  : $EHS_OS"
echo "#--> COMMON ARCH: $EHS_ARCH"
echo "#--------------------------------------------------------------------------------------------------"
echo "#--> SYSTEM_VARIANT: $SYSTEM_VARIANT"
echo "#--> PRODUCT_NAME: $PRODUCT_NAME"
echo "####################################################################################################"

EHS_SKIP_BIN_COPY=""
if [ "$2" = "--skip-bin-cpy" ]; then
	EHS_SKIP_BIN_COPY="yes"
fi

########################################################################
## Stop disturbances from cron to the environment if we are mapped to /root/ehs
#which crontab &> /dev/null && crontab -r

# export UPLOAD variable to let bash use it for doing uploads (e.g. to debian servers). 
export UPLOAD

########################################################
#create File system, including target specific parts and version information
#echo  "running ./target/envbuildscripts/targetenv_directories.sh" $SPECIFIC_TARGET
echo -e "\nXXXXXXXXXXXXXXXX  Copying eRT hosted assets to staging directory...    XXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
./target/envbuildscripts/targetenv_directories.sh $SPECIFIC_TARGET
### EHS ################################################
if [ "${EHS_SKIP_BIN_COPY}" = "" ]; then
if test -e ehs_${SPECIFIC_TARGET}.${EXE}; then 
	echo -e "Found ehs_"${SPECIFIC_TARGET}.${EXE}", copying to target staging directory:\n../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.${EXE}\n"; 
	cp "ehs_${SPECIFIC_TARGET}.${EXE}" "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/ehs.${EXE}"
else 
	echo "ERROR: ehs_${SPECIFIC_TARGET}.${EXE} not found! - please build using make all first or try make help for more details  - Exiting ..." 
	exit 1
fi
fi

EHS_TOOLCHAIN_PATH_FROM_BASE="ert-build-support/toolchains/${TOOLCHAIN_PATH}"
EHS_TOOLCHAIN_LOCAL_PATH="${LOCAL_BASE}/${EHS_TOOLCHAIN_PATH_FROM_BASE}"
EHS_TOOLCHAIN_SERVER_PATH="${REPOSITORY}/${EHS_TOOLCHAIN_PATH_FROM_BASE}"

echo "Adding Supporting Target Binaries to EHS File System..."
#Add user space support for EHS and components:
mkdir -p ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib
# toolchain sysroot shared libs
# note - may not exist for target
#todo this looks a bit of a mess?
if [ -e ../ert-build-support/toolchains/${TOOLCHAIN_PATH}/sysroot/lib ]; then
	echo "copying toolchain libraries from ../ert-build-support/toolchains/${TOOLCHAIN_PATH}/sysroot/lib/"
	cp -PR ../ert-build-support/toolchains/${TOOLCHAIN_PATH}/sysroot/lib/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib/ 
else 
	echo "No toolchain libraries found in ../ert-build-support/toolchains/${TOOLCHAIN_PATH}/sysroot/lib/"
fi
#Tidy up any accidently copied static libraries of toolchain objects 
rm -f ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib/*.a
rm -Rf ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib/gcc
# additional clib libraries if built separately
if [ ! -n "${EHS_CLIB_OVERRIDE_PATH}" ];then
	if [ -e "../ert-build-support/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/target_packages" ]; then
		echo "Found additional core libraries in "
		echo "../ert-build-support/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/target_packages"
		cp -PR "../ert-build-support/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/target_packages/"* "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin"
	else
		echo "No additional core libraries found in "
		echo "../ert-build-support/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/target_packages"
	fi
else
	echo "WARNING - using CLIB override path =${EHS_CLIB_OVERRIDE_PATH}"
	if [ -e "../ert-build-support/support_libs/target_libsx/${EHS_CLIB_OVERRIDE_PATH}/target_packages" ];then
		echo "Copying files from ../ert-build-support/support_libs/target_libs/${EHS_CLIB_OVERRIDE_PATH}/target_packages/"
		cp -PR "../ert-build-support/support_libs/target_libs/${EHS_CLIB_OVERRIDE_PATH}/target_packages/"* "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin"
	else
		echo "No additional support libraries found in"
		echo "../ert-build-support/support_libs/target_libs/${EHS_CLIB_OVERRIDE_PATH}/target_packages/"
	fi
fi 

#and finally the component libraries, we more often do expect to find
if [ -d "../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/target_packages/" ]; then
	echo "Copying files from"
	echo "../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/target_packages/"
	cp -PR --remove-destination "../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/target_packages/"* "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/" || exit
else 
	echo "No component libraries found in"
	echo "../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/target_packages/"
fi
echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

# "XXXXXXXXXXXXXXXXXXXXX Setting the internal version information file for the build XXXXXXXXXXXXXXXXXXXXXXX"
./target/envbuildscripts/targetenv_create_version_info.sh $SPECIFIC_TARGET
# "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

echo "XXXXXXXXXXXXXXXXXXXXX Installing the default app  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
if [ "${EHS_SKIP_REPO_PULL}" = "" ];then
echo "Checking out the latest PRODUCTION branch of your app repo..."
	if [ -d ../apps/ ];then
		if [ -f ../ert-components/COMMUNITY_RELEASE ]; then
			echo "Don't need to pull in github build"
		else  
			pushd ../apps/
			git pull origin RELEASE-PRODUCTION || exit 1
			popd
		fi
	else
		pushd ..
		if [ -f ../ert-components/COMMUNITY_RELEASE ]; then
			echo "Not getting the apps.git repo as the yaml action willdo this with credentials"
		#	git clone git@github.com:inxware/apps.git
		else
			git clone ssh://tech-data@dev.inx-systems.net:8822/home/inx-data/data/Repos/apps.git
		fi
		cd apps/
		git checkout RELEASE-PRODUCTION || exit 1
		popd
	fi
else 
	echo "WARNING!!! NOT Checking out the latest PRODUCTION branch of your app repo EHS_SKIP_REPO_PULL is set "
	echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
fi

EHS_APP_EXPORT_DIR=export-ert${ERT_SODL_VERSION}

if [ "${EHS_DEFAULT_APP}" = "" ]; then
	echo "Installing the desktop HOME app (../apps/systemapps/Home/) "
	cp -Rf ../apps/systemapps/Home/${EHS_APP_EXPORT_DIR}/* ../TARGET_TREES/ehs_env-$TARGET/appdata/default/ || exit 1
else 
   if [ "${EHS_DEFAULT_APP}" = "NONE" ]; then 
       echo "WARNING!!!!!!!!!: Not installing a default app (EHS_DEFAULT_APP = "NONE") !!!!!!!!!!"
   else
	   echo "Installing platform configured [../apps/${EHS_DEFAULT_APP}]"
	   cp -Rf ../apps/${EHS_DEFAULT_APP}/${EHS_APP_EXPORT_DIR}/* ../TARGET_TREES/ehs_env-$TARGET/appdata/default/ || exit 1
	   echo "Default app copied OK"
   fi
fi
echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
echo "XXXXXXXXXXXXXXXXXXXXX Configuring the IoT server service  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
if [ -f ./COMMUNITY_RELEASE ]; then
	echo "No Certificate repo used"
else
	if [ "${EHS_SKIP_REPO_PULL}" = "" ];then
		if [ -d ../DevmanSecurity/ ];then 
			pushd ../DevmanSecurity/
			git pull origin master
			popd
		else
			pushd ..
			git clone ssh://tech-data@dev.inx-systems.net:8822/home/inx-data/data/Repos/DevmanSecurity.git
			cd DevmanSecurity/
			git checkout master
			popd
		fi
	else 
		echo "WARNING!!! NOT Checking out the latest PRODUCTION branch of Devman Securiyu repo EHS_SKIP_REPO_PULL is set "
		echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
	fi
fi

function SetDevmanServer(){

local DEVMAN_SERVER_DOMAIN=$1
local DEVMAN_SERVER_DOMAIN_DEFAULT=$2

if [ "${DEVMAN_SERVER_DOMAIN}" != "" ]; then
	# devman cert destination directory
	DEVMAN_CERT_DIR=certs
	if [ "$DEVMAN_SERVER_DOMAIN_DEFAULT" != "default" ]; then
		DEVMAN_CERT_DIR="certs/$DEVMAN_SERVER_DOMAIN"
	fi
	if [ "${DEVMAN_SERVER_PROTOCOL}" = "mqtts" ]; then
		echo "Looking for certificates in  ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker"
	else
		echo "Looking for certificates in  ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs"
	fi
	#todo we should be able to remove the following if targetenv_directories has done it's job
	mkdir -p ../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR/
	echo "Target certificate directory ../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR/"
	
	# We will first always copy the single CA cert for Devman to the staging directory because this may be used by the downloader instead of a full bundle
	if [ "${DEVMAN_SERVER_PROTOCOL}" = "https" ] || [ "${DEVMAN_SERVER_PROTOCOL}" = "http" ]; then
		# devman-only-ca.crt - is only use by the android supervisor, and only needed when DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE is set and overwrites devman-ca.crt (see below)
		if [ "${BUILD_WITH_ANDROID_SUPERVISOR}" != "" -a "${BUILD_WITH_ANDROID_SUPERVISOR}" != "no" ]; then
			echo "Copying single Devman pem format client key to Devman directory" 
			if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca.crt ]; then
				cp -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca.crt ../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR/devman-only-ca.crt
			else 
				echo "Warning - You have specified DEVMAN_SERVER_PROTOCOL = https, but a CA cert is not found at"
				echo "        ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca.crt"
			fi
		fi
	elif [ "${DEVMAN_SERVER_PROTOCOL}" = "mqtts" ]; then
		echo "Copying Devman mqtt broker CA to Devman directory"
		CA_CERT_FILE=../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/ca.crt
		if [ -f "${CA_CERT_FILE}" ]; then
			cp -f ${CA_CERT_FILE} ../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR/ca.crt
		else 
			echo "Warning - You have specified DEVMAN_SERVER_PROTOCOL = mqtts, but a CA cert is not found at"
			echo "        ${CA_CERT_FILE}"
		fi
	else
		echo "Warning you have configured a non-secure http protocol for Devman"
	fi

    # We now decide wht the default CA certs should be for the eRT runtime as sometimes we don't have a full bundle).
	# e.g. 32bit version of linux curl needs a full CA bundle it seems 
	if [ "${DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE}" != "" -a "${DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE}" != "none"  ]; then
		echo "Copying pem format client key to Devman directory" 
		if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca-bundle.crt ]; then
			cp -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca-bundle.crt ../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR/devman-ca.crt
			# todo2023 This fCkd shizzle for some hardwired thing I can't find in the build that makes unity EHS always look for it, what ever the source code says.
			# I'm pretty sure we can get rid of this now
			cp -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca-bundle.crt ../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR/cacert.pem
		else 
			echo "ERROR - You have specified DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE, but a bundle does not exist at"
			echo "        ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca-bundle.crt"
			exit 1
		fi
	else 
		if [ "${DEVMAN_SERVER_PROTOCOL}" = "https" ] || [ "${DEVMAN_SERVER_PROTOCOL}" = "http" ]; then
			echo "Copying pem format client key to Devman directory" 
			if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca.crt ]; then
				cp -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca.crt ../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR/devman-ca.crt
			else 
				echo "Warning - You have specified DEVMAN_SERVER_PROTOCOL = https, but but a CA cert is not found at"
				echo "        ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca.crt"
			fi
		elif [ "${DEVMAN_SERVER_PROTOCOL}" = "mqtts" ]; then
			echo "Copying Devman mqtt broker clinet cert and key to Devman directory"
			CLIENT_CERT_FILE=../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/client.crt
			if [ -f "${CLIENT_CERT_FILE}" ]; then
				cp -f ${CLIENT_CERT_FILE} ../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR/client.crt
			else 
				echo "Warning - You have specified DEVMAN_SERVER_PROTOCOL = mqtts, but a CLIENT cert is not found at"
				echo "        ${CLIENT_CERT_FILE}"
			fi
			CLIENT_KEY_FILE=../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/client.key
			if [ -f "${CLIENT_KEY_FILE}" ]; then
				cp -f ${CLIENT_KEY_FILE} ../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR/client.key
			else 
				echo "Warning - You have specified DEVMAN_SERVER_PROTOCOL = mqtts, but a CLIENT key is not found at"
				echo "        ${CLIENT_KEY_FILE}"
			fi
		else
			echo "Warning you have configured a non-secure http protocol for Devman"
		fi
	fi
	if [ "${DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED}" != "" -a "${DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED}" != "no"  ]; then
		if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-client-crt-key.pem ]; then
			echo "Copying pem format client key to Devman config directory..." 
			cp  -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-client-crt-key.pem ../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR/
		else
			if [ "${DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED}" != "" -a "${DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED}" != "no"  ]; then
				echo "ERROR!!!! No client certificte in PEM format is available for ${DEVMAN_SERVER_DOMAIN} and you have set DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED"
				exit 1
			else
				echo "WARNING !!!! No client certificte in PEM format is available for ${DEVMAN_SERVER_DOMAIN}"
			fi
		fi
	fi

	if [ "${BUILD_WITH_ANDROID_SUPERVISOR}" != "" -a "${BUILD_WITH_ANDROID_SUPERVISOR}" != "no" ]; then
		if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-client-crt-key.p12 ]; then
			echo "Copying pem format client key to Devman config directory..." 
			cp  -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-client-crt-key.p12 ../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR/
		else
			echo "WARNING !!!! No client certificte in P12 format client cert is available for ${DEVMAN_SERVER_DOMAIN} (Required only for the Android supervisor)"
		fi
	fi

	if [ "${DEVMAN_SERVER_DOMAIN_DEFAULT}" = "default" ]; then

	# only needs to be set for the default domain
	echo "Updating the IoT Server URL..."
	mkdir -p ../TARGET_TREES/ehs_env-$TARGET/devman/core/config
	if [ "${DEVMAN_SERVER_PROTOCOL}" = "" ]; then
		echo "WARNING DEVMAN_SERVER_PROTOCOL is not set. Setting to https"
		DEVMAN_SERVER_PROTOCOL = "https"
	fi
	#configure the devman URLs:
	if [ "${DEVMAN_SERVER_PROTOCOL}" = "mqtts" ]; then
		echo "Setting MQTT (${DEVMAN_SERVER_DOMAIN}) devman URLs ====> DEVMANURL.000"
		echo "${DEVMAN_SERVER_DOMAIN}" > ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/DEVMANURL.000
		# check if mqtt devman clinet user name and password files are present
		if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/uname ]; then
			cp ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/uname ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/uname.000
			echo "Devman MQTT clinet username file copied"
		fi
		if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/passw ]; then
			cp ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/passw ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/passw.000
			echo "Devman MQTT clinet password file copied"
		fi
	else
		echo "Setting (${DEVMAN_SERVER_PROTOCOL}://${DEVMAN_SERVER_DOMAIN}) devman URLs ====> DEVMANURL.000"
		test -d ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/ || mkdir -p ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/
		echo "${DEVMAN_SERVER_PROTOCOL}://${DEVMAN_SERVER_DOMAIN}" > ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/DEVMANURL.000
	fi
	fi
else 
	echo "Warning!!!! DEVMAN_SERVER_DOMAIN is not set. Not configuring an IoT support server in targetenv"
fi

} # SetDevmanServer()

# check the default certificate domain
if [ "$DEVMAN_SERVER_DOMAIN" != "" ]; then

echo "Setting DEVMAN_SERVER_DOMAIN (default)"
SetDevmanServer ${DEVMAN_SERVER_DOMAIN} "default"

# check the second certificate domain
if [ "$DEVMAN_SERVER_DOMAIN_1" != "" ]; then
echo "Setting DEVMAN_SERVER_DOMAIN_1"
SetDevmanServer ${DEVMAN_SERVER_DOMAIN_1} ""
else
echo "Skip DEVMAN_SERVER_DOMAIN_1 ..."
fi

# check the third certificate domain
if [ "$DEVMAN_SERVER_DOMAIN_2" != "" ]; then
echo "Setting DEVMAN_SERVER_DOMAIN_2"
SetDevmanServer ${DEVMAN_SERVER_DOMAIN_2} ""
else
echo "Skip DEVMAN_SERVER_DOMAIN_2 ..."
fi

else 
	echo "Warning!!!! DEVMAN_SERVER_DOMAIN is not set. Not configuring an IoT support server in targetenv"
fi

echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

echo "XXXXXXXXXXXXXXXXXXXXX Looking for platform hacks script... XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
# Some devices have varying OSs features that need to be modified:
# Targetenv hacks are selected specifically with $INXWARE_TARGETENV_HACKS as there are other unrelated 
# factors defined by $SPECIFIC_TARGET used elsewhere too which may are may not be useful here.
# TODO we should probably support a list of hack files in the INXWARE_TARGETENV_HACKS variable
if  test -e ./target/envbuildscripts/platform-hacks/targetenv_hacks_${INXWARE_TARGETENV_HACKS}.sh 
then
	echo "Applying Hack: /target/envbuildscripts/platform-hacks/targetenv_hacks_${INXWARE_TARGETENV_HACKS}.sh $INXWARE_TARGETENV_HACKS"
	./target/envbuildscripts/platform-hacks/targetenv_hacks_${INXWARE_TARGETENV_HACKS}.sh $SPECIFIC_TARGET
else
	if [ "$INXWARE_TARGETENV_HACKS" != "" ]; then
		echo "No Hack file found for /target/envbuildscripts/platform-hacks/targetenv_hacks_${INXWARE_TARGETENV_HACKS}.sh"
		exit 1
	fi
fi

if [ -d ../apps ]; then
	pushd ../apps

	if [ -f ../ert-components/COMMUNITY_RELEASE ]; then
		git checkout main || :
	else
		git checkout master || :
	fi
fi

echo    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
echo -e "\n----------------  Completed make targetenv - staging directory ready at ------------------------------"
echo      "../TARGET_TREES/ehs_env-$SPECIFIC_TARGET"
echo      "------------------------------------------------------------------------------------------------------"
echo      "#############################  DONE TARGETENV BUILD ##################################################"
