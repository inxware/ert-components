/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here.
 * This file should only be included by hal_process.h
 *
 * @author: inx limited
 *
 */

#ifndef DEVMAN_HAL_H
#define DEVMAN_HAL_H

/*****************************************************************************/
/* Included files */
//#include "target.h"

/*****************************************************************************/
/* Define macros  */

/* If we are using SSL/TLS */
// todo: We need to get EHS to use a cert from a keystore so we don't have duplicates fo the downloader etc.

/* Select our custom (possibly self signed) CA bundle or single server certificate. 
   The paltform/.../config.mk for the target defines bundle or single when make targetenv is called */

#define EHS_DEVMAN_CERTIFICATES_BASE               "./core/certs/" 
#define EHS_DEVMAN_CA_CERTIFICATE_FILENAME         "devman-ca.crt"
#define EHS_DEVMAN_CLIENT_PRIVATEKEY_FILENAME 	   "devman-client.key"
#define EHS_DEVMAN_CLIENT_CERTIFICATE_FILENAME     "devman-client.crt"
#define EHS_DEVMAN_CLIENT_CERTIFICATE_KEY_FILENAME "devman-client-crt-key.pem"
/* These are the default locations in case thre is nothing special found*/
#define EHS_DEVMAN_CA_CERTIFICATE         EHS_DEVMAN_CERTIFICATES_BASE EHS_DEVMAN_CA_CERTIFICATE_FILENAME
#define EHS_DEVMAN_CLIENT_PRIVATEKEY 	   EHS_DEVMAN_CERTIFICATES_BASE EHS_DEVMAN_CLIENT_PRIVATEKEY_FILENAME
#define EHS_DEVMAN_CLIENT_CERTIFICATE     EHS_DEVMAN_CERTIFICATES_BASE EHS_DEVMAN_CLIENT_CERTIFICATE_FILENAME
#define EHS_DEVMAN_CLIENT_CERTIFICATE_KEY EHS_DEVMAN_CERTIFICATES_BASE EHS_DEVMAN_CLIENT_CERTIFICATE_KEY_FILENAME // Combined key and and certificate file

/* define a retry rate for starting at the top of the URL list */

#ifndef EHS_DEVMAN_CORE_RETRY_PERIOD_FIRSTURL
#define EHS_DEVMAN_CORE_RETRY_PERIOD_FIRSTURL 10 /*Retry top of every  n successful lower priority try */
#endif

#ifndef EHS_DEVMAN_CORE_RETRY_TIMES_FIRSTURL
#define EHS_DEVMAN_CORE_RETRY_TIMES_FIRSTURL 10 /* Number of times top of list is tried */
#endif

/* Define some fail-over paths/URLS for DEVMAN */
#ifndef EHS_DEVMAN_COREURLS
#define EHS_DEVMAN_COREURLS	"core/config/DEVMANURL"
#endif


#ifndef EHS_DEVMAN_DEFAULTBASEURL
#define EHS_DEVMAN_DEFAULTBASEURL	"https://devman.inx-systems.com"
#endif


/* This is the actual default target file for doing bitsn.bobs */
#ifndef EHS_DEVMAN_MONITORURLPATHONLY
#define EHS_DEVMAN_MONITORURLPATHONLY "/deviceHandler.php"
//#define EHS_DEVMAN_MONITORURLPATHONLY "/cgi-bin/devmon.cgi" /* We need this to write to the directories for devport admin... */
#endif

/* This is the actual default target file for requesting an app download */
#ifndef EHS_DEVMAN_MONITORURLS
#define EHS_DEVMAN_MONITORURLS EHS_DEVMAN_DEFAULTBASEURL EHS_DEVMAN_MONITORURLPATHONLY
#endif

/* This is the actual default target file for requesting an app download */
#ifndef EHS_DEVMAN_APPGETDEFAULTURLPATHONLY
#define EHS_DEVMAN_APPGETDEFAULTURLPATHONLY "/downloadApp.php"
#endif

/* This is the actual URI for requesting an app download */
#ifndef EHS_DEVMAN_APPGETDEFAULTURL
#define EHS_DEVMAN_APPGETDEFAULTURL EHS_DEVMAN_DEFAULTBASEURL EHS_DEVMAN_APPGETDEFAULTURLPATHONLY
#endif
/*****************************************************************************/
/* Define types */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

/**
 * Perform necessary Operating system setup upon system initialisation
 */
EHS_GLOBAL void DevmanMon_init(void);

/* shared devman failover URL utilities implemented in devman_mon.c*/
ehs_bool EhsHDevmanRemoveHeadURLFromList(ehs_char * ehs_path);
ehs_bool EhsHDevmanRemoveSpecificURLFromList(ehs_char * ehs_path, ehs_char * URL, ehs_uint16 startposition);
ehs_bool EhsHDevmanRemoveURLSFromList(ehs_char * ehs_path, ehs_uint16 index);
ehs_bool EhsHDevmanAddURLtoHeadList(ehs_char * ehs_path, ehs_char * URL);
ehs_bool EhsHDevmanGetURL(ehs_char * URL, ehs_char * list_file,ehs_uint16 maxlen, ehs_uint16 index);
ehs_bool GetDevmanBASEURL(ehs_char * szUrl);
void EhsHDevmanGetCoreURL(ehs_char * szUrl);

#endif /* DEVMAN_MON_H */
