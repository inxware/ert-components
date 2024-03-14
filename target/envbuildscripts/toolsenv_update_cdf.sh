#!/bin/bash

set -e

SKIP_DOCS=FALSE

if [ "$1" = "" ]; then
    # If we haven't been given an install directory then install to 
    # the ../dist directory where the development tools repo would look when running
    TOOLBOX_INSTALL_PATH=DIST_ONLY
else
   if [ $1 = "--skip-docs" ]; then
	SKIP_DOCS=TRUE
   else
     TOOLBOX_INSTALL_PATH=$1
   fi
fi

#To Delete:
#HOME_PATH=`eval echo "~"`
#INXWARE_APP_BUILDER_PATH=${HOME_PATH}/inxwareAppBuilder
#INXWARE_APP_BUILDER_DIST_PATH=${INXWARE_APP_BUILDER_PATH}/dist

INXWARE_APP_BUILDER_DIST_PATH=${TOOLBOX_INSTALL_PATH}


if [ -z "$INXWARE_TOOLBOXES_VERSION" ]; then
    # default version
    INXWARE_TOOLBOXES_VERSION="1.0.0"
fi

# TODO - this should be defined else where
# This is is the directory name  used in the tools toolboxes/ directory - it shouldn't need to be specific, 
# but should be consistent for a standard level install.
INXWARE_FEATURE_CLASS=5

# TODO - this path be defined else where for a specific feature class
# We should have a STDF.xml for each standard capability class identified in the a known location (possibly just ./Common/Componwnts/*.STDF 
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
        # make sure that old CDFs are removed
        if [ -d "$DST_PATH" ]; then
            rm -rf ${DST_PATH}
        fi
        mkdir -p ${DST_PATH}/CDF
        echo "copying to ${DST_PATH}"
        cp ${INXWARE_STDF_PATH} ${DST_PATH}
        find ./Common/Components/ -iname '*.cdf' -exec cp {} "${DST_PATH}/CDF/"  \;
        if [ "$SKIP_DOCS" = "FALSE" ] ; then
	 # update all docs
         if [ -z "$DO_NOT_UPDATE_DOCS" ]; then
            updateAllDocs $1
         fi
        fi
    fi
}

function checkDuplicatedContent(){
    find . ! -empty -iname '*.cdf' -exec md5sum {} + | sort | uniq -w32 -dD
}

function checkDuplicatedName(){
    find . -iname '*.cdf' | sed 's_.*/__' | sort|  uniq -d|
    while read fileName
    do
        find . -iname '*.cdf' | grep -iF "$fileName"
    done
}


if [ "$INXWARE_APP_BUILDER_DIST_PATH" != "DIST_ONLY" ]; then
  # copy toolboxes to installed iAB
  copyToolboxes ${INXWARE_APP_BUILDER_DIST_PATH}
fi

# copy toolboxes to dist directory placed next to the repo
copyToolboxes ../dist

# check if there are any duplicated CDFs present in the repo and report this to the user
DUPLICATED=$( checkDuplicatedContent )
if [ -n "$DUPLICATED" ]; then
    echo "Warning !!! There are CDF files with duplicated content. Make sure only relevant files are added to the toolboxes."
    checkDuplicatedContent
fi

DUPLICATED=$( checkDuplicatedName )
if [ -n "$DUPLICATED" ]; then
    echo "Warning !!! There are CDF files with the same name. Make sure only relevant files are added to the toolboxes."
    checkDuplicatedName
fi
