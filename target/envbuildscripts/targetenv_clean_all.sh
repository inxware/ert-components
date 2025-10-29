#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2008-2025, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------

source ./scripts/build-function-library/colour.sh

#stop any crontabs on this machine that will create guff. Too brutal? ... Sorry.

if [[ ! -n ${1} ]]; then
    echo
    err "No target path is defined '${1}' - exiting"
    exit 1
fi

SPECIFIC_TARGET=$1;
#echo Creating Target path same as source target path =$SPECIFIC_TARGET

# remove any logs from debug versions etc.
echo "Delete target trees at '../TARGET_TREES/ehs_env-$SPECIFIC_TARGET'"
rm -Rf ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET
