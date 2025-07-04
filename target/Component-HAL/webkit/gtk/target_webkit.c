/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#include "target_webkit.h"
#include "hal-api.h"


ehs_bool InitialiseWebkitJNICalls(struct android_app* state)
{

    return EHS_TRUE;
}

ehs_bool EhsTWebkitLoad(ehs_char * baseURL, ehs_char * URLorData,ehs_bool Data,int x, int y, int width, int height, int alpha, int timeout)
{

    return EHS_TRUE;
}

ehs_bool  EhsTWebkitRunFunction(ehs_char * baseURL, ehs_char * URLorData,ehs_char * JSON)
{

    return EHS_TRUE;
}
ehs_bool EhsTWebkitShow(ehs_bool show)
{
    return EHS_TRUE;
}
ehs_bool EhsTWebkitUnloadAll()
{
    return EHS_TRUE;
}

ehs_bool EhsTWebkitDestroy()
{
    return EHS_TRUE;
}


