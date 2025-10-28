/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * convertor.h
 *
 * function prototypes for convertorFunctions.c - Convertor blocks of all types
 *
 * @author: inx limited
 *
 */

#ifndef EHS_CONVERTOR_FUNCTIONS_H
#define EHS_CONVERTOR_FUNCTIONS_H
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare Bool->Trigger convertor */
#define EHS_FB_NAME_ConvertorBoolToTrigger "ConvertorBoolToTrigger"
#define EHS_FB_ID_ConvertorBoolToTrigger 0x44CD

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToTrigger);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorBoolToTrigger);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorBoolToTrigger);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorBoolToTrigger)

/******************************************************************************/
/* Declare Int->Bool convertor */
#define EHS_FB_NAME_ConvertorIntToBool "ConvertorIntToBool"
#define EHS_FB_ID_ConvertorIntToBool 0x119A

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorIntToBool);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorIntToBool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorIntToBool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorIntToBool)

/******************************************************************************/
/* Declare String->Bool convertor */
#define EHS_FB_NAME_ConvertorStringToBool "ConvertorStringToBool"
#define EHS_FB_ID_ConvertorStringToBool 0x63C9

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorStringToBool);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorStringToBool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorStringToBool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorStringToBool)

/******************************************************************************/
/* Declare String->Int convertor */
#define EHS_FB_NAME_ConvertorStringToInt "ConvertorStringToInt"
#define EHS_FB_ID_ConvertorStringToInt 0x90D8

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorStringToInt);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorStringToInt);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorStringToInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorStringToInt)

/******************************************************************************/
/* Declare Bool->Int convertor */

#define EHS_FB_NAME_ConvertorBoolToInt "ConvertorBoolToInt"
#define EHS_FB_ID_ConvertorBoolToInt 0xAC8B

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToInt);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorBoolToInt);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorBoolToInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorBoolToInt)

/******************************************************************************/
/* Declare Bool->String convertor */
#define EHS_FB_NAME_ConvertorBoolToString "ConvertorBoolToString"
#define EHS_FB_ID_ConvertorBoolToString 0xCCEB

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToString);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorBoolToString);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorBoolToString);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorBoolToString)

/******************************************************************************/
/* Declare Int->String convertor */
#define EHS_FB_NAME_ConvertorIntToString "ConvertorIntToString"
#define EHS_FB_ID_ConvertorIntToString 0x08B6

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorIntToString);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorIntToString);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorIntToString);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorIntToString)

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare Float->Bool convertor */
#define EHS_FB_NAME_ConvertorRealToBool "ConvertorRealToBool"
#define EHS_FB_ID_ConvertorRealToBool 0xE74B

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorRealToBool);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorRealToBool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorRealToBool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorRealToBool)

/******************************************************************************/
/* Declare Float->Int convertor */
#define EHS_FB_NAME_ConvertorRealToInt "ConvertorRealToInt"
#define EHS_FB_ID_ConvertorRealToInt 0xD329
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorRealToInt);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorRealToInt);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorRealToInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorRealToInt)

/******************************************************************************/
/* Declare Bool->Float convertor */
#define EHS_FB_NAME_ConvertorBoolToReal "ConvertorBoolToReal"
#define EHS_FB_ID_ConvertorBoolToReal 0xFC95
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToReal);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorBoolToReal);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorBoolToReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorBoolToReal)

/******************************************************************************/
/* Declare Bool->Float convertor */
#define EHS_FB_NAME_ConvertorIntToReal "ConvertorIntToReal"
#define EHS_FB_ID_ConvertorIntToReal 0xB3BA
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorIntToReal);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorIntToReal);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorIntToReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorIntToReal)

/******************************************************************************/
/* Declare Bool->Float convertor */
#define EHS_FB_NAME_ConvertorStringToReal "ConvertorStringToReal"
#define EHS_FB_ID_ConvertorStringToReal 0xC1E9

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorStringToReal);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorStringToReal);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorStringToReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorStringToReal)

/******************************************************************************/
/* Declare Bool->Float convertor */
#define EHS_FB_NAME_ConvertorRealToString "ConvertorRealToString"
#define EHS_FB_ID_ConvertorRealToString 0x1E6A

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConvertorRealToString);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConvertorRealToString);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConvertorRealToString);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConvertorRealToString)
#endif /* EHS_TARGET_FP_SUPPORT */

#endif /* EHS_CONVERTOR_FUNCTIONS_H */

