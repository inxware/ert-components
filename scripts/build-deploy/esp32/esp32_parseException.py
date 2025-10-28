#!/usr/bin/env python3

#if  ../../inxware ] ;then
#echo "This script must be run under docker"
#exit
#fi



import subprocess
import sys

def func(input):
    a = input.replace("\n", "").split()
    for i in a:
        if 1 == 1:
            temp = subprocess.run(f"../../inxware/ert-build-support/toolchains/x86_64/xtensa-esp32-elf-4.4.4/bin/xtensa-esp32s3-elf-addr2line pfiaC -e ../../inxware/TARGET_TREES/ehs_env-esp32s3_freertos-xtensor-base/bin/ehs.exe {i}", shell=True, check=True)
        else:
            temp = subprocess.run(f"../../inxware/ert-build-support/toolchains/x86_64/xtensa-esp32-elf-4.4.1/bin/xtensa-esp32-elf-addr2line pfiaC -e ../../inxware/TARGET_TREES/ehs_env-esp32_freertos-xtensor-base/bin/ehs.exe {i}", shell=True, check=True)

print(f"Warning: This is using fixed path to binaries - look inside and fiddle withthis script to make it work for other toolchains\n!!! ../../inxware/TARGET_TREES/ehs_env-esp32s3_freertos-xtensor-base/bin/ehs.exe")

if __name__ == '__main__':
    func(sys.argv[1])
