/*
 * inx_buttons.c
 *
 *  Created on: 6 Dec 2019
 *      Author: patrick
 */

#include "inx_buttons.h"
#include "inx_led.h"
#include "inx_iec.h"

volatile inxButtonState_t gButtonState=INX_BUTTON_STATE_DISPLAY_ERROR;

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
typedef enum {
	INX_BUTTON_NONE,
	INX_BUTTON_SET,
	INX_BUTTON_UP,
	INX_BUTTON_DOWN,
	INX_BUTTON_SET_DOWN,
	INX_BUTTON_SET_UP_DOWN,
	INX_BUTTON_10V,
	INX_BUTTON_MODE
} inxButton_t;

typedef enum {
	INX_BUTTON_IDLE,
	INX_BUTTON_TIMEOUT,
	INX_BUTTON_SET_SHORT_PRESS,
	INX_BUTTON_SET_LONG_PRESS,
	INX_BUTTON_SET_HELD,
	INX_BUTTON_UP_SHORT_PRESS,
	INX_BUTTON_UP_HELD,
	INX_BUTTON_DOWN_SHORT_PRESS,
	INX_BUTTON_DOWN_HELD,
	INX_BUTTON_SET_DOWN_LONG_PRESS,
	INX_BUTTON_SET_UP_DOWN_LONG_PRESS,
	INX_BUTTON_10V_SHORT_PRESS,
	INX_BUTTON_10V_LONG_PRESS,
	INX_BUTTON_10V_HELD,
	INX_BUTTON_MODE_SHORT_PRESS,
	INX_BUTTON_MODE_LONG_PRESS,
	INX_BUTTON_MODE_HELD
} inxButtonEvent_t;

extern inxPID_t gPIDs[];
float gTemporarySetPoint=0.0;
uint16_t gDisplayTemporaryTimerSeconds=0;
Schedule_Mode_t gButtonsTemporaryScheduleMode=SCHEDULE_MODE_MANUAL;
static uint8_t gMaxHoldTicks=CONFIG_BUTTONS_MAX_HOLD_TICKS;
static uint16_t gPressedTicks=0;
static uint8_t gHeldTicks=0;
static inxButton_t gPreviousButton=INX_BUTTON_NONE;
extern inxLedState_t gInxLed1RedState;
extern inxLedState_t gInxLed1GreenState;
extern bool gDisplayFlash; // todo  thee shouldn't be here....
extern bool gDisplay2Flash;
extern bool gDoReset;
extern volatile bool gSettingsUpdatedByUser;


/* Display / button state machine */

void inxButtonsGotoDisplayCurrentTemp(){
	gButtonState=INX_BUTTON_STATE_DISPLAY_TEMP;
}

void inxButtonsGotoDisplayCurrentTimer(){
	gButtonState=INX_BUTTON_STATE_DISPLAY_TIMER;
}

static void inxButtonsGotoDisplayTimerTemp(){
	gButtonState=INX_BUTTON_STATE_DISPLAY_TIMER_TEMPERATURE;
}

void inxButtonsGotoDisplayForMode(const Schedule_Mode_t scheduleMode){
	switch(scheduleMode){
		case SCHEDULE_MODE_TIMER_SET_POINT:
		case SCHEDULE_MODE_TIMER:
			switch(gPIDs[0].timerState){
				case INX_TIMER_STATE_GOING_TO_SET_POINT:
					inxButtonsGotoDisplayTimerTemp();
					break;
				default:
					inxButtonsGotoDisplayCurrentTimer();
					break;		
			}
			break;
		default:
			inxButtonsGotoDisplayCurrentTemp();
			break;
	}
}

/* Button press state machine event handling */
static void inxButtonsHandleEvent(volatile const inxButtonEvent_t event){
	//these should always be done no matter the state
	switch(event){
		case INX_BUTTON_SET_DOWN_LONG_PRESS:
			gDoReset=true;
			break;
		case INX_BUTTON_SET_UP_DOWN_LONG_PRESS:
			mainFactoryReset();
			break;
		default:
			break;
	}
}

bool inxGPIORead(const GPIO_Type* pBase,const uint32_t pin){
	volatile uint32_t reg=pBase->PDIR;
	return (~reg & (0x00000001U << pin));
}

static bool inxButtonPressed(const uint32_t pin){
	return inxGPIORead(GPIOC,pin);
}

static bool inxSetModeButtonPressed(const GPIO_Type* pBase,const uint32_t pin){
	return inxGPIORead(pBase,pin);
}

static bool inxButtons10VPressed(){
	//create a fake pid with which to get the value
	const float value=getConvertedValue(INX_CONTROLLER_SENSORS_10V,mainGetRawSensorValues());
	if(value>=CONFIG_BUTTONS_10V_THRESHOLD){
		return true;
	}
	return false;
}

void inxButtonsPoll(){
	inxButton_t currentButton = INX_BUTTON_NONE;
	//we should test multi long press stuff first
	//always use else if so that we only ever detect one kind of button press at a time
	//treat multi press buttons as just another button and detect them here
	if(inxSetModeButtonPressed(CONFIG_GPIO_BUTTON_SET_BASE,CONFIG_GPIO_BUTTON_SET_PIN) && inxButtonPressed(CONFIG_GPIO_BUTTON_DOWN_PIN) && inxButtonPressed(CONFIG_GPIO_BUTTON_UP_PIN)){
		currentButton = INX_BUTTON_SET_UP_DOWN;
	}else if(inxSetModeButtonPressed(CONFIG_GPIO_BUTTON_SET_BASE,CONFIG_GPIO_BUTTON_SET_PIN) && inxButtonPressed(CONFIG_GPIO_BUTTON_DOWN_PIN)){
		currentButton = INX_BUTTON_SET_DOWN;
	}else if(inxButtonPressed(CONFIG_GPIO_BUTTON_UP_PIN)){
		currentButton = INX_BUTTON_UP;
	}else if(inxButtonPressed(CONFIG_GPIO_BUTTON_DOWN_PIN)){
		currentButton = INX_BUTTON_DOWN;
	}else if(inxSetModeButtonPressed(CONFIG_GPIO_BUTTON_SET_BASE,CONFIG_GPIO_BUTTON_SET_PIN)){
		currentButton = INX_BUTTON_SET;
	#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
		}else if(inxSetModeButtonPressed(CONFIG_GPIO_BUTTON_MODE_BASE,CONFIG_GPIO_BUTTON_MODE_PIN)){
			currentButton=INX_BUTTON_MODE;
	#endif //INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	}else if(inxButtons10VPressed()){
		currentButton = INX_BUTTON_10V;
	}

	if(currentButton!=gPreviousButton){
		switch(gPreviousButton){
			case INX_BUTTON_SET_DOWN:
				if(gPressedTicks>=CONFIG_BUTTONS_LONG_PRESS_TICKS){
					inxButtonsHandleEvent(INX_BUTTON_SET_DOWN_LONG_PRESS);
				}
				break;
			case INX_BUTTON_SET_UP_DOWN:
				if(gPressedTicks>=CONFIG_BUTTONS_LONG_PRESS_TICKS){
					inxButtonsHandleEvent(INX_BUTTON_SET_UP_DOWN_LONG_PRESS);
				}
				break;
			default:
				switch(currentButton){
					case INX_BUTTON_NONE:
						//we have changed what button is held so detect short or long presses
						if(gPressedTicks>=CONFIG_BUTTONS_LONG_PRESS_TICKS){
							//long presses
							switch(gPreviousButton){
								case INX_BUTTON_SET:
									inxButtonsHandleEvent(INX_BUTTON_SET_LONG_PRESS);
									break;
								case INX_BUTTON_SET_DOWN:
									inxButtonsHandleEvent(INX_BUTTON_SET_DOWN_LONG_PRESS);
									break;
								case INX_BUTTON_SET_UP_DOWN:
									inxButtonsHandleEvent(INX_BUTTON_SET_UP_DOWN_LONG_PRESS);
									break;
								case INX_BUTTON_10V:
									inxButtonsHandleEvent(INX_BUTTON_10V_LONG_PRESS);
									break;
								case INX_BUTTON_MODE:
									inxButtonsHandleEvent(INX_BUTTON_MODE_LONG_PRESS);
									break;
								default:
									break;
							}
						//we test gHeldTicks to prevent a short press event after a held event
						}else if(gPressedTicks>1 && gHeldTicks==0){
							//short presses
							switch(gPreviousButton){
								case INX_BUTTON_UP:
									inxButtonsHandleEvent(INX_BUTTON_UP_SHORT_PRESS);
									break;
								case INX_BUTTON_DOWN:
									inxButtonsHandleEvent(INX_BUTTON_DOWN_SHORT_PRESS);
									break;
								case INX_BUTTON_SET:
									inxButtonsHandleEvent(INX_BUTTON_SET_SHORT_PRESS);
									break;
								case INX_BUTTON_10V:
									inxButtonsHandleEvent(INX_BUTTON_10V_SHORT_PRESS);
									break;
								case INX_BUTTON_MODE:
									inxButtonsHandleEvent(INX_BUTTON_MODE_SHORT_PRESS);
									break;
								default:
									break;
							}
						}
						break;
					default:
						break;
				}
				break;
		}

		gPressedTicks=0;
	}

	//deal with things we do before a release
	switch(currentButton){
		case INX_BUTTON_NONE:
			gPressedTicks++;
			gMaxHoldTicks=CONFIG_BUTTONS_MAX_HOLD_TICKS;
			if(gPressedTicks>CONFIG_BUTTONS_TIMEOUT_TICKS){
				inxButtonsHandleEvent(INX_BUTTON_TIMEOUT);
				gPressedTicks=0;
			}else if(gPressedTicks>CONFIG_BUTTONS_IDLE_TICKS){
				inxButtonsHandleEvent(INX_BUTTON_IDLE);
				//leave the pressed ticks so we get a timeout later
			}
			break;
		case INX_BUTTON_SET_UP_DOWN:
		case INX_BUTTON_SET_DOWN:
			if(gPressedTicks<CONFIG_BUTTONS_LONG_PRESS_TICKS){
				gPressedTicks++;
			}
			break;
		case INX_BUTTON_SET:
		case INX_BUTTON_UP:
		case INX_BUTTON_DOWN:
		case INX_BUTTON_10V:
		case INX_BUTTON_MODE:
			if(gPressedTicks<=50){
				gHeldTicks=0;
				gMaxHoldTicks=CONFIG_BUTTONS_MAX_HOLD_TICKS;
				gPressedTicks=gPressedTicks+1;
			}else{
				//already at long press, this would probably trigger repeat button presses
				gHeldTicks++;
				if(gHeldTicks>=gMaxHoldTicks){
					//TODO make this work for SET_DOWN so that reset happens while held
					switch(currentButton){
						case INX_BUTTON_UP:
							inxButtonsHandleEvent(INX_BUTTON_UP_HELD);
							break;
						case INX_BUTTON_DOWN:
							inxButtonsHandleEvent(INX_BUTTON_DOWN_HELD);
							break;
						case INX_BUTTON_SET:
							inxButtonsHandleEvent(INX_BUTTON_SET_HELD);
							break;
						case INX_BUTTON_10V:
							inxButtonsHandleEvent(INX_BUTTON_10V_HELD);
							break;
						case INX_BUTTON_MODE:
							inxButtonsHandleEvent(INX_BUTTON_MODE_HELD);
							break;
						default:
							break;
					}
					gHeldTicks=0;
				}
			}
			break;
		default:
			break;
	}
	gPreviousButton=currentButton;
	//above code generates inxButtonEvent_t
	//pass that in to inxButtonsHandleEvent
	//that maintains the state machine for the button UI
}

extern bool gDiagnosticBoot;
void inxButtonsDetectDiagnosticBoot(){
#if CONFIG_ALWAYS_DIAGNOSTIC_BOOT
	gDiagnosticBoot=true;
#else
	gDiagnosticBoot=false;
	if(inxButtonPressed(CONFIG_GPIO_BUTTON_UP_PIN) && inxButtonPressed(CONFIG_GPIO_BUTTON_DOWN_PIN)){
		gDiagnosticBoot=true;
	}
#endif //CONFIG_ALWAYS_DIAGNOSTIC_BOOT
}

#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
void inxDiagnosticWaitLongTime(int i);// defined in main

#define INX_HW_DAIG_BUTTON_TIMEOUT 20
void inxButtonsHWDiagnosticTest() {
	int i;
	HW_DIAG_printf("Key Test\n___________________\nPress the SET Key...");
	inxDiagnosticWaitLongTime(2);
	for  (i=0; i< INX_HW_DAIG_BUTTON_TIMEOUT && !inxSetButtonPressed(CONFIG_GPIO_BUTTON_SET_PIN);i++) {
		HW_DIAG_printf("%i,", INX_HW_DAIG_BUTTON_TIMEOUT-i);
		inxDiagnosticWaitLongTime(2);
	}
	i==10?HW_DIAG_printf(" No Press!!!\n"):HW_DIAG_printf(" Press Detected\n");
	HW_DIAG_printf("Press the UP Key...");
	for  (i=0; i< INX_HW_DAIG_BUTTON_TIMEOUT && !inxButtonPressed(CONFIG_GPIO_BUTTON_UP_PIN) ;i++) {
		HW_DIAG_printf("%i,", INX_HW_DAIG_BUTTON_TIMEOUT-i);
		inxDiagnosticWaitLongTime(2);
	}
	i==10?HW_DIAG_printf(" No Press!!!\n"):HW_DIAG_printf(" Press Detected\n");
	HW_DIAG_printf("Press the DOWN Key...");
	for  (i=0; i< INX_HW_DAIG_BUTTON_TIMEOUT && !inxButtonPressed(CONFIG_GPIO_BUTTON_DOWN_PIN) ;i++) {
		HW_DIAG_printf("%i,", INX_HW_DAIG_BUTTON_TIMEOUT-i);
		inxDiagnosticWaitLongTime(2);
	}
	i==10?HW_DIAG_printf(" No Press!!!\n"):HW_DIAG_printf(" Press Detected\n");
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	HW_DIAG_printf("Press the MODE Key...");
	for  (i=0; i< INX_HW_DAIG_BUTTON_TIMEOUT && !inxButtonPressed(CONFIG_GPIO_BUTTON_RUNSTOP_PIN) ;i++) {
		HW_DIAG_printf("%i,", INX_HW_DAIG_BUTTON_TIMEOUT-i);
		inxDiagnosticWaitLongTime(2);
	}
	i==10?HW_DIAG_printf(" No Press!!!\n"):HW_DIAG_printf(" Press Detected\n");
	HW_DIAG_printf("___________________\n");
#endif
}

#endif

#endif //INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
