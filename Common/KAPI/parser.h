/** @file parser.h
 * The SODL parser is declared in this file.
 * 
 * @author: inx limited
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_PARSER_H
#define EHS_PARSER_H
#include "ehs_types.h"

/**
 * Parse the specified SODL file. Failure to parse file leads to application data structures
 * being left empty.
 *
 * @param filename Name of file to read SODL file from.
 * @return true if successful (new application has loaded).
 */
EHS_GLOBAL ehs_bool EhsKP_parse(const char* filename);

/**
 * Read a word from a line of a SODL file. A word is a sequence of characters that do not include whitespace.
 *
 * @param output Word read from input.
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
EHS_GLOBAL const char* EhsGetWordFromString(char * output, const char* input);

/**
 * Read an unsigned 32-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * exceeds EHS_UINT32_MAX
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
EHS_GLOBAL const char* EhsGetUint32FromString(ehs_uint32 * output, const char* input);

/**
 * Read a signed 32-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * falls outside EHS_SINT32_MIN .. EHS_SINT32_MAX
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
EHS_GLOBAL const char* EhsGetSint32FromString(ehs_sint32 * output, const char* input);

/**
 * Read an unsigned 16-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * exceeds EHS_UINT16_MAX
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
EHS_GLOBAL const char* EhsGetUint16FromString(ehs_uint16 * output, const char* input);

/**
 * Read an unsigned 8-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * exceeds EHS_UINT8_MAX
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
EHS_GLOBAL const char* EhsGetUint8FromString(ehs_uint8* output, const char* input);

#endif

