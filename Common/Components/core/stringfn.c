/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
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
        if (totallen >= EHS_DATA_TABLE_STRING_DEFAULT_LENGTH) // @TODO - we should using macro for checking max output port size
            len2 = EHS_DATA_TABLE_STRING_DEFAULT_LENGTH - len1;
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
    EHS_FB_IDENTIFY_MEMORY = EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS)+1;
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
    EhsStrcpy(EHS_FB_INIT_CONTEXT,EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function. Use the inputs to format a string using our context
 * as the format parameter.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_format)
{
    ehs_char escaped[EHS_DATA_TABLE_STRING_DEFAULT_LENGTH];

    ehs_char * empty="";
    ehs_char *in_ptrs[2];
    ehs_uint8 i;
    for (i=0; i<2; i++) /* if we have missing inputs we will insert empty strings */
    {
        if (EHS_FB_IN_CONNECTED(i))
        {
            in_ptrs[i]=EHS_FB_IN_S(i); /* point at the connections */
        }
        else in_ptrs[i]=empty;
    }

    EhsParseEscapeChars(escaped, EHS_FB_RUN_CONTEXT);
#ifdef INX_DEPRECATED
    if (EHS_FB_IN_CONNECTED(0) && EHS_FB_IN_CONNECTED(1))
    {
        EhsSprintf(EHS_FB_OUT_S(0),escaped,EHS_FB_IN_S(0),EHS_FB_IN_S(1));
    }
    else if (EHS_FB_IN_CONNECTED(0))
    {
        EhsSprintf(EHS_FB_OUT_S(0),escaped,EHS_FB_IN_S(0));
    }
    else if (EHS_FB_IN_CONNECTED(1))
    {
        EhsSprintf(EHS_FB_OUT_S(0),escaped,EHS_FB_IN_S(1));
    }
    else
    {
        EhsSprintf(EHS_FB_OUT_S(0),"%s",escaped);
    }
#else
    EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1]);
#endif

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
    EHS_FB_IDENTIFY_MEMORY = EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS)+1; /* only the formatting string is stored */
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
    EhsStrcpy(EHS_FB_INIT_CONTEXT,EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function. Use the inputs to format a string using our context
 * as the format parameter.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_format8)
{
    ehs_uint8 i,connectioncount=0;
    ehs_uint8 fmt_count=0;
    ehs_char * empty="";
    ehs_char * fmt;
    ehs_char *in_ptrs[8];
    ehs_char escaped[EHS_DATA_TABLE_STRING_DEFAULT_LENGTH];

    for (i=0; i<8; i++) /* if we have missing inputs we will insert empty strings */
    {
        if (EHS_FB_IN_CONNECTED(i))
        {
            in_ptrs[i]=EHS_FB_IN_S(i); /* point at the connections */
            connectioncount++;
        }
        else in_ptrs[i]=empty;
    }
    if (EHS_FB_IN_CONNECTED(8)) fmt=EHS_FB_IN_S(8);
    else fmt=EHS_FB_RUN_CONTEXT;
    for (i=0; i<EhsStrlen(fmt)-1; i++)
    {
        if (fmt[i]=='%' && fmt[i+1]=='s') fmt_count++;
    }

    EhsParseEscapeChars(escaped, fmt);

    for (i=0; i<EhsStrlen(escaped)-1; i++) // remove non-string formatters that would need a different sink pointer type.
    {
        if (i == 0 || escaped[i-1] != '*')    // we can parse discarded numbers
        {
            if (escaped[i] == '%' && (
                        escaped[i+1] == 'i' ||
                        escaped[i+1] == 'd' ||
                        escaped[i+1] == 'u' ||
                        escaped[i+1] == 'o' ||
                        escaped[i+1] == 'x' ||
                        escaped[i+1] == 'f' ||
                        escaped[i+1] == 'e' ||
                        escaped[i+1] == 'g' ||
                        escaped[i+1] == 'a' ||
                        escaped[i+1] == 'p' ||
                        escaped[i+1] == 'n' ||
                        escaped[i+1] == 'h' ||
                        escaped[i+1] == 'l' ||
                        escaped[i+1] == 'j' ||
                        escaped[i+1] == 'z' ||
                        escaped[i+1] == 't' ||
                        escaped[i+1] == 'L'
                    ))
            {
                EHSH_LOG_ERROR(" Formatter contains non string specifiers, which are not supported");
                EHS_FB_FINISH(1);
                return;
            }

        }
    }

    switch (fmt_count)
    {
    case 0 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped); /* no string insertions */
        break;
    case 1 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0]);
        break;
    case 2 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1]);
        break;
    case 3 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2]);
        break;
    case 4 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3]);
        break;
    case 5 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4]);
        break;
    case 6 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4],in_ptrs[5]);
        break;
    case 7 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4],in_ptrs[5],in_ptrs[6]);
        break;
    case 8 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4],in_ptrs[5],in_ptrs[6],in_ptrs[7]);
        break;
    default :
        EHSH_LOG_ERROR("Format Specifier in string_format8 contain more than 8 place holders"); /*Tdo should allow for a variable arg list, and should assert an error event of more then 8 placeholders are found*/
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
    EHS_FB_IDENTIFY_MEMORY = EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS)+1; /* only the formatting string is stored */
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
    EhsStrcpy(EHS_FB_INIT_CONTEXT,EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function. Use the inputs to format a string using our context
 * as the format parameter.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_format8_int)
{
    ehs_uint8 i,connectioncount=0;
    ehs_uint8 fmt_count=0;
    ehs_char * fmt;
    ehs_sint32 in_ptrs[8];
    ehs_char escaped[EHS_DATA_TABLE_STRING_DEFAULT_LENGTH];

    for (i=0; i<8; i++) /* if we have missing inputs we will insert empty strings */
    {
        if (EHS_FB_IN_CONNECTED(i))
        {
            in_ptrs[i]=EHS_FB_IN_I(i); /* point at the connections */
            connectioncount++;
        }
        else in_ptrs[i]=0;
    }
    if (EHS_FB_IN_CONNECTED(8)) fmt=EHS_FB_IN_S(8);
    else fmt=EHS_FB_RUN_CONTEXT;
    for (i=0; i<EhsStrlen(fmt)-1; i++)
    {
        int d = i+1;
        if (fmt[i]=='%' && (fmt[d]=='d' || fmt[d]=='u' || fmt[d]=='i' || fmt[d]=='o' || fmt[d]=='x' || fmt[d]=='X')) fmt_count++;
    }

    EhsParseEscapeChars(escaped, fmt);

    for (i=0; i<EhsStrlen(escaped)-1; i++) // remove non-integer formatters that would need a different sink pointer type.
    {
        if (i == 0 || escaped[i-1] != '*')    // we can parse discarded numbers
        {
            int d = i+1;
            if (escaped[i] == '%' && !(
                        escaped[d] == 'i' || // integer (base 10)
                        escaped[d] == 'd' || // decimal (integer) number (base 10)
                        escaped[d] == 'u' || // unsigned decimal (integer) number
                        escaped[d] == 'o' || // octal number (base 8)
                        escaped[d] == 'x' || // number in hexadecimal (base 16)
                        escaped[d] == 'X'
                    ))
            {
                EHSH_LOG_ERROR(" Formatter contains non integer specifiers, which are not supported");
                EHS_FB_FINISH(1);
                return;
            }

        }
    }

    switch (fmt_count)
    {
    case 0 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped); /* no string insertions */
        break;
    case 1 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0]);
        break;
    case 2 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1]);
        break;
    case 3 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2]);
        break;
    case 4 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3]);
        break;
    case 5 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4]);
        break;
    case 6 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4],in_ptrs[5]);
        break;
    case 7 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4],in_ptrs[5],in_ptrs[6]);
        break;
    case 8 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4],in_ptrs[5],in_ptrs[6],in_ptrs[7]);
        break;
    default :
        EHSH_LOG_ERROR("Format Specifier in string_format8_int contain more than 8 place holders"); /*Tdo should allow for a variable arg list, and should assert an error event of more then 8 placeholders are found*/
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
    EHS_FB_IDENTIFY_MEMORY = EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS)+1; /* only the formatting string is stored */
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
    EhsStrcpy(EHS_FB_INIT_CONTEXT,EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function. Use the inputs to format a string using our context
 * as the format parameter.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(string_format8_real)
{
    ehs_uint8 i,connectioncount=0;
    ehs_uint8 fmt_count=0;
    ehs_char * fmt;
    ehs_float in_ptrs[8];
    ehs_char escaped[EHS_DATA_TABLE_STRING_DEFAULT_LENGTH];

    for (i=0; i<8; i++) /* if we have missing inputs we will insert empty strings */
    {
        if (EHS_FB_IN_CONNECTED(i))
        {
            in_ptrs[i]=EHS_FB_IN_F(i); /* point at the connections */
            connectioncount++;
        }
        else in_ptrs[i]=(ehs_float)0;
    }
    if (EHS_FB_IN_CONNECTED(8)) fmt=EHS_FB_IN_S(8);
    else fmt=EHS_FB_RUN_CONTEXT;
    for (i=0; i<EhsStrlen(fmt)-1; i++)
    {
        if (fmt[i]=='%' && (fmt[i+1]=='.' /*&& fmt[i+2]=='2'*/ && fmt[i+3]== 'f')) fmt_count++;
    }

    EhsParseEscapeChars(escaped, fmt);

    for (i=0; i<EhsStrlen(escaped)-1; i++) // remove non-real formatters that would need a different sink pointer type.
    {
        if (i == 0 || escaped[i-1] != '*')    // we can parse discarded numbers
        {
            if (escaped[i] == '%' && !(
                        (escaped[i+1]=='.' /*&& escaped[i+2]=='2'*/ && escaped[i+3]== 'f')
                    ))
            {
                EHSH_LOG_ERROR(" Formatter contains non real specifiers, which are not supported");
                EHS_FB_FINISH(1);
                return;
            }
        }
    }

    switch (fmt_count)
    {
    case 0 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped); /* no string insertions */
        break;
    case 1 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0]);
        break;
    case 2 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1]);
        break;
    case 3 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2]);
        break;
    case 4 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3]);
        break;
    case 5 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4]);
        break;
    case 6 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4],in_ptrs[5]);
        break;
    case 7 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4],in_ptrs[5],in_ptrs[6]);
        break;
    case 8 :
        EhsSprintf(EHS_FB_OUT_S(0),escaped,in_ptrs[0],in_ptrs[1],in_ptrs[2],in_ptrs[3],in_ptrs[4],in_ptrs[5],in_ptrs[6],in_ptrs[7]);
        break;
    default :
        EHSH_LOG_ERROR("Format Specifier in string_format8_real contain more than 8 place holders"); /*Tdo should allow for a variable arg list, and should assert an error event of more then 8 placeholders are found*/
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
    EHS_FB_IDENTIFY_MEMORY = EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS)+1; /* only the formatting string is stored */
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
    EhsStrcpy(EHS_FB_INIT_CONTEXT,EHS_FB_INIT_PARAMETERS);
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
    ehs_uint8 i,connectioncount=0; /* not really used - we need to know sequence */
    ehs_uint8 fmt_count=0;
    //ehs_char * empty="";
    ehs_char * fmt;
    ehs_char *out_ptrs[8];
    ehs_char escaped[EHS_DATA_TABLE_STRING_DEFAULT_LENGTH];
    ehs_bool allgood = EHS_TRUE;


    if (EHS_FB_IN_CONNECTED(SCANF_INPUT_FMT_PORT)) {
        fmt=EHS_FB_IN_S(SCANF_INPUT_FMT_PORT);
    }else{ 
        fmt=EHS_FB_RUN_CONTEXT;
    }
    if (EhsStrlen(fmt) > 0)
    {
        for (i=0; i<EhsStrlen(fmt)-1; i++)
        {
            if (fmt[i]=='%' && (fmt[i+1]=='s' ||  fmt[i+1]=='['))  fmt_count++;
            /* also count fixed width chars and ID them so we can null blank where they are written to */
            if (	(fmt[i]=='%' && fmt[i+1]=='c') ||
                    ((fmt[i+1] >= '0' && fmt[i+1] <= '9') && (fmt[i+2] == 'c')) ||
                    ((fmt[i+1] >= '0' && fmt[i+1] <= '9') && (fmt[i+2] >= '0' && fmt[i+2] <= '9' && fmt[i+3] == 'c')) )
            {
                fmt_count++;

                if (fmt_count < 8 && EHS_FB_OUT_CONNECTED(fmt_count)) {
                    EhsMemset(EHS_FB_OUT_S(fmt_count),'\0',EHS_STRING_LENGTH_MAX-1); // null everywhere as scanf doesn't for characters
                }
            }

        }

        /* check we are all connected properly and fail if we are not */
        for (i=0; i<8; i++) /* if we have missing inputs we will insert empty strings */
        {
            if (EHS_FB_OUT_CONNECTED(i))
            {
                
                connectioncount++;
            }
            out_ptrs[i]=EHS_FB_OUT_S(i); /* point at the connections */
            out_ptrs[i][0]='\0';
        }

        EhsParseEscapeChars(escaped, fmt);
        for (i=0; i<EhsStrlen(escaped)-1; i++) // remove non-string formatters that would need a different sink pointer type.
        {
            if (i == 0 || escaped[i-1] != '*')    // we can parse discarded numbers
            {
                if (escaped[i] == '%' && (
                            escaped[i+1] == 'i' ||
                            escaped[i+1] == 'd' ||
                            escaped[i+1] == 'u' ||
                            escaped[i+1] == 'o' ||
                            escaped[i+1] == 'x' ||
                            escaped[i+1] == 'f' ||
                            escaped[i+1] == 'e' ||
                            escaped[i+1] == 'g' ||
                            escaped[i+1] == 'a' ||
                            escaped[i+1] == 'p' ||
                            escaped[i+1] == 'n' ||
                            escaped[i+1] == 'h' ||
                            escaped[i+1] == 'l' ||
                            escaped[i+1] == 'j' ||
                            escaped[i+1] == 'z' ||
                            escaped[i+1] == 't' ||
                            escaped[i+1] == 'L'
                        ))
                {
                    EHSH_LOG_ERROR(" Formatter contains non string specifiers, which are not supported");
                    allgood = EHS_FALSE;
                }
            }
        }

        if (allgood && EHS_FB_IN_CONNECTED(SCANF_INPUT_STR_PORT))
        {
            switch (fmt_count)
            {
            case 0 :
                EhsSscanf(EHS_FB_IN_S(SCANF_INPUT_STR_PORT),escaped); /* no string insertions */
                break;
            case 1 :
                EhsSscanf(EHS_FB_IN_S(SCANF_INPUT_STR_PORT),escaped,out_ptrs[0]);
                break;
            case 2 :
                EhsSscanf(EHS_FB_IN_S(SCANF_INPUT_STR_PORT),escaped,out_ptrs[0],out_ptrs[1]);
                break;
            case 3 :
                EhsSscanf(EHS_FB_IN_S(SCANF_INPUT_STR_PORT),escaped,out_ptrs[0],out_ptrs[1],out_ptrs[2]);
                break;
            case 4 :
                EhsSscanf(EHS_FB_IN_S(SCANF_INPUT_STR_PORT),escaped,out_ptrs[0],out_ptrs[1],out_ptrs[2],out_ptrs[3]);
                break;
            case 5 :
                EhsSscanf(EHS_FB_IN_S(SCANF_INPUT_STR_PORT),escaped,out_ptrs[0],out_ptrs[1],out_ptrs[2],out_ptrs[3],out_ptrs[4]);
                break;
            case 6 :
                EhsSscanf(EHS_FB_IN_S(SCANF_INPUT_STR_PORT),escaped,out_ptrs[0],out_ptrs[1],out_ptrs[2],out_ptrs[3],out_ptrs[4],out_ptrs[5]);
                break;
            case 7 :
                EhsSscanf(EHS_FB_IN_S(SCANF_INPUT_STR_PORT),escaped,out_ptrs[0],out_ptrs[1],out_ptrs[2],out_ptrs[3],out_ptrs[4],out_ptrs[5],out_ptrs[6]);
                break;
            case 8 :
                EhsSscanf(EHS_FB_IN_S(SCANF_INPUT_STR_PORT),escaped,out_ptrs[0],out_ptrs[1],out_ptrs[2],out_ptrs[3],out_ptrs[4],out_ptrs[5],out_ptrs[6],out_ptrs[7]);
                break;
            default :
                EHSH_LOG_ERROR("Format Specifier in string_format8 contain more than 8 place holders"); /*Tdo should allow for a variable arg list, and should assert an error event of more then 8 placeholders are found*/
                allgood = EHS_FALSE;
            }
        }
        if ( allgood ) EHS_FB_FINISH(1);
        else EHS_FB_FINISH(2);
    }
    else
    {
        EHS_FB_FINISH(1); /* Don't show an error for an empty format */
    }
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
    ehs_char findstring[EHS_DATA_TABLE_STRING_DEFAULT_LENGTH];
} ;

EHS_FB_IDENTIFY_FUNCTION(string_find)
{
    //if (EHS_FB_INIT_PARAMETERS && EhsStrcmp(EHS_FB_IDENTIFY_PARAMETERS,"null")) { /* replace these nulls with a const (inited from a header MACRO) to save memory*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(struct String_find_struct);//EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS)+1;
    //	}
    //else EHS_FB_IDENTIFY_MEMORY=0; /* This will cause a null to be sent as the parameter */
}

EHS_FB_INIT_FUNCTION(string_find)
{
    struct String_find_struct *parms=(struct String_find_struct*)EHS_FB_INIT_CONTEXT;
    if (EHS_FB_INIT_PARAMETERS)
    {
        EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhd%s",&parms->backwards,parms->findstring);
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
        EhsStrcpy(EHS_FB_OUT_S(0), s3);
        EHS_FB_OUT_I(2) = s3-s1; /* return the 0-based index - Todo we should do pointer arithmatic.. */
        EHS_FB_OUT_B(1) = EHS_TRUE;
    }
    else
    {
        strcpy(EHS_FB_OUT_S(0), "");
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
        strcpy(szData2, &szData1[index]);
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
    ehs_char string[EHS_DATA_TABLE_STRING_DEFAULT_LENGTH]; /* should make this dynamic */
};

EHS_FB_IDENTIFY_FUNCTION(string_insert)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(struct EhsT_Insertstringparms);
}

EHS_FB_INIT_FUNCTION(string_insert)
{
    struct EhsT_Insertstringparms *parms=EHS_FB_INIT_CONTEXT;
    parms->index=0;
    parms->string[0]='\0';
    EhsSscanf(EHS_FB_INIT_PARAMETERS, "%hd%s",&parms->index,parms->string);
    if (EhsStrcmp(parms->string,"null")==0||EhsStrcmp(parms->string,"NULL")==0) EhsStrcpy(parms->string,"");
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
    int index, size2;
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
            size2 = EhsStrlen(szData2);
            EhsStrncpy(tmp, szData1, index);
            EhsStrcat(&tmp[index], szData2);
            EhsStrcat(&tmp[size2 + index], &szData1[index]);
        }
        else
            EhsStrcpy(EHS_FB_OUT_S(0), "");

    }
    else
        EhsStrcpy(EHS_FB_OUT_S(0), "");/* Nout if there is no inout */
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
