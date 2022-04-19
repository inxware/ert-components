/*
 * inx_led.c
 *
 *  Created on: 8 Nov 2019
 *      Author: patrick
 */

#include "heatrod_config.h"
#include "inx_led.h"
#include "fsl_gpio.h"

/* Only use this in boot test code to avoid locking processor down too long */
static void inxLedDelay(int i){
	uint32_t count=0;
	while(count<(2048000*i)){
		count++;
	}
}

static void inxLedPinOff(const uint8_t pin){
#if  INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	GPIO_PortClear(CONFIG_GPIO_PROCBOARDLEDS_BASE,1U << pin);
#endif
}


static void inxLedPinOn(const uint8_t pin){
#if  INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	GPIO_PortSet(CONFIG_GPIO_PROCBOARDLEDS_BASE, 1U << pin);
#endif
}

/*
typedef struct _inxLedState
{
	uint32_t cycleLengthTicks;
	uint32_t ticks;
    bool on;
    uint8_t pin;
} inxLedState_t;*/

inxLedState_t gInxLedGreen1State={
	0,0,0,CONFIG_GPIO_PROCBOARDLED1_GRN_PIN,INX_LED_MODE_OFF
};
inxLedState_t gInxLedGreen2State={
	0,0,0,CONFIG_GPIO_PROCBOARDLED2_GRN_PIN,INX_LED_MODE_OFF
};
inxLedState_t gInxLedRed1State={
	0,0,0,CONFIG_GPIO_PROCBOARDLED1_RED_PIN,INX_LED_MODE_OFF
};
inxLedState_t gInxLedRed2State={
	0,0,0,CONFIG_GPIO_PROCBOARDLED2_RED_PIN,INX_LED_MODE_OFF
};
static void inxLedSet(const inxLedState_t* pLed){
	/*switch(pLed->mode){
		case INX_LED_MODE_OFF:
			inxLedPinOff(pLed->pin);
			break;
		default:
			if(pLed->on){
				inxLedPinOn(pLed->pin);
			}else{
				inxLedPinOff(pLed->pin);
			}
			break;
	}*/
}

void inxLedDutyModeSet(const inxLedDutyMode_t mode){
	LWIP_DEBUGF(DEBUG_LED,("inxLedDutyModeSet: mode=%d",mode));
	gInxLedRed1State.mode=INX_LED_MODE_ON;
	switch(mode){
		case INX_LED_DUTY_MODE_OFF:
			gInxLedRed1State.on=false;
			break;
		case INX_LED_DUTY_MODE_ON:
			gInxLedRed1State.on=true;
			break;
		default:
			LWIP_DEBUGF(DEBUG_LED,("inxLedDutyModeSet: unrecognised mode"));
			break;
	}
	inxLedSet(&gInxLedRed1State);
}

void inxLedHeatingModeSet(const inxLedHeatingMode_t mode){
	LWIP_DEBUGF(DEBUG_LED,("inxLedHeatingModeSet: mode=%d",mode));
	switch(mode){
		case INX_LED_HEATING_MODE_MANUAL:
			gInxLedGreen1State.on=false;
			gInxLedGreen1State.mode=INX_LED_MODE_OFF;
			gInxLedGreen1State.cycleLengthTicks=0;
			break;
		case INX_LED_HEATING_MODE_SCHEDULE:
			gInxLedGreen1State.on=true;
			gInxLedGreen1State.mode=INX_LED_MODE_ON;
			gInxLedGreen1State.cycleLengthTicks=0;
			break;
		case INX_LED_HEATING_MODE_TIMER:
			gInxLedGreen1State.mode=INX_LED_MODE_ON;
			gInxLedGreen1State.cycleLengthTicks=CONFIG_LED_TIMER_FLASH_RATE_MILLISECONDS/CONFIG_NETWORK_TASK_POLL_DELAY;
			break;
		default:
			LWIP_DEBUGF(DEBUG_LED,("inxLedHeatingModeSet: unrecognised mode"));
			break;
	}
}

void inxLedNetworkModeSet(const inxLedNetworkMode_t mode){
	LWIP_DEBUGF(DEBUG_LED,("inxLedNetworkModeSet: mode=%d",mode));
	switch(mode){
		case INX_LED_NETWORK_MODE_DISABLED:
			//red
			switch(gInxLedRed2State.mode){
				case INX_LED_MODE_ON:
					break;
				default:
					gInxLedRed2State.on=true;
					gInxLedRed2State.mode=INX_LED_MODE_ON;
					gInxLedRed2State.ticks=0;
					break;
			}
			gInxLedGreen2State.mode=INX_LED_MODE_OFF;
			break;
		case INX_LED_NETWORK_MODE_LOCAL:
			//yellow
			switch(gInxLedRed2State.mode){
				case INX_LED_MODE_ON:
					break;
				default:
					//copy state of the other led so we end up in sync
					gInxLedRed2State.on=gInxLedGreen2State.on;
					gInxLedRed2State.mode=INX_LED_MODE_ON;
					gInxLedRed2State.ticks=gInxLedGreen2State.ticks;
					break;
			}
			switch(gInxLedGreen2State.mode){
				case INX_LED_MODE_ON:
					break;
				default:
					//copy state of the other led so we end up in sync
					gInxLedGreen2State.on=gInxLedRed2State.on;
					gInxLedGreen2State.mode=INX_LED_MODE_ON;
					gInxLedGreen2State.ticks=gInxLedRed2State.ticks;
					break;
			}
			break;
		case INX_LED_NETWORK_MODE_CLOUD:
			//green
			switch(gInxLedRed2State.mode){
				case INX_LED_MODE_OFF:
					break;
				default:
					gInxLedRed2State.on=false;
					gInxLedRed2State.mode=INX_LED_MODE_OFF;
					break;
			}
			switch(gInxLedGreen2State.mode){
				case INX_LED_MODE_ON:
					break;
				default:
					gInxLedGreen2State.on=true;
					gInxLedGreen2State.mode=INX_LED_MODE_ON;
					gInxLedGreen2State.ticks=0;
					break;
			}
			break;
		default:
			LWIP_DEBUGF(DEBUG_LED,("inxLedNetworkModeSet: unrecognised mode"));
			break;
	}
}

void inxLedStateToggle(inxLedState_t* pState){
	pState->on=!pState->on;
	inxLedSet(pState);
}

static void inxLedStateTick(inxLedState_t* pState){
	if(pState->cycleLengthTicks>0){
		pState->ticks++;
		if(pState->ticks>=pState->cycleLengthTicks){
			pState->ticks=0;
			inxLedStateToggle(pState);
		}
	}else{
		inxLedSet(pState);
	}
}

void inxLedTick(const bool provisioned){
	gInxLedGreen2State.cycleLengthTicks=0;
	gInxLedRed2State.cycleLengthTicks=0;
	if(provisioned){
		//we have to do this in case we got provisioned while the LEDs were off
		if(gInxLedGreen2State.mode==INX_LED_MODE_ON){
			gInxLedGreen2State.on=true;
		}
		if(gInxLedRed2State.mode==INX_LED_MODE_ON){
			gInxLedRed2State.on=true;
		}
	}else{
		if(gInxLedGreen2State.mode==INX_LED_MODE_ON){
			gInxLedGreen2State.cycleLengthTicks=CONFIG_LED_PROVISIONED_FLASH_RATE_MILLISECONDS/CONFIG_NETWORK_TASK_POLL_DELAY;
		}
		if(gInxLedRed2State.mode==INX_LED_MODE_ON){
			gInxLedRed2State.cycleLengthTicks=CONFIG_LED_PROVISIONED_FLASH_RATE_MILLISECONDS/CONFIG_NETWORK_TASK_POLL_DELAY;
		}
	}
	inxLedStateTick(&gInxLedRed1State);
	inxLedStateTick(&gInxLedGreen1State);
	inxLedStateTick(&gInxLedRed2State);
	inxLedStateTick(&gInxLedGreen2State);
}

static void inxLedWaitLongTime(){
	volatile uint32_t count=0;
	while(count<10000000){
		count++;
	}
}

/* Requires button press through */
void inxLedTest() {
	HW_DIAG_printf("_________________________________\n");
	HW_DIAG_printf("Flashing LEDS Red->Green:");
	for (int i= 0 ; i< 2;i++) {
		HW_DIAG_printf("%d,",i);
		inxLedPinOn(CONFIG_GPIO_PROCBOARDLED1_RED_PIN);
		inxLedWaitLongTime();
		inxLedPinOff(CONFIG_GPIO_PROCBOARDLED1_RED_PIN);
		inxLedWaitLongTime();
		inxLedPinOn(CONFIG_GPIO_PROCBOARDLED1_GRN_PIN);
		inxLedWaitLongTime();
		inxLedPinOff(CONFIG_GPIO_PROCBOARDLED1_GRN_PIN);
		inxLedWaitLongTime();
		inxLedPinOn(CONFIG_GPIO_PROCBOARDLED2_RED_PIN);
		inxLedWaitLongTime();
		inxLedPinOff(CONFIG_GPIO_PROCBOARDLED2_RED_PIN);
		inxLedWaitLongTime();
		inxLedPinOn(CONFIG_GPIO_PROCBOARDLED2_GRN_PIN);
		inxLedWaitLongTime();
		inxLedPinOff(CONFIG_GPIO_PROCBOARDLED2_GRN_PIN);
		inxLedWaitLongTime();
	}
	HW_DIAG_printf("\n_________________________________\n");
}

static void inxLedSetAll(){
	inxLedSet(&gInxLedGreen1State);
	inxLedSet(&gInxLedRed1State);
	inxLedSet(&gInxLedGreen2State);
	inxLedSet(&gInxLedRed2State);
}

void inxLedBootTest(){
	SW_DIAG_printf(DEBUG_LED,("inxLedBootTest: off\n"));
	gInxLedGreen1State.mode=INX_LED_MODE_ON;
	gInxLedGreen2State.mode=INX_LED_MODE_ON;
	gInxLedRed1State.mode=INX_LED_MODE_ON;
	gInxLedRed2State.mode=INX_LED_MODE_ON;
	gInxLedGreen1State.on=false;
	gInxLedRed1State.on=false;
	gInxLedGreen2State.on=false;
	gInxLedRed2State.on=false;
	inxLedSetAll();
	inxLedDelay(1);

	SW_DIAG_printf(DEBUG_LED,("inxLedBootTest: red 1\n"));
	gInxLedRed1State.on=true;
	inxLedSetAll();
	inxLedDelay(1);
	SW_DIAG_printf(DEBUG_LED,("inxLedBootTest: green 1\n"));
	gInxLedRed1State.on=false;
	gInxLedGreen1State.on=true;
	inxLedSetAll();
	inxLedDelay(1);
	SW_DIAG_printf(DEBUG_LED,("inxLedBootTest: yellow 1\n"));
	gInxLedRed1State.on=true;
	gInxLedGreen1State.on=true;
	inxLedSetAll();
	inxLedDelay(1);
	SW_DIAG_printf(DEBUG_LED,("inxLedBootTest: off\n"));
	gInxLedRed1State.on=false;
	gInxLedGreen1State.on=false;
	inxLedSetAll();

	SW_DIAG_printf(DEBUG_LED,("inxLedBootTest: red 2\n"));
	gInxLedRed2State.on=true;
	inxLedSetAll();
	inxLedDelay(1);
	SW_DIAG_printf(DEBUG_LED,("inxLedBootTest: green 2\n"));
	gInxLedRed2State.on=false;
	gInxLedGreen2State.on=true;
	inxLedSetAll();
	inxLedDelay(1);
	SW_DIAG_printf(DEBUG_LED,("inxLedBootTest: yellow 2\n"));
	gInxLedRed2State.on=true;
	gInxLedGreen2State.on=true;
	inxLedSetAll();
	inxLedDelay(1);
	SW_DIAG_printf(DEBUG_LED,("inxLedBootTest: off\n"));
	gInxLedRed2State.on=false;
	gInxLedGreen2State.on=false;
	inxLedSetAll();
	inxLedDelay(1);

	HW_DIAG_printf(DEBUG_LED,("inxLedBootTest: heating mode schedule\n"));
	inxLedHeatingModeSet(INX_LED_HEATING_MODE_SCHEDULE);
	inxLedDelay(5);

	HW_DIAG_printf(DEBUG_LED,("inxLedBootTest: heating mode manual\n"));
	inxLedHeatingModeSet(INX_LED_HEATING_MODE_MANUAL);
	inxLedDelay(5);

	HW_DIAG_printf(DEBUG_LED,("inxLedBootTest: network mode disabled\n"));
	inxLedNetworkModeSet(INX_LED_NETWORK_MODE_DISABLED);
	inxLedDelay(5);

	HW_DIAG_printf(DEBUG_LED,("inxLedBootTest: network mode local\n"));
	inxLedNetworkModeSet(INX_LED_NETWORK_MODE_LOCAL);
	inxLedDelay(5);

	HW_DIAG_printf(DEBUG_LED,("inxLedBootTest: network mode cloud\n"));
	inxLedNetworkModeSet(INX_LED_NETWORK_MODE_CLOUD);
	inxLedDelay(5);

	SW_DIAG_printf(DEBUG_LED,("inxLedBootTest: off\n"));
	gInxLedGreen1State.on=false;
	gInxLedRed1State.on=false;
	gInxLedGreen2State.on=false;
	gInxLedRed2State.on=false;
	gInxLedGreen1State.mode=INX_LED_MODE_OFF;
	gInxLedGreen2State.mode=INX_LED_MODE_OFF;
	gInxLedRed1State.mode=INX_LED_MODE_OFF;
	gInxLedRed2State.mode=INX_LED_MODE_OFF;
	inxLedSetAll();
}

