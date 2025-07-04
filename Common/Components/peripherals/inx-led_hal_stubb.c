/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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