/*
 * inx_display.c
 *
 *  Created on: 6 Dec 2019
 *      Author: patrick
 */

#include "inx_display.h"
#include "inx_buttons.h"
#include "errorLogs.h"
#include "fsl_gpio.h"

static uint8_t gDisplayTicks=0;
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
static uint8_t gDisplay2Ticks=0;
#endif

static uint8_t gDisplay1Chars[CONFIG_DISPLAY_NUM_CHARS];
static uint32_t gDisplay1CharsPins[CONFIG_DISPLAY_NUM_CHARS]={CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR1,CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR2,CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR3,CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR4};
static uint8_t gDisplay1CurrentChar=0;

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
static uint8_t gDisplay2Chars[CONFIG_DISPLAY_NUM_CHARS];
static uint32_t gDisplay2CharsPins[CONFIG_DISPLAY_NUM_CHARS]={CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR1,CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR2,CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR3,CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR4};
static uint8_t gDisplay2CurrentChar=0;
#endif


void inxDisplaySetChar(const uint8_t displayNo,const uint8_t index,uint8_t value){
	//printf("PBB d=%d c=%d v=%d\n",displayNo,index,value);
	uint8_t* _DisplayChars = gDisplay1Chars;
	#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
		if(displayNo>0){
			_DisplayChars=gDisplay2Chars;
		}
	#endif //if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	if(index<CONFIG_DISPLAY_NUM_CHARS){
		if(value>9){
			value=15;
		}
		_DisplayChars[index]=value;
	}
}

void inxDisplaySetFromTemperature(float temperature,const uint8_t displayNo){

	uint8_t* _DisplayChars = gDisplay1Chars;
	//LWIP_PLATFORM_DIAG(("temperature=%f displayNo=%d",temperature,displayNo));
	if (displayNo == 1) {
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
		_DisplayChars = gDisplay2Chars;
		//LWIP_PLATFORM_DIAG(("chose display 2"));
#else
		_DisplayChars = gDisplay1Chars;
#endif
	}

	if(temperature>=0.0){
		temperature=temperature+0.05; //we add 0.5 so that our integer conversion behaves like a standard round to nearest
	}else{
		temperature=temperature-0.05; //we add 0.5 so that our integer conversion behaves like a standard round to nearest
	}
	//printf("inxDIP=%f,%d\n",temperature,displayNo);
	int integer=(int)(temperature);
	int decimal=(int)(temperature*10)%10;
	LWIP_DEBUGF(DEBUG_BUTTONS,("inxButtons:temperature=%2.2f integer=%d decimal=%d",temperature,integer,decimal));
	if(temperature<0){
		temperature=temperature*-1; //turn it positive so we get correct numbers
		_DisplayChars[0]=0xE; //should be a dash to signify negative
		integer=(int)temperature;
		decimal=(int)(temperature*10)%10;
	}else{
		_DisplayChars[0] = integer%1000/100; // don't really need the od here.. but should show
		//blank first character if 0
		if (_DisplayChars[0] == 0){
			_DisplayChars[0] = 0xF;
		}
	}

	//blank the number if temperature less than 10
	if(temperature>0.0 && temperature<10.0){
		_DisplayChars[1]=0xF;
	}else{
		_DisplayChars[1] = (integer%100)/10;
	}
	_DisplayChars[2] = (integer%10);
	_DisplayChars[3] = decimal;
}

extern volatile inxButtonState_t gButtonState;
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
extern float gTemporarySetPoint; // todo we should get rid of this
extern uint16_t gDisplayTemporaryTimerSeconds;
extern Schedule_Mode_t gButtonsTemporaryScheduleMode;
#endif
bool gDisplayFlash=false;
bool gDisplay2Flash=false;
/* display one state variables */
static uint8_t gDisplayNumTicksOffLimit=CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_OFF_LIMIT;
static uint8_t gDisplayNumTicksOnLimit=CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_ON_LIMIT;
static uint8_t gDisplayFlashTicks=0;
/* display two  state variables */
static uint8_t gDisplay2NumTicksOffLimit=CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_OFF_LIMIT;
static uint8_t gDisplay2NumTicksOnLimit=CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_ON_LIMIT;
static uint8_t gDisplay2FlashTicks=0;
extern inxPID_t gPIDs[];
extern errorLogs_t gErrorLogs;

void inxDisplaySetForScheduleMode(const Schedule_Mode_t scheduleMode){
	switch(scheduleMode){
		case SCHEDULE_MODE_TIMER:
			inxButtonsGotoDisplayCurrentTimer();
			break;
		default:
			inxButtonsGotoDisplayCurrentTemp();
			break;
	}
}

void inxDisplay1NegativeOn(){
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	//we need to indicate the display should flash
	//PBB 20200603 disabled for now as it seems to be causing loads of issues
	//gDisplayNumTicksOffLimit=CONFIG_INX_DISPLAY_NEGATIVE_NUM_TICKS_OFF_LIMIT;
	//gDisplayNumTicksOnLimit=CONFIG_INX_DISPLAY_NEGATIVE_NUM_TICKS_ON_LIMIT;
	//gDisplayFlash=true;
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	
#ifdef INX_FIXED_NEGATIVE_DISPLAY_HARDWARE
	//waiting for new hardware
	//we need to turn on the negative pin
	GPIO_PortSet(CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_BASE,(1U << CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN));
#else
	gDisplayFlash=true;
#endif
#endif
}

void inxDisplay1NegativeOff(){
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	//we need to indicate the display should flash
	//PBB 20200603 disabled for now as it seems to be causing loads of
	//gDisplayFlash=false;
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	//we need to turn on the negative pin
	GPIO_PortClear(CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_BASE,(1U << CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN));
#endif
}

void inxDisplay1Poll(const bool ignoreErrors,bool* done){
	//const
	uint8_t currChar=gDisplay1Chars[gDisplay1CurrentChar];
	if(gDisplayTicks==0){
		if(gDisplay1CurrentChar==0){
			//check our error flags, if any any are set then display that error, prevent display from being used if an error is present
			if(ignoreErrors==true){

			}else{
				//inxDisplayClearError();
				for(errorLogsError_t i=0;i<ERROR_LOGS_DISPLAY_BOUNDARY;i++){
					if(gErrorLogs.errors[i].unixTime>0){
						//inxDisplaySetErrorCode(i,true);
						//only display the first error we come to
						break;
					}
				}
			}

			//update what value we are going to display
			switch(gButtonState){
				case INX_BUTTON_STATE_DISPLAY_UNDEFINED:
					//inxDisplay1NegativeOff();
					gDisplayNumTicksOffLimit=CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_OFF_LIMIT;
					gDisplayNumTicksOnLimit=CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_ON_LIMIT;
					gDisplayFlash=true;
					//inxDisplaySetFromTemperature(888.8,0);
					break;
			#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
				case INX_BUTTON_STATE_DISPLAY_TIMER:
				case INX_BUTTON_STATE_SET_TIMER:
				case INX_BUTTON_STATE_DISPLAY_MODE:
				case INX_BUTTON_STATE_SET_SET_POINT:
				case INX_BUTTON_STATE_DISPLAY_ERROR:
			#endif
				case INX_BUTTON_STATE_DISPLAY_TIMER_TEMPERATURE:
				case INX_BUTTON_STATE_DISPLAY_TEMP:
					gDisplayFlash=false;
					//inxDisplaySetFromTemperature(gPIDs[0].currentValue,0);
					// todo the following negative number bit needs to be moved into the number polling function which should decide how to resent negative numbers
					if(gPIDs[0].currentValue<0.0){
						//inxDisplay1NegativeOn();
					}else{
						//inxDisplay1NegativeOff();
					}
					break;
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi // we only have one display on HRC so we do the setting stuff instead of current temperature
				case INX_BUTTON_STATE_DISPLAY_TIMER:
					gDisplayFlash=false;
					//inxDisplaySetFromSeconds(0,gPIDs[0].timerSecondsRemaining,true);
					break;
				case INX_BUTTON_STATE_SET_TIMER:
					//inxDisplaySetFromSeconds(0,gDisplayTemporaryTimerSeconds,true);
					break;
				case INX_BUTTON_STATE_DISPLAY_MODE:
					gDisplayFlash=false;
					//inxDisplaySet(0xF,0xF,gButtonsTemporaryScheduleMode,0xF,true);
					break;
				case INX_BUTTON_STATE_SET_SET_POINT:
					//inxDisplaySetFromTemperature(gTemporarySetPoint,0);
					if(gTemporarySetPoint<0.0){
						//inxDisplay1NegativeOn();
					}else{
						//inxDisplay1NegativeOff();
					}
					break;
				case INX_BUTTON_STATE_DISPLAY_ERROR:
					//inxDisplay1NegativeOff();
					gDisplayNumTicksOffLimit=CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_OFF_LIMIT;
					gDisplayNumTicksOnLimit=CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_ON_LIMIT;
					gDisplayFlash=true;
					//here we should take whatever the current error code for the display is and use that
					//inxDisplaySet(8,8,8,8,true);
					break;
#endif // end of HRi conditional code
				default:
					break;
			}
		}

		if(gDisplayFlash){
			gDisplayFlashTicks++;
			if(gDisplayFlashTicks<gDisplayNumTicksOffLimit){
				//display blank
				switch(gButtonState){
					case INX_BUTTON_STATE_DISPLAY_ERROR:
						//only blank the last char when in error mode
						//gDisplay1Chars[CONFIG_DISPLAY_NUM_CHARS-1]=0xF;
						break;
					default:
						//blank all of them
						for(uint8_t i=0;i<CONFIG_DISPLAY_NUM_CHARS;i++){
							//gDisplay1Chars[i]=0xF;
						}
						break;
				}
			}else if(gDisplayFlashTicks>=gDisplayNumTicksOnLimit){
				gDisplayFlashTicks=0;
			}
		}else{ // need to make sure the display is actually on when flashing is stopped as we can get blank screens sometimes otherwise.
			gDisplayFlashTicks=0;
		}

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_FRDMK64EVAL
#else
		//start writing a number to a char
		(currChar & 0x01)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A));
		(currChar & 0x02)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B));
		(currChar & 0x04)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C));
		(currChar & 0x08)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D));
		GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << gDisplay1CharsPins[gDisplay1CurrentChar]));
		//LWIP_PLATFORM_DIAG(("wrote char %d %d",gCurrentChar,currChar));
#endif

	}
	gDisplayTicks++;
	if(gDisplayTicks>1){
		//finish writing the number
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_FRDMK64EVAL
#else
		//start writing a number to a char
		GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << gDisplay1CharsPins[gDisplay1CurrentChar]));
#endif


		gDisplayTicks=0;
		gDisplay1CurrentChar++;
		if(gDisplay1CurrentChar>=CONFIG_DISPLAY_NUM_CHARS){
			gDisplay1CurrentChar=0;
			*done=true;
		}
	}
}


/* non-statefull IO functions */

//extern float gDesiredTemp;
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
void inxDisplay2NegativeOn(){
	GPIO_PortSet(CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN_BASE,(1U << CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN));
}

void inxDisplay2NegativeOff(){
	GPIO_PortClear(CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN_BASE,(1U << CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN));
}

/* async update function for display2 numbers
 * This function is driven by UI states:
 * It will decide what values it should be displaying depending on the button state and the hardware build type
 * todo  This function will lock the whole board if run without the async falg during normal operation. We should gate this flag so it wil always be async after boot to avoid maintenance problems in te future.
 * */
void inxDisplay2Poll(const bool ignoreErrors,bool* done){


	//const
		uint8_t currChar=gDisplay2Chars[gDisplay2CurrentChar];
		if(gDisplay2Ticks==0){
			if(gDisplay2CurrentChar==0){
				//check our error flags, if any any are set then display that error, prevent display from being used if an error is present
				if(ignoreErrors==true){

				}else{
					//inxDisplayClearError();
					for(errorLogsError_t i=0;i<ERROR_LOGS_DISPLAY_BOUNDARY;i++){
						if(gErrorLogs.errors[i].unixTime>0){
							//inxDisplaySetErrorCode(i,true);
							//only display the first error we come to
							break;
						}
					}
				}

				//update what value we are going to display
				switch(gButtonState){
					//case INX_BUTTON_STATE_DISPLAY_UNDEFINED: // don't do this because it overwrites the proper value we updated just before.
					case INX_BUTTON_STATE_DISPLAY_TEMP:
						
						if(gPIDs[0].rampDesiredTemp==gPIDs[0].desiredTemp){
							gDisplay2Flash=false;
						}else{
							gDisplay2Flash=true;
						}
						//inxDisplaySetFromTemperature(gPIDs[0].rampDesiredTemp, 1 );
						// todo the following negative number bit needs to be moved into the number polling function which should decide how to resent negative numbers
	#ifdef INX_FIXED_NEGATIVE_DISPLAY_HARDWARE // this should be handled in the above function?
						if(gPIDs[0].rampDesiredTemp<0.0){
							//inxDisplay2NegativeOn();
						}else{
							//inxDisplay2NegativeOff();
						}
	#endif
						break;
					case INX_BUTTON_STATE_SET_SET_POINT:
						//inxDisplaySetFromTemperature(gTemporarySetPoint, 1 );
	#ifdef INX_FIXED_NEGATIVE_DISPLAY_HARDWARE
						if(gTemporarySetPoint<0.0){
							//inxDisplay2NegativeOn();
						}else{
							//inxDisplay2NegativeOff();
						}
	#endif
						break;
					case INX_BUTTON_STATE_SET_TIMER:
						//inxDisplaySetFromSeconds(1,gDisplayTemporaryTimerSeconds,true);
						break;
					case INX_BUTTON_STATE_DISPLAY_TIMER_TEMPERATURE:
					case INX_BUTTON_STATE_DISPLAY_TIMER:
						gDisplay2Flash=false;
						//inxDisplaySetFromSeconds(1,gPIDs[0].timerSecondsRemaining,true);
						break;
					case INX_BUTTON_STATE_DISPLAY_MODE:
						gDisplay2Flash=false;
						//inxDisplay2Set(0xF,0xF,gButtonsTemporaryScheduleMode,0xF,true);
						break;
					case INX_BUTTON_STATE_DISPLAY_ERROR:
						//inxDisplay2NegativeOff();
						gDisplay2NumTicksOffLimit=CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_OFF_LIMIT;
						gDisplay2NumTicksOnLimit=CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_ON_LIMIT;
						gDisplay2Flash=true;
						//here we should take whatever the current error code for the display is and use that
						//inxDisplaySet(8,8,8,8,true);
						break;
					default:
						break;
				}
			}

			if(gDisplay2Flash){
				gDisplay2FlashTicks++;
				if(gDisplay2FlashTicks<gDisplayNumTicksOffLimit){
					//display blank
					switch(gButtonState){
						case INX_BUTTON_STATE_DISPLAY_ERROR:
							//only blank the last char when in error mode
							//gDisplay2Chars[CONFIG_DISPLAY_NUM_CHARS-1]=0xF;
							break;
						default:
							//blank all of them
							for(uint8_t i=0;i<CONFIG_DISPLAY_NUM_CHARS;i++){
								//gDisplay2Chars[i]=0xF;
							}
							break;
					}
				}else if(gDisplay2FlashTicks>=gDisplayNumTicksOnLimit){
					gDisplay2FlashTicks=0;
				}
			}else{ // need to make sure the display is actually on when flashing is stopped as we can get blank screens sometimes otherwise.
				gDisplay2FlashTicks=0;
			}


#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc // lets be extra sure we don't waggle IO pins if we are not an HRc
		//start writing a number to a char
		(currChar & 0x01)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A));
		(currChar & 0x02)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B));
		(currChar & 0x04)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C));
		(currChar & 0x08)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D));
		GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,(1U << gDisplay2CharsPins[gDisplay2CurrentChar]));
		//LWIP_PLATFORM_DIAG(("wrote char %d",currChar));
#endif

	}
	gDisplay2Ticks++;
	if(gDisplay2Ticks>1){
		//finish writing the number
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_FRDMK64EVAL
#else
		//start writing a number to a char
		GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,(1U << gDisplay2CharsPins[gDisplay2CurrentChar]));
#endif


		gDisplay2Ticks=0;
		gDisplay2CurrentChar++;
		if(gDisplay2CurrentChar>=CONFIG_DISPLAY_NUM_CHARS){
			gDisplay2CurrentChar=0;
			*done=true;
		}
	}
}
#endif



static uint8_t gCurrentDisplay=0;
void inxDisplayPoll(const bool ignoreErrors){
	bool done=false;
	switch(gCurrentDisplay){
		case 0:
			inxDisplay1Poll(ignoreErrors,&done);
			break;
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
		case 1:
			inxDisplay2Poll(ignoreErrors,&done);
			break;
#endif
		default:
			break;
	}
	if(done==true){
		gCurrentDisplay++;
		if(gCurrentDisplay>=CONFIG_NUM_DISPLAYS){
			gCurrentDisplay=0;
		}
	}
}
/* Another state machine function lurkers */

void inxDisplayClearError(){
	switch(gButtonState){
		case INX_BUTTON_STATE_DISPLAY_ERROR:
		case INX_BUTTON_STATE_DISPLAY_UNDEFINED:
			gButtonState=INX_BUTTON_STATE_DISPLAY_TEMP;
			break;
		default:
			break;
	}
}

int8_t inxDisplayGetErrorCode(){
	int8_t errorCode=-1;
	if(gButtonState==INX_BUTTON_STATE_DISPLAY_ERROR){
		errorCode=(gDisplay1Chars[0]*10)+gDisplay1Chars[1];
	}
	return errorCode;
}

void inxDisplaySetErrorCode(const errorLogsError_t error,const bool async){
	const unsigned char char1 = error/10;
	const unsigned char char2 = error%10;
	gButtonState=INX_BUTTON_STATE_DISPLAY_ERROR;
	inxDisplaySet(char1,char2,0xF,0x0,async);
}

/* Warning! THis should only be used for boot up when there is no poll loop to do delays nicely */
static void inxDisplayWait(){
	volatile uint32_t count=0;
	while(count<300000){
		count++;
	}
}

void inxDisplaySet(const char char1,const char char2, const char char3, const char char4,const bool async){
	bool done=false;
	gDisplay1Chars[0]=char1;
	gDisplay1Chars[1]=char2;
	gDisplay1Chars[2]=char3;
	gDisplay1Chars[3]=char4;
	if(async){
		//if async wait for poll to set this for us
	}else{
		while(!done){
			inxDisplayWait();
			inxDisplay1Poll(true,&done);
		}
	}
}

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
void inxDisplay2Set(const char char1,const char char2, const char char3, const char char4,const bool async){
	bool done=false;
	gDisplay2Chars[0]=char1;
	gDisplay2Chars[1]=char2;
	gDisplay2Chars[2]=char3;
	gDisplay2Chars[3]=char4;
	if(async){
		//if async wait for poll to set this for us
	}else{
		while(!done){
			inxDisplayWait();
			inxDisplay2Poll(true,&done);
		}
	}
}
#endif
/****************************************************************************************/
/* Tests Code - only built for diagnostic images                                        */
/****************************************************************************************/

#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
/* Test-Only display functions - hacky duplicates of production functions */

/* blocks for about 1s - only use for debug vuilds */
static void inxDisplayWaitLongTime(){
	volatile uint32_t count=0;
	while(count<1000000){
		count++;
	}
}

/* assumes GPIO has been initialised */
/* todo tis code is total hack - needs reviewing (e.g. writing BCD values directly to regiters rather than 1 bit at a time)*/
void setNumericalDisplay(int integer, int decimal) {
#if INX_HEATROD_IOT_BOARD_VARIANT != INX_HR_FRDMK64EVAL
	int i;
	int char1 = integer%1000/100; // don't reallt need the od here.. but should show
	int char2 = (integer%100)/10;
	int char3 = (integer%10);
	int char4 = decimal;
#define A 0x01
#define B 0x02
#define C 0x04
#define D 0x08
  //printf("XXXXXXX DISPLAY VASLUES = [%d][%d][%d][%d]\n",char1,char2,char3,char4);
	/* char 1 - will blank this one*/
	if (char1 == 0) char1 = 0xF;
	(char1 & A)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A));
	(char1 & B)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B));
	(char1 & C)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C));
	(char1 & D)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D));

	GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR1));
	for (i = 0 ; i < 120000 ; i ++ ) {}
	GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR1));
	/* char number 2  -cut and paste!!*/
	if (char1 == 0) char1 = 0xF;
	(char2 & A)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A));
	(char2 & B)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B));
	(char2 & C)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C));
	(char2 & D)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D));

	GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR2));
	for (i = 0 ; i < 120000 ; i ++ ) {}
	GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR2));
	/* char number 3  - don't blank this */
	(char3 & A)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A));
	(char3 & B)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B));
	(char3 & C)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C));
	(char3 & D)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D));

	GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR3));
	for (i = 0 ; i < 120000 ; i ++ ) {}
	GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR3));

	/* char number 4  - don't blank this */
	(char4 & A)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A));
	(char4 & B)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B));
	(char4 & C)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C));
	(char4 & D)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D));


	GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR4));
	for (i = 0 ; i < 120000 ; i ++ ) {}
	GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR4));

#endif
}

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc

void setNumericalDisplay2(int integer, int decimal) {

	int i;
	int char1 = integer%1000/100; // don't reallt need the od here.. but should show
	int char2 = (integer%100)/10;
	int char3 = (integer%10);
	int char4 = decimal;
#define A 0x01
#define B 0x02
#define C 0x04
#define D 0x08
  //printf("XXXXXXX DISPLAY VASLUES = [%d][%d][%d][%d]\n",char1,char2,char3,char4);
	/* char 1 - will blank this one*/
	if (char1 == 0) char1 = 0xF;
	(char1 & A)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A));
	(char1 & B)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B));
	(char1 & C)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C));
	(char1 & D)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D));

	GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR1));
	for (i = 0 ; i < 120000 ; i ++ ) {}
	GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR1));
	/* char number 2  -cut and paste!!*/
	if (char1 == 0) char1 = 0xF;
	(char2 & A)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A));
	(char2 & B)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B));
	(char2 & C)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C));
	(char2 & D)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D));

	GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR2));
	for (i = 0 ; i < 120000 ; i ++ ) {}
	GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR2));
	/* char number 3  - don't blank this */
	(char3 & A)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A));
	(char3 & B)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B));
	(char3 & C)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C));
	(char3 & D)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D));

	GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR3));
	for (i = 0 ; i < 120000 ; i ++ ) {}
	GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR3));

	/* char number 4  - don't blank this */
	(char4 & A)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_A));
	(char4 & B)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_B));
	(char4 & C)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_C));
	(char4 & D)?GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D)) : GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY_D));


	GPIO_PortClear(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR4));
	for (i = 0 ; i < 120000 ; i ++ ) {}
	GPIO_PortSet(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,(1U << CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR4));
}
#endif


/* Hardware*/
void diagnostic_hw_test_display() {
	int i;
	HW_DIAG_printf("HW DIAG: DISPLAY testing:\n____________________________________\n");
	setNumericalDisplay(888,8); // need to set this once to initialise the digit selects to off
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	HW_DIAG_printf("Starting count down loops on both displays (x2)... ");
	setNumericalDisplay2(888,8); // need to set this once to initialise the digit selects to off
#else
	HW_DIAG_printf("Starting count down loops on single display (x2)... ");
#endif

	for (i = 0;i < 1; i++ ) {
#ifdef INX_FIXED_NEGATIVE_DISPLAY_HARDWARE
		inxDisplay1NegativeOn();
		setNumericalDisplay(99,9);
		setNumericalDisplay2(99,9);
		inxDisplay1NegativeOn();
		//inxDisplay2NegativeOn();
		inxDisplayWaitLongTime();
		inxDisplayWaitLongTime();
		inxDisplayWaitLongTime();
		inxDisplay1NegativeOn();
		inxDisplayWaitLongTime();
		inxDisplayWaitLongTime();
		inxDisplayWaitLongTime();
		inxDisplayWaitLongTime();
		inxDisplayWaitLongTime();
		inxDisplayWaitLongTime();
		//setNumericalDisplay(88,8);
		//setNumericalDisplay2(88,8);

		inxDisplay1NegativeOff();
		inxDisplay2NegativeOff();
#endif

		setNumericalDisplay(999,9);
		inxDisplayWaitLongTime();
		setNumericalDisplay(888,8);
		inxDisplayWaitLongTime();
		setNumericalDisplay(777,7);
		inxDisplayWaitLongTime();
		setNumericalDisplay(666,6);
		inxDisplayWaitLongTime();
		setNumericalDisplay(555,5);
		inxDisplayWaitLongTime();
		setNumericalDisplay(444,4);
		inxDisplayWaitLongTime();
		setNumericalDisplay(333,3);
		inxDisplayWaitLongTime();
		setNumericalDisplay(222,2);
		inxDisplayWaitLongTime();
		setNumericalDisplay(111,1);
		inxDisplayWaitLongTime();
		setNumericalDisplay(000,0);
		inxDisplayWaitLongTime();

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
		setNumericalDisplay2(999,9);
		inxDisplayWaitLongTime();
		setNumericalDisplay2(888,8);
		inxDisplayWaitLongTime();
		setNumericalDisplay2(777,7);
		inxDisplayWaitLongTime();
		setNumericalDisplay2(666,6);
		inxDisplayWaitLongTime();
		setNumericalDisplay2(555,5);
		inxDisplayWaitLongTime();
		setNumericalDisplay2(444,4);
		inxDisplayWaitLongTime();
		setNumericalDisplay2(333,3);
		inxDisplayWaitLongTime();
		setNumericalDisplay2(222,2);
		inxDisplayWaitLongTime();
		setNumericalDisplay2(111,1);
		inxDisplayWaitLongTime();
		setNumericalDisplay2(000,0);
		inxDisplayWaitLongTime();

#endif
	}

	HW_DIAG_printf(" -FINISHED!\n");
	HW_DIAG_printf("HW DIAG: DISPLAY testing:\n____________________________________\n");
}

#endif
