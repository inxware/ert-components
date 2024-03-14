//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-hex2string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_HexString2String_state
{
} inx_HexString2String_state_type; //Reference this, maybe store your config parameters in here too.
char* datahex(char* string, size_t *len);
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(HexString2String)
EHS_FB_FUNCTION_ENTRY("convert", 0x01, HexString2String_convert)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_HexString2String_ARG_convert_hexStringInput 1
#define INX_HexString2String_ARG_convert_stringOutput 1
#define INX_HexString2String_ARG_convert_length 2
#define INX_HexString2String_ARG_convert_OK 1
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
EHS_FB_IDENTIFY_FUNCTION(HexString2String)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_HexString2String_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(HexString2String)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	//inx_HexString2String_state_type* inx_HexString2String_state = (inx_HexString2String_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	//EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(HexString2String)
{
	// inx_HexString2String_state_type *inx_HexString2String_state = (inx_HexString2String_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
  return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION convert MACRO START -- DO NOT ALTER
/**
 * Definition of HexString2String_convert.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(HexString2String_convert)
{
	// inx_HexString2String_state_type* inx_HexString2String_state = (inx_HexString2String_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	size_t len;
	char* output = NULL;
	if (EHS_FB_IN_CONNECTED_API2(INX_HexString2String_ARG_convert_hexStringInput))
		output = datahex(EHS_FB_IN_S_API2(INX_HexString2String_ARG_convert_hexStringInput), &len) ;
	if (EHS_FB_OUT_CONNECTED_API2(INX_HexString2String_ARG_convert_stringOutput) && output != NULL)
		memcpy(EHS_FB_OUT_S_API2(INX_HexString2String_ARG_convert_stringOutput), output, len) ;
	if (EHS_FB_OUT_CONNECTED_API2(INX_HexString2String_ARG_convert_length))
		EHS_FB_OUT_I_API2(INX_HexString2String_ARG_convert_length) = len;
	if (output != NULL) free(output);
	EHS_FB_FINISH(INX_HexString2String_ARG_convert_OK);
}//ICB FUNCTION convert MACRO END -- DO NOT ALTER THIS LINE
//

/**
 * Convert the Hex data in string to the string with the actual characters
 * CC-BY-SA 4.0 From: https://stackoverflow.com/a/35452093
 * Modifications based on the original:
 *  1. One "0" will be prepended to the hex string if the length is odd.
 *  2. Therefore, a dynamically allocated string is added.
 * */
char* datahex(char* string, size_t *len) {

  if(string == NULL) 
     return NULL;

  size_t slength = strlen(string);
  char* internal_string = NULL;
  if((slength % 2) != 0) // must be even
  {
    internal_string = malloc(slength + 2);
    internal_string[0] = '0';
    memcpy(internal_string + 1, string, slength + 1);
    slength++;
  }
  else {
    internal_string = malloc(slength + 1);
    memcpy(internal_string, string, slength + 1);
  }

  size_t dlength = slength / 2;

  char* data = malloc(dlength);
  memset(data, 0, dlength);

  size_t index = 0;
  while (index < slength) {
      char c = internal_string[index];
      int value = 0;
      if(c >= '0' && c <= '9')
        value = (c - '0');
      else if (c >= 'A' && c <= 'F') 
        value = (10 + (c - 'A'));
      else if (c >= 'a' && c <= 'f')
        value = (10 + (c - 'a'));
      else {
        free(data);
        free(internal_string);
        return NULL;
      }

      data[(index/2)] += value << (((index + 1) % 2) * 4);

      index++;
  }
  *len = dlength;
  free(internal_string);

  return data;
}
