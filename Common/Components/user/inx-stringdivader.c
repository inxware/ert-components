//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-stringdivader.h"
//ICB HEADER MACRO END -- DO NOT ALTER
#define EHS_STR_DIV_DELIM_MAX_SIZE 32
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_StringDivader_state
{
	ehs_char delim1[EHS_STR_DIV_DELIM_MAX_SIZE];
	ehs_char delim2[EHS_STR_DIV_DELIM_MAX_SIZE];
	ehs_char delim3[EHS_STR_DIV_DELIM_MAX_SIZE];
} inx_StringDivader_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(StringDivader)
EHS_FB_FUNCTION_ENTRY("split", 0x01, StringDivader_split)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_StringDivader_ARG_split_str 1
#define INX_StringDivader_ARG_split_len 2
#define INX_StringDivader_ARG_split_str1 1
#define INX_StringDivader_ARG_split_str2 2
#define INX_StringDivader_ARG_split_str3 3
#define INX_StringDivader_ARG_split_str4 4
#define INX_StringDivader_ARG_split_len1 5
#define INX_StringDivader_ARG_split_len2 6
#define INX_StringDivader_ARG_split_len3 7
#define INX_StringDivader_ARG_split_len4 8
#define INX_StringDivader_ARG_split_done 1
#define INX_StringDivader_ARG_split_err 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(StringDivader)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_char* delim1;
	ehs_char* delim2;
	ehs_char* delim3;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %s %s",&delim1,&delim2,&delim3); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_StringDivader_state_type);
}

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(StringDivader)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_StringDivader_state_type* inx_StringDivader_state = (inx_StringDivader_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	inx_StringDivader_state->delim1[0] = '\0';
	inx_StringDivader_state->delim2[0] = '\0';
	inx_StringDivader_state->delim3[0] = '\0';
	const ehs_char* pParams = EHS_FB_INIT_PARAMETERS;
	pParams = EhsGetWordFromString(inx_StringDivader_state->delim1, pParams);
	if(EhsStrcmp(inx_StringDivader_state->delim1, "NULL") == 0){
        EhsMemset(inx_StringDivader_state->delim1, '\0', EHS_STR_DIV_DELIM_MAX_SIZE);
    }
	pParams = EhsGetWordFromString(inx_StringDivader_state->delim2, pParams);
	if(EhsStrcmp(inx_StringDivader_state->delim2, "NULL") == 0){
        EhsMemset(inx_StringDivader_state->delim2, '\0', EHS_STR_DIV_DELIM_MAX_SIZE);
    }
	pParams = EhsGetWordFromString(inx_StringDivader_state->delim3, pParams);
	if(EhsStrcmp(inx_StringDivader_state->delim3, "NULL") == 0){
        EhsMemset(inx_StringDivader_state->delim3, '\0', EHS_STR_DIV_DELIM_MAX_SIZE);
    }

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(StringDivader)
{
	//inx_StringDivader_state_type *inx_StringDivader_state = (inx_StringDivader_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION split MACRO START -- DO NOT ALTER
/**
 * Definition of StringDivader_split.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(StringDivader_split)
{
	inx_StringDivader_state_type* inx_StringDivader_state = (inx_StringDivader_state_type*)EHS_FB_RUN_CONTEXT;

	const ehs_char* str = NULL;
	ehs_sint32 len = 0;
	if (EHS_FB_IN_CONNECTED_API2(INX_StringDivader_ARG_split_str) && 
		EHS_FB_IN_CONNECTED_API2(INX_StringDivader_ARG_split_len)){
		str = EHS_FB_IN_S_API2(INX_StringDivader_ARG_split_str) ;
		len = EHS_FB_IN_I_API2(INX_StringDivader_ARG_split_len) ;
	}else{
		return;
	}
	ehs_char* str1 = NULL; ehs_char* str2 = NULL; ehs_char* str3 = NULL; ehs_char* str4 = NULL;
	if (EHS_FB_OUT_CONNECTED_API2(INX_StringDivader_ARG_split_str1)){
		str1 = EHS_FB_OUT_S_API2(INX_StringDivader_ARG_split_str1) ;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_StringDivader_ARG_split_str2)){
		str2 = EHS_FB_OUT_S_API2(INX_StringDivader_ARG_split_str2) ;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_StringDivader_ARG_split_str3)){
		str3 = EHS_FB_OUT_S_API2(INX_StringDivader_ARG_split_str3) ;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_StringDivader_ARG_split_str4)){
		str4 = EHS_FB_OUT_S_API2(INX_StringDivader_ARG_split_str4) ;
	}

	ehs_char *start = (char *)str;
    ehs_char *end;
	ehs_sint32 len_check = 0;
	// Find the first delimiter
    if (inx_StringDivader_state->delim1[0] != '\0') {
		if((end = EhsStrstr(start, inx_StringDivader_state->delim1)) != NULL){
			if(str1){
				ehs_sint32 str1_len = (ehs_sint32)(end - start);
				if(str1_len < 0 || str1_len > EHS_STRING_LENGTH_MAX){
					printf("Bad str1 len.\n");
					EHS_FB_FINISH(INX_StringDivader_ARG_split_err);
					return;
				}
				EhsSnprintf(str1, EHS_STRING_LENGTH_MAX, "%.*s", str1_len, start);
				if (EHS_FB_OUT_CONNECTED_API2(INX_StringDivader_ARG_split_len1)){
					EHS_FB_OUT_I_API2(INX_StringDivader_ARG_split_len1) = str1_len;
				}
				printf("Chunk 1 (%d): %s\n", str1_len, str1);
				len_check += str1_len;
				start = end;
				if (inx_StringDivader_state->delim2[0] != '\0') {
					// Find the second delimiter
					if ((end = EhsStrstr(start, inx_StringDivader_state->delim2)) != NULL) {
						if(str2){
							ehs_sint32 str2_len = (ehs_sint32)(end - start);
							if(str2_len < 0 || str2_len > EHS_STRING_LENGTH_MAX){
								printf("Bad str2 len.\n");
								EHS_FB_FINISH(INX_StringDivader_ARG_split_err);
								return;
							}
							EhsSnprintf(str2, EHS_STRING_LENGTH_MAX, "%.*s", str2_len, start);
							if (EHS_FB_OUT_CONNECTED_API2(INX_StringDivader_ARG_split_len2)){
								EHS_FB_OUT_I_API2(INX_StringDivader_ARG_split_len2) = str2_len;
							}
							printf("Chunk 2 (%d): %s\n", str2_len, str2);
							len_check += str2_len;
							start = end;
							if (inx_StringDivader_state->delim3[0] != '\0'){
								// Find the third delimiter
								if ((end = EhsStrstr(start, inx_StringDivader_state->delim3)) != NULL) {
									if(str3){
										ehs_sint32 str3_len = (ehs_sint32)(end - start);
										if(str3_len < 0 || str3_len > EHS_STRING_LENGTH_MAX){
											printf("Bad str3 len.\n");
											EHS_FB_FINISH(INX_StringDivader_ARG_split_err);
											return;
										}
										EhsSnprintf(str3, EHS_STRING_LENGTH_MAX, "%.*s", str3_len, start);
										if (EHS_FB_OUT_CONNECTED_API2(INX_StringDivader_ARG_split_len3)){
											EHS_FB_OUT_I_API2(INX_StringDivader_ARG_split_len3) = str3_len;
										}
										printf("Chunk 3 (%d): %s\n", str3_len, str3);
										len_check += str3_len;
										start = end;
										if(str4){
											// Remaining substring
											ehs_sint32 str4_len = len - len_check;
											if(str4_len < 0 || str4_len > EHS_STRING_LENGTH_MAX){
												printf("Bad str4 len.\n");
												EHS_FB_FINISH(INX_StringDivader_ARG_split_err);
												return;
											}
											EhsSnprintf(str4, EHS_STRING_LENGTH_MAX, "%.*s", str4_len, start);
											if (EHS_FB_OUT_CONNECTED_API2(INX_StringDivader_ARG_split_len4)){
												EHS_FB_OUT_I_API2(INX_StringDivader_ARG_split_len4) = str4_len;
											}
											printf("Chunk 4 (%d): %s\n", str4_len, str4);
										}
									}
								}else{
									EHS_FB_FINISH(INX_StringDivader_ARG_split_err);
									return;
								}
							}else{
								if(str3){
									// Remaining substring
									ehs_sint32 str3_len = len - len_check;
									if(str3_len < 0 || str3_len > EHS_STRING_LENGTH_MAX){
										printf("Bad str3 len.\n");
										EHS_FB_FINISH(INX_StringDivader_ARG_split_err);
										return;
									}
									EhsSnprintf(str3, EHS_STRING_LENGTH_MAX, "%.*s", str3_len, start);
									if (EHS_FB_OUT_CONNECTED_API2(INX_StringDivader_ARG_split_len3)){
										EHS_FB_OUT_I_API2(INX_StringDivader_ARG_split_len3) = str3_len;
									}
									printf("Chunk 3 (%d): %s\n", str3_len, str3);
								}
							}
						}
					}else{
						EHS_FB_FINISH(INX_StringDivader_ARG_split_err);
						return;
					}
				}else{
					if(str2){
						// Remaining substring
						ehs_sint32 str2_len = len - len_check;
						if(str2_len < 0 || str2_len > EHS_STRING_LENGTH_MAX){
							printf("Bad str2 len.\n");
							EHS_FB_FINISH(INX_StringDivader_ARG_split_err);
							return;
						}
						EhsSnprintf(str2, EHS_STRING_LENGTH_MAX, "%.*s", str2_len, start);
						if (EHS_FB_OUT_CONNECTED_API2(INX_StringDivader_ARG_split_len2)){
							EHS_FB_OUT_I_API2(INX_StringDivader_ARG_split_len2) = str2_len;
						}
						printf("Chunk 2 (%d): %s\n", str2_len, str2);
					}
				}
			}
		}else{
			EHS_FB_FINISH(INX_StringDivader_ARG_split_err);
			return;
		}
    }

	EHS_FB_FINISH(INX_StringDivader_ARG_split_done);
}//ICB FUNCTION split MACRO END -- DO NOT ALTER THIS LINE
