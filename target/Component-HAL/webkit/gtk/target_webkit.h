/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

#ifndef TARGET_WEBKIT_H__
#define TARGET_WEBKIT_H__

#include "hal-api.h"

/* HAL API Facing */
ehs_bool EhsTWebkitLoad(ehs_char * baseURL, ehs_char * URLorData,ehs_bool Data,int x, int y, int width, int height, int alpha, int timeout);
ehs_bool  EhsTWebkitRunFunction(ehs_char * baseURL, ehs_char * URLorData,ehs_char * JSON);
ehs_bool EhsTWebkitShow(ehs_bool show);
ehs_bool EhsTWebkitUnloadAll();
ehs_bool EhsTWebkitDestroy();

/* Internal - target specific */
ehs_bool InitialiseWebkitJNICalls(struct android_app* state);

#endif
