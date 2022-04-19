/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MK64FN1M0xxx12
package_id: MK64FN1M0VLL12
mcu_data: ksdk2_0
processor_version: 2.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "heatrod_config.h"
#include "config.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"



#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART 0 transmit data source select: UART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '62', peripheral: UART0, signal: RX, pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN}
  - {pin_num: '63', peripheral: UART0, signal: TX, pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b}
  - {pin_num: '69', peripheral: GPIOB, signal: 'GPIO, 23', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28}
  - {pin_num: '57', peripheral: GPIOB, signal: 'GPIO, 9', pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20}
  - {pin_num: '90', peripheral: ENET, signal: 'TMR_1588, 0', pin_signal: PTC16/UART3_RX/ENET0_1588_TMR0/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b}
  - {pin_num: '91', peripheral: ENET, signal: 'TMR_1588, 1', pin_signal: PTC17/UART3_TX/ENET0_1588_TMR1/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b}
  - {pin_num: '92', peripheral: ENET, signal: 'TMR_1588, 2', pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b}
  - {pin_num: '54', peripheral: ENET, signal: RMII_MDC, pin_signal: ADC0_SE9/ADC1_SE9/PTB1/I2C0_SDA/FTM1_CH1/RMII0_MDC/MII0_MDC/FTM1_QD_PHB}
  - {pin_num: '53', peripheral: ENET, signal: RMII_MDIO, pin_signal: ADC0_SE8/ADC1_SE8/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/RMII0_MDIO/MII0_MDIO/FTM1_QD_PHA, slew_rate: fast,
    open_drain: enable, drive_strength: low, pull_select: up, pull_enable: enable, passive_filter: disable}
  - {pin_num: '42', peripheral: ENET, signal: RMII_RXD1, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA}
  - {pin_num: '43', peripheral: ENET, signal: RMII_RXD0, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB}
  - {pin_num: '44', peripheral: ENET, signal: RMII_CRS_DV, pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: '45', peripheral: ENET, signal: RMII_TXEN, pin_signal: PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0}
  - {pin_num: '47', peripheral: ENET, signal: RMII_TXD1, pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1/I2S0_MCLK}
  - {pin_num: '46', peripheral: ENET, signal: RMII_TXD0, pin_signal: PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: '39', peripheral: ENET, signal: RMII_RXER, pin_signal: PTA5/USB_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

  const port_pin_config_t PinConfig_OpenDrain = { // todo rename this properly
		kPORT_PullDisable,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainEnable,                                   /* Open drain is disabled */
    kPORT_HighDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                           /* Pin is configured as gpio */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };

  const port_pin_config_t PinConfig_drive = {
		kPORT_PullDisable,                                            /* Internal pull-up resistor is enabled */
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
		kPORT_OpenDrainDisable,                                   /* Open drain is disabled */
		kPORT_HighDriveStrength,                                  /* Low drive strength is configured */
		kPORT_MuxAsGpio,                                           /* Pin is configured as gpio */
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */

  };

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/

/* todo we should have a separate version iof this function for each board type + a common one? */
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port C Clock Gate Control: Clock enabled */

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_FRDMK64EVAL
/* PORTB23 (pin 69) is configured as PTB23 */
    PORT_SetPinMux(PORTB, 23U, kPORT_MuxAsGpio);

    /* PORTB9 (pin 57) is configured as PTB9 */
    PORT_SetPinMux(PORTB, 9U, kPORT_MuxAsGpio);
#endif
    //our uart enable and disable pin
    PORT_SetPinMux(PORTB, 2U, kPORT_MuxAsGpio);

#if CONFIG_TLK106
  //this is the phy reset port and needs to be clocked as GPIO
  PORT_SetPinMux(PORTA,CONFIG_GPIO_PHY_RESET,kPORT_MuxAsGpio);

  const port_pin_config_t PinConfig_RMII_MDIO = {
      kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
      kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
      kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
      kPORT_OpenDrainDisable,                                   /* Open drain is enabled */
      kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
      kPORT_MuxAsGpio,                                           /* Pin is configured as RMII0_MDIO */
      kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
    };
#endif

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc  || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
  //Status LEDs
  PORT_SetPinConfig(PORTE, 0, &PinConfig_RMII_MDIO);
  PORT_SetPinMux(PORTE,CONFIG_GPIO_PROCBOARDLED1_RED_PIN,kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTE,CONFIG_GPIO_PROCBOARDLED1_GRN_PIN,kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTE,CONFIG_GPIO_PROCBOARDLED2_RED_PIN,kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTE,CONFIG_GPIO_PROCBOARDLED2_GRN_PIN,kPORT_MuxAsGpio);
  #if CONFIG_LIFESTYLE == INX_LIFESTYLE_COMMISSION
    PORT_SetPinMux(CONFIG_GPIO_SENSE_LED_PORT, CONFIG_GPIO_SENSE_LED_PIN, kPORT_MuxAsGpio);
  #endif //if CONFIG_LIFESTYLE == INX_LIFESTYLE_COMMISSION
#endif //INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD

  PORT_SetPinMux(PORTA, PIN12_IDX, kPORT_MuxAlt4);           /* PORTA12 (pin 42) is configured as RMII0_RXD1 */
  PORT_SetPinMux(PORTA, PIN13_IDX, kPORT_MuxAlt4);           /* PORTA13 (pin 43) is configured as RMII0_RXD0 */
  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt4);           /* PORTA14 (pin 44) is configured as RMII0_CRS_DV */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt4);           /* PORTA15 (pin 45) is configured as RMII0_TXEN */
  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt4);           /* PORTA16 (pin 46) is configured as RMII0_TXD0 */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt4);           /* PORTA17 (pin 47) is configured as RMII0_TXD1 */
  PORT_SetPinMux(PORTA, PIN5_IDX, kPORT_MuxAlt4);            /* PORTA5 (pin 39) is configured as RMII0_RXER */
  const port_pin_config_t PinConfig_RMII_MDIO_B = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt4,                                           /* Pin is configured as RMII0_MDIO */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };

  /* Port B Config */

  PORT_SetPinConfig(PORTB, PIN0_IDX, &PinConfig_RMII_MDIO_B);  /* PORTB0 (pin 53) is configured as RMII0_MDIO */
  PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_MuxAlt4);            /* PORTB1 (pin 54) is configured as RMII0_MDC */
  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 62) is configured as UART0_RX */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 63) is configured as UART0_TX */
/* todo Should explicitly set the CONTROL OUTPUTS to kPORT_MuxAsGpio here? or does the following do it? */



/* Control output DIO */
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
  /* todo - do we need to explicitly set these to GPIO using PORT_SetPinMux(... kPORT_MuxAsGpio);? */
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1,&PinConfig_OpenDrain);
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2,&PinConfig_OpenDrain);
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3,&PinConfig_OpenDrain);
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1,&PinConfig_drive);
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2,&PinConfig_drive);
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3,&PinConfig_drive);
#else
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1,&PinConfig_OpenDrain);
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2,&PinConfig_OpenDrain);
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3,&PinConfig_OpenDrain);
#endif

  PORT_SetPinMux(PORTB, CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1, kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTB, CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2, kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTB, CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3, kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTB, CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1, kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTB, CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2, kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTB, CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3, kPORT_MuxAsGpio);
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
  /* todo - do we need to explicitly set these to GPIO using PORT_SetPinMux(... kPORT_MuxAsGpio);? */
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1,&PinConfig_OpenDrain);

  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1,&PinConfig_OpenDrain);
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2,&PinConfig_OpenDrain);
  PORT_SetPinConfig(PORTB,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3,&PinConfig_OpenDrain);

  PORT_SetPinMux(PORTB, CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1, kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTB, CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1, kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTB, CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2, kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTB, CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3, kPORT_MuxAsGpio);
#endif

  /* Port C config */
  /* Numerical Display outputs */

  PORT_SetPinMux(PORTC, PIN16_IDX, kPORT_MuxAlt4);           /* PORTC16 (pin 90) is configured as ENET0_1588_TMR0 */
  PORT_SetPinMux(PORTC, PIN17_IDX, kPORT_MuxAlt4);           /* PORTC17 (pin 91) is configured as ENET0_1588_TMR1 */
  PORT_SetPinMux(PORTC, PIN18_IDX, kPORT_MuxAlt4);           /* PORTC18 (pin 92) is configured as ENET0_1588_TMR2 */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK)))                          /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART 0 transmit data source select: UART0_TX pin */
    );
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
  /* Display */
  PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY_A, kPORT_MuxAsGpio);
  PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY_B, kPORT_MuxAsGpio);
  PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY_C, kPORT_MuxAsGpio);
  PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY_D, kPORT_MuxAsGpio);
  PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR1, kPORT_MuxAsGpio);
  PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR2, kPORT_MuxAsGpio);
  PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR3, kPORT_MuxAsGpio);
  PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR4, kPORT_MuxAsGpio);

  /* Front Pnale Buttons */
  // This is for the set button only connected via the processor board OR the additioanl button (using the negative sign pin)
  PORT_SetPinMux(CONFIG_GPIO_BUTTON_SET_PORT, CONFIG_GPIO_BUTTON_SET_PIN,kPORT_MuxAsGpio);


  PORT_SetPinMux(CONFIG_GPIO_BUTTON_UP_PORT, CONFIG_GPIO_BUTTON_UP_PIN,kPORT_MuxAsGpio);
  PORT_SetPinMux(CONFIG_GPIO_BUTTON_DOWN_PORT, CONFIG_GPIO_BUTTON_DOWN_PIN,kPORT_MuxAsGpio);
  #if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
    PORT_SetPinMux(CONFIG_GPIO_BUTTON_MODE_PORT, CONFIG_GPIO_BUTTON_MODE_PIN,kPORT_MuxAsGpio);
  #endif
  /* to do set, 0-10v*4-20mA adc */

#endif
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
  // GPIO/Relay Checks
   PORT_SetPinMux(CONFIG_GPIO_SENSE_CONRELAY_PORT,CONFIG_GPIO_SENSE_CONRELAY_1,kPORT_MuxAsGpio);
   PORT_SetPinMux(CONFIG_GPIO_SENSE_CONRELAY_PORT,CONFIG_GPIO_SENSE_CONRELAY_2,kPORT_MuxAsGpio);
   PORT_SetPinMux(CONFIG_GPIO_SENSE_CONRELAY_PORT,CONFIG_GPIO_SENSE_CONRELAY_3,kPORT_MuxAsGpio);
   PORT_SetPinMux(CONFIG_GPIO_SENSE_CO_PORT,CONFIG_GPIO_SENSE_THERM_CO_1,kPORT_MuxAsGpio);
   PORT_SetPinMux(CONFIG_GPIO_SENSE_CO_PORT,CONFIG_GPIO_SENSE_EXT_CO_1,kPORT_MuxAsGpio);
#endif

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
   //PORT_SetPinMux(CONFIG_GPIO_SENSE_CO_PORT,CONFIG_GPIO_SENSE_THERM_CO_1,kPORT_MuxAsGpio);
   //PORT_SetPinMux(CONFIG_GPIO_SENSE_CO_PORT,CONFIG_GPIO_SENSE_EXT_CO_1,kPORT_MuxAsGpio);

   PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY2_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR1, kPORT_MuxAsGpio);
   PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY2_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR2, kPORT_MuxAsGpio);
   PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY2_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR3, kPORT_MuxAsGpio);
   PORT_SetPinMux(CONFIG_GPIO_NUMERICAL_DISPLAY2_PORT, CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR4, kPORT_MuxAsGpio);
   PORT_SetPinMux   (CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_PORT, CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN, kPORT_MuxAsGpio);
   PORT_SetPinConfig(CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_PORT, CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN,&PinConfig_OpenDrain);
   PORT_SetPinMux   (CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN_PORT, CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN, kPORT_MuxAsGpio); // this is portc not port b
   PORT_SetPinConfig(CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN_PORT, CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN,&PinConfig_OpenDrain);
#endif

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
 #if INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv1 || INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv2
   //no negative on v1 and v2 boards
 #else //if INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv1 || INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv2
  PORT_SetPinMux(CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_PORT, CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN, kPORT_MuxAsGpio);
  PORT_SetPinConfig(CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_PORT, CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN,&PinConfig_OpenDrain);
 #endif //else if INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv1 || INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv2
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
 #if  INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRc_DISPv2
   PORT_SetPinMux(CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_PORT, CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN, kPORT_MuxAsGpio);
   PORT_SetPinConfig(CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_PORT, CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN,&PinConfig_OpenDrain);
 #else
/* version 1 boards couldn't do negative....*/
 #endif
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
   PORT_SetPinMux   (CONFIG_GPIO_DIAGNOSTIC_PROD_HIGH_IN_PIN_PORT, CONFIG_GPIO_DIAGNOSTIC_PROD_HIGH_IN_PIN, kPORT_MuxAsGpio); // this is portc not port b
   PORT_SetPinConfig(CONFIG_GPIO_DIAGNOSTIC_PROD_HIGH_IN_PIN_PORT, CONFIG_GPIO_DIAGNOSTIC_PROD_HIGH_IN_PIN,&PinConfig_OpenDrain);
#endif
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
