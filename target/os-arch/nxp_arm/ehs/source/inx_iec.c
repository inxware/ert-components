#include "inx_iec.h"
#include "inx_display.h"
#include "board.h"
#include "fsl_adc16.h"
#include "fsl_lptmr.h"
#include "fsl_wdog.h"
#include "fsl_gpio.h"
#include "port.h"
#include "mbport.h"
#include "IEC60730_Safety.h"
#include "fsl_flash.h"
#include "errorLogs.h"

#if defined(FSL_FEATURE_HAS_L1CACHE) && FSL_FEATURE_HAS_L1CACHE
#include "fsl_cache.h"
#endif /* FSL_FEATURE_HAS_L1CACHE */

static inxIecVolatileRamWalkData_t gStackWalkData;
extern inxWatchDogState_t gWatchDogState;
extern inxSafetyMode_t gSafetyMode;
uint32_t* gpInxIecHeapStart = NULL;
uint32_t* gpInxIecHeapEnd = NULL;
extern bool gDiagnosticBoot;

void inxIecRebootDevice(){
	NVIC_SystemReset();
	while(1){}
}

bool inxIecEnableADCs(adc16_channel_config_t* pAdc0ChnConfig,adc16_channel_config_t* pAdc1ChnConfig,const bool ignoreDutyCycle){
	//we must disable interrupts before configuring ADC1 or nothing works properly
	//set priorities of our various interrupts to favour the adcs
	//portDISABLE_INTERRUPTS(); //TODO I don't understand why this is needed
	__ASM("cpsid i");
	NVIC_SetPriority(UART0_RX_TX_IRQn,7U);
	NVIC_SetPriority(ADC0_IRQn,2U); //lowest interrupt allowed is 2 or it breaks using Freertos queues for sending messages
	NVIC_SetPriority(ADC1_IRQn,2U);
	if (!ADC16_InitHardwareTrigger(CONFIG_ADC_BASE_0,pAdc0ChnConfig))
	{
		LWIP_DEBUGF(DEBUG_IEC,("Failed to do the ADC0 init"));
		return false;
	}

	if (!ADC16_InitHardwareTrigger(ADC1,pAdc1ChnConfig))
	{
		LWIP_DEBUGF(DEBUG_IEC,("Failed to do the ADC1 init"));
		return false;
	}

	//setup the HW trigger source
	updateLPTMRInterruptMilliseconds(ignoreDutyCycle);
	#if defined(FSL_FEATURE_ADC16_MAX_RESOLUTION) && (FSL_FEATURE_ADC16_MAX_RESOLUTION >= 16U)
		ADC16_EnableDMA(CONFIG_ADC_BASE_0, false);
		ADC16_EnableDMA(ADC1, false);
	#endif

	NVIC_EnableIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC1_IRQn);
	NVIC_EnableIRQ(UART0_RX_TX_IRQn);
	//enable all the interrupts again
	__asm volatile( "cpsie i" ::: "memory" );
	portENABLE_INTERRUPTS(); //TODO I don't understand why this is needed
	return true;
}

err_enum_t inxIecRAMWalkSegment(inxIecVolatileRamWalkData_t* pWalkData){
	//clear the RAM to 0
	pWalkData->currentAddress = pWalkData->startAddress;
	while(pWalkData->currentAddress<pWalkData->endAddress){
		*pWalkData->currentAddress=0;
		pWalkData->currentAddress+=1;
	}

	//walk from start to the end
	pWalkData->currentAddress = pWalkData->startAddress;
	while(pWalkData->currentAddress < pWalkData->endAddress){
		//printf("0x%X=0x%X\r\n",pWalkData->currentAddress,*pWalkData->currentAddress);
		if(*pWalkData->currentAddress==0){
			*pWalkData->currentAddress=0xff; //set the address behind us to all ones
			pWalkData->currentAddress += 1; //go forward a word
		}else{
			LWIP_PLATFORM_DIAG(("bad ram %X=%X",pWalkData->currentAddress,*pWalkData->currentAddress));
			return ERR_MEM;
		}
	}

	pWalkData->currentAddress = pWalkData->startAddress;
	while(pWalkData->currentAddress < pWalkData->endAddress){
		if(*pWalkData->currentAddress==0xff){
			*pWalkData->currentAddress=0;
			pWalkData->currentAddress+=1;
		}else{
			LWIP_PLATFORM_DIAG(("bad ram %X=%X",pWalkData->currentAddress,*pWalkData->currentAddress));
			return ERR_MEM;
		}
	}

	pWalkData->currentAddress=pWalkData->endAddress-1;
	while(pWalkData->currentAddress>=pWalkData->startAddress){
		if(*pWalkData->currentAddress==0){
			*pWalkData->currentAddress=0xff;
			pWalkData->currentAddress-=1;
		}else{
			LWIP_PLATFORM_DIAG(("bad ram %X=%X",pWalkData->currentAddress,*pWalkData->currentAddress));
			return ERR_MEM;
		}
	}

	pWalkData->currentAddress=pWalkData->endAddress-1;
	while(pWalkData->currentAddress>=pWalkData->startAddress){
		if(*pWalkData->currentAddress==0xff){
			*pWalkData->currentAddress=0;
			pWalkData->currentAddress-=1;
		}else{
			LWIP_PLATFORM_DIAG(("bad ram %X=%X",pWalkData->currentAddress,*pWalkData->currentAddress));
			return ERR_MEM;
		}
	}

	pWalkData->currentAddress = pWalkData->startAddress;
	while(pWalkData->currentAddress < pWalkData->endAddress){
		if(*pWalkData->currentAddress==0){
			pWalkData->currentAddress += 1; //go forward a word
		}else{
			LWIP_PLATFORM_DIAG(("bad ram %X=%X",pWalkData->currentAddress,*pWalkData->currentAddress));
			return ERR_MEM;
		}
	}
	return ERR_OK;
}

//IEC 60730 H.11.12.7 Section 4.2 Variable Memory H.2.19.6
//IEC 60730 H.11.12.7 Section 4.3 Addressing Variable Memory H.2.19.18.2 (a march also counts as a parity test of address)
//IEC 60730 H.11.12.7 Section 5.1 Data H.2.19.8.2 (a march also counts as a parity test of address)
//IEC 60730 H.11.12.7 Section 5.2 Addressing H.2.19.8.2 (a march also counts as a parity test of address)
err_enum_t inxIecVolatileRamWalk(){
	//use global that we know isn't in the stack to test the stack
	gStackWalkData.endAddress=(uint8_t*)(CONFIG_SRAM_UPPER_START_ADDRESS + CONFIG_SRAM_UPPER_SIZE);
	gStackWalkData.startAddress=gStackWalkData.endAddress - (CONFIG_STACK_SIZE+1); //we add 1 because we need to overlap by at least one memory address

	LWIP_DEBUGF(DEBUG_IEC,("inxIecVolatileRamWalk: starting stack walk startAddress=0x%X endAddress=0x%X",
			gStackWalkData.startAddress,
			gStackWalkData.endAddress));

	//copy stack somewhere safe
	//we assume the end of the heap is safe so just substract the stack size from the current stack start
	memcpy(gStackWalkData.startAddress - CONFIG_STACK_SIZE,gStackWalkData.endAddress - CONFIG_STACK_SIZE,CONFIG_STACK_SIZE);
	LWIP_DEBUGF(DEBUG_IEC,("inxIecVolatileRamWalk: stackStoredAt 0x%X copied from 0x%X",
			gStackWalkData.startAddress - CONFIG_STACK_SIZE,
			gStackWalkData.endAddress - CONFIG_STACK_SIZE));

	//DO NOT USE PRINTF TILL AFTER YOU HAVE FINISHED WALKING THE STACK

	//clear the stack to 0
	gStackWalkData.currentAddress = gStackWalkData.startAddress;
	while(gStackWalkData.currentAddress<gStackWalkData.endAddress){
		*gStackWalkData.currentAddress=0;
		gStackWalkData.currentAddress+=1;
	}

	//walk from start to the end
	gStackWalkData.currentAddress = gStackWalkData.startAddress;
	while(gStackWalkData.currentAddress < gStackWalkData.endAddress){
		//printf("0x%X=0x%X\r\n",gStackWalkData.currentAddress,*gStackWalkData.currentAddress);
		if(*gStackWalkData.currentAddress==0){
			*gStackWalkData.currentAddress=0xff; //set the address behind us to all ones
			gStackWalkData.currentAddress += 1; //go forward a word
		}else{
			//stack errors have to be fatal and we assume the watch dog will recover us
			do{
				LWIP_PLATFORM_DIAG(("bad ram %X=%X",gStackWalkData.currentAddress,*gStackWalkData.currentAddress));
			}while(1);
		}
	}

	gStackWalkData.currentAddress = gStackWalkData.startAddress;
	while(gStackWalkData.currentAddress < gStackWalkData.endAddress){
		if(*gStackWalkData.currentAddress==0xff){
			*gStackWalkData.currentAddress=0;
			gStackWalkData.currentAddress+=1;
		}else{
			do{
				LWIP_PLATFORM_DIAG(("bad ram %X=%X",gStackWalkData.currentAddress,*gStackWalkData.currentAddress));
			}while(1);
		}
	}

	gStackWalkData.currentAddress=gStackWalkData.endAddress-1;
	while(gStackWalkData.currentAddress>=gStackWalkData.startAddress){
		if(*gStackWalkData.currentAddress==0){
			*gStackWalkData.currentAddress=0xff;
			gStackWalkData.currentAddress-=1;
		}else{
			do{
				LWIP_PLATFORM_DIAG(("bad ram %X=%X",gStackWalkData.currentAddress,*gStackWalkData.currentAddress));
			}while(1);
		}
	}

	gStackWalkData.currentAddress=gStackWalkData.endAddress-1;
	while(gStackWalkData.currentAddress>=gStackWalkData.startAddress){
		if(*gStackWalkData.currentAddress==0xff){
			*gStackWalkData.currentAddress=0;
			gStackWalkData.currentAddress-=1;
		}else{
			do{
				LWIP_PLATFORM_DIAG(("bad ram %X=%X",gStackWalkData.currentAddress,*gStackWalkData.currentAddress));
			}while(1);
		}
	}

	gStackWalkData.currentAddress = gStackWalkData.startAddress;
	while(gStackWalkData.currentAddress < gStackWalkData.endAddress){
		if(*gStackWalkData.currentAddress==0){
			gStackWalkData.currentAddress += 1; //go forward a word
		}else{
			do{
				LWIP_PLATFORM_DIAG(("bad ram %X=%X",gStackWalkData.currentAddress,*gStackWalkData.currentAddress));
			}while(1);
		}
	}

	//put the stack back in the correct place
	memcpy(gStackWalkData.endAddress - CONFIG_STACK_SIZE,gStackWalkData.startAddress - CONFIG_STACK_SIZE,CONFIG_STACK_SIZE);
	LWIP_DEBUGF(DEBUG_IEC,("inxIecVolatileRamWalk: stackStoredAt 0x%X copied from 0x%X",
			gStackWalkData.endAddress - CONFIG_STACK_SIZE,
			gStackWalkData.startAddress - CONFIG_STACK_SIZE));

	//then use stack variables to test the rest of the ram
	inxIecVolatileRamWalkData_t ramWalkData;
	//partition ram in to X segments
	size_t chunkSize = CONFIG_SRAM_UPPER_SIZE / CONFIG_IEC_RAM_CHUNKS;
	//walk empty segment at end
	ramWalkData.endAddress=gStackWalkData.endAddress - CONFIG_STACK_SIZE; //set end address to be start of stack
	ramWalkData.startAddress=(ramWalkData.endAddress - chunkSize)-2; //we minus two so that we have enough space for future walks and so we can be overlapped
#ifdef INX_HEATROD_VERBOSE_DEBUG
	LWIP_DEBUGF(DEBUG_IEC,("inxIecVolatileRamWalk: starting ram walk startAddress=0x%X endAddress=0x%X",
			ramWalkData.startAddress,
			ramWalkData.endAddress));
#endif
	err_enum_t result=inxIecRAMWalkSegment(&ramWalkData);
	if(result==ERR_OK){}else{
		return result;
	}
	//we copy this over because we will destroy the gStackWalkData during testing
	void* safeMemoryStartAddress=ramWalkData.startAddress+1; //we plus one because we minused two earlier


	//for each remaining segment
	while((uint32_t)ramWalkData.startAddress>CONFIG_SRAM_UPPER_START_ADDRESS){
		ramWalkData.endAddress=ramWalkData.startAddress+1; // we overlap previous test
		ramWalkData.startAddress=ramWalkData.endAddress - chunkSize;
		if((uint32_t)ramWalkData.startAddress<CONFIG_SRAM_UPPER_START_ADDRESS){
			ramWalkData.startAddress=(uint8_t*)CONFIG_SRAM_UPPER_START_ADDRESS;
		}
#ifdef INX_HEATROD_VERBOSE_DEBUG
		LWIP_DEBUGF(DEBUG_IEC,("inxIecVolatileRamWalk: starting ram walk startAddress=0x%X endAddress=0x%X",ramWalkData.startAddress,ramWalkData.endAddress));
#endif
		//copy to empty segment
		memcpy(safeMemoryStartAddress,ramWalkData.startAddress,ramWalkData.endAddress - ramWalkData.startAddress);
#ifdef INX_HEATROD_VERBOSE_DEBUG
		LWIP_DEBUGF(DEBUG_IEC,("inxIecVolatileRamWalk: safeMemoryStartAddress=0x%X safeMemoryEndAddress=0x%X",
				safeMemoryStartAddress,
				safeMemoryStartAddress+(ramWalkData.endAddress - ramWalkData.startAddress)));
#endif
		//test
		result=inxIecRAMWalkSegment(&ramWalkData);
		if(result==ERR_OK){}else{
			return result;
		}
		//copy data back from empty segment
		memcpy(ramWalkData.startAddress,safeMemoryStartAddress,ramWalkData.endAddress - ramWalkData.startAddress);
	}

	//verify flex RAM
	ramWalkData.startAddress=(uint8_t*)CONFIG_FLEX_RAM_START_ADDRESS;
	ramWalkData.endAddress=ramWalkData.startAddress+CONFIG_FLAX_RAM_SIZE;
#ifdef INX_HEATROD_VERBOSE_DEBUG
	LWIP_DEBUGF(DEBUG_IEC,("inxIecVolatileRamWalk: starting ram walk startAddress=0x%X endAddress=0x%X",ramWalkData.startAddress,ramWalkData.endAddress));
#endif
	result=inxIecRAMWalkSegment(&ramWalkData);
	if(result==ERR_OK){}else{
		return result;
	}
#ifdef INX_HEATROD_VERBOSE_DEBUG
	LWIP_DEBUGF(DEBUG_IEC,("inxIecVolatileRamWalk: not walking SRAM lower because we don't use it"));
#endif
	//do we need to run this when we are at 120Mhz? we are because we do this periodically now
	LWIP_DEBUGF(DEBUG_IEC,("inxIecRamWalk: done"));
	return ERR_OK; //assume if we got here then the RAM must be ok
}

void inxIecWaitForWatchDogUpdate(WDOG_Type *base)
{
    /* Accessing register by bus clock */
    for (uint32_t i = 0; i < WDOG_WCT_INSTRUCITON_COUNT; i++)
    {
        (void)base->RSTCNT;
    }
}

/*!
 * @brief Gets the Watchdog timer output.
 *
 * @param base WDOG peripheral base address
 * @return Current value of watchdog timer counter.
 */
static inline uint32_t GetTimerOutputValue(WDOG_Type *base)
{
    return (uint32_t)((((uint32_t)base->TMROUTH) << 16U) | (base->TMROUTL));
}

void inxIecDisableADCs(){
	//portDISABLE_INTERRUPTS();
	__asm volatile ("cpsid i");
	NVIC_DisableIRQ(ADC0_IRQn);
	NVIC_DisableIRQ(ADC1_IRQn);
	NVIC_DisableIRQ(UART0_RX_TX_IRQn);
	NVIC_DisableIRQ(UART0_ERR_IRQn);
	ADC16_Deinit(CONFIG_ADC_BASE_0);
	ADC16_Deinit(ADC1);
}

extern volatile bool gRTCDisabled;
extern bool gRTCInitDone;
extern uint32_t gRTCPreviousSeconds;
extern uint32_t gLPTMRInterruptMilliseconds;
//IEC 60730 H.11.12.7 Section 2 Interrupt handling and execution H.2.16.5
static err_enum_t inxIecInterruptTests(inxIecInterruptData_t* pInterruptData,adc16_channel_config_t* adc0ChnConfig,adc16_channel_config_t* adc1ChnConfig){
	//LWIP_PLATFORM_DIAG(("inxIecInterruptTests: starting"));
	uint32_t i = 0;
	volatile uint32_t count = 0; //used for tracking how fast the cpu is running
	//set our initial state
	pInterruptData->interruptCount = 0;
	pInterruptData->doingTest = true;

	//remember previous interrupt timings
	uint32_t lptmrInterruptMilliseconds=gLPTMRInterruptMilliseconds;
	gLPTMRInterruptMilliseconds=333;

	//disable watch dogs, the cpu count will tell us if the adcs are firing correctly
	inxIecSetWatchDog(false,false,1100U,900U);

	if(inxIecEnableADCs(adc0ChnConfig,adc1ChnConfig,true)){

	}else{
		return ERR_MEM;
	}
	//Check timing of 10 interrupts to see if these occur at the proper rate of CPU cycles

	#if CONFIG_IEC_CHECK_RTC_DURING_ADC_CHECK
	//also check the RTC worked correctly
	if(gRTCDisabled==false && gRTCInitDone==true){
		gRTCPreviousSeconds=inxRTCGetSeconds();
	}
	#endif //CONFIG_IEC_CHECK_RTC_DURING_ADC_CHECK
	for(i=0;i<CONFIG_IEC_INTERRUPT_TEST_LENGTH_SECONDS;i++){
		count = 0;
		pInterruptData->interruptCount = 0;
		while(pInterruptData->interruptCount<1 && count<=13000000){
			count++;
		}
		//IEC 60730 H.11.12.7 Section 3 Clock H.2.18.10.1
		SW_DIAG_printf("\ninxIecInterruptTests: ADC watch dog count is %d cpu count=%d\n",GetTimerOutputValue(WDOG),count);
		//ignore first test as it always a bit out of band
		if(i>0){
			if(count>13000000 || count<5000000){ // todo the first count value varies on the first iteration - possibly build dependent) -
				// todo perhaps needs a wider acceptance band on first iteration?
				// todo review if these numbers should be changed depending on whether we have debug enabled and other build config.  This keeps tripping up the boot and results in a locked system
				SW_DIAG_printf("inxIecInterruptTests: CPU count out of bounds\n");
				//WDOG_Deinit(WDOG); //don't use deinit as it clears the reset count
				inxIecSetWatchDog(false,false,1000U,1000U); //these values can't be 0 if you are disabling the watch dog or it will fire immediately
				inxIecDisableADCs();
				return ERR_MEM;
			}
		}
		WDOG_Refresh(WDOG);
	}
	WDOG_Disable(WDOG);
	inxIecSetWatchDog(false,false,1000U,1000U);

	//disable the ADCs again
	inxIecDisableADCs();
	gLPTMRInterruptMilliseconds=lptmrInterruptMilliseconds;
	pInterruptData->doingTest=false;
	pInterruptData->interruptCount=0;

	//check the RTC
	#if CONFIG_IEC_CHECK_RTC_DURING_ADC_CHECK
	if(gRTCDisabled==false && gRTCInitDone==true){
		u8_t diff=inxRTCGetSeconds()-gRTCPreviousSeconds;
		if(diff<(CONFIG_IEC_INTERRUPT_TEST_LENGTH_SECONDS-2) || diff>(CONFIG_IEC_INTERRUPT_TEST_LENGTH_SECONDS+2)){
			SW_DIAG_printf("inxIecInterruptTests: RTC seconds out of bounds diff=%d prev=%d now=%d\r\n",diff,gRTCPreviousSeconds,inxRTCGetSeconds());
			return ERR_MEM;
		}
	}
	#endif //CONFIG_IEC_CHECK_RTC_DURING_ADC_CHECK
	//LWIP_PLATFORM_DIAG(("inxIecInterruptTests: done"));
	return ERR_OK;
}


/* Returns Power present as true */
void inxIecGPIOGetPins(const inxIecHeater_t heater,GPIO_Type** pBase,volatile uint32_t* pPin){
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	switch(heater){
		case INX_IEC_GPIO_HEATER_ONE_TEST:
			*pPin = CONFIG_GPIO_SENSE_CONRELAY_1;
			break;
		case INX_IEC_GPIO_HEATER_TWO_TEST:
			*pPin = CONFIG_GPIO_SENSE_CONRELAY_2;
			break;
		case INX_IEC_GPIO_HEATER_THREE_TEST:
			*pPin = CONFIG_GPIO_SENSE_CONRELAY_3;
			break;
		case INX_IEC_GPIO_THERMAL_CO_TEST:
			*pBase = CONFIG_GPIO_SENSE_CO_BASE;
			*pPin = CONFIG_GPIO_SENSE_THERM_CO_1;
			break;
		default:
			*pBase = NULL;
			*pPin = (uint32_t)0;
			break;
	}
#else
	*pBase = NULL;
	*pPin = 0; // assume there are no valid pin indexes of 0
#endif
}

extern inxControllerContactorMode_t gControllerContactorMode;
bool inxIecGPIOCheck(inxRelayCurrentState_t* pErrorState,volatile const inxIecHeater_t heater,volatile const inxIecGPIOState_t state,const uint32_t ticksSinceHeaterChange,const uint32_t minTicksToWaitForHeaterChange){
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	bool success = false;
	uint32_t expectedResult = 0;
	GPIO_Type* pBase=CONFIG_GPIO_SENSE_CONRELAY_BASE;
	volatile uint32_t pin = 0;
	volatile uint32_t result = 2; //we should never see this value if things are working correctly

	switch(gControllerContactorMode){
		case INX_CONTROLLER_CONTACTOR_MODE_SSR:
			return true;
			break;
		default:
			break;
	}

	if(ticksSinceHeaterChange<minTicksToWaitForHeaterChange){
		return true;
	}
#ifndef PMLD_NO_CHECKS

	switch(state){
		case INX_IEC_GPIO_OFF:
			expectedResult=0;
			break;
		case INX_IEC_GPIO_ON:
			expectedResult=1;
			break;
	}
	
	inxIecGPIOGetPins(heater,&pBase,&pin);	
	if(pBase){
		result = GPIO_PinRead(pBase,pin);

		//log this in case there is an error later
		switch(heater){
			case INX_IEC_GPIO_HEATER_ONE_TEST:
				pErrorState->element1Control=expectedResult;
				pErrorState->element1Monitor=result;
				break;
			case INX_IEC_GPIO_HEATER_TWO_TEST:
				pErrorState->element2Control=expectedResult;
				pErrorState->element2Monitor=result;
				break;
			case INX_IEC_GPIO_HEATER_THREE_TEST:
				pErrorState->element3Control=expectedResult;
				pErrorState->element3Monitor=result;
				break;
			case INX_IEC_GPIO_THERMAL_CO_TEST:
				pErrorState->thermalCO1Control=expectedResult;
				pErrorState->thermalCO1Monitor=result;
				#if CONFIG_THERMAL_CUT_OUT_DISABLED
					return true;
				#endif //CONFIG_THERMAL_CUT_OUT_DISABLED
				break;
			default:
				break;
		}
	}
	success = result==expectedResult;
	//LWIP_PLATFORM_DIAG(("heater=%d result=%d state=%d success=%d",heater,result,state,success));
	switch(gSafetyMode){
		case INX_SAFETY_MODE_DISABLE_ALL_HARDWARE_CHECKS:
			success=true;
			break;
		default:
			break;
	}
	return success;
#else
	return true;
#endif
#else // INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	return true;
#endif
}

void inxIecGPIOCheckFatal(inxRelayCurrentState_t* pRelayState,const inxIecHeater_t heater,const inxIecGPIOState_t state){
	bool success = inxIecGPIOCheck(pRelayState,heater,state,0,0);
	if(success){}else{
		LWIP_PLATFORM_DIAG(("inxIecGPIOCheckFatal: failed on %d==%d",heater,state));
		//TODO actually stop
		//while(1){}
	}
}

/* Safe way of setting relays that knows what is on and what is off - see inxIecGPIOState */
void inxIecGPIOSet(const inxIecHeater_t heater,const inxIecGPIOState_t state){
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	GPIO_Type* pBase = 0;
	uint32_t pin = 0;
 // todo make this work for the auxillary outputs also. below is heater specific but we have other outputs - use same function of another - need to decide where we use device or functional names etc.)
// todo check if we want this for HRc
	switch(heater){
		case INX_IEC_GPIO_HEATER_ONE_TEST:
			pBase = CONFIG_HEATER_ONE_GPIO_BASE;
			pin = CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1;
			break;
		case INX_IEC_GPIO_HEATER_TWO_TEST:
			pBase = CONFIG_HEATER_TWO_GPIO_BASE;
			pin = CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2;
			break;
		case INX_IEC_GPIO_HEATER_THREE_TEST:
			pBase = CONFIG_HEATER_THREE_GPIO_BASE;
			pin = CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3;
			break;
			// todo  @patrick Do we want to add THERMAL_CO_TEST here too?
		default:
			break;
	}
	if(pBase){
		switch(state){
			case INX_IEC_GPIO_OFF:
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
				GPIO_PortSet(pBase,1U<<pin);
#else
				GPIO_PortClear(pBase,1U<<pin);
#endif
				break;
			case INX_IEC_GPIO_ON:
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
				GPIO_PortClear(pBase, 1U << pin);
#else
				GPIO_PortSet(pBase, 1U << pin);
#endif
				break;
		}
	}
#endif
}

//IEC 60730 H.11.12.7 Section 7.1 I/O Periphery H.2.18.12
err_enum_t inxIecGPIOTests(inxRelayCurrentState_t* pRelayState){
	/* This is disabled for HRc because we may ise different GPIO and don't have sensors on HRc BoB boards */
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	//LWIP_PLATFORM_DIAG(("inxIecGPIOTests: start"));
	//assume 3 elements
	//set all off
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_OFF);
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_OFF);
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_OFF);

	//check all off
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}

	//check each one individually (i.e did the others stay off and the correct one come on)
	//heater 1
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_ON);
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_ON,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}

	//heater 2
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_OFF);
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_ON);
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_ON,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}

	//heater 3
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_OFF);
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_ON);
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_ON,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}

	//check all three (to test whether we brown out under too much load
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_ON);
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_ON);
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_ON);
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_ON,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_ON,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_ON,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}

	//finally turn them all off
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_OFF);
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_OFF);
	inxIecGPIOSet(INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_OFF);
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	if(inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_OFF,0,0)){}else{
		return ERR_BOB_HARDWARE;
	}
	//LWIP_PLATFORM_DIAG(("inxIecGPIOTests: done"));
#endif
	return ERR_OK;
}

void inxIecSetWatchDog(const bool enableWatchDog,const bool enableWindowMode,const uint32_t timeoutValue,const uint32_t windowValue){
	wdog_config_t config;
	WDOG_GetDefaultConfig(&config);
	config.enableWdog = enableWatchDog;
	config.enableUpdate = true;
	config.enableInterrupt = true;
	config.timeoutValue = timeoutValue; // 1100 milliseconds
	config.enableWindowMode = enableWindowMode; //dont trigger a reset if we clear the watch dog too early
	config.windowValue = windowValue; //900 milliseconds
	WDOG_Init(WDOG, &config);
	inxIecWaitForWatchDogUpdate(WDOG);
}

void inxIecSetNormalWatchDog(){
	//inxIecSetWatchDog(true,false,1100U,900U);
	//LWIP_PLATFORM_DIAG(("TODO enable watchdog"));
	inxIecSetWatchDog(true,false,5000U,900U);
}

void inxIecDiagnosticHalt(){
#if CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
#else
	while(1){}
#endif
}

void inxIecDiagnostic(inxIecInterruptData_t* pInterruptData,adc16_channel_config_t* adc0ChnConfig,adc16_channel_config_t* adc1ChnConfig,
		const uint32_t crcValue,const uint32_t programSizeBytes){

	//only actually do the checks if we are allowed to
	switch(gSafetyMode){
		case INX_SAFETY_MODE_DISABLE_SLOW_START_CHECKS:
		case INX_SAFETY_MODE_DISABLE_ALL_HARDWARE_CHECKS:
			return;
			break;
		default:
			break;
	}

	inxDisplaySetErrorCode(ERROR_LOGS_RAM_WALK,false);
	err_enum_t result=inxIecVolatileRamWalk();
	if(result==ERR_OK){}else{
		SW_DIAG_printf(DEBUG_IEC,("inxIecDiagnostic: ram walk failed, halting\n"));
		errorLogsWrite(ERROR_LOGS_RAM_WALK);
		inxIecDiagnosticHalt();
	}

#if CONFIG_DO_FLASH_TEST == 1
	//disable watch dogs, the cpu count will tell us if the adcs are firing correctly

#if CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
	//use watch dog timer here in case the flash crc check fails, we want to reboot and do a factory reset
	inxIecSetWatchDog(true,false,2000U,900U);
#else //CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT

#endif //CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT

	inxDisplaySetErrorCode(ERROR_LOGS_PROGRAM_FLASH_CRC,false);
	result=inxIecFlashCRC(crcValue,programSizeBytes);

	if(result==ERR_OK){
#if CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
		WDOG_Refresh(WDOG);
#else //CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
#endif //CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
	}else{
		LWIP_DEBUGF(DEBUG_IEC,("inxIecDiagnostic: flash CRC failed, halting"));
		errorLogsWrite(ERROR_LOGS_PROGRAM_FLASH_CRC);
		inxIecDiagnosticHalt();
	}

#if CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
	inxIecSetWatchDog(false,false,2000U,900U);
#else //CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
#endif //CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT


#endif //CONFIG_DO_FLASH_TEST

	inxDisplaySetErrorCode(ERROR_LOGS_INTERRUPTS,false);
	result=inxIecInterruptTests(pInterruptData,adc0ChnConfig,adc1ChnConfig);
	if(result==ERR_OK){}else{
		SW_DIAG_printf("inxIecDiagnostic: interrupts failed, halting\n");
		errorLogsWrite(ERROR_LOGS_INTERRUPTS);
		inxIecDiagnosticHalt();
	}

	//inxDisplaySetErrorCode(ERROR_LOGS_GPIO,false);
	//result=inxIecGPIOTests();
	if(result==ERR_OK){

	}else{
		LWIP_DEBUGF(DEBUG_IEC,("inxIecDiagnostic: GPIO failed in someway but we don't care and will just report it back"));
	}
	SW_DIAG_printf("TODO figure out what the uart diagnostic tests are\n");
	//inxIecUartTests();

	inxDisplaySetErrorCode(ERROR_LOGS_RTC_DISABLED,false);
	//try to init the RTC, this can lock up the device if vbat does not work
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
	HW_DIAG_printf("Doing RTC Init\n");
#endif
	inxRTCInit();
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
	HW_DIAG_printf("Done RTC Init\n");
#endif
}

static err_enum_t inxIecCRC(const uint32_t crcValue,const uint32_t startAddress,const uint32_t programSizeBytes,uint32_t* calculatedCrcValue){
	flash_runtime_test_parameters_t runTimeParams;
	flash_configuration_parameters_t configurationParameters;
	flash_dcp_state_t dcpState;
	safety_common_t safetyCommon;
	safetyCommon.IEC60730B_flash_test_result = 0;
	//SW_DIAG_printf("inxIecCRC: start crcValue=0x%X startAddress=0x%X programSizeBytes=0x%X\n",crcValue,startAddress,programSizeBytes);
	SafetyFlashTestInit(crcValue,startAddress,startAddress+(programSizeBytes),&runTimeParams, &configurationParameters, &dcpState);
	err_enum_t result = SafetyFlashAfterResetTest(&safetyCommon, &configurationParameters, &dcpState);
	*calculatedCrcValue=safetyCommon.IEC60730B_flash_test_result;
	//SW_DIAG_printf("inxIecCRC: done result=%d calculated crc=%d\n",result,*calculatedCrcValue);
	return result;
}

err_enum_t inxIecSettingsCRC(permStorage_t* pSettings){
	uint32_t calculatedCrcValue = 0;
	err_enum_t result = inxIecCRC(pSettings->settingsCrcValue,
		((uint32_t)pSettings)+sizeof(pSettings->settingsCrcValue)+sizeof(pSettings->size),
		pSettings->size-sizeof(pSettings->settingsCrcValue)-sizeof(pSettings->size),
		&calculatedCrcValue);
	//if we have no existing crc value then update the settings and report ok
	if(pSettings->settingsCrcValue==0){
		pSettings->settingsCrcValue=calculatedCrcValue;
		result=ERR_OK;
	}
	return result;
}

//IEC 60730 H.11.12.7 Section 4.1 Invariable Memory H.2.19.4.2 (assuming word size of 8 bits)
//IEC 60730 H.11.12.7 Section 4.3 Addressing Invariable Memory H.2.18.22
err_enum_t inxIecFlashCRC(const uint32_t crcValue,const uint32_t programSizeBytes){
	uint32_t calculatedCrcValue=0;
	err_enum_t result = inxIecCRC(crcValue,0,programSizeBytes,&calculatedCrcValue);
	return result;
}

extern dutyCycleState_t dutyCycleState0;
extern dutyCycleState_t dutyCycleState1;
extern dutyCycleState_t dutyCycleState2;
void inxIecTurnGPIOOff(){
	dutyCycleStateOffForced(&dutyCycleState0); //make sure we aren't heating
	dutyCycleStateOffForced(&dutyCycleState1);
	dutyCycleStateOffForced(&dutyCycleState2);
}

void inxIecTurnGPIOOn(){
	
}

void inxIecFaultShutdown(){
#if CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
	LWIP_PLATFORM_DIAG(("inxIecFaultShutdown: ignoring due to CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT"));
#else //CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
	WDOG_Refresh(WDOG); //give ourselves enough time to shut down
	inxIecTurnGPIOOff();
	//shutdown_cleanup(kShutdownType_Reset); //no longer going to manually shut down , we'll use the watch dog for this
	//wait for the watch dog timer to reset us
	while(1){}
#endif //CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
}

//static uint32_t gInxIecTestCount=0;
#define CONFIG_NUM_FRAG_BLOCK_POINTERS 256
static void* gFragBlocks[CONFIG_NUM_FRAG_BLOCK_POINTERS];
static uint8_t inxIecCalculateRAMUsedPercent(){
	void* p;
	//void* heapPointer;
	uint32_t siz = 1;
	while (true){
		  p = malloc(siz);
		  if(p){
			  //heapPointer=p;
			  free(p);
			  siz++;
		  }else{
			siz--;
			  break;
		  }
	}
	p=malloc(siz);
	uint16_t fragBlocksCount=0;
	if(p){
			for(int i=0;i<CONFIG_NUM_FRAG_BLOCK_POINTERS;i++){
				gFragBlocks[i]=malloc(4);
			}
			for(int i=0;i<CONFIG_NUM_FRAG_BLOCK_POINTERS;i++){
				if(gFragBlocks[i]==NULL){

				}else{
					fragBlocksCount++;
					free(gFragBlocks[i]);
				}
			}
			free(p);
	}
	LWIP_DEBUGF(DEBUG_IEC,("inxIecCalculateRAMUsedPercent: heapPointer=0x%X maxSiz=%d fragBlocks=%d",heapPointer,siz,fragBlocksCount));
	/*gInxIecTestCount++;
	if(gInxIecTestCount>10){
		siz=0;
		while(true){
			p=malloc(1);
			if(p){
				siz++;
			}else{
				LWIP_PLATFORM_DIAG(("PBB siz=%d",siz));
				break;
			}
		}
	}*/
	return 100-((siz*100)/CONFIG_HEAP_SIZE_BYTES);
}

static err_t inxIecRamBoundaries(){
	//check heap start not overwritten
	if(*gpInxIecHeapStart==INX_FLASH_VALIDATION_CODE){
		LWIP_DEBUGF(DEBUG_IEC,("inxIecRamBoundaries: heap start ok"));
	}else{
		return ERR_MEM;
	}

	if(*gpInxIecHeapEnd==INX_FLASH_VALIDATION_CODE){
		LWIP_DEBUGF(DEBUG_IEC,("inxIecRamBoundaries: heap end ok"));
	}else{
		return ERR_MEM;
	}
	return ERR_OK;
}

static status_t inxIecCheckSingleRelay(inxRelayCurrentState_t* pRelayState,dutyCycleState_t* pState,
																				const inxIecHeater_t heater,const bool checkTCO){
	status_t err=kStatus_Success;
	volatile uint32_t count=0;
	WDOG_Refresh(WDOG);
	//force on
	gWatchDogState.deviceFailed=false;
	dutyCycleStateOn(pState);
	while(count<CONFIG_STARTUP_RELAY_CHECK_TICKS){
		count++;
	}
	count = 0;
	//check on
	if(!inxIecGPIOCheck(pRelayState,heater,INX_IEC_GPIO_OFF,0,0)){
		err=kStatus_Fail;
		errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_OFF);
	}

	//if required check that the thermal cut out is on
	if(err==kStatus_Success && checkTCO){
		if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_THERMAL_CO_TEST,INX_IEC_GPIO_OFF,0,0)){
			err=kStatus_Fail;
			errorLogsWrite(ERROR_LOGS_THERMAL_CUT_OUT);
		}
	}

	//force off
	dutyCycleStateOffForced(pState);
	while(count<CONFIG_STARTUP_RELAY_CHECK_TICKS){
		count++;
	}
	if(err==kStatus_Success){
		//check off
		if(!inxIecGPIOCheck(pRelayState,heater,INX_IEC_GPIO_ON,0,0)){
			err=kStatus_Fail;
			errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_ON);
		}
	}
	gWatchDogState.deviceFailed=true;
	return err;
}

void inxIecShutOffAllRelays(const bool forever){
	//turn everything off
	dutyCycleStateOffForced(&dutyCycleState0);
	dutyCycleStateOffForced(&dutyCycleState1);
	dutyCycleStateOffForced(&dutyCycleState2);

	if(forever==true){
		//disable global ability to put the power on
		gWatchDogState.deviceFailed = true;
	}
}

extern inxPID_t gPIDs[];
status_t inxIecInternalRelayCheck(inxRelayCurrentState_t* pRelayState){
	status_t err=kStatus_Success;
	volatile uint32_t count=0;
	//clear error conditions
	errorLogsClearSingleError(ERROR_LOGS_INTERNAL_RELAY_STUCK_OFF);
	errorLogsClearSingleError(ERROR_LOGS_ADC_DISABLED);
	errorLogsClearSingleError(ERROR_LOGS_ADC_VALUES_UNSAFE);
	errorLogsClearSingleError(ERROR_LOGS_THERMAL_CUT_OUT);
	//only clear stuck on error if the user does a reset
	//errorLogsClearSingleError(ERROR_LOGS_INTERNAL_RELAY_STUCK_ON);

	switch(gSafetyMode){
		case INX_SAFETY_MODE_DISABLE_SLOW_START_CHECKS:
		case INX_SAFETY_MODE_DISABLE_ALL_HARDWARE_CHECKS:
			return err;
			break;
		default:
			break;
	}

	#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
		//set all relays to off
		inxIecShutOffAllRelays(false);
		//wait 10ms
		WDOG_Refresh(WDOG);
		count=0;
		while(count<CONFIG_STARTUP_RELAY_CHECK_TICKS){
			count++;
		}
		//check all off
		if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_ON,0,0)){
			err=kStatus_Fail;
			errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_ON);
		}
		switch(gPIDs[0].elementMode){
			case ELEMENT_MODE_DOUBLE_STEPPED:
			case ELEMENT_MODE_DOUBLE_TANDEM:
			case ELEMENT_MODE_TRIPLE_STEPPED:
			case ELEMENT_MODE_TRIPLE_TANDEM:
			case ELEMENT_MODE_SINGLE_TWO_POLE:
				if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_ON,0,0)){
					err=kStatus_Fail;
					errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_ON);
				}
				break;
			default:
				break;
		}
		switch(gPIDs[0].elementMode){
			case ELEMENT_MODE_TRIPLE_STEPPED:
			case ELEMENT_MODE_TRIPLE_TANDEM:
				if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_ON,0,0)){
					err=kStatus_Fail;
					errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_ON);
				}
				break;
			default:
				break;
		}

		if(err!=kStatus_Success){
			return err;
		}

		//check relay one
		if(inxIecCheckSingleRelay(pRelayState,&dutyCycleState0,INX_IEC_GPIO_HEATER_ONE_TEST,true)!=kStatus_Success){
			err=kStatus_Fail;
		}
		switch(gPIDs[0].elementMode){
			case ELEMENT_MODE_DOUBLE_STEPPED:
			case ELEMENT_MODE_DOUBLE_TANDEM:
			case ELEMENT_MODE_TRIPLE_STEPPED:
			case ELEMENT_MODE_TRIPLE_TANDEM:
				//check relay two
				if(inxIecCheckSingleRelay(pRelayState,&dutyCycleState1,INX_IEC_GPIO_HEATER_TWO_TEST,false)!=kStatus_Success){
					err=kStatus_Fail;
				}
				break;
			default:
				break;
		}

		switch(gPIDs[0].elementMode){
			case ELEMENT_MODE_TRIPLE_STEPPED:
			case ELEMENT_MODE_TRIPLE_TANDEM:
				//check relay three
				if(inxIecCheckSingleRelay(pRelayState,&dutyCycleState2,INX_IEC_GPIO_HEATER_THREE_TEST,false)!=kStatus_Success){
					err=kStatus_Fail;
				}
				break;
			default:
				break;
		}

		//don't proceed with rest of tests if something broke at this stage
		if(err!=kStatus_Success){
			return err;
		}

		//set all relays to on
		gWatchDogState.deviceFailed=false;
		dutyCycleStateOn(&dutyCycleState0);
		dutyCycleStateOn(&dutyCycleState1);
		dutyCycleStateOn(&dutyCycleState2);
		gWatchDogState.deviceFailed=true;
		//wait 10ms
		count=0;
		WDOG_Refresh(WDOG);
		while(count<CONFIG_STARTUP_RELAY_CHECK_TICKS){
			count++;
		}
		//check all on
		if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_OFF,0,0)){
			err=kStatus_Fail;
			errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_OFF);
		}
		switch(gPIDs[0].elementMode){
			case ELEMENT_MODE_DOUBLE_STEPPED:
			case ELEMENT_MODE_DOUBLE_TANDEM:
			case ELEMENT_MODE_TRIPLE_STEPPED:
			case ELEMENT_MODE_TRIPLE_TANDEM:
				if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_OFF,0,0)){
					err=kStatus_Fail;
					errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_OFF);
				}
				break;
			default:
				break;
		}
		switch(gPIDs[0].elementMode){
			case ELEMENT_MODE_TRIPLE_STEPPED:
			case ELEMENT_MODE_TRIPLE_TANDEM:
				if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_OFF,0,0)){
					err=kStatus_Fail;
					errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_OFF);
				}
				break;
			default:
				break;
		}
		
		//check thermal cut out is on, this will always pass in 3 phase 415 (delta) mode
		if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_THERMAL_CO_TEST,INX_IEC_GPIO_OFF,0,0)){
			err=kStatus_Fail;
			errorLogsWrite(ERROR_LOGS_THERMAL_CUT_OUT);
		}

		//set all relays to off
		dutyCycleStateOffForced(&dutyCycleState0);
		dutyCycleStateOffForced(&dutyCycleState1);
		dutyCycleStateOffForced(&dutyCycleState2);
		//wait 10ms
		count=0;
		WDOG_Refresh(WDOG);
		while(count<CONFIG_STARTUP_RELAY_CHECK_TICKS){
			count++;
		}
		//check all off
		if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_ONE_TEST,INX_IEC_GPIO_ON,0,0)){
			err=kStatus_Fail;
			errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_ON);
		}
		switch(gPIDs[0].elementMode){
			case ELEMENT_MODE_DOUBLE_STEPPED:
			case ELEMENT_MODE_DOUBLE_TANDEM:
			case ELEMENT_MODE_TRIPLE_STEPPED:
			case ELEMENT_MODE_TRIPLE_TANDEM:
				if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_TWO_TEST,INX_IEC_GPIO_ON,0,0)){
					err=kStatus_Fail;
					errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_ON);
				}
				break;
			default:
				break;
		}
		switch(gPIDs[0].elementMode){
			case ELEMENT_MODE_TRIPLE_STEPPED:
			case ELEMENT_MODE_TRIPLE_TANDEM:
				if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_HEATER_THREE_TEST,INX_IEC_GPIO_ON,0,0)){
					err=kStatus_Fail;
					errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_ON);
				}
				break;
			default:
				break;
		}
		//check thermal cut out is now off
		//this is mainly so when we check on the commissioning reports everything matches up
		if(!inxIecGPIOCheck(pRelayState,INX_IEC_GPIO_THERMAL_CO_TEST,INX_IEC_GPIO_ON,0,0)){
			err=kStatus_Fail;
			errorLogsWrite(ERROR_LOGS_THERMAL_CUT_OUT);
		}
#ifdef INX_HEATROD_STARTUP_CHECK_VERBOSE
	   for (int k = 0 ; k <5 ; k++) {

		   if (~CONFIG_GPIO_SENSE_CONRELAY_BASE->PDIR & (1U << CONFIG_GPIO_SENSE_CONRELAY_1))
			   printf("XXXXXXXX - CONTROL 1 ON\n");
		   else
			   printf("XXXXXXXX - CONTROL 1 OFF\n");
		   if (~CONFIG_GPIO_SENSE_CONRELAY_BASE->PDIR & (1U << CONFIG_GPIO_SENSE_CONRELAY_2))
			   printf("XXXXXXXX - CONTROL 2 ON\n");
		   else
			   printf("XXXXXXXX - CONTROL 2 OFF\n");
		   if (~CONFIG_GPIO_SENSE_CONRELAY_BASE->PDIR & (1U << CONFIG_GPIO_SENSE_CONRELAY_3))
			   printf("XXXXXXXX - CONTROL 3 ON\n");
		   else
			   printf("XXXXXXXX - CONTROL 3 OFF\n");
	   }
#endif

	 //#define TEST_DIO_FOREVER
#ifdef TEST_DIO_FOREVER
	while(1){
		GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1));
		GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2));
		GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3));
		GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1));
		GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2));
		GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3));
		printf("ptb3 on\r\n");
		count=0;
		while(count<120000000/4){
			count++;
		}
		GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1));
		GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2));
		GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3));
		GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1));
		GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2));
		GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT,(1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3));
		printf("ptb3 off\r\n");
		count=0;
	   while(count<120000000){
		   count++;
	   }
   }
#endif
#ifdef INX_HEATROD_SHOW_CONTROL_SENSOR_OUTPUTS
	for (int k = 0 ; k <5 ; k++) {
			if (~CONFIG_GPIO_SENSE_CONRELAY_BASE->PDIR & (1U << CONFIG_GPIO_SENSE_CONRELAY_1))
				printf("--------[%d] CONTROL 1 ON\n",k);
			else
				printf("--------[%d] CONTROL 1 OFF\n",k);
			if (~CONFIG_GPIO_SENSE_CONRELAY_BASE->PDIR & (1U << CONFIG_GPIO_SENSE_CONRELAY_2))
				printf("--------[%d] CONTROL 2 ON\n",k);
			else
				printf("--------[%d] CONTROL 2 OFF\n",k);
			if (~CONFIG_GPIO_SENSE_CONRELAY_BASE->PDIR & (1U << CONFIG_GPIO_SENSE_CONRELAY_3))
				printf("--------[%d] CONTROL 3 ON\n",k);
			else
				printf("--------[%d] CONTROL 3 OFF\n",k);
			//vTaskDelay(1000); //simulate the modbus poll delay
		}
	for (int k = 0 ; k <5 ; k++) {

		if (~CONFIG_GPIO_SENSE_CO_BASE->PDIR & (1U << CONFIG_GPIO_SENSE_THERM_CO_1))
			printf("--------[%d] THERMAL POWER CUTOUT1  : ON\n",k);
		else
			printf("--------[%d] THERMAL POWER CUTOUT1  : OFF\n",k);
		/*
		 if (~CONFIG_GPIO_SENSE_CO_BASE->PDIR & (1U << CONFIG_GPIO_SENSE_THERM_CO_2))
			printf("--------[%d] THERMAL POWER CUTOUT2  : ON\n",k);
		else
			printf("--------[%d] THERMAL POWER CUTOUT2  : OFF\n",k);
		if (~CONFIG_GPIO_SENSE_CO_BASE->PDIR & (1U << CONFIG_GPIO_SENSE_THERM_CO_3))
			printf("--------[%d] THERMAL POWER CUTOUT3  : ON\n",k);
		else
			printf("--------[%d] THERMAL POWER CUTOUT3  : OFF\n",k);
		*/
		if (~CONFIG_GPIO_SENSE_CO_BASE->PDIR & (1U << CONFIG_GPIO_SENSE_EXT_CO_1))
			printf("--------[%d] THERMAL SIGNAL CUTOUT : ON\n",k);
		else
			printf("--------[%d] THERMAL SIGNAL CUTOUT : OFF\n",k);

	}
#endif
#else //INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
#endif //INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	WDOG_Refresh(WDOG);
	return err;
}

uint8_t gRamUsedPercent = 0;
void inxIecPeriodicTest(const uint32_t crcValue,const uint32_t programSizeBytes,adc16_channel_config_t* pAdc0ChnConfig,adc16_channel_config_t* pAdc1ChnConfig){
	LWIP_DEBUGF(DEBUG_IEC,("inxIecPeriodicTest: start"));
	//irqCount=0;
	err_enum_t result=ERR_OK;
	//we don't need to disable interrupts because we assume we are being run by ADC which has highest interrupt priority

	//kick watch dog to prevent premature restart
	WDOG_Refresh(WDOG);

	//only actually do the checks if we are allowed to
	switch(gSafetyMode){
		case INX_SAFETY_MODE_DISABLE_ALL_HARDWARE_CHECKS:
			return;
			break;
		default:
			break;
	}

	//force GPIO off (do we need to do this?)
	//inxIecTurnGPIOOff();

	//we need to disable the ADCs or ADC0 will interrupt and break the stack test
	inxIecDisableADCs();

	//can we test the memory available here?
	gRamUsedPercent=inxIecCalculateRAMUsedPercent();
	LWIP_DEBUGF(DEBUG_IEC,("inxIecPeriodicTest: ram used %d percent",gRamUsedPercent));

	//if a fault occurs here we need to call our shutdown routine
#if CONFIG_DO_FLASH_TEST
	result=inxIecFlashCRC(crcValue,programSizeBytes);
	if(result==ERR_OK){}else{
		LWIP_DEBUGF(DEBUG_IEC,("inxIecPeriodicTest: flash error, shutting down"));
		errorLogsWrite(ERROR_LOGS_PROGRAM_FLASH_CRC);
		inxIecFaultShutdown();
	}
#endif
	result=inxIecRamBoundaries();
	if(result==ERR_OK){}else{
		LWIP_DEBUGF(DEBUG_IEC,("inxIecPeriodicTest: ram boundaries error, shutting down"));
		errorLogsWrite(ERROR_LOGS_RAM_BOUNDARIES);
		inxIecFaultShutdown();
	}

	result=inxIecVolatileRamWalk();
	if(result==ERR_OK){}else{
		LWIP_DEBUGF(DEBUG_IEC,("inxIecPeriodicTest: memory error, shutting down"));
		errorLogsWrite(ERROR_LOGS_RAM_WALK);
		inxIecFaultShutdown();
	}

	//inxIecTurnGPIOOn();

	//kick watch dog again to allow rest of system time to carry on
	inxIecEnableADCs(pAdc0ChnConfig,pAdc1ChnConfig,false);
	inxIecSetNormalWatchDog();
	LWIP_DEBUGF(DEBUG_IEC,("inxIecPeriodicTest: done"));
}
