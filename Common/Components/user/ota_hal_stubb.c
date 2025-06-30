/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/*
 * target_ota.c will be selected for platforms with EHS_OTA_SUPPORT=EHS_OTA_SUPPORT_STUBBED
 * OTA operations are NOPs in this mode to allow for application compatability 
 * where OTA functions are optional or for target specific purposes. 
 */
#include "ehs_types.h"
#include "hal_ota.h"

/**
 * @brief OTA process current state
 */
target_ota_state_t thOTA_current_state(void)
{
    return TARGET_OTA_IDLE;
}

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
ehs_sint32 thOTA_begin(ehs_bool alt_partition, ehs_sint32 partition_num)
{
    return 0;
}

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
ehs_uint8 thOTA_write_passthrough(ehs_char * databuf, ehs_sint32 size, ehs_sint32 offset)
{
    return 0;
}

/**
 * @brief End the OTA process
 */
void thOTA_end(void)
{
    return;
}

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
ehs_bool thOTA_checkChecksum(ehs_bool alt_partition, ehs_sint32 partition_num, ehs_char * CS_token, ehs_sint32 CS_token_len)
{
    return EHS_TRUE;
}

/**
 * @brief Check whether the written OTA image is valid on flash
 * 
 * @param[in] alt_partition Whether the target OTA partition can be chosen
 * @param[in] partition_num The partition number of hte OTA. Ignored if `alt_partition` is False.
 * @return
 *          - EHS_TRUE  if the OTA image is valid
 *          - EHS_FALSE if the OTA image is invalid
 */
ehs_bool thOTA_checkValid(ehs_bool alt_partition, ehs_sint32 partition_num)
{
    return EHS_TRUE;
}

/**
 * @brief Set the next boot partition to be the written OTA one
 * 
 * @param[in] alt_partition Whether the target OTA partition can be chosen
 * @param[in] partition_num The partition number of hte OTA. Ignored if `alt_partition` is False.
 * @return
 *          - EHS_TRUE  if switching success
 *          - EHS_FALSE if switching failed
 */
ehs_bool thOTA_switch(ehs_bool alt_partition, ehs_sint32 partition_num)
{
    return EHS_TRUE;
}

/**
 * @brief Abort the OTA update process
 */
void thOTA_abort(void)
{
    return;
}

/**
 * @brief Set the OTA on abort callback
 */
void thOTA_on_abort_callback(target_ota_on_abort_t callback)
{
    return;
}

/**
 * @brief Force the OTA process state to IDLE
 */
void thOTA_idle(void)
{
    return;
}