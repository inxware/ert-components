#ifndef _TARGET_MBPORT_H_ 
#define _TARGET_MBPORT_H_ 


/* ----------------------- Platform includes --------------------------------*/
#include "esp_log.h"

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "freertos/event_groups.h"
#include "port/port.h"
#include "mbconfig.h"

#include "globals.h"

/* ----------------------- Defines ------------------------------------------*/

#ifndef TRUE
#define TRUE                    1
#endif

#ifndef FALSE
#define FALSE                   0
#endif

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

/* ----------------------- Type definitions ---------------------------------*/

typedef struct {
    int xIndex;                 /*!< Slave information index */
    int xSockId;                /*!< Socket ID of slave */
    int xError;                 /*!< Socket error */
    int xRcvErr;                /*!< Socket receive error */
    const char* pcIpAddr;       /*!< TCP/UDP IP address */
    mbUCHAR ucSlaveAddr;          /*!< Slave short address */
    mbUCHAR* pucRcvBuf;           /*!< Receive buffer pointer */
    mbUSHORT usRcvPos;            /*!< Receive buffer position */
    int pcPort;                 /*!< TCP/UDP port number */
    eMBPortProto xMbProto;      /*!< Protocol type */
    int64_t xSendTimeStamp;     /*!< Send request time stamp */
    int64_t xRecvTimeStamp;     /*!< Receive response time stamp */
    uint16_t usTidCnt;          /*!< Transaction identifier (TID) for slave */
} MbSlaveInfo_t;

typedef struct {
    TaskHandle_t  xMbTcpTaskHandle;     /*!< Master TCP/UDP handling task handle */
    QueueHandle_t xConnectQueue;        /*!< Master connection queue */
    mbUSHORT usPort;                      /*!< Master TCP/UDP port number */
    mbUSHORT usMbSlaveInfoCount;          /*!< Master count of connected slaves */
    mbUSHORT ucCurSlaveIndex;             /*!< Master current processing slave index */
    eMBPortIpVer eMbIpVer;              /*!< Master IP version */
    eMBPortProto eMbProto;              /*!< Master protocol type */
    void* pvNetIface;                   /*!< Master netif interface pointer */
    MbSlaveInfo_t** pxMbSlaveInfo;      /*!< Master information structure for each connected slave */
    MbSlaveInfo_t* pxMbSlaveCurrInfo;   /*!< Master current slave information */
} MbPortConfig_t;

typedef struct {
    mbUSHORT usIndex;                     /*!< index of the address info */
    const char* pcIPAddr;               /*!< represents the IP address of the slave */
    mbUCHAR ucSlaveAddr;                  /*!< slave unit ID (UID) field for MBAP frame  */
} MbSlaveAddrInfo_t;

typedef struct {
    int xIndex;                     /*!< Modbus info index */
    int xSockId;                    /*!< Socket id */
    int xError;                     /*!< TCP/UDP sock error */
    const char* pcIpAddr;           /*!< TCP/UDP IP address (string) */
    mbUCHAR* pucTCPBuf;               /*!< buffer pointer */
    mbUSHORT usTCPBufPos;             /*!< buffer active position */
    mbUSHORT usTCPFrameBytesLeft;     /*!< buffer left bytes to receive transaction */
    int64_t xSendTimeStamp;         /*!< send request timestamp */
    int64_t xRecvTimeStamp;         /*!< receive response timestamp */
    mbUSHORT usTidCnt;                /*!< last TID counter from packet */
} MbClientInfo_t;

typedef struct {
    TaskHandle_t xMbTcpTaskHandle;      /*!< Server task handle */
    QueueHandle_t xRespQueueHandle;      /*!< Response queue handle */
    MbClientInfo_t* pxCurClientInfo;    /*!< Current client info */
    MbClientInfo_t** pxMbClientInfo;    /*!< Pointers to information about connected clients */
    mbUSHORT usPort;                      /*!< TCP/UDP port number */
    mbCHAR* pcBindAddr;                   /*!< IP address to bind */
    eMBPortProto eMbProto;              /*!< Protocol type used by port */
    mbUSHORT usClientCount;               /*!< Client connection count */
    void* pvNetIface;                   /*!< Network netif interface pointer for port */
    eMBPortIpVer xIpVer;                /*!< IP protocol version */
} MbSlavePortConfig_t;

typedef struct {
    esp_timer_handle_t xTimerIntHandle;
    mbUSHORT usT35Ticks;
    mbBOOL xTimerState;
} xTimerContext_t;

mbBOOL xMBPortSerialWaitEvent(QueueHandle_t xMbUartQueue, uart_event_t* pxEvent, mbULONG xTimeout);

EHS_GLOBAL void EhsHMBTaskInLoop( ehs_bool use_tcpip );

#endif//_TARGET_MBPORT_H_ 