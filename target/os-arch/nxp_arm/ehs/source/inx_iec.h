#ifndef INX_IEC_H_
#define INX_IEC_H_

#include "heatrod_config.h"
#include "fsl_adc16.h"

typedef struct _inxIecVolatileRamWalkData {
	uint8_t* startAddress;
	uint8_t* endAddress;
	uint8_t* currentAddress;
} inxIecVolatileRamWalkData_t;

//we have to use volatile here or the compiler will optimise reads of these
//variables and that causes weird issues
typedef struct _inxIecInterruptData {
	volatile bool doingTest;
	volatile uint32_t interruptCount;
} inxIecInterruptData_t;

typedef enum {
	INX_IEC_GPIO_OFF,
	INX_IEC_GPIO_ON
} inxIecGPIOState_t;

typedef enum {
	INX_IEC_GPIO_HEATER_ONE_TEST,
	INX_IEC_GPIO_HEATER_TWO_TEST,
	INX_IEC_GPIO_HEATER_THREE_TEST,
	INX_IEC_GPIO_THERMAL_CO_TEST
} inxIecHeater_t;

typedef struct _inxWatchDogState_t {
	volatile bool deviceFailed;
	volatile bool recoverableRTCError;
	volatile uint32_t supervisorTicks;
	volatile uint32_t supervisorPreviousTicks;
	volatile uint32_t adcTicks;
	volatile uint32_t adcPreviousTicks;
} inxWatchDogState_t;

void inxIecWaitForWatchDogUpdate(WDOG_Type *base);
void inxIecDisableADCs();
bool inxIecEnableADCs(adc16_channel_config_t* pAdc0ChnConfig,adc16_channel_config_t* pAdc1ChnConfig,const bool ignoreDutyCycle);
void inxIecSetWatchDog(const bool enableWatchDog,const bool enableWindowMode,const uint32_t timeoutValue,const uint32_t windowValue);
void inxIecSetNormalWatchDog();
err_enum_t inxIecFlashCRC(const uint32_t crcValue,const uint32_t programSizeBytes);
err_enum_t inxIecSettingsCRC(permStorage_t* pSettings);
err_enum_t inxIecVolatileRamWalk();
void inxIecDiagnostic(inxIecInterruptData_t* pInterruptData,adc16_channel_config_t* adc0ChnConfig,adc16_channel_config_t* adc1ChnConfig,const uint32_t crcValue,const uint32_t programSizeBytes);
bool inxIecGPIOCheck(inxRelayCurrentState_t* pRelayState,const inxIecHeater_t heater,const inxIecGPIOState_t state,const uint32_t ticksSinceChange,const uint32_t minTicksToWait);
void inxIecUartTests();
void inxIecPeriodicTest(const uint32_t crcValue,const uint32_t programSizeBytes,adc16_channel_config_t* pAdc0ChnConfig,adc16_channel_config_t* pAdc1ChnConfig);
void inxIecFaultShutdown();
void inxIecRebootDevice();
status_t inxIecInternalRelayCheck(inxRelayCurrentState_t* pRelayState);
void inxIecShutOffAllRelays(const bool forever);
#endif
