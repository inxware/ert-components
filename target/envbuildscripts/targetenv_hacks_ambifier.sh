#!/bin/bash

echo Gstreamer alsa audio libraries hack


# Avoid compaints from the new alsa build with older libraries?
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libasound*

pushd ../TARGET_TREES/ehs_env-$TARGET/bin/cslib
#ln -fs /usr/lib/libasound.so.2 libasound.so.2
#ln -fs /usr/lib/libasound.so.2 libasound.so.2.0.0
# This should be done by the lib but we'll do it again here as we don't trust it
#ln -fs libfaad.so.2 libfaad.so.0
popd

./targetenv_clean_config.h $TARGET

if [ -d ../apps/ ];then 
pushd ../apps/
git pull origin RELEASE-PRODUCTION
popd
else
pushd ..
git clone ssh://repo-user@server:/home/inx-data/data/Repos/apps.git
git checkout RELEASE-PRODUCTION
popd
fi

#cp -Rf ../apps/customer-apps/TSA/ambifier-v0.5.0-gui-priority-no-wds/export/* ../TARGET_TREES/ehs_env-$TARGET/appdata/default/
cp -Rf ../apps/customer-apps/TSA/ambifier-v0.6.0-integrated/export/* ../TARGET_TREES/ehs_env-$TARGET/appdata/default/

# Install ssl certificates and keys
mkdir -p ../TARGET_TREES/ehs_env-$TARGET/devman/core/certs/
# 32bit version of curl needs a full CA bundle it seems 
cp  -f ./target/envtree/certificates/ambifier/cacert-devman.pem ../TARGET_TREES/ehs_env-$TARGET/devman/core/certs/devman-ca.crt
cp  -f ./target/envtree/certificates/ambifier/devman-client-crt-key.pem ../TARGET_TREES/ehs_env-$TARGET/devman/core/certs/

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
