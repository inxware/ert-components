#!/bin/bash

# 'linux_x86_64-lucid-debian11'

target_platform=( 
 'linux_amd64'
 'linux_amd64_gtk_gst'
 'linux_x86_64_clang'
 'linux_x86_64_clang_gtk'
 'linux_x86_64_clang_gtk_gst_gg_debian11'
 'linux_x86_64_gtk_gst_debian11'
 'linux_x86_64_clang_lvgl_debian11'
 'linux_armv7l_clang_debian10'
 'linux_arm64_gtk_gst_gg_debian10'
 'linux_arm64_lvgl_gg_debian10'
 'linux_arm64_lvgl_gg_debian11'
 'linux_arm64_gtk_gst_gg_debian11'
 'esp32_freertos-xtensor-base'
 'esp32s3_freertos-xtensa-base'
 'win_x86_gtk_gst'
 'win_x86_32-lucid-win10'
 'linux_android_arm'
)


#Parked targets
# 'linux_armv7l_clang_debian11'
# 'linux_x86_gtk_vlc'
# 'linux_x86_64_clang_gg_debian11'
# 'linux_armv7l_clang_gtk_gst_debian11'


if [ "$1" = "--include-slow" ]; then

# Long running ones...

target_platform=( 
 ${target_platform}
 'linux_android_arm64_unity-lib'
 )
elif [ "$1" = "--compile-only" ]; then
  COMPILE_ONLY=yes
fi

#set -e

if [ -d ../CI ]; then
	ROOT_DIR=$PWD/../../
else
	ROOT_DIR=$PWD
fi

CI_RESULT_DIR=${ROOT_DIR}/SystemTests/CI/results
TARGET_TREE_DIR=${ROOT_DIR}/../TARGET_TREES
SYSTEM_TESTS_DIR=${ROOT_DIR}/SystemTests/LucidTestApps/DefaultTestApp/export/
TOOL_TEST_EHS_VARIANT=linux_x86_gtk

function display_target {
	echo "***************** ${platform} ********************"
}

function build_target {
	if [ ! -d ../ert-components ]; then 
		echo "ERROR not in EHS directory, in ${PWD}. press 'y' to continue"
		read -n 1 WHAT
		if [ $WHAT != "y" ] ; then 
			echo ".... Exiting"
			exit
		fi

	fi
#       Don't do this so we can test branches in different adjascent directories
#	cd ${ROOT_DIR}/../ert-components #just to make sure!
	make clean &> /dev/null ||:
	./configure ${platform} &> ${CI_RESULT_DIR}/${platform}/build.log || echo "ERROR!!!!!!!!!!! platform not found" 
	make targetenv_cleanall  &>> ${CI_RESULT_DIR}/${platform}/build.log ||:
	touch ${CI_RESULT_DIR}/${platform}/build.started
#Docker build (or not sometimes if not configured) - we probably don't need to check these days as make all_docker will default to a hist build if not image is found
 	if [ -f ${ROOT_DIR}/target/platform/${platform}/Dockerimagename ];then
		make all_docker   &>> ${CI_RESULT_DIR}/${platform}/build.log && touch ${CI_RESULT_DIR}/${platform}/build.pass || touch ${CI_RESULT_DIR}/${platform}/build.fail
		if test -f ${CI_RESULT_DIR}/${platform}/build.pass
		then 
			make targetenv  &>> ${CI_RESULT_DIR}/${platform}/build.log && touch ${CI_RESULT_DIR}/${platform}/build-target.pass || touch ${CI_RESULT_DIR}/${platform}/build-target.fail 
		else
			touch ${CI_RESULT_DIR}/${platform}/build-target.notrun
		fi
	else
# Build on the Host - we don't generally want to do this, but during experimentation stages it is usually easier to build interactively without a prebuilt Dockerfimage.
		make -j 8  &>> ${CI_RESULT_DIR}/${platform}/build.log && touch ${CI_RESULT_DIR}/${platform}/build.pass || touch ${CI_RESULT_DIR}/${platform}/build.fail  
		if test -f ${CI_RESULT_DIR}/${platform}/build.pass
		then 
			make targetenv  &>> ${CI_RESULT_DIR}/${platform}/build.log && touch ${CI_RESULT_DIR}/${platform}/build-target.pass || touch ${CI_RESULT_DIR}/${platform}/build-target.fail 
		else
			touch ${CI_RESULT_DIR}/${platform}/build-target.notrun
		fi
	fi
if [ "$COMPILE_ONLY" != "yes" ]; then

#Check if we need to build a Unity Android project.
        if [[ "${platform}" == *"_unity"* ]] && [ "${platform}" != "linux_android_arm64_unity-lib" ]; then 
		echo "========= Building unity app for the target ============="
		make targetenv_unity_export &>> ${CI_RESULT_DIR}/${platform}/build.log
	fi
#check if this is Aandroid
	echo "Looking for android ehs_${platform}.so  ..."
	if test -f ehs_${platform}.so
	then
		echo "Found ehs_${platform}.so - building .apk ... "
		make targetenv_apk  &>> ${CI_RESULT_DIR}/${platform}/build.log && touch ${CI_RESULT_DIR}/${platform}/build-apk.pass || touch ${CI_RESULT_DIR}/${platform}/build-apk.fail
	fi
fi
	rm ${CI_RESULT_DIR}/${platform}/build.started

}

function test_build {
	if test -e ${TARGET_TREE_DIR}/ehs_env-${platform}/bin/ehs.exe
	then
		 touch ${CI_RESULT_DIR}/${platform}/exe-built.pass
	else
	    if test -f "ehs_${platform}.so"
	    then
		 touch ${CI_RESULT_DIR}/${platform}/exe-built.pass
	     else  
		touch ${CI_RESULT_DIR}/${platform}/exe-built.fail
	     fi
	fi
}

function remove_build {
	rm ${TARGET_TREE_DIR}/ehs_env-${platform}/bin/ehs.exe &>/dev/null ||: 
	rm ${TARGET_TREE_DIR}/ehs_env-${platform}/bin/*.so &>/dev/null ||: 
}

###################### Main ####################################
test -n "${CI_RESULT_DIR}" && mkdir -p "${CI_RESULT_DIR}" || exit
cd ${ROOT_DIR} || exit

if [ "${CI_RESULT_DIR}" = "" ]; then
echo "WARNING SOMETHING IS WRONG WITH YOUR DIRICTORIES!"
exit
else
rm -Rf ${CI_RESULT_DIR}/*
fi

for platform in "${target_platform[@]}"
do
 echo "===============================${platform}======================"
 if [  "${platform}" == ".svn" -o "${platform}" == "depricated" -o "`expr "${platform}" : "base.*"`" -gt 0 ]; then
	echo "Ignoring directory ${platform}"
 else
	echo "Building and testing ${platform}"
	mkdir -p "${CI_RESULT_DIR}/${platform}"
	rm -Rf    "${CI_RESULT_DIR}/${platform}/*" &> /dev/null
 #remove all flags
	touch "${CI_RESULT_DIR}/${platform}/building.flag"
	remove_build
	display_target
	build_target
if [ "$COMPILE_ONLY" != "yes" ]; then
	test_build
fi
	rm "${CI_RESULT_DIR}/${platform}/building.flag"
#	tree "${CI_RESULT_DIR}"
 fi
done
#Report summary
LS_COLORS_ORIG=$LS_COLORS
export LS_COLORS=${LS_COLORS_ORIG}"*.fail=00;31:*.pass=00;32:"
ls -l --color=always ${CI_RESULT_DIR}/*/
export LS_COLORS=$LS_COLORS_ORIG
