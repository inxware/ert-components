#!/bin/bash

echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
echo " Copying TSA scheduler application "
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

cp -Rf ../apps/customer-apps/TSA/ambifier-v0.6.0-integrated/export/* ../TARGET_TREES/ehs_env-$TARGET/appdata/default/ || exit
echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"




# Install ssl certificates and keys

This is not done using filesystem for android apks for security

#make the media directory:
mkdir -p ../TARGET_TREES/ehs_env-$TARGET/userdata/media
#remove things we don't want

#./target/envbuildscripts/targetenv_remove_gtk_gst_libs-ambifier.sh
echo "######################################################################################################"
echo -e "To create the Debian package you need to run \n#make targetenv_deb."
echo -e "To create and Upload the Debian package to a Debian Server run \n#make targetenv_deb UPLOAD=\"ambifier@www.ambifier.com:/home/ambifier/deb-repo"
echo "######################################################################################################"