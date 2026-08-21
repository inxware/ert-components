#!/bin/bash
set -x
if [ -z "${INX_SERIAL}" ]; then
# We use what ever jtag is plugged in now

#    echo "Please set environment variable INX_SERIAL to the serial number of the debugger they will be using. E.g 611000002"
#    exit
export INX_SERIAL="ANYJTAG"
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

startingDir=`pwd`

cd ${DIR}

#create an empty release dir
releaseDirName="heatrodCommissioningRelease"
goScriptName="${releaseDirName}/go.sh"
rm -r ${releaseDirName}
mkdir -p ${releaseDirName}/firmwares

#create commission firmware
cd ../../../ || exit
./configure nxp_arm_heatrod_hri_commission ||exit
make clean 
make -j 5  || exit


#now create all the variants
#variants=("nxp_arm_heatrod_hrcdispv1_release" "nxp_arm_heatrod_hrcdispv2_release" "nxp_arm_heatrod_hridispv1_release" "nxp_arm_heatrod_hridispv2_release" "nxp_arm_heatrod_hridispv3_release")
variants=("nxp_arm_heatrod_hrcdispv2_release"  "nxp_arm_heatrod_hridispv3_release")
echo "Make sure you have built these variants:"

for variant in "${variants[@]}"
do
    echo COPYING VARIANT BIN: $variant
	INX_IOT_VARIANT=${variant} ./scripts/buid-deploy/iot-make.sh || exit
    mkdir -p ${DIR}/${releaseDirName}/firmwares/${variant}
    # we put a copy of the commissioning code into each target type, because curretly the processor commisioning is the same for all targets.
    cp ../TARGET_TREES/ehs_env-nxp_arm_heatrod_hri_commission/bin/ehs.exe ${DIR}/${releaseDirName}/firmwares/${variant}/commission.bin || exit
    cp ../TARGET_TREES/ehs_env-${variant}/bin/ehs.exe ${DIR}/${releaseDirName}/firmwares/${variant}/release.bin || exit
done
#make the Usual variants more easy to Identify
pushd ${DIR}/${releaseDirName}/firmwares/ || exit
test -d nxp_arm_heatrod_hridispv3_release  && mv nxp_arm_heatrod_hridispv3_release HRDI || exit
test -d nxp_arm_heatrod_hrcdispv2_release && mv nxp_arm_heatrod_hrcdispv2_release HRDC || exit
popd

cd ${DIR}

#create the go.sh
echo "#!/bin/bash" > ${goScriptName}
echo "cd runtime" >> ${goScriptName}
echo "SERIAL=${INX_SERIAL} ./flashandmonitor.sh" >> ${goScriptName}
chmod u+x ${goScriptName}

#copy in the scripts/firmwares/libs etc
cp -a ./runtime ${releaseDirName}

#copy in the stuff to make the runtime work
cp -a ../../../../ert-build-support/debuggers/SEGGER/JLink_V646j/JLinkGDBServerCLExe ${releaseDirName}/runtime
cp -a ../../../../ert-build-support/debuggers/SEGGER/JLink_V646j/libjlinkarm* ${releaseDirName}/runtime
mkdir ${releaseDirName}/runtime/arm-none-eabi
cp -a ../../../../ert-build-support/toolchains/x86_64/nxp-arm/arm-none-eabi/share ${releaseDirName}/runtime/arm-none-eabi
mkdir ${releaseDirName}/runtime/bin
cp -a ../../../../ert-build-support/toolchains/x86_64/nxp-arm/bin/arm-none-eabi-gdb-py ${releaseDirName}/runtime/bin

#build the tarball and remove the dir
tar -cjf ./${releaseDirName}.tar.bz2 ${releaseDirName}
rm -r ${releaseDirName}
mv ${releaseDirName}.tar.bz2 ${startingDir}
