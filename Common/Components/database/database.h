/*
 * url_get.h
 *
 *  Created on: 10 Dec 2010
 *      Author: pdrezet
 */

#ifndef URL_GET_H_
#define URL_GET_H_


//#define EHS_FB_URL_GET_NAME "UrlGet"
//#define EHS_FB_DEVMAN_PLAYER_S "get_post"
//#define EHS_FB_DEVMAN_PLAYER_STOP_CHECKING "abort"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(UrlGet);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(UrlGet);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_get);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_abort);

EHS_FB_FUNCTIONS(UrlGet)

#endif /* URL_GET_H_ */
