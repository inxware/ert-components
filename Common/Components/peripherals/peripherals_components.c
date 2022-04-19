

#include "blockref_table.h"
#include "dtv_remote2.h"
#include "keypress.h"
#include "gpio_out.h"
#include "gpio_in.h"

EhsBlockRefType EhsBlockRefTable_Peripherals[] =
{
#ifdef EHS_PERIPHERALS_RCU
		EHS_BLOCKREF_ENTRY(EHS_FB_DTV_REMOTE2_NAME, DtvRemote2),
#endif
#ifdef EHS_PERIPHERALS_GUI_KEYBOARD
		EHS_BLOCKREF_ENTRY(EHS_FB_KEYPRESS_NAME, GUI_keypress),
#endif
#ifdef EHS_PERIPHERALS_GPIO
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_GPIO_OUT_NAME, gpio_out),
		EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_GPIO_IN_NAME, gpio_in),
#endif
	{0}
};
