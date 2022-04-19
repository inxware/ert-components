/*
*  Function prototypes for parseSODL.c
*
*
*/

#ifndef _PARSESODL_H
#define _PARSESODL_H

#include "hal-api.h"
#include "blockref_table.h"


/* identifiers used in SODL...*/
#define NO_PARAMS "NoParams"
#define SIZE_OF_NO_PARAMS sizeof(NO_PARAMS)
#define EHS_KP_KEYWORD_BEGIN "BEGIN "
#define EHS_KP_KEYWORD_END "END"
/* longest string containing a datatype name (including terminating null) */
#define EHS_KP_DATATYPE_LEN 2


#define MAX_OBJ_NAME_LEN EHS_STRING_LENGTH_MAX
// #define MAX_FUNC_NAME_LEN EHS_STRING_LENGTH_MAX /* no longer required */
#define MAX_FILENAME_LEN EHS_STRING_LENGTH_MAX
#define MAX_PARAM_STR_LEN (EHS_STRING_LENGTH_MAX*2)
#define MAX_NUM_FUNCS_PER_OBJECT 20 /* no longer required @todo this is !!!! */


typedef struct
{
	/* struct to hold complete description of SODL text input object...*/
	/* this may be subject to some change at present...*/
	
	/* name and priority */
	EhsBlockRefType * pBlockRef; /* replaces szObjectName[MAX_OBJ_NAME_LEN]; */
	//int  nPriority;
	
	/* internal state data that may be held by the object...*/
	
	unsigned char  nNumFuncs;
	/* for each runnable function...*/
	EhsFuncRefType* pFuncRef[MAX_NUM_FUNCS_PER_OBJECT]; /* replaces char szNameFunc */
	EhsTriggerIdType  nFuncTrigID[MAX_NUM_FUNCS_PER_OBJECT];
	EhsFunctionInstanceIndexType  nFuncUniqueID[MAX_NUM_FUNCS_PER_OBJECT];	
} EhsKPFuncBlockStruct;


char * sgetInt(int * output, char* input);//used by other modules
const char* ReadParmFile(const char* szInputLine, char* szParmsParams);
int FilterFile( char* szInFile, char* szOutFile );
int ReHashSODLFile(EhsKPFuncBlockStruct * SODLtable,int objNumber);//MotIn NEC, int* mapEventIDToFuncID);
#endif

