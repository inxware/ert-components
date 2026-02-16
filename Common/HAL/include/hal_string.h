/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_string.h
 * In this file, all of the hardware abstraction layer declarations relating to string handling are given.
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section hal_string_h
 * @anchor hal_string_h
 * @subsection misra MISRA compliance:
 * Demonstrated MISRA compliant on 30/11/7
 * Last modified on $Date:$
 *
 * This file contained the following derogations to the MISRA standard.
 * - Rule 16.1 Functions shall not have variable number of arguments. Required for target library functions.
 */

#ifndef EHS_HAL_STRING_H
#define EHS_HAL_STRING_H

/*****************************************************************************/
/* Included files */
#include "globals.h"
#include "target_string.h"

/*****************************************************************************/
/* Define macros  */

/** Defines the end of line character used by EHS */
#define EHS_EOL (ehs_char)(0x0a)

#define EHS_TO_LOWER(x) (x | 0x20) /**< Simple conversion from either case to lower case */


/*****************************************************************************/
/* Define types */

/* ehs_FILE is defined in target_file.h */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

#ifndef EhsStrTrimL
/**
 * Trim the whitespace from the left hand end of a string. The
 * argument and return value can safely be const because trimming can be
 * done by changing the pointer.
 *
 * @param[in] pSrc Pointer to the string to trim
 * @return pointer to the trimmed string or NULL if error.
 */
EHS_EXTERN const ehs_char* EhsStrTrimL(const ehs_char* pSrc);
#endif

#ifndef EhsStrTrimR
/**
 * Trim the whitespace from the right hand end of a string
 *
 * @param[in] pSrc Pointer to the string to trim
 * @return pointer to the trimmed string (or NULL if error)
 */
EHS_EXTERN ehs_char* EhsStrTrimR(ehs_char* pSrc);
#endif

#ifndef EhsStrTrimLR
/**
 * Trim the whitespace from both ends of a string
 *
 * @param[in] pSrc Pointer to the string to trim
 * @return pointer to the trimmed string (or NULL if error)
 */
EHS_EXTERN ehs_char* EhsStrTrimLR(ehs_char* pSrc);
#endif

#ifndef EhsStrIsSpace
/**
 * Check if the specified character is a whitespace character or not.
 * @param c Character to check
 * @return EHS_TRUE if it is a whitespace character
 */
EHS_EXTERN ehs_bool EhsStrIsSpace(ehs_char c);
#endif

#if 0
ehs_char * EhsStrcpyUpTo( ehs_char * destination, const ehs_char * source, ehs_uint32 maxchars )
{
    for (ehs_int32 i = 0; i< maxchars; i++)
    {
        destination[i]=source[i];
        if (destination[i] == '\0') break;
    }
    if (destination[i] != '\0') destination[i+1] = '\0';
#error check the above and client trust of buffer size ...
}
#endif

/**
 * Determine if character is a-z or A-Z
 * @todo rename to EhsStrIsAlpha
 * @todo check to see if ctype provides a default implementation (guard with ifndef)
 */
EHS_EXTERN ehs_bool EhsIsAlpha(ehs_char ch);

/**
 * Determine if character is a-z or A-Z or 0-9
 * @todo rename to EhsStrIsAlNum
 * @todo check to see if ctype provides a default implementation (guard with ifndef)
 */
EHS_EXTERN ehs_bool EhsIsAlNum(ehs_char ch);

/**
 * Read an unsigned 32-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * exceeds EHS_UINT32_MAX
 *
 * @todo make this into a general-purpose function (not parse-only). It's already used that way.
 * @todo Move console printf into parser
 * @todo rename to EhsStr...
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
EHS_EXTERN const ehs_char* EhsGetUint32FromString(ehs_uint32 * output, const ehs_char* input);

/**
 * Read a signed 32-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * falls outside EHS_SINT32_MIN .. EHS_SINT32_MAX
 * @todo Move error message into parser
 * @todo rename to EhsStr...
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
extern const ehs_char* EhsGetSint32FromString(ehs_sint32 * output, const ehs_char* input);

/* and the float version */
const char* EhsGetDoubleFromString(ehs_float * output, const char* input) ;

/**
 * Read an unsigned 16-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * exceeds EHS_UINT16_MAX
 * @todo Move error message into parser
 * @todo rename to EhsStr...
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer), or null if parse fails
 */
extern const ehs_char* EhsGetUint16FromString(ehs_uint16* output,const ehs_char* input);

/**
 * Read a signed 16-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * exceeds EHS_UINT16_MAX
 * @todo Move error message into parser
 * @todo rename to EhsStr...
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer), or null if parse fails
 */
extern const ehs_char* EhsGetSint16FromString(ehs_sint16* output, const ehs_char* input);
/**
 * Read an unsigned 8-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * exceeds EHS_UINT8_MAX
 * @todo Move console printf into parser
 * @todo rename to EhsStr...
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
extern const ehs_char* EhsGetUint8FromString(ehs_uint8* output, const ehs_char* input);

/**
 * Read a word from a line of a SODL file. A word is a sequence of characters that do not include whitespace.
 *
 * @param[in,out] output Word read from input.
 * @param[in] input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
extern const ehs_char* EhsGetWordFromString(ehs_char * output, const ehs_char* input);

/**
 * @brief Parse all string words from a string. This is used for SODL parameters if there are only strings.
 * 
 * @param outputs The output string array. The string inside the array are allocated within the App scope. The string will be NULL pointer if it is empty.
 * @param input The input string
 * @param length The total number of words to be parsed from the input
 * @return ehs_uint8 The number of parsed words from string
 *
 * @code {.C}
 * ehs_char *array[2] = { NULL };
 * ehs_uint8 size = EhsGetWordsFromString(array, "Hello world", 2);
 * assert(size == 2);
 * @endcode
 * 
 */
EHS_EXTERN ehs_uint8 EhsGetWordsFromString(ehs_char **outputs, const ehs_char* input, ehs_uint8 length);

/**
 * Return a pointer to the character after the end of line character,
 * or null. The end of line character can be CR (0x0d), LF (0x0a) or CRLF.
 */
extern const ehs_char* EhsGetEol(const ehs_char* input);

/**
 * Convert a utf8 string into a utf32 value.
 * @param[out] pnUtf32 Points to the converted value (or EHS_UINT32_MAX if conversion failed)
 * @param[in] szSource Points to an entry within a string.
 * @return points to the next character in the string if the conversion succeeded, or null if it failed
 */
EHS_EXTERN const ehs_char* EhsHSUtil_getUtf32(ehs_uint32* pnUtf32, const ehs_char* szSource);


/**
 * \brief Returns a strings with all C escape sequences converted to ascii codes
 * Trusted client! - no length checks are done on buffers.
 */

EHS_EXTERN ehs_bool EhsParseEscapeChars(ehs_char * output, const ehs_char* input);

/**
 * \brief Returns a quote (") delimted string in output a pointer to the next char after in the input string.
 */

EHS_EXTERN const ehs_char* EhsGetQuoteDelimFromString(ehs_char * output, const ehs_char* input, ehs_uint16 max_length);

/* within target_file.h, file functions are normally #def'd to their stdio equivalents.
 * If the target cannot use the stdio version, the #define is removed from target_file,
 * and the below function is used. The declarations below also provide a secondary
 * purpose - they show the prototype of the function in question.
 */

#ifndef EhsSnprintf
EHS_EXTERN ehs_uint16 EhsSnprintf(ehs_char *pOutput, const ehs_char* pFmt, ...); /*lint !e960 Variable arguments required to support sprintf */
#endif

#ifndef EhsSprintf
EHS_EXTERN ehs_uint16 EhsSprintf(ehs_char *pOutput, const ehs_char* pFmt, ...); /*lint !e960 Variable arguments required to support sprintf */
#endif

#ifndef EhsSscanf
EHS_EXTERN ehs_uint16 EhsSscanf( const ehs_char *src, const ehs_char *format, ... ); /*lint !e960 Variable arguments required to support sscanf */
#endif

#ifndef EhsStricmp
EHS_EXTERN ehs_sint16 EhsStricmp(const ehs_char* sz1, const ehs_char* sz2);
#endif

#ifndef EhsStrnicmp
EHS_EXTERN ehs_sint16 EhsStrnicmp(const ehs_char* sz1, const ehs_char* sz2, ehs_uint16 len);
#endif

#ifndef EhsStrcmp
EHS_EXTERN ehs_sint16 EhsStrcmp(const ehs_char* sz1, const ehs_char* sz2);
#endif

#ifndef EhsStrlen
EHS_EXTERN ehs_sint16 EhsStrlen(const ehs_char*sz);
#endif

#ifndef EhsStrcat_s
EHS_EXTERN ehs_char* EhsStrcat_s(ehs_char* src, ehs_uint32 nNumElts, const ehs_char* dest);
#endif

#ifndef EhsStrncat
EHS_EXTERN ehs_char* EhsStrncat(ehs_char* src, const ehs_char* dest,ehs_uint32 nNumElts);
#endif

#ifndef EhsStrcat
EHS_EXTERN ehs_char* EhsStrcat(ehs_char* src, const ehs_char* dest);
#endif

#ifndef EhsStrchr
EHS_EXTERN ehs_char* EhsStrchr(const ehs_char*, ehs_char);
#endif

#ifndef EhsMemset
EHS_EXTERN void* EhsMemset(void*, ehs_sint32, ehs_uint32);
#endif

#ifndef EhsMemcpy
EHS_EXTERN void* EhsMemcpy(void*, void*, ehs_uint32);
#endif

#ifndef EhsStrcpy_s
ehs_char* EhsStrcpy_s(ehs_char* dest, ehs_uint32 nNumElts, const ehs_char* src);
#endif

#ifndef EhsStrncpy_s
ehs_char* EhsStrncpy_s(ehs_char* dest, ehs_uint32 nNumElts, const ehs_char* src, ehs_uint32 nCount);
#endif

#ifndef EhsStrstr
ehs_char* EhsStrstr(ehs_char*, ehs_char*);
#endif

/* Some more speciliased string parsing functions */
/* @brief Extracts specific http query strings from a string.*/
ehs_bool cgi_get_varval(const ehs_char *src, ehs_char *var_name, ehs_char *dst, ehs_uint32 length);

/* Simple URdecoder  - todo2024- call this EhsHSimpleUrlDecode() */
void cgi_urldecode(ehs_char *url);

#endif /* EHS_HAL_STRING_H */
