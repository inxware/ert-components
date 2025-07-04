/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * stringfn.h
 *
 * function prototypes for stringFunctions.c
 *
 * @author: inx limited
 *
 */

#ifndef EHS_STRING_FUNCTIONS_H
#define EHS_STRING_FUNCTIONS_H


/******************************************************************************/
/* Declare FormatString */

#define EHS_FB_NAME_string_format "string_format"
#define EHS_FB_ID_string_format 0xDF0A

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_format);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_format);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_format);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_format)

/******************************************************************************/
/* Declare FormatString8 */

#define EHS_FB_NAME_string_format8 "string_format8"
#define EHS_FB_ID_string_format8 0xD55E

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_format8);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_format8);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_format8);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_format8)

/* integer variant */
#define EHS_FB_NAME_string_format8_int "string_format8_int"
#define EHS_FB_ID_string_format8_int 0x5FEC

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_format8_int);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_format8_int);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_format8_int);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_format8_int)

#ifdef EHS_TARGET_FP_SUPPORT
/* real variant */
#define EHS_FB_NAME_string_format8_real "string_format8_real"
#define EHS_FB_ID_string_format8_real 0x1627

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_format8_real);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_format8_real);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_format8_real);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_format8_real)
#endif
/******************************************************************************/
/* Declare FormatString8 */

#define EHS_FB_NAME_stringfn_scan8 "stringfn_scan8"
#define EHS_FB_ID_stringfn_scan8 0xD325

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(stringfn_scanf8);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(stringfn_scanf8);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(stringfn_scanf8);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(stringfn_scanf8)

/******************************************************************************/
/* Declare string_cat */

#define EHS_FB_NAME_CatString "CatString"
#define EHS_FB_ID_CatString 0x56CC

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_cat);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_cat);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_cat);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_cat)

/******************************************************************************/
/* Declare string_cmp */

#define EHS_FB_NAME_CmpString "CmpString"
#define EHS_FB_ID_CmpString 0xC389

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_cmp);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_cmp);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_cmp);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_cmp)

/******************************************************************************/
/* Declare string_len */

#define EHS_FB_NAME_LenString "LenString"
#define EHS_FB_ID_LenString 0x9437

EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_len);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_len)

/******************************************************************************/
/* Declare string_find */

#define EHS_FB_NAME_FindString "FindString"
#define EHS_FB_ID_FindString 0x7D98

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_find);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_find);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_find);


/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_find)

/******************************************************************************/
/* Declare string_toUpper */

#define EHS_FB_NAME_ToUpperString "ToUpperString"
#define EHS_FB_ID_ToUpperString 0xF2A3

EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_toUpper);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_toUpper)

/******************************************************************************/
/* Declare string_toLower */

#define EHS_FB_NAME_ToLowerString "ToLowerString"
#define EHS_FB_ID_ToLowerString 0x3BCE

EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_toLower);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_toLower)

/******************************************************************************/
/* Declare string_charAt */
#define EHS_FB_NAME_CharAtString "CharAtString"
#define EHS_FB_ID_CharAtString 0xFE14

EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_charAt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_charAt)

/******************************************************************************/
/* Declare string_strAt */

#define EHS_FB_NAME_StrAtString "StrAtString"
#define EHS_FB_ID_StrAtString 0x64DA

EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_strAt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_strAt)

/******************************************************************************/
/* Declare string_insert */

#define EHS_FB_NAME_InsertString "InsertString"
#define EHS_FB_ID_InsertString 0xC01A

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_insert);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_insert);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_insert);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_insert)

/******************************************************************************/
/* Declare string_sub */

#define EHS_FB_NAME_SubString "SubString"
#define EHS_FB_ID_SubString 0xA876

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_sub);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_sub);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_sub);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_sub)

#endif /* EHS_STRING_FUNCTIONS_H */

