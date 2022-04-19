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

#ifndef _SAFETY_CONFIG_H_
#define _SAFETY_CONFIG_H_
   
#include "project_setup_frdmk3s_cm4.h"
#include "IEC60730_B_CM4_CM7.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define DEBUG_WD_REFRESH_TURN_OFF 0  /* 1 - simulate a breaking of refresh mechanism, 0 - normal operation */
#define DEBUG_WD_TIME_TO_RESET 40    /* HAS EFFECT IF PREVIOUS DEFINE IS IN 1. once this value decrements to 0, refreshing of WD will stop */

#if(1)
#define SAFETY_ERROR_ACTION
#endif


#if(1)
#define WATCHDOG_ON
#else
#warning "WATCHDOG TIMER IS DISABLED!!!!!!!!!!!!!"
#endif

#if(1)
#define PC_TEST_ON
#endif

#if(1)
#define RUN_CM0PLUS_CORE
#endif

/* define resets that trigger the watchdog test */
//#define WD_RUN_TEST_CONDITION_SRS0  SMC_SRS_JTAG_MASK | SMC_SRS_SW_MASK | SMC_SRS_PIN_MASK | SMC_SRS_POR_MASK
#define WD_RUN_TEST_CONDITION_SRS0  0 //PBB we don't have this
#define WD_RUN_TEST_CONDITION_SRS1  0
//#define WD_CHECK_TEST_CONDITION     SMC_SRS_WDOG_MASK
#define WD_CHECK_TEST_CONDITION     0
   
//#define Watchdog_refresh    WDOG0->CNT = 0xB480A602;
#define Watchdog_refresh    WDOG0->RSTCNT = (uint16_t)0xB480A602;

#define WDOG_USED      WDOG0
#define WATCHDOG_CLOCK            1000  /* Hz */   
#define ENDLESS_LOOP_ENABLE       0     /* set 1 or 0 */
#define WATCHDOG_RESETS_LIMIT     1000
#define WATCHDOG_TIMEOUT_VALUE    200
#define WATCHDOG_REFRESH_RATIO    1   
#define WD_TEST_TOLERANCE         20         /* % */ 
#define WD_REF_TIMER_CLOCK_FREQUENCY  500000 

#define LPTMR_USED                LPTMR0
#define SYSTEM_CLOCK              48000000
#define SYSTICK_RELOAD_VALUE      80000   
#define LPTMR_CLOCK_FREQUENCY     500000  /* 2000000/4 */
#define REF_TIMER_CLOCK_FREQUENCY  LPTMR_CLOCK_FREQUENCY    

#define ISR_FREQUENCY             600    /* SYSTEM_CLOCK / SYSTICK_RELOAD_VALUE */
#define CLOCK_TEST_TOLERANCE      20         /* % */
  

#define PC_TEST_PATTERN_ADDR0 	  0x20005554 /* #1 test address for Program counter test */ 
#define PC_TEST_PATTERN_ADDR1 	  0x2001AAAA /* #2 test address for Program counter test */  
   
#define STACK_TEST_PATTERN        0x77777777
//#define STACK_TEST_BLOCK_SIZE     0x10       /* must fit with the setup from linker configuration file */

#define FLASH_TEST_BLOCK_SIZE     0 //force it to use calculated program flash size

#if defined(__IAR_SYSTEMS_ICC__)
    #define FLASH_TEST_CONDITION_SEED 0x0000
#endif

#if defined(__GNUC__) || defined(__CC_ARM)

    #define FLASH_TEST_CONDITION_SEED 0x1D0F

		/* This must be in consistence with setting in "User AFTER BUILD = srec_cat*/
    //#define FLASH_TEST_START_ADDRESS  0x0
    //#define FLASH_TEST_END_ADDRESS    0x7A000 //the last block of memory before our CRC value
    //#define CRC_VALUE_ADDR            0x10000000 //TODO PBB I don't think we need this
#endif   


   
#define RAM_TEST_BACKUP_SIZE      0x20       /* must fit with the setup from linker configuration file */
#define RAM_TEST_BLOCK_SIZE       0x4        /* size of block for runtime testing */
   
#define UART_BAUD_RATE  115200   
   
/*Dio port settings*/
#define DIO_PORT_IN PORTD
#define DIO_PIN_IN  6


#define DIO_PORT_OUT PORTD
#define DIO_PIN_OUT  7

/*GPIO macros*/
#define PIN_IN  0
#define PIN_OUT 1
#define PIN_NON 2

/*Alternate function*/
#define ALT1    1
#define ALT3    3   
   
#define TESTED_ADC ADC0
   
#define ADC_RESOLUTION  12
#define ADC_MAX  ((1<<(ADC_RESOLUTION))-1)
#define ADC_REFERENCE  3.06
#define ADC_BANDGAP_LEVEL  1.0
#define ADC_BANDGAP_LEVEL_RAW  (((ADC_BANDGAP_LEVEL)*(ADC_MAX))/(ADC_REFERENCE))
#define ADC_DEVIATION_PERCENT 20
#define ADC_MIN_LIMIT(val)  (unsigned short)(((val) * (100 - ADC_DEVIATION_PERCENT)) / 100)
#define ADC_MAX_LIMIT(val)  (unsigned short)(((val) * (100 + ADC_DEVIATION_PERCENT)) / 100)
#define IEC60730B_CFG_AIO_CHANNELS_CNT    3
#define IEC60730B_CFG_AIO_CHANNELS_LIMITS_INIT \
{\
    {ADC_MIN_LIMIT(0), ADC_MAX_LIMIT(30)}, \
    {ADC_MIN_LIMIT(ADC_MAX), ADC_MAX_LIMIT(ADC_MAX)},\
    {ADC_MIN_LIMIT(ADC_BANDGAP_LEVEL_RAW), ADC_MAX_LIMIT(ADC_BANDGAP_LEVEL_RAW)}\
}
#define IEC60730B_CFG_AIO_CHANNELS_INIT {30, 29, 27}  /* ADC Channels for V_refh, V_refl, bandgap */
#define ADC_COMMAND_BUFFER  2   /* must be from 1 to 15 */

#define IEC60730B_CFG_AIO_SAMPLES_INIT {2}  /* ADC SAMPLES number for test  KV46 */

/*Program Counter TEST*/
#define PC_TEST_PATTERN    0x20000000/* test address for Program counter test (in RAM region) */

/* GPIO macros */
#define PIN_DIRECTION_IN  0
#define PIN_DIRECTION_OUT 1

#define PIN_MUX_GPIO 1
#define PIN_MUX_UART 3

#define LOGICAL_ONE   1
#define LOGICAL_ZERO  0
   
/*Dio port settings*/
#define DIO_EXPECTED_VALUE 0

#define DIO_WAIT_CYCLE 100

#define DIO_BACKUP_ENABLE    1
#define DIO_BACKUP_DISABLE   0
#define DIO_BACKUP  DIO_BACKUP_ENABLE 

#define DIO_SHORT_TO_GND_TEST   1
#define DIO_SHORT_TO_VDD_TEST   0

/******************************************************************************/   
/*Define of use SIM or PCC*/
#define USE_PCC 1
#define USE_SIM 0


/*UART marcors*/
#define APPLICATION_UART       LPUART0

#define UART_PORT_RX    IEC60730B_DIO_PORTB, 16
#define UART_PORT_TX    IEC60730B_DIO_PORTB, 17
#define UART_ALT PIN_MUX_UART

//#define UART_BAUD_RATE      57600
#define UART_CLOCK_INPUT    80e6

#define USE_UART 1 /*1 = use uart, 0 = use LPUART*/

/* unused */
#define CLOCK_GPT1_FREQ  1
#define CLOCK_GPT2_FREQ  1
#define CLOCK_GPT1_SRC   1
#define CLOCK_GPT1_CMP   1
#define CLOCK_GPT1_DIV   1
#define CLOCK_GPT2_SRC   1
#define CLOCK_GPT2_CMP   1
#define CLOCK_GPT2_DIV   1

#endif /* _SAFETY_CONFIG_H_ */
