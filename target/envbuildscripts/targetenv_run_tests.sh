#!/bin/bash

export SPECIFIC_TARGET=$1

echo "####################################################################"
echo "#################### Run regresion tests ###########################"
echo "####################################################################"

TEST_MAX_WAIT=10 # sec - max wait for test to compleate
TEST_ERT_VERSION=ert1
TEST_ROOT=./tests/root
TEST_RESULTS_ROOT=../TARGET_TREES/TEST_RESULTS
TEST_RESULTS=${TEST_RESULTS_ROOT}/results
TEST_TARGET=${TEST_RESULTS_ROOT}/ehs_env-${SPECIFIC_TARGET}
TEST_USERDATA=${HOME}/inxware/inx-tests
TEST_USERDATA_RESULTS=${TEST_USERDATA}/results

function ErrorPrint() {
    printf "\e[31m%s\e[0m" "$1"
}

function SuccessPrint() {
    printf "\e[32m%s\e[0m" "$1"
}

function SetupTest() {
    TEST_DIR=$1
    TEST_NAME=$2
    # clear appdata and copy test app to default
    rm -rf ${TEST_TARGET}/appdata/*
    cp -r ${TEST_DIR}/export-${TEST_ERT_VERSION} ${TEST_TARGET}/appdata/default

    # re-set userdata tests directory
    test -d ${TEST_USERDATA} && rm -rf ${TEST_USERDATA}
    mkdir -p ${TEST_USERDATA}

    # create results dir for this test and copy expected results
    mkdir -p ${TEST_RESULTS}/${TEST_NAME}
    cp ${TEST_DIR}/test_result.txt ${TEST_RESULTS}/${TEST_NAME}/expected_result.txt
}

function ExecTest() {
    TEST_DIR=$1
    TEST_NAME=$2
    
    # create and run eRT process
    {
        pushd ${TEST_TARGET}/bin > /dev/null
        ./ehs.exe > ../../results/${TEST_NAME}/test_stdout.txt
        popd > /dev/null
    }&
    
    # poll for test compleate
    count=0
    timeout=$TEST_MAX_WAIT # @TODO - create custom timeout file
    while true; do
        sleep 1
        # check if test has compleated and copy results
        if [ -f "${TEST_USERDATA_RESULTS}/test_done" ]; then
            cp ${TEST_USERDATA_RESULTS}/test_result.txt ${TEST_RESULTS}/${TEST_NAME}/test_result.txt
            break
        fi
        if [ $count -gt $timeout ]; then
            # create test time out flag
            touch ${TEST_RESULTS}/${TEST_NAME}/timeout
            break
        fi
        ((count++))
    done
    # close eRT app
    pkill ehs.exe

    # check results
    if [ -f "${TEST_RESULTS}/${TEST_NAME}/timeout" ]; then
        ErrorPrint "TIMEOUT"
    elif [ ! -f "${TEST_RESULTS}/${TEST_NAME}/test_result.txt" ]; then
        ErrorPrint "TEST RESULT NOT AVAILABLE"
    elif [ ! -f "${TEST_RESULTS}/${TEST_NAME}/expected_result.txt" ]; then
        ErrorPrint "TEST EXPECTED RESULT NOT AVAILABLE"
    else
        if diff "${TEST_RESULTS}/${TEST_NAME}/test_result.txt" "${TEST_RESULTS}/${TEST_NAME}/expected_result.txt" >/dev/null; then
            touch ${TEST_RESULTS}/${TEST_NAME}/passed
            SuccessPrint "PASSED"
        else
            ErrorPrint "FAILED"
        fi
    fi
    echo "" # new line
}

function RunTests() {
    SUB_TEST=$1
    echo "===================================================================="
    echo "Run tests : ${SUB_TEST}"
    for dir in ${TEST_ROOT}/${SUB_TEST}/*/
    do
        dir=${dir%*/}
        name="${dir##*/}"
        if [ ${name} = "*" ]; then
            # do not process empty directory
            echo "No test found!"
            continue
        fi
        echo "--------------------------------------------------------------------"
        printf "%-30s" "${name}"
        SetupTest "${dir}" ${name}
        ExecTest "${dir}" ${name}
        echo "--------------------------------------------------------------------"
    done
}

# re-set test results directory
test -d ${TEST_RESULTS_ROOT} && rm -rf ${TEST_RESULTS_ROOT}
mkdir -p ${TEST_RESULTS_ROOT}

# copy target env to the test results directory
cp -r ../TARGET_TREES/ehs_env-${SPECIFIC_TARGET} ${TEST_TARGET}

# run all 'core' tests
RunTests core
# run all 'network' tests
RunTests network
# run all 'ctrl_system' tests
RunTests ctrl_system

echo "===================================================================="