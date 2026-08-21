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

if [ -f configure ] ; then 
    source ./scripts/build-function-library/debian-utilities.sh
else
    source ../../build-function-library/debian-utilities.sh
    cd ../../../
fi

#Notes:
# We are only using ambifier2 builds on devman.ambifier.com and not on tsa.ambifier.com 
# so we only build these two ehs variants, one for TSA and one for Moodsonic 
echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
echo "Making Debian  8 32 bit ambifier (ehs) for tsa.ambifier.com - NO AMBIFIER2" 
echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
# old ambifier.com target: ./configure linux_amd64_gtk_gst_ambifier2_debian9
./configure linux_x86_gtk_gst_ambifier-tsa
make prepdeps # this needs to be done only once
if [ "$1" != "--skip-version" ]; then
	make targetenv_version
fi
BuildDebian
test -z "$NO_UPLOAD" || UploadDebian-Devman

#echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
#echo "Making Debian 9 64 bit ambifier (ehs) for tsa.ambifier.com - NO AMBIFIER2"
#echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
## old ambifier.com target: ./configure linux_amd64_gtk_gst_ambifier2_debian9
#./configure linux_amd64_gtk_gst_debian9_ambifier-tsa
#make prepdeps # this needs to be done only once
##make targetenv_version
#BuildDebian
#test -z "$NO_UPLOAD" || UploadDebian-Devman

echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
echo "Making Debian 9 ambifier2 (ehs-debug) for devman.ambifier.com - YES AMBIFIER2"
echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
./configure linux_amd64_gtk_gst_debian9_ambifier2-debug
BuildDebian
test -z "$NO_UPLOAD" || UploadDebian-Devman

echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
echo "Making Debian 11 ambifier2 (ehs) for tsa.ambifier.com - NO AMBIBIFER2" 
echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
./configure linux_x86_64_gtk_gst_debian11_ambifier-tsa
BuildDebian
test -z "$NO_UPLOAD" || UploadDebian-Devman


if [ 1 = 1 ]; then
# This seems to clobber the tsa server version...
#todo2023 - needs fixing!
echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
echo "Making Debian 11 ambifier2 (ehs-debug) for devman.ambifier.com - YES AMBIFIER2"
echo "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
# This does build the the package with the ambifier2 package (not ehs-debug deb) 
./configure linux_x86_64_gtk_gst_debian11_ambifier2
BuildDebian
test -z "$NO_UPLOAD" || UploadDebian-Devman
fi



