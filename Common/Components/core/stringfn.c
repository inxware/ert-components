/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* stringfn.c
 *
 * src file for string functions.  String functions manipulate strings.
 * Any function appearing in this file must also be listed in objRefTable.c
 * for it to be included in the list of functions available to the EHS.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
*/

#include <string.h>
#include <ctype.h>
#include "globals.h"

#include "ehs_fb_types.h"
#include "stringfn.h"
#include "fid.h"
#include "app_data.h"
#include "hal-api.h" /* Required for logging */


/******************************************************************************/
/* Define string_cat function block */

EHS_FB_FUNCTIONS_START(string_cat)

EHS_FB_FUNCTION_ENTRY("Run_CatString", 0x01, string_cat)
EHS_FB_FUNCTIONS_END

EHS_FB_IDENTIFY_FUNCTION(string_cat)
{
    if (EHS_FB_INIT_PARAMETERS && EhsStrcmp(EHS_FB_IDENTIFY_PARAMETERS,"null"))   /* replace these nulls with a const (inited from a header MACRO) to save memory*/
    {
        EHS_FB_IDENTIFY_MEMORY = EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS)+1;
    }
    else EHS_FB_IDENTIFY_MEMORY=0; /* This will cause a null to be sent as the parameter */
}

EHS_FB_INIT_FUNCTION(string_cat)
{
    if (EHS_FB_INIT_PARAMETERS && EHS_FB_INIT_CONTEXT)
    {
        EhsStrcpy(EHS_FB_INIT_CONTEXT,EHS_FB_INIT_PARAMETERS);
    }
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_cat)
{
    ehs_uint32 len1,len2,totallen;
    ehs_char* sOut=EHS_FB_OUT_S(0);
    ehs_char* s1=EHS_FB_IN_S(0);
    ehs_char* s2;
    if (EHS_FB_IN_CONNECTED(1)) s2=EHS_FB_IN_S(1);
    else s2=(char*)EHS_FB_RUN_CONTEXT;

    if (s2 && EHS_FB_IN_CONNECTED(0))
    {
        len1 = EhsStrlen(s1);
        len2 = EhsStrlen(s2);
        totallen = len1 + len2;
        /* Bound against this output row's own capacity - a per-connection size
         * can be smaller than EHS_DATA_TABLE_STRING_DEFAULT_LENGTH. */
        ehs_uint32 nOutMax = EhsDataString_maxLen(sOut);
        if (len1 > nOutMax)
            len1 = nOutMax;
        if (totallen > nOutMax)
            len2 = nOutMax - len1;
        EhsStrncpy(sOut, s1, len1);/*Use numbered copy here to avoid issue when the inout and output are the same buffer*/
        EhsStrncpy(&sOut[len1], s2, len2);
        sOut[len1 + len2] = '\0';// terminate it too
    }
    else
    {
        if (EHS_FB_IN_CONNECTED(0))
        {
            EhsStrcpy(sOut, s1);
        }
        else
        {
            EhsStrcpy(sOut, "");
        }
    }
    EHS_FB_FINISH(1);
    return;
}




/******************************************************************************/
/* Define string_cmp function block */

EHS_FB_FUNCTIONS_START(string_cmp)

EHS_FB_FUNCTION_ENTRY("Run_CmpString", 0x01, string_cmp)
EHS_FB_FUNCTIONS_END


EHS_FB_IDENTIFY_FUNCTION(string_cmp)
{
    if (EHS_FB_INIT_PARAMETERS && EhsStrcmp(EHS_FB_IDENTIFY_PARAMETERS,"null"))
    {

        EHS_FB_IDENTIFY_MEMORY = EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS)+1;
    }
    else EHS_FB_IDENTIFY_MEMORY=0; /* This will cause a null to be sent as the parameter */
}

EHS_FB_INIT_FUNCTION(string_cmp)
{

    if (EHS_FB_INIT_PARAMETERS && EHS_FB_INIT_CONTEXT)
    {
        EhsStrcpy(EHS_FB_INIT_CONTEXT,EHS_FB_INIT_PARAMETERS);
    }
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_cmp)
{
    char* s1;
    char* s2;
    int nCmp;

    s1=EHS_FB_IN_S(0);
    if (EHS_FB_IN_CONNECTED(1))
    {
        s2=EHS_FB_IN_S(1);
    }
    else   /* else use the iAB provided string */
    {
        s2=(char*)EHS_FB_RUN_CONTEXT;
    }
    if (s2 && EHS_FB_IN_CONNECTED(0))   /* we might be null */
    {
        nCmp = EhsStrcmp(s1, s2);
        if (!nCmp)
        {
            EHS_FB_OUT_B(0) = EHS_TRUE;
        }
        else
        {
            EHS_FB_OUT_B(0) = EHS_FALSE;
        }
    }
    else
    {
        EHS_FB_OUT_B(0) = EHS_FALSE;
    }
    EHS_FB_FINISH(1);
    return;
}
/******************************************************************************/
/* Shared plumbing for the string_format* blocks.
 *
 * Each block hands a printf format - from its Format parameter, or for the
 * 8-input variants from the fmt input port - to EhsSnprintf with a fixed number
 * of arguments chosen by a switch. That number must be exactly what printf will
 * consume, so it comes from EhsFormatScan (hal_string.c). Nothing here may
 * derive it by counting '%' characters.
 *
 * The parameter is escaped once, at init, in place in the context, so the
 * parameter path needs no run-time buffer. A format arriving on the fmt port
 * has to be copied instead: that memory is the upstream block's connection row,
 * and escaping it in place would both corrupt the producer's value and, on the
 * next scan, re-escape the result.
 */

/* EHS_FB_FORMAT_MAX, and the check that it nests inside EHS_STRING_LENGTH_MAX
 * and EHS_DATA_TABLE_STRING_DEFAULT_LENGTH, are in globals.h with the other
 * string size limits. */

/** Context shared by the string_format* blocks. */
typedef struct
{
    ehs_bool bReported;   /**< a bad format has already been logged for this block */
    ehs_char szFormat[1]; /**< escaped format from the parameter, sized at identify */
} EhsFormatCtxType;

/** What a run function needs in order to make the EhsSnprintf call. */
typedef struct
{
    const ehs_char* szFormat; /**< escaped and validated, never NULL */
    ehs_uint32      nArgs;    /**< arguments the format consumes, 0..nMaxArgs */
    ehs_bool        bValid;   /**< EHS_FALSE if the format was rejected */
} EhsFormatRunType;

/** Memory for a context holding szParams. Escaping never expands, so space
 * sized from the raw parameter always holds the escaped form. */
static ehs_uint32 EhsFormat_contextSize(const ehs_char* szParams)
{
    ehs_uint32 nLen = (szParams != NULL) ? (ehs_uint32)EhsStrlen(szParams) : 0u;
    return (ehs_uint32)sizeof(EhsFormatCtxType) + nLen; /* szFormat[1] covers the NUL */
}

/** Populate the context: copy the parameter in, then escape it where it lies. */
static ehs_bool EhsFormat_initContext(void* pContext, const ehs_char* szParams)
{
    EhsFormatCtxType* pCtx = (EhsFormatCtxType*)pContext;

    if (pCtx == NULL)
    {
        return EHS_TRUE; /* no parameter - the block formats an empty string */
    }
    pCtx->bReported = EHS_FALSE;
    pCtx->szFormat[0] = '\0';

    if (szParams != NULL)
    {
        ehs_uint32 nCap = (ehs_uint32)EhsStrlen(szParams) + 1u;
        EhsStrcpy(pCtx->szFormat, szParams);
        /* the whole parameter is one free-text value, so the SODL space
         * escaping has to be undone here - nothing else will do it */
        EhsParamUnescapeSpaces(pCtx->szFormat);
        (void)EhsParseEscapeChars(pCtx->szFormat, nCap, pCtx->szFormat);
    }
    return EHS_TRUE;
}

/** Log a rejected format once per block. Run functions execute every scan, so
 * an unlatched message would flood the log. */
static void EhsFormat_report(EhsFormatCtxType* pCtx, const ehs_char* szBlock,
                             const ehs_char* szReason, ehs_uint32 nOffset)
{
    if ((pCtx != NULL) && pCtx->bReported)
    {
        return;
    }
    if (pCtx != NULL)
    {
        pCtx->bReported = EHS_TRUE;
    }
    EHSH_LOG_ERROR("%s: format rejected at offset %u - %s",
                   szBlock, (unsigned int)nOffset,
                   (szReason != NULL) ? szReason : "unsupported format");
    /* EHSH_LOG_ERROR is empty when logging is compiled out */
    (void)szBlock;
    (void)szReason;
    (void)nOffset;
}

/**
 * Resolve, escape and validate the format for one run.
 *
 * @param pContext   the block's context (EhsFormatCtxType), may be NULL
 * @param szPortFmt  format from the fmt input port, or NULL when unconnected
 * @param szScratch  buffer for the fmt-port copy, may be NULL if the block has no fmt port
 * @param nScratchCap capacity of szScratch
 * @param eAllowed   argument class this block can supply
 * @param nMaxArgs   number of input ports the block can pass
 * @param szBlock    block name, for the log message
 */
static EhsFormatRunType EhsFormat_prepare(void* pContext,
                                          const ehs_char* szPortFmt,
                                          ehs_char* szScratch,
                                          ehs_uint32 nScratchCap,
                                          EhsFormatArgType eAllowed,
                                          ehs_uint32 nMaxArgs,
                                          const ehs_char* szBlock)
{
    EhsFormatCtxType* pCtx = (EhsFormatCtxType*)pContext;
    EhsFormatScanType scan;
    EhsFormatRunType res;

    res.szFormat = "";
    res.nArgs = 0u;
    res.bValid = EHS_TRUE;

    if (szPortFmt != NULL)
    {
        /* A run-time format is data, not SODL text: it carries real spaces, so
         * the parameter-only space un-escaping must not be applied to it. The
         * escape pass and everything after it are common to both paths. */
        ehs_bool bWhole = EhsParseEscapeChars(szScratch, nScratchCap, szPortFmt);
        res.szFormat = szScratch;
        if (!bWhole)
        {
            EhsFormat_report(pCtx, szBlock,
                             "format on the fmt input is too long (see EHS_FB_FORMAT_MAX)", 0u);
            res.bValid = EHS_FALSE; /* szFormat holds the truncated text, shown as plain text */
            return res;
        }
    }
    else if (pCtx != NULL)
    {
        res.szFormat = pCtx->szFormat; /* escaped at init */
    }
    else
    {
        /* nothing configured and nothing connected */
    }

    if (res.szFormat[0] == '\0')
    {
        return res; /* an empty format produces empty output, and is not an error */
    }

    if (!EhsFormatScan(&scan, res.szFormat, eAllowed, nMaxArgs))
    {
        EhsFormat_report(pCtx, szBlock, scan.szError, scan.nErrorOffset);
        res.bValid = EHS_FALSE;
        return res;
    }

    if (pCtx != NULL)
    {
        pCtx->bReported = EHS_FALSE; /* re-arm the log latch for the next bad format */
    }
    res.nArgs = scan.nArgs;
    return res;
}

/******************************************************************************/
/* Define FormatString function block 2 input*/

EHS_FB_FUNCTIONS_START(string_format)

EHS_FB_FUNCTION_ENTRY("run", 0x01, string_format)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(string_format)
{
    EHS_FB_IDENTIFY_MEMORY = EhsFormat_contextSize(EHS_FB_IDENTIFY_PARAMETERS);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(string_format)
{
    return EhsFormat_initContext(EHS_FB_INIT_CONTEXT, EHS_FB_INIT_PARAMETERS);
}

/**
 * Run the function. Use the inputs to format a string using our context
 * as the format parameter.
 *
 * This block has no fmt input port: the format is always the escaped parameter
 * held in the context.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_format)
{
    static const ehs_char* empty = "";
    const ehs_char* in_ptrs[2];
    ehs_char* szOut = EHS_FB_OUT_S(0);
    ehs_uint32 nOutCap = EHS_FB_OUT_S_CAP(0);
    EhsFormatRunType fmt;
    ehs_uint32 i;

    for (i = 0u; i < 2u; i++) /* missing inputs format as empty strings, silently */
    {
        in_ptrs[i] = EHS_FB_IN_CONNECTED(i) ? (const ehs_char*)EHS_FB_IN_S(i) : empty;
    }

    fmt = EhsFormat_prepare(EHS_FB_RUN_CONTEXT, NULL, NULL, 0u,
                            EHS_FMT_ARG_STRING, 2u, EHS_FB_NAME_string_format);

    if (!fmt.bValid)
    {
        /* Show the format as plain text. Passing it to EhsSnprintf as data
         * rather than as a format means no conversion is interpreted, and the
         * user sees what they typed instead of an empty string. */
        EhsSnprintf(szOut, nOutCap, "%s", fmt.szFormat);
        EHS_FB_FINISH(1);
        return;
    }

    switch (fmt.nArgs)
    {
    case 0:
    case 1:
        /* A zero-placeholder format still goes through EhsSnprintf so "%%"
         * renders as "%". The surplus argument is ignored by printf. */
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0]);
        break;
    default:
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1]);
        break;
    }

    EHS_FB_FINISH(1);
}


/* Define FormatString function block 8 input with dynamic formatting */

EHS_FB_FUNCTIONS_START(string_format8)

EHS_FB_FUNCTION_ENTRY("run", 0x01, string_format8)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(string_format8)
{
    EHS_FB_IDENTIFY_MEMORY = EhsFormat_contextSize(EHS_FB_IDENTIFY_PARAMETERS);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(string_format8)
{
    return EhsFormat_initContext(EHS_FB_INIT_CONTEXT, EHS_FB_INIT_PARAMETERS);
}

/**
 * Run the function. Use the inputs to format a string using our context
 * as the format parameter, or the fmt input port when it is connected.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_format8)
{
    static const ehs_char* empty = "";
    ehs_char szScratch[EHS_FB_FORMAT_MAX];
    const ehs_char* in_ptrs[8];
    ehs_char* szOut = EHS_FB_OUT_S(0);
    ehs_uint32 nOutCap = EHS_FB_OUT_S_CAP(0);
    EhsFormatRunType fmt;
    ehs_uint32 i;

    for (i = 0u; i < 8u; i++) /* missing inputs format as empty strings, silently */
    {
        in_ptrs[i] = EHS_FB_IN_CONNECTED(i) ? (const ehs_char*)EHS_FB_IN_S(i) : empty;
    }

    fmt = EhsFormat_prepare(EHS_FB_RUN_CONTEXT,
                            EHS_FB_IN_CONNECTED(8) ? (const ehs_char*)EHS_FB_IN_S(8) : NULL,
                            szScratch, (ehs_uint32)sizeof(szScratch),
                            EHS_FMT_ARG_STRING, 8u, EHS_FB_NAME_string_format8);

    if (!fmt.bValid)
    {
        /* Show the format as plain text. Passing it to EhsSnprintf as data
         * rather than as a format means no conversion is interpreted, and the
         * user sees what they typed instead of an empty string. */
        EhsSnprintf(szOut, nOutCap, "%s", fmt.szFormat);
        EHS_FB_FINISH(1);
        return;
    }

    switch (fmt.nArgs)
    {
    case 0 :
    case 1 :
        /* A zero-placeholder format still goes through EhsSnprintf so "%%"
         * renders as "%". The surplus argument is ignored by printf. */
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0]);
        break;
    case 2 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1]);
        break;
    case 3 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2]);
        break;
    case 4 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3]);
        break;
    case 5 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4]);
        break;
    case 6 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4], in_ptrs[5]);
        break;
    case 7 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4], in_ptrs[5], in_ptrs[6]);
        break;
    default :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4], in_ptrs[5], in_ptrs[6], in_ptrs[7]);
        break;
    }
    EHS_FB_FINISH(1);
}

/* Define FormatString function block 8 input integer with dynamic formatting */

EHS_FB_FUNCTIONS_START(string_format8_int)

EHS_FB_FUNCTION_ENTRY("run", 0x01, string_format8_int)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(string_format8_int)
{
    EHS_FB_IDENTIFY_MEMORY = EhsFormat_contextSize(EHS_FB_IDENTIFY_PARAMETERS);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(string_format8_int)
{
    return EhsFormat_initContext(EHS_FB_INIT_CONTEXT, EHS_FB_INIT_PARAMETERS);
}

/**
 * Run the function. Use the inputs to format a string using our context
 * as the format parameter, or the fmt input port when it is connected.
 *
 * An unconnected numeric input formats as 0; an integer conversion has no
 * blank rendering.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_format8_int)
{
    ehs_char szScratch[EHS_FB_FORMAT_MAX];
    ehs_sint32 in_ptrs[8];
    ehs_char* szOut = EHS_FB_OUT_S(0);
    ehs_uint32 nOutCap = EHS_FB_OUT_S_CAP(0);
    EhsFormatRunType fmt;
    ehs_uint32 i;

    for (i = 0u; i < 8u; i++)
    {
        in_ptrs[i] = EHS_FB_IN_CONNECTED(i) ? EHS_FB_IN_I(i) : 0;
    }

    fmt = EhsFormat_prepare(EHS_FB_RUN_CONTEXT,
                            EHS_FB_IN_CONNECTED(8) ? (const ehs_char*)EHS_FB_IN_S(8) : NULL,
                            szScratch, (ehs_uint32)sizeof(szScratch),
                            EHS_FMT_ARG_INT, 8u, EHS_FB_NAME_string_format8_int);

    if (!fmt.bValid)
    {
        /* Show the format as plain text. Passing it to EhsSnprintf as data
         * rather than as a format means no conversion is interpreted, and the
         * user sees what they typed instead of an empty string. */
        EhsSnprintf(szOut, nOutCap, "%s", fmt.szFormat);
        EHS_FB_FINISH(1);
        return;
    }

    switch (fmt.nArgs)
    {
    case 0 :
    case 1 :
        /* A zero-placeholder format still goes through EhsSnprintf so "%%"
         * renders as "%". The surplus argument is ignored by printf. */
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0]);
        break;
    case 2 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1]);
        break;
    case 3 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2]);
        break;
    case 4 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3]);
        break;
    case 5 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4]);
        break;
    case 6 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4], in_ptrs[5]);
        break;
    case 7 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4], in_ptrs[5], in_ptrs[6]);
        break;
    default :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4], in_ptrs[5], in_ptrs[6], in_ptrs[7]);
        break;
    }
    EHS_FB_FINISH(1);
}

#ifdef EHS_TARGET_FP_SUPPORT
/* Define FormatString function block 8 input with dynamic formatting */

EHS_FB_FUNCTIONS_START(string_format8_real)

EHS_FB_FUNCTION_ENTRY("run", 0x01, string_format8_real)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(string_format8_real)
{
    EHS_FB_IDENTIFY_MEMORY = EhsFormat_contextSize(EHS_FB_IDENTIFY_PARAMETERS);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(string_format8_real)
{
    return EhsFormat_initContext(EHS_FB_INIT_CONTEXT, EHS_FB_INIT_PARAMETERS);
}

/**
 * Run the function. Use the inputs to format a string using our context
 * as the format parameter, or the fmt input port when it is connected.
 *
 * An unconnected numeric input formats as 0. Any real conversion is accepted:
 * "%f", "%.2f", "%8.3f", "%e", "%g".
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_format8_real)
{
    ehs_char szScratch[EHS_FB_FORMAT_MAX];
    ehs_float in_ptrs[8];
    ehs_char* szOut = EHS_FB_OUT_S(0);
    ehs_uint32 nOutCap = EHS_FB_OUT_S_CAP(0);
    EhsFormatRunType fmt;
    ehs_uint32 i;

    for (i = 0u; i < 8u; i++)
    {
        in_ptrs[i] = EHS_FB_IN_CONNECTED(i) ? EHS_FB_IN_F(i) : (ehs_float)0;
    }

    fmt = EhsFormat_prepare(EHS_FB_RUN_CONTEXT,
                            EHS_FB_IN_CONNECTED(8) ? (const ehs_char*)EHS_FB_IN_S(8) : NULL,
                            szScratch, (ehs_uint32)sizeof(szScratch),
                            EHS_FMT_ARG_REAL, 8u, EHS_FB_NAME_string_format8_real);

    if (!fmt.bValid)
    {
        /* Show the format as plain text. Passing it to EhsSnprintf as data
         * rather than as a format means no conversion is interpreted, and the
         * user sees what they typed instead of an empty string. */
        EhsSnprintf(szOut, nOutCap, "%s", fmt.szFormat);
        EHS_FB_FINISH(1);
        return;
    }

    switch (fmt.nArgs)
    {
    case 0 :
    case 1 :
        /* A zero-placeholder format still goes through EhsSnprintf so "%%"
         * renders as "%". The surplus argument is ignored by printf. */
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0]);
        break;
    case 2 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1]);
        break;
    case 3 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2]);
        break;
    case 4 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3]);
        break;
    case 5 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4]);
        break;
    case 6 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4], in_ptrs[5]);
        break;
    case 7 :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4], in_ptrs[5], in_ptrs[6]);
        break;
    default :
        EhsSnprintf(szOut, nOutCap, fmt.szFormat, in_ptrs[0], in_ptrs[1], in_ptrs[2], in_ptrs[3], in_ptrs[4], in_ptrs[5], in_ptrs[6], in_ptrs[7]);
        break;
    }
    EHS_FB_FINISH(1);
}
#endif

/* Define string scan function block with up to 8 outputs real and dynamic formatting */

EHS_FB_FUNCTIONS_START(stringfn_scanf8)

EHS_FB_FUNCTION_ENTRY("run", 0x01, stringfn_scanf8)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(stringfn_scanf8)
{
    EHS_FB_IDENTIFY_MEMORY = EhsFormat_contextSize(EHS_FB_IDENTIFY_PARAMETERS);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(stringfn_scanf8)
{
    /* Shares EhsFormatCtxType with the format blocks for its log latch. The
     * backslash escaping is not done here: this block rewrites the format at
     * run time and escapes the result, so doing it twice is avoided by doing
     * it once, there. */
    EhsFormatCtxType* pCtx = (EhsFormatCtxType*)EHS_FB_INIT_CONTEXT;

    if (pCtx == NULL)
    {
        return EHS_TRUE;
    }
    pCtx->bReported = EHS_FALSE;
    pCtx->szFormat[0] = '\0';
    if (EHS_FB_INIT_PARAMETERS != NULL)
    {
        EhsStrcpy(pCtx->szFormat, EHS_FB_INIT_PARAMETERS);
        /* the whole parameter is one free-text format, so the SODL space
         * escaping has to be undone here - nothing else will do it */
        EhsParamUnescapeSpaces(pCtx->szFormat);
    }
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function. Use the inputs to format a string using our context
 * as the format parameter.
 *
 * @todo this is not reobust for non string formats (e.g. only does %s , others will cause a crash!
 * @todo  same string issue for sprintf
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(stringfn_scanf8)
{
    static const ehs_uint8 SCANF_INPUT_FMT_PORT = 0;
    static const ehs_uint8 SCANF_INPUT_STR_PORT = 1;

    ehs_char* out_ptrs[EHS_SCANF_MAX_ARGS];
    ehs_uint32 anMaxChars[EHS_SCANF_MAX_ARGS];
    /* One buffer, not two: the width injection runs first and the escape pass
     * then runs over its result in place. The two are independent - escaping
     * only rewrites backslash pairs, which are never part of a conversion
     * specifier - and this runs on the 2 kB dbgconsole stack. */
    ehs_char bounded[EHS_FB_FORMAT_MAX];
    EhsScanfBuildType build;
    EhsFormatCtxType* pCtx;
    const ehs_char* fmt;
    ehs_uint32 i;
    ehs_sint32 nAssigned = 0;

    /* Every output is blanked first, so a conversion that does not match leaves
     * an empty string rather than the previous run's value. anMaxChars is what
     * bounds sscanf: each row can have its own capacity. */
    for (i = 0u; i < EHS_SCANF_MAX_ARGS; i++)
    {
        out_ptrs[i] = EHS_FB_OUT_S(i);
        anMaxChars[i] = EHS_FB_OUT_S_MAXLEN(i);
        if (out_ptrs[i] != NULL)
        {
            out_ptrs[i][0] = '\0';
        }
    }

    pCtx = (EhsFormatCtxType*)EHS_FB_RUN_CONTEXT;
    fmt = EHS_FB_IN_CONNECTED(SCANF_INPUT_FMT_PORT)
              ? (const ehs_char*)EHS_FB_IN_S(SCANF_INPUT_FMT_PORT)
              : ((pCtx != NULL) ? (const ehs_char*)pCtx->szFormat : (const ehs_char*)NULL);

    if ((fmt == NULL) || (fmt[0] == '\0'))
    {
        EHS_FB_FINISH(1); /* an empty format is not an error */
        return;
    }

    if (!EhsScanfFormatBuild(bounded, (ehs_uint32)sizeof(bounded), fmt,
                             anMaxChars, EHS_SCANF_MAX_ARGS, &build))
    {
        /* latched: a run function executes every scan */
        EhsFormat_report(pCtx, "stringfn_scanf8",
                         build.szError, build.nErrorOffset);
        EHS_FB_FINISH(2);
        return;
    }
    if (pCtx != NULL)
    {
        pCtx->bReported = EHS_FALSE; /* re-arm for the next bad format */
    }

    /* in place: the escape conversion never expands */
    (void)EhsParseEscapeChars(bounded, (ehs_uint32)sizeof(bounded), bounded);

    if (EHS_FB_IN_CONNECTED(SCANF_INPUT_STR_PORT))
    {
        const ehs_char* szIn = EHS_FB_IN_S(SCANF_INPUT_STR_PORT);
        switch (build.nArgs)
        {
        case 0 :
            nAssigned = (ehs_sint32)EhsSscanf(szIn, bounded);
            break;
        case 1 :
            nAssigned = (ehs_sint32)EhsSscanf(szIn, bounded, out_ptrs[0]);
            break;
        case 2 :
            nAssigned = (ehs_sint32)EhsSscanf(szIn, bounded, out_ptrs[0], out_ptrs[1]);
            break;
        case 3 :
            nAssigned = (ehs_sint32)EhsSscanf(szIn, bounded, out_ptrs[0], out_ptrs[1], out_ptrs[2]);
            break;
        case 4 :
            nAssigned = (ehs_sint32)EhsSscanf(szIn, bounded, out_ptrs[0], out_ptrs[1], out_ptrs[2], out_ptrs[3]);
            break;
        case 5 :
            nAssigned = (ehs_sint32)EhsSscanf(szIn, bounded, out_ptrs[0], out_ptrs[1], out_ptrs[2], out_ptrs[3],
                                              out_ptrs[4]);
            break;
        case 6 :
            nAssigned = (ehs_sint32)EhsSscanf(szIn, bounded, out_ptrs[0], out_ptrs[1], out_ptrs[2], out_ptrs[3],
                                              out_ptrs[4], out_ptrs[5]);
            break;
        case 7 :
            nAssigned = (ehs_sint32)EhsSscanf(szIn, bounded, out_ptrs[0], out_ptrs[1], out_ptrs[2], out_ptrs[3],
                                              out_ptrs[4], out_ptrs[5], out_ptrs[6]);
            break;
        default :
            nAssigned = (ehs_sint32)EhsSscanf(szIn, bounded, out_ptrs[0], out_ptrs[1], out_ptrs[2], out_ptrs[3],
                                              out_ptrs[4], out_ptrs[5], out_ptrs[6], out_ptrs[7]);
            break;
        }

        if (nAssigned < 0) /* EOF - nothing matched */
        {
            nAssigned = 0;
        }
        /* %c writes an exact character count and no terminator, so terminate
         * the conversions scanf reports it actually assigned. */
        for (i = 0u; (i < build.nArgs) && ((ehs_sint32)i < nAssigned); i++)
        {
            if (build.anFixedWidth[i] > 0u)
            {
                out_ptrs[i][build.anFixedWidth[i]] = '\0';
            }
        }
    }

    EHS_FB_FINISH(1);
}












/******************************************************************************/
/* Define string_len function block */

EHS_FB_FUNCTIONS_START(string_len)

EHS_FB_FUNCTION_ENTRY("Run_LenString", 0x01, string_len)
EHS_FB_FUNCTIONS_END

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_len)
{
    EHS_FB_OUT_I(0) = (int)EhsStrlen(EHS_FB_IN_S(0)); //TODO:STRINGLENGTH! - should use a max string pending the length of the input.
    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define string_find function block */

EHS_FB_FUNCTIONS_START(string_find)

EHS_FB_FUNCTION_ENTRY("Run_FindString", 0x01, string_find)
EHS_FB_FUNCTIONS_END

struct String_find_struct
{
    ehs_bool backwards;
    ehs_char findstring[1]; /* sized at identify from the parameter */
} ;

EHS_FB_IDENTIFY_FUNCTION(string_find)
{
    /* The search string comes out of the parameter, so it cannot be longer
     * than the parameter. Sizing from a string-table constant instead cost
     * EHS_DATA_TABLE_STRING_DEFAULT_LENGTH - 32 KB on base_full - per instance. */
    ehs_uint32 nLen = (EHS_FB_IDENTIFY_PARAMETERS != NULL)
                          ? (ehs_uint32)EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS) : 0u;
    EHS_FB_IDENTIFY_MEMORY = (ehs_uint32)sizeof(struct String_find_struct) + nLen;
}

EHS_FB_INIT_FUNCTION(string_find)
{
    struct String_find_struct *parms=(struct String_find_struct*)EHS_FB_INIT_CONTEXT;
    if (parms == NULL)
    {
        return EHS_TRUE;
    }
    parms->backwards = EHS_FALSE;
    parms->findstring[0] = '\0';
    if (EHS_FB_INIT_PARAMETERS)
    {
        /* findstring holds EhsStrlen(parameters) characters plus the NUL, so a
         * width of that length can never overflow it */
        ehs_char szScanFmt[32];
        EhsSnprintf(szScanFmt, sizeof(szScanFmt), "%%hhd%%%us",
                    (unsigned int)EhsStrlen(EHS_FB_INIT_PARAMETERS));
        EhsSscanf(EHS_FB_INIT_PARAMETERS,szScanFmt,&parms->backwards,parms->findstring);
        EhsParamUnescapeSpaces(parms->findstring);
    }
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_find)
{
    char* s1;// = &szData1[0];
    char* s2;// = &szData2[0];
    char* s3;// = &szData3[0];
    ehs_uint16 i,j;
    struct String_find_struct *parms=(struct String_find_struct*)EHS_FB_RUN_CONTEXT;
    s1=EHS_FB_IN_S(0); /*OK unchecked -  This will point to our null string if not conncted */
    if (EHS_FB_IN_CONNECTED(1)) s2=EHS_FB_IN_S(1);
    else s2=(char*)parms->findstring;
    if (s1 == NULL )
    {
        return;
    }
    if (parms->backwards == EHS_FALSE)
    {
        s3 = EhsStrstr(s1, s2);
    }
    else   /* reverse search not so easy... */
    {
        ehs_uint16 targstrlen=EhsStrlen(s1); /* @todo string lenght must be less the 65K length. */
        ehs_uint16 matcstrlen=EhsStrlen(s2);
        j=0;
        s3 = NULL; /* Assume No match */
        for (i=targstrlen-matcstrlen; i>0; i--)
        {
            if (s1[i+1] == s2[0])
            {
                s3 = &s1[i+1]; /* set the output to the beginning assuming a match */
                for (j=1; j<matcstrlen; j++)
                {
                    if (s1[i+j+1] != s2[j])
                    {
                        break;
                    }
                }
                if (j != matcstrlen) s3 = NULL; /* We didn't get a match so dump - yes this works with 1 char patterns*/
                else
                {
                    break; /* we've got one so exit with S3*/

                }
            }
            else   /* optimised*/
            {
                s3 = NULL; /* No match */
            }
        }
    }

    if (s3 && s2 && EHS_FB_IN_CONNECTED(0)) /* Only if we have something to find */
    {
        EHS_FB_OUT_S_SET(0, s3);
        EHS_FB_OUT_I(2) = s3-s1; /* return the 0-based index - Todo we should do pointer arithmatic.. */
        EHS_FB_OUT_B(1) = EHS_TRUE;
    }
    else
    {
        EHS_FB_OUT_S_SET(0, "");
        EHS_FB_OUT_I(2) = -1; /* further error signal */
        EHS_FB_OUT_B(1) = EHS_FALSE;
    }
    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define string_toUpper function block */

EHS_FB_FUNCTIONS_START(string_toUpper)

EHS_FB_FUNCTION_ENTRY("Run_ToUpperString", 0x01, string_toUpper)
EHS_FB_FUNCTIONS_END

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_toUpper)
{
    char* s1 = EHS_FB_IN_S(0);
    char* s2 = EHS_FB_OUT_S(0);
    ehs_uint32 len = EhsStrlen(s1);
    ehs_uint16 i;
    for (i=0; i<len; i++)
    {
        s2[i]=toupper(s1[i]);
    }
    s2[len]='\0';
    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define string_toLower function block */

EHS_FB_FUNCTIONS_START(string_toLower)

EHS_FB_FUNCTION_ENTRY("Run_ToLowerString", 0x01, string_toLower)
EHS_FB_FUNCTIONS_END

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_toLower)
{
    char* s1 = EHS_FB_IN_S(0);
    char* s2 = EHS_FB_OUT_S(0);
    ehs_uint32 len = EhsStrlen(s1);
    ehs_uint16 i;
    for (i=0; i<len; i++)
    {
        s2[i]=tolower(s1[i]);
    }
    s2[len]='\0';
    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define string_charAt function block */

EHS_FB_FUNCTIONS_START(string_charAt)

EHS_FB_FUNCTION_ENTRY("Run_CharAtString", 0x01, string_charAt)
EHS_FB_FUNCTIONS_END

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 * 
 * TRUSTED CLIENT
 */
EHS_FB_RUN_FUNCTION(string_charAt)
{

    char *szData1;
    char *szData2;
    int index;

    szData1 = EHS_FB_IN_S(0);
    szData2 = EHS_FB_OUT_S(0);
    index = EHS_FB_IN_I(1);


    if (EHS_FB_IN_CONNECTED(1) && index >= 0 && index < EhsStrlen(szData1))
    {
        szData2[0] = szData1[index];
        szData2[1] = '\0';
    }
    else
    {
        szData2[0] = '\0';
    }


    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define string_strAt function block */

EHS_FB_FUNCTIONS_START(string_strAt)

EHS_FB_FUNCTION_ENTRY("Run_StrAtString", 0x01, string_strAt)
EHS_FB_FUNCTIONS_END

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_strAt)
{
    char *szData1;
    char *szData2;
    int index;

    szData1 = EHS_FB_IN_S(0);
    szData2 = EHS_FB_OUT_S(0);
    index = EHS_FB_IN_I(1);


    if (EHS_FB_IN_CONNECTED(1) && index >= 0 && index < EhsStrlen(szData1))
    {
        EhsDataString_set(szData2, &szData1[index]);
    }
    else
    {
        szData2[0] = '\0';
    }


    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define string_insert function block */

EHS_FB_FUNCTIONS_START(string_insert)

EHS_FB_FUNCTION_ENTRY("Run_InsertString", 0x01, string_insert)
EHS_FB_FUNCTIONS_END


struct EhsT_Insertstringparms
{
    ehs_uint16 index;
    ehs_char string[1]; /* sized at identify from the parameter */
};

EHS_FB_IDENTIFY_FUNCTION(string_insert)
{
    /* The insert string comes out of the parameter, so it cannot be longer
     * than the parameter. Sizing from a string-table constant instead cost
     * EHS_DATA_TABLE_STRING_DEFAULT_LENGTH - 32 KB on base_full - per instance. */
    ehs_uint32 nLen = (EHS_FB_IDENTIFY_PARAMETERS != NULL)
                          ? (ehs_uint32)EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS) : 0u;
    EHS_FB_IDENTIFY_MEMORY = (ehs_uint32)sizeof(struct EhsT_Insertstringparms) + nLen;
}

EHS_FB_INIT_FUNCTION(string_insert)
{
    struct EhsT_Insertstringparms *parms=EHS_FB_INIT_CONTEXT;
    if (parms == NULL)
    {
        return EHS_TRUE;
    }
    parms->index=0;
    parms->string[0]='\0';
    if (EHS_FB_INIT_PARAMETERS)
    {
        /* string holds EhsStrlen(parameters) characters plus the NUL, so a
         * width of that length can never overflow it */
        ehs_char szScanFmt[32];
        EhsSnprintf(szScanFmt, sizeof(szScanFmt), "%%hd%%%us",
                    (unsigned int)EhsStrlen(EHS_FB_INIT_PARAMETERS));
        EhsSscanf(EHS_FB_INIT_PARAMETERS, szScanFmt,&parms->index,parms->string);
        EhsParamUnescapeSpaces(parms->string);
    }
    if (EhsStrcmp(parms->string,"null")==0||EhsStrcmp(parms->string,"NULL")==0) parms->string[0]='\0';
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_insert)
{

    char *szData1; //base string
    char *szData2; //string to insert
    char *tmp;
    int index;
    struct EhsT_Insertstringparms *parms = EHS_FB_RUN_CONTEXT;

    if (EHS_FB_IN_CONNECTED(0))
    {
        tmp=EHS_FB_OUT_S(0);
        szData1 = EHS_FB_IN_S(0);

        if (EHS_FB_IN_CONNECTED(1))
            szData2 = EHS_FB_IN_S(1);
        else
            szData2 = parms->string;

        if (EHS_FB_IN_CONNECTED(2))
            index = EHS_FB_IN_I(2);
        else
            index = parms->index;

        if (index >= 0 && index < EhsStrlen(szData1))
        {
            /* Built in three pieces. The prefix is copied by offset (numbered
             * copy, so an in-place edit where input and output share a row still
             * works); the two appends find the current end of the row for
             * themselves and clamp to its capacity. */
            ehs_uint32 nMax = EhsDataString_maxLen(tmp);
            ehs_uint32 nPos = ((ehs_uint32)index > nMax) ? nMax : (ehs_uint32)index;
            EhsStrncpy(tmp, szData1, nPos);
            tmp[nPos] = '\0';
            EhsDataString_append(tmp, szData2);
            EhsDataString_append(tmp, &szData1[index]);
        }
        else
            EHS_FB_OUT_S_SET(0, "");

    }
    else
        EHS_FB_OUT_S_SET(0, "");/* Nout if there is no inout */
    EHS_FB_FINISH(1);
    return;
}


struct EhsT_Substringparms
{
    ehs_uint16 index;
    ehs_uint16 length;
};
/******************************************************************************/
/* Define string_sub function block */

EHS_FB_FUNCTIONS_START(string_sub)
EHS_FB_FUNCTION_ENTRY("Run_SubString", 0x01, string_sub)
EHS_FB_FUNCTIONS_END

EHS_FB_IDENTIFY_FUNCTION(string_sub)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(struct EhsT_Substringparms);
}

EHS_FB_INIT_FUNCTION(string_sub)
{
    struct EhsT_Substringparms *parms=EHS_FB_INIT_CONTEXT;
    parms->index=0;
    parms->length=0;
    EhsSscanf(EHS_FB_INIT_PARAMETERS, "%hd%hd",&parms->index,&parms->length);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_sub)
{
    char *szData1;
    char *szData2;
    int start, length, i, finish;

    struct EhsT_Substringparms* parms = (struct EhsT_Substringparms *) EHS_FB_RUN_CONTEXT;

    //	strcpy(szData1, EHS_FB_IN_S(0));
    szData1 = EHS_FB_IN_S(0);
    szData2 = EHS_FB_OUT_S(0);

    if (EHS_FB_IN_CONNECTED(1))
        start = EHS_FB_IN_I(1);
    else start=parms->index;
    if (EHS_FB_IN_CONNECTED(2))
        length = EHS_FB_IN_I(2);
    else length=parms->length;
    finish = start + length;
    if (finish > EhsStrlen(szData1))
    {
        finish = EhsStrlen(szData1);
    }
    if (EHS_FB_IN_CONNECTED(0) && start >= 0 && start
            < EhsStrlen(szData1))
    {
        szData2[0] = '\0'; /*todo we don't really need this if start=i after this loop? */
        for (i = start; i < finish; i++)
        {
            szData2[i - start] = szData1[i];
        }
        szData2[i - start] = '\0';
    }
    else   /* truncate if nothing sensible */
    {
        szData2[0] = '\0';
    }
    EHS_FB_FINISH(1);
    return;
}
