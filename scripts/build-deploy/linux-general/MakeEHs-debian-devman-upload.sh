#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

set -e

if [ "$1" = "-no-upload" ]; then
    echo "Not Uploading builds"
else
    NO_UPLOAD="TRUE"
fi

#check we are in the root or the script diretory
if [ -f configure ] ; then 
    source ./scripts/build-function-library/debian-utilities.sh
else
    source ../../build-function-library/debian-utilities.sh
    cd ../../../
fi

echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
echo "Making Debian  8 32 bit ambifier (ehs) for tsa.ambifier.com - NO AMBIFIER2" 
echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
# old ambifier.com target: ./configure linux_amd64_gtk_gst_ambifier2_debian9
#./configure < Uses You currently configured target >
# make prepdeps # this needs to be done only once # Assume you have already done this
# For release make sure you check in a tagged release in the repo
if [ "$1" == "--update-version" ]; then
	make targetenv_version
fi
# Build the code and .deb
echo "Buildng the debian package..."
BuildDebian
#Upload to the server specified in target/platform/$TARGET/config.mk
test -z "$NO_UPLOAD" || echo "Uploading the debian package ...."
test -z "$NO_UPLOAD" || UploadDebian-Devman




