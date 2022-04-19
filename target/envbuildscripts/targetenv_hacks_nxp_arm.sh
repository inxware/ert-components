#!/bin/bash

# Uplaod the NXP arm to MBED target

# Change the file extension
EHS_BIN_PATH=../TARGET_TREES/ehs_env-$TARGET/bin
EHS_BIN=$EHS_BIN_PATH/ehs
mv "$EHS_BIN_PATH/ehs.exe" "$EHS_BIN.$EXE"

# Convert target to bin file
OBJCOPY=$EHS_CORE_SUPPORT_BASE/toolchains/$TOOLCHAIN_PATH/bin/arm-none-eabi-objcopy
#arm-none-eabi-objcopy -v -O binary "${BuildArtifactFileName}" "${BuildArtifactFileBaseName}.bin"
$OBJCOPY -v -O binary "$EHS_BIN.$EXE" "$EHS_BIN.bin"

# Upload bin to target using MBED
MBED_PATH=/media/$USERNAME/MBED
echo "copying bin to MBED path ($MBED_PATH) ..."
cp "$EHS_BIN.bin" "$MBED_PATH/ehs.bin"
