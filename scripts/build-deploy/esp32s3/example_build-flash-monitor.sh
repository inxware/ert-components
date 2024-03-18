#!/bin/bash
# copyright inx limited UK 2010

# This script retrieves all support components built for the target type defined by the $OS and $ARCH
# This includes installing base host tools and the inxware dependency repos.
# This script should only be called via make prepdeps to setup the appropriate encironment variables.

set -e
./configure esp32_freertos-xtensor-base
make clean ||:
make all_docker
make targetenv_esp32
./esp32_flash.sh
./esp32_monitor_console.sh 