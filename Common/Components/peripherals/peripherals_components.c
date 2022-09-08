/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

#include "blockref_table.h"
#include "dtv_remote2.h"
#include "keypress.h"
#include "gpio_out.h"
#include "gpio_in.h"

EhsBlockRefType EhsBlockRefTable_Peripherals[] =
{
#ifdef EHS_PERIPHERALS_RCU
		EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DtvRemoteKey2,EHS_FB_ID_DtvRemoteKey2, DtvRemote2),
#endif
#ifdef EHS_PERIPHERALS_GUI_KEYBOARD
		EHS_BLOCKREF_ENTRY(EHS_FB_NAME_GUI_keypress,EHS_FB_ID_GUI_keypress, GUI_keypress),
#endif
#ifdef EHS_PERIPHERALS_GPIO
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_gpio_out,EHS_FB_ID_gpio_out, gpio_out),
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_gpio_in,EHS_FB_ID_gpio_in, gpio_in),
#endif
    {0}
};
