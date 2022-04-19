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
INX_IOT_VARIANT=${INX_IOT_VARIANT} ./iot-make.sh
pushd ../../
version=$(head -n 1 ../TARGET_TREES/ehs_env-${INX_IOT_VARIANT}/sysdata/version.nfo)
firmwareName=${INX_IOT_VARIANT}_${version}
#remove nxp_arm_ from the start of the firmware name
firmwareName="${firmwareName/nxp_arm_/}"
../ert-build-support/toolchains/x86_64/nxp-arm/bin/arm-none-eabi-objcopy -v -O binary ../TARGET_TREES/ehs_env-${INX_IOT_VARIANT}/bin/ehs.exe ${firmwareName}.bin
if [[ "nxp_arm_heatrod_hrcdispv1_release" = "${INX_IOT_VARIANT}" || "nxp_arm_heatrod_hridispv2_release" = "${INX_IOT_VARIANT}" || "nxp_arm_heatrod_hridispv1_release" = "${INX_IOT_VARIANT}" || "nxp_arm_heatrod_hridispv3_release" = "${INX_IOT_VARIANT}" || "nxp_arm_heatrod_hrcdispv2_release" = "${INX_IOT_VARIANT}" ]]; then
    echo "uploading to iot-vs1.nibe.se make sure you are on the VPN"
    scp -P 22 ${firmwareName}.bin extadmin@10.0.128.115:/home/inx-devman/bin/dldata-scripts/firmwares/${firmwareName}
else
    echo "uploading to www.inx-systems.net"
    scp -P 8822 ${firmwareName}.bin root@www.inx-systems.com:/home/inx-devman/bin/dldata-scripts/firmwares/${firmwareName}
fi
popd
