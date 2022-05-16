#!/bin/bash
# copyright inx limited 2010

# Sort the version info for the target & more:
# Reads the current version number for the repo and poplated the target's version info file.
# If INC_VERSION  is set to yes then the version number is incremeneted (causing a complete set of rebuilds)
# and also causing a tag entry to be netered into the repository 
#

########################################################################
## add version information to the tree

if [ -n "${1}" ]; then
   TARGET_BASE_DIR=../TARGET_TREES/ehs_env-$1
else
   echo "ERROR: No target directory name provided to create_version_info script"
   exit 	
fi

#test if we should create a new version number
if [ "${2}" == "INC_VERSION" ]; then
	INC_VERSION=yes
else
	INC_VERSION=no
fi


#We might be called without the SPECIFIC_TARGET VARIABLE SET so use the arg
if [ -z "${SPECIFIC_TARGET}" ]; then
	SPECIFIC_TARGET=$1
fi

# override path
if [ -n "${3}" ]; then
   TARGET_BASE_DIR=$3	
fi

echo adding Version Information to the tree
# This is done here (rather than earlier so that the TIMESTAMP identifier matches that in the repo
# try to use the same time stamp as the caller to be coherent with svn tag
if [ -z ${TIME_STAMP} ]; then
	TIME_STAMP=`date +%F-%Hh%Mm%Ss`
fi


#@TODO relies on SVN, change to GIT
if [ -n "${VERSION_NAME}" ] ;then
#@todo add a read prompt to request a version name.
	echo Version Name provided: $VERSION_NAME 
	#write line 1
	echo  "${VERSION_NAME}-" > $TARGET_BASE_DIR/sysdata/version.nfo
	git log -n 1| grep commit | sed 's/commit\s*\([0..9,a..f,A..F]*\)/\1/' >> $TARGET_BASE_DIR/sysdata/version.nfo
	## @todo Another script for uploading tarballs from the repo to server should be generated
else
		# Get the current version from the repo 
		echo "Getting the version information from the repo..."			
		
		#git pull ./Releases/version_string
		
		#git fetch
		#git checkout -m ./Releases/version_strings
		
		MAJORMAJOR_V=`sed -n 1p ./Releases/version_strings`
		MAJOR_V=`sed -n 2p ./Releases/version_strings`
		MINOR_V=`sed -n 3p ./Releases/version_strings`
		echo "got repo version ${MAJORMAJOR_V}.${MAJOR_V}.${MINOR_V}"
		if [ "${INC_VERSION}" == "yes" ];then
			MINOR_VV=`expr $MINOR_V + 1`
			echo "Updating repo file ./Releases/version_strings, with version  ${MAJORMAJOR_V}.${MAJOR_V}.${MINOR_VV}"
			echo  ${MAJORMAJOR_V} > ./Releases/version_strings
			echo ${MAJOR_V} >> ./Releases/version_strings
			echo ${MINOR_VV} >> ./Releases/version_strings
			#svn commit -m "Updating version string to ${MAJORMAJOR_V}.${MAJOR_V}.${MINOR_VV}" ./Releases/version_strings
			
			git add ./Releases/version_strings
			git commit -m "Checking in revision file for ${VERSION_NAME} with time stamp ${TIME_STAMP} (build=${TARGET})" ./Releases/version_strings
			git push
			
		else
			MINOR_VV=$MINOR_V	
		fi
#and finally write the data
mkdir -p $TARGET_BASE_DIR/sysdata/
echo  ${MAJORMAJOR_V}.${MAJOR_V}.${MINOR_VV} >  $TARGET_BASE_DIR/sysdata/version.nfo

fi


#write line 2
echo $TIME_STAMP >>  $TARGET_BASE_DIR/sysdata/version.nfo
#write line 3
#echo `svn info | grep Revision | sed 's/Revision:\s*\([0..9]*\)/\1/'` >> $TARGET_BASE_DIR/sysdata/version.nfo
git log -n 1| grep commit | sed 's/commit\s*\([0..9,a..f,A..F]*\)/\1/' >> $TARGET_BASE_DIR/sysdata/version.nfo
#write line 4
echo  "$SPECIFIC_TARGET" >> $TARGET_BASE_DIR/sysdata/version.nfo
#write line 5
echo  "EHS" >> $TARGET_BASE_DIR/sysdata/version.nfo
########################################################################

########################################################################
if [ "${INC_VERSION}" == "yes" ];then
VERSION_NAME="${MAJORMAJOR_V}.${MAJOR_V}.${MINOR_VV}-${TARGET}"
# Create in repo SVN tag and give this a proper name
#Checking in all code
echo "Creating Tag ${TIME_STAMP}-${VERSION_NAME} in the repository"
#svn commit -m "Creating TAG ${VERSION_NAME} for EHS system patch with time stamp ${TIME_STAMP}"
#svn copy svn://server/LucidSource/branches/INX/EHS svn://server/LucidSource/tags/EHS/${TIME_STAMP}-{$TARGET}-${VERSION_NAME} -m " Tag created for release ${VERSION_NAME} at ${TIME_STAMP}"

#git add ./Releases/version_string 
git commit -am "Adding full code for revision ${VERSION_NAME} with time stamp ${TIME_STAMP}"
git tag -a ${VERSION_NAME} -m "Build ${VERSION_NAME} - ${TIME_STAMP}"
# ./Releases/version_string
 
#write the version info into the target tree
echo Created Tag in svn repository
fi

########################################################################
