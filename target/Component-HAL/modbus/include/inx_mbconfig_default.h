#ifndef _INX_MBCONFIG_DEFAULT_H_ 
#define _INX_MBCONFIG_DEFAULT_H_ 

/*! \brief If Modbus Master/Slave ASCII support is enabled. */
#define EHS_CONFIG_FMB_COMM_MODE_ASCII_EN 1
/*! \brief If Modbus Master/Slave RTU support is enabled. */
#define EHS_CONFIG_FMB_COMM_MODE_RTU_EN 1
/*! \brief If Modbus Master/Slave TCP support is enabled. */
#define EHS_CONFIG_FMB_COMM_MODE_TCP_EN 1

/*! \brief The option is required for support of RTU over TCP.
 */
#define EHS_CONFIG_FMB_TCP_UID_ENABLED 1

/*! \brief This option defines the number of data bits per ASCII character.
 *
 * A parity bit is added before the stop bit which keeps the actual byte size at 10 bits.
 */
#define EHS_CONFIG_FMB_SERIAL_ASCII_BITS_PER_SYMB 8

#define EHS_CONFIG_FMB_CONTROLLER_NOTIFY_QUEUE_SIZE 20

/*! \brief The character timeout value for Modbus ASCII.
 *
 * The character timeout value is not fixed for Modbus ASCII and is therefore
 * a configuration option. It should be set to the maximum expected delay
 * time of the network.
 */
#define EHS_CONFIG_FMB_SERIAL_ASCII_TIMEOUT_RESPOND_MS 1000

/*! \brief Timeout to wait in ASCII prior to enabling transmitter.
 *
 * If defined the function calls vMBPortSerialDelay with the argument
 * MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS to allow for a delay before
 * the serial transmitter is enabled. This is required because some
 * targets are so fast that there is no time between receiving and
 * transmitting the frame. If the master is to slow with enabling its
 * receiver then he will not receive the response correctly.
 */
#define EHS_CONFIG_FMB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS 0

/*! \brief If the <em>Report Slave ID</em> function should be enabled. */
#define EHS_CONFIG_FMB_CONTROLLER_SLAVE_ID_SUPPORT 1

/*! \brief Check the option to place timer handler into IRAM */
#define EHS_CONFIG_FMB_TIMER_USE_ISR_DISPATCH_METHOD 1

/*! \brief If master send a broadcast frame, the master will wait time of convert to delay,
 * then master can send other frame */
#define EHS_CONFIG_FMB_MASTER_DELAY_MS_CONVERT 200

/*! \brief If master send a frame which is not broadcast,the master will wait sometime for slave.
 * And if slave is not respond in this time,the master will process this timeout error.
 * Then master can send other frame */
#define EHS_CONFIG_FMB_MASTER_TIMEOUT_MS_RESPOND 400

#endif//_INX_MBCONFIG_DEFAULT_H_ 