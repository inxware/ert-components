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
