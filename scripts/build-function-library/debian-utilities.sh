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

function UploadDebian-ambifier(){
    # todo2023 The default for the above may be encoded in the config.mk file
    make upload_ehs_deb UPLOAD="ambifier@www.ambifier.com:/home/ambifier/deb-repo" || exit 1
}
