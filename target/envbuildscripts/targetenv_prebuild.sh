#!/bin/bash

####################################################################################################
# This file script will run everything that is needed before building traget.
####################################################################################################

set -e

TARGET=$1

echo "************ targetenv prebuild ************"

# run targetenv before binary is built
./target/envbuildscripts/targetenv.sh ${TARGET} --skip-bin-cpy