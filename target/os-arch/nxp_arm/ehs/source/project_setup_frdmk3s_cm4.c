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

#include "project_setup_frdmk3s_cm4.h"
#include "safety_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   Watchdog configuration function
 *
 *          Enables the watchdog. Also in Wait and Stop mode. Updates are allowed
 * 
 * @param   *WDOGx - pointer to the base address of the periphery
 *          timeout - timeout to wd reset
 *
 * @return  None
 */
void WatchdogEnable(WDOG_Type * WDOGx, unsigned long timeout)   
{   
    //WDOGx->CNT = 0xD928C520U;            /* unlock sequence */
    //while(!(WDOGx->CS && WDOG_CS_ULK_MASK)); /* unlock succesful*/
    //WDOGx->TOVAL = (uint16_t)timeout;        /* Watchdog timeout value */
    
    /* Watchdog enabled in chip stop mode. */
    /* Watchdog enabled in chip wait mode. */
    /* Updates allowed. */
    /* Watchdog enabled. */
    /* Enables support for 32-bit refresh/unlock command write words. 16-bit or 8-bit is NOT supported. */
    /* LPO clock , 128 KHz */    
    //WDOGx->CS =  WDOG_CS_STOP_MASK | WDOG_CS_WAIT_MASK | WDOG_CS_UPDATE_MASK | WDOG_CS_EN_MASK | WDOG_CS_CMD32EN_MASK | WDOG_CS_CLK(1);
    
    //while(!(WDOGx->CS && WDOG_CS_RCS_MASK)); /* reconfiguration was succesful */
}  
     
/*!
 * @brief   Watchdog disabling function
 * 
 * @param   *WDOGx - pointer to the base address of the periphery
 *
 * @return  None
 */
void WatchdogDisable(WDOG_Type * WDOGx)
{  
    __asm("CPSID i"); /* disable interrupts */
    //WDOGx->CNT = 0xD928C520U;  /* unlock sequence */
    //while(!(WDOGx->CS && WDOG_CS_ULK_MASK)); /* unlock succesful*/
    //WDOGx->TOVAL = 0xFFFF;
    //WDOGx->CS = (uint32_t) ((WDOGx->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;
    //while(!(WDOGx->CS && WDOG_CS_RCS_MASK)); /* reconfiguration was succesful */
    __asm("CPSIE i"); /* enable interrupts */ 
}   


/*!
 * @brief   Setup of clock
 *
 *          SIRC --> SIRCDIV3 --> PCC0 --> LPTMR0
 *           
 *
 *
 *
 * @param   void
 *
 * @return  None
 *
 */
void ClockSetup(void)
{
    /* SIRC setup */
    //SCG->SIRCCSR = 0; /* clear the register, disable slow IRC */
    //SCG->SIRCDIV = 0;
    //SCG->SIRCDIV |= SCG_SIRCDIV_SIRCDIV3(DIVIDE_BY_4);
    //SCG->SIRCCFG &= ~SCG_SIRCCFG_RANGE_MASK; /* SIRC low range clock 2MHz */
    //SCG->SIRCCSR |= SCG_SIRCCSR_SIRCEN_MASK; /* SIRC enabled */
    //while (!(SCG->SIRCCSR & SCG_SIRCCSR_SIRCVLD_MASK)) /* Wait for SIRC clock to be valid. */
    //{
    //}
    //SCG->RCCR |= SCG_RCCR_SCS(2); /* SIRC is the system clock */
    
    /* FIRC setup */
    //SCG->FIRCCSR = 0; /* clear the register, disable fast IRC */
    //SCG->FIRCDIV = SCG_FIRCDIV_FIRCDIV1(DIVIDE_BY_1) | SCG_FIRCDIV_FIRCDIV2(DIVIDE_BY_1) | SCG_FIRCDIV_FIRCDIV3(DIVIDE_BY_1);
    //SCG->FIRCCFG = SCG_FIRCCFG_RANGE(0); /* FIRC is trimmed to 48 MHz */
    //SCG->FIRCCSR |= (SCG_FIRCCSR_FIRCEN_MASK | SCG_FIRCCSR_FIRCTREN_MASK); /* enable FIRC */
    //while (!(SCG->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK)) /* Wait for FIRC clock to be valid. */
    //{
    //}
   
    /* LPFLL setup */
    //SCG->LPFLLCSR = 0; /* clear the register, disable LPFLL */
    //SCG->LPFLLDIV = SCG_LPFLLDIV_LPFLLDIV1(CLOCK_DISABLED)|SCG_LPFLLDIV_LPFLLDIV2(CLOCK_DISABLED)|SCG_LPFLLDIV_LPFLLDIV3(DIVIDE_BY_1);
    //SCG->LPFLLCFG = SCG_LPFLLCFG_FSEL(0); /* LPFLL is trimmed to 48 MHz */
    //SCG->LPFLLCSR |= (SCG_LPFLLCSR_LPFLLEN_MASK); /* enable LPFLL */
    //while (!(SCG->LPFLLCSR & SCG_LPFLLCSR_LPFLLVLD_MASK)) /* Wait for LPFLL clock to be valid. */
    //{
    //}
    //SCG->RCCR |= SCG_RCCR_SCS(3); /* FIRC is the system clock */
}  


/*!
 * @brief   Initialization of Systick timer
 *
 *          This function configures the Systick as a source of interrupt
 *
 * @param   reload_value - defines the period of counter refresh
 *
 * @return  None
 */
void SystickInitialisation(uint32_t reload_value)
{
    SysTick->VAL = 0;
    SysTick->LOAD = reload_value;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}


/*!
 * @brief   Initialization of LPTMR
 *
 *          This function initializes the LPTMR. LPTMR is used for clock test.
 *
 *          SIRC --> SIRCDIV3 --> PCC0 --> LPTMR0
 *
 * @param   LPTMRx - pointer to base address of the periphery
 *          clock_source - clock source for the timer
 *
 * @return  None
 */
void LPTMR_initialisation(LPTMR_Type * LPTMRx, uint32_t clock_source)
{
    LPTMRx->CSR = 0;                                        /* time counter mode */
    LPTMRx->CSR = LPTMR_CSR_TCF_MASK | LPTMR_CSR_TFC_MASK;  /* CNR reset on overflow */
    LPTMRx->PSR |= LPTMR_PSR_PBYP_MASK;                     /* prescaler bypassed, selected clock directly clocks the CNR */     
    LPTMRx->PSR &= (~LPTMR_PSR_PCS_MASK);                   /* clear prescaler clock */
    LPTMRx->PSR |= LPTMR_PSR_PCS(clock_source);     
    LPTMRx->CMR = 0;                                        /* clear the compare register */ 
    LPTMRx->CSR |= LPTMR_CSR_TEN_MASK;                      /* enable timer */
}

/*!
* @brief  Sets port direction and mux
*
* @param  gpio   - definition from IEC60730_B_CM4_CM7_dio.h, for example IEC60730B_DIO_PORTA_K3S for PORT A
*         pinNum - pin number
*         pinDir - pin direction
*         pinMux - pin mux (usually GPIO_MUX == 1)
*
* @return  None
*/
 void PortSetup(uint32_t gpio, uint8_t pinNum, uint8_t pinDir, uint8_t pinMux)
{

}


/*!
 * @brief   Setup of LPUART
 *
 *          SIRC --> SIRCDIV2 --> PCC0 --> LPUART0
 *          
 *
 * @param   baudRate  - desired baud rate
 * @param   clockFreq - information about selected input clock frequency
 *
 * @return  1 - baud rate configurated correctly
 *          0 - baud rate is not correctly configurated
 */
uint32_t UartSetup(uint32_t baudRate, uint32_t clockFreq, uint32_t clockSource)
{
    return (1);
}


/*!
 * @brief   Setup of LPADC 
 *
 *          
 * 
 *
 * @param   command_buffer  - selects the command buffer
 *
 * @return  none
 */
void ADC_setup(uint32_t command_buffer)
{
}
