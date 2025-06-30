#include "hal_pwm_legacy.h"
#include <Arduino.h>

// @TODO - this a basic arduino pwm implementation, we should use the one with frequency instead

ehs_bool EhsPWMConfig(const ehs_uint8 pin, const ehs_uint32 periodHz)
{
    return EHS_TRUE;
}

ehs_bool EhsPWMEnable(const ehs_uint8 pin, const ehs_bool enable)
{
    if(enable == EHS_TRUE){
        pinMode(pin, OUTPUT);
    }else{
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    return EHS_TRUE;
}

ehs_bool EhsPWMDuty(const ehs_uint8 pin, const ehs_uint8 powerPercent)
{
    int percentage = constrain(powerPercent, 0, 100);
    // map 0-100% to 0-255
    int pwm = map(percentage, 0, 100, 0, 255);
    // write the PWM value
    analogWrite(pin, pwm);
    return EHS_TRUE;
}