#include "heatrod_config.h"
#include "inx_uart.h"
#include "fsl_uart.h"
#include "board.h"
#include "mbport.h"

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
  Ring buffer empty: (rxIndex == txIndex)
*/
uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */
volatile bool charStarted = false;
static bool gDoingUartDiagnostics = false;
uint8_t inxUartByte = 0;
inxModbusState_t gModbusState = INX_MODBUS_STATE_STOPPED;
uint32_t gUARTBaudRate=19200;
uint8_t gUARTStopBits=1;
uint8_t gUARTParity=2;
//static uint32_t irqCount = 0;
void DEMO_UART_IRQHandler(void)
{
    uint8_t data;
    //if we enable this then it breaks transmission
    //LWIP_PLATFORM_DIAG(("DEMO_UART_IRQHandler"));
    /* If new data arrived. */
    uint32_t statusFlags =UART_GetStatusFlags(DEMO_UART);
    if(kUART_IdleLineFlag & statusFlags){
    	LWIP_DEBUGF(DEBUG_MODBUS,("uartIrqHandler: idle, reading data to clear"));
    	UART_ReadByte(DEMO_UART);
    }else if(kUART_NoiseErrorFlag & statusFlags){
    	LWIP_DEBUGF(DEBUG_MODBUS,("uartIrqHandler: noise, reading data to clear"));
    	UART_ReadByte(DEMO_UART);
    } else if(kUART_FramingErrorFlag & statusFlags){
    	LWIP_DEBUGF(DEBUG_MODBUS,("uartIrqHandler: framing error, reading data to clear"));
		UART_ReadByte(DEMO_UART);
    } else if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & statusFlags){
    	if(gDoingUartDiagnostics){
    		data = UART_ReadByte(DEMO_UART);
    		//LWIP_PLATFORM_DIAG(("DEMO_UART_IRQHandler: %d",data));
			if(data==255 && charStarted==true){
				//only register a character as done we get the stop bits
				rxIndex++;
				rxIndex %= DEMO_RING_BUFFER_SIZE;
				charStarted=false;
			}else if(data!=255){
				//LWIP_PLATFORM_DIAG(("%d",data));
				/* If ring buffer is not full, add data to ring buffer. */
				//if (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) != txIndex)
				//{
					demoRingBuffer[rxIndex] = data;
					charStarted=true;
				//}
			}
    	}else{
    		if(statusFlags & kUART_RxOverrunFlag){
    			LWIP_DEBUGF(DEBUG_MODBUS,("uart overrun"));
    			gModbusState = INX_MODBUS_STATE_STOP;
    		    //overflow

    		      if ( (DEMO_UART-> SFIFO) & UART_SFIFO_RXOF_MASK)

    		      {

    		          DEMO_UART->C2 &= ~UART_C2_RE_MASK; // recommended to disable RE

    		#if 1

    		          //read the overflowed data
    		    	  UART_ReadByte(DEMO_UART);
    		          /*while (uart_getchar_present())

    		          {

    		              get=uart_getchar();    // will also clear the OR flag

    		              uart_putchar(get);

    		              time_delay_ms(2);

    		          }*/

    		          DEMO_UART-> CFIFO |= UART_CFIFO_RXFLUSH_MASK;    //flush RX buffer

    		          DEMO_UART-> SFIFO |= UART_SFIFO_RXOF_MASK;    //clear RXOF

    		#else

    		          //flush read FIFO buffer

    		          status = (UART_PORT-> S1) & UART_S1_OR_MASK; // clear OR - read OR and read Data (disable RE clears OR also)

    		          get = UART_PORT->D;

    		          UART_PORT-> CFIFO |= UART_CFIFO_RXFLUSH_MASK;    //flush RX buffer

    		          UART_PORT-> SFIFO |= UART_SFIFO_RXOF_MASK;    //clear RXOF

    		#endif

    		          DEMO_UART->C2 |= UART_C2_RE_MASK;

    		      }
    		}else{
    			uint8_t count=0;
    			do{
						inxUartByte=UART_ReadByte(DEMO_UART);
						pxMBFrameCBByteReceived();
						statusFlags=UART_GetStatusFlags(DEMO_UART);
						count++;
    			}while((statusFlags & kUART_RxDataRegFullFlag) && count<1);
    			//we only read a maxium of 1 byte, if we read more
    			//it breaks the board and we don't know why, probably because it messes up the fifo of the uart driver
    			//seems the kUART_RxDataRegFullFlag is bugged when the buffer is full
    			//also we can't change the buffer size with the API for some reason
 				if(statusFlags & (kUART_RxFifoOverflowFlag|kUART_RxFifoUnderflowFlag)){
					gModbusState = INX_MODBUS_STATE_STOP; //we have to do this to trigger tear down of the uart stack, it is the only way recover the serial
					DEMO_UART->C2 &= ~UART_C2_RE_MASK; // recommended to disable RE
					if(statusFlags & kUART_RxFifoOverflowFlag){
						LWIP_DEBUGF(DEBUG_MODBUS,("uartIrqHandler: overflow"));
					}else{
						LWIP_DEBUGF(DEBUG_MODBUS,("uartIrqHandler: underflow"));
					}
					//LWIP_DEBUGF(DEBUG_MODBUS,("uart bytes read=%d",count));
					DEMO_UART-> CFIFO |= UART_CFIFO_RXFLUSH_MASK;    //flush RX buffer
					DEMO_UART-> SFIFO |= (UART_SFIFO_RXOF_MASK|UART_SFIFO_RXUF_MASK);    //clear RXOF
					DEMO_UART->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK); //enable receive
				}
    		}
    		//xMBRTUReceiveFSM();

    	}

    }else if(kUART_TxDataRegEmptyFlag & statusFlags){
    	//LWIP_DEBUGF(DEBUG_MODBUS,("tx data empty, writing to clear"));
    	//UART_WriteByte(DEMO_UART, 0);
    	pxMBFrameCBTransmitterEmpty();
    }else{
    	LWIP_DEBUGF(DEBUG_MODBUS,("uartIrqHandler: unknown status %d",statusFlags));
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}


void inxIecUartTests(){
	gDoingUartDiagnostics = true;
	uart_config_t config;
	/*
	 * config.baudRate_Bps = 115200U;
	 * config.parityMode = kUART_ParityDisabled;
	 * config.stopBitCount = kUART_OneStopBit;
	 * config.txFifoWatermark = 0;
	 * config.rxFifoWatermark = 1;
	 * config.enableTx = false;
	 * config.enableRx = false;
	 */
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config.enableTx = true;
	config.enableRx = true;
	config.stopBitCount = kUART_OneStopBit;
	config.parityMode=kUART_ParityEven;

	UART_Init(DEMO_UART, &config, DEMO_UART_CLK_FREQ);
	/*LWIP_PLATFORM_DIAG(("S2=%d",DEMO_UART->S2));
	uint8_t update= ~UART_S2_MSBF(0);
	DEMO_UART->S2 &= update;
	//LWIP_PLATFORM_DIAG(("S2=%d update=%d",DEMO_UART->S2,update));*/

	/* Enable RX interrupt. */
	UART_EnableInterrupts(DEMO_UART, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
	EnableIRQ(DEMO_UART_IRQn);
	//uint32_t statusFlags = 0;
	bool reading = false;
	bool sending = false;
	//uint32_t count=0;
	//uint8_t data[2];
	while (1)
	{
		if(!sending && !reading){
			//go low on ptb2 to enable read
			GPIO_PortClear(GPIOB, 1U << 2);
			reading = true;
		}

		/*while(rxIndex!=txIndex){
			//statusFlags = UART_GetStatusFlags(DEMO_UART);
			//LWIP_PLATFORM_DIAG(("%d!=%d 0x%X",rxIndex,txIndex,statusFlags));
			if(!sending){
				reading = false;
				sending = true;

				//go high on ptb2 to enable read
				GPIO_PortSet(GPIOB, 1U << 2);

				//wait for transmit line to go high
				//this represents the 9 microseconds for the serial driver to come alive
				count = 0;
				while(count<2000){
					count++;
				}
			}

			UART_WriteBlocking(DEMO_UART, &demoRingBuffer[txIndex], 1);
			txIndex++;
			txIndex %= DEMO_RING_BUFFER_SIZE;

			//PBB we must do this or transmission breaks
			statusFlags=UART_GetStatusFlags(DEMO_UART);
			while(!(statusFlags & kUART_TransmissionCompleteFlag) || !(kUART_TxDataRegEmptyFlag & statusFlags)){
				//LWIP_PLATFORM_DIAG(("waiting for transmission to complete 0x%X",statusFlags));
				statusFlags=UART_GetStatusFlags(DEMO_UART);
			}
		}*/

		sending = false;
	}
	gDoingUartDiagnostics = false;
}

static bool gUARTEnabled=false;
void inxUARTDisable(){
	//it seems doing UART_Deinit breaks it permanently, no idea why
    //UART_Deinit(DEMO_UART);
	gUARTEnabled=false;
}

status_t inxUARTEnable(){
    status_t uartStatus=kStatus_Fail;
    uart_config_t config;

	if(gUARTBaudRate>0){
		if(gUARTEnabled){
			uartStatus=kStatus_Success;
		}else{
			UART_GetDefaultConfig(&config);
			config.baudRate_Bps = gUARTBaudRate;
			config.enableTx = true;
			config.enableRx = true;
			switch(gUARTStopBits){
				case 2:
					config.stopBitCount = kUART_TwoStopBit;
					break;
				default:
					config.stopBitCount = kUART_OneStopBit;
					break;
			}
			switch(gUARTParity){
				case 0:
					config.parityMode=kUART_ParityDisabled;
					break;
				case 1:
					config.parityMode=kUART_ParityOdd;
					break;
				default:
					config.parityMode=kUART_ParityEven;
					break;
			}
			/*switch(eParity){
				case MB_PAR_EVEN:
					config.parityMode=kUART_ParityEven;
					break;
				case MB_PAR_NONE:
					config.parityMode=kUART_ParityDisabled;
					break;
				case MB_PAR_ODD:
					config.parityMode=kUART_ParityOdd;
					break;
			}*/
			config.rxFifoWatermark = 1;
			//anything other than 0 seems to break receiving after a send
			config.txFifoWatermark = 0;
			uartStatus=UART_Init(DEMO_UART, &config, DEMO_UART_CLK_FREQ);
			gUARTEnabled=true;
		}
	}
    return uartStatus;
}

void inxUARTModbusDisable(){
    switch(gModbusState){
        case INX_MODBUS_STATE_INIT:
        case INX_MODBUS_STATE_RUN:
            gModbusState=INX_MODBUS_STATE_STOP;
        default:
            break;
    }
}

void inxUARTModbusEnable(){
    switch(gModbusState){
        case INX_MODBUS_STATE_STOPPED:
            gModbusState=INX_MODBUS_STATE_INIT;
        default:
            break;
    }
}