/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "fsl_flash.h"
#include "memory/src/flash_memory.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

const flash_driver_interface_t g_flashDriverInterface = {
    .version = {.name = kFLASH_DriverVersionName,
                .major = kFLASH_DriverVersionMajor,
                .minor = kFLASH_DriverVersionMinor,
                .bugfix = kFLASH_DriverVersionBugfix },
    .flash_init = FLASH_Init,
    .flash_erase_all = FLASH_EraseAll,
#if BL_FEATURE_ERASEALL_UNSECURE
    .flash_erase_all_unsecure = FLASH_EraseAllUnsecure,
#else
    .flash_erase_all_unsecure = NULL,
#endif
    .flash_erase = FLASH_Erase,
    .flash_program = FLASH_Program,
    .flash_get_security_state = FLASH_GetSecurityState,
    .flash_security_bypass = FLASH_SecurityBypass,
    .flash_verify_erase_all = FLASH_VerifyEraseAll,
    .flash_verify_erase = FLASH_VerifyErase,
    .flash_verify_program = FLASH_VerifyProgram,
    .flash_get_property = FLASH_GetProperty,
#if !BL_FEATURE_MIN_PROFILE
    .flash_program_once = FTFx_CMD_ProgramOnce,
    .flash_read_once = FTFx_CMD_ReadOnce,
#if defined(FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD) && FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD
    .flash_read_resource = FLASH_ReadResource,
#else
    .flash_read_resource = NULL,
#endif
#else
    .flash_program_once = NULL,
    .flash_read_once = NULL,
    .flash_read_resource = NULL,
#endif
    .flash_is_execute_only = FLASH_IsExecuteOnly,
#if BL_FEATURE_FAC_ERASE
    .flash_erase_all_execute_only_segments = FLASH_EraseAllExecuteOnlySegments,
    .flash_verify_erase_all_execute_only_segments = FLASH_VerifyEraseAllExecuteOnlySegments,
#else
    .flash_erase_all_execute_only_segments = NULL,
    .flash_verify_erase_all_execute_only_segments = NULL,
#endif
#if BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED
#if FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD
    .flash_set_flexram_function = FLASH_SetFlexramFunction,
#else
    .flash_set_flexram_function = NULL,
#endif
    .flash_program_section = FLASH_ProgramSection,
#else
    .flash_set_flexram_function = NULL,
    .flash_program_section = NULL,
#endif
};



//! @brief Bootloader global context data.
//!
//! @ingroup context
bootloader_context_t g_bootloaderContext = {.memoryInterface = &g_memoryInterface,
                                            .memoryMap = g_memoryMap,
#if BL_FEATURE_EXPAND_MEMORY
                                            .externalMemoryMap = g_externalMemoryMap,
#endif // BL_FEATURE_EXPAND_MEMORY
                                            //.allPeripherals = g_peripherals,
                                            //.activePeripheral = NULL, // Filled in at run time.
                                            .propertyInterface = &g_propertyInterface,
                                            //.commandInterface = &g_commandInterface,
#if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
                                            .flashDriverInterface = &g_flashDriverInterface,
#if !BL_DEVICE_IS_LPC_SERIES
                                            .allFlashState = &g_flashState,
                                            .allFlashCacheState = &g_flashcacheState,
#if BL_FEATURE_SUPPORT_DFLASH
                                            .dflashDriverInterface = &g_dflashDriverInterface,
                                            .dFlashState = &g_dflashState,
#endif // BL_FEATURE_SUPPORT_DFLASH
                                            
#else
                                            .allFlashState = g_flashiapState,
#endif // !BL_DEVICE_IS_LPC_SERIES
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
#if AES_SECURITY_SUPPORTED
                                            .aesInterface = &g_aesInterface,
#endif

};
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
