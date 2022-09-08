/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

#ifndef EHS_HAL_WEBKIT_H
#define EHS_HAL_WEBKIT_H

/*
 * For many platforms supporting webkit libraries there is a common implementation (direct to webkit libs).
 * For others (e.g. Android the interface is peculiar
 */

#include "target_webkit.h"

ehs_bool EhsHWebkitLoad(ehs_char * baseURL, ehs_char * URLorData,ehs_bool Data,int x, int y, int width, int height, int alpha, int timeout);

ehs_bool  EhsHWebkitRunFunction(ehs_char * baseURL, ehs_char * URLorData,ehs_char * JSON);

ehs_bool EhsHWebkitShow(ehs_bool show);

ehs_bool EhsHWebkitUnloadAll();

ehs_bool EhsHWebkitDestroy();
#endif
