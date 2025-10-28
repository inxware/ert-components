/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file html.h
 * Declares features needed to parse/handle HTML-like text for rendering.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_HAL_GRAPHICS_HTML_H
#define EHS_HAL_GRAPHICS_HTML_H

/*****************************************************************************/
/* Included files */
#include "globals.h"
/*****************************************************************************/
/* Define macros  */

/*****************************************************************************/
/* Define types */

/**
 * Contains an HTML string converted into a compressed form - this replaces
 * long HTML symbols with shorter items
 */
typedef struct
{
    ehs_char* szHtml;		/* actual HTML string */
    ehs_uint32 nMaxLen;		/* maximum length of the current HTML string */
    ehs_bool bFixedMaxLen;	/* maximum length is fixed (i.e. can't be increased) */
    ehs_char* szWord;		/* holds the last word read */
    ehs_uint32 nWordLen;	/* length of the memory allocated to hold the word */
} EhsHGHtmlStringClass;

/**
 * Returns the kind of word that we've extracted from the HTML. The values
 * of styles other than text have been chosen as:
 * 1. They are not valid values in Unicode
 * 2. They could be processed as if they are
 * The full range of these values is given by replacing 'x's in the following
 * binary string with 1s or 0s: 11011xxx1xxxxxxx (i.e. 0xd880 | 0-0x7f or 0x100-0x17f,
 * or 0x200-0x27f, ... or 0x700-77f
 */
typedef enum EhsHGHtmlWordEnum
{
    EHSHG_HTML_WORD_STYLE_TEXT,
    EHSHG_HTML_WORD_BREAK		= 0xd880,
    EHSHG_HTML_WORD_PARA		= 0xd881,
    EHSHG_HTML_WORD_STYLE_START	= 0xd882,
    EHSHG_HTML_WORD_STYLE_END   = 0xd883,
    EHSHG_HTML_WORD_IMG			= 0xd884,

    EHSHG_HTML_WORD_PARAM_END	= 0xdfff
} EhsHGHtmlWordType;

/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */

/**
 * Initialise the HtmlString.
 * @param[in,out] pHtml HtmlString to initialise
 * @param [in] nMaxLen Maximum length of the HTML string. 0 means unknown
 * @return true if successful
 */
EHS_GLOBAL ehs_bool EhsHGHtmlString_init(EhsHGHtmlStringClass* pHtml, ehs_uint32 nMaxLen);

/**
 * Parse a raw string into an instance of this class
 * @param[in,out] pHtml Pointer to the HTML structure that has been parsed
 * @param[in] szRawString The initial string to convert
 * @return true if parsing has been successful
 */
EHS_GLOBAL ehs_bool EhsHGHtmlString_parse(EhsHGHtmlStringClass* pHtml, const ehs_char* szRawString,ehs_bool parse);

/**
 * Get a word from the HTML string ready to render.
 * @param[in] pHtml Pointer to the HTML structure to read from.
 * @param[out] pszWord Pointer to the word that we've just read.
 * @param[out] peType Type of the word.
 * @param[in,out] ppCurrent Pointer to the start of the next word (used for internal purposes only).
 * If NULL when calling, starts the search from the beginning of the string.
 * @return NULL when there are no more words to return.
 */
EHS_GLOBAL void* EhsHGHtmlString_getWord(EhsHGHtmlStringClass* pHtml, const ehs_char** pszWord, EhsHGHtmlWordType* peType, const void** ppCurrent);
#endif /*EHS_HAL_GRAPHICS_HTML_H*/
