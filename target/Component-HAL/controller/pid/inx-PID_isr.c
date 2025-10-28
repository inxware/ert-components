#include "inx-PID_isr.h"

#include "hal_gpio.h"

//#ifdef EHS_MAX31343_SUPPORT
//#include "target_specific.h"
//#endif//EHS_MAX31343_SUPPORT

// isr friendly printf (needed especally for esp32 adc isr)
#ifdef EHS_ESP32_SUPPORT
#define isr_printf(...) ets_printf(__VA_ARGS__)
#else
#define isr_printf(...) printf(__VA_ARGS__)
#endif

#define FIXED_POINT_SCALER 1024 // 2^n
//TODO consider switch to bit shift
#define FIXED_POINT_TO(n)   (ehs_sint32)((n) * FIXED_POINT_SCALER)
#define FIXED_POINT_FROM(n) (ehs_sint32)((n) / FIXED_POINT_SCALER)
#define FLOATING_FIXED_POINT_FROM(n) ((ehs_float)(n) / (ehs_float)(FIXED_POINT_SCALER))

// Safty boundries of the pid controller output // @TODO Confirm !
#define INX_SAFE_TC_TEMPERATURE_LOWER_BOUNDS FIXED_POINT_TO(0)
#define INX_SAFE_TC_TEMPERATURE_UPPER_BOUNDS FIXED_POINT_TO(1200)
#define INX_SAFE_PT100_TEMPERATURE_LOWER_BOUNDS FIXED_POINT_TO(-60)
#define INX_SAFE_PT100_TEMPERATURE_UPPER_BOUNDS FIXED_POINT_TO(600)

// ====================================================================
// PT100 resistance to temperature Look Up Table
// Input range [18, 402] Ohms
// To look up in this table, the index range is (R-18)/3 ~ (R-18)/3+1
//  , where R is the resistance of the PT100
// The temperature is 1 decimal places. The very last digit is the first
//  decimal place
// Note that this is signed 16-bit integer array. Pay attention to the 
//  type conversion during the calculation!
#define PID_PT100_TEMP_LUT_SIZE 129
static const ehs_sint16 EHS_DATA_MEMORY_ATTRIB PT100_temperature_LUT[PID_PT100_TEMP_LUT_SIZE] = {
    -2012, -1943, -1873, -1802, -1732, -1661, -1589, -1517, -1445, -1373,
    -1300, -1227, -1154, -1080, -1006, -932, -858, -783, -708, -633, -558,
    -483, -407, -331, -255, -179, -102, -26, 51, 128, 205, 283, 360, 438,
    516, 594, 672, 750, 829, 908, 987, 1066, 1145, 1225, 1304, 1384, 1465,
    1545, 1625, 1706, 1787, 1868, 1950, 2031, 2113, 2195, 2277, 2359, 2442,
    2525, 2608, 2691, 2775, 2859, 2942, 3027, 3111, 3196, 3281, 3366, 3451,
    3537, 3623, 3709, 3795, 3881, 3968, 4055, 4143, 4230, 4318, 4406, 4495,
    4583, 4672, 4761, 4851, 4941, 5031, 5121, 5212, 5302, 5394, 5485, 5577,
    5669, 5761, 5854, 5947, 6040, 6134, 6228, 6322, 6417, 6511, 6607, 6702,
    6798, 6894, 6991, 7088, 7185, 7283, 7381, 7479, 7578, 7677, 7776, 7876,
    7976, 8077, 8178, 8279, 8381, 8484, 8586, 8689, 8793, 8897
};
// ====================================================================
#define PID_TC_COEFF_LUT_SIZE 13
#define PID_TC_COEFF_LUT_OFFSET 40
#define PID_TC_COEFF_LUT_DIVIDER 10
// Thermocouple Type J millivolts to temperature Look Up Table
// Input range [-8, 70] mV.
// To look up in this table, the index range is (mV+8)/2 ~ (mV+8)/2+1
//  , where mV is the thermal voltage of the thermocouple
// The temperature is 1 decimal places. The very last digit is the first
//  decimal place
// Note that this is signed 16-bit integer array. Pay attention to the 
//  type conversion during the calculation!
#define PID_TC_J_LUT_SIZE 40
#define PID_TC_J_LUT_OFFSET 8
#define PID_TC_J_LUT_SCALE 2
#define PID_TC_J_LUT_MV_LOW -8
#define PID_TC_J_LUT_MV_HIGH 70
static const ehs_sint16 EHS_DATA_MEMORY_ATTRIB TC_J_LUT[PID_TC_J_LUT_SIZE] = {
    -2052, -1355, -850, -408, 0, 389, 765, 1134, 1498, 1860, 2220, 2580, 2941,
    3303, 3665, 4028, 4390, 4750, 5108, 5462, 5811, 6153, 6488, 6817, 7139, 7456,
    7768, 8078, 8388, 8702, 9020, 9343, 9673, 10008, 10348, 10691, 11036, 11382,
    11729, 12078
};
// TC J temperature to millivolts coefficient Look Up Table
// Input range [-40, 80] C.
// To look up in this table, the index is (T+40)/10
//  , where T is the temperature of the cold junction
// The coefficient is in 1024 fixed point. In order to convert it back,
//  need to divide the result by 1024.
static const ehs_sint16 EHS_DATA_MEMORY_ATTRIB TC_J_COEFF_LUT[PID_TC_COEFF_LUT_SIZE] = {
    49, 49, 50, 51, 52, 52, 53, 53, 54, 54, 54, 55, 55
};
// TC J temperature to millivolts Look Up Table
// Input range [-40, 80] C.
// To look up in this table, the index is (T+40)/10
//  , where T is the temperature of the cold junction
// The millivolt is in 1024 fixed point. In order to convert it back,
//  need to divide the result by 1024.
static const ehs_sint16 EHS_DATA_MEMORY_ATTRIB TC_J_MV_LUT[PID_TC_COEFF_LUT_SIZE] = {
    -2008, -1517, -1019,  -513,     0,   519,  1044,  1574,
        2108,  2647,  3190,  3737,  4287
};
// ====================================================================
// Thermocouple Type K millivolts to temperature Look Up Table
// Input range [-6, 54] mV
// To look up in this table, the index range is (mV+6)/2 ~ (mV+6)/2+1
//  , where mV is the thermal voltage of the thermocouple
// The temperature is 1 decimal places. The very last digit is the first
//  decimal place
// Note that this is signed 16-bit integer array. Pay attention to the 
//  type conversion during the calculation!
#define PID_TC_K_LUT_SIZE 31
#define PID_TC_K_LUT_OFFSET 6
#define PID_TC_K_LUT_SCALE 2
#define PID_TC_K_LUT_MV_LOW -6
#define PID_TC_K_LUT_MV_HIGH 54
static const ehs_sint16 EHS_DATA_MEMORY_ATTRIB TC_K_LUT[PID_TC_K_LUT_SIZE] = {
    -2075, -1151, -531, 0, 494, 977, 1466, 1965, 2462, 2950, 3430, 3906, 4379,
    4849, 5318, 5787, 6258, 6731, 7208, 7690, 8177, 8670, 9169, 9674, 10186,
    10706, 11234, 11771, 12320, 12883, 13460
};
// TC K temperature to millivolts coefficient Look Up Table
// Input range [-40, 80] C.
// To look up in this table, the index is (T+40)/10
//  , where T is the temperature of the cold junction
// The coefficient is in 1024 fixed point. In order to convert it back,
//  need to divide the result by 1024.
static const ehs_sint16 EHS_DATA_MEMORY_ATTRIB TC_K_COEFF_LUT[PID_TC_COEFF_LUT_SIZE] = {
    38, 38, 39, 40, 40, 41, 41, 42, 42, 42, 42, 43, 43
};
// TC K temperature to millivolts Look Up Table
// Input range [-40, 80] C.
// To look up in this table, the index is (T+40)/10
//  , where T is the temperature of the cold junction
// The millivolt is in 1024 fixed point. In order to convert it back,
//  need to divide the result by 1024.
static const ehs_sint16 EHS_DATA_MEMORY_ATTRIB TC_K_MV_LUT[PID_TC_COEFF_LUT_SIZE] = {
    -1564, -1184,  -796,  -401,     0,   406,   817,  1232,
        1650,  2072,  2495,  2920,  3345
};
// ====================================================================
// Thermocouple Type N millivolts to temperature Look Up Table
// Input range [-4, 46] mV
// To look up in this table, the index range is (mV+4)/1 ~ (mV+4)/1+1
//  , where mV is the thermal voltage of the thermocouple
// The temperature is 1 decimal places. The very last digit is the first
//  decimal place
// Note that this is signed 16-bit integer array. Pay attention to the 
//  type conversion during the calculation!
#define PID_TC_N_LUT_SIZE 51
#define PID_TC_N_LUT_OFFSET 4
#define PID_TC_N_LUT_SCALE 1
#define PID_TC_N_LUT_MV_LOW -4
#define PID_TC_N_LUT_MV_HIGH 46
static const ehs_sint16 EHS_DATA_MEMORY_ATTRIB TC_N_LUT[PID_TC_N_LUT_SIZE] = {
    -2010, -1303, -812, -391, 0, 376, 734, 1076, 1403, 1720, 2026, 2325, 2617,
    2903, 3185, 3463, 3736, 4007, 4275, 4541, 4804, 5066, 5326, 5585, 5842, 6099,
    6355, 6611, 6866, 7121, 7394, 7630, 7884, 8139, 8394, 8649, 8905, 9256, 9418,
    9676, 9934, 10193, 10453, 10715, 10977, 11241, 11506, 11773, 12041, 12311, 12584
};
// TC N temperature to millivolts coefficient Look Up Table
// Input range [-40, 80] C.
// To look up in this table, the index is (T+40)/10
//  , where T is the temperature of the cold junction
// The coefficient is in 1024 fixed point. In order to convert it back,
//  need to divide the result by 1024.
static const ehs_sint16 EHS_DATA_MEMORY_ATTRIB TC_N_COEFF_LUT[PID_TC_COEFF_LUT_SIZE] = {
    25, 26, 26, 27, 27, 27, 27, 28, 28, 28, 29, 29, 30
};
// TC N temperature to millivolts Look Up Table
// Input range [-40, 80] C.
// To look up in this table, the index is (T+40)/10
//  , where T is the temperature of the cold junction
// The millivolt is in 1024 fixed point. In order to convert it back,
//  need to divide the result by 1024.
static const ehs_sint16 EHS_DATA_MEMORY_ATTRIB TC_N_MV_LUT[PID_TC_COEFF_LUT_SIZE] = {
    -1047,  -791,  -530,  -267,    -0,   267,   538,   812,
        1090,  1372,  1658,  1948,  2241
};
// ====================================================================
//TODO Thermocouple Type T millivolts to temperature Look Up Table
// ====================================================================

// ====================================================================

int8_t EHS_MEMORY_ATTRIB inxIecControllerOutputModeToDutyCycleStateIndex(const inxControllerOutputMode_t com){
    int8_t index=-1;
    switch(com){
        case INX_CONTROLLER_OUTPUT_MODE_INTERNAL_1:
            index=INX_DUTY_CYCLE_INTERNAL_1_INDEX;
            break;
        case INX_CONTROLLER_OUTPUT_MODE_INTERNAL_2:
            index=INX_DUTY_CYCLE_INTERNAL_2_INDEX;
            break;
        case INX_CONTROLLER_OUTPUT_MODE_INTERNAL_3:
            index=INX_DUTY_CYCLE_INTERNAL_3_INDEX;
            break;
        case INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_1:
            index=INX_DUTY_CYCLE_EXTERNAL_1_INDEX;
            break;
        case INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_2:
            index=INX_DUTY_CYCLE_EXTERNAL_2_INDEX;
            break;
        case INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_3:
            index=INX_DUTY_CYCLE_EXTERNAL_3_INDEX;
            break;
        default:
            break;
    }
    return index;
}

// ====================================================================

#define INX_PID_GPIO_COUNT 6
static volatile ehs_gpio_out_state_type EHS_DATA_MEMORY_ATTRIB g_inx_gpio[INX_PID_GPIO_COUNT] = {0};
static bool g_inx_gpio_inverse[INX_PID_GPIO_COUNT] = {
    #ifdef CONFIG_GPIO_INVERSE_INTERNAL_RELAY_1
    CONFIG_GPIO_INVERSE_INTERNAL_RELAY_1,
    #else
    false,
    #endif
    #ifdef CONFIG_GPIO_INVERSE_INTERNAL_RELAY_2
    CONFIG_GPIO_INVERSE_INTERNAL_RELAY_2,
    #else
    false,
    #endif
    #ifdef CONFIG_GPIO_INVERSE_INTERNAL_RELAY_3
    CONFIG_GPIO_INVERSE_INTERNAL_RELAY_3,
    #else
    false,
    #endif
    #ifdef CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_1
    CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_1,
    #else
    false,
    #endif
    #ifdef CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_2
    CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_2,
    #else
    false,
    #endif
    #ifdef CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_3
    CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_3
    #else
    false
    #endif
};

static bool EHS_MEMORY_ATTRIB inx_pid_ctrl_gpio_update(ehs_sint32 pin_no, ehs_bool value){
    for(ehs_uint8 i = 0; i < INX_PID_GPIO_COUNT; i++){
        if(g_inx_gpio[i].pin_id == pin_no){
            g_inx_gpio[i].pin_value = value;
            //isr_printf("pin = %d , value = %d \n", pin_no, (ehs_sint32)value);
            return EhsWriteOutputGPIO(&(g_inx_gpio[i]));
        }
    }
    return false;
}

// ====================================================================

void inxModbusWriteValue(const uint32_t index, const uint16_t value){ /*stub*/ }

//#define pwmcalc_printf(...) printf(__VA_ARGS__)
#define pwmcalc_printf(...) {}

volatile inxPowerModeEnum_t gPowerMode=INX_POWER_MODE_THREE_PHASE_415;

INX_Time_State_t gTimeState = {
    INX_TIME_STATE_SUBSCRIBING,
    0,
    INX_TIME_STATE_RTC_UNSET,
    0
};

typedef enum {
    INX_GPIO_CONDITION_DIAGNOSTICS,
    INX_GPIO_CONDITION_OK,
    INX_GPIO_CONDITION_DISABLED
} inxGpioConditionStateMode_t;

typedef struct _inxGpioConditionState_t {
    inxGpioConditionStateMode_t mode;
} inxGpioConditionState_t;
#if CONFIG_LIFESTYLE == INX_LIFESTYLE_COMMISSION
#else
    static uint32_t gFalseAlarmTicks=0;
    //static RCM_Type *rcm_base = RCM;
    //static wd_test_t gsSafetyWdTest;
#endif //else if CONFIG_LIFESTYLE == INX_LIFESTYLE_COMMISSION
//volatile int32_t gGlobalPIDUpperLimit=5000; // this is now set in the function block
volatile bool gControllerOutputUpdated=false;
volatile bool gDynamicSetPointUpdated=false;
globalADCValues_t gValues;
static volatile int nValueIndex = 0;
static volatile bool gADCValuesUnsafe=false;
volatile bool gSettingsUpdatedByUser=false;
volatile inxNoFactoryResetStorage_t gNoResetSettings={0};
//struct netif fsl_netif0;
static globalADCValues_t gADCValues[2];

static volatile bool gADCValuesReady = false;
inxPID_t gPIDs[CONFIG_PID_CONTROLLERS_NUM]={0}; /* IO mappings and parameters for each PID controller */

inxSensorType_t gSensorType = {0};

//PBB not doing this for now as it is out of band functionality
//static uint32_t* gpWatchDogFlag = 0x1fff0000; //the flag that tells us when we got shut down, current points to first chunk of SRAM_LOWER

volatile dutyCycleState_t dutyCycleStates[CONFIG_DUTY_CYCLES_NUM]={0};

static dutyCycleHistory_t dutyCycleHistory0={0};
static uint8_t gAdcCurrentElementCount=0;
adcRawValues_t gADCRawValues;
static volatile inxGpioConditionState_t gGpioConditionState;
uint32_t gElement2PowerThresholdPercent=0;
uint32_t gElement3PowerThresholdPercent=0;

/*******************************************************************************
* Variables
******************************************************************************/
inxRelayCurrentState_t gRelayErrorState;
volatile static uint8_t gADCSamplesPerSecond = 0;
volatile static uint8_t gADCSamplesCount = 0;
uint32_t gLPTMRInterruptMilliseconds=CONFIG_LPTMR_DEFAULT_INTERRUPT_MILLISECONDS;
uint32_t gMinTicksSinceHeaterChange=0;
//uint32_t gTicksSinceSettingsSave=0;

volatile inxCalibration_t EHS_DATA_MEMORY_ATTRIB gCalibration={
    // A thousands of millivolts (microvolts)
    .pt100Offset = 0,
    // Parts per million
    .pt100Scale = 1,
    .pt200Offset = 0,
    .pt200Scale = 1,
    .zeroTenVOffset = 0,
    .zeroTenVScale = 1,
    .fourTwentyMAOffset = 0,
    .fourTwentyMAScale = 1,
    .thermocoupleOneOffset = 0,
    .thermocoupleOneScale = 1,
    .thermocoupleTwoOffset = 0,
    .thermocoupleTwoScale = 1
};

static bool EHS_MEMORY_ATTRIB isSensorConnected(ehs_sint32 nSensorID, const adcRawConnected_t const* pConnected)
{
    ehs_bool isConnected = EHS_FALSE;
    if(nSensorID==INX_CONTROLLER_SENSORS_PT100_CHANNEL1){
        isConnected = pConnected->bPt100Channel1;
    }
    else if(nSensorID==INX_CONTROLLER_SENSORS_PT100_CHANNEL2){
        isConnected = pConnected->bPt100Channel2;
    }
    else if(nSensorID==INX_CONTROLLER_SENSORS_10V){
        isConnected = pConnected->bZeroTenV;
    }
    else if(nSensorID==INX_CONTROLLER_SENSORS_20MA){
        isConnected = pConnected->bFourTwentyMa;
    }
    else if(nSensorID==INX_CONTROLLER_SENSORS_THERMOCOUPLE_CHANNEL1){
        isConnected = pConnected->bThermocouple1;
    }
    else if(nSensorID==INX_CONTROLLER_SENSORS_THERMOCOUPLE_CHANNEL2){
        isConnected = pConnected->bThermocouple2;
    }else{
        // unknown sensor
        isConnected = EHS_FALSE;
    }
    return isConnected;
}

static void inxLPTMRInit(){
    // @TODO - confirm that the configuration is correct
	const float lengthSeconds=1.0;
	gADCSamplesPerSecond=CONFIG_SAMPLES_PER_DUTY_CYCLE/lengthSeconds;
	//update our RTC base because the interrupt timings are going to change
	//gADCRTCLastGoodSeconds=inxRTCADCGetSeconds();
	//gADCRTCTicks=0;
	gLPTMRInterruptMilliseconds=(lengthSeconds*1000) / (CONFIG_LPTMR_NUM_READS*CONFIG_SAMPLES_PER_DUTY_CYCLE);
	//gRTCCheckTicksToWait=(CONFIG_RTC_SECONDS_BETWEEN_CHECKS*1000)/(gLPTMRInterruptMilliseconds*CONFIG_LPTMR_NUM_READS);
	gMinTicksSinceHeaterChange=CONFIG_EMR_RESPONSE_TIME_MILLISECONDS/gLPTMRInterruptMilliseconds;
	//inxLPTMRInitTriggerSource();
}

bool EHS_MEMORY_ATTRIB inxIecOutputAlreadyInUse(const u8_t pidNum){
    //check the PIDs above me in priority
    for(u8_t outputNum=0;outputNum<CONFIG_PID_OUTPUTS_NUM;outputNum++){
        inxControllerOutputMode_t myMode=gPIDs[pidNum].controllerOutputMode[outputNum];
        switch(myMode){
            case INX_CONTROLLER_OUTPUT_MODE_DISABLED:
                break;
            default:
                for(u8_t i=0;i<pidNum;i++){
                    for(u8_t j=0;j<CONFIG_PID_OUTPUTS_NUM;j++){
                        if(gPIDs[i].controllerOutputMode[j]==myMode){
                            return true;
                        }
                    }
                }
                break;
        }
    }
    return false;
}

static void EHS_MEMORY_ATTRIB inxGPIOOff(uint32_t pin_no){
    inx_pid_ctrl_gpio_update((ehs_sint32)pin_no, EHS_FALSE);
}

static bool EHS_MEMORY_ATTRIB inxGPIOOn(uint32_t pin_no){
    
    bool ok=false;
    // @TODO - review the watchdog bit
    if(gGpioConditionState.mode == INX_GPIO_CONDITION_DISABLED /*|| gWatchDogState.recoverableRTCError || gWatchDogState.deviceFailed*/){
        //ignore because we have a fault
    }else{
        ok = inx_pid_ctrl_gpio_update((ehs_sint32)pin_no, EHS_TRUE);
    }
    return ok;
}

static float EHS_MEMORY_ATTRIB dutyCycleStateGetLengthSeconds(volatile dutyCycleState_t* pState){
    if(pState->cycleInterruptLength<2){
        pState->cycleInterruptLength=2;
    }
    return pState->cycleLengthSeconds;
    /*switch(gControllerContactorMode){
        case INX_CONTROLLER_CONTACTOR_MODE_EMR:
            lengthSeconds=gDutyCycleLengthEMRSeconds;
            break;
        case INX_CONTROLLER_CONTACTOR_MODE_SSR:
            lengthSeconds=gDutyCycleLengthSSRSeconds;
            //safety check to make sure we don't apply ssr timings to mechanical output
            #if CONFIG_ALLOW_SSR_TIMINGS_ON_INTERNAL
                
            #else //CONFIG_ALLOW_SSR_TIMINGS_ON_INTERNAL
                //loop through outputs, if any are internal set to EMR timings
                for(uint8_t pidNum=0;pidNum<CONFIG_PID_CONTROLLERS_NUM;pidNum++){
                    for(uint8_t outputNum=0;outputNum<CONFIG_PID_OUTPUTS_NUM;outputNum++){
                        switch(gPIDs[pidNum].controllerOutputMode[outputNum]){
                            case INX_CONTROLLER_OUTPUT_MODE_INTERNAL_1:
                            case INX_CONTROLLER_OUTPUT_MODE_INTERNAL_2:
                            case INX_CONTROLLER_OUTPUT_MODE_INTERNAL_3:
                                lengthSeconds=gDutyCycleLengthEMRSeconds;
                                break;
                            default:
                                break;
                        }
                    }
                }
            #endif //CONFIG_ALLOW_SSR_TIMINGS_ON_INTERNAL
            break;
        default:
            break;
    }
    return lengthSeconds;*/
}

/* Stateful Relay Control Layer functions */

void EHS_MEMORY_ATTRIB dutyCycleStateOff(volatile dutyCycleState_t* pState){
    //pwmcalc_printf("\n====SWITCHING OFF (pstate=%d):\n",pState->on);
    if (pState->inverse_logic) inxGPIOOn(pState->gpioPin);
    else inxGPIOOff(pState->gpioPin);

    /* Do some state handling stuff on state transitions */
    if(pState->on){
        //pwmcalc_printf("====pState PIN=%d\n",pState->internalGPIOPin);
        pState->on = false;
        //printf("Pstat=%d\n",);
        pState->ticksSinceChange=0;
        //pwmcalc_printf("====dutyCycleStates[0]%d,%d,%d\n",dutyCycleStates[0].on ,dutyCycleStates[1].on ,dutyCycleStates[2].on);
    }
    bool on=false;
    for(uint8_t dutyCycleStateIndex=0;dutyCycleStateIndex<CONFIG_DUTY_CYCLES_NUM;dutyCycleStateIndex++){
        if(dutyCycleStates[dutyCycleStateIndex].on){
            on=true;
            break;
        }
    }
    if(on){

    }else{
        ////////////////// STUB ////////////// inxLedDutyModeSet(INX_LED_DUTY_MODE_OFF);
    }
}

static void inxPWMOff(volatile dutyCycleState_t* pState){
    if(pState->ftmEnabled){
        //FTM_StopTimer(pState->ftmBase); - nxp func
        pState->ftmEnabled=false;
        pState->powerPercent=0;
    }
}

void dutyCycleStateOffForced(volatile dutyCycleState_t* pState){
    //isr_printf("!!!!!!!!!!!!!!!! FORCED RELAYS OFF!!!!!!!!!!!!\n");
    pState->on = true; //if way we are on the dutyCycleStateOff will always turn us off
    dutyCycleStateOff(pState);
    inxPWMOff(pState);
}

/* todo EN60XXXX GPIO / relay tests should only be done in internal relay mode */
static bool gFTM2Started=false;
static void EHS_MEMORY_ATTRIB inxPWMDeinit(){
    if(gFTM2Started){
        //FTM_StopTimer(FTM2); - nxp func
        gFTM2Started=false;
    }
}

static void EHS_MEMORY_ATTRIB inxPWMInit(const float lengthSeconds){
    // stub - hardware specific
    gFTM2Started=true;
}

static void EHS_MEMORY_ATTRIB inxPWMEnable(volatile dutyCycleState_t* pState){
    const float lengthSeconds=dutyCycleStateGetLengthSeconds(pState);
    uint32_t periodHz=(1.0f/lengthSeconds);
    //round to nearest 1000 if appropriate
    if(periodHz>500){
        periodHz=((periodHz+499)/1000)*1000;
    }
    // TODO - reinitalise if not initalised
    if(!gFTM2Started){
        inxPWMDeinit();
        inxPWMInit(lengthSeconds);
    }
    pState->ftmEnabled=true;
    pState->powerPercent=0;
}

void EHS_MEMORY_ATTRIB dutyCycleStateOn(volatile dutyCycleState_t* pState){
    bool ok=false;
    const bool prevState=pState->on;
    if(pState->ftmBase!=NULL && pState->ftmEnabled){
        ok=true;
    }else if(pState->ftmBase!=NULL && pState->ftmEnabled==false && pState->cycleLengthSeconds<=CONFIG_PWM_MAX_SECONDS){
        inxPWMEnable(pState);
        ok=true;
    }else if(pState->ftmBase==NULL || pState->ftmEnabled==false){
        if (pState->inverse_logic)
        {
            inxGPIOOff(pState->gpioPin);
            ok=true;
        }
        else {
            if(inxGPIOOn(pState->gpioPin)){
            ok=true;
            }
        }
    }
    if(ok){
        if(prevState==0){
            pState->onCount++;
        }
        //inxLedDutyModeSet(INX_LED_DUTY_MODE_ON);
        pState->on = true;
        pState->ticksSinceChange=0; //reset tick count so that we wait correct time before checking state
    }
}

void dutyCycleStateResetCycle(volatile dutyCycleState_t* pState){
    pState->numInterrupts=pState->cycleInterruptLength;
}

void inxIecShutOffRelay(const inxControllerOutputMode_t com){
    const int8_t index=inxIecControllerOutputModeToDutyCycleStateIndex(com);
    if(index>0){
        dutyCycleStateOffForced(&dutyCycleStates[index]);
    }
}

void EHS_MEMORY_ATTRIB inxIecShutOffAllRelays(const bool forever){
    //turn everything off
    for(uint8_t i=0;i<CONFIG_DUTY_CYCLES_NUM;i++){
        dutyCycleStateOffForced(&dutyCycleStates[i]);
    }
    
    if(forever==true){
        //disable global ability to put the power on
        ////////////////// STUB ////////////// gWatchDogState.deviceFailed = true;
    }
}

// Not trusted client
// staggerTicks tells us if the relay should shut off early so we can test the TCO
void EHS_MEMORY_ATTRIB dutyCycleStateSetPowerPercent(volatile dutyCycleState_t* pState,int32_t powerPercent,const uint32_t staggerTicks){
    powerPercent=(powerPercent<0)?0:powerPercent;
    powerPercent=(powerPercent>100)?100:powerPercent;
    if(pState->powerPercent==powerPercent){

    }else{
        if(pState->ftmBase==NULL || pState->ftmEnabled==false){
		}else{
			//FTM_UpdatePwmDutycycle(pState->ftmBase, pState->ftmChannel, kFTM_CenterAlignedPwm, powerPercent);
			//FTM_SetSoftwareTrigger(pState->ftmBase, true);
            // @TODO - apply duty change
		}
    }
    pState->powerPercent=powerPercent;
    pState->powerOffInterruptCount=((pState->cycleInterruptLength*powerPercent)/100);
    if(pState->powerOffInterruptCount>=staggerTicks){
        pState->powerOffInterruptCount=pState->powerOffInterruptCount-staggerTicks;
    }
    //so that we will force this relay on if needed
    if(pState->ftmBase!=NULL && pState->cycleLengthSeconds<=CONFIG_PWM_MAX_SECONDS && pState->powerPercent>0){
        pState->powerOffInterruptCount=pState->cycleInterruptLength;
    }
    //printf("PWM Duty info (from %d/%d)\n",pState->powerOffInterruptCount,pState->cycleInterruptLength,powerPercent);
}

void EHS_MEMORY_ATTRIB dutyCycleStateSetPowerPercentFromCOM(const inxControllerOutputMode_t com,const int32_t powerPercent,const uint32_t staggerTicks){
    int8_t index=inxIecControllerOutputModeToDutyCycleStateIndex(com);
    if(index>=0){
        dutyCycleStateSetPowerPercent(&dutyCycleStates[index],powerPercent,staggerTicks);
    }
}


static void inxPWMConfigure(volatile dutyCycleState_t* pState){
    const float lengthSeconds=dutyCycleStateGetLengthSeconds(pState);
    //first pass to disable any PWM timers and set the correct GPIOs
    if(pState->ftmBase==NULL){
        //ignore as no pwm capability
    }else{
        inxPWMOff(pState);
        if(lengthSeconds<=CONFIG_PWM_MAX_SECONDS){
            //configure for ftm
            pState->powerPercent=0; //so that the correct power is set later by dutyCycleStateSetPowerPercent
            if(/*pState->ftmBase==FTM2*/true){
                inxPWMEnable(pState);
            }
        }else{
            // stub - check what needed here
        }
    }
}

static void inxDutyCycleStateSetCycleLengthSeconds(const bool byUser,volatile dutyCycleState_t* pState,const float lengthSeconds){
    if(pState->cycleLengthSeconds==lengthSeconds || lengthSeconds<INX_SSR_MIN_SECONDS || lengthSeconds>INX_EMR_MAX_SECONDS){

    }else{
        pState->cycleLengthSeconds=lengthSeconds;
        const uint32_t lengthMillis=(const uint32_t)(lengthSeconds*1000.0f);
        uint32_t cycleInterruptLength=lengthMillis/gLPTMRInterruptMilliseconds;
        if(cycleInterruptLength<2){
            cycleInterruptLength=2;
        }
        pState->cycleInterruptLength=cycleInterruptLength;
        inxPWMConfigure(pState);
        gControllerOutputUpdated=true;
        //inxSettingsChanged(byUser);
    }
}

static void inxDutyCycleStateSetRelayType(const bool byUser,volatile dutyCycleState_t* pState,const inxRelayType_t relayType){
    if(pState->relayType==relayType){

    }else{
        pState->relayType=relayType;
        gControllerOutputUpdated=true;
        //inxSettingsChanged(byUser);
    }
}

/*  PWM function */
void EHS_MEMORY_ATTRIB dutyCycleStateTick(volatile dutyCycleState_t* pState){
    //printf("PBB 1563 %x %d %d\n",pState,pState->cycleInterruptLength,pState->powerOffInterruptCount);
    pState->numInterrupts++;
    pState->ticksSinceChange++;/*-Ints=%d,IntLen=%d*/
    pwmcalc_printf("\n IN dCycle #=%d, Off=%d, Loop=%d --- ",pState->numInterrupts, pState->powerOffInterruptCount, pState->cycleInterruptLength);
    pwmcalc_printf(" dCycle[%d]",pState->internalGPIOPin);
    if(pState->numInterrupts >= pState->cycleInterruptLength){
        //reached end of cycle so loop round
        pState->numInterrupts=0;
        pwmcalc_printf(" #I >>>> ");
        //do we need to turn the power on?
        if(pState->numInterrupts < pState->powerOffInterruptCount){
            pwmcalc_printf("__SWITCHING ON__ IN TICK \n");
            dutyCycleStateOn(pState);
        }else{
            pwmcalc_printf("__SWITCHING OFF A__ IN TICK \n");
            dutyCycleStateOff(pState);
        }
    }else if(pState->numInterrupts >= pState->powerOffInterruptCount){
        //we need to power off for this part of the duty cycle
        pwmcalc_printf("<<<< OFF IN TICK\n");
        dutyCycleStateOff(pState);
        //printf("PBB 1579 %x %d %d %d\n",pState,pState->cycleInterruptLength,pState->powerOffInterruptCount,pState->numInterrupts);
    }
    else {
        pwmcalc_printf("<<<< NOT HANDLED IN TICK\n");
    }
    // else do nothing?
    pwmcalc_printf("\n OUT dCycle ");
}

//TODO - ramp
//we should have another variable instead of "desiredTemp" that is our current ramp affected desired temp
//use that new variable in calculateRequiredPower
//new variable is rampDesiredTemp
//count seconds since rampDesiredTemp last changed
//if we hit rampSecondsPerDegree then increment rampDesiredTemp and reset seconds counter
//reset rampDesiredTemp to currentTemp on updateDesiredTemp

//input controller, output mode, relay set, set point
//we can then call this method for each controller
static void EHS_MEMORY_ATTRIB calculateRequiredPower(const u8_t pidNum,inxPID_t* pPID,const bool globalLimitReached){
    //SW_DIAG_printf("****** Desired temp =%f, current temp = %f \n",desiredTemp,currentTemp);
    //LWIP_PLATFORM_DIAG(("calculateRequiredPower"));
    uint32_t powerPercent = 0; //default to off
    const int32_t desiredTemp=pPID->desiredTemp;

    //do we need to do anything?
    //if(pPID->currentValue<desiredTemp){
        if(pPID->sensors==INX_CONTROLLER_SENSORS_NONE || pPID->scheduleMode==SCHEDULE_MODE_DISABLED || (globalLimitReached==true) ||
           !isSensorConnected(pPID->sensors, &(gADCRawValues.connected)) ){ 
            //output is disabled so do no calculations
            powerPercent=0;
        }else{
            //get difference between current temp and our desired temp
            const int32_t p=desiredTemp-pPID->currentValue;

            //for PID control our calculation is as follows
            //(P*kp) + (I*ki) + (D*kd)
            //where P = desiredTemp - currentTemp (error)
            //kp = coefficient of P from user
            //I = integral of P (error) e.g Pn+P(n+1)+P(n+2)...
            //ki = coefficient of I from user
            //D = differential of P e.g P(n+1)-P(n)
            //kd = coefficient of D from user

            pPID->d=p-pPID->p; //differential of p
            pPID->i=pPID->i+p; //integrate p
            pPID->p=p;

            //apply safety limits
            if(pPID->i > pPID->nIMax){
                pPID->i = pPID->nIMax;
            }else if(pPID->i < -pPID->nIMax){
                pPID->i = -pPID->nIMax;
            }

            const int32_t pid = FIXED_POINT_FROM((pPID->p*pPID->coefficients.kp) + (pPID->i*pPID->coefficients.ki) + (pPID->d*pPID->coefficients.kd));
            //#define INX_HEATROD_VERBOSE_DEBUG
            #ifdef INX_HEATROD_VERBOSE_DEBUG
                    isr_printf("SW DIAG: Controller %d info: desired=%d curr=%d p=%d kp=%d i=%d ki=%d d=%d kd=%d PID=%d\n",
                            pidNum, desiredTemp,pPID->currentValue,pPID->p,pPID->coefficients.kp,pPID->i,pPID->coefficients.ki,pPID->d,pPID->coefficients.kd,pid);
            #endif
            //turn that in to a percentage - not sloppy
            if (pid <= 0 ) powerPercent = 0;
            else if (pid >= FIXED_POINT_TO(100)) powerPercent = 100;
            else powerPercent = (int32_t) FIXED_POINT_FROM(pid);
        }
    //}
    //SW_DIAG_printf("SW DIAG: DUTY = %d **********\n",powerPercent);
    pPID->powerPercent=powerPercent;

    //this needs to work out based on our thresholds which elements to run at which power
    
    uint32_t minTicksSinceHeaterChange=0;
    #if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi // TODO - we may want to comment this out for now
        //in 3 phase delta mode we need to stagger the relays turning off so we can test TCO
        //but don't do this in 100 percent duty as it will cause more wear on the relays
        //unless the relays have been stuck on a long time
        bool stagger=false;
        if(gPowerMode==INX_POWER_MODE_THREE_PHASE_415){
            if(powerPercent<100){
                stagger=true;
            }else{
                int8_t relay2Index=inxIecControllerOutputModeToDutyCycleStateIndex(pPID->controllerOutputMode[1]);
                int8_t relay3Index=inxIecControllerOutputModeToDutyCycleStateIndex(pPID->controllerOutputMode[2]);
                switch(pPID->elementMode){
                    case ELEMENT_MODE_SINGLE_TWO_POLE:
                    //case ELEMENT_MODE_DOUBLE_STEPPED:
                    case ELEMENT_MODE_DOUBLE_TANDEM:
                        if(relay2Index>=0){
                            if(dutyCycleStates[relay2Index].ticksSinceChange>=(dutyCycleStates[relay2Index].cycleInterruptLength*CONFIG_DUTY_CYCLES_AT_FULL_POWER_BEFORE_FORCING_RELAY_OFF_TO_CHECK_TCO)){
                                stagger=true;
                            }
                        }
                        break;
                    case ELEMENT_MODE_TRIPLE_TANDEM:
                    //case ELEMENT_MODE_TRIPLE_STEPPED:
                        if(relay2Index>=0 && relay3Index>=0){
                            if(	dutyCycleStates[relay2Index].ticksSinceChange>=(dutyCycleStates[relay2Index].cycleInterruptLength*CONFIG_DUTY_CYCLES_AT_FULL_POWER_BEFORE_FORCING_RELAY_OFF_TO_CHECK_TCO) || 
                                dutyCycleStates[relay3Index].ticksSinceChange>=(dutyCycleStates[relay3Index].cycleInterruptLength*CONFIG_DUTY_CYCLES_AT_FULL_POWER_BEFORE_FORCING_RELAY_OFF_TO_CHECK_TCO)){
                                stagger=true;
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        if(stagger){
            minTicksSinceHeaterChange=gMinTicksSinceHeaterChange;
            if(minTicksSinceHeaterChange==0){
                minTicksSinceHeaterChange=1;
            }
        }
    #endif //INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi

    bool outputIsInUse=false;
    switch(pPID->sensors){
        case INX_CONTROLLER_SENSORS_NONE:
            //don't apply power settings if we are not sensing anything
            break;
        default:
            //we must make sure our outputs are not already in use
            outputIsInUse=inxIecOutputAlreadyInUse(pidNum);
            if(outputIsInUse){
                //don't do anything
            }else{
                switch(pPID->elementMode){ // TODO - create a parameter for this
                    case ELEMENT_MODE_SINGLE:
                        //all power is ran through the first element
                        gAdcCurrentElementCount=gAdcCurrentElementCount+1;
                        dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[0],powerPercent,0);
                        break;
                    //case ELEMENT_MODE_DOUBLE_STEPPED:
                    //     gAdcCurrentElementCount=gAdcCurrentElementCount+2;
                    //     if(gElement2PowerThresholdPercent>0 && powerPercent>=gElement2PowerThresholdPercent){
                    //         //run first element at full power, the rest goes through element 2
                    //         int32_t leftOverPower=(powerPercent-gElement2PowerThresholdPercent) * 100;
                    //         int32_t powerRange=(100-gElement2PowerThresholdPercent) * 100;
                    //         int32_t secondElementPowerPercent = leftOverPower/powerRange;
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[0],100,0);
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[1],secondElementPowerPercent,minTicksSinceHeaterChange);
                    //     }else{
                    //         int32_t firstElementPowerPercent=(100*powerPercent) / (100*gElement2PowerThresholdPercent);
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[0],firstElementPowerPercent,0);
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[1],0,minTicksSinceHeaterChange);
                    //     }
                    //     break;
                    case ELEMENT_MODE_SINGLE_TWO_POLE:
                    case ELEMENT_MODE_DOUBLE_TANDEM:
                        gAdcCurrentElementCount=gAdcCurrentElementCount+2;
                        //run both elements at the same power
                        dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[0],powerPercent,0);
                        dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[1],powerPercent,minTicksSinceHeaterChange);
                        break;
                    //case ELEMENT_MODE_TRIPLE_STEPPED:
                    //     gAdcCurrentElementCount=gAdcCurrentElementCount+3;
                    //     if(gElement3PowerThresholdPercent>0 && powerPercent>=gElement3PowerThresholdPercent){
                    //         //first element 1 and element 2 at full power, the rest goes through element 3
                    //         //calculate our power range
                    //         int32_t powerRange = (100 - gElement3PowerThresholdPercent) * 100;
                    //         //calculate how much power is left over
                    //         int32_t leftOverPower = (powerPercent-gElement3PowerThresholdPercent) * 100;
                    //         //turn that in to a percentage of our power range
                    //         int32_t thirdElementPowerPercent=(100*leftOverPower) / (100*powerRange);
                    //         
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[0],100,0);
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[1],100,minTicksSinceHeaterChange);
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[2],thirdElementPowerPercent,minTicksSinceHeaterChange);
                    //     }else if(gElement2PowerThresholdPercent>0 && powerPercent>=gElement2PowerThresholdPercent){
                    //         //run first element at full power, the rest goes through element 2
                    //         //calculate the range of power that element 2 handles
                    //         int32_t powerRange = gElement3PowerThresholdPercent - gElement2PowerThresholdPercent;
                    //         //calculate how much power is left
                    //         int32_t leftOverPower = powerPercent - gElement2PowerThresholdPercent;
                    //         //turn that in to a percentage of our power range
                    //         int32_t secondElementPowerPercent=(100*leftOverPower) / (100*powerRange);
                    //         
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[0],100,0);
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[1],secondElementPowerPercent,minTicksSinceHeaterChange);
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[2],0,minTicksSinceHeaterChange); //turn off element 3
                    //     }else{
                    //         //turn off element 2 and 3, all power goes through element 1
                    //         int32_t firstElementPowerPercent=(100*powerPercent) / (100*gElement2PowerThresholdPercent);
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[0],firstElementPowerPercent,0);
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[1],0,minTicksSinceHeaterChange); //turn off element 2
                    //         dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[2],0,minTicksSinceHeaterChange); //turn off element 3
                    //     }
                    //     break;
                    case ELEMENT_MODE_TRIPLE_TANDEM:
                        gAdcCurrentElementCount=gAdcCurrentElementCount+3;
                        //run all 3 elements at the same power
                        //SW_DIAG_printf("SW DIAG: TRIPLE MODE SET Power Perecent (%d)\n",powerPercent);
                        dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[0],powerPercent,0);
                        dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[1],powerPercent,minTicksSinceHeaterChange);
                        dutyCycleStateSetPowerPercentFromCOM(pPID->controllerOutputMode[2],powerPercent,minTicksSinceHeaterChange);
                        break;
                    default:
                        isr_printf("SW DIAG: ERROR elementMode not found %d\n",pPID->elementMode);
                        gAdcCurrentElementCount=1;
                        break;
                }//switch(pPID->elementMode)
            }//if(outputIsInUse)
            break; //default
    }//switch(pPID->sensors)
}

/* todo exctend this to have +/- control values and user configurable values */
//IEC 60730 H.11.12.7 7.2.1 Analog A/D and D/A converters (plausibility) H.2.18.13
//IEC 60730 H.11.12.7 7.2.2 Analog multiplexer (ignored because we don't have one)
static bool pidValuesSafe(inxPID_t* pPID){
    bool safe=false;
    // check if the sensor is connected
    switch (pPID->sensors)
    {
        case INX_CONTROLLER_SENSORS_PT100_CHANNEL1:
        case INX_CONTROLLER_SENSORS_PT100_CHANNEL2:{
            safe = (pPID->currentValue>=INX_SAFE_PT100_TEMPERATURE_LOWER_BOUNDS && pPID->currentValue<=INX_SAFE_PT100_TEMPERATURE_UPPER_BOUNDS);
            break;
        }
        case INX_CONTROLLER_SENSORS_THERMOCOUPLE_CHANNEL1:
        case INX_CONTROLLER_SENSORS_THERMOCOUPLE_CHANNEL2:{
            safe = (pPID->currentValue>=INX_SAFE_TC_TEMPERATURE_LOWER_BOUNDS && pPID->currentValue<=INX_SAFE_TC_TEMPERATURE_UPPER_BOUNDS);
            break;
        }
        case INX_CONTROLLER_SENSORS_NONE:
        case INX_CONTROLLER_SENSORS_MODBUS:
        case INX_CONTROLLER_SENSORS_10V:
        case INX_CONTROLLER_SENSORS_20MA:{
            /* @TODO - do any limits for these sensors */
            safe=true;
            break;
        }
        default:{
            break;
        }
    }
    
    return safe;
}

#define PT100_RBIAS (4700.0f*1.00f)
#define PT100_RREF (100.0f*1.00f)
#define PT100_RF (2200.0f*1.00f)
#define PT100_GAIN 23.0f
#define PT100_TEMPCOEF	0.385f
//#define ADC_FSD 0xFFFF - in 16 bit mode
#define ADC_FSD 0x0FFF

#ifndef TGT_PT100_ADC_OFFSET_FIXEDPOINT 
#define TGT_PT100_ADC_OFFSET_FIXEDPOINT 0//395640 // round(10**(11.74/20)*1024*100)
#endif

#ifndef EHS_PT100_AMP_GAIN_SCALE
    #define EHS_PT100_AMP_GAIN_SCALE 1
    #warning "Setting EHS_PT100_AMP_GAIN_SCALE to default (1)"
#endif

#ifndef EHS_PT100_AMP_GAIN_DIVIDER
    #define EHS_PT100_AMP_GAIN_DIVIDER 1
    #warning "Setting EHS_PT100_AMP_GAIN_DIVIDER to default (1)"
#endif

#ifndef EHS_THERMOCOUPLE_ADC_MAX_FP
    #define EHS_THERMOCOUPLE_ADC_MAX_FP 1843200
    #warning "Setting EHS_THERMOCOUPLE_ADC_MAX to default (1843200)"
#endif

#ifndef EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM
    #define EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM 1
    #warning "Setting EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM to default (1)"
#endif

#ifndef EHS_THERMOCOUPLE_GAIN_SHIFT_BITS
    #define EHS_THERMOCOUPLE_GAIN_SHIFT_BITS 0
    #warning "Setting EHS_THERMOCOUPLE_GAIN_SHIFT_BITS to default (0)"
#endif

#if (EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM) > (((1<<31) - 1) / EHS_THERMOCOUPLE_ADC_MAX_FP)
    #error "EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM exceeds maximum!"
#elif (EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM) <= 0
    #error "EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM below minimum (1)"
#endif

#if (EHS_THERMOCOUPLE_GAIN_SHIFT_BITS) < 0
    #error "EHS_THERMOCOUPLE_GAIN_SHIFT_BITS below minimum (0)"
#endif

// Scale factors should be Part-Per-Million (times 1024)
#ifndef TGT_PT100_ADC_SCALE_FIXEDPOINT
//#define TGT_PT100_ADC_SCALE_FIXEDPOINT 4036 // round((20/6.8+1) * 1024)
//#define TGT_PT100_ADC_SCALE_FIXEDPOINT 7652 // round((20/3.09+1) * 1024)
//#define TGT_PT100_ADC_SCALE_FIXEDPOINT 2731 // round((20/12+1) * 1024)
#define TGT_PT100_ADC_SCALE_FIXEDPOINT EHS_PT100_AMP_GAIN_SCALE // round((20/12+1) * 1024)
#endif

// This is to replace the division with multiplication
// Scale factors should be Part-Per-Million (times 1024)
#ifndef TGT_PT100_ADC_DIVIDER_SCALE_FIXEDPOINT
//#define TGT_PT100_ADC_DIVIDER_SCALE_FIXEDPOINT 268 // round(1/((20/6.8+1)) * 1024)
//#define TGT_PT100_ADC_DIVIDER_SCALE_FIXEDPOINT 137 // round(1/((20/3.09+1)) * 1024)
//#define TGT_PT100_ADC_DIVIDER_SCALE_FIXEDPOINT 384 // round(1/((20/12+1)) * 1024)
#define TGT_PT100_ADC_DIVIDER_SCALE_FIXEDPOINT EHS_PT100_AMP_GAIN_DIVIDER // round(1/((20/12+1)) * 1024)
#endif

#define PT100_COEFF_FIXEDPOINT 2666 // 1/0.384*1024

#define PT100_CALI_OFFSET_DIVIDER_SCALE_FP_FP 40854 // round(3/(51+26)*1024*1024)

/* converts 12 bit adc value to temperature for pt100 sensor using processor board pt100 input */
int32_t EHS_MEMORY_ATTRIB convert_pt100_to_temperature_v2(const int32_t raw_value, const int32_t offset, const int32_t scale, bool calibrate) {

//	float voltage     = ((float)(raw_value)/(float)ADC_FSD)*3.3f; // 12 bit ADC value with 3.3v reference
//	float pt100_resistance = (-PT100_RREF*PT100_RBIAS*voltage)/(PT100_RREF*voltage-3.3f*PT100_RF-3.3f*PT100_RREF);
//	float temperature = ((pt100_resistance - (PT100_RREF))/(PT100_TEMPCOEF+scale)) + offset;
    /* cal1 is offfset in degC after adjusting for gain.
     * cal2 is gain adjustment +/- from nominal value */

    // @TODO - apply correct conversions
    if (TGT_PT100_ADC_SCALE_FIXEDPOINT <= 0)
    {
        // Scale should not be less or equal to 0.
        //TODO Report error and exit
        isr_printf("REPORT ERROR. ZERO or MINUS SCALE\n");
        return -999*FIXED_POINT_SCALER;
    }
    #if 0
    // ehs_sint32 _resistance = ((/*FIXED_POINT_TO(*/raw_value/*)*/ + offset)) * TGT_PT100_ADC_DIVIDER_SCALE_FIXEDPOINT;
    ehs_sint32 _resistance = FIXED_POINT_FROM(raw_value * TGT_PT100_ADC_DIVIDER_SCALE_FIXEDPOINT + FIXED_POINT_FROM((int64_t)offset * (int64_t)PT100_CALI_OFFSET_DIVIDER_SCALE_FP_FP) * 10);
    // It returns the resistance to the normal 1st order fixed point
    ehs_sint32 _temperature = FIXED_POINT_FROM((_resistance - FIXED_POINT_TO(100)) * PT100_COEFF_FIXEDPOINT);
    // isr_printf("raw:%d r:%d t:%d\n", raw_value, _resistance, _temperature);
    return FIXED_POINT_FROM(_temperature * scale);
    #else
    ehs_sint32 _voltage = raw_value;
    int32_t offset_ = offset;
    int32_t scale_ = scale;
    if (calibrate)
    {
        offset_ = 0;
        scale_ = FIXED_POINT_TO(1);
    }
    ehs_sint32 _resistance = FIXED_POINT_FROM(_voltage * TGT_PT100_ADC_DIVIDER_SCALE_FIXEDPOINT + FIXED_POINT_FROM((int64_t)offset_ * (int64_t)PT100_CALI_OFFSET_DIVIDER_SCALE_FP_FP) * 10);
    ehs_sint32 _index = (FIXED_POINT_FROM(_resistance) - 18) / 3;
    if (_index >= PID_PT100_TEMP_LUT_SIZE - 1)
        return (ehs_sint32)PT100_temperature_LUT[PID_PT100_TEMP_LUT_SIZE - 1];
    if (_index < 0)
        return (ehs_sint32)PT100_temperature_LUT[0];
    ehs_sint32 _res_low = _index * 3 + 18;
    if (_resistance == _res_low)
        return (ehs_sint32)PT100_temperature_LUT[_index];
    ehs_sint32 _res_high = _res_low + 3;
    ehs_sint32 _temp_low = PT100_temperature_LUT[_index];
    ehs_sint32 _temp_high = PT100_temperature_LUT[_index + 1];
    ehs_sint32 _grad = FIXED_POINT_TO(_temp_high - _temp_low) / (_res_high - _res_low);
    // PT100_temperature_LUT
    int32_t _temperature = (FIXED_POINT_FROM(_grad * (_resistance - FIXED_POINT_TO(_res_low))) + FIXED_POINT_TO(_temp_low)) / 10;
    // isr_printf("resistance: %d temp: %d grad[%d] res_low[%d] temp_low[%d]\n", _resistance, _temperature, _grad, _res_low, _temp_low);
    return FIXED_POINT_FROM(_temperature * scale_);
    #endif
}

int32_t convert_pt100_3W_to_temperature(const int32_t raw_value, const int32_t offset, const int32_t scale) {
    // NXP device conversion
    // i = 3/4700 = 0.64mA
    // V = I(100+tempC*0.375)
    // tempC = (V/0.00064-100)/0.375
//	float temperature = offset + (((float)raw_value/4096.0f)*3.30f-0.84074f)/(0.003277778f+scale/1000.0f);
//	float temperature = offset + (((float)raw_value/4096.0f)*3.30f-0.84074f)/(0.003223f+scale/1000.0f); // 0.325 need to check where these numbers came from in CAD!

    // @TODO - apply correct conversions
    int32_t voltage = raw_value;
    int32_t temperature = voltage;
    return temperature;
}

#define XERO10V_HIGH_RESISTOR 2200.0
#define XERO10V_LOW_RESISTOR 1000.0

#ifndef EHS_PID_10V_AMP_GAIN_DIVIDER_FACTOR
#define EHS_PID_10V_AMP_GAIN_DIVIDER_FACTOR FIXED_POINT_TO(1)
#endif//EHS_PID_10V_AMP_GAIN_DIVIDER_FACTOR

// Correct the gain value due to the fixed point quantisation error
#ifndef EHS_PID_10V_AMP_GAIN_CORRECTION_FACTOR
#define EHS_PID_10V_AMP_GAIN_CORRECTION_FACTOR FIXED_POINT_TO(1)
#endif//EHS_PID_10V_AMP_GAIN_CORRECTION_FACTOR

static int32_t EHS_MEMORY_ATTRIB convert10VToTemperature(const int32_t in, const int32_t offset, const int32_t scale, bool calibrate){
    // NXP device conversion
    //float temp=(float)((((float)(in*100))/4096.0)*(XERO10V_HIGH_RESISTOR+XERO10V_LOW_RESISTOR)/(XERO10V_LOW_RESISTOR*3.3f));
    //float adjusted=(temp*(scale+1))+offset;

    // @TODO - apply correct conversions
    int32_t adjusted = 0;
    int32_t voltage = in;
    int32_t offset_ = offset;
    int32_t scale_ = scale;
    if (calibrate)
    {
        offset_ = 0;
        scale_ = FIXED_POINT_TO(1);
    }
    adjusted = FIXED_POINT_FROM(voltage * EHS_PID_10V_AMP_GAIN_DIVIDER_FACTOR) + offset_;
    adjusted = FIXED_POINT_FROM(adjusted * EHS_PID_10V_AMP_GAIN_CORRECTION_FACTOR);
    return FIXED_POINT_FROM(FIXED_POINT_FROM(adjusted * scale_));
}

#ifndef EHS_PID_20MA_AMP_GAIN_DIVIDER_FACTOR
#define EHS_PID_20MA_AMP_GAIN_DIVIDER_FACTOR FIXED_POINT_TO(1)
#endif//EHS_PID_20MA_AMP_GAIN_DIVIDER_FACTOR

// Correct the gain value due to the fixed point quantisation error
#ifndef EHS_PID_20MA_AMP_GAIN_CORRECTION_FACTOR
#define EHS_PID_20MA_AMP_GAIN_CORRECTION_FACTOR FIXED_POINT_TO(1)
#endif//EHS_PID_20MA_AMP_GAIN_CORRECTION_FACTOR

// Compensate the ambient temperature difference between the inner and outer enclosure env for reference temperature sensor
#ifndef EHS_REFERENCE_TEMP_DIFF
#define EHS_REFERENCE_TEMP_DIFF FIXED_POINT_TO(0.875)
#endif//EHS_REFERENCE_TEMP_DIFF

static int32_t EHS_MEMORY_ATTRIB convert20MAToTemperature(const int32_t in, const int32_t offset, const int32_t scale, bool calibrate){
    // NXP device conversion
    //float temp=(float)(((float)((in-819)*100))/3277.0);
    //float adjusted=(temp*(scale+1))+offset;

    // @TODO - apply correct conversions
    int32_t adjusted = 0;
    int32_t voltage = in;
    int32_t offset_ = offset;
    int32_t scale_ = scale;
    if (calibrate)
    {
        offset_ = 0;
        scale_ = FIXED_POINT_TO(1);
    }
    adjusted = FIXED_POINT_FROM(voltage * EHS_PID_20MA_AMP_GAIN_DIVIDER_FACTOR) + offset_;
    adjusted = FIXED_POINT_FROM(adjusted * EHS_PID_20MA_AMP_GAIN_CORRECTION_FACTOR);
    return FIXED_POINT_FROM(adjusted * scale_);
}

static int32_t convertModbusToTemperature(const uint32_t in){
    return in;
}

/* Prexision opa mp gain (R1/R2) * 2
 * Vo (U6) Type K
1000/4.7 *2 * 0.0406 = 17.1277mV/C (0-190C range)
470/4.7 *2 * 0.0406 = 8.12mV/C (0-400C range)
412/4.7 *2 * 0.0406 = 7.118mV/C (0-463C range)
 */

#ifdef INX_HEATROD_SAMPLES_1_2

#define INX_HEATROD_HRC_THERMOCOUPLE_ADC_VREF_MV 3300.0f
#define INX_HEATROD_HRC_THERMOCOUPLE_MV_PER_C 17.1277f
#else
// These the new production unit (PCBCart) units.
#define INX_HEATROD_HRC_THERMOCOUPLE_ADC_VREF_MV 3300.0f
//#define INX_HEATROD_HRC_THERMOCOUPLE_MV_PER_C 7.118f
#define INX_HEATROD_HRC_THERMOCOUPLE_MV_PER_C 7.29595f
#endif
#ifndef INX_HEATROD_HRC_THERMOCOUPLE_AVG_OPAMP_OFFSET_MV
#define INX_HEATROD_HRC_THERMOCOUPLE_AVG_OPAMP_OFFSET_MV 115279  //(INX_HEATROD_HRC_THERMOCOUPLE_MV_PER_C*10)
#endif
// This is to ensure that the pre-defined numbers are non-negative hence making the instruction optimisation later valid
#if INX_HEATROD_HRC_THERMOCOUPLE_AVG_OPAMP_OFFSET_MV < 0
    #error "INX_HEATROD_HRC_THERMOCOUPLE_AVG_OPAMP_OFFSET_MV must be a non-negative number!"
#endif
#if EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM <= 0
    #error "EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM must be a positive number"
#endif
#define INX_TEMPERARY_TC

#ifndef EHS_THERMOCOUPLE_RESULT_OFFSET
#define EHS_THERMOCOUPLE_RESULT_OFFSET 0
#endif//EHS_THERMOCOUPLE_RESULT_OFFSET

// Reference Temperature in fixed point (*1024)
ehs_sint32 gRefTemperature = FIXED_POINT_TO(23);
// 1000/4.7 *2 * 0.0406 = 17.1277mV/C
static int32_t EHS_MEMORY_ATTRIB convertThermocoupleToTemperature(const int32_t in, const inxThermoCoupleType_t tc_type, const int32_t offset, const int32_t scale, bool calibrate){
    //float temp = (((float)in/4096.0f)*INX_HEATROD_HRC_THERMOCOUPLE_ADC_VREF_MV-INX_HEATROD_HRC_THERMOCOUPLE_AVG_OPAMP_OFFSET_MV)/INX_HEATROD_HRC_THERMOCOUPLE_MV_PER_C;
    //float adjusted = (temp*(scale+1))+offset;
    //printf ("ADC= %d, Thermocouple Temp (10mV/C) = %f\r\n",in, temperature);

    // @TODO - apply correct conversions
    int32_t adjusted = 0;
    // voltage should be the fixed point value
    //TODO convert the input to the actual mV voltage as per the circuit
    // Voltage is in fixed point (*1024)
    //int32_t voltage = FIXED_POINT_TO(FIXED_POINT_TO(in)) / FIXED_POINT_TO(175.319149) + offset;
    //int32_t voltage = (/*FIXED_POINT_TO(*/in/*)*/) / 34 + offset;//(175) + offset;
    // Given ambient temperature 20C, delta T = 0 for offset calibration
    
    //int32_t voltage = ((in - INX_HEATROD_HRC_THERMOCOUPLE_AVG_OPAMP_OFFSET_MV) * EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM) / (1 << EHS_THERMOCOUPLE_GAIN_SHIFT_BITS);
    // The following will generate one less instruction than the one above in the assembly given both `in` and `INX_HEATROD_HRC_THERMOCOUPLE_AVG_OPAMP_OFFSET_MV` are positive
    int32_t voltage = (
        (int32_t)(((uint32_t)(in * EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM)) / (1 << EHS_THERMOCOUPLE_GAIN_SHIFT_BITS)) - 
        (int32_t)(((uint32_t)(INX_HEATROD_HRC_THERMOCOUPLE_AVG_OPAMP_OFFSET_MV * EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM)) / (1 << EHS_THERMOCOUPLE_GAIN_SHIFT_BITS))
    );
    
    int32_t vv = voltage;
    ehs_sint32 mV_offset = 0;
    ehs_sint32 mV_divider = 1;
    ehs_sint32 mV_low = 0;
    ehs_sint32 mV_high = 0;
    ehs_sint16 *pLUT = NULL;
    ehs_sint16 *pCoeffLUT = NULL;
    ehs_sint16 *pMvLUT = NULL;
    ehs_uint8 lut_length = 40;
    ehs_sint32 _index = 0;
    switch (tc_type) {
        case INX_CONTROLLER_TC_TYPE_J:
            // Set the Calculation parameters
            mV_offset = PID_TC_J_LUT_OFFSET;
            mV_divider = PID_TC_J_LUT_SCALE;
            mV_low = PID_TC_J_LUT_MV_LOW;
            mV_high = PID_TC_J_LUT_MV_HIGH;
            pLUT = TC_J_LUT;
            pCoeffLUT = TC_J_COEFF_LUT;
            pMvLUT = TC_J_MV_LUT;
            lut_length = PID_TC_J_LUT_SIZE;
            break;
        case INX_CONTROLLER_TC_TYPE_K:
#ifdef INX_TEMPERARY_TC
TAG_INX_TEMPERARY_TC_FLAG:
#endif
            // Set the Calculation parameters
            mV_offset = PID_TC_K_LUT_OFFSET;
            mV_divider = PID_TC_K_LUT_SCALE;
            mV_low = PID_TC_K_LUT_MV_LOW;
            mV_high = PID_TC_K_LUT_MV_HIGH;
            pLUT = TC_K_LUT;
            pCoeffLUT = TC_K_COEFF_LUT;
            pMvLUT = TC_K_MV_LUT;
            lut_length = PID_TC_K_LUT_SIZE;
            break;
        case INX_CONTROLLER_TC_TYPE_N:
            // Set the Calculation parameters
            mV_offset = PID_TC_N_LUT_OFFSET;
            mV_divider = PID_TC_N_LUT_SCALE;
            mV_low = PID_TC_N_LUT_MV_LOW;
            mV_high = PID_TC_N_LUT_MV_HIGH;
            pLUT = TC_N_LUT;
            pCoeffLUT = TC_N_COEFF_LUT;
            pMvLUT = TC_N_MV_LUT;
            lut_length = PID_TC_N_LUT_SIZE;
            break;
//        case INX_CONTROLLER_TC_TYPE_T:
//            // Set the Calculation parameters
//            mV_offset = PID_TC_T_LUT_OFFSET;
//            mV_divider = PID_TC_T_LUT_SCALE;
//            mV_low = PID_TC_T_LUT_MV_LOW;
//            mV_high = PID_TC_T_LUT_MV_HIGH;
//            pLUT = TC_T_LUT;
//            lut_length = PID_TC_T_LUT_SIZE;
//            break;
//        case INX_CONTROLLER_TC_TYPE_R:
//            // Set the Calculation parameters
//            mV_offset = PID_TC_R_LUT_OFFSET;
//            mV_divider = PID_TC_R_LUT_SCALE;
//            mV_low = PID_TC_R_LUT_MV_LOW;
//            mV_high = PID_TC_R_LUT_MV_HIGH;
//            pLUT = TC_R_LUT;
//            lut_length = PID_TC_R_LUT_SIZE;
//            break;
//        case INX_CONTROLLER_TC_TYPE_S:
//            // Set the Calculation parameters
//            mV_offset = PID_TC_S_LUT_OFFSET;
//            mV_divider = PID_TC_S_LUT_SCALE;
//            mV_low = PID_TC_S_LUT_MV_LOW;
//            mV_high = PID_TC_S_LUT_MV_HIGH;
//            pLUT = TC_S_LUT;
//            lut_length = PID_TC_S_LUT_SIZE;
//            break;
        case INX_CONTROLLER_TC_TYPE_NONE:
        case INX_CONTROLLER_TC_TYPE_NOTDEFINED:
        default:
            #ifdef INX_TEMPERARY_TC
            // Temperary default to type K
            goto TAG_INX_TEMPERARY_TC_FLAG;
            #else
            isr_printf("Thermocouple type not defined\n");
            return FIXED_POINT_TO(-999);
            break;
            #endif
    }
    #if 1
    #ifdef EHS_MAX31343_SUPPORT
    // Compensate the temperature difference between the inner and outter air of the device enclosure
    gRefTemperature = gEhsAmbientTemp - EHS_REFERENCE_TEMP_DIFF;
    #endif//EHS_MAX31343_SUPPORT
    if (calibrate) goto calc_temperature;
    // Add compensated voltage from the cold-junction (ambient) temperature
    _index = (FIXED_POINT_FROM(gRefTemperature) + PID_TC_COEFF_LUT_OFFSET) / PID_TC_COEFF_LUT_DIVIDER;
    if (_index >= PID_TC_COEFF_LUT_SIZE) _index = PID_TC_COEFF_LUT_SIZE - 1;
    voltage += FIXED_POINT_FROM((gRefTemperature - FIXED_POINT_TO(_index * PID_TC_COEFF_LUT_DIVIDER - PID_TC_COEFF_LUT_OFFSET)) * pCoeffLUT[_index]) + pMvLUT[_index];
    // Add the mV offset from the temperature offset
    #if 0 // Switch between the non-20C and 20C compensation method
    //  Note that the offset calibration uses J-type only
    ehs_sint32 temp_with_offset = offset;
    _index = (temp_with_offset + FIXED_POINT_TO(PID_TC_COEFF_LUT_OFFSET)) / FIXED_POINT_TO(PID_TC_COEFF_LUT_DIVIDER);
    if (_index >= PID_TC_COEFF_LUT_SIZE) _index = PID_TC_COEFF_LUT_SIZE - 1;
    voltage += FIXED_POINT_FROM((temp_with_offset - FIXED_POINT_TO(_index * PID_TC_COEFF_LUT_DIVIDER - PID_TC_COEFF_LUT_OFFSET)) * TC_J_COEFF_LUT[_index]) + TC_J_MV_LUT[_index];
    #else
    ehs_sint32 temp_with_offset = FIXED_POINT_TO(20) - offset - EHS_THERMOCOUPLE_RESULT_OFFSET;
    _index = (temp_with_offset + FIXED_POINT_TO(PID_TC_COEFF_LUT_OFFSET)) / FIXED_POINT_TO(PID_TC_COEFF_LUT_DIVIDER);
    if (_index >= PID_TC_COEFF_LUT_SIZE) _index = PID_TC_COEFF_LUT_SIZE - 1;
    voltage -= FIXED_POINT_FROM((temp_with_offset - FIXED_POINT_TO(_index * PID_TC_COEFF_LUT_DIVIDER - PID_TC_COEFF_LUT_OFFSET)) * TC_J_COEFF_LUT[_index]) + TC_J_MV_LUT[_index];
    _index = (FIXED_POINT_TO(20) + FIXED_POINT_TO(PID_TC_COEFF_LUT_OFFSET)) / FIXED_POINT_TO(PID_TC_COEFF_LUT_DIVIDER);
    if (_index >= PID_TC_COEFF_LUT_SIZE) _index = PID_TC_COEFF_LUT_SIZE - 1;
    voltage += FIXED_POINT_FROM((FIXED_POINT_TO(20) - FIXED_POINT_TO(_index * PID_TC_COEFF_LUT_DIVIDER - PID_TC_COEFF_LUT_OFFSET)) * TC_J_COEFF_LUT[_index]) + TC_J_MV_LUT[_index];
    #endif
calc_temperature:
    // Calculate the temperature from voltage
    if (voltage == FIXED_POINT_TO(mV_low)) return pLUT[0];
    if (voltage < FIXED_POINT_TO(mV_low)) return -999*1024;
    if (voltage == FIXED_POINT_TO(mV_high)) return pLUT[lut_length - 1];
    if (voltage > FIXED_POINT_TO(mV_high)) return -999*1024;
    _index = FIXED_POINT_FROM((voltage + FIXED_POINT_TO(mV_offset)) / mV_divider);
    if (_index < 0 || _index >= lut_length) return -999*1024;
    ehs_sint32 _grad = FIXED_POINT_TO((pLUT[_index + 1] - pLUT[_index])) / mV_divider;
    adjusted = (FIXED_POINT_FROM(_grad * (voltage - FIXED_POINT_TO(_index * mV_divider - mV_offset))) + FIXED_POINT_TO(pLUT[_index])) / 10;
    #else 
    #ifdef EHS_MAX31343_SUPPORT
    adjusted = FIXED_POINT_TO(voltage) / 43 + gEhsAmbientTemp + offset;
    #else
    adjusted = FIXED_POINT_TO(voltage) / 43 + offset;
    #endif//EHS_MAX31343_SUPPORT
    #endif
    // isr_printf("in:%d vv:%d v:%d t:%d room_T:%d off:%d type:%d\n", in, vv, voltage, adjusted, gRefTemperature, offset, tc_type);
    return adjusted;
}

/* Returns the converted value of the sensor identified by sensor enum */

int32_t EHS_MEMORY_ATTRIB getConvertedValue(const inxPID_t *pid_config, const inxSensorType_t* pSensorType, const adcRawValues_t* pRawValues){
    int32_t value=0;

    switch(pid_config->sensors){
        case INX_CONTROLLER_SENSORS_10V:
            value = convert10VToTemperature(pRawValues->zeroTenV, gCalibration.zeroTenVOffset, gCalibration.zeroTenVScale, pid_config->calibrate);
            break;
        case INX_CONTROLLER_SENSORS_20MA:
            value = convert20MAToTemperature(pRawValues->fourTwentyMa, gCalibration.fourTwentyMAOffset, gCalibration.fourTwentyMAScale, pid_config->calibrate);
            break;
        case INX_CONTROLLER_SENSORS_MODBUS:
            value = convertModbusToTemperature(0);
            break;
        case INX_CONTROLLER_SENSORS_THERMOCOUPLE_CHANNEL1:
            value = convertThermocoupleToTemperature(pRawValues->thermocouple1, pSensorType->tc_type[0], gCalibration.thermocoupleOneOffset, gCalibration.thermocoupleOneScale, pid_config->calibrate);
            break;
        case INX_CONTROLLER_SENSORS_THERMOCOUPLE_CHANNEL2:
            value = convertThermocoupleToTemperature(pRawValues->thermocouple2, pSensorType->tc_type[1], gCalibration.thermocoupleTwoOffset, gCalibration.thermocoupleTwoScale, pid_config->calibrate);
            break;
        case INX_CONTROLLER_SENSORS_PT100_CHANNEL1:
            value = convert_pt100_to_temperature_v2(pRawValues->pt100Channel1, gCalibration.pt100Offset, gCalibration.pt100Scale, pid_config->calibrate);
            break;
        case INX_CONTROLLER_SENSORS_PT100_CHANNEL2:
            value = convert_pt100_to_temperature_v2(pRawValues->pt100Channel2, gCalibration.pt200Offset, gCalibration.pt200Scale, pid_config->calibrate);
            break;
        default:
            break;
    }

    return value;
}

void EHS_MEMORY_ATTRIB copyGlobalADCValues(globalADCValues_t* pDst,globalADCValues_t* pSrc){
    memcpy(pDst,pSrc,sizeof(globalADCValues_t));
}

volatile static bool gWriteToBufferZero=1;
static void EHS_MEMORY_ATTRIB writeGlobalADCValues(globalADCValues_t* pNewValues){
    globalADCValues_t* pValues;
    //LWIP_PLATFORM_DIAG(("writeGlobalADCValues: gWriteToBufferZero=%d",gWriteToBufferZero));
    if(gWriteToBufferZero){
        pValues=&gADCValues[0];
    }else{
        pValues=&gADCValues[1];
    }
    copyGlobalADCValues(pValues,pNewValues);
}

static globalADCValues_t readGlobalADCValues(){
    globalADCValues_t values;
    //LWIP_PLATFORM_DIAG(("readGlobalADCValues: gWriteToBufferZero=%d",gWriteToBufferZero));
    if(gWriteToBufferZero){
        gWriteToBufferZero=false; //so that the ADC stops writing to it
        copyGlobalADCValues(&values,&gADCValues[0]);
    }else{
        gWriteToBufferZero=true;
        copyGlobalADCValues(&values,&gADCValues[1]);
    }
    return values;
}

#define INX_MAX_CELSIUS_DEVIATION 10.0f
/* todo - turn this into a few functions */
void EHS_MEMORY_ATTRIB calculateAverageAndVariance(globalADCValues_t* pValues) {
#ifdef INX_USE_OUTLIER_DETECTION
    float _average_1 = 0.0f;
    float _average_2 = 0.0f;
    int _good_count_1 = 0;
    int _good_count_2 = 0;
#endif
    //unsigned char _good_samples[INX_SAMPLEBUF_SIZE]; // explicitly assigned

    adcConvertedValues_t averageValues={0};
    //adcConvertedValues_t varianceValues={0};

    //calculate average values
    for (int i=0; i < INX_SAMPLEBUF_SIZE;i++) {
        averageValues.pid1 += pValues->values[i].pid1;
        averageValues.pid2 += pValues->values[i].pid2;
        averageValues.cpuTemp1 += pValues->values[i].cpuTemp1;
        averageValues.pid3 += pValues->values[i].pid3;
    }
    averageValues.pid1 /= INX_SAMPLEBUF_SIZE;
    ////TODO ALL the changes below are HACKs.
    // gPIDs[0].currentValue = averageValues.pid1;
    gPIDs[0].currentValue = (averageValues.pid1 * 16 + gPIDs[0].currentValue * 240) / 256;

    averageValues.pid2 /= INX_SAMPLEBUF_SIZE;
    // gPIDs[1].currentValue = averageValues.pid2;
    gPIDs[1].currentValue = (averageValues.pid2 * 16 + gPIDs[1].currentValue * 240) / 256;
    
    averageValues.cpuTemp1 /= INX_SAMPLEBUF_SIZE;
    
    averageValues.pid3 /= INX_SAMPLEBUF_SIZE;
    // gPIDs[2].currentValue = averageValues.pid3;
    gPIDs[2].currentValue = (averageValues.pid3 * 16 + gPIDs[2].currentValue * 240) / 256;
#ifdef INX_USE_OUTLIER_DETECTION

    /* recalculate mean temperature excluding outliers */
    for (int i=0; i < INX_SAMPLEBUF_SIZE;i++) {
        if (fabs(pValues->values[i].temperature1-averageValues.temperature1) > INX_MAX_CELSIUS_DEVIATION) {
            //_good_samples[i] = 0;
        }
        else {
            //_good_samples[i] = 1;
            _average_1 += pValues->values[i].temperature1;
            _good_count_1 ++;
        }
        if (fabs(pValues->values[i].temperature2-averageValues.temperature2) > INX_MAX_CELSIUS_DEVIATION) {
            //_good_samples[i] = 0;
        }
        else {
            //_good_samples[i] = 1;
            _average_2 += pValues->values[i].temperature2;
            _good_count_2 ++;
        }
    }
    if (_good_count_1 > 3*(INX_SAMPLEBUF_SIZE/4)  ) { // need 7 out of ten to use this value.
        averageValues.temperature1 = _average_1/_good_count_1;
        pValues->adc0.status = ADC_STATUS_OK;
    }
    else {
        pValues->adc0.status = ADC_STATUS_OUTLIERS;
    }
    if (_good_count_2 > 3*(INX_SAMPLEBUF_SIZE/4)  ) { // need 7 out of ten to use this value.
        averageValues.temperature2 = _average_2/_good_count_2;
        pValues->adc1.status = ADC_STATUS_OK;
    }
    else {
        pValues->adc1.status = ADC_STATUS_OUTLIERS;
    }
#endif
    //calculate pseudo variance (including outliers)
    //for (int i=0; i < INX_SAMPLEBUF_SIZE;i++) {
    //	varianceValues.pid1 += (pValues->values[i].pid1-averageValues.pid1)*(pValues->values[i].pid1-averageValues.pid1);
    //	varianceValues.pid2 += (pValues->values[i].pid2-averageValues.pid2)*(pValues->values[i].pid2-averageValues.pid2);
    //	varianceValues.cpuTemp1 += (pValues->values[i].cpuTemp1-averageValues.cpuTemp1)*(pValues->values[i].cpuTemp1-averageValues.cpuTemp1);
    //	varianceValues.pid3 += (pValues->values[i].pid3-averageValues.pid3)*(pValues->values[i].pid3-averageValues.pid3);
    //}
    //varianceValues.pid1 /= (float)INX_SAMPLEBUF_SIZE;
    //varianceValues.pid2 /= (float)INX_SAMPLEBUF_SIZE;
    //varianceValues.cpuTemp1 /= (float)INX_SAMPLEBUF_SIZE;
    //varianceValues.pid3 /= (float)INX_SAMPLEBUF_SIZE;

    //copy our new values in to our structure
    gValues.current=averageValues;
#ifdef INX_HEATROD_VERBOSE_DEBUG
    globalADCValuesPrint(pValues);
#endif
}

#warning "why is this file called heatrod - should it be being built or is this an accident?

#ifdef INX_ERT_WDT_ENABLED
/* Should this go?*/
typedef enum {
    INX_WATCH_DOG_KICK_ADC,
    INX_WATCH_DOG_KICK_SUPERVISOR
} inxWatchDogKickID_t;
static void inxWatchDogKick(inxWatchDogKickID_t id){
    // TODO - watchdog kick
}
#endif

/*
 * This is called when all the ADC values are in
 * Converts the raw values to temperature values for use on control and monitoring
 *
 * NOTES: We only convert sensor values if they are used in PIDs, so we can't monitor anything in Devman or dashboards unless it's used in a PID controller.
 */
static void EHS_MEMORY_ATTRIB convertADCValuesToRealValues(adcRawValues_t* pRawValues,adcConvertedValues_t* pConverted){


pConverted->pid1=/*applyGlobalCalibrations(*/getConvertedValue(&gPIDs[0], &gSensorType, pRawValues)/*)*/; // only applu sur galibration to channeel 1
pConverted->pid2=/*applyGlobalCalibrations(*/getConvertedValue(&gPIDs[1], &gSensorType, pRawValues)/*)*/;
pConverted->pid3=/*applyGlobalCalibrations(*/getConvertedValue(&gPIDs[2], &gSensorType, pRawValues)/*)*/;


#if DEBUG_ADC_CONVERT_VALUES
    LWIP_PLATFORM_DIAG(("temp1 %d becomes %3.1f",pRawValues->temperature1,pConverted->temperature1));
    LWIP_PLATFORM_DIAG(("temp2 %d becomes %3.1f",pRawValues->temperature2,pConverted->temperature2));
#endif

    //now convert cpuTemp1 - @TODO - abstract out cpu temp
    pConverted->cpuTemp1 = 0; //(int32_t)(CONFIG_ADC_CPU_TEMP_STANDARD_TEMP - ((int32_t)pRawValues->cpuTemp1 - (int32_t)adcrTemp25) * 100 / (int32_t)adcr100m);
    //LWIP_PLATFORM_DIAG(("cpuTemp1 raw=%d converted=%d",pRawValues->cpuTemp1,pConverted->cpuTemp1));
#if defined(INX_HEATROD_IOT_DIAGNOSTIC_BUILD)
    inxDiagnosticsADC(pRawValues,pConverted);
#endif

}


//we assume this method is only ever called by the adc interrupt handler thread
volatile static bool gAdc0Done=0;
volatile static bool gAdc1Done=0;
static inxIecInterruptData_t gInxIecInterruptData={0,0};
static volatile bool gADCBuffersReady=false;


/* Warning This runs in an interrupt context
 * It calculated duty values and sets duty values (directly via GPIO presumably)
 * */
static void EHS_MEMORY_ATTRIB adcSampleProcessing(){
    //LWIP_PLATFORM_DIAG(("adcSampleProcessing %d %d %d %d",gAdc0Done,gAdc1Done,gInxIecInterruptData.doingTest,gInxIecInterruptData.interruptCount));
    if(gAdc0Done && gAdc1Done){
        gAdc0Done=0;
        gAdc1Done=0;
        
        //if we are in diagnostic mode then just clear the flag and stop
        if(gInxIecInterruptData.doingTest){
            gInxIecInterruptData.interruptCount++;
            return;
        }
#ifdef INX_ERT_WDT_ENABLED
        inxWatchDogKick(INX_WATCH_DOG_KICK_ADC);
#endif

        //convert the ADC values to actual temperatures and voltages
        convertADCValuesToRealValues(&gADCRawValues,&gValues.values[nValueIndex]);
        //adcConditionMonitoring(&gAdcConditionState,&gValues.values[nValueIndex]);

        //handle any PID timers
        // this is for a dynamic scheduler mode in heatrod that may not be even used
        //for(u8_t i=0;i<CONFIG_PID_CONTROLLERS_NUM;i++){
        //	inxPIDRampRun(&gPIDs[i]);
        //}

        if(		(gADCBuffersReady==true) //so that we only force a cycle update when we have fully booted up
            && 	(gControllerOutputUpdated==true || gDynamicSetPointUpdated==true)){
            //this forces next nValueIndex increment to trigger power recalculation
            nValueIndex = (INX_SAMPLEBUF_SIZE-1);
            //samples and duty log must be kept in sync or we get stale values back in to devman
            dutyCycleHistory0.powerHistoryHead = CONFIG_DUTY_CYCLE_HISTORY_LENGTH-1;
            if(gDynamicSetPointUpdated==true){
                //if power required > 0 and relay is off then we need to force
                //reset of cycle so power comes on
                for(u8_t i=0;i<CONFIG_PID_CONTROLLERS_NUM;i++){
                    if(gPIDs[i].powerPercent>0){
                        for(u8_t outputNum=0;outputNum<CONFIG_PID_OUTPUTS_NUM;outputNum++){
                            int8_t index=inxIecControllerOutputModeToDutyCycleStateIndex(gPIDs[i].controllerOutputMode[outputNum]);
                            if(index>=0){
                                if(dutyCycleStates[index].on){
                                    gControllerOutputUpdated=true;
                                }
                            }
                        }
                    }
                }
            }
            if(gControllerOutputUpdated){
                //our controller output mode was changed so reset the tick count to force a quick update of elements and power
                //force everything off since we don't know what output mode we'll be using when the power comes back on
                inxIecShutOffAllRelays(false);
                for(u8_t i=0;i<CONFIG_DUTY_CYCLES_NUM;i++){
                    dutyCycleStates[i].numInterrupts=dutyCycleStates[i].cycleInterruptLength-2;
                }
            }
        }else if(gADCBuffersReady==false){
            memcpy(&gValues.current,&gValues.values[nValueIndex],sizeof(gValues.current));
        }

        //increment our buffer index
        nValueIndex++;
        if (nValueIndex >= INX_SAMPLEBUF_SIZE) {
            //printf("PBB 6193\n");
            nValueIndex = 0;
            gControllerOutputUpdated=false;
            gDynamicSetPointUpdated=false;
            calculateAverageAndVariance(&gValues);
            writeGlobalADCValues(&gValues);
            gADCValuesReady = true;
            //assume all elements should be off
            for(u8_t i=0;i<CONFIG_DUTY_CYCLES_NUM;i++){
                if(dutyCycleStates[i].ftmEnabled){
                    //don't bother resetting devices that are using ftm instead
                }else{
                    dutyCycleStateSetPowerPercent(&dutyCycleStates[i],0,0);
                }
            }

            gADCValuesUnsafe=false;
            for(u8_t i=0;i<CONFIG_PID_CONTROLLERS_NUM;i++){
                //we must do this after calculatingAverageAndVariance so that pid values are accurate
                //if(gADCBuffersReady==false){
                    //this is the first full set of data so update ramp starting point
                //	inxSetPIDRampDesired(&gPIDs[i],gPIDs[i].currentValue+1,false);
                //}
                if(true /*pidValuesSafe(&gPIDs[i])*/){ // @TODO - sort out pid safe values handler
                    //recalculate required power
                    bool globalLimitReached=gPIDs[i].currentValue>=gPIDs[i].nPIDUpperLimit;
                    calculateRequiredPower(i,&gPIDs[i],globalLimitReached);
                }else{
                    //shut off the power
                    //printf("##==== Unsafe values = %f\n",gValues.adc0.temperature);
                    gADCValuesUnsafe=true;
                    for(u8_t i=0;i<CONFIG_DUTY_CYCLES_NUM;i++){
                        dutyCycleStateSetPowerPercent(&dutyCycleStates[i],0,0);
                    }
                    inxIecShutOffAllRelays(false);
                }
            }
            gADCBuffersReady=true;
        }else {
            if(gADCBuffersReady){
                //calculate new averages and update our global
                calculateAverageAndVariance(&gValues);
            }
        }

        //uint32_t ticksToWait=(CONFIG_MAX_MILLISECONDS_BETWEEN_SAVES/(gLPTMRInterruptMilliseconds*CONFIG_LPTMR_NUM_READS));
        //printf("gTicks=%d ticksToWait=%d\r\n",gTicksSinceSettingsSave,ticksToWait);
        //gTicksSinceSettingsSave++;
        //if(gTicksSinceSettingsSave >= (CONFIG_MAX_MILLISECONDS_BETWEEN_SAVES/(gLPTMRInterruptMilliseconds*CONFIG_LPTMR_NUM_READS))){
        //    gTicksSinceSettingsSave=0;
            //so that we send back the relay state
            ////////////////// STUB ////////////// gGenerateDiagnosticReport=true;
        //    gSettingsUpdatedByUser=true;
        //}
    }
}

void inxInitDutyCycleStates(){
    //initialise our duty cycle states
    int8_t dutyCycleStateIndex=inxIecControllerOutputModeToDutyCycleStateIndex(INX_CONTROLLER_OUTPUT_MODE_INTERNAL_1);
    if(dutyCycleStateIndex>=0){
        //dutyCycleStates[dutyCycleStateIndex].gpioBase=CONFIG_HEATER_ONE_GPIO_BASE;
        dutyCycleStates[dutyCycleStateIndex].inverse_logic=CONFIG_GPIO_INVERSE_INTERNAL_RELAY_1;
        dutyCycleStates[dutyCycleStateIndex].gpioPin=CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1;// todo - this can be dynamically switched to external(auxillary outputs too. should read from the config at start up case oo)
    }
    dutyCycleStateIndex=inxIecControllerOutputModeToDutyCycleStateIndex(INX_CONTROLLER_OUTPUT_MODE_INTERNAL_2);
    if(dutyCycleStateIndex>=0){
        //dutyCycleStates[dutyCycleStateIndex].gpioBase=CONFIG_HEATER_TWO_GPIO_BASE;
        dutyCycleStates[dutyCycleStateIndex].inverse_logic=CONFIG_GPIO_INVERSE_INTERNAL_RELAY_2;
        dutyCycleStates[dutyCycleStateIndex].gpioPin=CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2;
    }
    dutyCycleStateIndex=inxIecControllerOutputModeToDutyCycleStateIndex(INX_CONTROLLER_OUTPUT_MODE_INTERNAL_3);
    if(dutyCycleStateIndex>=0){
        //dutyCycleStates[dutyCycleStateIndex].gpioBase=CONFIG_HEATER_THREE_GPIO_BASE;
        dutyCycleStates[dutyCycleStateIndex].inverse_logic=CONFIG_GPIO_INVERSE_INTERNAL_RELAY_3;
        dutyCycleStates[dutyCycleStateIndex].gpioPin=CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3;
    }
    dutyCycleStateIndex=inxIecControllerOutputModeToDutyCycleStateIndex(INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_1);
    if(dutyCycleStateIndex>=0){
        //dutyCycleStates[dutyCycleStateIndex].gpioBase=CONFIG_HEATER_ONE_GPIO_BASE;
        dutyCycleStates[dutyCycleStateIndex].inverse_logic=CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_1;
        dutyCycleStates[dutyCycleStateIndex].gpioPin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1;
    }
    dutyCycleStateIndex=inxIecControllerOutputModeToDutyCycleStateIndex(INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_2);
    if(dutyCycleStateIndex>=0){
        //dutyCycleStates[dutyCycleStateIndex].gpioBase=CONFIG_HEATER_TWO_GPIO_BASE;
        dutyCycleStates[dutyCycleStateIndex].inverse_logic=CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_2;
        dutyCycleStates[dutyCycleStateIndex].gpioPin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2;
        //dutyCycleStates[dutyCycleStateIndex].ftmBase=FTM2;
        //dutyCycleStates[dutyCycleStateIndex].ftmChannel=kFTM_Chnl_0;
    }
    dutyCycleStateIndex=inxIecControllerOutputModeToDutyCycleStateIndex(INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_3);
    if(dutyCycleStateIndex>=0){
        //dutyCycleStates[dutyCycleStateIndex].gpioBase=CONFIG_HEATER_THREE_GPIO_BASE;
        dutyCycleStates[dutyCycleStateIndex].inverse_logic=CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_3;
        dutyCycleStates[dutyCycleStateIndex].gpioPin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3;
        //dutyCycleStates[dutyCycleStateIndex].ftmBase=FTM2;
        //dutyCycleStates[dutyCycleStateIndex].ftmChannel=kFTM_Chnl_1;
    }
}

static void updateControllerOutputMode(const bool byUser,inxPID_t* pPID,const uint8_t outputNum,const inxControllerOutputMode_t newMode){
    //if ( INX_CONTROLLER_OUTPUT_MODE_DISABLED != gControllerOutputMode  && INX_CONTROLLER_OUTPUT_MODE_DISABLED  != newMode) {
    /*todo - There are quite a few other ways relays might get switched on without a doce errors (e.g. of we have an aux outout mapped to the wrong relays,
    but we want any subsequent errors (e.g. tripped TCO or relays status errors)
    Hence we might want to add another state to this mode which is "permenantly" disabled, which can't be got out of without a factory reset or something harsh on the user.
    Or we might (better still) use the system state variable for gateing any DIO control of relays instead of the user configuravble outout mode)
    */
        if (outputNum>=CONFIG_PID_OUTPUTS_NUM || newMode==pPID->controllerOutputMode[outputNum]){
            //ignore this settings change, unless it is setting the mode to disabled, which we do want to work and trigger any settings write etc.
        }else{
            //shut off the previous relay
            inxIecShutOffRelay(pPID->controllerOutputMode[outputNum]);
            pPID->controllerOutputMode[outputNum]=newMode;
            //inxAuxControlEnum_t auxControlOutput = gAuxControlOutput;
            //inxSetAuxControlOutput(byUser,INX_AUX_DISABLED);
            //now it's disabled try turning it back on
            //inxSetAuxControlOutput(byUser,auxControlOutput);
            //inxSettingsChanged(byUser);
            gControllerOutputUpdated = true;
        }
    //}
}

typedef enum {
    INX_IEC_GPIO_OFF,
    INX_IEC_GPIO_ON
} inxIecGPIOState_t;

inxControllerOutputMode_t inx_get_controller_output_mode(ehs_sint32 mode, ehs_pid_ctrl_error_id* error_id)
{
    if(mode >= INX_CONTROLLER_OUTPUT_MODE_DISABLED && mode < INX_CONTROLLER_OUTPUT_MODE_END){
        return (inxControllerOutputMode_t)mode;
    }
    *error_id = PID_CTRL_INVALID_CTRL_OUTPUT_MODE;
    return INX_CONTROLLER_OUTPUT_MODE_DISABLED;
}

inxControllerSensors_t inx_get_controller_sensor(ehs_sint32 adc_ctrl_sensor, ehs_pid_ctrl_error_id* error_id)
{
    if(adc_ctrl_sensor >= INX_CONTROLLER_SENSORS_NONE && adc_ctrl_sensor <= INX_CONTROLLER_SENSORS_THERMOCOUPLE_CHANNEL2){
        return (inxControllerSensors_t)adc_ctrl_sensor;
    }
    *error_id = PID_CTRL_INVALID_CTRL_SENSOR_ID;
    return INX_CONTROLLER_SENSORS_NONE;
}

inxThermoCoupleType_t inx_get_controller_sensor_type(ehs_sint32 adc_ctrl_sensor_type, ehs_pid_ctrl_error_id *error_id)
{
    if (adc_ctrl_sensor_type > INX_CONTROLLER_TC_TYPE_NONE && adc_ctrl_sensor_type <= INX_CONTROLLER_TC_TYPE_NOTDEFINED)
    {
        return (inxThermoCoupleType_t)adc_ctrl_sensor_type;
    }
    *error_id = PID_CTRL_INVALID_CTRL_SENSOR_TYPE;
    return INX_CONTROLLER_TC_TYPE_NONE;
}

/* function returns an index for a specifc PID controller */
ehs_bool inx_get_pid_index(const ehs_pid_ctrl_type* pid_ctrl, ehs_sint32* ppid_ctrl_index)
{
    if(!pid_ctrl || !ppid_ctrl_index){
        return EHS_FALSE;
    }
    ehs_sint32 pid_ctrl_index = (pid_ctrl->pid_no - 1);
    if(pid_ctrl_index < 0 || pid_ctrl_index > EHS_PID_CTRL_MAX_INDEX){
        return EHS_FALSE;
    }
    *ppid_ctrl_index = pid_ctrl_index;
    return EHS_TRUE;
}

ehs_pid_ctrl_error_id inx_pid_ctrl_init()
{
    ehs_pid_ctrl_error_id nError = PID_CTRL_NO_ERROR;
    for(u8_t i=0; i < CONFIG_DUTY_CYCLES_NUM; i++){
        inxDutyCycleStateSetRelayType(false, &dutyCycleStates[i], INX_CONTROLLER_CONTACTOR_MODE_EMR);
        inxDutyCycleStateSetCycleLengthSeconds(false, &dutyCycleStates[i], 10);
    }
    
    // these seems to be set as default values when settings is not available
    gElement2PowerThresholdPercent=33;
    gElement3PowerThresholdPercent=66;

    inxLPTMRInit();

    inxInitDutyCycleStates();

    // enable gpio out
    gGpioConditionState.mode = INX_GPIO_CONDITION_OK;

    // initalise sensor type
    // set default thermocouple type 1 and 2
    gSensorType.tc_type[0] = inx_get_controller_sensor_type(INX_CONTROLLER_TC_TYPE_J, &nError);
    gSensorType.tc_type[1] = inx_get_controller_sensor_type(INX_CONTROLLER_TC_TYPE_J, &nError);

    return nError;
}

ehs_pid_ctrl_error_id inx_pid_ctrl_channel_init(const ehs_pid_ctrl_type* pid_ctrl)
{
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        return PID_CTRL_BAD_PID_INDEX;
    }
    // set calibration mode of the controller
    gPIDs[pid_ctrl_index].calibrate = (pid_ctrl->calib_mode==EHS_TRUE);
    return PID_CTRL_NO_ERROR;
}

ehs_pid_ctrl_error_id inx_pid_ctrl_gpio_init()
{
    ehs_pid_ctrl_error_id nError = PID_CTRL_NO_ERROR;

    g_inx_gpio[0].pin_id = CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1;
    g_inx_gpio[1].pin_id = CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2;
    g_inx_gpio[2].pin_id = CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3;
    g_inx_gpio[3].pin_id = CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1;
    g_inx_gpio[4].pin_id = CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2;
    g_inx_gpio[5].pin_id = CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3;

    // TODO - configure external relays

    for(ehs_uint8 i = 0; i < INX_PID_GPIO_COUNT; i++){
        g_inx_gpio[i].pin_value = g_inx_gpio_inverse[i] ? EHS_TRUE : EHS_FALSE;
        g_inx_gpio[i].open_drain_mode = EHS_FALSE;
        g_inx_gpio[i].error_state = EHS_FALSE;
        g_inx_gpio[i].initial_state = g_inx_gpio_inverse[i] ? EHS_TRUE : EHS_FALSE;
        g_inx_gpio[i].target_data = NULL;
        if(EhsInitOutputGPIO(&(g_inx_gpio[i])) == EHS_FALSE){
            nError = PID_CTRL_GPIO_INIT_FAILED;
        }
    }

    return nError;
}

ehs_pid_ctrl_error_id inx_pid_ctrl_gpio_destroy()
{
    ehs_pid_ctrl_error_id nError = PID_CTRL_NO_ERROR;

    for(ehs_uint8 i = 0; i < INX_PID_GPIO_COUNT; i++){
        if(EhsDestroyOutputGPIO(&(g_inx_gpio[i])) == EHS_FALSE){
            nError = PID_CTRL_GPIO_DESTROY_FAILED;
        }
    }

    return nError;
}

ehs_pid_ctrl_error_id inx_set_setpoint_value(const ehs_pid_ctrl_type* pid_ctrl)
{
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        return PID_CTRL_BAD_PID_INDEX;
    }
    // set the 'set point' for the PID controller if it changes
    ehs_sint32 setpoint = FIXED_POINT_TO(pid_ctrl->setpointValue);
    if(gPIDs[pid_ctrl_index].desiredTemp != setpoint){
        gPIDs[pid_ctrl_index].desiredTemp = setpoint;
        //printf("setpoint=%d\n", setpoint);
    }
    return PID_CTRL_NO_ERROR;
}

ehs_pid_ctrl_error_id inx_set_pid_config(const ehs_pid_ctrl_type* pid_ctrl)
{
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        return PID_CTRL_BAD_PID_INDEX;
    }
    /*printf("id=%d, cs=%d, kp=%f, ki=%f, kd=%f, gl=%f, il=%f\n",pid_ctrl->pid_no,pid_ctrl->nSensorID,pid_ctrl->P,pid_ctrl->I,
                                                               pid_ctrl->D,pid_ctrl->maxGlobalValue,pid_ctrl->maxIValue);
                                                               */
    gPIDs[pid_ctrl_index].nPIDUpperLimit = FIXED_POINT_TO(pid_ctrl->maxGlobalValue);
    gPIDs[pid_ctrl_index].nIMax = FIXED_POINT_TO(pid_ctrl->maxIValue);

    gPIDs[pid_ctrl_index].coefficients.kp = FIXED_POINT_TO(pid_ctrl->P);
    gPIDs[pid_ctrl_index].coefficients.ki = FIXED_POINT_TO(pid_ctrl->I);
    gPIDs[pid_ctrl_index].coefficients.kd = FIXED_POINT_TO(pid_ctrl->D);


    return PID_CTRL_NO_ERROR;
}

ehs_pid_ctrl_error_id inx_set_io_config(const ehs_pid_ctrl_type* pid_ctrl)
{
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        return PID_CTRL_BAD_PID_INDEX;
    }

    ehs_pid_ctrl_error_id error_id =  PID_CTRL_NO_ERROR;

    /* Configure sensor for this controller */
    gPIDs[pid_ctrl_index].sensors = inx_get_controller_sensor(pid_ctrl->nSensorID, &error_id);
    if(error_id != PID_CTRL_NO_ERROR){
        return error_id;
    }

    gPIDs[pid_ctrl_index].controllerOutputMode[0] = inx_get_controller_output_mode(pid_ctrl->nChannelOutput1, &error_id);
    gPIDs[pid_ctrl_index].controllerOutputMode[1] = inx_get_controller_output_mode(pid_ctrl->nChannelOutput2, &error_id);
    gPIDs[pid_ctrl_index].controllerOutputMode[2] = inx_get_controller_output_mode(pid_ctrl->nChannelOutput3, &error_id);
    if(error_id != PID_CTRL_NO_ERROR){
        return error_id;
    }
    
    // @TODO - we may want to use 'updateControllerOutputMode' instead, or at least set gControllerOutputUpdated to true
    //for(uint8_t outputNum=0;outputNum<CONFIG_PID_OUTPUTS_NUM;outputNum++){
    //	updateControllerOutputMode(false,&gPIDs[pid_ctrl_index],outputNum,gPIDs[pid_ctrl_index].controllerOutputMode[outputNum]);
    //}

    if(pid_ctrl->nOutputMode >= ELEMENT_MODE_SINGLE && pid_ctrl->nOutputMode <= ELEMENT_MODE_SINGLE_TWO_POLE){
        gPIDs[pid_ctrl_index].elementMode = pid_ctrl->nOutputMode;
        //printf("id=%d, element mode=%d\n", pid_ctrl->pid_no, pid_ctrl->nOutputMode);
    }else{
        return PID_CTRL_INVALID_ELEMENT_MODE;
    }	

    return PID_CTRL_NO_ERROR;
}

void inx_set_calibration_offset(ehs_sint32 id, float offset)
{
    if(id < SENSOR_ID_PT100 || id >= SENSOR_ID_COUNT){
        return;
    }
    switch ((inxAdcSensorId_t)id)
    {
        case SENSOR_ID_PT100:
            gCalibration.pt100Offset = FIXED_POINT_TO(offset);
            break;
        case SENSOR_ID_PT200:
            gCalibration.pt200Offset = FIXED_POINT_TO(offset);
            break;
        case SENSOR_ID_10V:
            gCalibration.zeroTenVOffset = FIXED_POINT_TO(offset);
            break;
        case SENSOR_ID_20MA:
            gCalibration.fourTwentyMAOffset = FIXED_POINT_TO(offset);
            break;
        case SENSOR_ID_THERMOCOUPLE1:
            gCalibration.thermocoupleOneOffset = FIXED_POINT_TO(offset);
            break;
        case SENSOR_ID_THERMOCOUPLE2:
            gCalibration.thermocoupleTwoOffset = FIXED_POINT_TO(offset);
            break;
        default:
            break;
    }
}

void inx_set_calibration_scale(ehs_sint32 id, float scale)
{
    if(id < SENSOR_ID_PT100 || id >= SENSOR_ID_COUNT){
        return;
    }
    switch ((inxAdcSensorId_t)id)
    {
        case SENSOR_ID_PT100:
            gCalibration.pt100Scale  = FIXED_POINT_TO(scale);
            break;
        case SENSOR_ID_PT200:
            gCalibration.pt200Scale  = FIXED_POINT_TO(scale);
            break;
        case SENSOR_ID_10V:
            gCalibration.zeroTenVScale  = FIXED_POINT_TO(scale);
            break;
        case SENSOR_ID_20MA:
            gCalibration.fourTwentyMAScale  = FIXED_POINT_TO(scale);
            break;
        case SENSOR_ID_THERMOCOUPLE1:
            gCalibration.thermocoupleOneScale  = FIXED_POINT_TO(scale);
            break;
        case SENSOR_ID_THERMOCOUPLE2:
            gCalibration.thermocoupleTwoScale  = FIXED_POINT_TO(scale);
            break;
        default:
            break;
    }
}

ehs_float inx_get_adc_converted_value(const ehs_pid_ctrl_type* pid_ctrl)
{
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        return 0;
    }
    return FLOATING_FIXED_POINT_FROM(gPIDs[pid_ctrl_index].currentValue);
}

ehs_bool inx_get_pid_output_index_for_pin(const ehs_sint32 pin, ehs_sint32* pid_index, ehs_sint32* pid_out_index)
{
    uint8_t dutyCycleIndex = -1;
    for(uint8_t i=0;i<CONFIG_DUTY_CYCLES_NUM;i++){
        if(dutyCycleStates[i].gpioPin==pin){
            dutyCycleIndex = i;
            break;
        }
    }
    if(dutyCycleIndex > -1){
        for(ehs_uint8 i = 0; i < EHS_PID_CTRL_MAX_COUNT; i++){
            for(ehs_uint8 j = 0; j < CONFIG_PID_OUTPUTS_NUM; j++){
                int8_t index = inxIecControllerOutputModeToDutyCycleStateIndex(gPIDs[i].controllerOutputMode[j]);
                if(index == dutyCycleIndex){
                    *pid_index = i;
                    *pid_out_index = j;
                    return EHS_TRUE;
                }
            }
        }
    }

    return EHS_FALSE;
}

void EHS_MEMORY_ATTRIB inx_run_adc_pid(adcRawValues_t* adc_raw)
{	
    gAdc0Done = 1;
    gAdc1Done = 1;

    gADCRawValues.pt100Channel1 = adc_raw->pt100Channel1;
    gADCRawValues.pt100Channel2 = adc_raw->pt100Channel2;
    gADCRawValues.zeroTenV = adc_raw->zeroTenV;
    gADCRawValues.fourTwentyMa = adc_raw->fourTwentyMa;
    gADCRawValues.thermocouple1 = adc_raw->thermocouple1;
    gADCRawValues.thermocouple2 = adc_raw->thermocouple2;
    gADCRawValues.connected = adc_raw->connected;

    adcSampleProcessing();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(INX_HEATROD_IOT_DIAGNOSTIC_BUILD) && INX_HEATROD_IOT_BOARD_VARIANT != INX_HR_HRx_COMMISSIONING_BOARD
    inxIecGPIOState_t expectedState = INX_IEC_GPIO_OFF;
    bool heaterOk[CONFIG_DUTY_CYCLES_NUM]={0};
    bool allOk=true;
    for(uint8_t i=0;i<CONFIG_DUTY_CYCLES_NUM;i++){
        //IEC 60730 H.11.12.7 7.1 I/O Periphery H.2.18.13
        expectedState = INX_IEC_GPIO_OFF;
        //verify that our expected state matches our real state
        if(dutyCycleStates[i].on){
            expectedState = INX_IEC_GPIO_OFF;
        }else{
            expectedState = INX_IEC_GPIO_ON;
        }
        //override for when relay 2 does not have correct gpio readings
        if(i==INX_DUTY_CYCLE_INTERNAL_2_INDEX){
            switch(gPIDs[0].elementMode){
                case ELEMENT_MODE_SINGLE_TWO_POLE:
                    expectedState = INX_IEC_GPIO_ON;
                    break;
                default:
                    break;
            }
        }
        /*heaterOk[i]=inxIecGPIOCheck(&gRelayErrorState,i,
            expectedState,dutyCycleStates[i].ticksSinceChange,
            gMinTicksSinceHeaterChange);
        if(heaterOk[i]){

        }else{
            allOk=false;
        }*/
    }
    gFalseAlarmTicks++;
    if(gFalseAlarmTicks>100){
        gFalseAlarmTicks=0;
    }
    //read the thermal CO so we update the global state in case we are asked for a report
    //thermal CO should follow relay 1 unless it has tripped in which case it should always be off
    expectedState=INX_IEC_GPIO_ON;
    if(dutyCycleStates[INX_DUTY_CYCLE_INTERNAL_1_INDEX].on){
        expectedState=INX_IEC_GPIO_OFF;
    }

//in 3 phase 415 mode we need to check the TCO while relay 3 is off
//need to modify the duty state so that relays 2 and 3 turn off gMinTicks before relay 1 so that we can test relay 1
//on it's own
//then the only problem is in 100% duty mode relay 3 is never off
//therefore we need to intermittently knock relay 2 and 3 off so we can test the thermal CO
//best way of doing that is with a separate counter on the duty state that counts how long a relay has been on continuously
//if it gets above the limit for that relay then it tweaks down the ticks needed by gMinTicks
#if	CONFIG_THERMAL_CUT_OUT_DISABLED
    bool thermalCOOk=true;
#else
    bool thermalCOOk=inxIecGPIOCheck(&gRelayErrorState,INX_IEC_GPIO_THERMAL_CO_TEST,expectedState,0,0);
#endif

    pwmcalc_printf("-->IQ:");
    switch(gGpioConditionState.mode){
        case INX_GPIO_CONDITION_OK:
            //pwmcalc_printf("COND=GOOD -");
            if(allOk && thermalCOOk){
                pwmcalc_printf("heaters good\n");
                for(u8_t i=0;i<CONFIG_DUTY_CYCLES_NUM;i++){
                    dutyCycleStateTick(&dutyCycleStates[i]);
                }
            }else{
                //bad state
                pwmcalc_printf("STATE=BAD,");
            //	inxGPIOFaultDetected(&gRelayErrorState,heaterOk,thermalCOOk); // what does this actually do?
            }
            break;
        case INX_GPIO_CONDITION_DISABLED:
            pwmcalc_printf("COND=DISABLED,");
            if(gFalseAlarmTicks==0){
                //indicate that the relay state should be sent back to head quarters
                gRelayErrorState.time = 0;
            }
            break;
        case INX_GPIO_CONDITION_DIAGNOSTICS:
            pwmcalc_printf("COND=DIAGNOSTICS,");
            //LWIP_PLATFORM_DIAG(("Not doing anything till diagnostics are over"));
            break;
        default:
            LWIP_PLATFORM_DIAG(("Should never get here"));
            pwmcalc_printf("COND=BAD,");
            bool bools[CONFIG_DUTY_CYCLES_NUM]={0};
            //inxGPIOFaultDetected(&gRelayErrorState,bools,false);
            break;
    }
    pwmcalc_printf("IQ-->\n");
#endif // DIAGNOSTIC BUILD DONT INCLUDE
}



/*****************************************************************/
/**************** Implement 'hal_pid.h' functions ****************/
/*****************************************************************/

ehs_bool EhsPIDCtrlConfigurePID(ehs_pid_ctrl_type* pid_ctrl)
{
    pid_ctrl->nError = inx_set_pid_config(pid_ctrl);
    return (pid_ctrl->nError == PID_CTRL_NO_ERROR) ? EHS_TRUE : EHS_FALSE;
}

ehs_bool EhsPIDCtrlConfigureIO(ehs_pid_ctrl_type* pid_ctrl)
{
    pid_ctrl->nError = inx_set_io_config(pid_ctrl);
    return (pid_ctrl->nError == PID_CTRL_NO_ERROR) ? EHS_TRUE : EHS_FALSE;
}

ehs_bool EhsPIDCtrlSetSetpointValue(ehs_pid_ctrl_type* pid_ctrl)
{
    pid_ctrl->nError = inx_set_setpoint_value(pid_ctrl);
    return (pid_ctrl->nError == PID_CTRL_NO_ERROR) ? EHS_TRUE : EHS_FALSE;
}

ehs_bool EhsPIDCtrlUpdateOutputs(ehs_pid_ctrl_type* pid_ctrl)
{
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        pid_ctrl->nError = PID_CTRL_BAD_PID_INDEX;
        return EHS_FALSE;
    }
    int8_t index;
    index = inxIecControllerOutputModeToDutyCycleStateIndex(gPIDs[pid_ctrl_index].controllerOutputMode[0]);
    if(index >= 0){
        pid_ctrl->bPWMOut1 = (dutyCycleStates[index].on) ? EHS_TRUE : EHS_FALSE;
    }else{
        pid_ctrl->bPWMOut1 = EHS_FALSE;
    }
    index = inxIecControllerOutputModeToDutyCycleStateIndex(gPIDs[pid_ctrl_index].controllerOutputMode[1]);
    if(index >= 0){
        pid_ctrl->bPWMOut2 = (dutyCycleStates[index].on) ? EHS_TRUE : EHS_FALSE;
    }else{
        pid_ctrl->bPWMOut2 = EHS_FALSE;
    }
    index = inxIecControllerOutputModeToDutyCycleStateIndex(gPIDs[pid_ctrl_index].controllerOutputMode[2]);
    if(index >= 0){
        pid_ctrl->bPWMOut3 = (dutyCycleStates[index].on) ? EHS_TRUE : EHS_FALSE;
    }else{
        pid_ctrl->bPWMOut3 = EHS_FALSE;
    }

    pid_ctrl->nOutputPercent = gPIDs[pid_ctrl_index].powerPercent;

    return EHS_TRUE;
}

ehs_bool EhsPIDCtrlDisable(ehs_pid_ctrl_type* pid_ctrl)
{
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        pid_ctrl->nError = PID_CTRL_BAD_PID_INDEX;
        return EHS_FALSE;
    }
    if(pid_ctrl->bDisableControl == EHS_TRUE){
        gPIDs[pid_ctrl_index].scheduleMode = SCHEDULE_MODE_DISABLED;
        //printf("id=%d, disabled mode\n", pid_ctrl->pid_no);
    }else{
        // we set to manual control if not disabled, as setpoint (manula, schedule etc.) is controlled in the app
        gPIDs[pid_ctrl_index].scheduleMode = SCHEDULE_MODE_MANUAL;
    }
    return EHS_TRUE;
}

ehs_bool inx_get_adc_calibration_state(const ehs_pid_ctrl_type* pid_ctrl)
{
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        return 0;
    }
    return gPIDs[pid_ctrl_index].calibrate;
}

ehs_float EhsPIDCtrlGetSensorValue(const ehs_pid_ctrl_type* pid_ctrl, ehs_bool* isConnected)
{
    if (inx_get_adc_calibration_state(pid_ctrl)) return inx_get_adc_converted_value(pid_ctrl);
    *isConnected = isSensorConnected(pid_ctrl->nSensorID, &(gADCRawValues.connected));
    // get a current value of converted adc value e.g. temperature
    return (*isConnected) ? inx_get_adc_converted_value(pid_ctrl) : 0; // @TODO - confirm that we want to output 0 for sensor not connected
}

/* This comes from the calibration block */
ehs_sint32 EhsPIDCtrlCalibrate(ehs_sint32 id, ehs_bool bScale, float scale, ehs_bool bOffset, float offset)
{
    //printf("EhsCalibratePIDCtrl id=%d scale=%.2f offset=%.2f\n", id, scale, offset);
    if(bScale == EHS_TRUE){
        inx_set_calibration_scale(id, scale);
    }
    if(bOffset == EHS_TRUE){
        inx_set_calibration_offset(id, offset);
    }
    return 0;
}

#define INX_RELAY_TYPE_EXTERNAL_INDEX 3

ehs_bool EhsPIDCtrlConfigureRelays(ehs_pid_ctrl_relay_type_id id, ehs_sint32 idx, ehs_sint32 rel, ehs_float cyc_len)
{
    ehs_bool success = EHS_FALSE;
    if(rel >= INX_CONTROLLER_CONTACTOR_MODE_EMR && rel <= INX_CONTROLLER_CONTACTOR_MODE_SSR){
        switch (id)
        {
            case PID_CTRL_RELAY_TYPE_INTERNAL:
            {
                ehs_sint32 index = idx;
                //printf("EhsPIDCtrlConfigureRelays (internal) index=%d, rel=%d, len=%.3f  \n", index, rel, cyc_len);
                if(idx >= 0  && idx < INX_RELAY_TYPE_EXTERNAL_INDEX){
                    inxDutyCycleStateSetRelayType(false, &dutyCycleStates[index], (inxRelayType_t)rel);
                    inxDutyCycleStateSetCycleLengthSeconds(false, &dutyCycleStates[index], (const float)cyc_len);
                    success = EHS_TRUE;
                }
                break;
            }
            case PID_CTRL_RELAY_TYPE_EXTERNAL:
            {
                ehs_sint32 index = idx + INX_RELAY_TYPE_EXTERNAL_INDEX;
                //printf("EhsPIDCtrlConfigureRelays (external) index=%d, rel=%d, len=%.3f  \n", index, rel, cyc_len);
                if(idx >= 0  && idx < CONFIG_DUTY_CYCLES_NUM){
                    inxDutyCycleStateSetRelayType(false, &dutyCycleStates[index], (inxRelayType_t)rel);
                    inxDutyCycleStateSetCycleLengthSeconds(false, &dutyCycleStates[index], (const float)cyc_len);
                    success = EHS_TRUE;
                }
                break;
            }
            default:
                break;
        }
    }
    return success;
}

ehs_bool EhsPIDCtrlConfigureHardware(ehs_sint32 id, ehs_sint32 value, ehs_uint32* error_no)
{
    ehs_bool success = (id > INX_HDWR_CONF_UNKNOWN && id < INX_HDWR_CONF_COUNT) ? EHS_TRUE : EHS_FALSE;
    ehs_uint32 err = PID_CTRL_INVALID_HDWR_CONFIG_ID;
    if(success == EHS_TRUE) {
        err = PID_CTRL_NO_ERROR;
        //printf("EhsPIDCtrlConfigureHardware id = %d, value = %d \n", id, value);
        switch ((inxHardwareConfigType_t)id)
        {
            case INX_HDWR_CONF_SENSOR_TYPE_TC_1:{
                inxThermoCoupleType_t type = inx_get_controller_sensor_type(value, &err);
                success = (err != PID_CTRL_INVALID_CTRL_SENSOR_TYPE);
                if(success == EHS_TRUE){
                    gSensorType.tc_type[0] = type;
                }
                break;
            }
            case INX_HDWR_CONF_SENSOR_TYPE_TC_2:{
                inxThermoCoupleType_t type = inx_get_controller_sensor_type(value, &err);
                success = (err != PID_CTRL_INVALID_CTRL_SENSOR_TYPE);
                if(success == EHS_TRUE){
                    gSensorType.tc_type[1] = type;
                }
                break;
            }
            default:{
                success = EHS_FALSE;
                err = PID_CTRL_INVALID_HDWR_CONFIG_ID;
                break;
            }
        }
    }
    if(error_no){
        *error_no = err;
    }
    return success;
}

/*****************************************************************/
