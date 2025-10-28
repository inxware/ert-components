
/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * @file devman_mon_ota.h
 *
 * @author: inx limited
 *
 */

#ifndef COMMON_HAL_DEVMAN_MON_OTA_H_
#define COMMON_HAL_DEVMAN_MON_OTA_H

#include "globals.h"

#define DEVMAN_MON_OTA_CHUNK_SIZE 240
#define DEVMAN_MON_OTA_SHA256_BUFF_SIZE (64 + 1)
#define DEVMAN_MON_OTA_PARTITION_NUMBER 0
#define DEVMAN_MON_OTA_PARTITION_ALT EHS_FALSE

#define DEVMAN_MON_OTA_NO_ERROR 0
#define DEVMAN_MON_OTA_WRITE_IN_PROGRESS 1

typedef void (*EhsOtaDevmanMonNextCallback)(const ehs_char* payload);

/* Initalise devman mon ota handler and return true on success */
ehs_bool EhsOtaDevmanMonSupportInit(EhsOtaDevmanMonNextCallback callback);

/* Start devman mon OTA process */
ehs_bool EhsOtaDevmanMonStart(const ehs_char* infoData, ehs_uint32 infoDataSize);

/* Return true if OTA process has started */
ehs_bool EhsOtaDevmanMonStarted();

/* Writes raw data to devman mon OTA process */
ehs_bool EhsOtaDevmanMonWrite(const ehs_char* rawData, ehs_uint32 rawDataSize);

/* Abort devman mon OTA process */
ehs_bool EhsOtaDevmanMonAbort();

/* Used for processing any pending states of the OTA process */
void EhsOtaDevmanMonProcess();

#endif /* COMMON_HAL_DEVMAN_MON_OTA_H */