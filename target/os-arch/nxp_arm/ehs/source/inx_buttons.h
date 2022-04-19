/*
 * inx_buttons.h
 *
 *  Created on: 6 Dec 2019
 *      Author: patrick
 */

#ifndef INX_BUTTONS_H_
#define INX_BUTTONS_H_

#include "heatrod_config.h"

typedef enum {
	INX_BUTTON_STATE_DISPLAY_TEMP,
	INX_BUTTON_STATE_SET_SET_POINT,
	INX_BUTTON_STATE_DISPLAY_MODE,
	INX_BUTTON_STATE_SET_TIMER,
	INX_BUTTON_STATE_DISPLAY_TIMER,
	INX_BUTTON_STATE_DISPLAY_TIMER_TEMPERATURE,
	INX_BUTTON_STATE_DISPLAY_ERROR,
	INX_BUTTON_STATE_DISPLAY_UNDEFINED
} inxButtonState_t;

void inxButtonsDetectDiagnosticBoot();
void inxButtonsPoll();
void inxButtonsGotoDisplayCurrentTemp();
void inxButtonsGotoDisplayCurrentTimer();
bool inxGPIORead(const GPIO_Type* pBase,const uint32_t pin);
#endif /* INX_BUTTONS_H_ */
