/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PROJECT_SETUP_K32W_CM4_H_
#define _PROJECT_SETUP_K32W_CM4_H_

#include "MK64F12.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

//#define USE_PCC 1   
   
#define CLOCK_DISABLED 0
#define DIVIDE_BY_1    1
#define DIVIDE_BY_2    2
#define DIVIDE_BY_4    3
#define DIVIDE_BY_8    4
#define DIVIDE_BY_16   5   
#define DIVIDE_BY_32   6
#define DIVIDE_BY_64   7   
   
#define LPTMR_CLOCK_SIRC      0
#define LPTMR_CLOCK_LPO_1KHZ  1
#define LPTMR_CLOCK_RTC_32KHZ 2
#define LPTMR_CLOCK_RFOSC     3  
   
#define PCC_OPT1_SOSC   1
#define PCC_OPT2_SIRC   2
#define PCC_OPT3_FIRC   3
#define PCC_OPT6_LPFLL  6 
   
#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @name Project setup functions
 * @{
 */
/*******************************************************************************
 * API
 ******************************************************************************/
 
void WatchdogEnable(WDOG_Type * WDOGx, unsigned long timeout);  
void WatchdogDisable(WDOG_Type * WDOGx);  
void ClockSetup(void);
void SystickInitialisation(uint32_t reload_value);
void LPTMR_initialisation(LPTMR_Type * LPTMRx, uint32_t clock_source);
void PortSetup(uint32_t gpio, uint8_t pinNum, uint8_t pinDir, uint8_t pinMux);
uint32_t UartSetup(uint32_t baudRate, uint32_t clockFreq, uint32_t clockSource);
void ADC_setup(uint32_t command_buffer);

#ifdef __cplusplus
}
#endif

#endif /* _PROJECT_SETUP_K32W_CM4_H_ */ 
 
