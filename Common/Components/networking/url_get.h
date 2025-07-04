/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* @file url_get.h
 *
 * @author: inx limited
 *
 */

#ifndef URL_GET_H_
#define URL_GET_H_



#define EHS_FB_NAME_UrlGet "UrlGet"
#define EHS_FB_ID_UrlGet 0x3CB2


/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(UrlGet);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(UrlGet);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_get);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_abort);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_savecookies);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_clearcookies);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_getmore);
EHS_FB_FUNCTIONS(UrlGet)

#endif /* URL_GET_H_ */
