#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H

#include "httpsrv.h"

#define CONFIG_CGI_DATA_LENGTH_MAX (1600) //how much data we can receive or send in a post request

/* Private/target specific prototypes for the HTTP server for lwip */
int cgi_example(HTTPSRV_CGI_REQ_STRUCT *param);
int ssi_date_time(HTTPSRV_SSI_PARAM_STRUCT *param);


#endif 
