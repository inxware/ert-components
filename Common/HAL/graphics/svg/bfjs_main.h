/*
 * Filename : bfjs_main.h
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
#ifndef BFJS_MAIN
#define BFJS_MAIN

#include "BFSVGT.h"
#include "../ECMABindings/bfjs_binding.h"

#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif

BF_PLAYER_IMPORT void BFJS_D_SetLogFile(BFJS_ScriptContext pJSContext, void *pLogFile);

BF_PLAYER_IMPORT BFJS_ScriptContext BFJS_D_InitScriptingEngine( BFJS_InitScriptParams const *pInitParams );

BF_PLAYER_IMPORT BF_BOOL BFJS_D_UpdateTimer( BFJS_ScriptContext pJSContext, BF_UINT32 uTimeAdvanced );


#ifdef __cplusplus
BF_END_EXTERN_C
#endif

#endif

