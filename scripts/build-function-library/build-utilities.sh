

function cdBuildTreeRoot() {

    if [ -f ./configure ] ; then 
    echo "."
    else
        cd ../../../
        if [ -f ./configure ]; then
            echo "."
        else
            return 1
        fi
    fi
}