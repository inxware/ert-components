#ifndef TARGET_WEBKIT_H__
#define TARGET_WEBKIT_H__

#include "hal-api.h"
#include "android_native_app_glue.h"
/* HAL API Facing */
ehs_bool EhsTWebkitLoad(ehs_char * baseURL, ehs_char * URLorData,ehs_bool Data,int x, int y, int width, int height, int alpha, int timeout);
ehs_bool  EhsTWebkitRunFunction(ehs_char * baseURL, ehs_char * URLorData,ehs_char * JSON);
ehs_bool EhsTWebkitShow(ehs_bool show);
ehs_bool EhsTWebkitUnloadAll();
ehs_bool EhsTWebkitDestroy();

/* Internal - target specific */
ehs_bool InitialiseWebkitJNICalls(struct android_app* state);

#endif
