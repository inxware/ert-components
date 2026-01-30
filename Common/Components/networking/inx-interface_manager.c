//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-interface_manager.h"
#include "hal_network.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_interface_manager_state
{
} inx_interface_manager_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(interface_manager)
EHS_FB_FUNCTION_ENTRY("configure", 0x01, interface_manager_configure)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_interface_manager_ARG_configure_wifi_enable 1
#define INX_interface_manager_ARG_configure_eth_enable 2
#define INX_interface_manager_ARG_configure_errorCode 1
#define INX_interface_manager_ARG_configure_configured 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(interface_manager)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_interface_manager_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(interface_manager)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_interface_manager_state_type* inx_interface_manager_state = (inx_interface_manager_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(interface_manager)
{
	inx_interface_manager_state_type *inx_interface_manager_state = (inx_interface_manager_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

// THe following should go in the target/Component-HAL/ files
#if EHS_HAL_INTERFACE_CONFIG_SUPPORT == EHS_HAL_INTERFACE_CONFIG_STUBBED
ehs_sint32 EhsNetworkInterfaceConfigure(const EhsNetworkInterfaceConfigDataType* config)
{
	return 0;
}
#endif

//ICB FUNCTION configure MACRO START -- DO NOT ALTER
/**
 * Definition of interface_manager_configure.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(interface_manager_configure)
{
	inx_interface_manager_state_type* inx_interface_manager_state = (inx_interface_manager_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_sint32 err = -1; // perhaps this should be an error number?
	EhsNetworkInterfaceConfigDataType config = {0};
	config.save = EHS_TRUE;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_interface_manager_ARG_configure_wifi_enable))
		config.b_wifi_enable = EHS_FB_IN_B_API2(INX_interface_manager_ARG_configure_wifi_enable) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_interface_manager_ARG_configure_eth_enable))
		config.b_eth_enable = EHS_FB_IN_B_API2(INX_interface_manager_ARG_configure_eth_enable) ;
	//todo2026 - we need to add this to more target types and remove this conditional
#ifdef EHS_HAL_INTERFACE_CONFIG_SUPPORT
	err = EhsNetworkInterfaceConfigure(&config);
#endif
	if (EHS_FB_OUT_CONNECTED_API2(INX_interface_manager_ARG_configure_errorCode))
		EHS_FB_OUT_I_API2(INX_interface_manager_ARG_configure_errorCode) = err;
	EHS_FB_FINISH(INX_interface_manager_ARG_configure_configured);
}//ICB FUNCTION configure MACRO END -- DO NOT ALTER THIS LINE
