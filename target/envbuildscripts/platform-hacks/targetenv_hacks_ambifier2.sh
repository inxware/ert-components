#!/bin/bash

# Add the ambifier 2 daemons we now integrate into one package
if [ "${INXWARE_TARGETENV_HACKS}" = "ambifier2" ] ; then
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Installing Ambifier dependencies >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
	pushd ../Ambifier/Ambifier2/builds || exit
	git pull || exit
	popd
	cp -rp ../Ambifier/Ambifier2/builds/Ambifier2_LinuxServer ../TARGET_TREES/ehs_env-$TARGET/bin/ || exit 1
	echo "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
else 
  echo "WARNING!!! you have not set INXWARE_TARGETENV_HACKS to ambifier2, but seem to be trying to install it?" 
  echo "!!!!!!!!!!---------------------- NOT Installing Ambifier dependencies -------------------------!!!!!!!"
fi

#and make the media directory (we probably don't need this anymore, but just in case...):
mkdir -p ../TARGET_TREES/ehs_env-$TARGET/userdata/media
echo "######################################################################################################"
echo -e "All Done!\nTo create the Debian package you need to run \n#make targetenv_deb."
echo -e "To create and Upload the Debian package to a Debian Server run \n#make upload_ehs_deb UPLOAD=\"ambifier@www.ambifier.com:/home/ambifier/deb-repo"
echo "######################################################################################################"