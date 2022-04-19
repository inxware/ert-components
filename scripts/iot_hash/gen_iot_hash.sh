#!/bin/bash

id=$1
len=$2
seed=$3
genIotHashExe="gen_iot_hash"
if [ -f ${genIotHashExe} ]; then
  rm ${genIotHashExe}
fi
gcc ./gen_iot_hash.c -o gen_iot_hash

./gen_iot_hash $id $len $seed
