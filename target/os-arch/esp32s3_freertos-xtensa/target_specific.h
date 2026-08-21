/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target.h
 * In this file, all of the hardware abstraction layer functions provided for
 * EHS are given.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_SPECIFIC_H
#define EHS_TARGET_SPECIFIC_H


#ifndef EHS_GLOBALS_H
#error "This file should only be included by globals.h" 
#endif

#include <stdlib.h> /* required for malloc, exit */
#include <string.h>

//#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

#ifndef TS_TAG
#define TS_TAG "esp32_ehs"
#endif

#include "globals.h"

/**
 * Forces the declared variable to be aligned in the way specified by
 * EHS_TGT_MEMORY_ALLOC_ALIGN
 */
#define EHS_TGT_MEMORY_ALIGNED(type, name) type name

#ifndef EHSStdioPrintf
  #define EhsStdioPrintf(x, y, z, ...) {if (strcmp(z,"Error") == 0) ESP_LOGE(TS_TAG, x, y, z, __VA_ARGS__);else if (strcmp(z,"Warning") == 0) ESP_LOGW(TS_TAG, x, y, z, __VA_ARGS__);else if (strcmp(z,"Info") == 0) ESP_LOGI(TS_TAG, x, y, z, __VA_ARGS__);else ESP_LOGD(TS_TAG, x, y, z, __VA_ARGS__);}
  /* Was ESP_LOGD(...) — but EhsStdioSimplePrintf is the kernel-side TTY
   * path used by EhsKernelMessage / EhsParserError / etc. to surface
   * load + parse errors on the UART console. ESP_LOGD is silently dropped
   * at the default IDF runtime log level (INFO), so errors never reached
   * the TTY — only the debugger console saw them. Plain printf() matches
   * what every other target does (gnu_ALL, zephyr, nxp, xcore) and lands
   * the message on UART0 regardless of ESP log filter. */
  #define EhsStdioSimplePrintf(...)  printf(__VA_ARGS__)
#endif

/* math functions not implemented in Windows */

/**
 * Performs sine function using integers.
 *
 * @param nMult multiplier for output
 * @param nAngle angle to find (0 to 360)
 * @return nMult * sin(nAngle % 360)
 */
ehs_sint32 EhsTgtInt_sin(ehs_sint32 nMult, ehs_sint32 nAngle);
/**
 * Performs cosine function using integers.
 *
 * @param nMult multiplier for output
 * @param nAngle angle to find (0 to 360)
 * @return nMult * cos(nAngle % 360)
 */
ehs_sint32 EhsTgtInt_cos(ehs_sint32 nMult, ehs_sint32 nAngle);

ehs_float EhsTgtFloat_log10(ehs_float);
ehs_float EhsTgtFloat_loge(ehs_float);

#ifdef EHS_TARGET_CODE
/**
 * Initialise shared memory. Called by the main process and the tcpip process.
 *
 * @param[in] bUniqueCheck If true, check to ensure that this is the only
 * instance that currently holds the shared memory.
 */
void EhsTargetInitSharedMemory(ehs_bool bUniqueCheck);

#endif /* EHS_TARGET_CODE */

#ifdef EHS_I2C_SUPPORT 
ehs_bool EhsTI2CMasterInit( void );
#ifdef EHS_MAX31343_SUPPORT 
extern ehs_sint32 gEhsAmbientTemp;
void EhsTMax31343Init( void );
float EhsTMax31343GetTemperature( void );
ehs_sint32 EhsTMax31343GetTemperatureFP( void );
void EhsTMax31343GetRTC(ehs_uint8 *seconds, ehs_uint8 *minutes, ehs_uint8 *hours, ehs_uint8 *day, ehs_uint8 *date, ehs_uint8 *month, ehs_uint16 *year);
void EhsTMax31343SetYearMonthDateDay(ehs_uint16 year, ehs_uint8 month, ehs_uint8 date, ehs_uint8 day);
void EhsTMax31343SetHourMinuteSecond(ehs_uint8 hours, ehs_uint8 minutes, ehs_uint8 seconds);
#endif//EHS_MAX31343_SUPPORT 
#endif//EHS_I2C_SUPPORT 

#endif /* EHS_TARGET_SPECIFIC_H */
