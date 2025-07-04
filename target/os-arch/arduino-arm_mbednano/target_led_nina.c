/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/


#include "ehs_types.h"
#include "hal-api.h"
#include "hal_led.h"

#include <WiFiNINA.h>

#define NINA_LEDR 0
#define NINA_LEDG 1
#define NINA_LEDB 2

EHS_LOCAL ehs_bool gLedR_Enabled = EHS_FALSE;
EHS_LOCAL ehs_bool gLedG_Enabled = EHS_FALSE;
EHS_LOCAL ehs_bool gLedB_Enabled = EHS_FALSE;


EHS_GLOBAL ehs_bool EhsEnableLED(ehs_uint8 id)
{
    if      (id == NINA_LEDR){
        if(gLedR_Enabled == EHS_FALSE){
            pinMode(LEDR, OUTPUT);
            gLedR_Enabled = EHS_TRUE;
        }
    }else if(id == NINA_LEDG){
        if(gLedG_Enabled == EHS_FALSE){
            pinMode(LEDG, OUTPUT);
            gLedG_Enabled = EHS_TRUE;
        }
    }else if(id == NINA_LEDB){
        if(gLedB_Enabled == EHS_FALSE){
            pinMode(LEDB, OUTPUT);
            gLedB_Enabled = EHS_TRUE;
        }
    }else{
        // unknown id
        return EHS_FALSE;
    }

    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDisableLED(ehs_uint8 id)
{
    if      (id == NINA_LEDR){
        if(gLedR_Enabled == EHS_TRUE){
            pinMode(LEDR, OUTPUT);
            digitalWrite(LEDR, LOW);
            gLedR_Enabled = EHS_FALSE;
        }
    }else if(id == NINA_LEDG){
        if(gLedG_Enabled == EHS_TRUE){
            pinMode(LEDG, OUTPUT);
            digitalWrite(LEDG, LOW);
            gLedG_Enabled = EHS_FALSE;
        }
    }else if(id == NINA_LEDB){
        if(gLedB_Enabled == EHS_TRUE){
            pinMode(LEDB, OUTPUT);
            digitalWrite(LEDB, LOW);
            gLedB_Enabled = EHS_FALSE;
        }
    }else{
        // unknown id
        return EHS_FALSE;
    }

    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsSetLED(ehs_uint8 id, ehs_bool state, ehs_uint8 brightness)
{
    // led nina brightness is inverted, so map (0-100)% => (255-0)
    int pwm = map(brightness, 0, 100, 255, 0);
    if      (id == NINA_LEDR){
        if(gLedR_Enabled == EHS_TRUE){
            if(state == EHS_FALSE){
                pinMode(LEDR, OUTPUT);
                digitalWrite(LEDR, LOW);
            }else{
                analogWrite(LEDR, pwm);
            }
        }
    }else if(id == NINA_LEDG){
        if(gLedG_Enabled == EHS_TRUE){
            if(state == EHS_FALSE){
                pinMode(LEDG, OUTPUT);
                digitalWrite(LEDG, LOW);
            }else{
                analogWrite(LEDG, pwm);
            }
        }
    }else if(id == NINA_LEDB){
        if(gLedB_Enabled == EHS_TRUE){
            if(state == EHS_FALSE){
                pinMode(LEDB, OUTPUT);
                digitalWrite(LEDB, LOW);
            }else{
                analogWrite(LEDB, pwm);
            }
        }
    }else{
        // unknown id
        return EHS_FALSE;
    }

    return EHS_TRUE;
}
