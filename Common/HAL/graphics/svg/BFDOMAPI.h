/*
 * Filename : BFDOMAPI.h
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
#ifndef BITFLASH_DOMAPI_H_
#define BITFLASH_DOMAPI_H_

#include "BFBasicTypes.h"

#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif

/* forward declarations for internal type aliasing */
typedef struct BFSVG_Event              BFSVG_Event;
typedef struct BFXML_Element            BFXML_Element;
typedef struct BFXML_Document           BFXML_Document;
typedef struct BFSVG_Document           BFSVG_Document;
typedef struct BFMatrix2D               BFMatrix2D;
typedef struct BFPathObject             BFPathObject;
typedef struct BFSVG_Element            BFSVG_Element;
typedef struct BFSVG_DocumentHandle     BFSVG_DocumentHandle;
typedef struct BFSVG_Global             BFSVG_Global;

#include "BFXMLDOM.h"
#include "BFSVGDOM.h"

#ifdef __cplusplus
BF_END_EXTERN_C
#endif

#endif


