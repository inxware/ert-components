/**
 * convertor.h
 *
 * function prototypes for convertorFunctions.c - Convertor blocks of all types
 *
 * @author: Dr A Coombes
 * @version: $Revision: 333 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef EHS_CONVERTOR_FUNCTIONS_H
#define EHS_CONVERTOR_FUNCTIONS_H
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare Bool->Trigger convertor */
#define EHS_FB_CONVERTOR_BOOL_TO_TRIGGER_NAME "ConvertorBoolToTrigger"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToTrigger);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorBoolToTrigger);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorBoolToTrigger);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorBoolToTrigger)

/******************************************************************************/
/* Declare Int->Bool convertor */
#define EHS_FB_CONVERTOR_INT_TO_BOOL_NAME "ConvertorIntToBool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorIntToBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorIntToBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorIntToBool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorIntToBool)

/******************************************************************************/
/* Declare String->Bool convertor */
#define EHS_FB_CONVERTOR_STRING_TO_BOOL_NAME "ConvertorStringToBool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorStringToBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorStringToBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorStringToBool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorStringToBool)

/******************************************************************************/
/* Declare String->Int convertor */
#define EHS_FB_CONVERTOR_STRING_TO_INT_NAME "ConvertorStringToInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorStringToInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorStringToInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorStringToInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorStringToInt)

/******************************************************************************/
/* Declare Bool->Int convertor */
#define EHS_FB_CONVERTOR_BOOL_TO_INT_NAME "ConvertorBoolToInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorBoolToInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorBoolToInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorBoolToInt)

/******************************************************************************/
/* Declare Bool->String convertor */
#define EHS_FB_CONVERTOR_BOOL_TO_STRING_NAME "ConvertorBoolToString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorBoolToString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorBoolToString);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorBoolToString)

/******************************************************************************/
/* Declare Int->String convertor */
#define EHS_FB_CONVERTOR_INT_TO_STRING_NAME "ConvertorIntToString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorIntToString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorIntToString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorIntToString);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorIntToString)

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare Float->Bool convertor */
#define EHS_FB_CONVERTOR_FLOAT_TO_BOOL_NAME "ConvertorRealToBool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorRealToBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorRealToBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorRealToBool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorRealToBool)

/******************************************************************************/
/* Declare Float->Int convertor */
#define EHS_FB_CONVERTOR_FLOAT_TO_INT_NAME "ConvertorRealToInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorRealToInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorRealToInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorRealToInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorRealToInt)

/******************************************************************************/
/* Declare Bool->Float convertor */
#define EHS_FB_CONVERTOR_BOOL_TO_FLOAT_NAME "ConvertorBoolToReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorBoolToReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorBoolToReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorBoolToReal)

/******************************************************************************/
/* Declare Bool->Float convertor */
#define EHS_FB_CONVERTOR_INT_TO_FLOAT_NAME "ConvertorIntToReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorIntToReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorIntToReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorIntToReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorIntToReal)

/******************************************************************************/
/* Declare Bool->Float convertor */
#define EHS_FB_CONVERTOR_STRING_TO_FLOAT_NAME "ConvertorStringToReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorStringToReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorStringToReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorStringToReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorStringToReal)

/******************************************************************************/
/* Declare Bool->Float convertor */
#define EHS_FB_CONVERTOR_FLOAT_TO_STRING_NAME "ConvertorRealToString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConvertorRealToString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConvertorRealToString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConvertorRealToString);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorRealToString)
#endif /* EHS_TARGET_FP_SUPPORT */

#endif /* EHS_CONVERTOR_FUNCTIONS_H */

