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
 * @param[in] max_len Maximum length of output buffer (including null terminator).
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
extern ehs_char* EhsGetWordFromString(ehs_char * output, const ehs_char* input, ehs_uint32 max_len);

/**
 * Read a record from a line of a SODL file. A rcord is a sequence of characters using unit seperators 0x1F instead of spaces
 # and accepts either spaces or end of record (0x1E) char to searate strinfs from Lucid.
 * 
 * @param[in,out] output Word read from input.
 * @param[in] input String containing SODL input.
 * @param[in] max_len Maximum length of output buffer (including null terminator).
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
extern ehs_char* EhsGetRecordFromString(ehs_char * output, const ehs_char* input, ehs_uint32 max_len);


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
 * \brief Converts the C escape sequences in input into their control characters.
 *
 * Handles \\n \\t \\r \\f; any other backslash pair is copied verbatim. The
 * conversion never expands - each input character yields at most one output
 * character - so an output buffer of EhsStrlen(input)+1 always suffices, and
 * output == input (in-place) is explicitly supported: the write index never
 * runs ahead of the read index.
 *
 * \param[out] output     destination, always NUL terminated when nOutputCap > 0
 * \param[in]  nOutputCap capacity of output in bytes, including the NUL
 * \param[in]  input      source string
 * \return EHS_TRUE if the whole of input was converted, EHS_FALSE if the
 *         result had to be truncated to fit nOutputCap (or on a null argument)
 */
EHS_EXTERN ehs_bool EhsParseEscapeChars(ehs_char * output, ehs_uint32 nOutputCap, const ehs_char* input);

/**
 * \brief Restore the spaces in a SODL parameter value, in place.
 *
 * The SODL parameter line is space separated, so Lucid replaces every space
 * inside a value with the ASCII Unit Separator (0x1F) before writing it - see
 * escape_param_spaces() in the Lucid tree. A block that reads its parameter
 * with EhsGetRecordFromString gets the spaces back as part of tokenising; a
 * block that takes the whole parameter as one free-text value (a format string,
 * a search string) copies it verbatim and must call this, or every space the
 * user typed stays an unprintable control character.
 *
 * \param[in,out] sz value to convert; ignored if NULL
 */
EHS_EXTERN void EhsParamUnescapeSpaces(ehs_char* sz);

/**
 * Class of argument a printf conversion specifier consumes. Used by
 * EhsFormatScan to police a format string against the argument type a
 * function block is actually able to supply.
 */
typedef enum
{
    EHS_FMT_ARG_NONE = 0, /**< consumes no argument - only "%%" */
    EHS_FMT_ARG_STRING,   /**< %s - consumes an ehs_char*      */
    EHS_FMT_ARG_INT,      /**< %d %i %u %o %x %X %c            */
    EHS_FMT_ARG_REAL,     /**< %f %F %e %E %g %G %a %A         */
    EHS_FMT_ARG_INVALID   /**< not a supported conversion      */
} EhsFormatArgType;

/** Result of EhsFormatScan. */
typedef struct
{
    ehs_uint32      nArgs;        /**< arguments the format consumes (valid only on success) */
    ehs_uint32      nErrorOffset; /**< byte offset of the offending '%' when szError is set  */
    const ehs_char* szError;      /**< static reason string, or NULL when the format is good */
} EhsFormatScanType;

/**
 * \brief Counts and validates the conversion specifiers in a printf format.
 *
 * Walks the full C99 specifier grammar - %[flags][width][.precision][length]conversion -
 * so the count it returns is exactly the number printf will consume, including
 * for specifiers carrying a flag, width or precision ("%10s", "%.5s", "%05d").
 * Use it in place of any '%'-counting scheme: supplying fewer arguments than
 * the format consumes makes printf read past the arguments it was given.
 *
 * "%%" is a literal percent and consumes nothing.
 *
 * Rejected unconditionally, whatever eAllowed is:
 *  - '*' as a width or precision, because it takes an extra int argument
 *  - length modifiers (h l j z t L), because they restate the argument width
 *    that the caller has already fixed
 *  - %n, which writes through its argument
 *  - any conversion whose class is not eAllowed, so %p and junk go too
 *
 * \param[out] pResult   scan result; nArgs is meaningful only when the return is EHS_TRUE
 * \param[in]  szFormat  format string to check (already escape-converted)
 * \param[in]  eAllowed  the one argument class this caller can supply
 * \param[in]  nMaxArgs  most arguments the caller is able to pass
 * \return EHS_TRUE if the format is safe to pass to EhsSnprintf with
 *         pResult->nArgs arguments of class eAllowed
 */
EHS_EXTERN ehs_bool EhsFormatScan(EhsFormatScanType* pResult,
                                  const ehs_char* szFormat,
                                  EhsFormatArgType eAllowed,
                                  ehs_uint32 nMaxArgs);

/** Most string conversions EhsScanfFormatBuild will accept in one format. */
#define EHS_SCANF_MAX_ARGS 8u

/** Result of EhsScanfFormatBuild. */
typedef struct
{
    ehs_uint32      nArgs;        /**< conversions that consume a destination pointer */
    ehs_uint32      anFixedWidth[EHS_SCANF_MAX_ARGS];
                                  /**< per argument: for %c the exact character count,
                                   *   which scanf does NOT terminate, so the caller must
                                   *   write the NUL. 0 for %s and %[, which terminate. */
    ehs_uint32      nErrorOffset; /**< byte offset of the offending '%' when szError is set */
    const ehs_char* szError;      /**< static reason string, or NULL on success */
} EhsScanfBuildType;

/**
 * \brief Rewrite a scanf format so every string conversion carries a field width.
 *
 * sscanf's %s, %c and %[ write through their destination pointer with no bound
 * of their own: without a field width, the length of the *scanned text* decides
 * how many bytes are written. Callers whose destinations are string connection
 * rows must therefore never hand a user-supplied format straight to sscanf.
 *
 * This copies szSrc to szDst, giving conversion i an explicit width no greater
 * than pnMaxChars[i] - clamping a width that is too large and supplying one
 * where the format omitted it. Conversions are matched to destinations in
 * order of appearance.
 *
 * Assignment-suppressed conversions ("%*d") consume no destination and are
 * copied through unchanged, whatever their type; they cannot write anywhere.
 *
 * Rejected: %n, length modifiers, and any non-suppressed conversion other than
 * s, c or [ - this builder is for callers whose destinations are all strings.
 *
 * \param[out] szDst      rewritten format, NUL terminated when nDstCap > 0
 * \param[in]  nDstCap    capacity of szDst in bytes, including the NUL
 * \param[in]  szSrc      format to rewrite
 * \param[in]  pnMaxChars per destination, the most characters it can accept
 *                        excluding the NUL; entries may differ per row
 * \param[in]  nMaxArgs   destinations available, at most EHS_SCANF_MAX_ARGS
 * \param[out] pResult    argument count and per-argument %c widths
 * \return EHS_TRUE if szDst is safe to pass to EhsSscanf with pResult->nArgs
 *         destination pointers
 */
EHS_EXTERN ehs_bool EhsScanfFormatBuild(ehs_char* szDst, ehs_uint32 nDstCap,
                                        const ehs_char* szSrc,
                                        const ehs_uint32* pnMaxChars,
                                        ehs_uint32 nMaxArgs,
                                        EhsScanfBuildType* pResult);

/**
 * \brief Returns a quote (") delimted string in output a pointer to the next char after in the input string.
 */

EHS_EXTERN const ehs_char* EhsGetQuoteDelimFromString(ehs_char * output, const ehs_char* input, ehs_uint16 max_length);

/**
 * \brief Searches through the string and replaces the first char that is either \n or \r with \0.
 * If none is found, sets the first char to \0 and returns false, otherwise returns true.
 */
EHS_EXTERN ehs_bool EhsReplaceNextEolWithNull(ehs_char* p, ehs_uint32 max_length);

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
