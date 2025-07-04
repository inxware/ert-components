/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#include "hal_webkit.h"


ehs_bool EhsHWebkitLoad(ehs_char * baseURL, ehs_char * URLorData,ehs_bool Data,int x, int y, int width, int height, int alpha, int timeout)
{
    //CreateWebView
    return EhsTWebkitLoad(baseURL, URLorData, Data,x,y,width,height,alpha,timeout);
}

ehs_bool  EhsHWebkitRunFunction(ehs_char * baseURL, ehs_char * URLorData,ehs_char * JSON)
{
    return EhsTWebkitRunFunction(baseURL,  URLorData, JSON);
}

ehs_bool EhsHWebkitShow(ehs_bool show)
{
    return EhsTWebkitShow( show);
}

ehs_bool EhsHWebkitUnloadAll()
{
    return EhsTWebkitUnloadAll();
}

ehs_bool EhsHWebkitDestroy()
{
    return EhsTWebkitDestroy();
}
