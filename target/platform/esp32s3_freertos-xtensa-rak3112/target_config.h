/** @file target_config.h
 * Target config for RAK3112 (ESP32-S3 + SX1262).
 * Inherits n8r8 base (PSRAM, display dimensions) unchanged — the RAK3112
 * module doesn't dictate display/touch pinouts, those are set by the host
 * board that hosts the module.
 */

#ifndef TARGET_CONFIG_RAK3112_H
#define TARGET_CONFIG_RAK3112_H

#include "../esp32s3_freertos-xtensa-base_n8r8/target_config.h"

#endif /* TARGET_CONFIG_RAK3112_H */
