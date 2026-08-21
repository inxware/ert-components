#!/bin/env bash

set -e

#make prepdeps
make targetenv_version

make clean
make targetenv_prebuild
make targetenv_littlefs
make all_docker
make targetenv_esp32_docker
make targetenv_upload_ota
