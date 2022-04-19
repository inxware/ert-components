//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-inx-mqtt_client.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct
{
	ehs_char* client cert;
	ehs_char* client key;
	ehs_char* server cert;
} inx_mqtt_client_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(mqtt_client)
EHS_FB_FUNCTION_ENTRY("connect", mqtt_client_connect)
EHS_FB_FUNCTION_ENTRY("disconnect", mqtt_client_disconnect)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_mqtt_client_ARG_connect_host 1
#define INX_mqtt_client_ARG_connect_port 2
#define INX_mqtt_client_ARG_connect_tls 3
#define INX_mqtt_client_ARG_connect_username 4
#define INX_mqtt_client_ARG_connect_password 5
#define INX_mqtt_client_ARG_connect_finishconnect 1
#define INX_mqtt_client_ARG_connect_finishdisconnect 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_mqtt_client_client cert client.pem
#define INX_FB_mqtt_client_client key client.key
#define INX_FB_mqtt_client_server cert server.crt
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(mqtt_client)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_char* client cert;
	ehs_char* client key;
	ehs_char* server cert;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %s %s",&client cert,&client key,&server cert); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_mqtt_client_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(mqtt_client)
{
	ehs_char* client cert;
	ehs_char* client key;
	ehs_char* server cert;
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_mqtt_client_state_type* inx_mqtt_client_state = (inx_mqtt_client_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%s %s %s",&client cert,&client key,&server cert);

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(mqtt_client)
{
	inx_mqtt_client_state_type *inx_mqtt_client_state = (inx_mqtt_client_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION connect MACRO START -- DO NOT ALTER
/**
 * Definition of mqtt_client_connect.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(mqtt_client_connect)
{
	inx_mqtt_client_state_type* inx_mqtt_client_state = (inx_mqtt_client_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_host))
		EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_host) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_port))
		EHS_FB_IN_I_API2(INX_mqtt_client_ARG_connect_port) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_tls))
		EHS_FB_IN_B_API2(INX_mqtt_client_ARG_connect_tls) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_username))
		EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_username) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_password))
		EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_password) ;
	EHS_FB_FINISH(INX_mqtt_client_ARG_connect_finishconnect);
	EHS_FB_FINISH(INX_mqtt_client_ARG_connect_finishdisconnect);
}//ICB FUNCTION connect MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION disconnect MACRO START -- DO NOT ALTER
/**
 * Definition of mqtt_client_disconnect.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(mqtt_client_disconnect)
{
	inx_mqtt_client_state_type* inx_mqtt_client_state = (inx_mqtt_client_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
}//ICB FUNCTION disconnect MACRO END -- DO NOT ALTER THIS LINE
