//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-cgi2json.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_cgi2json_state
{
} inx_cgi2json_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(cgi2json)
EHS_FB_FUNCTION_ENTRY("convert", 0x01, cgi2json_convert)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_cgi2json_ARG_convert_cgi 1
#define INX_cgi2json_ARG_convert_cgi_len 2
#define INX_cgi2json_ARG_convert_json 1
#define INX_cgi2json_ARG_convert_json_len 2
#define INX_cgi2json_ARG_convert_done 1
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
EHS_FB_IDENTIFY_FUNCTION(cgi2json)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_cgi2json_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(cgi2json)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(cgi2json)
{
	//inx_cgi2json_state_type *inx_cgi2json_state = (inx_cgi2json_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

ehs_bool cgiToJson(const ehs_char* cgiString, ehs_sint32 cgiLength, ehs_char* jsonString, ehs_sint32* pJsonLength)
{
	if(cgiString == NULL || cgiString[0] == '\0' || cgiLength == 0 || 
	   jsonString == NULL || pJsonLength == NULL){
		return EHS_FALSE;
	}
	// Count the number of '=' and '&' characters
    ehs_sint32 equalsCount = 0;
    ehs_sint32 ampersandCount = 0;
    for (ehs_sint32 i = 0; i < cgiLength; i++) {
        if (cgiString[i] == '=') {
            equalsCount++;
        } else if (cgiString[i] == '&') {
            ampersandCount++;
        }
    }
	// No point to process if there's no key=value pair
	if(equalsCount == 0){
		return EHS_FALSE;
	}

    // Calculate the length of the JSON string
    //ehs_sint32 jsonLength = cgiLength + equalsCount * 2 + ampersandCount * 3 + 3;
    // Plus equalsCount * 2 for adding 2x'"' 
    // Plus ampersandCount * 3 for adding 2x'"' and ':'
    // Plus 3 for '{', '}' and '\0'

	// Construct the JSON string
    ehs_sint32 j = 0;
    jsonString[j++] = '{'; // Start JSON string with '{'
    jsonString[j++] = '"'; // Start JSON string with '{'
    for (ehs_sint32 i = 0; i < cgiLength; i++) {
        if (cgiString[i] == '=') {
            jsonString[j++] = '"';
            jsonString[j++] = ':';
            jsonString[j++] = '"';
        } else if (cgiString[i] == '&') {
			if(i + 1 == cgiLength){
				// ignore the last ampersand if present
                break;
            }
            jsonString[j++] = '"';
            jsonString[j++] = ',';
            jsonString[j++] = '"';
        } else {
            jsonString[j++] = cgiString[i];
        }
    }
    jsonString[j++] = '"'; // Close the last array
    jsonString[j++] = '}'; // End JSON string with '}'
    jsonString[j] = '\0';  // Null-terminate the JSON string

	*pJsonLength = EhsStrlen(jsonString);

	return EHS_TRUE;
}

//ICB FUNCTION convert MACRO START -- DO NOT ALTER
/**
 * Definition of cgi2json_convert.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(cgi2json_convert)
{
	//inx_cgi2json_state_type* inx_cgi2json_state = (inx_cgi2json_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (!EHS_FB_IN_CONNECTED_API2(INX_cgi2json_ARG_convert_cgi) || 
	    !EHS_FB_OUT_CONNECTED_API2(INX_cgi2json_ARG_convert_json)){
		// no point to do any conversion if these are not connected
		return;
	}
	const ehs_char* cgi_str = EHS_FB_IN_S_API2(INX_cgi2json_ARG_convert_cgi);
	ehs_sint32 cgi_str_len = 0;
	if (EHS_FB_IN_CONNECTED_API2(INX_cgi2json_ARG_convert_cgi_len)){
		cgi_str_len = EHS_FB_IN_I_API2(INX_cgi2json_ARG_convert_cgi_len) ;
	}else{
		cgi_str_len = EhsStrlen(cgi_str);
	}

	ehs_sint32 json_str_len = 0;
	ehs_char* json_str = EHS_FB_OUT_S_API2(INX_cgi2json_ARG_convert_json);

	if(cgiToJson(cgi_str, cgi_str_len, json_str, &json_str_len) == EHS_TRUE){
		//printf("%s \n",json_str);
		if (EHS_FB_OUT_CONNECTED_API2(INX_cgi2json_ARG_convert_json_len)){
			EHS_FB_OUT_I_API2(INX_cgi2json_ARG_convert_json_len) = json_str_len;
		}	

		EHS_FB_FINISH(INX_cgi2json_ARG_convert_done);
	}
}//ICB FUNCTION convert MACRO END -- DO NOT ALTER THIS LINE
