#ifndef __HAL_HTTPSERVER_H
#define __HAL_HTTPSERVER_H
#include "ehs_types.h"

void httpServerRequestGet(char* reqName, char* data);
void httpServerRequestPost(char* reqName, char* buffer, char* data, uint32_t dataSize, void (*callback)(void*, char*), void* respObj);
void cgi_responseFunction(void* responseObj, char* data);
//static char gCGIBuffer[CGI_BUFFER_SIZE] = {0};

/* Example Common Gateway Interface callback. */
//static int cgi_example(HTTPSRV_CGI_REQ_STRUCT *param);
// This is more hal stuff now as we use it for non http use cases too.
//bool cgi_get_varval(const char *src, char *var_name, char *dst, uint32_t length);

/* Example Server Side Include callback. */
//static int ssi_date_time(HTTPSRV_SSI_PARAM_STRUCT *param);
/* Decode URL encoded string in place. */
void cgi_urldecode(char *url);

//uint32_t ws_echo_connect(void *param, WS_USER_CONTEXT_STRUCT context);
//uint32_t ws_echo_disconnect(void *param, WS_USER_CONTEXT_STRUCT context);
/*
typedef struct ws_response_struct
{
    WS_USER_CONTEXT_STRUCT* context;
    const char* callback;
} WebSocketResponse;
*/
void ws_responseFunction(void* responseObj, char* data);
//uint32_t ws_echo_message(void *param, WS_USER_CONTEXT_STRUCT context);

//uint32_t ws_echo_error(void *param, WS_USER_CONTEXT_STRUCT context);

#if 0
/* Other protocol todo2024 move these*/
typedef enum
{
    SMTP_IDLE,
    SMTP_NAME_RESOLVING,
    SMTP_NAME_RESOLVED,
} smtp_state_t;

typedef enum
{
    SMTP_ERR_NONE,
    SMTP_ERR_UNKNOWN_HOST,
} smtp_error_t;

typedef struct
{
    smtp_state_t state;
    const char *serverName;
    ip_addr_t serverIP;
    smtp_error_t lastError;
} smtp_t;

static void smtp_serverFound(const char *name, const ip_addr_t *ipaddr, void *arg);
static void InitSMTP(smtp_t *smtp);


typedef enum
{
    HTTP_SERVER_WEBPAGE_PATH_SETTINGS = 0,
    HTTP_SERVER_WEBPAGE_PATH_INDEX,
    HTTP_SERVER_WEBPAGE_PATH_MOBILE_INDEX,
    HTTP_SERVER_WEBPAGE_PATH_NETWORK,
    HTTP_SERVER_WEBPAGE_PATH_DASHBOARD,
    HTTP_SERVER_WEBPAGE_PATH_MINI_DASHBOARD,
    HTTP_SERVER_WEBPAGE_PATH_COUNT,   	 // !!! this must always be after last specified web page
    HTTP_SERVER_WEBPAGE_PATH_UNSPECIFIED // this is used for marking web page not specified in the array
} HttpServerWebpage;

/*static void http_server_socket_init(void)
{
    HTTPSRV_PARAM_STRUCT params;
    httpsrv_set_redirect_function(http_server_redirect);
    HTTPSRV_FS_init(httpsrv_fs_data);
    memset(&params, 0, sizeof(params));
    params.root_dir = "";
    params.index_page = "/index.html";
    params.auth_table = auth_realms;
    params.cgi_lnk_tbl = cgi_lnk_tbl;
    params.ssi_lnk_tbl = ssi_lnk_tbl;
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
    params.ws_tbl = ws_tbl;
#endif

#if HTTPSRV_CFG_MBEDTLS_ENABLE
    gHttpdTlsParams.certificate_buffer=gHttpdCrt;
    gHttpdTlsParams.certificate_buffer_size=gHttpdCrtSize;
    gHttpdTlsParams.private_key_buffer=gHttpdCrtKey;
    gHttpdTlsParams.private_key_buffer_size=gHttpdCrtKeySize;
    params.tls_param=&gHttpdTlsParams;
#endif
	gHttpsrvHandle = HTTPSRV_init(&params);
	if (gHttpsrvHandle == 0)
	{
		LWIP_PLATFORM_DIAG(("HTTPSRV_init() is Failed"));
	}
}*/

err_enum_t inxNetworkTaskWaitForDNS(const char* host,ip4_addr_t* pAddress);

#endif

#endif