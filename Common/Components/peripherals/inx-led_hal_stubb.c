/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/


#include "ehs_types.h"
#include "hal_led.h"


EHS_GLOBAL ehs_bool EhsEnableLED(ehs_uint8 id)
{
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDisableLED(ehs_uint8 id)
{
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsSetLED(ehs_uint8 id, ehs_bool state, ehs_uint8 brightness)
{
    return EHS_TRUE;
}