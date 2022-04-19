/**
 * stringfn.h
 *
 * function prototypes for stringFunctions.c
 *
 * @author: ??
 * @version: $Revision: 4565 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef EHS_STRING_FUNCTIONS_H
#define EHS_STRING_FUNCTIONS_H


/******************************************************************************/
/* Declare FormatString */
#define EHS_STRING_FORMAT_NAME "string_format"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_format);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_format);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_format);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_format)

/******************************************************************************/
/* Declare FormatString8 */
#define EHS_STRING_FORMAT_NAME8 "string_format8"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_format8);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_format8);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_format8);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_format8)

/******************************************************************************/
/* Declare FormatString8 */
#define EHS_STRING_SCANF8_NAME "stringfn_scan8"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(stringfn_scanf8);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(stringfn_scanf8);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(stringfn_scanf8);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(stringfn_scanf8)

/******************************************************************************/
/* Declare string_cat */
#define EHS_CAT_STRING_NAME "CatString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_cat);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_cat);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_cat);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_cat)

/******************************************************************************/
/* Declare string_cmp */
#define EHS_CMP_STRING_NAME "CmpString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_cmp);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_cmp);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_cmp);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_cmp)

/******************************************************************************/
/* Declare string_len */
#define EHS_LEN_STRING_NAME "LenString"
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_len);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_len)

/******************************************************************************/
/* Declare string_find */
#define EHS_FIND_STRING_NAME "FindString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_find);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_find);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_find);


/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_find)

/******************************************************************************/
/* Declare string_toUpper */
#define EHS_TO_UPPER_STRING_NAME "ToUpperString"
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_toUpper);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_toUpper)

/******************************************************************************/
/* Declare string_toLower */
#define EHS_TO_LOWER_STRING_NAME "ToLowerString"
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_toLower);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_toLower)

/******************************************************************************/
/* Declare string_charAt */
#define EHS_CHAR_AT_STRING_NAME "CharAtString"
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_charAt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_charAt)

/******************************************************************************/
/* Declare string_strAt */
#define EHS_STR_AT_STRING_NAME "StrAtString"
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_strAt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_strAt)

/******************************************************************************/
/* Declare string_insert */
#define EHS_INSERT_STRING_NAME "InsertString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_insert);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_insert);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_insert);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_insert)

/******************************************************************************/
/* Declare string_sub */
#define EHS_SUBSTRING_NAME "SubString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(string_sub);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(string_sub);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(string_sub);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(string_sub)

#endif /* EHS_STRING_FUNCTIONS_H */

