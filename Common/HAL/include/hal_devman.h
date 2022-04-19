/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here. 
 * This file should only be included by hal_process.h
 * 
 * @author: inx limited
 * @version: $Revision: 1242 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef DEVMAN_HAL_H
#define DEVMAN_HAL_H

/*****************************************************************************/
/* Included files */
#include "target.h"

/*****************************************************************************/
/* Define macros  */

/* If we are using SSL/TLS */
// @TODO - this needs to be reviewd e.g. cert for android, ambifier, unity etc.
#if defined(EHS_ANDROID) && !defined(EHS_AMBIFIER) // this should  really be the default for anything tha is not ambifier.com really.
#define EHS_DEVMAN_CA_CERTIFICATE          "./core/certs/cacert.pem"
#else
//#error "not expecting a non android build at mo"
#define EHS_DEVMAN_CA_CERTIFICATE          "./core/certs/devman-ca.crt"
#endif

#define EHS_DEVMAN_CLIENT_PRIVATEKEY 	   "./core/certs/devman-client.key"
#define EHS_DEVMAN_CLIENT_CERTIFICATE      "./core/certs/devman-client.crt"
#define EHS_DEVMAN_CLIENT_CERTIFICATE_KEY  "./core/certs/devman-client-crt-key.pem" // Combined key and and certificate file


/* define a retry rate for starting at the top of the URL list */

#ifndef EHS_DEVMAN_CORE_RETRY_PERIOD_FIRSTURL
#define EHS_DEVMAN_CORE_RETRY_PERIOD_FIRSTURL 10 /*Retry top of every  n successful lower priority try */
#endif

#ifndef EHS_DEVMAN_CORE_RETRY_TIMES_FIRSTURL
#define EHS_DEVMAN_CORE_RETRY_TIMES_FIRSTURL 10 /* Number of times top of list is tried */
#endif

#ifndef EHS_MAXDEVMANNAMELEN
#define EHS_MAXDEVMANNAMELEN 2048 // must be smaller than default string length
#endif

/* Define some fail-over paths/URLS for DEVMAN */
#ifndef EHS_DEVMAN_COREURLS
#define EHS_DEVMAN_COREURLS	"core/config/DEVMANURL"
#endif



#define EHS_MAXDEVMANNAMELEN 2048 // must be smaller than default string length //@todo

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
