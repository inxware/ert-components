#assuming we are called from 2 directories above from a  

if [ -f configure ] ; then 
    source ./scripts/build-function-library/build-utilities.sh
else
    source ../../build-function-library/build-utilities.sh
    cd ../../../
fi

#source ../../build-function-library/build-utilities.sh

function BuildDebian(){
    make clean ||:
    make all_docker || exit 1
    make targetenv || exit 1
    #make targetenv_apk_docker || exit 1 # not docker for apk downloads gradle everytime (fix it before using) !
    make targetenv_deb_docker || exit 1
}

# Upload the package in staging directory to the approp server.
# The config.mk will read the devman server config files with all that is needed to do this to the right place
function UploadDebian-Devman() {
    make upload_ehs_deb
}