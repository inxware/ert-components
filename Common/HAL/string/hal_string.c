/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
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

#include "hal_string.h"
#include "globals.h"
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
EHS_GLOBAL ehs_bool EhsIsAlpha(char c)
{
    return (EHS_TO_LOWER(c) >= 'a' && EHS_TO_LOWER(c) <= 'z');
}

/**
 * Determine if character is a-z or A-Z or 0-9
 */
EHS_GLOBAL ehs_bool EhsIsAlNum(char c)
{
    return ((EHS_TO_LOWER(c) >= 'a' && EHS_TO_LOWER(c) <= 'z')||
            (c >= '0' && c <= '9'));

}

/**
 * Determine if character is a-f or A-F or 0-9
 */
EHS_GLOBAL ehs_bool EhsIsHexNum(char ch)
{
    return (ch >= '0' && ch <= '9') ||
           (ch >= 'A' && ch <= 'F') ||
           (ch >= 'a' && ch <= 'f');
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
 * Read a word from a line of a SODL file. A word is a sequence of characters that do not include whitespace.
 *
 * @param output Word read from input. NULL just to skip the word
 * @param input String containing SODL input.
 * @return Pointer to updated input string (i.e. after reading the integer)
 */
const ehs_char* EhsGetWordFromString(ehs_char * output, const ehs_char* input)
{
    int i=0;
    input = EhsStrTrimL(input);

    EHS_TRACE_FUNC2(EHS_TRACE_FLAG_PARSER|EHS_TRACE_FLAG_ATOM, EhsGetWordFromString, "%x,%x", output, input);

    if (!input||*input==25||*input==0)
    {
        input = NULL;
        if (output)
        {
            *output = '\0';
        }
    }
    else
    {
        for (i=0; (i<EHS_STRING_LENGTH_MAX)&&('\0' != *input)&&(!EhsStrIsSpace(*input))&&(*input != 1); i++)
        {
            if (output)
            {
                *(output++)=*(input++);
            }
            else
            {
                input++;
            }
        }
        if (output)
        {
            *output=0;
        }
    }

    return input;
}

/*** @brief This function replaces escaped control characters into the control characters
 *
 *
 *  */
ehs_bool EhsParseEscapeChars(ehs_char * output, const ehs_char* input)
{
    int i,j=0;
    for (i = 0 ; i <= EhsStrlen(input); i++)
    {
        if ( input[i] != '\\' )
        {
            output[j++] = input[i];
        }
        else
        {
            i++;
            switch (input[i])
            {
            case 'n':
                output[j++] = '\n';
                break;
            case 't':
                output[j++] = '\t';
                break;
            case 'r' :
                output[j++] = '\r';
                break;
            case 'f':
                output[j++] = '\f';
                break;
            default: /* copy verbatim if not in above */
                output[j++] = input[i-1];
                output[j++] = input[i];
                break;
            }
        }
    }
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
EHS_GLOBAL ehs_uint8 EhsGetWordsFromString(ehs_char **outputs, const ehs_char* input, ehs_uint8 length)
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
            input = EhsGetWordFromString(in_temp, input);
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
