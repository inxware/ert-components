#!/bin/bash

echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
echo " Copying TSA scheduler application "
if [ -d ../apps/ ];then 
pushd ../apps/ || exit
git checkout RELEASE-PRODUCTION || exit
git pull origin  RELEASE-PRODUCTION  || exit
popd
else
pushd .. || exit
git clone ssh://repo-user@server:/home/inx-data/data/Repos/apps.git || exit
git checkout  RELEASE-PRODUCTION
popd
fi

cp -Rf ../apps/customer-apps/TSA/ambifier-v0.6.0-integrated/export/* ../TARGET_TREES/ehs_env-$TARGET/appdata/default/ || exit
echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"




# Install ssl certificates and keys
mkdir -p ../TARGET_TREES/ehs_env-$TARGET/devman/core/certs/
cp  -f ./target/envtree/certificates/ambifier/devman-ca.crt ../TARGET_TREES/ehs_env-$TARGET/devman/core/certs/
cp  -f ./target/envtree/certificates/ambifier/devman-client-crt-key.pem ../TARGET_TREES/ehs_env-$TARGET/devman/core/certs/

echo "XX ========== XXXXXXXX Install Dependencies (e.g. Ambifier) XXXXXXXXXXXX ================ "
# Add some ambifier 2 daemons we now integrate into one package
if [ "${SYSTEM_VARIANT}" == "ambifier2-debug" -o "${SYSTEM_VARIANT}" == "ambifier2-deb11" ] ; then
	pushd ../Ambifier/Ambifier2/builds || exit
	git pull || exit
	popd
	cp -rp ../Ambifier/Ambifier2/builds/Ambifier2_LinuxServer ../TARGET_TREES/ehs_env-$TARGET/bin/ || exit
	
fi

echo "XX ========== XXXXXXXX ============= XXXXXXXXXXXX ================ "



#configure the devman URLs:
echo "Adding TSA's devman URLs"
#echo "http://devman.ambifier.com" > ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/DEVMANURL.000
echo "https://devman.ambifier.com" > ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/DEVMANURL.000
#echo "http://devman.ambifier.com/cgi-bin/devman_player.cgi" > ../TARGET_TREES/ehs_env-$TARGET/devman/plugins/1/DEVMANPLAYERURL.000

#and make the media directory:
mkdir -p ../TARGET_TREES/ehs_env-$TARGET/userdata/media
#remove things we don't want

#./target/envbuildscripts/targetenv_remove_gtk_gst_libs-ambifier.sh

echo -e "To create the Debian package you need to run \n#make targetenv_deb."
echo -e "To create and Upload the Debian package to a Debian Server run \n#make targetenv_deb UPLOAD=\"ambifier@www.ambifier.com:/home/ambifier/deb-repo"
