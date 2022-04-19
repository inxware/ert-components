/*
 * inx_led.h
 *
 *  Created on: 8 Nov 2019
 *      Author: patrick
 */

#ifndef INX_LED_H_
#define INX_LED_H_

typedef enum {
	INX_LED_GREEN_1,
	INX_LED_GREEN_2,
	INX_LED_RED_1,
	INX_LED_RED_2
} inxLed_t;

typedef enum {
	INX_LED_COLOUR_OFF,
	INX_LED_COLOUR_RED,
	INX_LED_COLOUR_GREEN,
	INX_LED_COLOUR_YELLOW
} inxLedColour_t;

typedef enum {
	INX_LED_DUTY_MODE_OFF,
	INX_LED_DUTY_MODE_ON
} inxLedDutyMode_t;

typedef enum {
	INX_LED_HEATING_MODE_MANUAL,
	INX_LED_HEATING_MODE_SCHEDULE,
	INX_LED_HEATING_MODE_TIMER
} inxLedHeatingMode_t;

typedef enum {
	INX_LED_NETWORK_MODE_DISABLED,
	INX_LED_NETWORK_MODE_LOCAL,
	INX_LED_NETWORK_MODE_CLOUD
} inxLedNetworkMode_t;

typedef enum {
	INX_LED_MODE_OFF,
	INX_LED_MODE_ON
} inxLedMode_t;

typedef struct _inxLedState
{
	uint32_t cycleLengthTicks;
	uint32_t ticks;
    bool on;
    uint8_t pin;
	inxLedMode_t mode;
} inxLedState_t;

void inxLedBootTest();
void inxLedDutyModeSet(const inxLedDutyMode_t mode);
void inxLedHeatingModeSet(const inxLedHeatingMode_t mode);
void inxLedNetworkModeSet(const inxLedNetworkMode_t mode);
void inxLedTick(const bool provisioned);
void inxLedStateToggle(inxLedState_t* pState);

void inxLedTest();

#endif /* INX_LED_H_ */
