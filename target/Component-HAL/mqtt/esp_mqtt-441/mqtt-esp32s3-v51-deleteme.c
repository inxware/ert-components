
#include "globals.h"
#include "hal_mqtt.h"
#include "mqtt.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include "ehs_types.h"

/* esp32 mqtt support*/
#include "mqtt_client.h"

#include "hal_network.h"
#include "hal-api.h"
#include "esp_heap_caps.h"
#include "target_file.h"
#include "targetos_init.h"

#ifdef MQTT_USE_TLS
#include "esp_tls.h"
#endif
#ifdef ESP_TLS_DEBUG_INX
#include "mbedtls/esp_debug.h"
int esp_tls_debug_calltimes = 0;
#endif



#define CONFIG_MQTT_MAX_TOPIC_LENGTH (EHS_STRING_LENGTH_MAX)
#define INX_MQTT_MAX_PAYLOAD_SIZE (EHS_STRING_LENGTH_MAX)
#define INX_MQTT_MAX_ERROR_MSG_BUFFER_SIZE (EHS_STRING_LENGTH_MAX)
/**
 * MQTT client connection states
 */
enum
{
    TCP_DISCONNECTED,
    TCP_CONNECTING,
#ifdef MQTT_USE_TLS
    TLS_HANDSHAKING,
#endif
    MQTT_CONNECTING,
    MQTT_CONNECTED
};

static bool gSubscribe = false;
static bool gPublish= false;
static mqtt_qos_t gQoS = MQTT_QOS_0;
static mqtt_qos_t gQoS_pub = MQTT_QOS_0;
static char gTopicNameTemporary[CONFIG_MQTT_MAX_TOPIC_LENGTH] = "";
static char gTopicNameTemporaryPub[CONFIG_MQTT_MAX_TOPIC_LENGTH] = "";
char **topic_list;
int *gQoS_list;
ehs_uint8 topic_count = 0;
ehs_bool gSubsribeAgain = EHS_FALSE;
static char gMqttPayload[INX_MQTT_MAX_PAYLOAD_SIZE];

// server.crt
//#define MQTT_CERT_TEST
#ifdef MQTT_CERT_TEST
static const unsigned char *CACert  = "-----BEGIN CERTIFICATE-----\n\
MIIDljCCAn6gAwIBAgIJAN2e3u3nI1pTMA0GCSqGSIb3DQEBCwUAMGAxHDAaBgNV\n\
BAMME3d3dy5pbngtc3lzdGVtcy5jb20xDDAKBgNVBAoMA0lOWDENMAsGA1UECwwE\n\
Um9vdDEjMCEGCSqGSIb3DQEJARYUaW5mb0Bpbngtc3lzdGVtcy5jb20wHhcNMjAw\n\
NDA5MTM0MDE3WhcNMzIwNDA2MTM0MDE3WjBgMRwwGgYDVQQDDBN3d3cuaW54LXN5\n\
c3RlbXMuY29tMQwwCgYDVQQKDANJTlgxDTALBgNVBAsMBFJvb3QxIzAhBgkqhkiG\n\
9w0BCQEWFGluZm9AaW54LXN5c3RlbXMuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOC\n\
AQ8AMIIBCgKCAQEA2MHAHGCdKEd/qBA7AkmbpOFoc/freNCuMbW+JtqwaovLCVCn\n\
upFRMENlWZ2FgIFNYMTFP0PjK5TzrCD1sOajjdJ8VK+2pDisWQUfi59Fh3Bv6dtZ\n\
XpeYYYe14cRG3JpfIdQz8cWiULtgHYRAMklMLPABHpb3Lv+agFzqgU0liYD32f7M\n\
X30KwBuQKs1t7XpopcCyZ/grQpIAqAAWyTIZArlv78slejeN/Vqzdgf0sRx6L7VW\n\
WKmvOCvl5yVZSyd5NVB8+FN1xOcx20I8LY3AQqDC3genUPE5PBq8wYBhSLxzQkkA\n\
bJWkHOmWe/kdf9Ft3UBJuj2stc5BWmySoo9TkwIDAQABo1MwUTAdBgNVHQ4EFgQU\n\
n4Sa2K/JwlIOPyJx6UxZZYxQ8YYwHwYDVR0jBBgwFoAUn4Sa2K/JwlIOPyJx6UxZ\n\
ZYxQ8YYwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEABwrgYAkh\n\
wwuZUyUjE8QhCNMKZjcRcgBtEtrlFcpfMStQitezVRl7ZJqEWBcntOKLLI7yvrMd\n\
QuEM7dT/5vGzWW5xepXqLwN+b8pFkZwCJHWViirF8KxR91zUYA5G+n9ULCszX0ox\n\
tM6YTAqKw/Su6HTXfEPWR/U9NFkjbyX1AX0q0VOTAonaZfntjaPXJ0JX5o7RL5k6\n\
DD0z94ojcJHFGs1RJHY3X97XHlTX6T4wkdRRxUmLaKQzxcnVvtJxjAmluQ5Pls7j\n\
ORez5qnIqQNkMOaw19XPKyNowcZoveiV/qYv7XnldSXXATClLPi+BTWb3I0V0Hwz\n\
rgE+55lUkgkUzg==\n\
-----END CERTIFICATE-----\n";

// client.pem
static const unsigned char *CLientCert = "-----BEGIN CERTIFICATE-----\n\
MIIEMjCCAxqgAwIBAgIJAK2dYmH3roCLMA0GCSqGSIb3DQEBCwUAMGAxHDAaBgNV\n\
BAMME3d3dy5pbngtc3lzdGVtcy5jb20xDDAKBgNVBAoMA0lOWDENMAsGA1UECwwE\n\
Um9vdDEjMCEGCSqGSIb3DQEJARYUaW5mb0Bpbngtc3lzdGVtcy5jb20wHhcNMjAw\n\
NjEyMTAxNDIxWhcNMzIwNjA5MTAxNDIxWjAeMRwwGgYDVQQDDBNocmlAaW54LXN5\n\
c3RlbXMuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAzApXJE/i\n\
mk/GeIPMgGEcpLOc2PpFxiD0YoiznCILZvCQjjPCE3z57T/SC2DegX5vGw3NL7nd\n\
mDW5/vuMOTU6+GDEp4sd58CSkcg2SA1X6/5DghFS0tVGlbz6Xfay/hxlf+TUhKTi\n\
89A7TZvJesQUsz2+gjIAX9T5o5/kxk3tKly189MwcsNAF5sEDIG4I7LTyO0U1Yvi\n\
P8yd9ATeStC3ZMkcyPIup16ihjSyo9IyM4DcMiNstsPKLNlSBv9/XBpu5KvLC/RR\n\
OzpLwViJ14jkTT7uVGQIQcp4jLCVq/XhFN65ywz+gy4uNgp+8AaB+p9y0KWV+2lX\n\
niM2SOuY9x3DswIDAQABo4IBLzCCASswDAYDVR0TAQH/BAIwADARBglghkgBhvhC\n\
AQEEBAMCBaAwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMEMAsGA1UdDwQE\n\
AwIDqDAoBglghkgBhvhCAQ0EGxYZQ2xpZW50IEJyb2tlciBDZXJ0aWZpY2F0ZTAd\n\
BgNVHQ4EFgQUgpB2TvibcxeWOiU6wP0UKvQKq/8wgZIGA1UdIwSBijCBh4AUn4Sa\n\
2K/JwlIOPyJx6UxZZYxQ8YahZKRiMGAxHDAaBgNVBAMME3d3dy5pbngtc3lzdGVt\n\
cy5jb20xDDAKBgNVBAoMA0lOWDENMAsGA1UECwwEUm9vdDEjMCEGCSqGSIb3DQEJ\n\
ARYUaW5mb0Bpbngtc3lzdGVtcy5jb22CCQDdnt7t5yNaUzANBgkqhkiG9w0BAQsF\n\
AAOCAQEAyXvRAF8u1O9HH7N6iYeoXFfHxHnC/vXQonwAPMWaVeC83VLAl09+IBQ7\n\
J4hFUINwXS5PaxVoI4pNxZW0qHBVVIGCaFuZwpyzxQbIJzHa3GmMohldqczPcrp5\n\
dNxl+jLFLjtlwxBrzPsWEgzcBOlW/sHpfwpqC12ngQD/67ICfb6lOmaH0SG/uPT8\n\
QZBWiR0gXkeuaeeXyLiQuxi4gecrDrW53L5bf4BHcYtL+zsdVGLJ2YtgZCIfGaiL\n\
cBUpcZRiDQK1AEToFv+zRsTjhns+DjEvbDe0e35xqU3HIBiu0SlRdjpIA+51MQp0\n\
EYF93v0EthfLYBgvEz/Ex1Wo83LDdg==\n\
-----END CERTIFICATE-----\n";

// client.pem
static const unsigned char *ClientKey = "-----BEGIN RSA PRIVATE KEY-----\n\
MIIEpAIBAAKCAQEAzApXJE/imk/GeIPMgGEcpLOc2PpFxiD0YoiznCILZvCQjjPC\n\
E3z57T/SC2DegX5vGw3NL7ndmDW5/vuMOTU6+GDEp4sd58CSkcg2SA1X6/5DghFS\n\
0tVGlbz6Xfay/hxlf+TUhKTi89A7TZvJesQUsz2+gjIAX9T5o5/kxk3tKly189Mw\n\
csNAF5sEDIG4I7LTyO0U1YviP8yd9ATeStC3ZMkcyPIup16ihjSyo9IyM4DcMiNs\n\
tsPKLNlSBv9/XBpu5KvLC/RROzpLwViJ14jkTT7uVGQIQcp4jLCVq/XhFN65ywz+\n\
gy4uNgp+8AaB+p9y0KWV+2lXniM2SOuY9x3DswIDAQABAoIBAEeEXFTq9zc1PRCi\n\
ybw5zathFVnsJyayCFa9KY9RU4PKUDC3rY/dH1DsLLg2ROdIpLLr3H6c/DHMqxvX\n\
mae3o6rJaER9e4butbEJZxVMMAQUxB7D6a7ieHIloe/rd4Of+megkmjHzpC87Pxf\n\
3Xf9YHAO9UZ3NjBg2vtiIPLs/3L4GcYWxtEK4vUDSaktjR36ZE+mhCi9zi6/raQs\n\
ciC8dctg55STjiruH2mjHKc0Q5BZIm/cEmdzlI1KIr6qcan9qxcOWkAJvk1iw2OE\n\
E/BFuOmSyF05Ku+B9h9Ca9epCf9OmjJ4rOgSFpe8ax3scxKuX7B1v1oE7BI9taW9\n\
D/dJzfkCgYEA6CQxO45dvTqfQc09uhtMOYrkKmOZxAjHgg+atQDB1HNrvQTsMIUc\n\
c9Dry8AzQvp1qCtxFHJJI36qLR40z+Mgv0W9G0AV6gD80dGg9DsksmZfz1I0IHPt\n\
APzK/rzmBwUxha+mbhUyIEHr0E1k4c5KTdUk9ty111quy9UIU0WAVc0CgYEA4QLK\n\
dOwLgiD5JLnL/xA38s6dOo9YZykaH4QO+UbdQRIqk/YMq83Y0ZOLaLJUZxjlMRhg\n\
lf2wgyoUtYW8yNrXruv6/J9B6Jdc3JhtpZaT5wTMQ+4fAjhfxcKv9g/wgZTQJaQC\n\
3IQP1vf5yM9TWMqRdEyyQAP2Zn7hLRYN78bs/38CgYEA3mv3BBI3PJzThI+rCgsn\n\
sKakUP0Xkl27EqHy4GR2UyG34+a4ixCvH0TY+GMSeqVRYC0nbqjF7Q1q7knX3ivH\n\
75xZQafYl1oU4tCRABE+6vPt7s3bhhCsz9pmgUbqu2U9etK+ixEh8whWckIFW9l1\n\
FjgX77OvQ0Tu3/wguvCHsy0CgYACaCI8P+4blTi1ANvCvayrhzQ9VR0v9UuXPLVg\n\
SzbApG4aKQ6Xo9sG2top44Ztl+GTtdwKnq0NqhyQmH6dc2HNOC72FsoAr+XJYkQB\n\
+mtYlD+PFnP0dR/pIJz7N85txGa5kFiLfUoiFYLxd+4/S+NkaBGcB67SB9dmwQ6j\n\
GvCcBwKBgQCC1txp4BCp7O6GLl51K9qNLWGzdBhY1PPu5O14atvzLS9EJtxmhpqY\n\
5FJwUVWLVemnrjIT5GRioLrtoz97o1Tmle1soW5RALPHFRDgj9bfLFI1OoTbUZNH\n\
eL/rtlUM1NUQMzKUeo37vC8D0gUTRrXvbbCZ3/EbJ/k7z0/neUh8Yg==\n\
-----END RSA PRIVATE KEY-----\n";

#else
static const unsigned char CACert[1] = "";
static const unsigned char CLientCert[1] = "";
static const unsigned char ClientKey[1] = "";
#endif
// actual mqtt stuff

esp_mqtt_client_config_t mqtt_cfg;
esp_mqtt_client_handle_t client;
static MQTT_State_t MQTT_state = MQTT_STATE_INIT;
static ehs_uint32 gMqttConnectionAttempts = 0;


ehs_uint8 gUseTLS = 0;
char* host = NULL;
char* clientid = NULL;
char* username = NULL;
char* password = NULL;

ehs_uint16 port = 8883;

char *CA_str = NULL, *ClientCRT_str = NULL, *ClientKEY_str = NULL;

ehs_char mqtt_error_msg_buffer[INX_MQTT_MAX_ERROR_MSG_BUFFER_SIZE];


void mqttSetGlobalState(const MQTT_State_t newState)
{
    MQTT_state = newState;
}

//static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    char *temp_buf, *temp_topic;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        printf("MQTT msgid= %d event: %d. MQTT_EVENT_CONNECTED to: %s\n", event->msg_id, event->event_id, host);
        mqttSetGlobalState(MQTT_STATE_CONNECTED);
        EhsMQTTConnectEvent(true);
        break;
    case MQTT_EVENT_DISCONNECTED:
        printf("MQTT event: %d. MQTT_EVENT_DISCONNECTED\n", event->event_id);
        if (*bNewSodlFlagRef == EHS_TRUE) mqttSetGlobalState(MQTT_STATE_DISCONNECTED_NEW);
        else mqttSetGlobalState(MQTT_STATE_DISCONNECTED);
        EhsMQTTConnectEvent(false);
        break;

    case MQTT_EVENT_SUBSCRIBED:
        printf("MQTT_EVENT_SUBSCRIBED, msg_id=%d\n", event->msg_id);
        //   msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        //     printf("sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        printf("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d\n", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        printf("MQTT_EVENT_PUBLISHED, msg_id=%d\n", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        temp_topic = malloc(sizeof(char) * (event->topic_len + 1));
        memcpy(temp_topic, event->topic, event->topic_len * sizeof(char));
        memset(temp_topic + event->topic_len, 0, 1);
        EhsMQTTSubscribeEvent(temp_topic, event->data, event->data_len);
        free(temp_topic);

        break;
    case MQTT_EVENT_ERROR:
        printf("MQTT_EVENT_ERROR\n");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            printf("Last error code reported from esp-tls: 0x%x\n", event->error_handle->esp_tls_last_esp_err);
            EhsSprintf(mqtt_error_msg_buffer, "tcp transport error : 0x%x", event->error_handle->esp_tls_last_esp_err);
            EhsMQTTReportError(mqtt_error_msg_buffer);

            // printf("Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            // printf("Last captured errno : %d (%s)", event->error_handle->esp_transport_sock_errno,
            //        strerror(event->error_handle->esp_transport_sock_errno));
        }
        else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED)
        {
            printf("Connection refused error: 0x%x\n", event->error_handle->connect_return_code);
            EhsSprintf(mqtt_error_msg_buffer, "connection refused error : 0x%x", event->error_handle->connect_return_code);
            EhsMQTTReportError(mqtt_error_msg_buffer);
        }
        else
        {
            printf("Unknown error type: 0x%x\n", event->error_handle->error_type);
            EhsSprintf(mqtt_error_msg_buffer, "unknown error : 0x%x", event->error_handle->error_type);
            EhsMQTTReportError(mqtt_error_msg_buffer);
        }
        break;
    default:
//        printf("Other event id:%d\n", event->event_id);
//        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    }
}

// `output` is an unallocated pointer
ehs_bool readAppFileIntoString(const char *filename, char **output)
{
    ehs_FILE *tempFile;
    struct stat FileStat;
    size_t readLen = 0;
    size_t heap_free;
    int ret;

    tempFile = Ehs_AppFopen(filename, "r");
    if (tempFile == NULL)
    {
        printf("readAppFileIntoString: File does not exist or FS corrupted!\n");
        return EHS_FALSE;
    }
    int Ffileno = fileno(tempFile);
    ret = fstat(Ffileno, &FileStat);
    if (ret == -1) 
    {
        printf("fstat failed! fp: %p\n");
        EhsFclose(tempFile);
        return EHS_FALSE;
    }
    if (*output == NULL)
    {
        if ( (*output = malloc((FileStat.st_size + 1) * sizeof(char) )) == NULL)
        {
            printf("readAppFileIntoString: no enough heap left!\n");
            heap_free = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
            printf("Heap left:\t%15d\nWant to use:\t%15d\n", heap_free, (FileStat.st_size + 1) * sizeof(char));
            EhsFclose(tempFile);
            return EHS_FALSE;
        }
    }
    else 
    {
        if ((*output = realloc(*output, (FileStat.st_size + 1) * sizeof(char))) == NULL)
        {
            printf("readAppFileIntoString: no enough heap left!\n");
            heap_free = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
            printf("Heap left:\t%15d\nWant to use:\t%15d\n", heap_free, (FileStat.st_size + 1) * sizeof(char));
            EhsFclose(tempFile);
            return EHS_FALSE;
        }
    }
    readLen = EhsFread(*output, sizeof(char), FileStat.st_size, tempFile);
    if (ferror(tempFile) != 0)
    {
        printf("readAppFileIntoString: Error reading file\n");
        EhsFclose(tempFile);
        return EHS_FALSE;
    }
    else
    {
        (*output)[readLen++] = '\0';
    }
    EhsFclose(tempFile);
    return EHS_TRUE;
}

EHS_GLOBAL void* EhsMqttClientLoop(void*)
{
    //mbedtls_esp_enable_debug_log();
    //esp_tls_debug_calltimes += 1;

    esp_err_t err;

    char *clientCertFileName = NULL;
    char *clientKeyFileName = NULL;
    char *rootCAFileName = NULL;
    ehs_bool connect = 0;
    ehs_bool confirm_subscribe = 0, confirm_unsubscribe = 0;
    ehs_uint8 i;
    ehs_bool ret;
#ifdef MQTT_USE_TLS
    mbedtls_x509_crt certificate;
    mbedtls_x509_crt_init(&certificate);
#endif
    strcpy(gMqttPayload, ""); // Reset MQTT Publish Payload
    
    // printf("%s", host);
    switch (MQTT_state)
    {

    case MQTT_STATE_IDLE:
        printf("MQTT_STATE_IDLE\n");
        if (*bNewSodlFlagRef != EHS_TRUE)
        {
            mqttSetGlobalState(MQTT_STATE_INIT);
            Ehs_FB_ThreadStarted();
        }
        break;

    case MQTT_STATE_INIT:
        if (*bNewSodlFlagRef == EHS_TRUE)
        {
            break;
        }

        EhsMQTTConnectPoll(&connect, &host, &port, &gUseTLS, &clientid, &username, &password, &clientCertFileName, &clientKeyFileName, &rootCAFileName);
        if (connect && gMqttConnectionAttempts == 0)
        {
            printf("Init connection init %s %i\n", host, port);
#ifdef USE_ESP32S3_LEGACY_API
            mqtt_cfg.host = host;
            mqtt_cfg.port = port;

            mqtt_cfg.username = username;
            mqtt_cfg.password = password;
            mqtt_cfg.keepalive = 15;
#else
            mqtt_cfg.broker.address.hostname = host;
            mqtt_cfg.broker.address.port = port;
            mqtt_cfg.broker.address.transport = MQTT_TRANSPORT_OVER_TCP;

            mqtt_cfg.credentials.client_id = clientid;
            mqtt_cfg.credentials.username = username;
            mqtt_cfg.credentials.authentication.password = password;
            mqtt_cfg.session.keepalive = 15;
#endif
            //mqtt_cfg.event_handle = mqtt_event_handler;
            if(gUseTLS==1){
                printf("using tls mqtt\n\n");
                
                #ifndef MQTT_CERT_TEST
                if(rootCAFileName && rootCAFileName[0] != '\0'){
                    ret = readAppFileIntoString(rootCAFileName, &CA_str);
                    if (ret == EHS_TRUE)
                    {
#ifdef USE_ESP32S3_LEGACY_API
                        mqtt_cfg.cert_pem = CA_str;
                        //mqtt_cfg.cert_len = strlen(CA_str);
#else
                        mqtt_cfg.broker.verification.certificate = CA_str;
                        //mqtt_cfg.broker.verification.certificate_len = strlen(CA_str);
#endif
                        
                    }else{
                        EhsMQTTReportError("failed to open ca cert");
                    }
                }else{
                    printf("CA Cert not specified.\n");
                }
                #else
                {
#ifdef USE_ESP32S3_LEGACY_API
                    mqtt_cfg.cert_pem = CACert;
                    //mqtt_cfg.cert_len = strlen(CACert);
#else
                    mqtt_cfg.broker.verification.certificate = CACert;
                    //mqtt_cfg.broker.verification.certificate_len = strlen(CACert);
#endif
                }
                #endif

                #ifndef MQTT_CERT_TEST
                if(clientCertFileName && clientCertFileName[0] != '\0'){
                    ret = readAppFileIntoString(clientCertFileName, &ClientCRT_str);
                    if (ret == EHS_TRUE) 
                    {
#ifdef USE_ESP32S3_LEGACY_API
                        mqtt_cfg.client_cert_pem = ClientCRT_str;
                        //mqtt_cfg.client_cert_len = strlen(ClientCRT_str);
#else
                        mqtt_cfg.credentials.authentication.certificate = ClientCRT_str;
                        //mqtt_cfg.credentials.authentication.certificate_len = strlen(ClientCRT_str);
#endif
                    }else{
                        EhsMQTTReportError("failed to open client cert");
                    }
                }else{
                    printf("Client Cert not specified.\n");
                }
                #else 
                {
#ifdef USE_ESP32S3_LEGACY_API
                    mqtt_cfg.client_cert_pem = CLientCert;
                    //mqtt_cfg.client_cert_len = strlen(CLientCert);
#else
                    mqtt_cfg.credentials.authentication.certificate = CLientCert;
                    //mqtt_cfg.credentials.authentication.certificate_len = strlen(CLientCert);
#endif              
                }
                #endif

                #ifndef MQTT_CERT_TEST
                if(clientKeyFileName && clientKeyFileName[0] != '\0'){
                    ret = readAppFileIntoString(clientKeyFileName, &ClientKEY_str);
                    if (ret == EHS_TRUE)
                    {
#ifdef USE_ESP32S3_LEGACY_API
                        mqtt_cfg.client_key_pem = ClientKEY_str;
                        //mqtt_cfg.client_key_len = strlen(ClientKEY_str);
#else
                        mqtt_cfg.credentials.authentication.key = ClientKEY_str;
                        //mqtt_cfg.credentials.authentication.key_len = strlen(ClientKEY_str);
#endif
                    }else{
                      EhsMQTTReportError("failed to open client key");
                    }
                }else{
                    printf("Client Key not specified.\n");
                }
                #else
                {
#ifdef USE_ESP32S3_LEGACY_API
                    mqtt_cfg.client_key_pem = ClientKey;
                    //mqtt_cfg.client_key_len = strlen(ClientKey);
#else
                    mqtt_cfg.credentials.authentication.key = ClientKey;
                    //mqtt_cfg.credentials.authentication.key_len = strlen(ClientKey);
#endif                    
                }
                #endif
#ifdef USE_ESP32S3_LEGACY_API
                mqtt_cfg.transport = MQTT_TRANSPORT_OVER_SSL;
#else
                mqtt_cfg.broker.address.transport = MQTT_TRANSPORT_OVER_SSL;
#endif              
                //mbedtls_x509_crt_parse(&certificate, mqtt_cfg.cert_pem, mqtt_cfg.cert_len);
                //printf("mbedtls verify result: %d\n", mbedtls_ssl_get_verify_result());
                //printf("The issuer is: %.*s\n", (int)certificate.issuer.val.len, certificate.issuer.val.p);
            }
            
            client = esp_mqtt_client_init(&mqtt_cfg);
            
            esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);

            mqttSetGlobalState(MQTT_STATE_DO_CONNECT);
        }
        break;

    case MQTT_STATE_DO_CONNECT:
//        printf("MQTT_STATE_DO_CONNECT\n");
        EhsMQTTConnectPoll(&connect, &host, &port, &gUseTLS, &clientid, &username, &password, &clientCertFileName, &clientKeyFileName, &rootCAFileName);
        mqttSetGlobalState(MQTT_STATE_WAIT_FOR_CONNECTION);
        if (connect && gMqttConnectionAttempts == 0)
        {
            err = esp_mqtt_client_start(client);
            gMqttConnectionAttempts++;

        }
        //if (*bNewSodlFlagRef == EHS_TRUE) mqttSetGlobalState(MQTT_STATE_DO_DISCONNECT);

        break;

    case MQTT_STATE_WAIT_FOR_CONNECTION:
        //printf("MQTT_STATE_WAIT_FOR_CONNECTION\n");
        break;

    case MQTT_STATE_CONNECTED:
//        printf("MQTT_STATE_CONNECTED\n");
        EhsMQTTConnectPoll(&connect, &host, &port, &gUseTLS, &clientid, &username, &password, &clientCertFileName, &clientKeyFileName, &rootCAFileName);
        if (*bNewSodlFlagRef == EHS_TRUE || connect == 0)
        {
            // Go to DO_DISCONNECT, reset gMqttConnectionAttempts to 0.
            mqttSetGlobalState(MQTT_STATE_DO_DISCONNECT);
            gMqttConnectionAttempts = 0;
            break;
        }
        if (gSubsribeAgain)
        {
            for (i = 0; i < topic_count; i++) esp_mqtt_client_subscribe(client, topic_list[i], gQoS_list[i]);
            gSubsribeAgain = EHS_FALSE;
        }
        EhsMQTTSubscribeWritePoll(gTopicNameTemporary, &gSubscribe, &gQoS);
        EhsMQTTPublishWritePoll(gTopicNameTemporaryPub, gMqttPayload, &gQoS_pub);
        if (gSubscribe)
        {
            if (topic_list == NULL)
            {
                topic_count = 1;
                topic_list = calloc(topic_count, sizeof(char *));
                gQoS_list = calloc(topic_count, sizeof(int));
                topic_list[topic_count - 1] = calloc(strlen(gTopicNameTemporary), sizeof(char));
                gQoS_list[topic_count - 1] = gQoS;
                strcpy(topic_list[topic_count - 1], gTopicNameTemporary);
                confirm_subscribe = 1;
            }
            else {
                for (i = 0; i < topic_count ; i++)
                {
                    if (strcmp(gTopicNameTemporary, topic_list[i]) != 0) // Not in list
                    {
                        if (i == topic_count - 1)
                        {
                            topic_count += 1;
                            topic_list = realloc(topic_list, topic_count * sizeof(char *));
                            gQoS_list = realloc(gQoS_list, topic_count * sizeof(int));
                            topic_list[topic_count - 1] = calloc(strlen(gTopicNameTemporary), sizeof(char));
                            strcpy(topic_list[topic_count - 1], gTopicNameTemporary);
                            gQoS_list[topic_count - 1] = gQoS;
                            confirm_subscribe = 1;
                            break;
                        }
                    }
                    else 
                    {
                        confirm_subscribe = 0;
                        break;
                    }
                }
            }
            //printf("Subscribe defined. Topic: %s, QoS: %d\n", gTopicNameTemporary, gQoS);
            if (confirm_subscribe) esp_mqtt_client_subscribe(client, gTopicNameTemporary, gQoS);
        }
        else {
            // Unsubscribe or not valid
            if (topic_list != NULL)
            {
                for (i = 0; i < topic_count ; i++)
                {
                    if (strcmp(gTopicNameTemporary, topic_list[i]) == 0 ) // In list, subscribed
                    {
                        memset(topic_list[i], 0, sizeof(char) * strlen(topic_list[i]));
                        free(topic_list[i]);
                        topic_list[i] = NULL;
                        topic_list[i] = topic_list[--topic_count];
                        gQoS_list[i] = gQoS_list[--topic_count];
                        topic_list = realloc(topic_list, topic_count * sizeof(char *));
                        gQoS_list = realloc(gQoS_list, topic_count * sizeof(int));
                        confirm_unsubscribe = 1;
                        break;
                    }
                }
            }
            if (confirm_unsubscribe) esp_mqtt_client_unsubscribe(client, gTopicNameTemporary);
        }

        // Publish the MQTT message to a topic if it exists. Determine the number of times of publish.
        if (gMqttPayload[0] != 0)
            esp_mqtt_client_publish(client, gTopicNameTemporaryPub, gMqttPayload, 0, gQoS_pub, 0); // retain flag false
        break;

    case MQTT_STATE_DO_DISCONNECT:
        printf("MQTT_STATE_DO_DISCONNECT\n");
        esp_mqtt_client_destroy(client);
        mqttSetGlobalState(MQTT_STATE_IDLE);
        if (*bNewSodlFlagRef == EHS_TRUE) mqttSetGlobalState(MQTT_STATE_DISCONNECTED_NEW);
        else mqttSetGlobalState(MQTT_STATE_DISCONNECTED);
        break;

    case MQTT_STATE_RECV_DATA:
        break;

    case MQTT_STATE_DISCONNECTED:
        printf("MQTT_STATE_DISCONNECTED\n");
        EhsMQTTConnectPoll(&connect, &host, &port, &gUseTLS, &clientid, &username, &password, &clientCertFileName, &clientKeyFileName, &rootCAFileName);
        if (connect)
        {
            gSubsribeAgain = EHS_TRUE;
        }
        else
        {
            gSubsribeAgain = EHS_FALSE;;
            for (i = 0; i < topic_count; i++)
            {
                if (topic_list[i] != NULL)
                {
                    memset(topic_list[i], 0, sizeof(char) * strlen(topic_list[0]));
                    free(topic_list[i]);
                    topic_list[i] = NULL;
                }
            }
        
            printf("topic: %p, QoS: %p\n", topic_list, gQoS_list);
            if (topic_list != NULL)
            {
                free(topic_list);
                topic_list = NULL;
            }
            if (gQoS_list != NULL)
            {
                memset(gQoS_list, 0, sizeof(int) * topic_count);
                free(gQoS_list);
                gQoS_list = NULL;
            }
            topic_count = 0;

            if (CA_str != NULL)
            {
                memset(CA_str, 0, sizeof(char) * strlen(CA_str));
                free(CA_str);
                CA_str = NULL;
            }
            if (ClientKEY_str != NULL)
            {
                memset(ClientKEY_str, 0, sizeof(char) * strlen(ClientKEY_str));
                free(ClientKEY_str);
                ClientKEY_str = NULL;
            }
            if (ClientCRT_str != NULL)
            {
                memset(ClientCRT_str, 0, sizeof(char) * strlen(ClientCRT_str));
                free(ClientCRT_str);
                ClientCRT_str = NULL;
            }
        }
        mqttSetGlobalState(MQTT_STATE_INIT);
        break;

    case MQTT_STATE_DISCONNECTED_NEW:
        printf("MQTT_STATE_DISCONNECTED_NEW\n");
        gSubsribeAgain = EHS_FALSE;
        for (i = 0; i < topic_count; i++)
        {
            if (topic_list[i] != NULL)
            {
                memset(topic_list[i], 0, sizeof(char) * strlen(topic_list[0]));
                free(topic_list[i]);
                topic_list[i] = NULL;
            }
        }

        printf("topic: %p, QoS: %p\n", topic_list, gQoS_list);
        if (topic_list != NULL)
        {
            free(topic_list);
            topic_list = NULL;
        }
        if (gQoS_list != NULL)
        {
            memset(gQoS_list, 0, sizeof(int) * topic_count);
            free(gQoS_list);
            gQoS_list = NULL;
        }
        topic_count = 0;

        if (CA_str != NULL)
        {
            memset(CA_str, 0, sizeof(char) * strlen(CA_str));
            free(CA_str);
            CA_str = NULL;
        }
        if (ClientKEY_str != NULL)
        {
            memset(ClientKEY_str, 0, sizeof(char) * strlen(ClientKEY_str));
            free(ClientKEY_str);
            ClientKEY_str = NULL;
        }
        if (ClientCRT_str != NULL)
        {
            memset(ClientCRT_str, 0, sizeof(char) * strlen(ClientCRT_str));
            free(ClientCRT_str);
            ClientCRT_str = NULL;
        }
        mqttSetGlobalState(MQTT_STATE_IDLE);
        Ehs_FB_ThreadComplete();
        break;
    }
}
