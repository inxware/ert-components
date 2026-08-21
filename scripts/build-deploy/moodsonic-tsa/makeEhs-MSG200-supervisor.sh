set -e

if [ -f ./configure ] ; then 
    source ./scripts/build-function-library/debian-utilities.sh
else
    source ../build-function-library/debian-utilities.sh
    cd ../../../
fi

cdBuildTreeRoot || exit 1

echo "================================================"
echo "Building the MSG200 supervisor armbian build"
./configure linux_arm64_clang_a64_supervisor
BuildDebian
UploadDebian-Devman
#make upload_ehs_deb UPLOAD="ambifier@www.ambifier.com:/home/ambifier/deb-repo"
echo "================================================"
