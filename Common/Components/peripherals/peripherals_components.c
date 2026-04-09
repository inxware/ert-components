/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#include "blockref_table.h"
#include "dtv_remote2.h"
#include "keypress.h"
#include "gpio_out.h"
#include "gpio_in.h"
#include "inx-led.h"
#include "inx-accel_gyro.h"

#ifdef EHS_WATCHDOG_SUPPORT
#include "watchdog.h"
#endif

#ifdef EHS_UPS_SUPPORT
#include "ups.h"
#endif

#ifdef EHS_BUZZER_SUPPORT
#include "buzzer.h"
#endif

#ifdef EHS_SD_SELECT_SUPPORT
#include "sd_select.h"
#endif

#ifdef EHS_USB_POWER_SUPPORT
#include "usb_power.h"
#endif

#ifdef EHS_RS485_CONFIG_SUPPORT
#include "rs485_config.h"
#endif

// Components moved from user directory
#ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
#include "inx-adc_config.h"
#ifdef EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT
#include "inx-adc_read_continuous.h"
#endif
#include "inx-adc_read_single.h"
#include "inx-dac.h"
#endif

#ifdef EHS_PERIPHERALS_PWM_SUPPORT
//#include "inx-pwm.h"
#include "inx-pwm_gen.h"
#endif

#ifdef EHS_UART_SUPPORT
#include "inx-uart.h"
#include "inx-uart_config.h"
#endif

#ifdef EHS_MODBUS_SUPPORT
#include "inx-modbus_read.h"
#include "inx-modbus_write.h"
#include "inx-modbus_config.h"
#include "inx-modbus_slave_register.h"
#endif

EHS_C_CPP_EXPORT const EhsBlockRefType EhsBlockRefTable_Peripherals[] =
{
#ifdef EHS_PERIPHERALS_RCU
		EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DtvRemoteKey2,EHS_FB_ID_DtvRemoteKey2, DtvRemote2),
#endif
#ifdef EHS_PERIPHERALS_GUI_KEYBOARD
		EHS_BLOCKREF_ENTRY(EHS_FB_NAME_GUI_keypress,EHS_FB_ID_GUI_keypress, GUI_keypress),
#endif

#ifdef EHS_PERIPHERALS_GPIO_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_gpio_out,EHS_FB_ID_gpio_out, gpio_out),
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_gpio_in,EHS_FB_ID_gpio_in, gpio_in),
#endif

#ifdef EHS_PERIPHERALS_LED_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_led, INXWARE_FB_ID_led, led),
#endif

#ifdef EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_accel_gyro, INXWARE_FB_ID_accel_gyro, accel_gyro),
#endif

// Components moved from user directory
#ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_adc_config,INXWARE_FB_ID_adc_config,adc_config),
#ifdef EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_adc_read_continuous,INXWARE_FB_ID_adc_read_continuous,adc_read_continuous),
#endif
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_adc_read_single,INXWARE_FB_ID_adc_read_single,adc_read_single),
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_dac,INXWARE_FB_ID_dac,dac),
#endif

#ifdef EHS_PERIPHERALS_PWM_SUPPORT
//		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_pwm,INXWARE_FB_ID_pwm ,pwm),
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_pwm_gen,INXWARE_FB_ID_pwm_gen,pwm_gen),
#endif

#ifdef EHS_UART_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_UART,INXWARE_FB_ID_UART,UART),
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_uart_config,INXWARE_FB_ID_uart_config,uart_config),
#endif

#ifdef EHS_MODBUS_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_config,INXWARE_FB_ID_modbus_config,modbus_config),
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_read,INXWARE_FB_ID_modbus_read ,modbus_read),
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_write,INXWARE_FB_ID_modbus_write ,modbus_write),
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_slave_register,INXWARE_FB_ID_modbus_slave_register,modbus_slave_register),
#endif

#ifdef EHS_WATCHDOG_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_watchdog, EHS_FB_ID_watchdog, watchdog),
#endif

#ifdef EHS_UPS_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_ups, EHS_FB_ID_ups, ups),
#endif

#ifdef EHS_BUZZER_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_buzzer, EHS_FB_ID_buzzer, buzzer),
#endif

#ifdef EHS_SD_SELECT_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_sd_select, EHS_FB_ID_sd_select, sd_select),
#endif

#ifdef EHS_USB_POWER_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_usb_power, EHS_FB_ID_usb_power, usb_power),
#endif


#ifdef EHS_RS485_CONFIG_SUPPORT
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_rs485_config, EHS_FB_ID_rs485_config, rs485_config),
#endif

    {0}
};
