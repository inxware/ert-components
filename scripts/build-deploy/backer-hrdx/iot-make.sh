#!/bin/bash
if [[ -z "${INX_IOT_VARIANT}" ]]; then
    echo "You need to set INX_IOT_VARIANT to a valid ehs build target"
    exit
fi

#get current directory so we don't have to worry about paths
#taken from https://stackoverflow.com/questions/59895/getting-the-source-directory-of-a-bash-script-from-within
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"

#going to root of EHS directory
cd ${DIR}
pushd ../../
./configure ${INX_IOT_VARIANT} || exit 1
make targetenv_version || exit 1
version=$(head -n 1 ../TARGET_TREES/ehs_env-${INX_IOT_VARIANT}/sysdata/version.nfo)
echo "version=${version}"
make clean
firmwareName=${INX_IOT_VARIANT}_${version}
#remove nxp_arm_ from the start of the firmware name
firmwareName="${firmwareName/nxp_arm_/}"
rm ${firmwareName}.bin
CFLAGS=-DCONFIG_FIRMWARE_VERSION=\\\"${firmwareName}\\\" make all -j 8 || exit 1
popd
