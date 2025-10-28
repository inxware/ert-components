#!/bin/bash
set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
source "${SCRIPT_DIR}/../../../target/envbuildscripts/targetenv_esp32_sdk-packager_paths.sh"
# TOOLCHAIN_FLASHING_VERSION
# CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

source "${SCRIPT_DIR}/../../../target/envbuildscripts/targetenv_esp32_sdk-packager_paths.sh"

source TARGET.cfg
ELF_FILE=ehs_${TARGET}.elf

if ! [ -f "${ELF_FILE}" ]; then
    echo "File ${ELF_FILE} doesn't exist."
    echo "Make sure to build esp32 project before running this script."
    exit 1
fi

# setup env for esp tools
source ../ert-contrib-middleware/contrib/esp-idf/${CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION}/export.sh

# ESP32-S3 Memory Limits (Adjust if needed)
TOTAL_IRAM=$((128 * 1024))  # ESP32-S3 has ~128 KB IRAM
TOTAL_DRAM=$((320 * 1024))  # ESP32-S3 has ~320 KB DRAM

# Function to convert hex to decimal and human-readable format
convert_size() {
    local size_hex="$1"
    local size_dec=$((16#$size_hex))  # Convert hex to decimal

    if ((size_dec >= 1048576)); then
        echo "$((size_dec / 1048576)) MB"
    elif ((size_dec >= 1024)); then
        echo "$((size_dec / 1024)) KB"
    else
        echo "$size_dec Bytes"
    fi
}

# Function to calculate remaining RAM
calculate_remaining() {
    local used="$1"
    local total="$2"
    local free=$((total - used))
    echo "$free"
}

# Extract IRAM & DRAM section sizes from the ELF file
iram_vectors=$(xtensa-esp32-elf-objdump -h "$ELF_FILE" | awk '/\.iram0.vectors/ {print $3}')
iram_text=$(xtensa-esp32-elf-objdump -h "$ELF_FILE" | awk '/\.iram0.text[^_]/ {print $3}')
iram_text_end=$(xtensa-esp32-elf-objdump -h "$ELF_FILE" | awk '/\.iram0.text_end/ {print $3}')
iram_data=$(xtensa-esp32-elf-objdump -h "$ELF_FILE" | awk '/\.iram0.data/ {print $3}')
iram_bss=$(xtensa-esp32-elf-objdump -h "$ELF_FILE" | awk '/\.iram0.bss/ {print $3}')

dram_data=$(xtensa-esp32-elf-objdump -h "$ELF_FILE" | awk '/\.dram0.data/ {print $3}')
dram_bss=$(xtensa-esp32-elf-objdump -h "$ELF_FILE" | awk '/\.dram0.bss/ {print $3}')

# Convert hex sizes to decimal (handle empty values safely)
iram_vectors=$((16#${iram_vectors:-0}))
iram_text=$((16#${iram_text:-0}))
iram_text_end=$((16#${iram_text_end:-0}))
iram_data=$((16#${iram_data:-0}))
iram_bss=$((16#${iram_bss:-0}))

dram_data=$((16#${dram_data:-0}))
dram_bss=$((16#${dram_bss:-0}))

# Calculate total used memory
iram_total=$((iram_vectors + iram_text + iram_text_end + iram_data + iram_bss))
dram_total=$((dram_data + dram_bss))

# Calculate remaining RAM
iram_free=$(calculate_remaining $iram_total $TOTAL_IRAM)
dram_free=$(calculate_remaining $dram_total $TOTAL_DRAM)

# Print results
echo "ESP32-S3 Memory Usage Summary for $ELF_FILE:"
echo "----------------------------------"
echo "IRAM Vectors  : $(convert_size $(printf '%x' $iram_vectors))"
echo "IRAM Text     : $(convert_size $(printf '%x' $iram_text))"
echo "IRAM Text End : $(convert_size $(printf '%x' $iram_text_end))"
echo "IRAM Data     : $(convert_size $(printf '%x' $iram_data))"
echo "IRAM BSS      : $(convert_size $(printf '%x' $iram_bss))"
echo "----------------------------------"
echo "Total IRAM Used: $(convert_size $(printf '%x' $iram_total))"
echo "Remaining IRAM : $(convert_size $(printf '%x' $iram_free))"
echo "----------------------------------"
echo "DRAM Data      : $(convert_size $(printf '%x' $dram_data))"
echo "DRAM BSS       : $(convert_size $(printf '%x' $dram_bss))"
echo "----------------------------------"
echo "Total DRAM Used: $(convert_size $(printf '%x' $dram_total))"
echo "Remaining DRAM : $(convert_size $(printf '%x' $dram_free))"
echo "----------------------------------"

