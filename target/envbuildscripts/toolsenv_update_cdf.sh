#!/bin/bash

set -e

HOME_PATH=`eval echo "~"`

INXWARE_APP_BUILDER_PATH=${HOME_PATH}/inxwareAppBuilder
INXWARE_APP_BUILDER_DIST_PATH=${INXWARE_APP_BUILDER_PATH}/dist

if [ -z "$INXWARE_TOOLBOXES_VERSION" ]; then
    # default version
    INXWARE_TOOLBOXES_VERSION="1.0.0"
fi

# TODO - this should be defined else where
INXWARE_FEATURE_CLASS=5

# TODO - this path be defined else where for a specific feature class
INXWARE_STDF_PATH=./Common/Components/STDF.xml

function updateAllDocs(){
    DOCS_PATH=$1/docs
    if ! [ -d "$DOCS_PATH" ]; then
        echo "create $DOCS_PATH"
        mkdir ${DOCS_PATH}
    fi
    for f in `find ./Common/Components/ -name "docs"` 
    do
        DOC_NAME=$( echo $f | awk -F"/" '{ print "/"$(NF-1)"/"$NF }' )
        DST_DOC=${DOCS_PATH}${DOC_NAME}
        mkdir -p ${DST_DOC}
        cp -r ${f}/* ${DST_DOC}
    done
    # copy generic style
    GENERIC_STYLE=./Common/Components/iabStyle.css
    [ -f "$GENERIC_STYLE" ] && cp ${GENERIC_STYLE} ${DOCS_PATH}
}

function copyToolboxes(){
    if [ -d "$1" ]; then
        DST_PATH=$1/toolboxes/${INXWARE_FEATURE_CLASS}/${INXWARE_TOOLBOXES_VERSION}
        mkdir -p ${DST_PATH}/CDF
        echo "copying to ${DST_PATH}"
        cp ${INXWARE_STDF_PATH} ${DST_PATH}
        find ./Common/Components/ -iname '*.cdf' -exec cp {} "${DST_PATH}/CDF/"  \;
        # update all docs
        if [ -z "$DO_NOT_UPDATE_DOCS" ]; then
            updateAllDocs $1
        fi
    fi
}

# copy toolboxes to installed iAB
copyToolboxes ${INXWARE_APP_BUILDER_DIST_PATH}

# copy toolboxes to dist directory placed next to the repo
copyToolboxes ../dist

exit 0

########################################################################################
# TODO - remove legacy stuff

homePath=`eval echo "~"`
winePath="${homePath}/.wine/drive_c/users/patrick/Application Data/inx/brix"
if ! [ -d "${winePath}" ]; then
	winePath="${homePath}/.wine/drive_c/users/pdrezet/Application Data/inx/brix"
fi
if [ -d "${winePath}" ]; then
    echo "copying to ${winePath}"
    find ./Common/Components/ -iname '*.idf.*' -exec cp {} "${winePath}/IDF/"  \;
    find ./Common/Components/ -iname '*.cdf' -exec cp {} "${winePath}/CDF/"  \;
    find ./Common/Components/ -iname '*.bmp' -exec cp {} "${winePath}/BMP/"  \;
fi

newToolsPath="${homePath}/work/inx/tools/V2-OpenGL-Wx/brix-tools/iab/dist"
if [ -d "${newToolsPath}" ]; then
    echo "copying to ${newToolsPath}"
    find ./Common/Components/ -iname '*.idf.*' -exec cp {} "${newToolsPath}/IDF/"  \;
    find ./Common/Components/ -iname '*.cdf' -exec cp {} "${newToolsPath}/CDF/"  \;
    find ./Common/Components/ -iname '*.bmp' -exec cp {} "${newToolsPath}/BMP/"  \;
fi

if [ -d "../dist/CDF" ]; then
    echo "copying to ../dist/"
    find ./Common/Components/ -iname '*.idf.*' -exec cp {} "../dist/IDF/"  \;
    find ./Common/Components/ -iname '*.cdf' -exec cp {} "../dist/CDF/"  \;
    find ./Common/Components/ -iname '*.bmp' -exec cp {} "../dist/BMP/"  \;
else
    echo "Please check out the dist path"
fi
