#!/bin/bash

@TODO finish me and move me to scripts/build-deploy/ or delete me.

foreach ./target/platform

make clean


./configure $1

make -j$(nproc)

make targetenv # note does this have a tartenv clean option?? Use it if so

cp -Rf ../SystemTests/StartTest/export/* ../TARGET_TREES/$TARGET_PATH/app_data/

make sys


