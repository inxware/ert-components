#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-network_config.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal-api.h"
#include "hal_network.h"

/**  Stub the HAL calls when not using a real implementation. */
 /**********************************************************************/
#if EHS_HAL_NETWORK_CONFIG_SUPPORT == EHS_HAL_NETWORK_CONFIG_STUBBED

/* Returns true when the eRT target network is connected */
ehs_bool EhsNetworkIsConnected()
{
	//printf("EhsNetworkIsConnected - Stubbed network config interface.\n");
	return EHS_TRUE;
}

/* Configures the traget network interface */
ehs_sint32 EhsNetworkConfigure(const EhsNetworkConfigDataType* config)
{
	//printf("EhsNetworkConfigure - Stubbed network config interface.\n");
	return EHS_NETWORK_CONFIG_NO_ERROR_ID;
}
#endif
/**********************************************************************/

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_network_config_state
{
	/* We don't have any state informationfor this! */
} inx_network_config_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(network_config)
EHS_FB_FUNCTION_ENTRY("get_status", 0x01, network_config_get_status)
EHS_FB_FUNCTION_ENTRY("get_config", 0x02, network_config_get_config)
EHS_FB_FUNCTION_ENTRY("set_config", 0x03, network_config_set_config)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_network_config_ARG_get_status_connected 1
#define INX_network_config_ARG_get_status_status_done 1
#define INX_network_config_ARG_get_config_get_config_err 1
#define INX_network_config_ARG_get_config_get_mode 2
#define INX_network_config_ARG_get_config_get_address 3
#define INX_network_config_ARG_get_config_get_gateway 4
#define INX_network_config_ARG_get_config_get_mask 5
#define INX_network_config_ARG_get_config_get_dns 6
#define INX_network_config_ARG_get_config_get_done 1
#define INX_network_config_ARG_set_config_set_mode 1
#define INX_network_config_ARG_set_config_set_address 2
#define INX_network_config_ARG_set_config_set_gateway 3
#define INX_network_config_ARG_set_config_set_mask 4
#define INX_network_config_ARG_set_config_set_dns 5
#define INX_network_config_ARG_set_config_set_err 1
#define INX_network_config_ARG_set_config_set_done 1
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
EHS_FB_IDENTIFY_FUNCTION(network_config)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_network_config_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(network_config)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	//inx_network_config_state_type* inx_network_config_state = (inx_network_config_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */


	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(network_config)
{
	//inx_network_config_state_type *inx_network_config_state = (inx_network_config_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get_status MACRO START -- DO NOT ALTER
/**
 * Definition of network_config_get_status.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(network_config_get_status)
{
	//inx_network_config_state_type* inx_network_config_state = (inx_network_config_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_network_config_ARG_get_status_connected)){
		EHS_FB_OUT_B_API2(INX_network_config_ARG_get_status_connected) = EhsNetworkIsConnected();
	}
	EHS_FB_FINISH(INX_network_config_ARG_get_status_status_done);
}//ICB FUNCTION get_status MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get_config MACRO START -- DO NOT ALTER
/**
 * Definition of network_config_get_config.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(network_config_get_config)
{
	//inx_network_config_state_type* inx_network_config_state = (inx_network_config_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_sint32 nError = EHS_NETWORK_CONFIG_NO_ERROR_ID;

	// TODO - assign error code in case of failure
	EhsHMetaUpdateNetwork();

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_network_config_ARG_get_config_get_mode)){
		EHS_FB_OUT_I_API2(INX_network_config_ARG_get_config_get_mode) = EhsHMetaGetNetworkMode();
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_network_config_ARG_get_config_get_address)){
		EHS_FB_OUT_S_SET_API2(INX_network_config_ARG_get_config_get_address, EhsHMetaGetIPAddr());
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_network_config_ARG_get_config_get_gateway)){
		EHS_FB_OUT_S_SET_API2(INX_network_config_ARG_get_config_get_gateway, EhsHMetaGetGateway());
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_network_config_ARG_get_config_get_mask)){
		EHS_FB_OUT_S_SET_API2(INX_network_config_ARG_get_config_get_mask, EhsHMetaGetMask());
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_network_config_ARG_get_config_get_dns)){
		EHS_FB_OUT_S_SET_API2(INX_network_config_ARG_get_config_get_dns, EhsHMetaGetDNS1());
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_network_config_ARG_get_config_get_config_err)){
		EHS_FB_OUT_I_API2(INX_network_config_ARG_get_config_get_config_err) = nError;
	}
	EHS_FB_FINISH(INX_network_config_ARG_get_config_get_done);
}//ICB FUNCTION get_config MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set_config MACRO START -- DO NOT ALTER
/**
 * Definition of network_config_set_config.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(network_config_set_config)
{
	//inx_network_config_state_type* inx_network_config_state = (inx_network_config_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_sint32 nError = EHS_NETWORK_CONFIG_NO_ERROR_ID;

	EhsNetworkConfigDataType config = { 0 };

	// we want to persist network setting - @TODO make this optional in params or/and port ?
	config.save = EHS_TRUE;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_network_config_ARG_set_config_set_mode)){
		config.mode = EHS_FB_IN_I_API2(INX_network_config_ARG_set_config_set_mode) ;
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_network_config_ARG_set_config_set_address)){
		config.address = EHS_FB_IN_S_API2(INX_network_config_ARG_set_config_set_address) ;
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_network_config_ARG_set_config_set_gateway)){
		config.gateway = EHS_FB_IN_S_API2(INX_network_config_ARG_set_config_set_gateway) ;
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_network_config_ARG_set_config_set_mask)){
		config.mask = EHS_FB_IN_S_API2(INX_network_config_ARG_set_config_set_mask) ;
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_network_config_ARG_set_config_set_dns)){
		config.dns = EHS_FB_IN_S_API2(INX_network_config_ARG_set_config_set_dns);
	}

	nError = EhsNetworkConfigure(&config);

	if (EHS_FB_OUT_CONNECTED_API2(INX_network_config_ARG_set_config_set_err)){
		EHS_FB_OUT_I_API2(INX_network_config_ARG_set_config_set_err) = nError;
	}
	
	EHS_FB_FINISH(INX_network_config_ARG_set_config_set_done);
}//ICB FUNCTION set_config MACRO END -- DO NOT ALTER THIS LINE
