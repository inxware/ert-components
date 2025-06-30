#include "ehs_types.h"

#include "port.h"
#include "stdio.h"
#include "string.h"
#include "err.h"
#include "dns.h"
#include "netif.h"
#include "ethernetif.h"
#include "netifapi.h"
#include "app_dhcp.h"
#include "hal_httpserver.h"
#include "hal.h" //in case we use some generic functions rather than target specific
#include "httpserver.h"

/* todo this should be moved to a network management */
#define CONFIG_DHCP_WAIT_TICKS (64U) //how long we wait for DHCP to work

/* Here for now until needed in other places in lwIP */
#ifndef isprint
#define in_range(c, lo, up)  ((u8_t)c >= lo && (u8_t)c <= up)
#define isprint(c)           in_range(c, 0x20, 0x7f)
#define isdigit(c)           in_range(c, '0', '9')
#define isxdigit(c)          (isdigit(c) || in_range(c, 'a', 'f') || in_range(c, 'A', 'F'))
#define islower(c)           in_range(c, 'a', 'z')
#define isspace(c)           (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
#endif

/* these are declared in main, they mauy need to be moved to here instead?? todo2024 */
extern char *socket_data;
extern char *gWebSocketData;
extern WS_USER_CONTEXT_STRUCT gWebSocketContext;
extern bool gDHCP;
extern ip4_addr_t * gDNS;

extern struct netif fsl_netif0;
/* these may need to be conditional?*/
//static uint8_t gProvisionFailedCount = 0;

/* todo2024 - Extracted from HRD - requires genericizing */
/* Filter on pages and respond */

void httpServerRequestGet(char* reqName, char* data)
{
    // todo this should look throug  alinked list of API page function blocks, make something happen in the app and respond.
    if(strncmp(reqName,"get_network",sizeof("get_network"))==0)
    {
        //send back our network information
       // buildStatusPayload(data);
    }
 

//#if CONFIG_MQTT_ENABLED
//        MQTT_State_t mqttState=mqttGetGlobalState();
//#else
//        MQTT_State_t mqttState=MQTT_STATE_INIT;
//#endif
/*
        INX_Provison_State provisionState = getProvisionState();
        //  get system error state
        uint32_t systemErrorState = getSystemErrorState();
        int ret=snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,
                         "{\"cal1\":%2.1f,"
                         "\"cal2\":%2.1f,"
                         "\"cal3\":%2.1f,"
                         "\"powerConnection\":%d,"
                         "\"td1\":%d,\"td2\":%d,\"td3\":%d,"
                         "\"cs1\":%d,"
                         "\"cs2\":%d,"
                         "\"cs3\":%d,"
                         "\"em1\":%d,"
                         "\"em2\":%d,"
                         "\"em3\":%d,"
                         "\"emrMode\":%2.1f,"
                         "\"msp1\":%2.1f,"
                         "\"msp2\":%2.1f,"
                         "\"msp3\":%2.1f,"
                         "\"pid_p1\":%.4f,\"pid_i1\":%.4f,\"pid_d1\":%.4f,"
                         "\"pid_p2\":%.4f,\"pid_i2\":%.4f,\"pid_d2\":%.4f,"
                         "\"pid_p3\":%.4f,\"pid_i3\":%.4f,\"pid_d3\":%.4f,"
                         "\"ram1\":%d,\"ram2\":%d,\"ram3\":%d,"
                         "\"ssrMode\":%2.1f,"
                         "\"controllerContactor\":%d,"
                         "\"safetyMode\":%d,"
                         "\"p\":%d,"
                         "\"sm1\":%d,\"sm2\":%d,\"sm3\":%d,"
                         "\"steppedModeElement3Threshold\":%d,\"steppedModeElement2Threshold\":%d,"
                         "\"lat1\":%2.1f,\"uat1\":%2.1f,\"lat2\":%2.1f,\"uat2\":%2.1f,\"lat3\":%2.1f,\"uat3\":%2.1f,"
                         "\"elementRatingKW\":%2.1f,\"location\":\"%s\",\"localName\":\"%s\",\"mqttState\":%d,"
                         "\"pid1\":%3.1f,\"pid2\":%2.1f,"
                         "\"power1\":%d,\"pid3\":%3.1f,\"power2\":%d,\"deviceID\":\"%s\",\"controllerModel\":\""CONFIG_MODEL"\",\"com\":%d,\"systemErrors\":%d}",
                         gCal1,
                         gCal2,
                         gCal3,
                         gPowerMode,
                         gPIDs[0].timerDurationSeconds,gPIDs[1].timerDurationSeconds,gPIDs[2].timerDurationSeconds,
                         gPIDs[0].sensors,
                         gPIDs[1].sensors,
                         gPIDs[2].sensors,
                         gPIDs[0].elementMode,
                         gPIDs[1].elementMode,
                         gPIDs[2].elementMode,
                         gDutyCycleLengthEMRSeconds,
                         gPIDs[0].manualSetPoint,
                         gPIDs[1].manualSetPoint,
                         gPIDs[2].manualSetPoint,
                         gPIDs[0].coefficients.kp,gPIDs[0].coefficients.ki,gPIDs[0].coefficients.kd,
                         gPIDs[1].coefficients.kp,gPIDs[1].coefficients.ki,gPIDs[1].coefficients.kd,
                         gPIDs[2].coefficients.kp,gPIDs[2].coefficients.ki,gPIDs[2].coefficients.kd,
                         gPIDs[0].rampSecondsPerDegree,gPIDs[1].rampSecondsPerDegree,gPIDs[2].rampSecondsPerDegree,
                         gDutyCycleLengthSSRSeconds,
                         gControllerContactorMode,
                         gSafetyMode,
                         provisionState,
                         gPIDs[0].scheduleMode,gPIDs[1].scheduleMode,gPIDs[2].scheduleMode,
                         dutyCycleState2.powerOnThresholdPercent,dutyCycleState1.powerOnThresholdPercent,
                         gPIDs[0].lowerAlarmTemp,gPIDs[0].upperAlarmTemp,gPIDs[1].lowerAlarmTemp,gPIDs[1].upperAlarmTemp,gPIDs[2].lowerAlarmTemp,gPIDs[2].upperAlarmTemp,
                         gElementRatingKW,gLocation,gLocalName,mqttState,
                         gPIDs[0].currentValue,gPIDs[1].currentValue,
                         gPIDs[0].powerPercent,gPIDs[2].currentValue,gPIDs[1].powerPercent,clientId,gControllerOutputMode,systemErrorState);

        if(ret<0 || ret>=CONFIG_CGI_DATA_LENGTH_MAX)
        {
            SW_DIAG_printf("bad cgi_data write ret=%d",ret);
            data[0]='\0';
        }
    }
    / *char *c;
    // Replace '+' with spaces.
    * /
    while ((c = strchr(data, '+')) != NULL)
    {
    	*c = ' ';
    }*/
}

#ifdef INX_MQTT_SUPPORT_HARDWIRED_CONTROLLER_DATA 
/* Example post handler, that only works when a device has been provisioned */
void httpServerRequestPost(char* reqName, char* buffer, char* data, uint32_t dataSize, void (*callback)(void*, char*), void* respObj)
{
    if(!gProvisioned && strncmp(reqName,"post_mobile_provision",sizeof("post_mobile_provision"))==0)
    {
        if(validateCertificate(buffer, data, dataSize, /*extract details=*/true))
        {
            if((gProvisionFailedCount <= MAX_PROVISION_FAILED_COUNT) &&
                    isPinValid(buffer, data, dataSize) && isPasswordValid(buffer, data, dataSize))
            {
                //we now need to send this certificate to devman via mqtt
                //then devman can pair us with the appropriate organisation
#if CONFIG_MQTT_ENABLED
                snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"orgID\":%lld,\"expire\":\"%d %d %d %d %d %d\"}",
                         gCertificateOrganisationID, gCertificateExpieryDate.year, gCertificateExpieryDate.mon,
                         gCertificateExpieryDate.day, gCertificateExpieryDate.hour, gCertificateExpieryDate.min,
                         gCertificateExpieryDate.sec);
                mqttSendProvisioningCertificate(data);
                //LWIP_PLATFORM_DIAG((data));
#endif //CONFIG_MQTT_ENABLED
                gProvisionCertificateValidated=true;
                gProvisionFailedCount=0;
                //now respond saying everything went ok
                snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"success\":1}");
            }
            else
            {
                gProvisionFailedCount++;
                if(gProvisionFailedCount <= MAX_PROVISION_FAILED_COUNT)  // MAX number of attempts
                {
                    snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX, "{\"success\":0,\"count\":%d}",gProvisionFailedCount);
                }
                else
                {
                    // Send zero after reaching the maximum number of attempts.
                    snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"success\":0,\"count\":0}");
                }
            }
        }
        else
        {
            snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"Bad certificate.");
        }
        (*callback) (respObj, data); // response callback
    }
    else if(gProvisionCertificateValidated && strncmp(reqName,"post_mobile_provisioning_information",sizeof("post_mobile_provisioning_information"))==0)
    {
        bool success = cgi_get_varval(buffer,"location",data,dataSize);
        if(success)
        {
            cgi_urldecode(data);
            updateLocation(data);
        }
        success = success && cgi_get_varval(buffer,"localName",data,dataSize);
        if(success)
        {
            cgi_urldecode(data);
            updateLocalName(data);
        }
        success = success && cgi_get_varval(buffer,"localUIMode",data,dataSize);
        if(success)
        {
            cgi_urldecode(data);
            int localUIMode = atoi(data);
            success = (localUIMode >= INX_LOCAL_UI_MODE_MOBILE) && (localUIMode < INX_LOCAL_UI_MODE_COUNT);
            if(success)
            {
                updateLocalUIMode(localUIMode);
            }
        }
        if(success)
        {
            gProvisioned=true;
            gProvisionCertificateValidated=false;
            snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"success\":1}");
        }
        else
        {
            snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"success\":0}");
        }

        (*callback) (respObj, data); // response callback

        gSettingsUpdatedByUser=true;
    }
    else if(strncmp(reqName,"post_mobile_status",sizeof("post_mobile_status"))==0)
    {
        if(validateCertificate(buffer, data, dataSize, /*extract details=*/false))
        {
            if(gProvisioned)
            {
                snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"status\":1,\"localName\":\"%s\",\"orgID\":\"%lld\"}", gLocalName, gOrganisationID);
            }
            else
            {
                snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"status\":0}");
            }
            (*callback) (respObj, data); // response callback
        }
    }
    else if(gProvisioned && strncmp(reqName,"post_network",sizeof("post_network"))==0)
    {
        if(cgi_get_varval(buffer, "dhcp", data, dataSize))
        {
            cgi_urldecode(data);
            bool dhcp=false;
            if(strncmp(data,"on",sizeof("on"))==0)
            {
                dhcp=true;
            }
            if(!dhcp)
            {
                //read ip address config
                u32_t addr0,addr1,addr2,addr3;
                if(cgi_get_varval(buffer, "ipaddr", data, dataSize))
                {
                    cgi_urldecode(data);
                    sscanf(data,"%d.%d.%d.%d",&addr0,&addr1,&addr2,&addr3);
                    IP4_ADDR(&gfsl_netif0_ipaddr,addr0,addr1,addr2,addr3);
                }
                if(cgi_get_varval(buffer, "gateway", data, dataSize))
                {
                    cgi_urldecode(data);
                    sscanf(data,"%d.%d.%d.%d",&addr0,&addr1,&addr2,&addr3);
                    IP4_ADDR(&gfsl_netif0_gw,addr0,addr1,addr2,addr3);
                }
                if(cgi_get_varval(buffer, "netmask", data, dataSize))
                {
                    cgi_urldecode(data);
                    sscanf(cgi_data,"%d.%d.%d.%d",&addr0,&addr1,&addr2,&addr3);
                    IP4_ADDR(&gfsl_netif0_netmask,addr0,addr1,addr2,addr3);
                }
            }
            gDHCP=dhcp;
            HW_DIAG_printf("do disconnect because network settings changeda\n");
            gNetworkState=NETWORK_STATE_DO_DISCONNECT; // todo - use the function
            gSettingsUpdatedByUser=true;
        }

    }
    else if(gProvisioned && strncmp(reqName,"post_reset",sizeof("post_reset"))==0)
    {
        //LWIP_PLATFORM_DIAG(("reset post request."));
        if(cgi_get_varval(buffer, "resetMode", data, dataSize))
        {
            cgi_urldecode(data);
            inxResetMode(atoi(data));
        }
    }
    else if(gProvisioned && strncmp(reqName,"post_hardware",sizeof("post_hardware"))==0)
    {
        inxUpdateSettings(buffer);
    }
    else if(gProvisioned && strncmp(reqName,"post",sizeof("post"))==0)
    {
        inxUpdateSettings(buffer);
    }
    else
    {
        //LWIP_PLATFORM_DIAG(("unrecognised post request \"%s\"",reqName));
    }
}
#endif

void cgi_responseFunction(void* responseObj, char* data)
{
    HTTPSRV_CGI_RES_STRUCT* response = (HTTPSRV_CGI_RES_STRUCT*)responseObj;
    if(!response)
    {
        // invalid response object
        return;
    }
    response->content_type = HTTPSRV_CONTENT_TYPE_PLAIN;
    response->data = data;
    response->data_length = strlen(data);
    response->content_length = response->data_length;
    HTTPSRV_cgi_write(response);
}

#ifdef INX_MQTT_SUPPORT_HARDWIRED_CONTROLLER_DATA 
// todo2024 not nice
#define CGI_BUFFER_SIZE (sizeof("scheduleMode = steppedModeElement2Threshold = steppedModeElement3Threshold = elementMode = lowerAlarmTemperature = upperAlarmTemperature = manualSetPoint = tempScaleRange = localName = location = ") + CONFIG_CGI_DATA_LENGTH_MAX)
static char gCGIBuffer[CGI_BUFFER_SIZE] = {0};
char cgi_data[CONFIG_CGI_DATA_LENGTH_MAX + 1];

/* Example Common Gateway Interface callback. */
int cgi_example(HTTPSRV_CGI_REQ_STRUCT *param)
{
    HTTPSRV_CGI_RES_STRUCT response = {0};

    response.ses_handle = param->ses_handle;
    response.status_code = HTTPSRV_CODE_OK;
    if (param->request_method == HTTPSRV_REQ_GET)
    {
        httpServerRequestGet(param->script_name, cgi_data);
        response.content_type = HTTPSRV_CONTENT_TYPE_PLAIN;
        response.data = cgi_data;
        response.data_length = strlen(cgi_data);
        response.content_length = response.data_length;
        HTTPSRV_cgi_write(&response);
    }
    else if (param->request_method == HTTPSRV_REQ_POST)
    {
        uint32_t length = 0;
        uint32_t read;
        memset(gCGIBuffer,0,CGI_BUFFER_SIZE);
        length = param->content_length;
        read = HTTPSRV_cgi_read(param->ses_handle, gCGIBuffer, (length > sizeof(gCGIBuffer)) ? sizeof(gCGIBuffer) : length);

        if (read > 0)
        {
            #ifdef INX_MQTT_SUPPORT_HARDWIRED_CONTROLLER_DATA 
            httpServerRequestPost(param->script_name, gCGIBuffer, cgi_data, sizeof(cgi_data), cgi_responseFunction, &response);
            if(strcmp(param->script_name,"post")==0 || strcmp(param->script_name,"post_reset")==0 || strcmp(param->script_name,"post_hardware")==0)
            {
                /* Write the response using chunked transmission coding. */
                /* Set content length to -1 to indicate unknown content length. */
                response.content_type = HTTPSRV_CONTENT_TYPE_HTML;
                response.content_length = -1;
                response.data = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
                response.data_length = strlen(response.data);
                HTTPSRV_cgi_write(&response);
                response.data = "<html><head><title>POST successfull!</title>";
                response.data_length = strlen(response.data);
                HTTPSRV_cgi_write(&response);
                if(strcmp(param->script_name,"post_reset")==0 || strcmp(param->script_name,"post_hardware")==0)
                {
                    response.data = "<meta http-equiv=\"refresh\" content=\"0; url=cgi.html?tab=hardware\"></head><body></body></html>";
                }
                else
                {
                    response.data = "<meta http-equiv=\"refresh\" content=\"0; url=cgi.html\"></head><body></body></html>";
                }
                response.data_length = strlen(response.data);
                HTTPSRV_cgi_write(&response);
                response.data_length = 0;
                HTTPSRV_cgi_write(&response);
            }
            else if(strcmp(param->script_name,"post_network")==0)
            {
                /* Write the response using chunked transmission coding. */
                /* Set content length to -1 to indicate unknown content length. */
                response.content_type = HTTPSRV_CONTENT_TYPE_HTML;
                response.content_length = -1;
                response.data = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
                response.data_length = strlen(response.data);
                HTTPSRV_cgi_write(&response);
                response.data = "<html><head><title>POST successfull!</title>";
                response.data_length = strlen(response.data);
                HTTPSRV_cgi_write(&response);
                response.data = "<meta http-equiv=\"refresh\" content=\"0; url=network.html\"></head><body></body></html>";
                response.data_length = strlen(response.data);
                HTTPSRV_cgi_write(&response);
                response.data_length = 0;
                HTTPSRV_cgi_write(&response);
            }

        }
    }

    return (response.content_length);
}
#else
#warning "Not compiled cgi_example"
#endif

/* Example Server Side Include callback. */
int ssi_date_time(HTTPSRV_SSI_PARAM_STRUCT *param)
{
    /*if (strcmp(param->com_param, "time") == 0)
    {
        HTTPSRV_ssi_write(param->ses_handle, __TIME__, strlen(__TIME__));
    }
    else if (strcmp(param->com_param, "date") == 0)
    {
        HTTPSRV_ssi_write(param->ses_handle, __DATE__, strlen(__DATE__));
    }*/
    return (0);
}

/* Decode URL encoded string in place. */
void cgi_urldecode(char *url)
{
    char *src = url;
    char *dst = url;

    while (*src != '\0')
    {
        if ((*src == '%') && (isxdigit((int)*(src + 1))) && (isxdigit((int)*(src + 2))))
        {
            *src = *(src + 1);
            *(src + 1) = *(src + 2);
            *(src + 2) = '\0';
            *dst++ = strtol(src, NULL, 16);
            src += 3;
        }
        else
        {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}



#if HTTPSRV_CFG_WEBSOCKET_ENABLED
/*
 * Echo plugin code - simple plugin which echoes any message it receives back to
 * client.
 */
#define WSC_BUFFER_SIZE CONFIG_CGI_DATA_LENGTH_MAX
#define WSC_CALLBACK_SIZE 128
#define WSC_NO_CUMULATIVE_DATA -1

static int32_t gWSCumulativeDataLength=WSC_NO_CUMULATIVE_DATA;

static uint32_t gWebSocketHandle=0;
uint32_t ws_echo_connect(void *param, WS_USER_CONTEXT_STRUCT context)
{
#ifdef   INX_MONITOR_STACKHIGHWATERMARK
    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
    LWIP_PLATFORM_DIAG(("ws stack bytes free=%d",uxHighWaterMark*4));
#endif
#if DEBUG_WS
    LWIP_PLATFORM_DIAG(("ws_echo_connect start gWebSocketHandle=%d context.handle=%d",gWebSocketHandle,context.handle));
#endif
    if(gWebSocketHandle==0)
    {
        gWebSocketHandle=context.handle;
    }
#if DEBUG_WS
    LWIP_PLATFORM_DIAG(("ws_echo_connect end gWebSocketHandle=%d context.handle=%d",gWebSocketHandle,context.handle));
#endif
    gWSCumulativeDataLength=WSC_NO_CUMULATIVE_DATA;
    return (0);
}

uint32_t ws_echo_disconnect(void *param, WS_USER_CONTEXT_STRUCT context)
{
#if DEBUG_WS
    LWIP_PLATFORM_DIAG(("ws_echo_disconnect start gWebSocketHandle=%d context.handle=%d",gWebSocketHandle,context.handle));
#endif
    if(context.handle==gWebSocketHandle)
    {
        gWebSocketHandle=0;
    }
#if DEBUG_WS
    LWIP_PLATFORM_DIAG(("ws_echo_disconnect end gWebSocketHandle=%d context.handle=%d",gWebSocketHandle,context.handle));
#endif
    gWSCumulativeDataLength=WSC_NO_CUMULATIVE_DATA;
    return (0);
}

typedef struct ws_response_struct
{
    WS_USER_CONTEXT_STRUCT* context;
    const char* callback;
} WebSocketResponse;

void ws_responseFunction(void* responseObj, char* data)
{
    WebSocketResponse* response = (WebSocketResponse*)responseObj;
    if(!response)
    {
        // invalid response struct
        return;
    }
    snprintf(gWebSocketData,CONFIG_CGI_DATA_LENGTH_MAX,"{\"callback\":\"%s\",\"data\":%s}",
             response->callback, data);
    /*response->context->data.length = strlen(gWebSocketData);
    response->context->data.data_ptr[response->context->data.length] = 0;
    response->context->data.data_ptr = (uint8_t*)gWebSocketData;
    WS_send(response->context);*/

    gWebSocketContext.handle=gWebSocketHandle;
    gWebSocketContext.data.data_ptr=(uint8_t*)gWebSocketData;
    gWebSocketContext.data.length=strlen(gWebSocketData);
    gWebSocketContext.data.type=WS_DATA_TEXT;
    gWebSocketContext.fin_flag=true;
#if DEBUG_WS
    LWIP_PLATFORM_DIAG(("ws_responseFunction WS_send gWebSocketHandle=%d gWebSocketData=%s len=%d",gWebSocketHandle,gWebSocketData,strlen(gWebSocketData)));
#endif
    WS_send(&gWebSocketContext);
#if DEBUG_WS
    LWIP_PLATFORM_DIAG(("ws_responseFunction WS_send done"));
#endif
}
static uint8_t gWSBuffer[WSC_BUFFER_SIZE]="";
static char gWSCallback[WSC_CALLBACK_SIZE]="";
uint32_t ws_echo_message(void *param, WS_USER_CONTEXT_STRUCT context)
{
    if (context.data.type == WS_DATA_TEXT)
    {
        #if CONFIG_WEB_SOCKET_MESSAGE_ENABLED
        WebSocketResponse response = {0};
        #endif
        //LWIP_PLATFORM_DIAG(("WebSocket message received (length:%d,finished:%d): \r\n%s",
        //					context.data.length,context.fin_flag,context.data.data_ptr));
        // handle data that gets received in multiple stages
        if(!context.fin_flag && gWSCumulativeDataLength < WSC_BUFFER_SIZE)
        {
            if(gWSCumulativeDataLength == WSC_NO_CUMULATIVE_DATA)
            {
                context.data.data_ptr[context.data.length] = 0;
                strncpy((char*)gWSBuffer, (char*)context.data.data_ptr, WSC_BUFFER_SIZE);
                gWSCumulativeDataLength = context.data.length;
            }
            else
            {
                strncat((char*)gWSBuffer, (char*)context.data.data_ptr,WSC_BUFFER_SIZE);
                gWSCumulativeDataLength += context.data.length;
            }
            return (0);
        }
        else if(context.fin_flag && gWSCumulativeDataLength != WSC_NO_CUMULATIVE_DATA)
        {
            strncat((char*)gWSBuffer, (char*)context.data.data_ptr,WSC_BUFFER_SIZE);
            gWSCumulativeDataLength += context.data.length;
            context.data.length = gWSCumulativeDataLength;
            context.data.data_ptr = gWSBuffer;
            //LWIP_PLATFORM_DIAG(("WebSocket cumulative message received (length:%d,finished:%d): \r\n%s",
            //							context.data.length,context.fin_flag,context.data.data_ptr));
            gWSCumulativeDataLength = WSC_NO_CUMULATIVE_DATA; // no more cumulative data
        }
        context.data.data_ptr[context.data.length] = 0;
        char* buffer = (char*)context.data.data_ptr;
        if(cgi_get_varval(buffer,"callback",socket_data,sizeof(socket_data)))  // decode method
        {
            //LWIP_PLATFORM_DIAG(("WebSocket callback: %s", socket_data));
            strncpy((char*)gWSCallback,(char*)socket_data,WSC_CALLBACK_SIZE);
        }
#if CONFIG_WEB_SOCKET_MESSAGE_ENABLED
        // assign callback/message id
        cgi_urldecode(socket_data);
        response.callback = gWSCallback;
#ifdef   INX_MONITOR_STACKHIGHWATERMARK
        UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
        LWIP_PLATFORM_DIAG(("ws_echo_message stack bytes free=%d",uxHighWaterMark*4));
#endif
        if(cgi_get_varval(buffer,"method",socket_data,sizeof(socket_data)))  // decode method
        {
            //LWIP_PLATFORM_DIAG(("WebSocket method: %s", socket_data));
        }
        cgi_urldecode(socket_data);
        if((strncmp(socket_data,"GET",sizeof("GET"))==0))
        {
            if(cgi_get_varval(buffer,"url",socket_data,sizeof(socket_data)))  // decode method
            {
                //LWIP_PLATFORM_DIAG(("WebSocket url: %s", socket_data));
                cgi_urldecode(socket_data);

                httpServerRequestGet(socket_data, socket_data);
                response.context = &context;
                ws_responseFunction(&response, socket_data);
            }
        }
        else   // POST
        {
            if(cgi_get_varval(buffer,"url",socket_data,sizeof(socket_data)))  // decode method
            {
                //LWIP_PLATFORM_DIAG(("WebSocket url: %s", socket_data));
                cgi_urldecode(socket_data);
                response.context = &context;
                httpServerRequestPost(socket_data, buffer, socket_data, sizeof(socket_data), ws_responseFunction, &response);
            }
        }
#endif //CONFIG_WEB_SOCKET_MESSAGE_ENABLED
    }
    return (0);
}

uint32_t ws_echo_error(void *param, WS_USER_CONTEXT_STRUCT context)
{
#if DEBUG_WS
    PRINTF("WebSocket error: 0x%X.\r\n", context.error);
#endif
    return (0);
}

WS_PLUGIN_STRUCT ws_tbl[] = {{"/ws", ws_echo_connect, ws_echo_message, ws_echo_error, ws_echo_disconnect, NULL},
    {0, 0, 0, 0, 0, 0}
};
#endif /* HTTPSRV_CFG_WEBSOCKET_ENABLED */

#if 0
/* This is moved back into the main_utilities - can be deleted! */
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

static void smtp_serverFound(const char *name, const ip_addr_t *ipaddr, void *arg)
{
    smtp_t *smtp = (smtp_t*)arg;

    if ((ipaddr) && (ipaddr->addr))
    {
        ip4_addr_copy(smtp->serverIP, *ipaddr);
        smtp->lastError = SMTP_ERR_NONE;
        smtp->state = SMTP_NAME_RESOLVED;
        return;
    }
    else
    {
        smtp->lastError = SMTP_ERR_UNKNOWN_HOST;
    }
    smtp->state = SMTP_IDLE;
}

static void InitSMTP(smtp_t *smtp)
{
    smtp->state = SMTP_IDLE;
    smtp->serverName = "";
    IP4_ADDR(&smtp->serverIP, 0, 0, 0, 0);
    smtp->lastError = SMTP_ERR_NONE;
}

typedef enum
{
    INX_DNS_LOOKUP_STATE_INIT,
    INX_DNS_LOOKUP_STATE_RESOLVING
} inxDNSLookupState;
#endif

#if HTTPSRV_CFG_MBEDTLS_ENABLE
static HTTPSRV_TLS_PARAM_STRUCT gHttpdTlsParams;
#endif

/* Redirect any pages that needs redirecting*/
#define HTTP_SERVER_REQUEST_UNPROVISIONED_PATH "\\provision.html"
#define HTTP_SERVER_REQUEST_RESTRICTED_PATH    "\\restricted.html"
#define HTTP_SERVER_REQUEST_ICON_PATH    	   "\\favicon.ico"

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
/*
static char* HTTP_SERVER_WEBPAGE_PATHS[HTTP_SERVER_WEBPAGE_PATH_COUNT] =
{
	"\\cgi.html",		// HTTP_SERVER_WEBPAGE_PATH_SETTINGS
	"\\index.html",		// HTTP_SERVER_WEBPAGE_PATH_INDEX
	"\\mobile.html",	// HTTP_SERVER_WEBPAGE_PATH_MOBILE_INDEX
	"\\network.html",	// HTTP_SERVER_WEBPAGE_PATH_NETWORK
	"\\welcome.html",	// HTTP_SERVER_WEBPAGE_PATH_DASHBOARD
	"\\minidash.html",	// HTTP_SERVER_WEBPAGE_PATH_MINI_DASHBOARD
};

static HttpServerWebpage getHttpServerWebpage(const char* path){
	for(int i = HTTP_SERVER_WEBPAGE_PATH_SETTINGS; i < HTTP_SERVER_WEBPAGE_PATH_COUNT; i++){
		const char* webpage = HTTP_SERVER_WEBPAGE_PATHS[i];
		if(strncmp(path, webpage, strlen(webpage)) == 0){
			return i;
		}
	}
	return HTTP_SERVER_WEBPAGE_PATH_UNSPECIFIED;
}

static bool isWebpageFullSettings(HttpServerWebpage webpage){
	return (webpage == HTTP_SERVER_WEBPAGE_PATH_DASHBOARD ||
			webpage == HTTP_SERVER_WEBPAGE_PATH_SETTINGS ||
		    webpage == HTTP_SERVER_WEBPAGE_PATH_NETWORK);
}

static bool isWebpageDashboard(HttpServerWebpage webpage){
	return (webpage == HTTP_SERVER_WEBPAGE_PATH_INDEX ||
			webpage == HTTP_SERVER_WEBPAGE_PATH_MOBILE_INDEX ||
			webpage == HTTP_SERVER_WEBPAGE_PATH_DASHBOARD);
}

static bool isWebpageUnspecified(HttpServerWebpage webpage){
	return (webpage == HTTP_SERVER_WEBPAGE_PATH_UNSPECIFIED);
}


static char* http_server_redirect(char* path)
{
	// ignore icon request
	if(strncmp(path, HTTP_SERVER_REQUEST_ICON_PATH, strlen(HTTP_SERVER_REQUEST_ICON_PATH)) == 0){
		return path;
	}
	HttpServerWebpage webpage = getHttpServerWebpage(path);
	if(!gProvisioned){ // redirect unprovisioned recognised pages
		if(webpage == HTTP_SERVER_WEBPAGE_PATH_MOBILE_INDEX){ // @TODO Atm we only allow to provision via mobile device
			return HTTP_SERVER_REQUEST_UNPROVISIONED_PATH;
		}else{
			//LWIP_PLATFORM_DIAG(("The web page path ( %s ) is not recognised.", path));
			return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
		}
	}else{
		// Http server should no longer need to request the provisioning page
		if(isWebpageUnspecified(webpage) && strncmp(path, HTTP_SERVER_REQUEST_UNPROVISIONED_PATH,
				strlen(HTTP_SERVER_REQUEST_UNPROVISIONED_PATH)) == 0){
			//LWIP_PLATFORM_DIAG(("Should no longer need to request the provisioning page."));
			return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
		}else{
			switch(gLocalUIMode){
				case INX_LOCAL_UI_MODE_MOBILE:
					if(webpage == HTTP_SERVER_WEBPAGE_PATH_MOBILE_INDEX || isWebpageFullSettings(webpage)){
						break; // correct web page request
					}else{
						LWIP_PLATFORM_DIAG(("Page ( %s ) not served in MOBILE mode.", path));
						return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
					}
				case INX_LOCAL_UI_MODE_MINI_DASHBOARD:
					if(isWebpageDashboard(webpage)){
						char* minidash = HTTP_SERVER_WEBPAGE_PATHS[HTTP_SERVER_WEBPAGE_PATH_MINI_DASHBOARD];
						return minidash;
					}else{
						LWIP_PLATFORM_DIAG(("Page ( %s ) not served in MINI DASHBOARD mode.", path));
						return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
					}
				case INX_LOCAL_UI_MODE_FULL_DASHBOARD:
					if(isWebpageDashboard(webpage)){
						return HTTP_SERVER_WEBPAGE_PATHS[HTTP_SERVER_WEBPAGE_PATH_DASHBOARD];
					}else{
						LWIP_PLATFORM_DIAG(("Page ( %s ) not served in FULL DASHBOARD mode.", path));
						return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
					}
					break;
				case INX_LOCAL_UI_MODE_FULL_SETTINGS:
					if(isWebpageDashboard(webpage) || isWebpageFullSettings(webpage)){
						break; // correct web page request
					}else{
						LWIP_PLATFORM_DIAG(("Page ( %s ) not served in FULL SETTINGS mode.", path));
						return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
					}
				case INX_LOCAL_UI_MODE_DISABLE:
					LWIP_PLATFORM_DIAG(("Device doesn't serve pages as it's in DISABLED mode."));
					return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
				default:
					LWIP_PLATFORM_DIAG(("No pages are served in UNKNOWN mode."));
					return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
			}
		}
	}
	return path;
}
*/
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
