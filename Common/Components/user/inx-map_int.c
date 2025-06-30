//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-map_int.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#define EHS_CAPACITY_OF_INT_MAP 30
#define EHS_NO_INDEX_INT_MAP -1

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_MapInt_state
{
	ehs_sint32 nDefault;
	ehs_sint32 aMap[EHS_CAPACITY_OF_INT_MAP][2];
	ehs_uint32 nSize;
	ehs_bool bIndexed;
} inx_MapInt_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(MapInt)
EHS_FB_FUNCTION_ENTRY("Run_Input", 0x01, MapInt_Run_Input)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_MapInt_ARG_Run_Input_i1 1
#define INX_MapInt_ARG_Run_Input_i2 2
#define INX_MapInt_ARG_Run_Input_i3 3
#define INX_MapInt_ARG_Run_Input_i4 4
#define INX_MapInt_ARG_Run_Input_i5 5
#define INX_MapInt_ARG_Run_Input_i6 6
#define INX_MapInt_ARG_Run_Input_i7 7
#define INX_MapInt_ARG_Run_Input_i8 8
#define INX_MapInt_ARG_Run_Input_o1 1
#define INX_MapInt_ARG_Run_Input_o2 2
#define INX_MapInt_ARG_Run_Input_o3 3
#define INX_MapInt_ARG_Run_Input_o4 4
#define INX_MapInt_ARG_Run_Input_o5 5
#define INX_MapInt_ARG_Run_Input_o6 6
#define INX_MapInt_ARG_Run_Input_o7 7
#define INX_MapInt_ARG_Run_Input_o8 8
#define INX_MapInt_ARG_Run_Input_o 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

void ehs_parse_string_to_map(const ehs_char* str, ehs_sint32 arr[EHS_CAPACITY_OF_INT_MAP][2], ehs_uint32* size) {
	if(str == NULL || size == NULL || arr == NULL){
		return;
	}
	const ehs_char *ptr = str;
    ehs_uint32 index = 0;

	while (*ptr) {
        // Skip any leading whitespace or commas
        while (*ptr == ' ' || *ptr == ',') {
            ptr++;
        }

        int sign = 1;
        if (*ptr == '-') {
            sign = -1;
            ptr++;
        }

        // Read the key
        int key = 0;
        while (isdigit(*ptr)) {
            key = key * 10 + (*ptr - '0');
            ptr++;
        }
        key *= sign;

        // Skip any whitespace or '='
        while (*ptr == ' ' || *ptr == '=') {
            ptr++;
        }

        sign = 1;
        if (*ptr == '-') {
            sign = -1;
            ptr++;
        }

        // Read the value
        int value = 0;
        while (isdigit(*ptr)) {
            value = value * 10 + (*ptr - '0');
            ptr++;
        }
        value *= sign;

        // Store the key-value pair in the array
        arr[index][0] = key;
        arr[index][1] = value;
        index++;
		if(index >= EHS_CAPACITY_OF_INT_MAP){
			EHSH_LOG_ERROR("Integer map reached max capacity (%d)", EHS_CAPACITY_OF_INT_MAP);
			break;
		}
    }

    // Set the number of items parsed
    *size = index;
}

ehs_sint32 ehs_index(const inx_MapInt_state_type* pMap, ehs_sint32 index){
	return (pMap->bIndexed==EHS_TRUE) ? index : EHS_NO_INDEX_INT_MAP;
}

ehs_bool ehs_get_map_value(const inx_MapInt_state_type* pMap, ehs_sint32 key, ehs_sint32* value, ehs_sint32 index)
{
	if(pMap == NULL || value == NULL){
		EHSH_LOG_ERROR("Invalid integer map request.");
		return EHS_FALSE;
	}
	if(index > EHS_NO_INDEX_INT_MAP){
		if(pMap->bIndexed==EHS_TRUE){
			if(index < pMap->nSize && pMap->aMap[index][0] == key){
				*value = pMap->aMap[index][1];
				return EHS_TRUE;
			}else{
				return EHS_FALSE;
			}
		}
	}else{
		for(int i = 0; i < pMap->nSize; i++){
			if(pMap->aMap[i][0] == key){
				*value = pMap->aMap[i][1];
				return EHS_TRUE;
			}
		}
	}
	EHSH_LOG_ERROR("Failed find integer map key (%d).", key);
	return EHS_FALSE;
}


//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(MapInt)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_char* map;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s",&map); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_MapInt_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(MapInt)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_MapInt_state_type* inx_MapInt_state = (inx_MapInt_state_type*)EHS_FB_INIT_CONTEXT;
	EhsMemset(inx_MapInt_state->aMap, 0, sizeof(inx_MapInt_state->aMap));

	/* read the initialisation parameters */
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	pParams = EhsGetSint32FromString(&inx_MapInt_state->nDefault, pParams);
	ehs_parse_string_to_map(pParams, inx_MapInt_state->aMap, &inx_MapInt_state->nSize);

	inx_MapInt_state->bIndexed = EHS_FALSE;
	pParams = EhsGetUint8FromString(&inx_MapInt_state->bIndexed, pParams);

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(MapInt)
{
	//inx_MapInt_state_type *inx_MapInt_state = (inx_MapInt_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION Run_Input MACRO START -- DO NOT ALTER
/**
 * Definition of MapInt_Run_Input.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MapInt_Run_Input)
{
	inx_MapInt_state_type* inx_MapInt_state = (inx_MapInt_state_type*)EHS_FB_RUN_CONTEXT;
	// no point to check agains an empty map
	if(inx_MapInt_state->nSize==0){
		return;
	}
	ehs_sint32 value = 0;
	if (EHS_FB_IN_CONNECTED_API2(INX_MapInt_ARG_Run_Input_i1) && EHS_FB_OUT_CONNECTED_API2(INX_MapInt_ARG_Run_Input_o1)){
		if(ehs_get_map_value(inx_MapInt_state, EHS_FB_IN_I_API2(INX_MapInt_ARG_Run_Input_i1), &value, ehs_index(inx_MapInt_state, 0))==EHS_FALSE){
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o1)=inx_MapInt_state->nDefault;
		}else{
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o1)=value;
		}
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_MapInt_ARG_Run_Input_i2) && EHS_FB_OUT_CONNECTED_API2(INX_MapInt_ARG_Run_Input_o2)){
		if(ehs_get_map_value(inx_MapInt_state, EHS_FB_IN_I_API2(INX_MapInt_ARG_Run_Input_i2), &value, ehs_index(inx_MapInt_state, 1))==EHS_FALSE){
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o2)=inx_MapInt_state->nDefault;
		}else{
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o2)=value;
		}
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_MapInt_ARG_Run_Input_i3) && EHS_FB_OUT_CONNECTED_API2(INX_MapInt_ARG_Run_Input_o3)){
		if(ehs_get_map_value(inx_MapInt_state, EHS_FB_IN_I_API2(INX_MapInt_ARG_Run_Input_i3), &value, ehs_index(inx_MapInt_state, 2))==EHS_FALSE){
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o3)=inx_MapInt_state->nDefault;
		}else{
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o3)=value;
		}
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_MapInt_ARG_Run_Input_i4) && EHS_FB_OUT_CONNECTED_API2(INX_MapInt_ARG_Run_Input_o4)){
		if(ehs_get_map_value(inx_MapInt_state, EHS_FB_IN_I_API2(INX_MapInt_ARG_Run_Input_i4), &value, ehs_index(inx_MapInt_state, 3))==EHS_FALSE){
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o4)=inx_MapInt_state->nDefault;
		}else{
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o4)=value;
		}
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_MapInt_ARG_Run_Input_i5) && EHS_FB_OUT_CONNECTED_API2(INX_MapInt_ARG_Run_Input_o5)){
		if(ehs_get_map_value(inx_MapInt_state, EHS_FB_IN_I_API2(INX_MapInt_ARG_Run_Input_i5), &value, ehs_index(inx_MapInt_state, 4))==EHS_FALSE){
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o5)=inx_MapInt_state->nDefault;
		}else{
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o5)=value;
		}
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_MapInt_ARG_Run_Input_i6) && EHS_FB_OUT_CONNECTED_API2(INX_MapInt_ARG_Run_Input_o6)){
		if(ehs_get_map_value(inx_MapInt_state, EHS_FB_IN_I_API2(INX_MapInt_ARG_Run_Input_i6), &value, ehs_index(inx_MapInt_state, 5))==EHS_FALSE){
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o6)=inx_MapInt_state->nDefault;
		}else{
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o6)=value;
		}
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_MapInt_ARG_Run_Input_i7) && EHS_FB_OUT_CONNECTED_API2(INX_MapInt_ARG_Run_Input_o7)){
		if(ehs_get_map_value(inx_MapInt_state, EHS_FB_IN_I_API2(INX_MapInt_ARG_Run_Input_i7), &value, ehs_index(inx_MapInt_state, 6))==EHS_FALSE){
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o7)=inx_MapInt_state->nDefault;
		}else{
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o7)=value;
		}
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_MapInt_ARG_Run_Input_i8) && EHS_FB_OUT_CONNECTED_API2(INX_MapInt_ARG_Run_Input_o8)){
		if(ehs_get_map_value(inx_MapInt_state, EHS_FB_IN_I_API2(INX_MapInt_ARG_Run_Input_i8), &value, ehs_index(inx_MapInt_state, 7))==EHS_FALSE){
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o8)=inx_MapInt_state->nDefault;
		}else{
			EHS_FB_OUT_I_API2(INX_MapInt_ARG_Run_Input_o8)=value;
		}
	}

	EHS_FB_FINISH(INX_MapInt_ARG_Run_Input_o);
}//ICB FUNCTION Run_Input MACRO END -- DO NOT ALTER THIS LINE
