#!/usr/bin/env python3

import subprocess
import sys

def func(input):
    a = input.replace("\n", "").split()
    for i in a:
        temp = subprocess.run(f"../../inxware/ert-build-support/toolchains/x86_64/xtensa-esp32-elf-4.4.1/bin/xtensa-esp32-elf-addr2line pfiaC -e ../../inxware/TARGET_TREES/ehs_env-esp32_freertos-xtensor-base/bin/ehs.exe {i}", shell=True, check=True)

if __name__ == '__main__':
    func(sys.argv[1])
