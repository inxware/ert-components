/** @file utils.c
 * In this file, declaration for useful utilities for EHS are given
 * 
 * @author: inx limited
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_UTILS_H
#define EHS_UTILS_H
#include "ehs_types.h"

#define EHS_TO_LOWER(x) (x | 0x20) /**< Simple conversion from either case to lower case */

/**
 * Convert a value into uint16, saturating where limits are reached
 */
#define EHS_CONVERT_UINT16_SATURATE(x) (ehs_uint16)(x < 0?0:(((x) > EHS_UINT16_MAX))?EHS_UINT16_MAX:(x))

/**
 * Get current type from a string
 */
EHS_GLOBAL const ehs_char * EhsGetType(EhsDataTypeEnum * output, const ehs_char* input);

/**
 * Determine if character is a-z or A-Z
 */
EHS_GLOBAL ehs_bool EhsIsAlpha(ehs_char ch);

/**
 * Determine if character is a-z or A-Z or 0-9
 */
EHS_GLOBAL ehs_bool EhsIsAlNum(ehs_char ch);

/**
 * Read an unsigned 32-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * exceeds EHS_UINT32_MAX
 *
 * @todo make this into a general-purpose function (not parse-only). It's already used that way.
 * @todo Move console printf into parser
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
const ehs_char* EhsGetUint32FromString(ehs_uint32 * output, const ehs_char* input);

/**
 * Read a signed 32-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * falls outside EHS_SINT32_MIN .. EHS_SINT32_MAX
 * @todo Move console printf into parser
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
const ehs_char* EhsGetSint32FromString(ehs_sint32 * output, const ehs_char* input);

/**
 * Read a signed 16-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * falls outside EHS_SINT16_MIN .. EHS_SINT16_MAX
 * @todo Move console printf into parser
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
const ehs_char* EhsGetSint16FromString(ehs_sint16 * output, const ehs_char* input);

/**
 * Read an unsigned 16-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * exceeds EHS_UINT16_MAX
 * @todo Move console printf into parser
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer), or null if parse fails
 */
const ehs_char* EhsGetUint16FromString(ehs_uint16* output, const ehs_char* input);

/**
 * Read an unsigned 8-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * exceeds EHS_UINT8_MAX
 * @todo Move console printf into parser
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
const ehs_char* EhsGetUint8FromString(ehs_uint8* output, const ehs_char* input);

/**
 * Return a pointer to the character after the end of line character,
 * or null. The end of line character can be CR (0x0d), LF (0x0a) or CRLF.
 */
const ehs_char* EhsGetEol(const ehs_char* input);

#endif /* EHS_UTILS_H */
