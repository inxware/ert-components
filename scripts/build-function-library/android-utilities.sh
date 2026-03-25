
#assuming we are called from 2 directories above from a  
if [ -f configure ] ; then 
    source ./scripts/build-function-library/build-utilities.sh
else
    source ../../build-function-library/build-utilities.sh
    cd ../../../
fi

#Assumed PWD is already the build tree root directory
function BuildAndroid(){
    make clean ||:
    make all_docker || exit 1
    make targetenv || exit 1
    #make targetenv_apk_docker || exit 1 # not docker for apk downloads gradle everytime (fix it before using) !
    make targetenv_apk || exit 1
    #Add any other pacakges that get bundled with eRT
}

function UploadApk-ambifier() {
    make targetenv_android_dep_pack || exit
    # The default for the upload url/path may be encoded in the config.mk file
    make upload_ehs_sys_patch || exit 1
}

function InstallToDevice() {
    make upload_ehs_via_adb  || exit
    # OR make upload_ehs_via_adb  ADB_IP=192.168.1.1 || exit 
}

