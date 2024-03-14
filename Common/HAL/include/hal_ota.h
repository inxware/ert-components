/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file hal_ota.h
 * In this file, all of the hardware abstraction layer functions relating to OTA are given.
 *
 * @author: inx limited
 *
 */

#ifndef _HAL_OTA_H_
#define _HAL_OTA_H_

#include "target_types.h"

typedef enum {
    TARGET_OTA_IDLE = 0,
    TARGET_OTA_BEGAN,
} target_ota_state_t;

/**
 * @brief Internally defined function. Do not re-implement this in the target code!
 * This is called when the OTA write is done.
 * 
 * @param[in] errno Error number of the write ACK
 *
 */
void Common_OTA_Write_ACK(ehs_uint8 errno);

/**
 * @brief Begin the OTA update process
 * 
 * @param[in] alt_partition Whether the target OTA partition can be chosen
 * @param[in] partition_num The partition number of the OTA. Ignored if `alt_partition` is False.
 * @return
 *          - 0 if Success
 *          - 1 if Already began
 *          - 2 if OTA partition is not available
 *          - 3 if Memory inadaquate
 *          - 4 if Failed otherwise
 */
ehs_sint32 thOTA_begin(ehs_bool alt_partition, ehs_sint32 partition_num);

/**
 * @brief Pass the OTA data to the actual write process
 * 
 * @param[in] databuf   Array of data to be written
 * @param[in] size      Size of the data buffer
 * @param[in] offset    The offset of the buffer in the partition
 * @return
 *          - 0 if Success
 *          - 1 if OTA is writing
 *          - 2 if size <= 0
 *          - 3 if databuf is NULL
 *          - 4 if offset < 0
 *          - 5 if heap is invalid
 */
ehs_uint8 thOTA_write_passthrough(ehs_char * databuf, ehs_sint32 size, ehs_sint32 offset);

/**
 * @brief End the OTA process
 */
void thOTA_end(void);

/**
 * @brief Check the Checksum of the OTA image with the partition
 * 
 * @param[in] alt_partition Whether the target OTA partition can be chosen
 * @param[in] partition_num The partition number of hte OTA. Ignored if `alt_partition` is False.
 * @param[in] CS_token      The Checksum token to be checked with the parition
 * @param[in] CS_token_len  The length of the Checksum token
 * @return
 *          - EHS_TRUE  if CRC check matches
 *          - EHS_FALSE if CRC check fails
 */
ehs_bool thOTA_checkChecksum(ehs_bool alt_partition, ehs_sint32 partition_num, ehs_uint8 * CS_token, ehs_sint32 CS_token_len);

/**
 * @brief Check whether the written OTA image is valid on flash
 * 
 * @param[in] alt_partition Whether the target OTA partition can be chosen
 * @param[in] partition_num The partition number of hte OTA. Ignored if `alt_partition` is False.
 * @return
 *          - EHS_TRUE  if the OTA image is valid
 *          - EHS_FALSE if the OTA image is invalid
 */
ehs_bool thOTA_checkValid(ehs_bool alt_partition, ehs_sint32 partition_num);

/**
 * @brief Set the next boot partition to be the written OTA one
 * 
 * @param[in] alt_partition Whether the target OTA partition can be chosen
 * @param[in] partition_num The partition number of hte OTA. Ignored if `alt_partition` is False.
 * @return
 *          - EHS_TRUE  if switching success
 *          - EHS_FALSE if switching failed
 */
ehs_bool thOTA_switch(ehs_bool alt_partition, ehs_sint32 partition_num);

/**
 * @brief Abort the OTA update process
 */
void thOTA_abort(void);

#endif//_HAL_OTA_H_