/**
 * @file
 * MQTT client
 *
 * @defgroup mqtt MQTT client
 * @ingroup apps
 * @verbinclude mqtt_client.txt
 */

// #define CONFIG_USE_SERVER_VERIFICATION 0

#include <string.h>
#include <stdio.h>
#include "globals.h"

#include "mqtt.h"
#include "hal_network.h"
#include "hal_mqtt.h"
//#include "inx-mqtt_publish.h"
#include "mqtt_client.h"
#include "hal_file.h"

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

/********************* INX CHANGES (START) *********************************************************************************/

typedef enum
{
    INX_FIRMWARE_UPDATE_NONE,
    INX_FIRMWARE_UPDATE_FIRMWARE,
    INX_FIRMWARE_UPDATE_SODL
} inxFirmwareUpdateType;

/********************* INX CHANGES (END) *********************************************************************************/


/* Global variables hidden away we wil want to fix*/

ehs_uint8 gUseTLS = 0;
char *host = "devman.inx-systems.com";
char *clientid = "xxxxx";
char *username = "xxxx";
char *password = "xxxxx";

ehs_uint16 port = 8883;

/* todo2024 we must remove this certificte from here! */
// server.crt
#include "esp_tls.h"
static const unsigned char CACert[]  = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDljCCAn6gAwIBAgIJAN2e3u3nI1pTMA0GCSqGSIb3DQEBCwUAMGAxHDAaBgNV
BAMME3d3dy5pbngtc3lzdGVtcy5jb20xDDAKBgNVBAoMA0lOWDENMAsGA1UECwwE
Um9vdDEjMCEGCSqGSIb3DQEJARYUaW5mb0Bpbngtc3lzdGVtcy5jb20wHhcNMjAw
NDA5MTM0MDE3WhcNMzIwNDA2MTM0MDE3WjBgMRwwGgYDVQQDDBN3d3cuaW54LXN5
c3RlbXMuY29tMQwwCgYDVQQKDANJTlgxDTALBgNVBAsMBFJvb3QxIzAhBgkqhkiG
9w0BCQEWFGluZm9AaW54LXN5c3RlbXMuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOC
AQ8AMIIBCgKCAQEA2MHAHGCdKEd/qBA7AkmbpOFoc/freNCuMbW+JtqwaovLCVCn
upFRMENlWZ2FgIFNYMTFP0PjK5TzrCD1sOajjdJ8VK+2pDisWQUfi59Fh3Bv6dtZ
XpeYYYe14cRG3JpfIdQz8cWiULtgHYRAMklMLPABHpb3Lv+agFzqgU0liYD32f7M
X30KwBuQKs1t7XpopcCyZ/grQpIAqAAWyTIZArlv78slejeN/Vqzdgf0sRx6L7VW
WKmvOCvl5yVZSyd5NVB8+FN1xOcx20I8LY3AQqDC3genUPE5PBq8wYBhSLxzQkkA
bJWkHOmWe/kdf9Ft3UBJuj2stc5BWmySoo9TkwIDAQABo1MwUTAdBgNVHQ4EFgQU
n4Sa2K/JwlIOPyJx6UxZZYxQ8YYwHwYDVR0jBBgwFoAUn4Sa2K/JwlIOPyJx6UxZ
ZYxQ8YYwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEABwrgYAkh
wwuZUyUjE8QhCNMKZjcRcgBtEtrlFcpfMStQitezVRl7ZJqEWBcntOKLLI7yvrMd
QuEM7dT/5vGzWW5xepXqLwN+b8pFkZwCJHWViirF8KxR91zUYA5G+n9ULCszX0ox
tM6YTAqKw/Su6HTXfEPWR/U9NFkjbyX1AX0q0VOTAonaZfntjaPXJ0JX5o7RL5k6
DD0z94ojcJHFGs1RJHY3X97XHlTX6T4wkdRRxUmLaKQzxcnVvtJxjAmluQ5Pls7j
ORez5qnIqQNkMOaw19XPKyNowcZoveiV/qYv7XnldSXXATClLPi+BTWb3I0V0Hwz
rgE+55lUkgkUzg==
-----END CERTIFICATE-----
)EOF";

// client.pem
static const unsigned char CLientCert[] = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEMjCCAxqgAwIBAgIJAK2dYmH3roCLMA0GCSqGSIb3DQEBCwUAMGAxHDAaBgNV
BAMME3d3dy5pbngtc3lzdGVtcy5jb20xDDAKBgNVBAoMA0lOWDENMAsGA1UECwwE
Um9vdDEjMCEGCSqGSIb3DQEJARYUaW5mb0Bpbngtc3lzdGVtcy5jb20wHhcNMjAw
NjEyMTAxNDIxWhcNMzIwNjA5MTAxNDIxWjAeMRwwGgYDVQQDDBNocmlAaW54LXN5
c3RlbXMuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAzApXJE/i
mk/GeIPMgGEcpLOc2PpFxiD0YoiznCILZvCQjjPCE3z57T/SC2DegX5vGw3NL7nd
mDW5/vuMOTU6+GDEp4sd58CSkcg2SA1X6/5DghFS0tVGlbz6Xfay/hxlf+TUhKTi
89A7TZvJesQUsz2+gjIAX9T5o5/kxk3tKly189MwcsNAF5sEDIG4I7LTyO0U1Yvi
P8yd9ATeStC3ZMkcyPIup16ihjSyo9IyM4DcMiNstsPKLNlSBv9/XBpu5KvLC/RR
OzpLwViJ14jkTT7uVGQIQcp4jLCVq/XhFN65ywz+gy4uNgp+8AaB+p9y0KWV+2lX
niM2SOuY9x3DswIDAQABo4IBLzCCASswDAYDVR0TAQH/BAIwADARBglghkgBhvhC
AQEEBAMCBaAwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMEMAsGA1UdDwQE
AwIDqDAoBglghkgBhvhCAQ0EGxYZQ2xpZW50IEJyb2tlciBDZXJ0aWZpY2F0ZTAd
BgNVHQ4EFgQUgpB2TvibcxeWOiU6wP0UKvQKq/8wgZIGA1UdIwSBijCBh4AUn4Sa
2K/JwlIOPyJx6UxZZYxQ8YahZKRiMGAxHDAaBgNVBAMME3d3dy5pbngtc3lzdGVt
cy5jb20xDDAKBgNVBAoMA0lOWDENMAsGA1UECwwEUm9vdDEjMCEGCSqGSIb3DQEJ
ARYUaW5mb0Bpbngtc3lzdGVtcy5jb22CCQDdnt7t5yNaUzANBgkqhkiG9w0BAQsF
AAOCAQEAyXvRAF8u1O9HH7N6iYeoXFfHxHnC/vXQonwAPMWaVeC83VLAl09+IBQ7
J4hFUINwXS5PaxVoI4pNxZW0qHBVVIGCaFuZwpyzxQbIJzHa3GmMohldqczPcrp5
dNxl+jLFLjtlwxBrzPsWEgzcBOlW/sHpfwpqC12ngQD/67ICfb6lOmaH0SG/uPT8
QZBWiR0gXkeuaeeXyLiQuxi4gecrDrW53L5bf4BHcYtL+zsdVGLJ2YtgZCIfGaiL
cBUpcZRiDQK1AEToFv+zRsTjhns+DjEvbDe0e35xqU3HIBiu0SlRdjpIA+51MQp0
EYF93v0EthfLYBgvEz/Ex1Wo83LDdg==
-----END CERTIFICATE-----
)EOF";

// client.pem
static const unsigned char ClientKey[] = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAzApXJE/imk/GeIPMgGEcpLOc2PpFxiD0YoiznCILZvCQjjPC
E3z57T/SC2DegX5vGw3NL7ndmDW5/vuMOTU6+GDEp4sd58CSkcg2SA1X6/5DghFS
0tVGlbz6Xfay/hxlf+TUhKTi89A7TZvJesQUsz2+gjIAX9T5o5/kxk3tKly189Mw
csNAF5sEDIG4I7LTyO0U1YviP8yd9ATeStC3ZMkcyPIup16ihjSyo9IyM4DcMiNs
tsPKLNlSBv9/XBpu5KvLC/RROzpLwViJ14jkTT7uVGQIQcp4jLCVq/XhFN65ywz+
gy4uNgp+8AaB+p9y0KWV+2lXniM2SOuY9x3DswIDAQABAoIBAEeEXFTq9zc1PRCi
ybw5zathFVnsJyayCFa9KY9RU4PKUDC3rY/dH1DsLLg2ROdIpLLr3H6c/DHMqxvX
mae3o6rJaER9e4butbEJZxVMMAQUxB7D6a7ieHIloe/rd4Of+megkmjHzpC87Pxf
3Xf9YHAO9UZ3NjBg2vtiIPLs/3L4GcYWxtEK4vUDSaktjR36ZE+mhCi9zi6/raQs
ciC8dctg55STjiruH2mjHKc0Q5BZIm/cEmdzlI1KIr6qcan9qxcOWkAJvk1iw2OE
E/BFuOmSyF05Ku+B9h9Ca9epCf9OmjJ4rOgSFpe8ax3scxKuX7B1v1oE7BI9taW9
D/dJzfkCgYEA6CQxO45dvTqfQc09uhtMOYrkKmOZxAjHgg+atQDB1HNrvQTsMIUc
c9Dry8AzQvp1qCtxFHJJI36qLR40z+Mgv0W9G0AV6gD80dGg9DsksmZfz1I0IHPt
APzK/rzmBwUxha+mbhUyIEHr0E1k4c5KTdUk9ty111quy9UIU0WAVc0CgYEA4QLK
dOwLgiD5JLnL/xA38s6dOo9YZykaH4QO+UbdQRIqk/YMq83Y0ZOLaLJUZxjlMRhg
lf2wgyoUtYW8yNrXruv6/J9B6Jdc3JhtpZaT5wTMQ+4fAjhfxcKv9g/wgZTQJaQC
3IQP1vf5yM9TWMqRdEyyQAP2Zn7hLRYN78bs/38CgYEA3mv3BBI3PJzThI+rCgsn
sKakUP0Xkl27EqHy4GR2UyG34+a4ixCvH0TY+GMSeqVRYC0nbqjF7Q1q7knX3ivH
75xZQafYl1oU4tCRABE+6vPt7s3bhhCsz9pmgUbqu2U9etK+ixEh8whWckIFW9l1
FjgX77OvQ0Tu3/wguvCHsy0CgYACaCI8P+4blTi1ANvCvayrhzQ9VR0v9UuXPLVg
SzbApG4aKQ6Xo9sG2top44Ztl+GTtdwKnq0NqhyQmH6dc2HNOC72FsoAr+XJYkQB
+mtYlD+PFnP0dR/pIJz7N85txGa5kFiLfUoiFYLxd+4/S+NkaBGcB67SB9dmwQ6j
GvCcBwKBgQCC1txp4BCp7O6GLl51K9qNLWGzdBhY1PPu5O14atvzLS9EJtxmhpqY
5FJwUVWLVemnrjIT5GRioLrtoz97o1Tmle1soW5RALPHFRDgj9bfLFI1OoTbUZNH
eL/rtlUM1NUQMzKUeo37vC8D0gUTRrXvbbCZ3/EbJ/k7z0/neUh8Yg==
-----END RSA PRIVATE KEY-----
)EOF";

// actual mqtt stuff

esp_mqtt_client_config_t mqtt_cfg;
esp_mqtt_client_handle_t client;
static MQTT_State_t MQTT_state = MQTT_STATE_INIT;
static ehs_uint32 gMqttConnectionAttempts = 0;




void mqttSetGlobalState(const MQTT_State_t newState)
{
    MQTT_state = newState;
}

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
  //  printf("mqtt_event_handler\n");

    /*ESP_LOGI ("TEST", "MQTT msgid= %d event: %d. MQTT_EVENT_CONNECTED", event->msg_id, event->event_id);
            esp_mqtt_client_subscribe (client, "test/hello", 0);
            esp_mqtt_client_publish (client, "test/status", "1", 1, 0, false);
        }
        else if (event->event_id == MQTT_EVENT_DISCONNECTED) {
            ESP_LOGI ("TEST", "MQTT event: %d. MQTT_EVENT_DISCONNECTED", event->event_id);
            //esp_mqtt_client_reconnect (event->client); //not needed if autoconnect is enabled
        } else  if (event->event_id == MQTT_EVENT_SUBSCRIBED) {
            ESP_LOGI ("TEST", "MQTT msgid= %d event: %d. MQTT_EVENT_SUBSCRIBED", event->msg_id, event->event_id);
        } else  if (event->event_id == MQTT_EVENT_UNSUBSCRIBED) {
            ESP_LOGI ("TEST", "MQTT msgid= %d event: %d. MQTT_EVENT_UNSUBSCRIBED", event->msg_id, event->event_id);
        } else  if (event->event_id == MQTT_EVENT_PUBLISHED) {
            ESP_LOGI ("TEST", "MQTT event: %d. MQTT_EVENT_PUBLISHED", event->event_id);
        } else  if (event->event_id == MQTT_EVENT_DATA) {
            ESP_LOGI ("TEST", "MQTT msgid= %d event: %d. MQTT_EVENT_DATA", event->msg_id, event->event_id);
            ESP_LOGI ("TEST", "Topic length %d. Data length %d", event->topic_len, event->data_len);
            ESP_LOGI ("TEST","Incoming data: %.*s %.*s\n", event->topic_len, event->topic, event->data_len, event->data);

        } else  if (event->event_id == MQTT_EVENT_BEFORE_CONNECT) {
            ESP_LOGI ("TEST", "MQTT event: %d. MQTT_EVENT_BEFORE_CONNECT", event->event_id);


            */
    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
       // printf("MQTT msgid= %d event: %d. MQTT_EVENT_CONNECTED to: %s\n", event->msg_id, event->event_id, host);
        mqttSetGlobalState(MQTT_STATE_CONNECTED);
        EhsMQTTConnectEvent(true);
        break;
    case MQTT_EVENT_DISCONNECTED:
      //  printf("MQTT event: %d. MQTT_EVENT_DISCONNECTED", event->event_id);
        mqttSetGlobalState(MQTT_STATE_DISCONNECTED);
        EhsMQTTConnectEvent(false);
        break;

    case MQTT_EVENT_SUBSCRIBED:
        //printf("MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        //   msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        //     printf("sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
      //  printf("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
      //  printf("MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
      //  printf("MQTT_EVENT_DATA");
      //  printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
      //  printf("DATA=%.*s\r\n", event->data_len, event->data);
        // if (strncmp(event->data, "send binary please", event->data_len) == 0)
        // {
        //     printf("Sending the binary");
        //     send_binary(client);
        // }
        break;
    case MQTT_EVENT_ERROR:
    //todo change all these prnitfs to use EHSH_LOG_ERROR so we enable and disable them
        //printf("MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
           // printf("Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
            // printf("Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            // printf("Last captured errno : %d (%s)", event->error_handle->esp_transport_sock_errno,
            //        strerror(event->error_handle->esp_transport_sock_errno));
        }
        else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED)
        {
            //printf("Connection refused error: 0x%x", event->error_handle->connect_return_code);
        }
        else
        {
            //printf("Unknown error type: 0x%x", event->error_handle->error_type);
        }
        break;
    default:
        //printf("Other event id:%d", event->event_id);
        //printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    }
}

/* This polls MQTT for connections, When they are established initialises the MQTTs

*/
void* EhsMqttClientLoop(void* arg)
{

    esp_err_t err;

    // char *host = "test.mosquitto.org";
    // char *clientid = "clientid";
    // char *username = "";
    // char *password = "";
    char *clientCertFileName = "";
    char *clientKeyFileName = "";
    char *rootCAFileName = "";
    ehs_bool connect = 0;
    
    EhsMQTTConnectPoll(&connect, &host, &port, &gUseTLS, &clientid, &username, &password, &clientCertFileName, &clientKeyFileName, &rootCAFileName);
   
    //printf("%s", host);
    switch (MQTT_state)
    {
    case MQTT_STATE_INIT:

        if (connect && gMqttConnectionAttempts == 0)
        {
            //printf("Init connection init %s %i\n", host, port);
            mqtt_cfg.host = host;
            mqtt_cfg.port = port;

            mqtt_cfg.username = username;
            mqtt_cfg.password = password;
            mqtt_cfg.keepalive = 15;
            mqtt_cfg.event_handle = mqtt_event_handler;
            if(gUseTLS==1){
                printf("useing inx ssl\n\n");
                mqtt_cfg.cert_pem = CACert;
                mqtt_cfg.client_cert_pem = CLientCert;
                mqtt_cfg.client_key_pem = ClientKey;
                mqtt_cfg.transport = MQTT_TRANSPORT_OVER_SSL;
            }
            // mqtt_cfg.lwt_topic = "test/status";
            // mqtt_cfg.lwt_msg = "0";
            // mqtt_cfg.lwt_msg_len = 1;
            // printf("filetest %s\n",inxEHSFilesGetData(inxEHSFilesFind(clientCertFileName)));

            // // read file
            // const char *name = "/ehs/";

            // char *name_with_extension;
            // name_with_extension = malloc(strlen(name) + 1 + 4); /* make space for the new string (should check the return value ...) */
            // strcpy(name_with_extension, name);                  /* copy name into the new var */
            // strcat(name_with_extension, clientCertFileName);    /* add the extension */
            //                                                     // free(name_with_extension);
            // FILE *demo;
            // int display;

            // Creates a file "demo_file"
            // with file access as read mode
           // printf("name with : %s\n", name_with_extension);
            //     demo = fopen(name_with_extension, "r");
            // char *CACertificate;
            // int CALen = 0;
            // // loop to extract every characters
            // while (1)
            // {
            //     // reading file
            //     display = fgetc(demo);

            //     // end of file indicator
            //     if (feof(demo))
            //         break;
            //     CACertificate = malloc(strlen(CALen) + 1);
            //     strcat(CACertificate, CACertificate);
            //     // displaying every characters
            //     printf("%c\n", display);
            // }

            // // closes the file pointed by demo
            // fclose(demo);
            // printf("%s\n", CACertificate);
            // // end read file?
            // 
            //  esp_err_t err = esp_tls_set_global_ca_store(CACertificate, sizeof(CACertificate));
            client = esp_mqtt_client_init(&mqtt_cfg);

            err = esp_mqtt_client_start(client);

            gMqttConnectionAttempts++;
          //  printf("end init %i state\n", err);
        }
        break;
    case MQTT_STATE_IDLE:
      //  printf("MQTT_STATE_IDEL\n");
       
        break;

    case MQTT_STATE_CONNECTED:
      // printf("MQTT_STATE_CONNECTED\n");
        break;

    case MQTT_STATE_DISCONNECTED:
      //  printf("MQTT_STATE_DISCONNECTED\n");

        break;
    }
    return NULL;
}