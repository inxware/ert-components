/* (c) inx limited 2015 
    Fragment of NXP kentid GPIO code from legacy projects.

*/


/*
Internal device specific functions
Tis is all NXP GPIO API specific so is nly relevant in principle - not symbols are expected to found in esp32
*/
static GPIO_Type* get_base(const ehs_sint32 gpio)
{
    /*switch(gpio){
        case 0:
        case 1:
        case 2:
        case 3:
            return CONFIG_GPIO_PROCBOARDLEDS_BASE;
        case 4:
            return CONFIG_GPIO_BUTTON_SET_BASE;
    #if INX_NXP_KENETIS_BOARD_VARIANT == INX_HR_HRc
        case 5:
            return CONFIG_GPIO_BUTTON_MODE_BASE;
    #endif
        case 6:
            return CONFIG_GPIO_BUTTON_DOWN_BASE;
        case 7:
            return CONFIG_GPIO_BUTTON_UP_BASE;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return CONFIG_GPIO_CONTROL_BASE_OUT;
        case 14:
        case 15:
            return CONFIG_GPIO_SENSE_CONRELAY_BASE;
        default:
            return NULL;
    }
    */
   return NULL;
}

static uint32_t get_pin(const ehs_sint32 gpio)
{
    /*switch(gpio){
        case 0:
            return CONFIG_GPIO_PROCBOARDLED1_RED_PIN;
        case 1:
            return CONFIG_GPIO_PROCBOARDLED1_GRN_PIN;
        case 2:
            return CONFIG_GPIO_PROCBOARDLED2_RED_PIN;
        case 3:
            return CONFIG_GPIO_PROCBOARDLED2_GRN_PIN;
        case 4:
            return CONFIG_GPIO_BUTTON_SET_PIN;
    #if INX_NXP_KENETIS_BOARD_VARIANT == INX_HR_HRc
        case 5:
            return CONFIG_GPIO_BUTTON_MODE_PIN;
    #endif
        case 6:
            return CONFIG_GPIO_BUTTON_DOWN_PIN;
        case 7:
            return CONFIG_GPIO_BUTTON_UP_PIN;
        case 8:
            return CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1;
        case 9:
            return CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2;
        case 10:
            return CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3;
        case 11:
            return CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1;
        case 12:
            return CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2;
        case 13:
            return CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3;
        case 14:
            return CONFIG_GPIO_SENSE_CONRELAY_1;
        case 15:
            return CONFIG_GPIO_SENSE_CONRELAY_2;
        default:
            return 0;
    }
    */
   return 0;
}

static PORT_Type* get_port(const ehs_sint32 gpio){
    /*
    switch(gpio){
        case 1:
        case 2:
            return PORTE;
        case 3:
            return CONFIG_GPIO_BUTTON_DOWN_PORT;
        default:
            return CONFIG_GPIO_BUTTON_SET_PORT;
    }
    */
   return 0;
}
