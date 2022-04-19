/**
 * @file
 * MQTT client
 *
 * @defgroup mqtt MQTT client
 * @ingroup apps
 * @verbinclude mqtt_client.txt
 */

#include "heatrod_config.h"
#include "errorLogs.h"
#include "inx_led.h"
#include "inx_utils.h"
#include "inx_display.h"

#if MQTT_USE_TLS || CONFIG_USE_DNS || CONFIG_USE_DHCP
#include "RNG1.h"
#include "fsl_rnga.h"
/*!
 * \brief return a 32bit random number
 */
unsigned int random32(void) {
  uint32_t data = 0;
  RNGA_GetRandomData(RNG, &data, sizeof(data));
  return data;
}
#endif //MQTT_USE_TLS || CONFIG_USE_DNS || CONFIG_USE_DHCP

#if CONFIG_MQTT_ENABLED
#include "certificate.h"
#include "mqtt.h"
#include "fsl_rtc.h"
#include "lwip/timeouts.h"
#include "lwip/ip_addr.h"
#include "lwip/mem.h"
#include "lwip/err.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

#if MQTT_USE_TLS
#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509.h"
#include "mbedtls/debug.h"
#include "mbedtls/certs.h"
#include "mbedtls/pk.h"
#endif

#if LWIP_TCP && LWIP_CALLBACK_API

/**
 * MQTT_DEBUG: Default is off.
 */
#if !defined MQTT_DEBUG || defined __DOXYGEN__
#define MQTT_DEBUG                  LWIP_DBG_OFF /*LWIP_DBG_OFF*/
#endif

#define MQTT_DEBUG_TRACE        (MQTT_DEBUG | LWIP_DBG_TRACE)
#define MQTT_DEBUG_STATE        (MQTT_DEBUG | LWIP_DBG_STATE)
#define MQTT_DEBUG_WARN         (MQTT_DEBUG | LWIP_DBG_LEVEL_WARNING)
#define MQTT_DEBUG_WARN_STATE   (MQTT_DEBUG | LWIP_DBG_LEVEL_WARNING | LWIP_DBG_STATE)
#define MQTT_DEBUG_SERIOUS      (MQTT_DEBUG | LWIP_DBG_LEVEL_SERIOUS)

static void mqtt_cyclic_timer(void *arg);
bool EhsMQTTPublishWritePoll(char* topic, char* payload,uint8_t* qos);
/**
 * MQTT client connection states
 */
enum {
  TCP_DISCONNECTED,
  TCP_CONNECTING,
#if MQTT_USE_TLS
  TLS_HANDSHAKING,
#endif
  MQTT_CONNECTING,
  MQTT_CONNECTED
};
typedef enum {
  INX_FIRMWARE_UPDATE_NONE,
  INX_FIRMWARE_UPDATE_FIRMWARE,
  INX_FIRMWARE_UPDATE_SODL
} inxFirmwareUpdateType;
extern inxPID_t gPIDs[CONFIG_PID_CONTROLLERS_NUM];
static uint8_t gUseTLS=0;
extern uint32_t gMqttConnections;
static volatile bool gGenerateCommissioningReport=false;
volatile bool gGenerateDiagnosticReport=false;
static volatile bool gUserTemperatureAlarmTriggeredPreviousValue=false;
static ip4_addr_t brokerServerAddress;
static char gFirmwareVersion[CONFIG_FIRMWARE_NAME_MAX_LENGTH+1];
static uint32_t gFirmwareCrc = 0; //gets set when we are told to download some firmware
static MQTT_State_t MQTT_state = MQTT_STATE_INIT;
static int32_t gMqttInpubId=-1; /* ID of incoming data */
static mqtt_client_t gMqttClient; /* descriptor holding all the needed client information */
static uint32_t gMqttConnectionAttempts = 0;
static uint32_t gMqttTlsWaitForConnectionTicks=0;
static uint32_t gFirmwareByteWeAreWaitingFor=0;
//static char gStatusPayload[INX_MQTT_MAX_PAYLOAD_SIZE];
static char gMqttPayload[INX_MQTT_MAX_PAYLOAD_SIZE];
static inxFirmwareUpdateType gFirmwareStartOTA=INX_FIRMWARE_UPDATE_NONE;
static inxFirmwareUpdateType gFirmwareOTAStarted=INX_FIRMWARE_UPDATE_NONE;
static uint32_t gFirmwareCurrentByte=0;
static uint32_t gFirmwareWaitingForChunkTicks=0;
static uint32_t gFirmwareCurrentSector=0;
extern u8_t gOTAUpdateOneSectorBuffer[CONFIG_FLASH_SECTOR_SIZE_BYTES+CONFIG_FIRMWARE_CHUNK_SIZE];
static char gTopicNameTemporary[CONFIG_MQTT_MAX_TOPIC_LENGTH] = "";
static char gMqttProvisioningCertificate[CONFIG_PROVISIONING_CERTIFICATE_MAX_LENGTH] = "";
static const char* gMqttClientId; //is set when our topic names are set

/**
 * MQTT control message types
 */
enum mqtt_message_type {
  MQTT_MSG_TYPE_CONNECT = 1,
  MQTT_MSG_TYPE_CONNACK = 2,
  MQTT_MSG_TYPE_PUBLISH = 3,
  MQTT_MSG_TYPE_PUBACK = 4,
  MQTT_MSG_TYPE_PUBREC = 5,
  MQTT_MSG_TYPE_PUBREL = 6,
  MQTT_MSG_TYPE_PUBCOMP = 7,
  MQTT_MSG_TYPE_SUBSCRIBE = 8,
  MQTT_MSG_TYPE_SUBACK = 9,
  MQTT_MSG_TYPE_UNSUBSCRIBE = 10,
  MQTT_MSG_TYPE_UNSUBACK = 11,
  MQTT_MSG_TYPE_PINGREQ = 12,
  MQTT_MSG_TYPE_PINGRESP = 13,
  MQTT_MSG_TYPE_DISCONNECT = 14
};

/** Helpers to extract control packet type and qos from first byte in fixed header */
#define MQTT_CTL_PACKET_TYPE(fixed_hdr_byte0) ((fixed_hdr_byte0 & 0xf0) >> 4)
#define MQTT_CTL_PACKET_QOS(fixed_hdr_byte0) ((fixed_hdr_byte0 & 0x6) >> 1)

/**
 * MQTT connect flags, only used in CONNECT message
 */
enum mqtt_connect_flag {
  MQTT_CONNECT_FLAG_USERNAME = 1 << 7,
  MQTT_CONNECT_FLAG_PASSWORD = 1 << 6,
  MQTT_CONNECT_FLAG_WILL_RETAIN = 1 << 5,
  MQTT_CONNECT_FLAG_WILL = 1 << 2,
  MQTT_CONNECT_FLAG_CLEAN_SESSION = 1 << 1
};


//#if defined(LWIP_DEBUG)
/*static const char * const mqtt_message_type_str[15] =
{
  "UNDEFINED",
  "CONNECT",
  "CONNACK",
  "PUBLISH",
  "PUBACK",
  "PUBREC",
  "PUBREL",
  "PUBCOMP",
  "SUBSCRIBE",
  "SUBACK",
  "UNSUBSCRIBE",
  "UNSUBACK",
  "PINGREQ",
  "PINGRESP",
  "DISCONNECT"
};
*/
MQTT_State_t mqttGetGlobalState(){
	return MQTT_state;
}

void mqttSetGlobalState(const MQTT_State_t newState){
	MQTT_state=newState;
}

void mqttSetTopicNames(const char* clientId){
	gMqttClientId=clientId;
}

/**
 * Message type value to string
 * @param msg_type see enum mqtt_message_type
 *
 * @return Control message type text string
 */
/*static const char *
mqtt_msg_type_to_str(u8_t msg_type)
{
  if (msg_type >= LWIP_ARRAYSIZE(mqtt_message_type_str)) {
    msg_type = 0;
  }
  return mqtt_message_type_str[msg_type];
}
*/
//#endif

void mqttSetBrokerAddress(const ip4_addr_t address){
	brokerServerAddress=address;
}

void mqttSendProvisioningCertificate(const char* certificate){
	//copy the certificate somewhere safe
	strncpy(gMqttProvisioningCertificate,certificate,CONFIG_PROVISIONING_CERTIFICATE_MAX_LENGTH-1);
	//raise a flag for our network handling loop to send it?
	//actually we can just use the strlen of the certificate to know if we should sent it or not
}

/**
 * Generate MQTT packet identifier
 * @param client MQTT client
 * @return New packet identifier, range 1 to 65535
 */
static u16_t
msg_generate_packet_id(mqtt_client_t *client)
{
  client->pkt_id_seq++;
  if (client->pkt_id_seq == 0) {
    client->pkt_id_seq++;
  }
  return client->pkt_id_seq;
}

/*--------------------------------------------------------------------------------------------------------------------- */
/* Output ring buffer */


#define MQTT_RINGBUF_IDX_MASK ((MQTT_OUTPUT_RINGBUF_SIZE) - 1)

/** Add single item to ring buffer */
#define mqtt_ringbuf_put(rb, item) \
  ((rb)->buf)[(rb)->put++ & MQTT_RINGBUF_IDX_MASK] = (item)

/** Return number of bytes in ring buffer */
#define mqtt_ringbuf_len(rb) ((u16_t)((rb)->put - (rb)->get))

/** Return number of bytes free in ring buffer */
#define mqtt_ringbuf_free(rb) (MQTT_OUTPUT_RINGBUF_SIZE - mqtt_ringbuf_len(rb))

/** Return number of bytes possible to read without wrapping around */
#define mqtt_ringbuf_linear_read_length(rb) LWIP_MIN(mqtt_ringbuf_len(rb), (MQTT_OUTPUT_RINGBUF_SIZE - ((rb)->get & MQTT_RINGBUF_IDX_MASK)))

/** Return pointer to ring buffer get position */
#define mqtt_ringbuf_get_ptr(rb) (&(rb)->buf[(rb)->get & MQTT_RINGBUF_IDX_MASK])

#define mqtt_ringbuf_advance_get_idx(rb, len) ((rb)->get += (len))

static void buildFirmwareChunkPayload(char* payload,const uint32_t startByte){
	snprintf(payload,INX_MQTT_MAX_PAYLOAD_SIZE,"{\"startByte\":%d, \"numBytes\":"CONFIG_FIRMWARE_CHUNK_SIZE_STRING", \"firmware\":\"%s\"}",
			startByte,gFirmwareVersion);
}

extern uint8_t gRamUsedPercent;

extern bool gProvisioned;
extern INX_Local_UI_Mode_t gLocalUIMode;
extern inxAuxControlEnum_t gAuxControlOutput;
extern inxPowerModeEnum_t gPowerMode;
extern inxControllerContactorMode_t gControllerContactorMode;
extern inxControllerOutputMode_t gControllerOutputMode;
extern dutyCycleState_t dutyCycleState0;
extern dutyCycleState_t dutyCycleState1;
extern dutyCycleState_t dutyCycleState2;
extern inxPID_t gPIDs[];
extern float gDutyCycleLengthEMRSeconds;
extern bool gRTCDisabled;
extern inxSafetyMode_t gSafetyMode;
extern float gDutyCycleLengthSSRSeconds;
extern float gCal1;
extern float gCal2;
extern float gCal3;
extern u16_t gScheduleCrc;

/**
 * Try send as many bytes as possible from output ring buffer
 * @param rb Output ring buffer
 * @param tpcb TCP connection handle
 */
static void
mqtt_output_send(mqtt_client_t *client, struct mqtt_ringbuf_t *rb, struct tcp_pcb *tpcb)
{
  int nof;
  err_t err;
  if(tpcb==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("tpcb == NULL, ignoring send"));
	  return;
  }
  u8_t wrap = 0;
  u16_t ringbuf_lin_len = mqtt_ringbuf_linear_read_length(rb);
  u16_t send_len = tcp_sndbuf(tpcb);

  if (send_len == 0 || ringbuf_lin_len == 0) {
    return;
  }

  LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_output_send: tcp_sndbuf: %d bytes, ringbuf_linear_available: %d, get %d, put %d\n",
                                send_len, ringbuf_lin_len, ((rb)->get & MQTT_RINGBUF_IDX_MASK), ((rb)->put & MQTT_RINGBUF_IDX_MASK)));

  if (send_len > ringbuf_lin_len) {
    /* Space in TCP output buffer is larger than available in ring buffer linear portion */
    send_len = ringbuf_lin_len;
    /* Wrap around if more data in ring buffer after linear portion */
    wrap = (mqtt_ringbuf_len(rb) > ringbuf_lin_len);
  }
  if(gUseTLS){
    LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_output_send: mbedtls_ssl_write: bytes %d\r\n", send_len));
    nof = mbedtls_ssl_write(client->ssl_context, mqtt_ringbuf_get_ptr(rb), send_len);
    if (nof==send_len) {
      err = ERR_OK;
    } else {
      err = ERR_BUF; /* just assign an error */
    }
  }else{//if(gUseTLS)
    err = tcp_write(tpcb, mqtt_ringbuf_get_ptr(rb), send_len, TCP_WRITE_FLAG_COPY | (wrap ? TCP_WRITE_FLAG_MORE : 0));
  }//else if(gUseTLS)
  if ((err == ERR_OK) && wrap) {
    mqtt_ringbuf_advance_get_idx(rb, send_len);
    /* Use the lesser one of ring buffer linear length and TCP send buffer size */
    send_len = LWIP_MIN(tcp_sndbuf(tpcb), mqtt_ringbuf_linear_read_length(rb));
    if(gUseTLS){
      printf("PBB 321 mbedtls_ssl_write: bytes %d\n", send_len);
      nof = mbedtls_ssl_write(client->ssl_context, mqtt_ringbuf_get_ptr(rb), send_len);
      if (nof==send_len) {
        err = ERR_OK;
      } else {
        err = ERR_BUF; /* just assign an error */
      }
    }else{//if(gUseTLS)
      err = tcp_write(tpcb, mqtt_ringbuf_get_ptr(rb), send_len, TCP_WRITE_FLAG_COPY);
    }//else if(gUseTLS)
  }
  if (err == ERR_OK) {
    mqtt_ringbuf_advance_get_idx(rb, send_len);
    /* Flush */
    tcp_output(tpcb);
  } else {
    LWIP_DEBUGF(MQTT_DEBUG_WARN, ("mqtt_output_send: Send failed with err %d (\"%s\")\n", err, lwip_strerr(err)));
  }
}



/*--------------------------------------------------------------------------------------------------------------------- */
/* Request queue */

/**
 * Create request item
 * @param r_objs Pointer to request objects
 * @param pkt_id Packet identifier of request
 * @param cb Packet callback to call when requests lifetime ends
 * @param arg Parameter following callback
 * @return Request or NULL if failed to create
 */
static struct mqtt_request_t *
mqtt_create_request(struct mqtt_request_t *r_objs, u16_t pkt_id, mqtt_request_cb_t cb, void *arg)
{
  struct mqtt_request_t *r = NULL;
  u8_t n;
  if(r_objs==NULL){
	  return r;
  }

  u8_t freeRequests=0;
  for (n = 0; n < MQTT_REQ_MAX_IN_FLIGHT; n++) {
      /* Item point to itself if not in use */
      if (r_objs[n].next == &r_objs[n]) {
        freeRequests=freeRequests+1;
      }
    }
  //printf("mqtt free slots=%d\n",freeRequests);
  for (n = 0; n < MQTT_REQ_MAX_IN_FLIGHT; n++) {
    /* Item point to itself if not in use */
    if (r_objs[n].next == &r_objs[n]) {
      r = &r_objs[n];
      r->next = NULL;
      r->cb = cb;
      r->arg = arg;
      r->pkt_id = pkt_id;
      break;
    }
  }
  return r;
}


/**
 * Append request to pending request queue
 * @param tail Pointer to request queue tail pointer
 * @param r Request to append
 */
static void
mqtt_append_request(struct mqtt_request_t **tail, struct mqtt_request_t *r)
{
  struct mqtt_request_t *head = NULL;
  s16_t time_before = 0;
  struct mqtt_request_t *iter;

  if(tail==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_append_request: tail==null cannot append request"));
	  return;
  }

  /* Iterate trough queue to find head, and count total timeout time */
  for (iter = *tail; iter != NULL; iter = iter->next) {
    time_before += iter->timeout_diff;
    head = iter;
  }

  if(time_before > MQTT_REQ_TIMEOUT){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_append_request: time_before > MQTT_REQ_TIMEOUT cannot append request"));
	  return;
  }

  r->timeout_diff = MQTT_REQ_TIMEOUT - time_before;
  if (head == NULL) {
    *tail = r;
  } else {
    head->next = r;
  }
}


/**
 * Delete request item
 * @param r Request item to delete
 */
static void
mqtt_delete_request(struct mqtt_request_t *r)
{
  if (r != NULL) {
    r->next = r;
  }
}

/**
 * Remove a request item with a specific packet identifier from request queue
 * @param tail Pointer to request queue tail pointer
 * @param pkt_id Packet identifier of request to take
 * @return Request item if found, NULL if not
 */
static struct mqtt_request_t *
mqtt_take_request(struct mqtt_request_t **tail, u16_t pkt_id)
{
  struct mqtt_request_t *iter = NULL, *prev = NULL;
  if(tail==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_take_request: tail == NULL cannot take request"));
	  return NULL;
  }

  /* Search all request for pkt_id */
  for (iter = *tail; iter != NULL; iter = iter->next) {
    if (iter->pkt_id == pkt_id) {
      break;
    }
    prev = iter;
  }

  /* If request was found */
  if (iter != NULL) {
    /* unchain */
    if (prev == NULL) {
      *tail= iter->next;
    } else {
      prev->next = iter->next;
    }
    /* If exists, add remaining timeout time for the request to next */
    if (iter->next != NULL) {
      iter->next->timeout_diff += iter->timeout_diff;
    }
    iter->next = NULL;
  }
  return iter;
}

/**
 * Handle requests timeout
 * @param tail Pointer to request queue tail pointer
 * @param t Time since last call in seconds
 */
static void
mqtt_request_time_elapsed(struct mqtt_request_t **tail, u8_t t)
{
  struct mqtt_request_t *r;
  if(tail==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_request_time_elapsed: tail == NULL cannot request"));
	  return;
  }

  r = *tail;
  while (t > 0 && r != NULL) {
    if (t >= r->timeout_diff) {
      t -= (u8_t)r->timeout_diff;
      /* Unchain */
      *tail = r->next;
      /* Notify upper layer about timeout */
      if (r->cb != NULL) {
        r->cb(r->arg, ERR_TIMEOUT);
      }
      mqtt_delete_request(r);
      /* Tail might be be modified in callback, so re-read it in every iteration */
      r = *(struct mqtt_request_t * const volatile *)tail;
    } else {
      r->timeout_diff -= t;
      t = 0;
    }
  }
}

/**
 * Free all request items
 * @param tail Pointer to request queue tail pointer
 */
static void
mqtt_clear_requests(struct mqtt_request_t **tail)
{
  struct mqtt_request_t *iter, *next;
  if(tail==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_clear_requests: tail == NULL cannot clear"));
	  return;
  }

  for (iter = *tail; iter != NULL; iter = next) {
    next = iter->next;
    mqtt_delete_request(iter);
  }
  *tail = NULL;
}
/**
 * Initialize all request items
 * @param r_objs Pointer to request objects
 */
static void
mqtt_init_requests(struct mqtt_request_t *r_objs)
{
  u8_t n;
  if(r_objs==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_init_requests: r_objs == NULL cannot init"));
	  return;
  }

  for (n = 0; n < MQTT_REQ_MAX_IN_FLIGHT; n++) {
    /* Item pointing to itself indicates unused */
    r_objs[n].next = &r_objs[n];
  }
}

/*--------------------------------------------------------------------------------------------------------------------- */
/* Output message build helpers */


static void
mqtt_output_append_u8(struct mqtt_ringbuf_t *rb, u8_t value)
{
  mqtt_ringbuf_put(rb, value);
}

static
void mqtt_output_append_u16(struct mqtt_ringbuf_t *rb, u16_t value)
{
  mqtt_ringbuf_put(rb, value >> 8);
  mqtt_ringbuf_put(rb, value & 0xff);
}

static void
mqtt_output_append_buf(struct mqtt_ringbuf_t *rb, const void *data, u16_t length)
{
  u16_t n;
  for (n = 0; n < length; n++) {
    mqtt_ringbuf_put(rb, ((const u8_t *)data)[n]);
  }
}

static void
mqtt_output_append_string(struct mqtt_ringbuf_t *rb, const char *str, u16_t length)
{
  u16_t n;
  mqtt_ringbuf_put(rb, length >> 8);
  mqtt_ringbuf_put(rb, length & 0xff);
  for (n = 0; n < length; n++) {
    mqtt_ringbuf_put(rb, str[n]);
  }
}

/**
 * Append fixed header
 * @param rb Output ring buffer
 * @param msg_type see enum mqtt_message_type
 * @param dup MQTT DUP flag
 * @param qos MQTT QoS field
 * @param retain MQTT retain flag
 * @param r_length Remaining length after fixed header
 */

static void
mqtt_output_append_fixed_header(struct mqtt_ringbuf_t *rb, u8_t msg_type, u8_t dup,
                 u8_t qos, u8_t retain, u16_t r_length)
{
  /* Start with control byte */
  mqtt_output_append_u8(rb, (((msg_type & 0x0f) << 4) | ((dup & 1) << 3) | ((qos & 3) << 1) | (retain & 1)));
  /* Encode remaining length field */
  do {
    mqtt_output_append_u8(rb, (r_length & 0x7f) | (r_length >= 128 ? 0x80 : 0));
    r_length >>= 7;
  } while (r_length > 0);
}


/**
 * Check output buffer space
 * @param rb Output ring buffer
 * @param r_length Remaining length after fixed header
 * @return 1 if message will fit, 0 if not enough buffer space
 */
static u8_t
mqtt_output_check_space(struct mqtt_ringbuf_t *rb, u16_t r_length)
{
  /* Start with length of type byte + remaining length */
  u16_t total_len = 1 + r_length;

  if(rb==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_output_check_space: rb == NULL cannot check space"));
	  return 0;
  }


 /* Calculate number of required bytes to contain the remaining bytes field and add to total*/
  do {
    total_len++;
    r_length >>= 7;
  } while (r_length > 0);

  return (total_len <= mqtt_ringbuf_free(rb));
}

#if MQTT_USE_TLS
err_t tls_close(mqtt_client_t* pClient) {
  void MQTT_TlsClose(void); /*! \todo closing connection is not done in a clean way yet! */

  MQTT_TlsClose();
  pClient->ssl_context=NULL; //NULL the ssl_context because we can no longer use it
  return tcp_close(pClient->conn);
}
#endif


/**
 * Close connection to server
 * @param client MQTT client
 * @param reason Reason for disconnection
 */
static void
mqtt_close(mqtt_client_t *client, mqtt_connection_status_t reason)
{
  printf("PBB 656 mqtt_close %d\n",reason);
	if(client==NULL){
		LWIP_DEBUGF(MQTT_DEBUG,("mqtt_close: client == NULL cannot close"));
		return;
	}


  /* Bring down TCP connection if not already done */
  if (client->conn != NULL) {
    err_t res;
    tcp_recv(client->conn, NULL);
    tcp_err(client->conn,  NULL);
    tcp_sent(client->conn, NULL);
    if(gUseTLS){
      res = tls_close(client); /*! \todo this should be done in a better way! */
    }else{//if(gUseTLS)
      res = tcp_close(client->conn);
    }//else if(gUseTLS)
    if (res != ERR_OK) {
      tcp_abort(client->conn);
      LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_close: Close err=%s\n", lwip_strerr(res)));
    }
    client->conn = NULL;
  }

  /* Remove all pending requests */
  mqtt_clear_requests(&client->pend_req_queue);
  /* Stop cyclic timer */
  sys_untimeout(mqtt_cyclic_timer, client);

  /* Notify upper layer of disconnection if changed state */
  if (client->conn_state != TCP_DISCONNECTED) {
    client->conn_state = TCP_DISCONNECTED;
    if (client->connect_cb != NULL) {
      client->connect_cb(client, client->connect_arg, reason);
    }
  }
}


/**
 * Interval timer, called every MQTT_CYCLIC_TIMER_INTERVAL seconds in MQTT_CONNECTING and MQTT_CONNECTED states
 * @param arg MQTT client
 */
static void
mqtt_cyclic_timer(void *arg)
{
  u8_t restart_timer = 1;
  mqtt_client_t *client = (mqtt_client_t *)arg;
  if(client==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_cyclic_timer: client == NULL cannot cyclic timer"));
	  return;
  }

  if (client->conn_state == MQTT_CONNECTING) {
    client->cyclic_tick++;
    if ((client->cyclic_tick * MQTT_CYCLIC_TIMER_INTERVAL) >= MQTT_CONNECT_TIMOUT) {
      LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_cyclic_timer: CONNECT attempt to server timed out\n"));
      /* Disconnect TCP */
      mqtt_close(client, MQTT_CONNECT_TIMEOUT);
      restart_timer = 0;
    }
  } else if (client->conn_state == MQTT_CONNECTED) {
    /* Handle timeout for pending requests */
    mqtt_request_time_elapsed(&client->pend_req_queue, MQTT_CYCLIC_TIMER_INTERVAL);

    /* keep_alive > 0 means keep alive functionality shall be used */
    if (client->keep_alive > 0) {
      client->server_watchdog++;
      /* If reception from server has been idle for 1.5*keep_alive time, server is considered unresponsive */
      if ((client->server_watchdog * MQTT_CYCLIC_TIMER_INTERVAL) > (client->keep_alive + client->keep_alive/2)) {
        LWIP_DEBUGF(MQTT_DEBUG_WARN,("mqtt_cyclic_timer: Server incoming keep-alive timeout\n"));
        mqtt_close(client, MQTT_CONNECT_TIMEOUT);
        restart_timer = 0;
      }

      /* If time for a keep alive message to be sent, transmission has been idle for keep_alive time */
      if ((client->cyclic_tick * MQTT_CYCLIC_TIMER_INTERVAL) >= client->keep_alive) {
        LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_cyclic_timer: Sending keep-alive message to server\n"));
        if (mqtt_output_check_space(&client->output, 0) != 0) {
          mqtt_output_append_fixed_header(&client->output, MQTT_MSG_TYPE_PINGREQ, 0, 0, 0, 0);
          client->cyclic_tick = 0;
        }
      } else {
        client->cyclic_tick++;
      }
    }
#if MQTT_USE_TLS
  } else if (client->conn_state == TLS_HANDSHAKING) {
    /* nothing special to do while doing the TLS handshaking */
#endif
  } else {
    LWIP_DEBUGF(MQTT_DEBUG_WARN,("mqtt_cyclic_timer: Timer should not be running in state %d\n", client->conn_state));
    restart_timer = 0;
  }
  if (restart_timer) {
    sys_timeout(MQTT_CYCLIC_TIMER_INTERVAL*1000, mqtt_cyclic_timer, arg);
  }
}


/**
 * Send PUBACK, PUBREC or PUBREL response message
 * @param client MQTT client
 * @param msg PUBACK, PUBREC or PUBREL
 * @param pkt_id Packet identifier
 * @param qos QoS value
 * @return ERR_OK if successful, ERR_MEM if out of memory
 */
static err_t
pub_ack_rec_rel_response(mqtt_client_t *client, u8_t msg, u16_t pkt_id, u8_t qos)
{
  err_t err = ERR_OK;
  if (mqtt_output_check_space(&client->output, 2)) {
    mqtt_output_append_fixed_header(&client->output, msg, 0, qos, 0, 2);
    mqtt_output_append_u16(&client->output, pkt_id);
    mqtt_output_send(client, &client->output, client->conn);
  } else {
    LWIP_DEBUGF(MQTT_DEBUG_TRACE,("pub_ack_rec_rel_response: OOM creating response: %s with pkt_id: %d\n",
                                  mqtt_msg_type_to_str(msg), pkt_id));
    err = ERR_MEM;
  }
  return err;
}

/**
 * Subscribe response from server
 * @param r Matching request
 * @param result Result code from server
 */
static void
mqtt_incomming_suback(struct mqtt_request_t *r, u8_t result)
{
  printf("PBB 789 suback %d\n",result);
  if (r->cb != NULL) {
    r->cb(r->arg, result < 3 ? ERR_OK : ERR_ABRT);
  }
}


/**
 * Complete MQTT message received or buffer full
 * @param client MQTT client
 * @param fixed_hdr_idx header index
 * @param length length received part
 * @param remaining_length Remaining length of complete message
 */
static mqtt_connection_status_t
  mqtt_message_received(mqtt_client_t *client, u8_t fixed_hdr_idx, u16_t length, u32_t remaining_length)
{
  mqtt_connection_status_t res = MQTT_CONNECT_ACCEPTED;

  u8_t *var_hdr_payload = client->rx_buffer + fixed_hdr_idx;

  /* Control packet type */
  u8_t pkt_type = MQTT_CTL_PACKET_TYPE(client->rx_buffer[0]);
  u16_t pkt_id = 0;
  printf("PBB 810 mqtt_message_received %d\n",pkt_type);
  if (pkt_type == MQTT_MSG_TYPE_CONNACK) {
    if (client->conn_state == MQTT_CONNECTING) {
      /* Get result code from CONNACK */
      res = (mqtt_connection_status_t)var_hdr_payload[1];
      LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_message_received: received CONNACK, Connect response code %d\n", res));
      if (res == MQTT_CONNECT_ACCEPTED) {
        /* Reset cyclic_tick when changing to connected state */
        client->cyclic_tick = 0;
        client->conn_state = MQTT_CONNECTED;
        /* Notify upper layer */
        if (client->connect_cb != 0) {
          client->connect_cb(client, client->connect_arg, res);
        }
      }
    } else {
      LWIP_DEBUGF(MQTT_DEBUG_WARN,("mqtt_message_received: Received CONNACK in connected state\n"));
    }
  } else if (pkt_type == MQTT_MSG_TYPE_PINGRESP) {
    LWIP_DEBUGF(MQTT_DEBUG_TRACE,( "mqtt_message_received: Received PINGRESP from server\n"));

  } else if (pkt_type == MQTT_MSG_TYPE_PUBLISH) {
    u16_t payload_offset = 0;
    u16_t payload_length = length;
    u8_t qos = MQTT_CTL_PACKET_QOS(client->rx_buffer[0]);

    if (client->msg_idx <= MQTT_VAR_HEADER_BUFFER_LEN) {
      /* Should have topic and pkt id*/
      uint8_t *topic;
      uint16_t after_topic;
      u8_t bkp;
      u16_t topic_len = var_hdr_payload[0];
      topic_len = (topic_len << 8) + (u16_t)(var_hdr_payload[1]);

      topic = var_hdr_payload + 2;
      after_topic = 2 + topic_len;

      //Check length is valid for the kind of QoS we have
      if ((after_topic + (qos? 2 : 0)) > length) {
        LWIP_DEBUGF(MQTT_DEBUG_WARN,("mqtt_message_received: Receive buffer can not fit topic + pkt_id\n"));
        goto out_disconnect;
      }

      /* id for QoS 1 and 2 */
      if (qos > 0) {
        client->inpub_pkt_id = ((u16_t)var_hdr_payload[after_topic] << 8) + (u16_t)var_hdr_payload[after_topic + 1];
        after_topic += 2;
      } else {
        client->inpub_pkt_id = 0;
      }
      /* Take backup of byte after topic */
      bkp = topic[topic_len];
      /* Zero terminate string */
      topic[topic_len] = 0;
      /* Payload data remaining in receive buffer */
      payload_length = length - after_topic;
      payload_offset = after_topic;

      LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_incomming_publish: Received message with QoS %d at topic: %s, payload length %d\n",
                                    qos, topic, remaining_length + payload_length));
      if (client->pub_cb != NULL) {
        client->pub_cb(client->inpub_arg, (const char *)topic, remaining_length + payload_length);
      }
      /* Restore byte after topic */
      topic[topic_len] = bkp;
    }
    if (payload_length > 0 || remaining_length == 0) {
      client->data_cb(client->inpub_arg, var_hdr_payload + payload_offset, payload_length, remaining_length == 0 ? MQTT_DATA_FLAG_LAST : 0);
      /* Reply if QoS > 0 */
      if (remaining_length == 0 && qos > 0) {
        /* Send PUBACK for QoS 1 or PUBREC for QoS 2 */
        u8_t resp_msg = (qos == 1) ? MQTT_MSG_TYPE_PUBACK : MQTT_MSG_TYPE_PUBREC;
        LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_incomming_publish: Sending publish response: %s with pkt_id: %d\n",
                                      mqtt_msg_type_to_str(resp_msg), client->inpub_pkt_id));
        pub_ack_rec_rel_response(client, resp_msg, client->inpub_pkt_id, 0);
      }
    }
  } else {
    /* Get packet identifier */
    pkt_id = (u16_t)var_hdr_payload[0] << 8;
    pkt_id |= (u16_t)var_hdr_payload[1];
    if (pkt_id == 0) {
      LWIP_DEBUGF(MQTT_DEBUG_WARN,("mqtt_message_received: Got message with illegal packet identifier: 0\n"));
      goto out_disconnect;
    }
    if (pkt_type == MQTT_MSG_TYPE_PUBREC) {
      LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_message_received: PUBREC, sending PUBREL with pkt_id: %d\n",pkt_id));
      pub_ack_rec_rel_response(client, MQTT_MSG_TYPE_PUBREL, pkt_id, 1);

    } else if (pkt_type == MQTT_MSG_TYPE_PUBREL) {
      LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_message_received: PUBREL, sending PUBCOMP response with pkt_id: %d\n",pkt_id));
      pub_ack_rec_rel_response(client, MQTT_MSG_TYPE_PUBCOMP, pkt_id, 0);

    } else if (pkt_type == MQTT_MSG_TYPE_SUBACK || pkt_type == MQTT_MSG_TYPE_UNSUBACK ||
              pkt_type == MQTT_MSG_TYPE_PUBCOMP || pkt_type == MQTT_MSG_TYPE_PUBACK) {
      struct mqtt_request_t *r = mqtt_take_request(&client->pend_req_queue, pkt_id);
      if (r != NULL) {
        LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_message_received: %s response with id %d\n", mqtt_msg_type_to_str(pkt_type), pkt_id));
        if (pkt_type == MQTT_MSG_TYPE_SUBACK) {
          if (length < 3) {
            LWIP_DEBUGF(MQTT_DEBUG_WARN,("mqtt_message_received: To small SUBACK packet\n"));
            goto out_disconnect;
          } else {
            mqtt_incomming_suback(r, var_hdr_payload[2]);
          }
        } else if (r->cb != NULL) {
          r->cb(r->arg, ERR_OK);
        }
        mqtt_delete_request(r);
      } else {
        LWIP_DEBUGF(MQTT_DEBUG_WARN,( "mqtt_message_received: Received %s reply, with wrong pkt_id: %d\n", mqtt_msg_type_to_str(pkt_type), pkt_id));
      }
    } else {
      LWIP_DEBUGF(MQTT_DEBUG_WARN,( "mqtt_message_received: Received unknown message type: %d\n", pkt_type));
      goto out_disconnect;
    }
  }
  return res;
out_disconnect:
  printf("PBB 932 out_disconnect\n");
  return MQTT_CONNECT_DISCONNECTED;
}


/**
 * MQTT incoming message parser
 * @param client MQTT client
 * @param p PBUF chain of received data
 * @return Connection status
 */
static mqtt_connection_status_t
mqtt_parse_incoming(mqtt_client_t *client, struct pbuf *p)
{
  u16_t in_offset = 0;
  u32_t msg_rem_len = 0;
  u8_t fixed_hdr_idx = 0;
  u8_t b = 0;

  while (p->tot_len > in_offset) {
    if ((fixed_hdr_idx < 2) || ((b & 0x80) != 0)) {

      if (fixed_hdr_idx < client->msg_idx) {
        b = client->rx_buffer[fixed_hdr_idx];
      } else {
        b = pbuf_get_at(p, in_offset++);
        client->rx_buffer[client->msg_idx++] = b;
      }
      fixed_hdr_idx++;

      if (fixed_hdr_idx >= 2) {
        msg_rem_len |= (u32_t)(b & 0x7f) << ((fixed_hdr_idx - 2) * 7);
        if ((b & 0x80) == 0) {
          LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_parse_incoming: Remaining length after fixed header: %d\n", msg_rem_len));
          if (msg_rem_len == 0) {
            /* Complete message with no extra headers of payload received */
            mqtt_message_received(client, fixed_hdr_idx, 0, 0);
            client->msg_idx = 0;
            fixed_hdr_idx = 0;
          } else {
            /* Bytes remaining in message */
            msg_rem_len = (msg_rem_len + fixed_hdr_idx) - client->msg_idx;
          }
        }
      }
    } else {
      u16_t cpy_len, cpy_start, buffer_space;

      cpy_start = (client->msg_idx - fixed_hdr_idx) % (MQTT_VAR_HEADER_BUFFER_LEN - fixed_hdr_idx) + fixed_hdr_idx;

      /* Allow to copy the lesser one of available length in input data or bytes remaining in message */
      cpy_len = (u16_t)LWIP_MIN((u16_t)(p->tot_len - in_offset), msg_rem_len);

      /* Limit to available space in buffer */
      buffer_space = MQTT_VAR_HEADER_BUFFER_LEN - cpy_start;
      if (cpy_len > buffer_space) {
        cpy_len = buffer_space;
      }
      pbuf_copy_partial(p, client->rx_buffer+cpy_start, cpy_len, in_offset);

      /* Advance get and put indexes  */
      client->msg_idx += cpy_len;
      in_offset += cpy_len;
      msg_rem_len -= cpy_len;

      LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_parse_incoming: msg_idx: %d, cpy_len: %d, remaining %d\n", client->msg_idx, cpy_len, msg_rem_len));
      if (msg_rem_len == 0 || cpy_len == buffer_space) {
        /* Whole message received or buffer is full */
        mqtt_connection_status_t res = mqtt_message_received(client, fixed_hdr_idx, (cpy_start + cpy_len) - fixed_hdr_idx, msg_rem_len);
        if (res != MQTT_CONNECT_ACCEPTED) {
          return res;
        }
        if (msg_rem_len == 0) {
          /* Reset parser state */
          client->msg_idx = 0;
          /* msg_tot_len = 0; */
          fixed_hdr_idx = 0;
        }
      }
    }
  }
  return MQTT_CONNECT_ACCEPTED;
}

/**
 * TCP received callback function. @see tcp_recv_fn
 * @param arg MQTT client
 * @param p PBUF chain of received data
 * @param err Passed as return value if not ERR_OK
 * @return ERR_OK or err passed into callback
 */
static err_t
mqtt_tcp_recv_cb(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  mqtt_client_t *client = (mqtt_client_t *)arg;
  LWIP_ASSERT("mqtt_tcp_recv_cb: client != NULL", client != NULL);
  LWIP_ASSERT("mqtt_tcp_recv_cb: client->conn == pcb", client->conn == pcb);

  if (p == NULL) {
    printf("PBB 1031 mqtt_tcp_recv_cb: Recv pbuf=NULL, remote has closed connection\n");
    mqtt_close(client, MQTT_CONNECT_DISCONNECTED);
  } else {
    mqtt_connection_status_t res;
    if (err != ERR_OK) {
      LWIP_DEBUGF(MQTT_DEBUG_WARN,("mqtt_tcp_recv_cb: Recv err=%d\n", err));
      pbuf_free(p);
      return err;
    }

    /* Tell remote that data has been received */
    tcp_recved(pcb, p->tot_len);
    res = mqtt_parse_incoming(client, p);
    pbuf_free(p);

    if (res != MQTT_CONNECT_ACCEPTED) {
      mqtt_close(client, res);
    }
    /* If keep alive functionality is used */
    if (client->keep_alive != 0) {
      /* Reset server alive watchdog */
      client->server_watchdog = 0;
    }

  }
  return ERR_OK;
}

err_t mqtt_recv_from_tls(mqtt_client_t *client) {
  int nof;
  mqtt_connection_status_t status;
  struct pbuf p;

  /*! \todo check if can we really use rx_buffer here? */
  nof = mbedtls_ssl_read(client->ssl_context, client->rx_buffer, sizeof(client->rx_buffer));
  if (nof>0) {
    memset(&p, 0, sizeof(struct pbuf)); /* initialize */
    p.len = nof;
    p.tot_len = p.len;
    p.payload = client->rx_buffer;
    status = mqtt_parse_incoming(client, &p);
    if (status!=MQTT_CONNECT_ACCEPTED) {
      return ERR_CONN; /* connection error */ /*! \todo In case of communication error, have to close connection! */
    }
  }else if(nof<0){
	  return ERR_CONN;
  }
  return ERR_OK;
}

/**
 * TCP data sent callback function. @see tcp_sent_fn
 * @param arg MQTT client
 * @param tpcb TCP connection handle
 * @param len Number of bytes sent
 * @return ERR_OK
 */
static err_t
mqtt_tcp_sent_cb(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  mqtt_client_t *client = (mqtt_client_t *)arg;

  LWIP_UNUSED_ARG(tpcb);
  LWIP_UNUSED_ARG(len);

  if (client->conn_state == MQTT_CONNECTED) {
    struct mqtt_request_t *r;

    /* Reset keep-alive send timer and server watchdog */
    client->cyclic_tick = 0;
    client->server_watchdog = 0;
    /* QoS 0 publish has no response from server, so call its callbacks here */
    while ((r = mqtt_take_request(&client->pend_req_queue, 0)) != NULL) {
      LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_tcp_sent_cb: Calling QoS 0 publish complete callback\n"));
      if (r->cb != NULL) {
        r->cb(r->arg, ERR_OK);
      }
      mqtt_delete_request(r);
    }
    /* Try send any remaining buffers from output queue */
    mqtt_output_send(client, &client->output, client->conn);
  }
  return ERR_OK;
}

#if MQTT_USE_TLS
/**
 * TCP received callback function. @see tcp_recv_fn
 * @param arg MQTT client
 * @param p PBUF chain of received data
 * @param err Passed as return value if not ERR_OK
 * @return ERR_OK or err passed into callback
 */
static err_t tls_tcp_recv_cb(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  mqtt_client_t *client = (mqtt_client_t *)arg;
  if(client==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("tls_tcp_recv_cb: client == NULL cannot recv"));
	  return err;
  }

  if(client->conn!=pcb){
	  LWIP_DEBUGF(MQTT_DEBUG,("tls_tcp_recv_cb: client->conn != pcb cannot recv"));
	  return err;
  }

  if (p == NULL) {
    printf("PBB 1140 tls_tcp_recv_cb: Recv pbuf=NULL, remote has closed connection\n");
    mqtt_close(client, MQTT_CONNECT_DISCONNECTED);
  } else {
    mqtt_connection_status_t res;
    if (err != ERR_OK) {
      LWIP_DEBUGF(MQTT_DEBUG_WARN,("tls_tcp_recv_cb: Recv err=%d\n", err));
      pbuf_free(p);
      return err;
    }

    /* Tell remote that data has been received */
    tcp_recved(pcb, p->tot_len);
    res = mbedtls_net_incoming(client, p->payload, p->len);
    pbuf_free(p);

    if (res != 0) {
      mqtt_close(client, res);
    }
    /* If keep alive functionality is used */
    if (client->keep_alive != 0) {
      /* Reset server alive watchdog */
      client->server_watchdog = 0;
    }
  }
  return ERR_OK;
}
#endif

#if MQTT_USE_TLS
err_t mqtt_tls_output_send(mqtt_client_t *client) {
  /* try to send, if there is anything to send */
  mqtt_output_send(client, &client->output, client->conn);
  return ERR_OK;
}
#endif


#if MQTT_USE_TLS
/**
 * TCP data sent callback function. @see tcp_sent_fn
 * @param arg MQTT client
 * @param tpcb TCP connection handle
 * @param len Number of bytes sent
 * @return ERR_OK
 */
static err_t tls_tcp_sent_cb(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  LWIP_DEBUGF(MQTT_DEBUG_TRACE,("tls_tcp_sent_cb\r\n"));
  return mqtt_tcp_sent_cb(arg, tpcb, 0); /* call normal (non-tls) callback */
}
#endif /*MQTT_USE_TLS */

/**
 * TCP error callback function. @see tcp_err_fn
 * @param arg MQTT client
 * @param err Error encountered
 */
static void
mqtt_tcp_err_cb(void *arg, err_t err)
{
  mqtt_client_t *client = (mqtt_client_t *)arg;
  LWIP_UNUSED_ARG(err); /* only used for debug output */
  LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_tcp_err_cb: TCP error callback: error %d, arg: %p\n", err, arg));
  if(client==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_tcp_err_cb: client == NULL cannot tcp"));
	  return;
  }

  /* Set conn to null before calling close as pcb is already deallocated*/
  client->conn = 0;
  mqtt_close(client, MQTT_CONNECT_DISCONNECTED);
}

/**
 * TCP poll callback function. @see tcp_poll_fn
 * @param arg MQTT client
 * @param tpcb TCP connection handle
 * @return err ERR_OK
 */
static err_t
mqtt_tcp_poll_cb(void *arg, struct tcp_pcb *tpcb)
{
  mqtt_client_t *client = (mqtt_client_t *)arg;
  if (client->conn_state == MQTT_CONNECTED) {
    /* Try send any remaining buffers from output queue */
    mqtt_output_send(client, &client->output, tpcb);
  }
  return ERR_OK;
}

/**
 * TCP connect callback function. @see tcp_connected_fn
 * @param arg MQTT client
 * @param err Always ERR_OK, mqtt_tcp_err_cb is called in case of error
 * @return ERR_OK
 */
static err_t
mqtt_tcp_connect_cb(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  mqtt_client_t* client = (mqtt_client_t *)arg;
  printf("PBB 1236 mqtt_tcp_connect_cb %d\n",err);
  if (err != ERR_OK) {
    LWIP_DEBUGF(MQTT_DEBUG_WARN,("mqtt_tcp_connect_cb: TCP connect error %d\n", err));
    return err;
  }

  /* Initiate receiver state */
  client->msg_idx = 0;

 if(gUseTLS){
  /* Setup TCP callbacks */
  tcp_recv(tpcb, tls_tcp_recv_cb);
  tcp_sent(tpcb, tls_tcp_sent_cb);
  tcp_poll(tpcb, NULL, 0);

  LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_tcp_connect_cb: TCP connection established to server, starting TLS handshake\n"));
  /* Enter MQTT connect state */
  client->conn_state = TLS_HANDSHAKING;

  /* Start cyclic timer */
  sys_timeout(MQTT_CYCLIC_TIMER_INTERVAL*1000, mqtt_cyclic_timer, client);
  client->cyclic_tick = 0;
  printf("PBB 1258\n");
 }else{ //if(gUseTLS)
    /* Setup TCP callbacks */
    tcp_recv(tpcb, mqtt_tcp_recv_cb);
    tcp_sent(tpcb, mqtt_tcp_sent_cb);
    tcp_poll(tpcb, mqtt_tcp_poll_cb, 2);

    LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_tcp_connect_cb: TCP connection established to server\n"));
    /* Enter MQTT connect state */
    client->conn_state = MQTT_CONNECTING;

    /* Start cyclic timer */
    sys_timeout(MQTT_CYCLIC_TIMER_INTERVAL*1000, mqtt_cyclic_timer, client);
    client->cyclic_tick = 0;

    /* Start transmission from output queue, connect message is the first one out*/
    mqtt_output_send(client, &client->output, client->conn);
  }//else if(gUseTLS)
  return ERR_OK;
}



/*---------------------------------------------------------------------------------------------------- */
/* Public API */

#if MQTT_USE_TLS
err_t mqtt_start_mqtt(mqtt_client_t* client) {
  LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_tcp_connect_cb: TCP connection established to server\n"));
  /* Enter MQTT connect state */
  client->conn_state = MQTT_CONNECTING; /* start MQTT connection sequence */

  /* Start transmission from output queue, connect message is the first one out*/
  mqtt_output_send(client, &client->output, client->conn);
  return ERR_OK;
}
#endif /* MQTT_USE_TLS */


/**
 * @ingroup mqtt
 * MQTT publish function.
 * @param client MQTT client
 * @param topic Publish topic string
 * @param payload Data to publish (NULL is allowed)
 * @param payload_length: Length of payload (0 is allowed)
 * @param qos Quality of service, 0 1 or 2
 * @param retain MQTT retain flag
 * @param cb Callback to call when publish is complete or has timed out
 * @param arg User supplied argument to publish callback
 * @return ERR_OK if successful
 *         ERR_CONN if client is disconnected
 *         ERR_MEM if short on memory
 */
err_t
mqtt_publish(mqtt_client_t *client, const char *topic, const void *payload, u16_t payload_length, u8_t qos, u8_t retain,
             mqtt_request_cb_t cb, void *arg)
{
  struct mqtt_request_t *r;
  u16_t pkt_id;
  size_t topic_strlen;
  size_t total_len;
  u16_t topic_len;
  u16_t remaining_length;

  if(client==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_publish: client == NULL cannot publish"));
	  return ERR_MEM;
  }

  if(topic==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_publish: topic == NULL cannot publish"));
	  return ERR_MEM;
  }

  LWIP_ERROR("mqtt_publish: TCP disconnected", (client->conn_state != TCP_DISCONNECTED), return ERR_CONN);

  topic_strlen = strlen(topic);
  LWIP_ERROR("mqtt_publish: topic length overflow", (topic_strlen <= (0xFFFF - 2)), return ERR_ARG);
  topic_len = (u16_t)topic_strlen;
  total_len = 2 + topic_len + payload_length;
  LWIP_ERROR("mqtt_publish: total length overflow", (total_len <= 0xFFFF), return ERR_ARG);
  remaining_length = (u16_t)total_len;

  LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_publish: Publish with payload length %d to topic \"%s\"\n", payload_length, topic));

  if (qos > 0) {
    remaining_length += 2;
    /* Generate pkt_id id for QoS1 and 2 */
    pkt_id = msg_generate_packet_id(client);
  } else {
    /* Use reserved value pkt_id 0 for QoS 0 in request handle */
    pkt_id = 0;
  }

  r = mqtt_create_request(client->req_list, pkt_id, cb, arg);
  if (r == NULL) {
    return ERR_MEM;
  }

  if (mqtt_output_check_space(&client->output, remaining_length) == 0) {
    mqtt_delete_request(r);
    return ERR_MEM;
  }
  /* Append fixed header */
  mqtt_output_append_fixed_header(&client->output, MQTT_MSG_TYPE_PUBLISH, 0, qos, retain, remaining_length);

  /* Append Topic */
  mqtt_output_append_string(&client->output, topic, topic_len);

  /* Append packet if for QoS 1 and 2*/
  if (qos > 0) {
    mqtt_output_append_u16(&client->output, pkt_id);
  }

  /* Append optional publish payload */
  if ((payload != NULL) && (payload_length > 0)) {
    mqtt_output_append_buf(&client->output, payload, payload_length);
  }

  mqtt_append_request(&client->pend_req_queue, r);
  mqtt_output_send(client, &client->output, client->conn);
  return ERR_OK;
}


/**
 * @ingroup mqtt
 * MQTT subscribe/unsubscribe function.
 * @param client MQTT client
 * @param topic topic to subscribe to
 * @param qos Quality of service, 0 1 or 2 (only used for subscribe)
 * @param cb Callback to call when subscribe/unsubscribe reponse is received
 * @param arg User supplied argument to publish callback
 * @param sub 1 for subscribe, 0 for unsubscribe
 * @return ERR_OK if successful, @see err_t enum for other results
 */
err_t
mqtt_sub_unsub(mqtt_client_t *client, const char *topic, u8_t qos, mqtt_request_cb_t cb, void *arg, u8_t sub)
{
  size_t topic_strlen;
  size_t total_len;
  u16_t topic_len;
  u16_t remaining_length;
  u16_t pkt_id;
  struct mqtt_request_t *r;

  if(client==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_sub_unsub: client == NULL cannot unsub"));
	  return ERR_ARG;
  }

  if(topic==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_sub_unsub: topic == NULL"));
	  return ERR_ARG;
  }


  topic_strlen = strlen(topic);
  LWIP_ERROR("mqtt_sub_unsub: topic length overflow", (topic_strlen <= (0xFFFF - 2)), return ERR_ARG);
  topic_len = (u16_t)topic_strlen;
  /* Topic string, pkt_id, qos for subscribe */
  total_len =  topic_len + 2 + 2 + (sub != 0);
  LWIP_ERROR("mqtt_sub_unsub: total length overflow", (total_len <= 0xFFFF), return ERR_ARG);
  remaining_length = (u16_t)total_len;

  if(qos > 2){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_sub_unsub: qos > 2"));
	  return ERR_ARG;
  }

  if (client->conn_state == TCP_DISCONNECTED) {
    LWIP_DEBUGF(MQTT_DEBUG_WARN,("mqtt_sub_unsub: Can not (un)subscribe in disconnected state\n"));
    return ERR_CONN;
  }

  pkt_id = msg_generate_packet_id(client);
  r = mqtt_create_request(client->req_list, pkt_id, cb, arg);
  if (r == NULL) {
    return ERR_MEM;
  }

  if (mqtt_output_check_space(&client->output, remaining_length) == 0) {
    mqtt_delete_request(r);
    return ERR_MEM;
  }

  LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_sub_unsub: Client (un)subscribe to topic \"%s\", id: %d\n", topic, pkt_id));

  mqtt_output_append_fixed_header(&client->output, sub ? MQTT_MSG_TYPE_SUBSCRIBE : MQTT_MSG_TYPE_UNSUBSCRIBE, 0, 1, 0, remaining_length);
  /* Packet id */
  mqtt_output_append_u16(&client->output, pkt_id);
  /* Topic */
  mqtt_output_append_string(&client->output, topic, topic_len);
  /* QoS */
  if (sub != 0) {
    mqtt_output_append_u8(&client->output, LWIP_MIN(qos, 2));
  }

  mqtt_append_request(&client->pend_req_queue, r);
  mqtt_output_send(client, &client->output, client->conn);
  return ERR_OK;
}

/**
 * @ingroup mqtt
 * Create a new MQTT client instance
 * @return Pointer to instance on success, NULL otherwise
 */
/*mqtt_client_t *
mqtt_client_new(void)
{
  mqtt_client_t *client = (mqtt_client_t *)mem_malloc(sizeof(mqtt_client_t));
  if (client != NULL) {
    memset(client, 0, sizeof(mqtt_client_t));
  }
  return client;
}*/

/**
 * @ingroup mqtt
 * Set callback to handle incoming publish requests from server
 * @param client MQTT client
 * @param pub_cb Callback invoked when publish starts, contain topic and total length of payload
 * @param data_cb Callback for each fragment of payload that arrives
 * @param arg User supplied argument to both callbacks
 */
void
mqtt_set_inpub_callback(mqtt_client_t *client, mqtt_incoming_publish_cb_t pub_cb,
                             mqtt_incoming_data_cb_t data_cb, void *arg)
{
	if(client==NULL){
		LWIP_DEBUGF(MQTT_DEBUG,("mqtt_set_inpub_callback: client == NULL"));
		return;
	}

  client->data_cb = data_cb;
  client->pub_cb = pub_cb;
  client->inpub_arg = arg;
}

extern bool gScheduleApplyNew;
extern u8_t gScheduleBuffer[CONFIG_MAX_SCHEDULE_SIZE_BYTES];

static void inxMqttProcessSODLChunkV1(const u16_t len,const u8_t* data){
  //this is a chunk of firmware, first 4 bytes are the start byte, fifth byte is the length
  if(len<6){
    LWIP_DEBUGF(MQTT_DEBUG,("missing header"));
    return;
  }

  uint32_t* pStartByte=(uint32_t*)&data[0];
  uint32_t startByte=*pStartByte;
  uint16_t* pPublishedDataLength=(uint16_t*)&data[4];
  uint16_t publishedDataLength=*pPublishedDataLength;
  uint16_t calculatedDataLength=len-6;
  LWIP_DEBUGF(MQTT_DEBUG,("startByte=%d dataLength=%d calculatedDataLength=%d",startByte,publishedDataLength,calculatedDataLength));
  if(publishedDataLength!=calculatedDataLength){
    LWIP_DEBUGF(MQTT_DEBUG,("dataLength != calculatedDataLength"));
    return;
  }

  //is this for our current sector?
  uint32_t flashSectorSize=mainGetFlashSectorSize();
  uint32_t sector=startByte/flashSectorSize;
  if(sector!=gFirmwareCurrentSector){
    LWIP_DEBUGF(MQTT_DEBUG,("Binning because not our sector %d != %d",sector,gFirmwareCurrentSector));
    return;
  }

  size_t cpySize=publishedDataLength;
  uint32_t sectorByte=startByte % flashSectorSize;

  if(publishedDataLength==0){
    //end of firmware
    gFirmwareOTAStarted=INX_FIRMWARE_UPDATE_NONE;
    //TODO trigger EHS_RELOAD_EHS_FROM_FILE
  }else{
    //we need to copy this in to our sector
    LWIP_DEBUGF(MQTT_DEBUG,("memcpy sectorByte=%d cpySize=%d",sectorByte,cpySize));
    memcpy(&gOTAUpdateOneSectorBuffer[sectorByte],&data[6],cpySize);
    //uint32_t totalBytesFetched=gFirmwareCurrentByte+publishedDataLength;
    //LWIP_DEBUGF(MQTT_DEBUG,("startByte=%d, totalBytesFetched=%d",startByte,totalBytesFetched));
    uint32_t endByte=sectorByte+publishedDataLength;
    if(endByte >= flashSectorSize){
      //we have finished a sector
      //inxWriteSODLBuffer();
      //go up to the next sector
      gFirmwareCurrentSector=sector+1;
      gFirmwareCurrentByte=0;
    }else{
      //go up to the next byte
      gFirmwareCurrentByte=endByte;
    }
    //tell mqtt we are ready for a new chunk
    gFirmwareWaitingForChunkTicks=0;
  }
}

static char gInxIncomingTopic[CONFIG_MQTT_MAX_TOPIC_LENGTH];
bool EhsMQTTSubscribeWritePoll(char* buffer,bool* subscribe,u8_t* qos);
bool EhsMQTTSubscribeEvent(char* topic,char* event);
extern uint8_t inxSODLFlashSectorIndex;
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
  LWIP_DEBUGF(MQTT_DEBUG,("Incoming publish payload with length %d, flags %u", len, (unsigned int)flags));
    char tempString[INX_MQTT_MAX_PAYLOAD_SIZE+1];
    memcpy(tempString,data,len);
    tempString[len]='\0';
    EhsMQTTSubscribeEvent(gInxIncomingTopic,tempString);
  if(flags & MQTT_DATA_FLAG_LAST) {
	  uint32_t result;
    /* Last fragment of payload received (or whole part if payload fits receive buffer
       See MQTT_VAR_HEADER_BUFFER_LEN)  */
	  SW_DIAG_printf("\nMQTT data cb [%s] \n",(char*)data);
    /* Call function or do action depending on reference, in this case inpub_id */
    if(gMqttInpubId == 0) { /* received message for topic CONFIG_TOPIC_NAME */
      //was inx/time but we ignore those now
    } else if(gMqttInpubId == 1) {
    	//this is a settings update from devman
    	//treat it as http post data
      if(len<=INX_MQTT_MAX_PAYLOAD_SIZE){
        //turn buffer in to a string
        memcpy(tempString,data,len);
        tempString[len]='\0';
        inxUpdateSettings(tempString);
      }
    } else if(gMqttInpubId==2){
    	//we have been asked to generate a report
      if(len<=INX_MQTT_MAX_PAYLOAD_SIZE){
        //turn buffer in to a string
        memcpy(tempString,data,len);
        tempString[len]='\0';
        if(strcmp(tempString,"generateCommissioning")==0){
          //we need to set some flags that will cause a commissioning report to be done
          gGenerateCommissioningReport=true;
        }else if(strcmp(tempString,"generateDiagnostic")==0){
          gGenerateDiagnosticReport=true;
        }
      }
    }else if(gMqttInpubId==3){
      #if CONFIG_FIRMWARE_IGNORE_UPDATES
      #else
        //we've been asked to start an ota firmware update
        if(len<CONFIG_FIRMWARE_NAME_MAX_LENGTH){
          strncpy(tempString,(char*)data,len);
          tempString[len]='\0';
          if(strstr(tempString,"sodl")){
            gFirmwareByteWeAreWaitingFor=0;
            gFirmwareCurrentSector=0;
            gFirmwareCurrentByte=0;
            inxSODLFlashSectorIndex=0;
            gFirmwareOTAStarted=INX_FIRMWARE_UPDATE_NONE;
            gFirmwareStartOTA=INX_FIRMWARE_UPDATE_SODL;
          }else{
            uint32_t tempFirmwareCRC=0;
            result=sscanf(tempString,"crc=%d,version=%"CONFIG_FIRMWARE_NAME_MAX_LENGTH_STRING"s",&tempFirmwareCRC,tempString);
            if(result==2){
              //are we already downloading this firmware?
              result=strcmp(tempString,gFirmwareVersion);
              if(tempFirmwareCRC==gFirmwareCrc && result==0){
                //same os so ignore this
              }else{
                gFirmwareCrc=tempFirmwareCRC;
                strncpy(gFirmwareVersion,tempString,sizeof(gFirmwareVersion));
                //make sure our firmware buffer is clear
                memset(gOTAUpdateOneSectorBuffer,0,mainGetFlashSectorSize()+1);
                gFirmwareByteWeAreWaitingFor=0;
                gFirmwareCurrentSector=0;
                gFirmwareCurrentByte=0;
                //signal that we should start requesting firmware chunks
                gFirmwareOTAStarted=INX_FIRMWARE_UPDATE_NONE;
                gFirmwareStartOTA=INX_FIRMWARE_UPDATE_FIRMWARE;
              }
            }else{
              LWIP_DEBUGF(MQTT_DEBUG,("failed to read firmware version from %s",(char*)data));
            }
          }
        }else{
          LWIP_DEBUGF(MQTT_DEBUG,("message length too short"));
        }
      #endif //CONFIG_FIRMWARE_IGNORE_UPDATES
    }else if(gMqttInpubId==4){
    	//this is a chunk of firmware, first 4 bytes are the start byte, fifth byte is the length
    	if(len<6){
    		LWIP_DEBUGF(MQTT_DEBUG,("missing header"));
    		return;
    	}

    	uint32_t* pStartByte=(uint32_t*)&data[0];
		  uint32_t startByte=*pStartByte;
		  uint16_t* pPublishedDataLength=(uint16_t*)&data[4];
    	uint16_t publishedDataLength=*pPublishedDataLength;
    	uint16_t calculatedDataLength=len-6;
    	LWIP_DEBUGF(MQTT_DEBUG,("startByte=%d dataLength=%d calculatedDataLength=%d",startByte,publishedDataLength,calculatedDataLength));
    	if(publishedDataLength!=calculatedDataLength){
    		LWIP_DEBUGF(MQTT_DEBUG,("dataLength != calculatedDataLength"));
    		return;
    	}

    	//is this for our current sector?
    	uint32_t flashSectorSize=mainGetFlashSectorSize();
    	uint32_t sector=startByte/flashSectorSize;
    	if(sector!=gFirmwareCurrentSector){
    		LWIP_DEBUGF(MQTT_DEBUG,("Binning because not our sector %d != %d",sector,gFirmwareCurrentSector));
    		return;
    	}

    	size_t cpySize=publishedDataLength;
    	uint32_t sectorByte=startByte % flashSectorSize;

    	if(publishedDataLength==0){
    		//end of firmware
    		gFirmwareOTAStarted=INX_FIRMWARE_UPDATE_NONE;
    		firmwareWriteSector(true,gFirmwareCurrentSector,gOTAUpdateOneSectorBuffer,startByte,gFirmwareCrc);
    	}else{
    		//we need to copy this in to our sector
    		LWIP_DEBUGF(MQTT_DEBUG,("memcpy sectorByte=%d cpySize=%d",sectorByte,cpySize));
    		memcpy(&gOTAUpdateOneSectorBuffer[sectorByte],&data[6],cpySize);
    		//uint32_t totalBytesFetched=gFirmwareCurrentByte+publishedDataLength;
    		//LWIP_DEBUGF(MQTT_DEBUG,("startByte=%d, totalBytesFetched=%d",startByte,totalBytesFetched));
    		uint32_t endByte=sectorByte+publishedDataLength;
        if(endByte >= flashSectorSize){
          //we have finished a sector
          firmwareWriteSector(false,gFirmwareCurrentSector,gOTAUpdateOneSectorBuffer,startByte,gFirmwareCrc);
          //go up to the next sector
          gFirmwareCurrentSector=sector+1;
          gFirmwareCurrentByte=0;
        }else{
          //go up to the next byte
          gFirmwareCurrentByte=endByte;
        }
        //tell mqtt we are ready for a new chunk
        gFirmwareWaitingForChunkTicks=0;
    	}
    }else if(gMqttInpubId==7){
    	uint8_t factory=0;
      memcpy(tempString,data,len);
      tempString[len]='\0';
      SW_DIAG_printf("%s\r\n",tempString);
    	result=sscanf(tempString,"factory=%d",&factory);
    	if(result==1 && factory==1){
    		mainFactoryReset();
    	}else{
    		mainReset();
    	}
    }else if(gMqttInpubId==8){
      memcpy(tempString,data,len);
      tempString[len]='\0';
    	// Receive organisation ID binded to this device.
    	mainUpdateOrganisationID(tempString);
    }else if(gMqttInpubId==9){
      //TODO handle new sodl chunk
      inxMqttProcessSODLChunkV1(len,data);
    }else {
      LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("mqtt_incoming_data_cb: Ignoring payload...\n"));
    }
  } else {
    /* Handle fragmented payload, store in buffer, write to file or whatever */
  }
}

/* The idea is to demultiplex topic and create some reference to be used in data callbacks
   Example here uses a global variable, better would be to use a member in arg
   If RAM and CPU budget allows it, the easiest implementation might be to just take a copy of
   the topic string and use it in mqtt_incoming_data_cb
*/
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
  /* always look at comnpiler warnings - double pointer erroneously declared here */
	char tempTopicName[CONFIG_MQTT_MAX_TOPIC_LENGTH];
  LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("Incoming publish at topic \"%s\" with total length %u\n", topic, (unsigned int)tot_len));
  gMqttInpubId=-1;
  /* Decode topic string into a user defined reference */
  strcpy(gInxIncomingTopic,topic);
  if(strcmp(topic, CONFIG_MQTT_TIME_CHANNEL) == 0) {
	  gMqttInpubId = 0;
  }else{
    snprintf(tempTopicName,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_FIRMWARECHUNK_POST,gMqttClientId);
    if(strcmp(topic,tempTopicName)==0){
      gMqttInpubId=4;
    }else{
      snprintf(tempTopicName,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_INFO_LOGS,gMqttClientId);
      if(strcmp(topic,tempTopicName)==0){
        gMqttInpubId=2;
      }else{
        snprintf(tempTopicName,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_FIRMWARE_STARTOTA,gMqttClientId);
        if(strcmp(topic,tempTopicName)==0){
          gMqttInpubId=3;
        }else{
          snprintf(tempTopicName,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_UPDATE_ORGANISATION,gMqttClientId);
          if(strcmp(topic,tempTopicName)==0){
            gMqttInpubId=8;
          }else{
            snprintf(tempTopicName,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_RESET,gMqttClientId);
            if(strcmp(topic,tempTopicName)==0){
              gMqttInpubId=7;
            }else{
              snprintf(tempTopicName,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_UPDATE_SETTINGS,gMqttClientId);
              if(strcmp(topic,tempTopicName)==0){
                gMqttInpubId = 1;
              }else{
                snprintf(tempTopicName,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_SODLCHUNK_POST,gMqttClientId);
                if(strcmp(topic,tempTopicName)==0){
                  gMqttInpubId = 9;
                }
              }
            }
          }
        }
      }
    }
  }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
  //err_t err;

  if(status == MQTT_CONNECT_ACCEPTED) {
    LWIP_DEBUGF(MQTT_DEBUG,("mqtt_connection_cb: Successfully connected"));

    /* Setup callback for incoming publish requests */
    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);
  } else {
    printf("PBB 1782 mqtt_connection_cb: Disconnected, reason: %d\n", status);
    MQTT_state = MQTT_STATE_DO_DISCONNECT;
  }
}

static mbedtls_entropy_context entropy;
static mbedtls_ctr_drbg_context ctr_drbg;
static mbedtls_ssl_context ssl;
static mbedtls_ssl_config conf;
static mbedtls_x509_crt cacert;
static mbedtls_x509_crt clinet_cert;
static mbedtls_pk_context pk_key;
static mbedtls_ctr_drbg_context ctr_drbg;

void MQTT_TlsClose(void) { /* called from mqtt.c */
	/*! \todo This should be in a separate module */
	mbedtls_x509_crt_free(&cacert); //not doing this now as we never reparse the cert to avoid fragmentation
	mbedtls_x509_crt_free(&clinet_cert);
	mbedtls_pk_free(&pk_key);
	mbedtls_ssl_free( &ssl );
	mbedtls_ssl_config_free( &conf );
	mbedtls_ctr_drbg_free( &ctr_drbg );
	mbedtls_entropy_free( &entropy );
	RNG1_Clear(); //PBB this is very important, this our decoded packet buffer, must be cleared or weird things happen
}

static int mqtt_do_connect(const char* username,const char* password,const uint8_t useTLS,const uint16_t port,mqtt_client_t *client,const ip4_addr_t *broker_ipaddr,const char *clientId) {
  struct mqtt_connect_client_info_t ci;
  err_t err;

  memset(client, 0, sizeof(mqtt_client_t)); /* initialize all fields */

  /* Setup an empty client info structure */
  memset(&ci, 0, sizeof(ci));
  /* Minimal amount of information required is client identifier, so set it here */
  ci.client_id = clientId;
  //ci.client_id="basicPubSub";
  ci.client_user = username;
  ci.client_pass = password;
  ci.keep_alive = 600; /* timeout */

  /* Initiate client and connect to server, if this fails immediately an error code is returned
     otherwise mqtt_connection_cb will be called with connection result after attempting
     to establish a connection with the server.
     For now MQTT version 3.1.1 is always used */
  #if MQTT_USE_TLS
    if(useTLS){
      client->ssl_context = &ssl;
    }
  #endif
  err = mqtt_client_connect(client, broker_ipaddr, port, mqtt_connection_cb, 0, &ci);
  /* For now just print the result code if something goes wrong */
  if(err != ERR_OK) {
    LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("mqtt_connect return %d\n", err));
    return -1; /* error */
  }
  return 0; /* ok */
}

static void mqtt_sub_request_cb(void *arg, err_t result) {
  /* Just print the result code here for simplicity,
     normal behaviour would be to take some action if subscribe fails like
     notifying user, retry subscribe or disconnect from server */
  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_sub_request_cb: result=%d", result));
}

/* Called when publish is complete either with success or failure */
static void mqtt_pub_request_cb(void *arg, err_t result) {
	LWIP_DEBUGF(MQTT_DEBUG,("mqtt_pub_request_cb: result=%d", result));
  if(result != ERR_OK) {
    printf("PBB do disconnect because mqtt publish callback failed %d\n",result);
    MQTT_state=MQTT_STATE_DO_DISCONNECT;
  }
}

static err_t my_mqtt_subscribe(mqtt_client_t *client,const char* topic, void *arg, mqtt_qos_t qos) {
  err_t err;
  //const char *topic = CONFIG_TOPIC_NAME;
  /* Subscribe to a topic named topic with QoS level 1, call mqtt_sub_request_cb with result */
  err = mqtt_subscribe(client, topic, qos, mqtt_sub_request_cb, arg);
  return err;
}

static void my_debug(void *ctx, int level, const char *file, int line, const char *str) {
  ((void)level);

  LWIP_DEBUGF(MQTT_DEBUG,("%s, at line %d in file %s", str, line, file));
}
/*
static void* inxCalloc(size_t num,size_t size){
	void* p=calloc(num,size);
	LWIP_PLATFORM_DIAG(("PBB calloc:0x%X %d %d",p,num,size));
	return p;
}

static void inxFree(void* p){
	if(p==NULL){
		LWIP_PLATFORM_DIAG(("PBB free:ignoring null"));
	}else{
		free(p);
		LWIP_PLATFORM_DIAG(("PBB free:0x%X",p));
	}
}
*/
static int TLS_Init(const char* host,const unsigned char* mbedtls_m2mqtt_client_crt,const unsigned char* mbedtls_m2mqtt_client_pk_key,const unsigned char* mbedtls_m2mqtt_srv_crt) {
  // inspired by https://tls.mbed.org/kb/how-to/mbedtls-tutorial
  int ret;
  const char *pers = "seed for entropy 12";
  //mbedtls_platform_set_calloc_free(&inxCalloc,&inxFree);
  MQTT_TlsClose();
  /* initialize the different descriptors */
  mbedtls_ssl_init( &ssl );
  mbedtls_ssl_config_init( &conf );
  mbedtls_ctr_drbg_init( &ctr_drbg );
  mbedtls_entropy_init( &entropy );
  if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                                 (const unsigned char *) pers,
                                 strlen(pers ) ) ) != 0 )
      {
          LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,( " failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret ));
          return -1;
      }

  	  /*
      * First prepare the SSL configuration by setting the endpoint and transport type, and loading reasonable
      * defaults for security parameters. The endpoint determines if the SSL/TLS layer will act as a server (MBEDTLS_SSL_IS_SERVER)
      * or a client (MBEDTLS_SSL_IS_CLIENT). The transport type determines if we are using TLS (MBEDTLS_SSL_TRANSPORT_STREAM)
      * or DTLS (MBEDTLS_SSL_TRANSPORT_DATAGRAM).
      */
     if( ( ret = mbedtls_ssl_config_defaults( &conf,
                     MBEDTLS_SSL_IS_CLIENT,
                     MBEDTLS_SSL_TRANSPORT_STREAM,
                     MBEDTLS_SSL_PRESET_DEFAULT ) ) != 0 )
     {
         LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,( " failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret ));
         return -1;
     }
     mbedtls_x509_crt_init( &clinet_cert );
     size_t len=strlen((const char*)mbedtls_m2mqtt_client_crt)+1; //we add 1 because mbedtls needs to know about the null terminator on the end of the buffer
      ret = mbedtls_x509_crt_parse(&clinet_cert, mbedtls_m2mqtt_client_crt, len );
      if(ret != 0) {
        LWIP_DEBUGF(MQTT_DEBUG,( "failed\n  !  mbedtls_x509_crt_parse returned (client) -0x%x", -ret ));
        return -1;
      }
      mbedtls_pk_init(&pk_key);
      len=strlen((const char*)mbedtls_m2mqtt_client_pk_key)+1;
      ret = mbedtls_pk_parse_key(&pk_key, mbedtls_m2mqtt_client_pk_key, len, NULL,0 );
      if(ret != 0) {
        LWIP_DEBUGF(MQTT_DEBUG,( "failed\n  !  mbedtls_pk_parse_key returned -0x%x", -ret ));
        return -1;
      }
      ret = mbedtls_ssl_conf_own_cert(&conf, &clinet_cert, &pk_key);
      if(ret != 0) {
        LWIP_DEBUGF(MQTT_DEBUG,( "failed\n  !  mbedtls_ssl_conf_own_cert returned -0x%x", -ret ));
        return -1;
      }
    
#if CONFIG_USE_SERVER_VERIFICATION
/* The authentication mode determines how strict the certificates that are presented are checked.  */
     mbedtls_x509_crt_init( &cacert );
     len=strlen((const char*)mbedtls_m2mqtt_srv_crt)+1;
      ret = mbedtls_x509_crt_parse(&cacert,mbedtls_m2mqtt_srv_crt, len );
      if(ret != 0) {
      LWIP_DEBUGF(MQTT_DEBUG,( " failed\n  !  mbedtls_x509_crt_parse returned -0x%x", -ret ));
      return -1;
      }
#endif //CONFIG_USE_SERVER_VERIFICATION

#if CONFIG_USE_SERVER_VERIFICATION
    mbedtls_ssl_conf_ca_chain( &conf, &cacert, NULL );
    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_REQUIRED);
  #else
    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_NONE);
  #endif

    /* The library needs to know which random engine to use and which debug function to use as callback. */
    mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );
    mbedtls_ssl_conf_dbg( &conf, my_debug, stdout );

    if(mbedtls_ssl_setup(&ssl, &conf)!=0){
  	  return -1;
    }

    if(host!=NULL && ( ret = mbedtls_ssl_set_hostname(&ssl, host) ) != 0 )
    {
        LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,( " failed\n  ! mbedtls_ssl_set_hostname returned %d\n\n", ret ));
        return -1;
    }
    /* the SSL context needs to know the input and output functions it needs to use for sending out network traffic. */
    mbedtls_ssl_set_bio(&ssl, &gMqttClient, mbedtls_net_send, mbedtls_net_recv, NULL);
#if CONFIG_MQTT_TEST_TLS_MEM_LEAK

#else


#endif //CONFIG_MQTT_TEST_TLS_MEM_LEAK

  return 0; /* no error */
}

/*!
 * @brief Callback function to generate TXT mDNS record for HTTP service.
 */
/*static void http_srv_txt(struct mdns_service *service, void *txt_userdata)
{
    mdns_resp_add_service_txtitem(service, "path=/", 6);
}*/

static int mqtt_do_tls_handshake(mqtt_client_t *mqtt_client) {
  int ret;

  if (!mqtt_client_is_handshaking(mqtt_client)) {
     return -1; /* still connecting on the TCP level */
  }

  ret = mbedtls_ssl_handshake(mqtt_client->ssl_context);
  if (ret < 0) {
    if (ret != MBEDTLS_ERR_SSL_WANT_READ &&
       ret != MBEDTLS_ERR_SSL_WANT_WRITE)
    {
    	if(ret==MBEDTLS_ERR_X509_CERT_VERIFY_FAILED){
    		MQTT_state=MQTT_STATE_DO_DISCONNECT;
    		LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("mqtt_do_tls_handshake: do disconnect certificate not valid"));
    	}/*else if(ret==MBEDTLS_ERR_SSL_CONN_EOF){
    		MQTT_state=MQTT_STATE_DO_DISCONNECT;
    		LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("mqtt_do_tls_handshake: do disconnect end of connection"));
    	}*/
      return -1;
    }
  }else if(ret>0){
	  return -1;
  }
  return 0;
}

static err_t my_mqtt_publish(mqtt_client_t *client, const char *topic, const char *payload,u8_t qos, void *arg) {
  err_t err;
  u8_t retain = 0; /* No don't retain such crappy payload... */
  printf("PBB 2020 %s %s\n",topic,payload);
  err = mqtt_publish(client, (const char*)topic, payload, strlen((char*)payload), qos, retain, mqtt_pub_request_cb, arg);
  if(err != ERR_OK) {
    LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("Publish err: %d\n", err));
  }
  //LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("Published to topic \"%s\", payload \"%s\", res: %d\r\n", topic, payload, (int)err));
  return err;
}

extern uint32_t gSettingsWriteId;
extern volatile bool gErrorLogsUpdated;
extern errorLogs_t gErrorLogs;
extern float gUserAlarmsAdcValues;
extern bool gSendIdentification;
extern bool gSendSettings;
extern inxRelayCurrentState_t gRelayErrorState;
extern inxRelayCurrentState_t gStartupRelayErrorState;
extern adcRawValues_t gADCRawValues;
extern volatile inxNoFactoryResetStorage_t gNoResetSettings;
/*static void buildStatePayload(inxRelayCurrentState_t* pState,char payload[512]){
  int written=snprintf(payload,INX_MQTT_MAX_PAYLOAD_SIZE,"{"
      "\"sw\":%d," //number of setting writes
      "\"roc\":[%d,%d,%d,%d,%d,%d]," //relay on counts
      "\"p1o\":%2.1f," //pt100channel1offset
      "\"p1s\":%2.1f," //pt100channel1scale
      "\"p2o\":%2.1f," //pt100channel2offset
      "\"p2s\":%2.1f," //pt100channel2scale
      "\"t1o\":%2.1f," //thermocoupleoffset
      "\"t1s\":%2.1f," //thermocouplescale
      "\"z1o\":%2.1f," //zeroTenVoffset
      "\"z1s\":%2.1f," //zeroTenVscale
      "\"f1o\":%2.1f," //fourTwentyMAoffset
      "\"f1s\":%2.1f," //fourTwentyMAscale
      "\"cpuTemp1\":%d,"
      "\"pt100Channel1\":%d,"
      "\"pt100Channel2\":%d,"
      "\"thermocouple1\":%d,"
      "\"fourTwentyMa\":%d,"
      "\"zeroTenV\":%d,"
      "\"rtc\":%d,"
      "\"artc\":%d,"
		  "\"e1c\":%d,"
		  "\"e1m\":%d,"
		  "\"e2c\":%d,"
		  "\"e2m\":%d,"
		  "\"e3c\":%d,"
		  "\"e3m\":%d,"
		  "\"t1c\":%d,"
		  "\"t1m\":%d}",
      gSettingsWriteId,
      dutyCycleState0.internalOnCount,dutyCycleState1.internalOnCount,dutyCycleState2.internalOnCount,dutyCycleState0.externalOnCount,dutyCycleState1.externalOnCount,dutyCycleState2.externalOnCount,
      gNoResetSettings.pt1001Offset,
      gNoResetSettings.pt1001Scale,
      gNoResetSettings.pt1002Offset,
      gNoResetSettings.pt1002Scale,
      gNoResetSettings.thermoCoupleOffset,
      gNoResetSettings.thermoCoupleScale,
      gNoResetSettings.zeroTenVOffset,
      gNoResetSettings.zeroTenVScale,
      gNoResetSettings.fourTwentyMAOffset,
      gNoResetSettings.fourTwentyMAScale,
      gADCRawValues.cpuTemp1,
      gADCRawValues.pt100Channel1,
      gADCRawValues.pt100Channel2,
      gADCRawValues.thermocouple1,
      gADCRawValues.fourTwentyMa,
      gADCRawValues.zeroTenV,
      inxRTCGetSeconds(),
      inxRTCADCGetSeconds(),
		  pState->element1Control,
		  pState->element1Monitor,
		  pState->element2Control,
		  pState->element2Monitor,
		  pState->element3Control,
		  pState->element3Monitor,
		  pState->thermalCO1Control,
		  pState->thermalCO1Monitor);
  if(written>0 && written<INX_MQTT_MAX_PAYLOAD_SIZE){
    //written ok
  }else{
    SW_DIAG_printf("failed to build state payload written=%d",written);
  }
}*/

static void buildUserTemperatureAlarmsPayload(char payload[CONFIG_ERROR_MESSAGE_SIZE],const float temperature){
	snprintf(payload,CONFIG_ERROR_MESSAGE_SIZE,"{\"temperature1\":%3.1f}",temperature);
}
void EhsMQTTConnectPoll(bool* connect,char** host,uint16_t* pPort,uint8_t* pTLS,char** clientid,char** username,char** password,char** clientCertFileName,char** clientKeyFileName,char** rootCAFileName);
void EhsMQTTConnectEvent(bool connect);
void mqttMainLoop(){
  bool connect=0;
  char* host=NULL;
  char* clientid=NULL;
  char* username=NULL;
  char* password=NULL;
  char* clientCertFileName=NULL;
  char* clientKeyFileName=NULL;
  char* rootCAFileName=NULL;
  uint16_t port=0;
  EhsMQTTConnectPoll(&connect,&host,&port,&gUseTLS,&clientid,&username,&password,&clientCertFileName,&clientKeyFileName,&rootCAFileName);
	err_t err=ERR_OK;
	char publishPayload[INX_MQTT_MAX_PAYLOAD_SIZE];
  printf("PBB MQTT_state=%d %d %s %d %d %s %s %s %s %s %s\n",MQTT_state,connect,host,port,gUseTLS,clientid,username,password,clientCertFileName,clientKeyFileName,rootCAFileName);
	switch(MQTT_state) {
		    case MQTT_STATE_INIT:
		    	gMqttConnectionAttempts = 0;
          if(connect){
            //after we have inited then start connection
		    	  mqttSetGlobalState(MQTT_STATE_DNS_LOOKUP);
          }
		    	break;
		    case MQTT_STATE_IDLE:
		      	break;
        case MQTT_STATE_DNS_LOOKUP:
            switch(inxNetworkTaskWaitForDNS(host,&brokerServerAddress)){
              case ERR_OK:
                printf("PBB 2147 %u.%u.%u.%u\n", ((u8_t *)&brokerServerAddress)[0],
							((u8_t *)&brokerServerAddress)[1], ((u8_t *)&brokerServerAddress)[2],
							((u8_t *)&brokerServerAddress)[3]);
                mqttSetGlobalState(MQTT_STATE_DO_CONNECT);
                break;
              case ERR_INPROGRESS:
                break;
              default:
                mqttSetGlobalState(MQTT_STATE_DO_DISCONNECT);
                break;
            }
          break;
		    case MQTT_STATE_DO_CONNECT:
				#if MQTT_USE_TLS
          if(connect){
            if(gUseTLS){
              if(TLS_Init(
                host,
                inxEHSFilesGetData(inxEHSFilesFind(clientCertFileName)),
                inxEHSFilesGetData(inxEHSFilesFind(clientKeyFileName)),
                inxEHSFilesGetData(inxEHSFilesFind(rootCAFileName))
              )!=0) {
                /* failed? */
                  LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("ERROR: failed to initialize for TLS!"));
                  MQTT_state=MQTT_STATE_DO_DISCONNECT;
                  break;
              }
            }
          }else{
            break;
          }
				#endif
				LWIP_DEBUGF(MQTT_DEBUG,("Connecting to broker attempt %d",gMqttConnectionAttempts));
				if (mqtt_do_connect(username,password,gUseTLS,port,&gMqttClient,&brokerServerAddress,clientid)==0) {
					MQTT_state = MQTT_STATE_DO_TLS_HANDSHAKE;
					gMqttTlsWaitForConnectionTicks=0;
				} else {
					LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("Failed to connect to broker"));
					MQTT_state=MQTT_STATE_DO_DISCONNECT;
				}
		      	break;
		#if MQTT_USE_TLS
		    case MQTT_STATE_DO_TLS_HANDSHAKE:
			    if (!gUseTLS || mqtt_do_tls_handshake(&gMqttClient)==0) {
			      	LWIP_DEBUGF(MQTT_DEBUG,("TLS handshake completed"));
              if(gUseTLS){
                mqtt_start_mqtt(&gMqttClient);
              }
			        MQTT_state = MQTT_STATE_WAIT_FOR_CONNECTION;
			        gMqttTlsWaitForConnectionTicks=0;
			    }else{
			    	gMqttTlsWaitForConnectionTicks++;
			    	if(gMqttTlsWaitForConnectionTicks>CONFIG_MQTT_MAX_TLS_TICKS){
              LWIP_DEBUGF(MQTT_DEBUG,("abandoning TLS handshake"));
              MQTT_state=MQTT_STATE_DO_DISCONNECT;
            }
			    }
			    break;
		#endif
		    case MQTT_STATE_WAIT_FOR_CONNECTION:
		      if (mqtt_client_is_connected(&gMqttClient)) {
		        LWIP_DEBUGF(MQTT_DEBUG,("Client is connected"));
		        MQTT_state = MQTT_STATE_CONNECTED;
            //err=my_mqtt_subscribe(&gMqttClient,"sdk/test/Python",NULL,0);
            err=ERR_OK;
            if(err==ERR_OK){
              //PRINTF("do disconnect because mqtt publish connectivity failed\n");
              //let everyone know we have connected and what state we are in
              gMqttConnections++; //record how many connections since boot
              //buildStatusPayload(gStatusPayload);
              //snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_CONNECTIVITY_DIDCONNECT,gMqttClientId);
              //err=my_mqtt_publish(&gMqttClient,gTopicNameTemporary,gStatusPayload,1,NULL);
              //if(err!=ERR_OK){
                //PRINTF("do disconnect because mqtt publish connectivity failed\n");
              //  MQTT_state=MQTT_STATE_DO_DISCONNECT;
              //}else{
                EhsMQTTConnectEvent(true);
              //} 
            }else{
              printf("PBB 2215 disconnected due to subscribe error %d\n",err);
              MQTT_state=MQTT_STATE_DO_DISCONNECT;
            }
		      } else {
		#if MQTT_USE_TLS
		    	  err=mqtt_tls_output_send(&gMqttClient); /* send (if any)  - we may want this outside the above condition when we send other date ...*/
		        err=mqtt_recv_from_tls(&gMqttClient);
		        gMqttTlsWaitForConnectionTicks++;
		        //deal with network failing after TLS but before connection is triggered
		        if(gMqttTlsWaitForConnectionTicks>CONFIG_MQTT_MAX_TLS_TICKS){
		        	LWIP_DEBUGF(MQTT_DEBUG,("abandoning TLS handshake"));
		        	MQTT_state=MQTT_STATE_DO_DISCONNECT;
		        }
		#endif
		      }
		      break;
		    case MQTT_STATE_CONNECTED:
		    	inxLedNetworkModeSet(INX_LED_NETWORK_MODE_CLOUD);
		      if (!mqtt_client_is_connected(&gMqttClient)) {
		        printf("PBB 2249 Client got disconnected?!?\n");
		        MQTT_state = MQTT_STATE_DO_DISCONNECT;
		      }
		#if MQTT_USE_TLS
		      else {
            #ifdef MBEDTLS_MEMORY_DEBUG
              mbedtls_memory_buffer_alloc_status();
            #endif //MBEDTLS_MEMORY_DEBUG
		      		gMqttConnectionAttempts = 0;
              if(connect){

              }else{
                MQTT_state=MQTT_STATE_DO_DISCONNECT;      
                break;
              }
#if CONFIG_MQTT_TEST_TLS_MEM_LEAK
					MQTT_state=MQTT_STATE_DO_DISCONNECT;
					break;
#endif //CONFIG_MQTT_TEST_TLS

		    	  if(gErrorLogs.locked){
		    		  LWIP_PLATFORM_DIAG(("mqtt:error logs locked"));
		    	  }else{
		    		  gErrorLogs.locked=true;
		    		  for(errorLogsError_t i=0;i<ERROR_LOGS_END;i++){
		    			  if(gErrorLogs.errors[i].unixTime>1){
		    				  switch(i){
		    				  	  case ERROR_LOGS_USER_ALARM_TEMPERATURE:
		    				  		  buildUserTemperatureAlarmsPayload(publishPayload,gUserAlarmsAdcValues);
								  	  break;
		    				  	  default:
		    				  		  snprintf(publishPayload,CONFIG_ERROR_MESSAGE_SIZE,"");
		    				  		  break;
		    				  }
		    				  snprintf(gMqttPayload,INX_MQTT_MAX_PAYLOAD_SIZE,"{\"a\":%d,\"err\":%d,\"m\":\"%s\",\"ut\":%d}",i,inxDisplayGetErrorCode(),publishPayload,gErrorLogs.errors[i].unixTime);
		    				  switch(i){
		    				  	  case ERROR_LOGS_ADC_DISABLED:
		    				  		  snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,"alarms/"CONFIG_CLIENT_GROUP_ID"/%s/adcdisabled",gMqttClientId);
		    				  		  break;
		    				  	  case ERROR_LOGS_USER_ALARM_TEMPERATURE:
		    				  		  snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,"alarms/"CONFIG_CLIENT_GROUP_ID"/%s/userTemperatureAlarm",gMqttClientId);
		    				  		  break;
		    				  	  case ERROR_LOGS_ADC_VALUES_UNSAFE:
		    				  		  snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,"alarms/"CONFIG_CLIENT_GROUP_ID"/%s/adcunsafe",gMqttClientId);
		    				  		  break;
		    				  	  default:
		    				  		  LWIP_PLATFORM_DIAG(("mqtt:unknown error %d",i));
		    				  		  snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,"alarms/"CONFIG_CLIENT_GROUP_ID"/%s/unknown",gMqttClientId);
		    				  		  break;
		    				  }

		    				  /*err=my_mqtt_publish(&gMqttClient,gTopicNameTemporary,gMqttPayload,0,NULL);
                  if(err==ERR_OK){
                    gErrorLogs.errors[i].unixTime=1;
                    gErrorLogsUpdated=true;
                  }else{
                    printf("PBB 2305 do disconnect because of send alarm error=%d",err);
                    MQTT_state=MQTT_STATE_DO_DISCONNECT;
                    break;
                  }*/
		    			  }
		    		  }
		    		  gErrorLogs.locked=false;
		    	  }

		    	  /*if(gRelayErrorState.time>0){
              //we send to settings because that is easiest way to get something back at this stage
              buildStatePayload(&gRelayErrorState,publishPayload);
              snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_SETTINGS_TOPIC_NAME,gMqttClientId);
              err=my_mqtt_publish(&gMqttClient,gTopicNameTemporary,publishPayload,MQTT_QOS_0,NULL);
              if(err==ERR_OK){
                gRelayErrorState.time=0;
              }else{
                printf("PBB 2322 do disconnect because mqtt publish relay state failed");
                MQTT_state=MQTT_STATE_DO_DISCONNECT;
              }
            }else if(gGenerateCommissioningReport==true){
              snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,"update/reports/commissioning/" CONFIG_CLIENT_GROUP_ID "/%s",gMqttClientId);
              //for now just send back our last known relay state
              buildStatePayload(&gStartupRelayErrorState,gMqttPayload);
		    		  err=my_mqtt_publish(&gMqttClient,gTopicNameTemporary,gMqttPayload,0,NULL);
		    		  if(err==ERR_OK){
		    			  gGenerateCommissioningReport=false;
		    		  }else{
		    			  LWIP_DEBUGF(MQTT_DEBUG,("do disconnect because of send commissioning error=%d",err));
						    MQTT_state=MQTT_STATE_DO_DISCONNECT;
						    break;
		    		  }
            }else if(gGenerateDiagnosticReport==true){
              snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,"update/reports/diagnostic/" CONFIG_CLIENT_GROUP_ID "/%s",gMqttClientId);
              buildStatePayload(&gRelayErrorState,gMqttPayload);
              err=my_mqtt_publish(&gMqttClient,gTopicNameTemporary,gMqttPayload,0,NULL);
		    		  if(err==ERR_OK){
		    			  gGenerateDiagnosticReport=false;
		    		  }else{
		    			  LWIP_DEBUGF(MQTT_DEBUG,("do disconnect because of send diagnostic error=%d",err));
						    MQTT_state=MQTT_STATE_DO_DISCONNECT;
						    break;
		    		  }
            }else{
              printf("PBB 2349 poll EHS\n");
              
            }*/

            bool subscribe=false;
              uint8_t qos=0;
              bool needProcessing=EhsMQTTSubscribeWritePoll(gTopicNameTemporary,&subscribe,&qos);
              if(needProcessing==false){
                needProcessing=EhsMQTTPublishWritePoll(gTopicNameTemporary,gMqttPayload,&qos);
                if(needProcessing==false){
                  
                }else{
                  err=my_mqtt_publish(&gMqttClient,gTopicNameTemporary,gMqttPayload,qos,NULL);
                  if(err==ERR_OK){

                  }else{
                    printf("PBB do disconnect because of send EHS=%d\n",err);
                    MQTT_state=MQTT_STATE_DO_DISCONNECT;
                    break;
                  }
                }
              }else{
                if(subscribe){
                  err=my_mqtt_subscribe(&gMqttClient,gTopicNameTemporary,NULL,qos);
                }else{
                  err=mqtt_unsubscribe(&gMqttClient,gTopicNameTemporary,NULL,NULL);
                }
                if(err==ERR_OK){

                }else{
                  printf("PBB do disconnect because of subscribe EHS=%d\n",err);
                  MQTT_state=MQTT_STATE_DO_DISCONNECT;
                  break;  
                }
              }

            //do we need to request a firmware chunk
            if(gFirmwareOTAStarted!=INX_FIRMWARE_UPDATE_NONE){
              if(gFirmwareWaitingForChunkTicks==0){
                uint32_t startByte = (mainGetFlashSectorSize() * gFirmwareCurrentSector) + gFirmwareCurrentByte;
                gFirmwareByteWeAreWaitingFor=startByte;
                LWIP_DEBUGF(MQTT_DEBUG,("requesting sector %d byte %d",gFirmwareCurrentSector,startByte));
                buildFirmwareChunkPayload(publishPayload,startByte);
                switch(gFirmwareOTAStarted){
                  case INX_FIRMWARE_UPDATE_SODL:
                    snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_SODLCHUNK_GET,gMqttClientId);
                    break;
                  default:
                    snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_FIRMWARECHUNK_GET,gMqttClientId);
                    break;
                }
                err=my_mqtt_publish(&gMqttClient,gTopicNameTemporary,publishPayload,0,NULL);
                if(err!=ERR_OK){
                  LWIP_DEBUGF(MQTT_DEBUG,("do disconnect because mqtt publish get firmware chunk failed"));
                  MQTT_state=MQTT_STATE_DO_DISCONNECT;
                }else{
                  gFirmwareWaitingForChunkTicks++;
                }
              }else{
                gFirmwareWaitingForChunkTicks++;
                if(gFirmwareWaitingForChunkTicks>5){
                  gFirmwareWaitingForChunkTicks=0;
                }
              }
            }else if(gFirmwareStartOTA==INX_FIRMWARE_UPDATE_FIRMWARE){
              //we've been asked to start an OTA update so subscribe to the posted chunks channel
              snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_FIRMWARECHUNK_POST,gMqttClientId);
              my_mqtt_subscribe(&gMqttClient,gTopicNameTemporary,NULL,MQTT_QOS_0);
              gFirmwareStartOTA=INX_FIRMWARE_UPDATE_NONE;
              gFirmwareOTAStarted=INX_FIRMWARE_UPDATE_FIRMWARE;
            }else if(gFirmwareStartOTA==INX_FIRMWARE_UPDATE_SODL){
              snprintf(gTopicNameTemporary,CONFIG_MQTT_MAX_TOPIC_LENGTH,CONFIG_MQTT_TOPIC_NAME_SODLCHUNK_POST,gMqttClientId);
              err=my_mqtt_subscribe(&gMqttClient,gTopicNameTemporary,NULL,MQTT_QOS_0);
              if(err==ERR_OK){
                gFirmwareStartOTA=INX_FIRMWARE_UPDATE_NONE;
                gFirmwareOTAStarted=INX_FIRMWARE_UPDATE_SODL;
              }else{
                MQTT_state=MQTT_STATE_DO_DISCONNECT;
              }
            }

		    	 err=mqtt_tls_output_send(&gMqttClient); /* send (if any)  - we may want this outside the above condition when we send other data ...*/
		    	 if(err!=ERR_OK){
		    		 MQTT_state=MQTT_STATE_DO_DISCONNECT;
		    		 printf("PBB 2417 disconnect due to bad send\n");
		    	 }
           if(gUseTLS){
            err=mqtt_recv_from_tls(&gMqttClient); /* poll if we have incoming packets */
            if(err!=ERR_OK){
              printf("PBB do disconnect because mqtt recv failed %d\n",err);
              MQTT_state=MQTT_STATE_DO_DISCONNECT;
            }
           }//if(gUseTLS)
		    	 
		      }
		#endif
		      break;
		#if 0
		    case MQTT_STATE_DO_PUBLISH:
		    LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("Publish to broker\r\n"));
		    my_mqtt_publish(gMqttClient, NULL);
		    MQTT_state = MQTT_STATE_CONNECTED;
		    break;
		#endif
		    case MQTT_STATE_DO_DISCONNECT:
		    	LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("Disconnect from broker"));
		    	mqtt_disconnect(&gMqttClient);
		    	MQTT_TlsClose();
          EhsMQTTConnectEvent(false);
          if(gMqttConnectionAttempts>=CONFIG_MQTT_MAX_CONNECTION_ATTEMPTS){
            //we've tried too many times to connect, our local network connection is probably broken
            mainSetNetworkState(NETWORK_STATE_DO_DISCONNECT);
            LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("mqtt:trigger network restart"));
          }else{
            MQTT_state = MQTT_STATE_DNS_LOOKUP;
            gMqttConnectionAttempts=gMqttConnectionAttempts+1;
          }
          inxLedNetworkModeSet(INX_LED_NETWORK_MODE_LOCAL);
          break;
			default:
				break;
		}
}


/**
 * @ingroup mqtt
 * Connect to MQTT server
 * @param client MQTT client
 * @param ip_addr Server IP
 * @param port Server port
 * @param cb Connection state change callback
 * @param arg User supplied argument to connection callback
 * @param client_info Client identification and connection options
 * @return ERR_OK if successful, @see err_t enum for other results
 */
err_t
mqtt_client_connect(mqtt_client_t *client, const ip_addr_t *ip_addr, u16_t port, mqtt_connection_cb_t cb, void *arg,
                    const struct mqtt_connect_client_info_t *client_info)
{
  err_t err;
  size_t len;
  u16_t client_id_length;
  /* Length is the sum of 2+"MQTT", protocol level, flags and keep alive */
  u16_t remaining_length = 2 + 4 + 1 + 1 + 2;
  u8_t flags = 0, will_topic_len = 0, will_msg_len = 0;
  u8_t client_user_len = 0, client_pass_len = 0;

  if(client==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_client_connect: client == NULL"));
	  return ERR_ARG;
  }

  if(ip_addr==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_client_connect: ip_addr == NULL"));
	  return ERR_ARG;
  }

  if(client_info==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_client_connect: client_info == NULL"));
	  return ERR_ARG;
  }

  if(client_info->client_id==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_client_connect: client_info->client_id == NULL"));
	  return ERR_ARG;
  }


  if (client->conn_state != TCP_DISCONNECTED) {
    LWIP_DEBUGF(MQTT_DEBUG_WARN,("mqtt_client_connect: Already connected\n"));
    return ERR_ISCONN;
  }

  /* Wipe clean */
  //memset(client, 0, sizeof(mqtt_client_t)); // this must be done by the caller!
  client->connect_arg = arg;
  client->connect_cb = cb;
  client->keep_alive = client_info->keep_alive;
  mqtt_init_requests(client->req_list);

  /* Build connect message */
  if (client_info->will_topic != NULL && client_info->will_msg != NULL) {
    flags |= MQTT_CONNECT_FLAG_WILL;
    flags |= (client_info->will_qos & 3) << 3;
    if (client_info->will_retain) {
      flags |= MQTT_CONNECT_FLAG_WILL_RETAIN;
    }
    len = strlen(client_info->will_topic);
    LWIP_ERROR("mqtt_client_connect: client_info->will_topic length overflow", len <= 0xFF, return ERR_VAL);
    LWIP_ERROR("mqtt_client_connect: client_info->will_topic length must be > 0", len > 0, return ERR_VAL);
    will_topic_len = (u8_t)len;
    len = strlen(client_info->will_msg);
    LWIP_ERROR("mqtt_client_connect: client_info->will_msg length overflow", len <= 0xFF, return ERR_VAL);
    will_msg_len = (u8_t)len;
    len = remaining_length + 2 + will_topic_len + 2 + will_msg_len;
    LWIP_ERROR("mqtt_client_connect: remaining_length overflow", len <= 0xFFFF, return ERR_VAL);
    remaining_length = (u16_t)len;
  }
  if (client_info->client_user != NULL && strlen(client_info->client_user)>0) {
    flags |= MQTT_CONNECT_FLAG_USERNAME;
    len = strlen(client_info->client_user);
    LWIP_ERROR("mqtt_client_connect: client_info->client_user length overflow", len <= 0xFF, return ERR_VAL);
    LWIP_ERROR("mqtt_client_connect: client_info->client_user length must be > 0", len > 0, return ERR_VAL);
    client_user_len = (u8_t)len;
    len = remaining_length + 2 + client_user_len;
    LWIP_ERROR("mqtt_client_connect: remaining_length overflow", len <= 0xFFFF, return ERR_VAL);
    remaining_length = (u16_t)len;
  }
  if (client_info->client_pass != NULL && strlen(client_info->client_pass)>0) {
    flags |= MQTT_CONNECT_FLAG_PASSWORD;
    len = strlen(client_info->client_pass);
    LWIP_ERROR("mqtt_client_connect: client_info->client_pass length overflow", len <= 0xFF, return ERR_VAL);
    LWIP_ERROR("mqtt_client_connect: client_info->client_pass length must be > 0", len > 0, return ERR_VAL);
    client_pass_len = (u8_t)len;
    len = remaining_length + 2 + client_pass_len;
    LWIP_ERROR("mqtt_client_connect: remaining_length overflow", len <= 0xFFFF, return ERR_VAL);
    remaining_length = (u16_t)len;
  }

  /* Don't complicate things, always connect using clean session */
  flags |= MQTT_CONNECT_FLAG_CLEAN_SESSION;

  len = strlen(client_info->client_id);
  LWIP_ERROR("mqtt_client_connect: client_info->client_id length overflow", len <= 0xFFFF, return ERR_VAL);
  client_id_length = (u16_t)len;
  len = remaining_length + 2 + client_id_length;
  LWIP_ERROR("mqtt_client_connect: remaining_length overflow", len <= 0xFFFF, return ERR_VAL);
  remaining_length = (u16_t)len;

  if (mqtt_output_check_space(&client->output, remaining_length) == 0) {
    return ERR_MEM;
  }

  client->conn = tcp_new();
  if (client->conn == NULL) {
    return ERR_MEM;
  }

  /* Set arg pointer for callbacks */
  tcp_arg(client->conn, client);
  /* Any local address, pick random local port number */
  err = tcp_bind(client->conn, IP_ADDR_ANY, 0);
  if (err != ERR_OK) {
    LWIP_DEBUGF(MQTT_DEBUG_WARN,("mqtt_client_connect: Error binding to local ip/port, %d\n", err));
    goto tcp_fail;
  }
  LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_client_connect: Connecting to host: %s at port:%"U16_F"\n", ipaddr_ntoa(ip_addr), port));

  /* Connect to server */
  err = tcp_connect(client->conn, ip_addr, port, mqtt_tcp_connect_cb);
  if (err != ERR_OK) {
    LWIP_DEBUGF(MQTT_DEBUG_TRACE,("mqtt_client_connect: Error connecting to remote ip/port, %d\n", err));
    goto tcp_fail;
  }
  /* Set error callback */
  tcp_err(client->conn, mqtt_tcp_err_cb);
  client->conn_state = TCP_CONNECTING;

//us
//101700044d51545404020258000b6261736963507562537562
//101700044d51545404020258000b6261736963507562537562
//10CONNECT 170  44d5154544203c0b6261736963507562537562
//10CONNECT 140  44d5154544203c0873646b2d6a617661
//10CONNECT 17LENGTH   044d515454MQTT
//10CONNECT 17LENGTH 00044d515454MQTT PROTO04 FLAGS02 KEEPALIVE003c CLIENTID000b6261736963507562537562
//python
//10CONNECT 17LENGTH 00044d515454MQTT PROTO04 FLAGS02 KEEPALIVE0258 CLIENTID000b6261736963507562537562
//10CONNECT 3200044d515454
//10CONNECT 3200044d51545404820258000b626173696350756253756200193f53444b3d507974686f6e2656657273696f6e3d312e342e39
  //printf("PBB 2599 mqtt_client_connect: Sending CONNECT message\n", err);
  /* Append fixed header */
  mqtt_output_append_fixed_header(&client->output, MQTT_MSG_TYPE_CONNECT, 0, 0, 0, remaining_length);
  //printf("PBB 2608 header, now MQTT to follow\n");
  /* Append Protocol string */
  mqtt_output_append_string(&client->output, "MQTT", 4);
  //printf("PBB 2611 MQTT done, now protocol level\n");
  /* Append Protocol level */
  mqtt_output_append_u8(&client->output, 4);
  //printf("\nPBB 2617 flags to follow\n");
  /* Append connect flags */
  mqtt_output_append_u8(&client->output, flags);
  //printf("\nPBB 2620 keep_alive to follow\n");
  /* Append keep-alive */
  mqtt_output_append_u16(&client->output, client_info->keep_alive);
  //printf("\nPBB 2623 client_id to follow\n");
  /* Append client id */
  mqtt_output_append_string(&client->output, client_info->client_id, client_id_length);
  /* Append will message if used */
  if ((flags & MQTT_CONNECT_FLAG_WILL) != 0) {
    mqtt_output_append_string(&client->output, client_info->will_topic, will_topic_len);
    mqtt_output_append_string(&client->output, client_info->will_msg, will_msg_len);
  }
  /* Append user name if given */
  if ((flags & MQTT_CONNECT_FLAG_USERNAME) != 0) {
    mqtt_output_append_string(&client->output, client_info->client_user, client_user_len);
  }
  /* Append password if given */
  if ((flags & MQTT_CONNECT_FLAG_PASSWORD) != 0) {
    mqtt_output_append_string(&client->output, client_info->client_pass, client_pass_len);
  }
  return ERR_OK;

tcp_fail:
  tcp_abort(client->conn);
  client->conn = NULL;
  return err;
}


/**
 * @ingroup mqtt
 * Disconnect from MQTT server
 * @param client MQTT client
 */
void
mqtt_disconnect(mqtt_client_t *client)
{
  if(client==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("can not disconnect null mqtt client"));
  }else{
	  /* If connection in not already closed */
	    if (client->conn_state != TCP_DISCONNECTED) {
	      /* Set conn_state before calling mqtt_close to prevent callback from being called */
	      if (mqtt_output_check_space(&client->output, 0) != 0) { /* << EST added */
	        mqtt_output_append_fixed_header(&client->output, MQTT_MSG_TYPE_DISCONNECT, 0, 0, 0, 0);
	        mqtt_output_send(client, &client->output, client->conn);
	        client->cyclic_tick = 0;
	      }
	      client->conn_state = TCP_DISCONNECTED;
	      mqtt_close(client, (mqtt_connection_status_t)0);
	    }
  }

}

/**
 * @ingroup mqtt
 * Check connection with server
 * @param client MQTT client
 * @return 1 if connected to server, 0 otherwise
 */
u8_t
mqtt_client_is_connected(mqtt_client_t *client)
{
  if(client==NULL){
	  LWIP_DEBUGF(MQTT_DEBUG,("mqtt_client_is_connected: client == NULL"));
	  return 0;
  }
  return client->conn_state == MQTT_CONNECTED;
}

#if MQTT_USE_TLS
/**
 * @ingroup mqtt
 * Check connection with server
 * @param client MQTT client
 * @return 1 if TLS handshaking to server, 0 otherwise
 */
u8_t
mqtt_client_is_handshaking(mqtt_client_t *client)
{
	if(client==NULL){
		LWIP_DEBUGF(MQTT_DEBUG,("mqtt_client_is_handshaking: client == NULL"));
		return 0;
	}

  return client->conn_state == TLS_HANDSHAKING;
}
#endif


#endif /* LWIP_TCP && LWIP_CALLBACK_API */
#endif //CONFIG_MQTT_ENABLED
