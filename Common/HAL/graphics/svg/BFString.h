/*
 * Filename : BFString.h
 *
 *  Version : 6.4.0_2271s
 *     Date : 20080508-20.49.08
 *
 *
 * (c) COPYRIGHT 2002- 2008 Open Text Corp. - BitFlash Division. ALL RIGHTS RESERVED
 *
 *
 * This software is the property and confidential information of Open Text and is furnished under
 * license by BitFlash Inc.  This software may be used only in accordance with the terms of said license.
 *
 * This software may not be copied, transmitted, provided to or otherwise made available to any other person,
 * company, corporation or other entity except as specified in the terms of said license.
 *
 * This copyright notice may not be removed or altered without the prior written permission of Open Text.
 *
 *
 */
/**
 * \file BFString.h
 *
 * This file defines functions for string manipulation.
 *
 * Copyright ©2000-2002 BitFlash Graphics Inc. All rights reserved.
 */

#ifndef BF_STRING_H_
#define BF_STRING_H_

#include "BFBasicTypes.h"
#include "BFGlobalHeap.h"
#include <stdarg.h>

#define BF_TOLOWER(x) ((BF_CHAR)(((x) >= 'A' && (x) <= 'Z') ? (x) + ('a' - 'A') : (x)))
#define BF_TOUPPER(x) ((BF_CHAR)(((x) >= 'a' && (x) <= 'z') ? (x) - ('a' - 'A') : (x)))

#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif

BF_PLAYER_IMPORT BF_WCHAR *
BFwcsdup( BFHeapContext *pMemCtx, const BF_WCHAR *szString);

BF_PLAYER_IMPORT BF_CHAR *
BFstrdup( BFHeapContext *pMemCtx, const BF_CHAR *szString);

BF_PLAYER_IMPORT BFUInteger
BFwcslen( const BF_WCHAR *szString);

BF_PLAYER_IMPORT BF_WCHAR*
BFwcscpy( BF_WCHAR *szDestination, const BF_WCHAR *szSource );

BF_PLAYER_IMPORT BF_WCHAR*
BFwcsncpy( BF_WCHAR *szDestination, const BF_WCHAR *szSource, BFUInteger uCount );

BF_PLAYER_IMPORT BFInteger
BFwcscmp( const BF_WCHAR *szString1, const BF_WCHAR *szString2 );

BF_PLAYER_IMPORT BFUInteger
BFstrlen( const BF_CHAR *szString);

BF_PLAYER_IMPORT BF_CHAR*
BFstrcat( BF_CHAR *szDestination, const BF_CHAR *szSource );

BF_PLAYER_IMPORT BF_CHAR*
BFstrcpy( BF_CHAR *szDestination, const BF_CHAR *szSource );

BF_PLAYER_IMPORT BFInteger
BFstrcmp( const BF_CHAR *szString1, const BF_CHAR *szString2 );

BF_PLAYER_IMPORT BF_CHAR*
BFstrncpy( BF_CHAR *szDestination, const BF_CHAR *szSource, BFUInteger uCount );

BF_PLAYER_IMPORT BFInteger
BFstrncmp( const BF_CHAR *szString1, const BF_CHAR *szString2, BFUInteger uCount );

BF_PLAYER_IMPORT BFInteger
BFstrncmpi( const BF_CHAR *szString1, const BF_CHAR *szString2, BFUInteger uCount );

BF_PLAYER_IMPORT const BF_CHAR*
BFstrchr( const BF_CHAR *szString, BF_CHAR c );

BF_PLAYER_IMPORT const BF_CHAR*
BFstrrchr( const BF_CHAR *szString, BF_CHAR c );

/* Warning: The signature is non-standard. It returns a const BFCHAR* instead
            of a (non-const) BF_CHAR*. Rationale: blindly throwing away
            constness is immoral.
*/
BF_PLAYER_IMPORT const BF_CHAR*
BFstrstr( const BF_CHAR *szTargetString, BF_CHAR const *szSearchString );

BF_PLAYER_IMPORT BF_UINT32 BFsnprintf( BF_CHAR* dst, BF_UINT32 dst_size, BF_CHAR const * format, ... );
BF_PLAYER_IMPORT BF_UINT32 BFvsnprintf( BF_CHAR* dst, BF_UINT32 dst_size, BF_CHAR const * format, va_list args );

BF_PLAYER_IMPORT const BF_CHAR*
BFstrnstr( const BF_CHAR *szTarget, BF_CHAR const *szSearch, BFUInteger uCount );

BF_PLAYER_IMPORT void BFftoa( BF_CHAR *pString, BF_UINT8 nLength, BFFixedPoint number );

BF_PLAYER_IMPORT BF_UINT32 BFappendString( BFHeapContext *pMemCtx,
        BF_CHAR **ppDst, BF_UINT32 uLength, BF_CHAR const *szString, BF_CHAR uAppendChar );

#if 0
/* Not needed with current code base */
BF_PLAYER_IMPORT const BF_WCHAR*
BFwstrstr( const BF_WCHAR *szTargetString, const BF_WCHAR *szSearchString );
#endif


#ifdef __cplusplus
BF_END_EXTERN_C
#endif

#endif

