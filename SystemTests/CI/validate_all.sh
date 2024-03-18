#!/bin/bash

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
		echo "ERROR not in EHS directory, in ${PWD}"
		exit
	fi
	cd ${ROOT_DIR}/../ert-components #just to make sure!
	make clean
	rm -f *.o
	rm -f *.obj
	./configure ${platform}
	make targetenv_cleanall
	touch ${CI_RESULT_DIR}/${platform}/build
	make prepdeps 		
	make all_docker  && touch ${CI_RESULT_DIR}/${platform}/build.pass || touch ${CI_RESULT_DIR}/${platform}/build.fail 
	make targetenv && touch ${CI_RESULT_DIR}/${platform}/build-target.pass || touch ${CI_RESULT_DIR}/${platform}/build-target.fail 
}

function test_build {
	if test -e ${TARGET_TREE_DIR}/ehs_env-${platform}/bin/ehs.exe
	then
		 touch ${CI_RESULT_DIR}/${platform}/exe-built.pass
	else
	     if test -e ${TARGET_TREE_DIR}/ehs_env-${platform}/bin/ehs.exe
	    then
		 touch ${CI_RESULT_DIR}/${platform}/exe-built.pass
	     else  
		touch ${CI_RESULT_DIR}/${platform}/exe-built.fail
	     fi
	fi
}

function remove_build {
	rm ${TARGET_TREE_DIR}/ehs_env-${platform}/bin/ehs.exe ||: 
}

function test_run_apps {
 	pushd .
	cp -f ${SYSTEM_TESTS_DIR}/* ${TARGET_TREE_DIR}/ehs_env-${platform}/appdata/ || return 
	rm -f "${TARGET_TREE_DIR}/ehs_env-${platform}/userdata/test.pass" # shouldn't be needed - but just in case
	cd ${TARGET_TREE_DIR}/ehs_env-${platform}/bin || return
	./run_ehs.sh NO_RESTART LIB_HOST & # run usinghost libraries for complete functionality.
	for (( i = 1 ; i <= 10 ; i++ ))
	do
		sleep 6
		test -n "`pidof -s ehs.exe`" || break
	done
	if [ -n "`pidof -s ehs.exe`" ];then
		touch ${CI_RESULT_DIR}/${platform}/exe-host-run-app.timedout
	else 
		test -e "${TARGET_TREE_DIR}/ehs_env-${platform}/userdata/test.pass" && touch ${CI_RESULT_DIR}/${platform}/exe-host-run-app.pass ||touch ${CI_RESULT_DIR}/${platform}/exe-host-run-app.fail
	fi
	./restart.sh #this kills ehs - if it's still running
	popd
}
#remove all results

###################### Main ####################################
test -n "${CI_RESULT_DIR}" && mkdir -p "${CI_RESULT_DIR}" || exit
cd ${ROOT_DIR}/../ert-components || exit

rm -Rf ${CI_RESULT_DIR}/*
rm -Rf ${CI_RESULT_DIR}/*/*

for platform in `find ./target/platform/* -maxdepth 0 -type d  -printf "%f\n"`
do
 echo "===============================${platform}======================"
 if [  "${platform}" == ".svn" -o "${platform}" == "depricated" -o "`expr "${platform}" : "base.*"`" -gt 0 ]; then
 #if [ ("${platform}" = ".svn") -o (expr "${platform}" : "base.*") ]; then 
	echo "Ignoring directory ${platform}"
 else
	echo "Building and testing ${platform}"
	mkdir -p "${CI_RESULT_DIR}/${platform}"
	rm -Rf    "${CI_RESULT_DIR}/${platform}/*"
 #remove all flags
	touch "${CI_RESULT_DIR}/${platform}/building.flag"
	remove_build
	display_target	
	build_target
	test_build
	if [ "`expr "${platform}" : "linux_x86.*"`" -gt 0 ];then
		test_runs_empty_linux
		test_run_apps
	fi
	if [ "`expr "${platform}" : "${TOOL_TEST_EHS_VARIANT}"`" -gt 0 ];then
		test_runs_empty_linux
		test_run_apps
	fi
	
	rm "${CI_RESULT_DIR}/${platform}/building.flag"
#	tree "${CI_RESULT_DIR}"
 fi
done

