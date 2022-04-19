/** @file modules.c
 * Definitions of statically linked library modules.
 * 
 * @author: Pierre Drezet
 * @version: $Revision: 2710 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inxLtd, 2011. All rights reserved.
 */

//#include "target_config.h"
#include "statmodules.h"
#include "blockref_table.h"
//#include "hal-api.h"

/* For the core toolkit */
#include "timer.h"
/**
 * Declares the block reference table containing all functions held within EHS
 */
//extern EhsBlockRefTableType EhsBlockRefTable_core[];

extern EhsBlockRefType EhsBlockRefTable_core[];


		/**
 * Declares the core GUI function block toolkit
 */
#ifdef EHS_GUI_SUPPORT
#include "hal_viewport.h"
#include "widget.h"
extern EhsBlockRefType  EhsBlockRefTable_coreGui[];
#endif

/**
 * Depricated Declares the DTV toolkit
 */
//#ifdef EHS_AV_SUPPORT
//EHS_GLOBAL EhsBlockRefTableType EhsBlockRefTable_Dtv;
//#endif

/**
* Declares the Media block ref tabl
*/

#ifdef EHS_PERIPHERAL_DEVICE_SUPPORT
extern EhsBlockRefType  EhsBlockRefTable_Peripherals[];
#endif


extern EhsBlockRefType  EhsBlockRefTable_Media[];

/*****************************************************************************/
/* Declare function prototypes */

/**
 * Declares a toolkit that is used to hold deprecated components
 */
#ifdef EHS_TOOLKIT_DEPRECATED
extern EhsBlockRefType EhsBlockRefTable_deprecated[];
#endif

/**
 * Declares a toolkit that is used to hold experimental components
 */
#ifdef EHS_TOOLKIT_SANDBOX
extern EhsBlockRefType  EhsBlockRefTable_sandbox[];
#endif

/**
 * Declares a toolkit that is used to hold networking components
 */
#ifdef EHS_COMPONENT_NETWORKING_SUPPORT
extern EhsBlockRefType  EhsBlockRefTable_networking[];
#endif

/**
 * Declares a toolkit for user components
 */
#define EHS_USER_COMPONENT_SUPPORT
#ifdef EHS_USER_COMPONENT_SUPPORT
extern EhsBlockRefType  EhsBlockRefTable_usercomponents[];
#endif




/**
 * Statically linked libraries here. You may not trust the dll loader for disaster recovery apps
 */
void EhsAddStaticModules()
{
	/* now add the toolkits */
	EhsToolkitTable_addTable(EhsBlockRefTable_core);
#ifdef EHS_TOOLKIT_DEPRECATED
	EhsToolkitTable_addTable(EhsBlockRefTable_deprecated);
#endif

#ifdef EHS_TOOLKIT_SANDBOX
	EhsToolkitTable_addTable(EhsBlockRefTable_sandbox);
#endif

#ifdef EHS_GUI_SUPPORT
	EhsToolkitTable_addTable(EhsBlockRefTable_coreGui);
	//@todo the following should really be renamed to Keyboard Support and made a generic component.
	//@todo Also there is no header construct for cases where we do have HW specific components
    //Badidea:extern EhsBlockRefType EhsBlockRefTable_gtk[]; //@todo This should be declared in the same way as the above...
	//Badidea:EhsToolkitTable_addTable(EhsBlockRefTable_gtk);

#endif /* EHS_GUI_SUPPORT */

#ifdef EHS_PERIPHERAL_DEVICE_SUPPORT
	EhsToolkitTable_addTable(EhsBlockRefTable_Peripherals);
#endif

// PPP://THE DTV TOOLKIT WAS LOADED IN THE INITIALISATION FUNCTIONS FOR DTV IMPLEMENTATIONS.
#ifdef EHS_AV_SUPPORT
	// content moved the media toolkit EhsToolkitTable_addTable(EhsBlockRefTable_Dtv); //@toodo the Dtv stuff should be refactored into the media directory
	EhsToolkitTable_addTable(EhsBlockRefTable_Media);
#else
	#if defined(EHS_ANDROID) || defined(EHS_UNITY3D_WIDGETS)
		EhsToolkitTable_addTable(EhsBlockRefTable_Media);
	#endif
#endif
#ifdef EHS_COMPONENT_NETWORKING_SUPPORT
	EhsToolkitTable_addTable(EhsBlockRefTable_networking);
#endif

#ifdef EHS_USER_COMPONENT_SUPPORT
	EhsToolkitTable_addTable(EhsBlockRefTable_usercomponents);
#endif
}

ehs_bool EhsInitStaticModules()
{
	//*bNewSodlFlagRef=0 This is done in the app reset already; // This is the signal for closing Dynamic Threads in component software
	#ifdef EHS_GUI_SUPPORT
		EhsWidgetTable_init(&EhsWidgetTable); //
		EhsTV_reset(&EhsTV); // @todo PD this should be moved into toolkit initialisation functions - instead of conditional compile.
		EhsTV_clear(&EhsTV);
	#endif /* EHS_GUI_SUPPORT */

return EHS_TRUE;
}

ehs_bool EhsShutdownStaticModules()
{
	return EHS_TRUE;
}


/* Lightweight resets when application is terminated and new is to load */
ehs_bool EhsResetStaticModules()
{
	EhsTimer_init(); /* releases memory previously allocated, ... */
#ifdef EHS_GUI_SUPPORT
	EhsWidgetTable_reset(&EhsWidgetTable); //
	EhsTV_reset(&EhsTV); // @todo PD this should be moved into toolkit initialisation functions - instead of conditional compile.
	//EhsTV_clear(&EhsTV); //THIS IS DONE INSIDE EhsTV_reset() clear the viewport screen
	//EHSH_LOG_TRACE("Clearing the VIEWPORT");
#endif /* EHS_GUI_SUPPORT */
	return EHS_TRUE;
}
