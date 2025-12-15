/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file modules.c
 * Definitions of statically linked library modules.
 *
 * @author: inx limited
 *
 */

//#include "target_config.h"
#include "statmodules.h"
#include "blockref_table.h"
//#include "hal-api.h"
#include "hal_logger.h" /* EHSH_LOG_INFO */

/* For the core toolkit */
#include "timer.h"

/**
 * Declares the block reference table containing all functions held within EHS
 */
//extern EhsBlockRefTableType EhsBlockRefTable_core[];

extern const EhsBlockRefType EhsBlockRefTable_core[];

/**
* Declares the core GUI function block toolkit
*/
#ifdef EHS_GUI_SUPPORT
#include "hal_viewport.h"
#include "widget.h"
extern const   EhsBlockRefTable_coreGui[];
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
extern const EhsBlockRefType  EhsBlockRefTable_Peripherals[];
#endif

#ifdef EHS_AV_SUPPORT
extern const EhsBlockRefType  EhsBlockRefTable_Media[];
#endif

/*****************************************************************************/
/* Declare function prototypes */

/**
 * Declares a toolkit that is used to hold deprecated components
 */
#ifdef EHS_TOOLKIT_DEPRECATED
extern const EhsBlockRefType EhsBlockRefTable_deprecated[];
#endif

/**
 * Declares a toolkit that is used to hold experimental components
 */
#ifdef EHS_TOOLKIT_SANDBOX
extern const EhsBlockRefType  EhsBlockRefTable_sandbox[];
#endif

/**
 * Declares a toolkit that is used to hold networking components
 */
#ifdef EHS_COMPONENT_NETWORKING_SUPPORT
extern const EhsBlockRefType  EhsBlockRefTable_networking[];
#endif

/**
 * Declares a toolkit that is used to hold camera components
 */
#ifdef EHS_MV_SUPPORT
extern const EhsBlockRefType  EhsBlockRefTable_mv[];
#endif

/**
 * Declares a toolkit that is used to hold ml components
 */
#ifdef EHS_ML_SUPPORT
extern const EhsBlockRefType EhsBlockRefTable_ml[];
#endif

/**
 * Declares a toolkit for user components
 */
#define EHS_USER_COMPONENT_SUPPORT
#ifdef EHS_USER_COMPONENT_SUPPORT
extern const EhsBlockRefType  EhsBlockRefTable_usercomponents[];
#endif




/**
 * Statically linked libraries here. You may not trust the dll loader for disaster recovery apps
 */
void EhsAddStaticModules()
{
    printf("*** QT DEBUG: EhsAddStaticModules CALLED ***\n");
    fflush(stdout);
    EHSH_LOG_INFO("=== EhsAddStaticModules: ENTRY - Registering component tables ===");
    /* now add the toolkits */
    printf("*** QT DEBUG: Registering core table ***\n");
    fflush(stdout);
    EHSH_LOG_INFO("  Registering EhsBlockRefTable_core");
    EhsToolkitTable_addTable(EhsBlockRefTable_core);
#ifdef EHS_TOOLKIT_DEPRECATED
    printf("*** QT DEBUG: Registering deprecated table ***\n");
    fflush(stdout);
    EHSH_LOG_INFO("  Registering EhsBlockRefTable_deprecated");
    EhsToolkitTable_addTable(EhsBlockRefTable_deprecated);
#endif

#ifdef EHS_TOOLKIT_SANDBOX
    printf("*** QT DEBUG: Registering sandbox table ***\n");
    fflush(stdout);
    EHSH_LOG_INFO("  Registering EhsBlockRefTable_sandbox");
    EhsToolkitTable_addTable(EhsBlockRefTable_sandbox);
#endif

#ifdef EHS_GUI_SUPPORT
    printf("*** QT DEBUG: EHS_GUI_SUPPORT is DEFINED, registering GUI table ***\n");
    fflush(stdout);
    EHSH_LOG_INFO("  Registering EhsBlockRefTable_coreGui (GUI components)");
    EhsToolkitTable_addTable(EhsBlockRefTable_coreGui);
    //@todo the following should really be renamed to Keyboard Support and made a generic component.
    //@todo Also there is no header construct for cases where we do have HW specific components
    //Badidea:extern EhsBlockRefType EhsBlockRefTable_gtk[]; //@todo This should be declared in the same way as the above...
    //Badidea:EhsToolkitTable_addTable(EhsBlockRefTable_gtk);

#else
    printf("*** QT DEBUG: EHS_GUI_SUPPORT is NOT DEFINED ***\n");
    fflush(stdout);
#endif /* EHS_GUI_SUPPORT */

#ifdef EHS_PERIPHERAL_DEVICE_SUPPORT
    EhsToolkitTable_addTable(EhsBlockRefTable_Peripherals);
#endif

// PPP://THE DTV TOOLKIT WAS LOADED IN THE INITIALISATION FUNCTIONS FOR DTV IMPLEMENTATIONS.
#ifdef EHS_AV_SUPPORT
    // content moved the media toolkit EhsToolkitTable_addTable(EhsBlockRefTable_Dtv); //@toodo the Dtv stuff should be refactored into the media directory
    EhsToolkitTable_addTable(EhsBlockRefTable_Media);
#else
//todo we are planning to remove this and the above EHS_AV_SUPPORT should be set to Android by default.
#if defined(EHS_ANDROID)
    EhsToolkitTable_addTable(EhsBlockRefTable_Media);
#endif
#endif
#ifdef EHS_COMPONENT_NETWORKING_SUPPORT
    EhsToolkitTable_addTable(EhsBlockRefTable_networking);
#endif

#ifdef EHS_MV_SUPPORT
    EhsToolkitTable_addTable(EhsBlockRefTable_mv);
   
#endif

#ifdef EHS_ML_SUPPORT
    EhsToolkitTable_addTable(EhsBlockRefTable_ml);
#endif

#ifdef EHS_USER_COMPONENT_SUPPORT
    printf("*** QT DEBUG: Registering user components table ***\n");
    fflush(stdout);
    EHSH_LOG_INFO("  Registering EhsBlockRefTable_usercomponents");
    EhsToolkitTable_addTable(EhsBlockRefTable_usercomponents);
#endif
    printf("*** QT DEBUG: EhsAddStaticModules COMPLETED ***\n");
    fflush(stdout);
    EHSH_LOG_INFO("=== EhsAddStaticModules: EXIT - All component tables registered ===");
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
#endif /* EHS_GUI_SUPPORT */
    return EHS_TRUE;
}
