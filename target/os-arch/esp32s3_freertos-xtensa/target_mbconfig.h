#ifndef _TARGET_MBCONFIG_H_ 
#define _TARGET_MBCONFIG_H_ 

/* @brief This is the target specific configuration for MODBUS if this enabled for a particular target */

#define EHS_CONFIG_FMB_TCP_PORT_DEFAULT 502
#define EHS_CONFIG_FMB_TCP_PORT_MAX_CONN 5
#define EHS_CONFIG_FMB_TCP_CONNECTION_TOUT_SEC 20
#define EHS_CONFIG_FMB_PORT_TASK_STACK_SIZE 2048
#define EHS_CONFIG_FMB_PORT_TASK_PRIO 10
#define EHS_CONFIG_FMB_PORT_TASK_AFFINITY_CPU0 1
#define EHS_CONFIG_FMB_PORT_TASK_AFFINITY 0x0
#define EHS_CONFIG_FMB_EVENT_QUEUE_TIMEOUT 20
#define EHS_CONFIG_FMB_SERIAL_BUF_SIZE 256
#define CONFIG_FMB_MASTER_TIMEOUT_MS_RESPOND 400
#define CONFIG_FMB_MASTER_DELAY_MS_CONVERT 200
#define CONFIG_FMB_TIMER_PORT_ENABLED 1
#define CONFIG_FMB_TCP_CONNECTION_TOUT_SEC 20

#define CONFIG_UART_ISR_IN_IRAM 1


/*! \brief If Modbus Master/Slave ASCII support is enabled. */
#ifndef EHS_CONFIG_FMB_COMM_MODE_ASCII_EN
#define EHS_CONFIG_FMB_COMM_MODE_ASCII_EN 1
#endif
/*! \brief If Modbus Master/Slave RTU support is enabled. */
#ifndef EHS_CONFIG_FMB_COMM_MODE_RTU_EN
#define EHS_CONFIG_FMB_COMM_MODE_RTU_EN 1
#endif
/*! \brief If Modbus Master/Slave TCP support is enabled. */
#ifndef EHS_CONFIG_FMB_COMM_MODE_TCP_EN
#define EHS_CONFIG_FMB_COMM_MODE_TCP_EN 1
#endif

/*! \brief The option is required for support of RTU over TCP.
 */
#ifndef EHS_CONFIG_FMB_TCP_UID_ENABLED
#define EHS_CONFIG_FMB_TCP_UID_ENABLED 1
#endif

/*! \brief This option defines the number of data bits per ASCII character.
 *
 * A parity bit is added before the stop bit which keeps the actual byte size at 10 bits.
 */
#ifndef EHS_CONFIG_FMB_SERIAL_ASCII_BITS_PER_SYMB
#define EHS_CONFIG_FMB_SERIAL_ASCII_BITS_PER_SYMB 8
#endif

#ifndef EHS_CONFIG_FMB_CONTROLLER_NOTIFY_QUEUE_SIZE
#define EHS_CONFIG_FMB_CONTROLLER_NOTIFY_QUEUE_SIZE 20
#endif

/*! \brief The character timeout value for Modbus ASCII.
 *
 * The character timeout value is not fixed for Modbus ASCII and is therefore
 * a configuration option. It should be set to the maximum expected delay
 * time of the network.
 */
#ifndef EHS_CONFIG_FMB_SERIAL_ASCII_TIMEOUT_RESPOND_MS
#define EHS_CONFIG_FMB_SERIAL_ASCII_TIMEOUT_RESPOND_MS 1000
#endif

/*! \brief Timeout to wait in ASCII prior to enabling transmitter.
 *
 * If defined the function calls vMBPortSerialDelay with the argument
 * MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS to allow for a delay before
 * the serial transmitter is enabled. This is required because some
 * targets are so fast that there is no time between receiving and
 * transmitting the frame. If the master is to slow with enabling its
 * receiver then he will not receive the response correctly.
 */
#ifndef EHS_CONFIG_FMB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS
#define EHS_CONFIG_FMB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS 0
#endif

/*! \brief If the <em>Report Slave ID</em> function should be enabled. */
#ifndef EHS_CONFIG_FMB_CONTROLLER_SLAVE_ID_SUPPORT
#define EHS_CONFIG_FMB_CONTROLLER_SLAVE_ID_SUPPORT 1
#endif

/*! \brief Check the option to place timer handler into IRAM */
#ifndef EHS_CONFIG_FMB_TIMER_USE_ISR_DISPATCH_METHOD
#define EHS_CONFIG_FMB_TIMER_USE_ISR_DISPATCH_METHOD 1
#endif

/*! \brief If master send a broadcast frame, the master will wait time of convert to delay,
 * then master can send other frame */
#ifndef EHS_CONFIG_FMB_MASTER_DELAY_MS_CONVERT
#define EHS_CONFIG_FMB_MASTER_DELAY_MS_CONVERT 200
#endif

/*! \brief If master send a frame which is not broadcast,the master will wait sometime for slave.
 * And if slave is not respond in this time,the master will process this timeout error.
 * Then master can send other frame */
#ifndef EHS_CONFIG_FMB_MASTER_TIMEOUT_MS_RESPOND
#define EHS_CONFIG_FMB_MASTER_TIMEOUT_MS_RESPOND 400
#endif

/*********************** TARGET SPECIFIC CONFIG ***********************/

#ifndef EHS_CONFIG_FMB_QUEUE_LENGTH
#define EHS_CONFIG_FMB_QUEUE_LENGTH 20
#endif//EHS_CONFIG_FMB_QUEUE_LENGTH

// #ifndef EHS_CONFIG_FMB_CHANNEL_LIMIT 
// #define EHS_CONFIG_FMB_CHANNEL_LIMIT 32
// #endif//EHS_CONFIG_FMB_CHANNEL_LIMIT 

#ifndef EHS_CONFIG_MB_MAX_RESPONSE_TIMEOUT_MS 
#define EHS_CONFIG_MB_MAX_RESPONSE_TIMEOUT_MS (5000)
#endif//EHS_CONFIG_MB_MAX_RESPONSE_TIMEOUT_MS 

#endif//_TARGET_MBCONFIG_H_ 