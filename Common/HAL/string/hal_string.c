/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file hal_string.c
 * In this file, target-indpendent code used by the hardware abstraction layer is defined.
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section hal_string_c
 * @anchor hal_string_c
 * @subsection misra MISRA compliance:
 * Demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained the following derogations to the MISRA standard.
 * - Rule 16.1 Functions shall not have variable number of arguments. Required for target library functions.
 */

/*****************************************************************************/
/* Included files */

//#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_STRING /**< required to be defined for hal_logger.h */
#include "globals.h"
#include "hal_string.h"
#include "messages.h"
#include "hal_logger.h"
// Used for EhsHMem_writableAlloc
#include "hal_mem.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/*****************************************************************************/
/* Declare prototypes of local functions */
/*****************************************************************************/
/* Variables defined with file-scope */
/*****************************************************************************/
/* Variables defined with global-scope */
/*****************************************************************************/
/* Function definitions */

/**
 * Trim the whitespace from the left hand end of a string. The
 * argument and return value can safely be const because trimming can be
 * done by changing the pointer.
 *
 * @param[in] pSrc Pointer to the string to trim
 * @return pointer to the trimmed string or NULL if error.
 */
const ehs_char* EhsStrTrimL(const ehs_char* pSrc)
{
    if (pSrc)
    {
        while (EhsStrIsSpace(*pSrc)  || *pSrc == 10 || *pSrc == 12 || *pSrc == '\t' )
            pSrc++;
    }

    return pSrc;
}

/**
 * Trim the whitespace from the right hand end of a string
 *
 * @param[in] pSrc Pointer to the string to trim
 * @return pointer to the trimmed string (or NULL if error)
 */
ehs_char* EhsStrTrimR(ehs_char* pSrc)
{
    ehs_sint16 nLast; /* index of last character */

    if (pSrc)
    {
        nLast = EhsStrlen(pSrc); /* get last character */

        while (nLast > 0u) /* go backwards until we get to the first character */
        {
            nLast--;
            if (EhsStrIsSpace(pSrc[nLast]))
            {
                pSrc[nLast] = '\0'; /* overwrite spaces with line terminator */
            }
            else
            {
                break; /* if it's not a space we stop trimming */
            }

        }
    }

    return pSrc;
}

/**
 * Trim the whitespace from both ends of a string
 *
 * @param[in] pSrc Pointer to the string to trim
 * @return pointer to the trimmed string (or NULL if error)
 */
ehs_char* EhsStrTrimLR(ehs_char* pSrc)
{
    pSrc = (ehs_char*)EhsStrTrimL(pSrc); /* discard const-ness of EhsStrTrimL return value */
    pSrc = EhsStrTrimR(pSrc);

    return pSrc;
}

/**
 * Determine if character is a-z or A-Z
 */
ehs_bool EhsIsAlpha(char c)
{
    return (EHS_TO_LOWER(c) >= 'a' && EHS_TO_LOWER(c) <= 'z');
}

/**
 * Determine if character is a-z or A-Z or 0-9
 */
ehs_bool EhsIsAlNum(char c)
{
    return ((EHS_TO_LOWER(c) >= 'a' && EHS_TO_LOWER(c) <= 'z')||
            (c >= '0' && c <= '9'));

}

/**
 * Determine if character is a-f or A-F or 0-9
 */
ehs_bool EhsIsHexNum(char ch)
{
    return (ch >= '0' && ch <= '9') ||
           (ch >= 'A' && ch <= 'F') ||
           (ch >= 'a' && ch <= 'f');
}

/* replaces any EOL chr with NULL if found. returns false otherwise*/
ehs_bool EhsReplaceNextEolWithNull( ehs_char* p, ehs_uint32 max_length ) {
    ehs_uint32 i;
    if (p == NULL || max_length == 0) {
        return EHS_FALSE;
    }
    for (i = 0; i < max_length; i++) {
        if (p[i] == '\n' || p[i] == '\r') {
            p[i] = '\0';
            return EHS_TRUE;
        }
    }
    // No EOL found, set first char to NULL and return false
    //printf("Eol-Swap-WRONG!!!\n");
    p[0] = '\0';
    return EHS_FALSE;
}

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
const char* EhsGetUint32FromString(ehs_uint32 * output, const char* input)
{
    ehs_uint16 i;
    ehs_uint32 digit;

    input = EhsStrTrimL(input);
    *output=0;

    if ((NULL == input) || (*input < '0') || (*input > '9'))
    {
        input = NULL;
    }
    else
    {
        /* convert any numbers into an integer */
        for (i=0; (i<EHS_STRING_LENGTH_MAX)&&((*input >= '0') && (*input <= '9')); i++)
        {
            digit = (*input - '0');
            if (*output >= (EHS_UINT32_MAX/10u))
            {
                if ((*output != (EHS_UINT32_MAX/10u)) || ((EHS_UINT32_MAX % 10u) < digit))
                {
                    EHSH_LOG_ERROR(EHS_MSG_PARSER_UINT32_RANGE(*output,digit));
                }
            }
            *output = (*output * 10) + (*input - '0');
            input++;
        }
    }

    /* check for end-of-file */
    if ((NULL != input) && (*input == 25))
    {
        input = NULL;
    }

    return input;
}

const char* EhsGetDoubleFromString(ehs_float * output, const char* input)
{
    const ehs_char * in = input;
    int i;
    if (EhsSscanf(input,EHS_FL_FMT,output)>0)
    {
        in = EhsStrTrimL(in);
        //find the end of the string
        for (i=0; (i<EHS_STRING_LENGTH_MAX)&&((((*in >= '0') && (*in <= '9'))||(*in == '.'))); i++)
        {

            in++;
        }
    }

    return in;
}

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
const char* EhsGetSint32FromString(ehs_sint32 * output, const char* input)
{
    //int i=0;
    ehs_bool isNegative = EHS_FALSE;
    ehs_uint32 nAbsVal; /* unsigned value from SODL_GetUint */

    input = EhsStrTrimL(input);

    /* check for leading '-' */
    if ((NULL != input) && (*input == '-'))
    {
        isNegative = EHS_TRUE;
        input++;
    }

    input = EhsGetUint32FromString(&nAbsVal,input);

    if (input)
    {
        if ((nAbsVal <= EHS_SINT32_MAX) ||
                (isNegative && (nAbsVal <= (ehs_uint32)(-1 * EHS_SINT32_MAX))))
        {
            *output = (isNegative)?nAbsVal*-1:nAbsVal;
        }
        else
        {
            EHSH_LOG_ERROR(EHS_MSG_PARSER_SINT32_RANGE(nAbsVal));
            input = NULL;
        }

        /* check for end-of-file */
        if(input!=NULL)
        {
            // if ((*input == 25) || (*input == 0))
            if (*input == 25)
            {
                input = NULL;
            }
        }

    }

    return input;
}

/**
 * Read a signed 16-bit integer from a line of the SODL file.
 * Generates an error message and sets EhsParseFailed if the value
 * exceeds EHS_SINT16_MAX
 * @todo Move console printf into parser
 *
 * @param output Integer read from input (0 if integer can't be found).
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer), or null if parse fails
 */
const char* EhsGetSint16FromString(ehs_sint16* output, const char* input)
{
    ehs_sint32 nValue;

    input = EhsGetSint32FromString(&nValue, input);

    if (input)
    {
        if ((nValue > EHS_SINT16_MAX) || (nValue < EHS_SINT16_MIN))
        {
            EHSH_LOG_ERROR(EHS_MSG_PARSER_UINT16_RANGE(nValue));
            input = NULL;
        }
        else
        {
            *output = (ehs_uint16)nValue;
        }
    }

    return input;
}


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
const ehs_char* EhsGetUint16FromString(ehs_uint16* output,const ehs_char* input)
{
    ehs_uint32 nValue;
    input = EhsGetUint32FromString(&nValue, input);
    if (input)
    {
        if (nValue > EHS_UINT16_MAX)
        {
            EHSH_LOG_ERROR(EHS_MSG_PARSER_UINT16_RANGE(nValue));
            input = NULL;
        }
        else
        {
            *output = (ehs_uint16)nValue;
        }
    }
    return input;
}

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
const char* EhsGetUint8FromString(ehs_uint8* output, const char* input)
{
    ehs_uint32 nValue;

    input = EhsGetUint32FromString(&nValue, input);

    if (input)
    {
        if (nValue > EHS_UINT8_MAX)
        {
            EHSH_LOG_ERROR(EHS_MSG_PARSER_UINT8_RANGE(nValue));
            input = NULL;
        }
        else
        {
            if(output!=NULL){
                *output = (ehs_uint8)nValue;
            }
        }
    }

    return input;
}


/**
 * Return a pointer to the character after the end of line character,
 * or null. The end of line character can be CR (0x0d), LF (0x0a) or CRLF.
 */
const ehs_char* EhsGetEol(const ehs_char* input)
{
    ehs_char* pEol = strchr(input,0x0d);
    if (pEol)
    {
        /* is CR followed by LF? */
        if (pEol[1] == 0xa)
        {
            pEol++;
        }
    }
    else
    {
        pEol = strchr(input, 0x0a);
    }
    if (pEol)
    {
        pEol++;
    }
    return pEol;
}

/**
 * Convert a utf8 string into a utf32 value. See Wikipedia entry http://en.wikipedia.org/wiki/Utf8
 * for details.
 * @param[out] pnUtf32 Points to the converted value (or EHS_UINT32_MAX if conversion failed)
 * @param[in] szSource Points to an entry within a string.
 * @return points to the next character in the string if the conversion succeeded, or null if it failed
 */
const ehs_char* EhsHSUtil_getUtf32(ehs_uint32* pnUtf32, const ehs_char* szSource)
{
    const ehs_char* pRet = NULL; /* assume failure */
    ehs_uint16 nCount = 0u; /* number of bytes after the first to convert */
    const ehs_uint8* pSource = (ehs_uint8*)szSource; /* use uint8 to prevent sign extension on bytes > 0x7f */
    //EHSH_LOG_ENTER("EhsHUtil_getUtf32(%x,%s)",(ehs_uint32)pnUtf32,szSource);

    if (((*pSource) & 0x80u) == 0u)
    {
        /* single byte sequence */
        *pnUtf32 = *pSource++;
    }
    else if (((*pSource) & 0xe0u) == 0xc0u)     /* double byte sequence */
    {
        *pnUtf32 = ((*pSource++) & 0x1fu);
        nCount = 1;
    }
    else if (((*pSource) & 0xf0u) == 0xe0u)     /* triple byte sequence */
    {
        *pnUtf32 = ((*pSource++) & 0x0fu);
        nCount = 2;
    }
    else if (((*pSource) & 0xf8u) == 0xf0u)     /* quad byte sequence */
    {
        if (*pSource >= 0xf5u)
        {
            *pnUtf32 = EHS_UINT32_MAX;
            EHSH_LOG_ERROR(EHS_MSG_ERROR_HS_UTF32_RANGE(szSource));
        }
        else
        {
            *pnUtf32 = ((*pSource++) & 0x07u);
            nCount = 3;
        }
    }
    else     /* error */
    {
        *pnUtf32 = *pSource++;
        //*pnUtf32 = EHS_UINT32_MAX;
        EHSH_LOG_INFO(EHS_MSG_ERROR_HS_UTF32_FAILED(szSource));
    }

    /* convert multi-byte sequence */
    while (nCount > 0u)
    {
        if (((*pSource) & 0xc0u) == 0x80u)
        {
            *pnUtf32 = ((*pnUtf32) << 6u) | ((*pSource++) & 0x3fu);
            nCount--;
        }
        else
        {
            //*pnUtf32 = EHS_UINT32_MAX;
            *pnUtf32 = *pSource++;
            EHSH_LOG_INFO(EHS_MSG_ERROR_HS_UTF32_FAILED(szSource));
            break;
        }
    }

    if (*pnUtf32 != EHS_UINT32_MAX)
    {
        pRet= (ehs_char*)pSource;
    }

    //EHSH_LOG_EXIT("EhsHUtil_getUtf32() -> %x", (ehs_uint32)pRet);
    return pRet;
}


/**
 * Copies a word from a line of a SODL file. A word is a sequence of characters that do not include whitespace.
 *
 * @param output Word read from input. NULL just to skip the word
 * @param input String containing SODL input.
 * @param max_len Maximum length of output buffer (including null terminator).
 * @return Pointer to updated input string (i.e. after reading the integer) or NULL if it exceeds the bbuffer size of there are no words to return.
 */
ehs_char* EhsGetWordFromString(ehs_char *output,
                                const ehs_char *input,
                                ehs_uint32 max_len)
{
    
    if (input == NULL)
        return NULL;
    //printf("IP=%s\n",input);
    input = EhsStrTrimL(input);

    // no input / end markers
    if (*input == '\0' || *input == 25)
    {
        if (output && max_len > 0)
            output[0] = '\0';
        return NULL;
    }

    ehs_char *out_start = output;
    const ehs_char *word_start = input;

    // copy word
    while (*input &&
           !EhsStrIsSpace(*input) &&
           *input != 1 &&
           *input != 25)
    {
        if (output)
        {
            // ensure space for next char + null terminator
            if ((ehs_uint32)(output - out_start) + 1 >= max_len)
            {
                printf("\nERROR: GetWord O/F [%s] (max size=%d)\n", word_start,max_len);
                //while (1) {} // temporary to stop spamming.
                return NULL;
            }

            *output++ = *input;
        }

        input++;
    }

    // if no characters copied : no word
    if (output && output == out_start)
        return NULL;

    // null terminate
    if (output && max_len > 0)
        *output = '\0';

    return (ehs_char*)input;
}

/**
 * Copies a record from a line of a SODL file. A rcord is a sequence of characters using unit seperators 0x1F instead of spaces
 # and accepts either spaces or end of record (0x1E) char to searate strinfs from Lucid.
 * 
 * @param[in,out] output Word read from input.
 * @param[in] input String containing SODL input.
 * @param[in] max_len Maximum length of output buffer (including null terminator).
 * @return Pointer to updated input string (i.e. after reading the integer)
 * - Note #1 near copy of EhsGetWordFromString() for speed.
 * - Note #2 we should probably get Lucid and kernel parser to striclty use 0x1E (end of record to deliniate paramters, 
 *           but for now we convert 0x1Fs to spaces. We should support 0x1Fs as spaces anyway)
 */

ehs_char* EhsGetRecordFromString(ehs_char *output,
                                const ehs_char *input,
                                ehs_uint32 max_len)
{
    
    if (input == NULL)
        return NULL;
    input = EhsStrTrimL(input);

    // no input / end markers
    if (*input == '\0' || *input == 0x19) // 0x19 (ascii 25) is end of medium
    {
        if (output && max_len > 0)
            output[0] = '\0';
        return NULL;
    }

    ehs_char *out_start = output;
    const ehs_char *word_start = input;

    // copy word
    while (*input &&
           !EhsStrIsSpace(*input) && //delimit with normal space tab \n \v \f \r
           *input != 1 &&
           *input != 0x19 &&   // checkfor ed of medium
           *input != 0x1E      // check for end of record
        )
    {
        if (output)
        {
            // ensure space for next char + null terminator
            if ((ehs_uint32)(output - out_start) + 1 >= max_len)
            {
                out_start[max_len - 1] = '\0';
                printf("\nERROR: GetWord O/F [%s] (max size=%d)\n", word_start,max_len);
                return NULL;
            }
            if ( *input  == 0x1F )
                *output++ = ' '; // replacing Unit seperators (that Lucid produces with normal spaces)
            else
                *output++ = *input;
        }
        input++;
    }

    // if no characters copied : no word
    if (output && output == out_start)
        return NULL;

    // null terminate
    if (output && max_len > 0)
        *output = '\0';

    return (ehs_char*)input;
}



/*** @brief This function replaces escaped control characters into the control characters
 *
 * Bounded by nOutputCap, and safe to call in place (output == input): no branch
 * writes more characters than it consumed, so the write index never overtakes
 * the read index. See the header for the full contract.
 *
 *  */
ehs_bool EhsParseEscapeChars(ehs_char * output, ehs_uint32 nOutputCap, const ehs_char* input)
{
    ehs_uint32 i = 0u;
    ehs_uint32 j = 0u;
    ehs_uint32 nMax;

    if ((output == NULL) || (input == NULL) || (nOutputCap == 0u))
    {
        if ((output != NULL) && (nOutputCap > 0u))
        {
            output[0] = '\0';
        }
        return EHS_FALSE;
    }
    nMax = nOutputCap - 1u; /* leave room for the terminator */

    while (input[i] != '\0')
    {
        if (input[i] != '\\')
        {
            if (j >= nMax) break;
            output[j++] = input[i++];
        }
        else if (input[i + 1u] == '\0')
        {
            /* trailing lone backslash - emit it and stop */
            if (j >= nMax) break;
            output[j++] = input[i++];
        }
        else
        {
            ehs_char cEscaped = '\0';
            switch (input[i + 1u])
            {
            case 'n': cEscaped = '\n'; break;
            case 't': cEscaped = '\t'; break;
            case 'r': cEscaped = '\r'; break;
            case 'f': cEscaped = '\f'; break;
            default:  cEscaped = '\0'; break; /* not one of ours */
            }

            if (cEscaped != '\0')
            {
                if (j >= nMax) break;
                output[j++] = cEscaped;
                i += 2u;
            }
            else
            {
                /* both characters or neither, so a truncation never leaves a
                 * dangling backslash */
                if ((j + 1u) >= nMax) break;
                output[j++] = input[i];
                output[j++] = input[i + 1u];
                i += 2u;
            }
        }
    }
    output[j] = '\0';

    return (input[i] == '\0') ? EHS_TRUE : EHS_FALSE;
}

void EhsParamUnescapeSpaces(ehs_char* sz)
{
    ehs_uint32 i;

    if (sz == NULL)
    {
        return;
    }
    for (i = 0u; sz[i] != '\0'; i++)
    {
        if (sz[i] == (ehs_char)0x1F)
        {
            sz[i] = ' ';
        }
    }
}

/*****************************************************************************/
/* printf format specifier scanning - see hal_string.h for the contract */

/** flags: the characters permitted between '%' and the width */
static ehs_bool EhsFormat_isFlag(ehs_char c)
{
    return ((c == '-') || (c == '+') || (c == ' ') ||
            (c == '#') || (c == '0')) ? EHS_TRUE : EHS_FALSE;
}

static ehs_bool EhsFormat_isDigit(ehs_char c)
{
    return ((c >= '0') && (c <= '9')) ? EHS_TRUE : EHS_FALSE;
}

/** length modifiers - always rejected; the caller has already fixed the
 * argument width */
static ehs_bool EhsFormat_isLength(ehs_char c)
{
    return ((c == 'h') || (c == 'l') || (c == 'j') ||
            (c == 'z') || (c == 't') || (c == 'L')) ? EHS_TRUE : EHS_FALSE;
}

/** Map a conversion character onto the class of argument it consumes. */
static EhsFormatArgType EhsFormat_classify(ehs_char c)
{
    switch (c)
    {
    case 's':
        return EHS_FMT_ARG_STRING;
    case 'd':
    case 'i':
    case 'u':
    case 'o':
    case 'x':
    case 'X':
    case 'c':
        return EHS_FMT_ARG_INT;
    case 'f':
    case 'F':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
    case 'a':
    case 'A':
        return EHS_FMT_ARG_REAL;
    default:
        return EHS_FMT_ARG_INVALID;
    }
}

/** Record a rejection and return EHS_FALSE, so callers can 'return EhsFormat_fail(...)'. */
static ehs_bool EhsFormat_fail(EhsFormatScanType* pResult, ehs_uint32 nOffset, const ehs_char* szReason)
{
    pResult->nArgs = 0u;
    pResult->nErrorOffset = nOffset;
    pResult->szError = szReason;
    return EHS_FALSE;
}

ehs_bool EhsFormatScan(EhsFormatScanType* pResult,
                       const ehs_char* szFormat,
                       EhsFormatArgType eAllowed,
                       ehs_uint32 nMaxArgs)
{
    ehs_uint32 i = 0u;
    ehs_uint32 nArgs = 0u;

    if (pResult == NULL)
    {
        return EHS_FALSE;
    }
    pResult->nArgs = 0u;
    pResult->nErrorOffset = 0u;
    pResult->szError = NULL;

    if (szFormat == NULL)
    {
        return EhsFormat_fail(pResult, 0u, "no format string");
    }

    /* Every predicate below is false for '\0', and the only unconditional step
     * lands on the terminator at worst, so the walk cannot run off the end. */
    while (szFormat[i] != '\0')
    {
        ehs_uint32 nSpecStart;
        EhsFormatArgType eType;

        if (szFormat[i] != '%')
        {
            i++;
            continue;
        }

        nSpecStart = i;
        i++; /* step over the '%' */

        if (szFormat[i] == '%')
        {
            i++; /* "%%" - a literal percent, consumes no argument */
            continue;
        }

        while (EhsFormat_isFlag(szFormat[i]))
        {
            i++;
        }
        if (szFormat[i] == '*')
        {
            return EhsFormat_fail(pResult, nSpecStart,
                                  "'*' width is not supported - it consumes an extra argument");
        }
        while (EhsFormat_isDigit(szFormat[i]))
        {
            i++;
        }
        if (szFormat[i] == '.')
        {
            i++;
            if (szFormat[i] == '*')
            {
                return EhsFormat_fail(pResult, nSpecStart,
                                      "'*' precision is not supported - it consumes an extra argument");
            }
            while (EhsFormat_isDigit(szFormat[i]))
            {
                i++;
            }
        }
        if (EhsFormat_isLength(szFormat[i]))
        {
            return EhsFormat_fail(pResult, nSpecStart,
                                  "length modifiers (h l j z t L) are not supported");
        }
        if (szFormat[i] == 'n')
        {
            return EhsFormat_fail(pResult, nSpecStart,
                                  "'%n' is not supported - it writes through its argument");
        }

        eType = EhsFormat_classify(szFormat[i]);
        if (eType != eAllowed)
        {
            /* also catches %p, a positional '$', a trailing '%', and anything
             * else unrecognised */
            switch (eAllowed)
            {
            case EHS_FMT_ARG_STRING:
                return EhsFormat_fail(pResult, nSpecStart, "only %s placeholders are supported here");
            case EHS_FMT_ARG_INT:
                return EhsFormat_fail(pResult, nSpecStart,
                                      "only integer placeholders (%d %i %u %o %x %X %c) are supported here");
            case EHS_FMT_ARG_REAL:
                return EhsFormat_fail(pResult, nSpecStart,
                                      "only real placeholders (%f %e %g %a) are supported here");
            default:
                return EhsFormat_fail(pResult, nSpecStart, "placeholders are not supported here");
            }
        }
        i++; /* step over the conversion character */

        nArgs++;
        if (nArgs > nMaxArgs)
        {
            return EhsFormat_fail(pResult, nSpecStart, "too many placeholders for the inputs available");
        }
    }

    pResult->nArgs = nArgs;
    return EHS_TRUE;
}

/*****************************************************************************/
/* scanf format rewriting - see hal_string.h for the contract */

/** Append one character, or report that szDst is full. */
static ehs_bool EhsScanf_put(ehs_char* szDst, ehs_uint32 nDstCap, ehs_uint32* pnAt, ehs_char c)
{
    if (*pnAt + 1u >= nDstCap)
    {
        return EHS_FALSE; /* leave room for the terminator */
    }
    szDst[*pnAt] = c;
    (*pnAt)++;
    return EHS_TRUE;
}

/** Append a decimal field width. */
static ehs_bool EhsScanf_putWidth(ehs_char* szDst, ehs_uint32 nDstCap, ehs_uint32* pnAt, ehs_uint32 nWidth)
{
    ehs_char szNum[12];
    ehs_uint32 nDigits = 0u;
    ehs_uint32 n = nWidth;

    if (n == 0u)
    {
        szNum[nDigits++] = '0';
    }
    while ((n > 0u) && (nDigits < sizeof(szNum)))
    {
        szNum[nDigits++] = (ehs_char)('0' + (n % 10u));
        n /= 10u;
    }
    while (nDigits > 0u)
    {
        nDigits--;
        if (!EhsScanf_put(szDst, nDstCap, pnAt, szNum[nDigits]))
        {
            return EHS_FALSE;
        }
    }
    return EHS_TRUE;
}

static ehs_bool EhsScanf_fail(EhsScanfBuildType* pResult, ehs_uint32 nOffset, const ehs_char* szReason)
{
    pResult->nArgs = 0u;
    pResult->nErrorOffset = nOffset;
    pResult->szError = szReason;
    return EHS_FALSE;
}

ehs_bool EhsScanfFormatBuild(ehs_char* szDst, ehs_uint32 nDstCap,
                             const ehs_char* szSrc,
                             const ehs_uint32* pnMaxChars,
                             ehs_uint32 nMaxArgs,
                             EhsScanfBuildType* pResult)
{
    ehs_uint32 i = 0u;   /* read index into szSrc */
    ehs_uint32 o = 0u;   /* write index into szDst */
    ehs_uint32 nArgs = 0u;
    ehs_uint32 k;

    if (pResult == NULL)
    {
        return EHS_FALSE;
    }
    pResult->nArgs = 0u;
    pResult->nErrorOffset = 0u;
    pResult->szError = NULL;
    for (k = 0u; k < EHS_SCANF_MAX_ARGS; k++)
    {
        pResult->anFixedWidth[k] = 0u;
    }

    if ((szDst == NULL) || (nDstCap == 0u) || (szSrc == NULL) || (pnMaxChars == NULL))
    {
        if ((szDst != NULL) && (nDstCap > 0u))
        {
            szDst[0] = '\0';
        }
        return EhsScanf_fail(pResult, 0u, "no format string");
    }
    szDst[0] = '\0';
    if (nMaxArgs > EHS_SCANF_MAX_ARGS)
    {
        nMaxArgs = EHS_SCANF_MAX_ARGS;
    }

    while (szSrc[i] != '\0')
    {
        ehs_uint32 nSpecStart;
        ehs_bool bSuppressed = EHS_FALSE;
        ehs_uint32 nWidth = 0u;
        ehs_bool bHasWidth = EHS_FALSE;
        ehs_uint32 nMax;
        ehs_char cConv;

        if (szSrc[i] != '%')
        {
            if (!EhsScanf_put(szDst, nDstCap, &o, szSrc[i]))
            {
                return EhsScanf_fail(pResult, i, "format too long once field widths are added");
            }
            i++;
            continue;
        }

        nSpecStart = i;
        i++; /* step over the '%' */

        if (szSrc[i] == '%')
        {
            if (!EhsScanf_put(szDst, nDstCap, &o, '%') ||
                !EhsScanf_put(szDst, nDstCap, &o, '%'))
            {
                return EhsScanf_fail(pResult, nSpecStart, "format too long once field widths are added");
            }
            i++;
            continue;
        }

        if (szSrc[i] == '*')
        {
            bSuppressed = EHS_TRUE;
            i++;
        }
        while ((szSrc[i] >= '0') && (szSrc[i] <= '9'))
        {
            bHasWidth = EHS_TRUE;
            if (nWidth < 1000000u) /* saturate rather than wrap on a silly width */
            {
                nWidth = (nWidth * 10u) + (ehs_uint32)(szSrc[i] - '0');
            }
            i++;
        }
        if ((szSrc[i] == 'h') || (szSrc[i] == 'l') || (szSrc[i] == 'j') ||
            (szSrc[i] == 'z') || (szSrc[i] == 't') || (szSrc[i] == 'L'))
        {
            return EhsScanf_fail(pResult, nSpecStart,
                                 "length modifiers (h l j z t L) are not supported");
        }
        cConv = szSrc[i];
        if (cConv == 'n')
        {
            return EhsScanf_fail(pResult, nSpecStart,
                                 "'%n' is not supported - it writes through its argument");
        }
        if (cConv == '\0')
        {
            return EhsScanf_fail(pResult, nSpecStart, "format ends in an incomplete conversion");
        }

        if (bSuppressed)
        {
            /* Nothing is assigned, so the conversion type does not matter and
             * no width is needed. Copy the specifier through as written. */
            ehs_uint32 nCopy;
            for (nCopy = nSpecStart; nCopy <= i; nCopy++)
            {
                if (!EhsScanf_put(szDst, nDstCap, &o, szSrc[nCopy]))
                {
                    return EhsScanf_fail(pResult, nSpecStart, "format too long once field widths are added");
                }
            }
            i++;
            if (cConv == '[')
            {
                /* copy the scanset through as well */
                ehs_uint32 nSet = i;
                if (szSrc[nSet] == '^') { nSet++; }
                if (szSrc[nSet] == ']') { nSet++; } /* a leading ']' is a literal */
                while ((szSrc[nSet] != '\0') && (szSrc[nSet] != ']')) { nSet++; }
                if (szSrc[nSet] != ']')
                {
                    return EhsScanf_fail(pResult, nSpecStart, "unterminated '[' scanset");
                }
                while (i <= nSet)
                {
                    if (!EhsScanf_put(szDst, nDstCap, &o, szSrc[i]))
                    {
                        return EhsScanf_fail(pResult, nSpecStart, "format too long once field widths are added");
                    }
                    i++;
                }
            }
            continue;
        }

        if ((cConv != 's') && (cConv != 'c') && (cConv != '['))
        {
            return EhsScanf_fail(pResult, nSpecStart,
                                 "only %s, %c and %[ are supported - the destinations are strings");
        }
        if (nArgs >= nMaxArgs)
        {
            return EhsScanf_fail(pResult, nSpecStart, "more conversions than outputs available");
        }

        nMax = pnMaxChars[nArgs];
        if (nMax == 0u)
        {
            return EhsScanf_fail(pResult, nSpecStart, "destination has no capacity");
        }
        /* Supply a width if the format omitted one... */
        if (!bHasWidth || (nWidth == 0u))
        {
            nWidth = (cConv == 'c') ? 1u : nMax;
        }
        /* ...then clamp whatever we have to what the destination can take. */
        if (nWidth > nMax)
        {
            nWidth = nMax;
        }

        if (!EhsScanf_put(szDst, nDstCap, &o, '%') ||
            !EhsScanf_putWidth(szDst, nDstCap, &o, nWidth) ||
            !EhsScanf_put(szDst, nDstCap, &o, cConv))
        {
            return EhsScanf_fail(pResult, nSpecStart, "format too long once field widths are added");
        }
        i++;

        if (cConv == '[')
        {
            ehs_uint32 nSet = i;
            if (szSrc[nSet] == '^') { nSet++; }
            if (szSrc[nSet] == ']') { nSet++; }
            while ((szSrc[nSet] != '\0') && (szSrc[nSet] != ']')) { nSet++; }
            if (szSrc[nSet] != ']')
            {
                return EhsScanf_fail(pResult, nSpecStart, "unterminated '[' scanset");
            }
            while (i <= nSet)
            {
                if (!EhsScanf_put(szDst, nDstCap, &o, szSrc[i]))
                {
                    return EhsScanf_fail(pResult, nSpecStart, "format too long once field widths are added");
                }
                i++;
            }
        }

        /* %c writes exactly nWidth characters and adds no terminator */
        pResult->anFixedWidth[nArgs] = (cConv == 'c') ? nWidth : 0u;
        nArgs++;
    }

    szDst[o] = '\0';
    pResult->nArgs = nArgs;
    return EHS_TRUE;
}

/*** @brief Read a " dlimted string from a space saprated list of paramters
 *
 * Requires an output buffer to write results into
 */

const ehs_char* EhsGetQuoteDelimFromString(ehs_char * output, const ehs_char* input, ehs_uint16 max_length)
{
    ehs_bool start_copy = EHS_FALSE;
    if (output)
    {
        output[0]= '\0';
        while (max_length-- >= 0 && *input !='\0' )
        {
            if (*input == '"' )
            {
                if (start_copy == EHS_FALSE)
                {
                    input++;
                    start_copy=EHS_TRUE;
                }
                else
                {
                    *output = '\0';
                    break;

                }
            }
            else if (start_copy == EHS_TRUE)
            {
                *output++=*input++;
            }
        } //end while
    } // else report no buffer

    return input+1; //return pointer to the next character after the quote
}

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
ehs_uint8 EhsGetWordsFromString(ehs_char **outputs, const ehs_char* input, ehs_uint8 length)
{
    ehs_char in_temp[EHS_STRING_LENGTH_MAX];
    ehs_uint8 i = 0;
    ehs_uint16 str_count = 0;
    // Parse nothing if there is no enough memory left to be allocated
    if (in_temp == NULL) return 0;
    for (i = 0 ; i < length && input ; i++)
    {
        EhsMemset(in_temp, '\0', str_count + 1);
        input = EhsStrTrimL(input);
        if (input[0] != '"')
        {
            /* When it's not a string containing spaces */
            input = EhsGetWordFromString(in_temp, input, sizeof(in_temp));
            str_count = EhsStrlen(in_temp);
            if (str_count == 4 && EhsStrncmp(in_temp, "NULL", 4) == 0)
            {
                outputs[i] = NULL;
                continue;
            }
            else
            {
                goto jump_point;
            }
        }
        else
        {
            /* When it's a string containing spaces */
            input = EhsGetQuoteDelimFromString(in_temp, input, EHS_STRING_LENGTH_MAX);
            str_count = EhsStrlen(in_temp);
            goto jump_point;
        }
jump_point:
        if (str_count > 0)
        {
            outputs[i] = EhsHMem_writeableAlloc(str_count + 1);
            if (outputs[i] != NULL)
            {
                EhsStrcpy(outputs[i], in_temp);
                outputs[i][str_count] = '\0';
            }
        }
        else outputs[i] = NULL;
    }
    return i;
}

/* This is used by https and could also be used by the cgi conversion function block to convert 
   urlencoded query strings - thoug is doesn't URL decode so expects all variables to be standard strings
   todo2024 rename this EhsHCgiGetVarVal 
*/
ehs_bool cgi_get_varval(const ehs_char *src, ehs_char *var_name, ehs_char *dst, ehs_uint32 length)
{
    ehs_char *name;
    ehs_bool result;
    ehs_uint32 index;
    ehs_uint32 n_length;

    result = EHS_FALSE;
    dst[0] = 0;
    name = (ehs_char*)src;

    n_length = strlen(var_name);

    while ((name = strstr(name, var_name)) != 0)
    {
        if (name[n_length] == '=')
        {
            name += n_length + 1;

            index = strcspn(name, "&");
            if (index >= length)
            {
                index = length - 1;
            }
            strncpy(dst, name, index);
            dst[index] = '\0';
            result = EHS_TRUE;
            break;
        }
        else
        {
            name = strchr(name, '&');
        }
    }

    return (result);
}


/* Simple URdecoder  - Consider putting this is common code */
void cgi_urldecode(ehs_char *url)
{
    ehs_char *src = url;
    ehs_char *dst = url;

    while (*src != '\0')
    {
        if ((*src == '%') && (isxdigit((int)*(src + 1))) && (isxdigit((int)*(src + 2))))
        {
            *src = *(src + 1);
            *(src + 1) = *(src + 2);
            *(src + 2) = '\0';
            *dst++ = strtol(src, NULL, 16);
            src += 3;
        }
        else
        {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}
