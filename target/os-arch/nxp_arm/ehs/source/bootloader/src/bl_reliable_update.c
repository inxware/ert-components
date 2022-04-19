/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "bootloader/bl_reliable_update.h"
#include "bootloader/bl_shutdown_cleanup.h"
#if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
#if !BL_DEVICE_IS_LPC_SERIES
#include "fsl_flash.h"
#else
#include "fsl_flashiap_wrapper.h"
#endif
#endif
#include "crc/crc32.h"
#include "utilities/fsl_assert.h"
#define BL_FEATURE_RELIABLE_UPDATE 1
#if BL_FEATURE_RELIABLE_UPDATE

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#if (BL_BACKUP_APP_START & (FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE - 1))
#error "The backup application start address must be sector-aligned!"
#endif

// image header for Cortex-M series
typedef struct
{
    uint32_t stackPointer;
    uint32_t applicationPointer;
} appliation_header_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
//! @brief Determine if the reliable update feature is active.
//static bool is_reliable_update_active(uint32_t backupApplicationBase);

//! @brief Determine if the specified application is valid
static bool is_specified_application_valid(uint32_t applicationBase);

//! @brief Update the property of reliable update status
static void update_reliable_update_status(uint32_t status);

//! @brief Get the start address of specified application
static uint32_t get_application_base(specified_application_type_t applicationType);

//! @brief Get the base address of Bootloader Config Area
//static uint32_t get_bootloader_config_area_base(uint32_t applicationBase);

//! @brief Get the maxmimum backup application size.
//static uint32_t get_max_backup_app_size(uint32_t address);

#if BL_IS_HARDWARE_SWAP_ENABLED

#if BL_TARGET_FLASH
//! @brief Determine if the backup bootloader is valid
//static bool is_backup_bootloader_valid(void);
#endif

//! @brief Do hardware reliable application update if backup application is valid
static status_t hardware_reliable_update(uint32_t swapIndicatorAddress);

//! @brief Get the swap indicator address from IFR if swap system is in ready state.
static status_t get_swap_indicator_address_if_system_is_in_ready(uint32_t *address);
#else
//! @brief Do software reliable application update if backup application is valid
static status_t software_reliable_update(uint32_t backupApplicationBase);
#endif // BL_IS_HARDWARE_SWAP_ENABLED

//! @brief Copy source appliction to destination application region and return result
static bool get_result_after_copying_application(uint32_t src, uint32_t dst, uint32_t len);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
#define HW_DIAG_printf(fmt,...) printf(fmt, ##__VA_ARGS__ )
#else // possibly a production build
//#define HW_DIAG_printf(fmt,...) printf(fmt, ##__VA_ARGS__ )
#define HW_DIAG_printf(fmt,...) {}
#endif


// See bl_reliable_update.h for documents on this function.
void bootloader_reliable_update_as_requested(reliable_update_option_t option, uint32_t address)
{
    // For software implementation, the option doesn't take effect, It always be kReliableUpdateOption_Swap.
    // For hardware implementation, the option works properly
    //uint32_t backupApplicationBase;
#if BL_IS_HARDWARE_SWAP_ENABLED
    uint32_t swapIndicatorAddress = address;
    if (option == kReliableUpdateOption_Normal)
    {
        uint32_t mainApplicationBase = get_application_base(kSpecifiedApplicationType_Main);
        if (is_specified_application_valid(mainApplicationBase))
        {
        	HW_DIAG_printf("Failed update swap - Still in main application\n");
        	update_reliable_update_status(kStatus_ReliableUpdateStillInMainApplication);
            return;
        }
        else
        {
            // Checking the flash swap status to see if it is in Ready state
            //  if true, get the swap indicator address from IFR by ReadResource API
            status_t result = get_swap_indicator_address_if_system_is_in_ready(&swapIndicatorAddress);
            if (result != kStatus_FLASH_Success)
            {
                HW_DIAG_printf("Failed to get the swap status register?\n");
            	update_reliable_update_status(kStatus_ReliableUpdateSwapSystemNotReady);
                return;
            }
        }
    }
    else {
    	HW_DIAG_printf("option != kReliableUpdateOption_Normal\n");
    }
    //backupApplicationBase = get_application_base(kSpecifiedApplicationType_Backup);
#else
    uint32_t backupApplicationBase = (address == 0) ? get_application_base(kSpecifiedApplicationType_Backup) : address;
#warning "Hardware flash swap is not enabled"
#endif // BL_IS_HARDWARE_SWAP_ENABLED

    // Below implementaion is for both kReliableUpdateOption_Normal and kReliableUpdateOption_Swap
    //if (!is_reliable_update_active(backupApplicationBase))
    if(0)
    {
        update_reliable_update_status(kStatus_ReliableUpdateInactive);
    }
    else
    {
        //if (is_specified_application_valid(backupApplicationBase))
    	if(1)
        {
#if BL_IS_HARDWARE_SWAP_ENABLED
        	status_t status = hardware_reliable_update(swapIndicatorAddress);
            // A system reset is needed here, or we can't guarantee the integrity of swap process.
            if ((option == kReliableUpdateOption_Swap) && (status == kStatus_ReliableUpdateSuccess))
            {
                // Prepare for shutdown.
            	HW_DIAG_printf("Preparing for shutdown\n");
                shutdown_cleanup(kShutdownType_Reset);

                NVIC_SystemReset();
                // Does not get here.
                assert(0);
            }
            else {
            	HW_DIAG_printf("Not doing shutdown\n");
            }
#else
            status_t status = software_reliable_update(backupApplicationBase);
#endif // BL_IS_HARDWARE_SWAP_ENABLED
            update_reliable_update_status(status);
        }
        else
        {
        	HW_DIAG_printf("Reliable updates staus is invalid\n");
            update_reliable_update_status(kStatus_ReliableUpdateBackupApplicationInvalid);
        }
    }
}

//! @brief Get the maxmimum backup application size.
/*static uint32_t get_max_backup_app_size(uint32_t address)
{
#if BL_TARGET_ROM
    return (g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.totalSize >> 1);
#elif BL_TARGET_FLASH
#if BL_IS_HARDWARE_SWAP_ENABLED
    return (g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.blockBase + (g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.totalSize >> 1) -
            BL_APP_VECTOR_TABLE_ADDRESS);
#else
    int32_t maxAppSize;
    int32_t maxBackupAppSize;

    maxAppSize = address - get_application_base(kSpecifiedApplicationType_Main);
    maxBackupAppSize =
        g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.blockBase + g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.totalSize - address;

    maxAppSize = ALIGN_DOWN(maxAppSize, g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.sectorSize);
    maxBackupAppSize = ALIGN_UP(maxBackupAppSize, g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.sectorSize);

    assert((maxAppSize > 0) && (maxBackupAppSize > 0));

    return (uint32_t)MIN(maxAppSize, maxBackupAppSize);
#endif
#else
#error "This Bootloader type is NOT supported!"
#endif
}*/

//! @brief Get the start address of specified application
static uint32_t get_application_base(specified_application_type_t applicationType)
{
    if (applicationType == kSpecifiedApplicationType_Main)
    {
#if BL_TARGET_ROM
        return g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.blockBase;
#elif BL_TARGET_FLASH
        return BL_APP_VECTOR_TABLE_ADDRESS;
#else
#error "This Bootloader type is NOT supported!"
#endif
    }
    else if (applicationType == kSpecifiedApplicationType_Backup)
    {
#if BL_TARGET_ROM
        return g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.blockBase + (g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.totalSize >> 1);
#elif BL_TARGET_FLASH
#if BL_IS_HARDWARE_SWAP_ENABLED
        return g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.blockBase + (g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.totalSize >> 1) +
               BL_APP_VECTOR_TABLE_ADDRESS;
#else
        return BL_BACKUP_APP_START;
#endif // #if BL_IS_HARDWARE_SWAP_ENABLED
#else
#error "This Bootloader type is NOT supported!"
#endif
    }

    return 0;
}

//! @brief Get the base address of Bootloader Config Area
/*static uint32_t get_bootloader_config_area_base(uint32_t applicationBase)
{
    return (applicationBase + 0x3c0);
}*/

//! A given jump address is considered valid if:
//! - Not 0x00000000
//! - Not 0xffffffff
//! - Not the reset handler entry point for the bootloader
//! - Is in flash or is in RAM or QuadSPI (if available)
//! @note this interface is also used by the configure_quadspi command
bool is_valid_application_location(uint32_t applicationAddress)
{
    const memory_map_entry_t *map;
    // Verify that the jumpLocation is non zero and then either within flash or RAM, both calculations are:
    // (jumpLocation >= startAddress) && (jumpLocation < (startAddress + size))
    if ((!applicationAddress) ||              // address is not null AND
        (applicationAddress == 0xffffffff)  // address is not blank Flash (0xff) AND
        //|| (applicationAddress == (uint32_t)&Reset_Handler) //TODO PBB I'm sure we don't care about this
		)
    {
        return false;
    }

    bool isValid = false;
    const uint32_t minThumb2InstructionSize = 2; // smallest thumb2 instruction size is 16-bit.
    // Check if the application address is in valid executable memory range
    status_t status = find_map_entry(applicationAddress, minThumb2InstructionSize, &map);
    if ((status == kStatus_Success) && (map->memoryProperty & kMemoryIsExecutable))
    {
        isValid = true;
    }

    return isValid;
}

// Determine if the reliable update feature is active
// Note : the reliable update feature is active only when following conditions are met:
//        1. the backup application is valid
//        2. the BCA is enabled.
/*static bool is_reliable_update_active(uint32_t backupApplicationBase)
{
	appliation_header_t *pAppHeader = (appliation_header_t *)backupApplicationBase;
#if BL_FEATURE_CRC_CHECK
    // The reliable udpate feature is active only when  and the BCA is enabled.
    uint32_t backupCrcChecksumBase = get_bootloader_config_area_base(backupApplicationBase);
    crc_checksum_header_t *pchecksumHeader = (crc_checksum_header_t *)backupCrcChecksumBase;
    if (is_valid_application_location(pAppHeader->applicationPointer) && (kPropertyStoreTag == pchecksumHeader->tag))
    {
        return true;
    }
    else
    {
        return false;
    }
#else
    if (is_valid_application_location(pAppHeader->applicationPointer))
	{
		return true;
	}
	else
	{
		return false;
	}
#endif
}*/

// Update the status for reliable update
static void update_reliable_update_status(uint32_t status)
{
    property_store_t *propertyStore = g_bootloaderContext.propertyInterface->store;
    propertyStore->reliableUpdateStatus = status;
}

// Determine if the application is valid.
// Note: the applicaiton is valid only if following conditions are met:
//       1. (backup image only)crcByteCount <= backup app start - BL_APP_VECTOR_TABLE_ADDRESS -
//       sizeof(header.expectedCrcValue).
//       2. crcStartAddress = BL_APP_VECTOR_TABLE_ADDRESS
//       3. The calculated crc checksum = expectedCrcValue
static bool is_specified_application_valid(uint32_t applicationBase)
{
#if BL_FEATURE_CRC_CHECK
	crc_checksum_header_t header;
    uint32_t crcChecksumBase = get_bootloader_config_area_base(applicationBase);
    uint32_t mainApplicationBase = get_application_base(kSpecifiedApplicationType_Main);

    memcpy(&header, (void *)crcChecksumBase, sizeof(header));

    // The size of the backup image must be less than or equal to maximumn reserved backup application space
    if (applicationBase != mainApplicationBase)
    {
        uint32_t maxBackupAppSize = get_max_backup_app_size(applicationBase);
        int32_t backupAppSize = header.crcByteCount;
        if (backupAppSize > maxBackupAppSize)
        {
            return false;
        }
    }
    // crcStartAddress must be BL_BACKUP_APP_START, and calculated crc checksum must be expectedCrcValue
    if (header.crcStartAddress != mainApplicationBase)
    {
        return false;
    }
    else
    {
        header.crcStartAddress = applicationBase;

        uint32_t calculatedCrc = calculate_application_crc32(&header, crcChecksumBase);
        if (calculatedCrc != header.crcExpectedValue)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
#else
    //PBB just ignore crc checks
    return true;
#endif
}

//! @brief Copy source appliction to destination application region and return result
static bool get_result_after_copying_application(uint32_t src, uint32_t dst, uint32_t len)
{
    bool updateResult = true;
    status_t status;

    // Erase the destination application region
    status = mem_erase(dst, len, kMemoryInternal);
    if (kStatus_Success != status)
    {
        updateResult = false;
    }
    else
    {
        uint32_t copyBuffer[8]; // Bufer used to hold the data to be written.
        uint32_t writeSize;
        uint32_t bufferSize = sizeof(copyBuffer);
        // Copy the source application to destination application region.
        while (len)
        {
            if (len >= bufferSize)
            {
                writeSize = bufferSize;
            }
            else
            {
                writeSize = len;
            }
            memcpy(copyBuffer, (uint8_t *)src, writeSize);
            status = mem_write(dst, writeSize, (uint8_t *)&copyBuffer[0], kMemoryInternal);
            if (kStatus_Success != status)
            {
                updateResult = false;
                break;
            }
            else
            {
                src += writeSize;
                dst += writeSize;
                len -= writeSize;
            }
        } // while(len)
    }
    return updateResult;
}

#if BL_IS_HARDWARE_SWAP_ENABLED

#if BL_TARGET_FLASH
//! @brief Determine if the backup bootloader is valid
/*static bool is_backup_bootloader_valid(void)
{
    uint32_t mainBootloaderCrc32;
    uint32_t backupBootloaderCrc32;
    uint32_t bootloaderSizeInByte =
        get_application_base(kSpecifiedApplicationType_Main) - g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.blockBase;
    uint32_t mainBootloaderBase = g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.blockBase;
    uint32_t backupBootloaderBase =
        g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.blockBase + (g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.totalSize >> 1);

    crc32_data_t crcInfo;
    crc32_init(&crcInfo);
    // Calculate crc for main bootloader
    crc32_update(&crcInfo, (uint8_t *)mainBootloaderBase, bootloaderSizeInByte);
    crc32_finalize(&crcInfo, &mainBootloaderCrc32);
    crc32_init(&crcInfo);
    // Calculate crc for backup bootloader
    crc32_update(&crcInfo, (uint8_t *)backupBootloaderBase, bootloaderSizeInByte);
    crc32_finalize(&crcInfo, &backupBootloaderCrc32);

    return (mainBootloaderCrc32 == backupBootloaderCrc32);
}*/
#endif

//! @brief Get the swap indicator address from IFR if swap system is in ready state.
static status_t get_swap_indicator_address_if_system_is_in_ready(uint32_t *address)
{
    ftfx_swap_state_config_t returnSwapInfo;
    status_t result =
        FTFx_CMD_SwapControl(&g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main], 0x10, kFTFx_SwapControlOptionReportStatus, &returnSwapInfo);
    *address = 0;
    if ((result == kStatus_FLASH_Success) && (returnSwapInfo.flashSwapState == kFTFx_SwapStateReady))
    {
        struct _flash_swap_ifr_field_config
        {
            uint16_t swapIndicatorAddress; /*!< A Swap indicator address field.*/
            uint16_t swapEnableWord;       /*!< A Swap enable word field.*/
            uint8_t reserved0[4];          /*!< A reserved field.*/
            uint8_t reserved1[2];          /*!< A reserved field.*/
            uint16_t swapDisableWord;      /*!< A Swap disable word field.*/
            uint8_t reserved2[4];          /*!< A reserved field.*/
        } flashSwapIfrFieldData;
        result = FLASH_ReadResource(g_bootloaderContext.allFlashState, g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].ifrDesc.resRange.pflashSwapIfrStart,
                                    (uint8_t *)&flashSwapIfrFieldData, sizeof(flashSwapIfrFieldData),
                                    kFTFx_ResourceOptionFlashIfr);
        if (result == kStatus_FLASH_Success)
        {
            *address = (uint32_t)flashSwapIfrFieldData.swapIndicatorAddress *
                       FSL_FEATURE_FLASH_PFLASH_SWAP_CONTROL_CMD_ADDRESS_ALIGMENT;

            return kStatus_FLASH_Success;
        }
    }
    // Set the result as kStatus_FTFx_CommandFailure temporary to inform upper layer
    return kStatus_FTFx_CommandFailure;
}

// Execute hardware reliable update
// There are 3 steps needed to complete the reliable update process:
//      1. Check if backup bootloader is valid, if true, copy main bootloader to backup bootloader region
//      2. Check if provided address is in valid swap indicator address range
//      3. Swap flash system
status_t hardware_reliable_update(uint32_t swapIndicatorAddress)
{
    bool updateResult = true;
    status_t status;

#if BL_TARGET_FLASH
    // Check if backup bootloader is valid
    //if (!is_backup_bootloader_valid())
    if(0)
    {
        uint32_t mainBootloaderBase = g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.blockBase;
        uint32_t backupBootloaderBase =
            g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.blockBase + (g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.totalSize >> 1);
        uint32_t bootloaderSizeInByte =
            get_application_base(kSpecifiedApplicationType_Main) - g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.blockBase;

        // Copy the Main Bootloader to Backup Bootloader region
        updateResult =
            get_result_after_copying_application(mainBootloaderBase, backupBootloaderBase, bootloaderSizeInByte);

        if (!updateResult)
        {
            update_reliable_update_status(kStatus_ReliableUpdateBackupBootloaderNotReady);
            return kStatus_ReliableUpdateBackupBootloaderNotReady;
        }
    }
#endif
    // Swap flash system
    HW_DIAG_printf("About to swap flash ....\n");
    status = FLASH_Swap(g_bootloaderContext.allFlashState, swapIndicatorAddress, true);
    if ((kStatus_FTFx_SwapIndicatorAddressError == status) || (kStatus_FTFx_AlignmentError == status))
    {
    	HW_DIAG_printf("Alignment Error.. returning\n");
    	update_reliable_update_status(kStatus_ReliableUpdateSwapIndicatorAddressInvalid);
        return kStatus_ReliableUpdateSwapIndicatorAddressInvalid;
    }
    else if (kStatus_FLASH_Success != status)
    {
    	HW_DIAG_printf("Some other Error (%d).. returning\n",status);
    	updateResult = false;
    }
    //HW_DIAG_printf("Returning with  (%d [%d is good]).. returning\n",status,kStatus_ReliableUpdateSuccess);
    return (updateResult) ? kStatus_ReliableUpdateSuccess : kStatus_ReliableUpdateFail;
}
#else
// Execute software reliable update
// There are 4 steps needed to complete the reliable update process:
//      1. Erase the application region
//      2. Copy the back applcation to the applicaion region
//      3. Do integrity check for the copied application
//      4. Erase the backup application
status_t software_reliable_update(uint32_t backupApplicationBase)
{
    bool updateResult = true;
    uint32_t applicationSizeInByte;

    uint32_t backupCrcChecksumBase = get_bootloader_config_area_base(backupApplicationBase);
    uint32_t mainApplicationBase = get_application_base(kSpecifiedApplicationType_Main);

    // Get actual length to be erased.
    crc_checksum_header_t header;
    memcpy(&header, (uint8_t *)backupCrcChecksumBase, sizeof(header));
    header.crcStartAddress = backupApplicationBase;

    applicationSizeInByte = header.crcByteCount;
    applicationSizeInByte = ALIGN_UP(applicationSizeInByte, g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main].flashDesc.sectorSize);

    // Copy the Backup Application to Main Appliction region
    updateResult =
        get_result_after_copying_application(backupApplicationBase, mainApplicationBase, applicationSizeInByte);

    // Erase the Backup Application region
    if (updateResult)
    {
        // Reload the user configuration data so that we can validate if the updated application is valid
        g_bootloaderContext.propertyInterface->load_user_config();

        if (!is_application_crc_check_pass())
        {
            updateResult = false;
        }
        else
        {
            status_t status = mem_erase(backupApplicationBase, applicationSizeInByte, kMemoryInternal);
            if (kStatus_Success != status)
            {
                updateResult = false;
            }
        } // if (!is_application_crc_check_pass())
    }     // if (updateResult)

    return (updateResult) ? kStatus_ReliableUpdateSuccess : kStatus_ReliableUpdateFail;
}
#endif // BL_IS_HARDWARE_SWAP_ENABLED

#endif // BL_FEATURE_RELIABLE_UPDATE
