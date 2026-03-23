/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file hal_peripheral_errors.h
 * @brief Standard error codes for eRT peripheral function blocks.
 *
 * All peripheral function blocks that expose an error_id output port use
 * these codes.  They are returned by HAL functions (EhsTWatchdogEnable2,
 * EhsTAccelGyroEnable, etc.) and passed directly to error_id.
 *
 * The sysfs-specific code (-2) allows application logic on Sfera Labs
 * targets to distinguish a transient I/O error from a permanently
 * unsupported board feature (-3).
 */

#ifndef _HAL_PERIPHERAL_ERRORS_H
#define _HAL_PERIPHERAL_ERRORS_H

/** @defgroup ehs_periph_errors Peripheral error codes
 *  @{
 */
typedef enum
{
    EHS_PERIPH_OK              =  0, /**< Success.                                              */
    EHS_PERIPH_ERR_GENERAL     = -1, /**< Unspecified failure.                                  */
    EHS_PERIPH_ERR_SYSFS       = -2, /**< sysfs I/O error — node not accessible or I/O failed.  */
    EHS_PERIPH_ERR_NOT_SUPPORTED = -3, /**< Feature not available on this board/platform.       */
} ehs_periph_error_t;
/** @} */

#endif /* _HAL_PERIPHERAL_ERRORS_H */
