#ifndef INX_UART_H_
#define INX_UART_H_

#include "fsl_common.h"

void inxUARTDisable();
status_t inxUARTEnable();
void inxUARTModbusDisable();
void inxUARTModbusEnable();

#endif //INX_UART_H_