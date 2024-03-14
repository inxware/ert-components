/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file ehs_fb_types.h
 * In this file, all of the type definitions required by EHS function blocks are given.
 * Any target-specific type definitions are given in target_types.h, which comes from
 * the target folder.
 *
 * @author: inx limited
 *
 */

/* @todo refactor needs renaming to fb_api.h
 * @todo refactor incliude the function instance data from here (rather than visa-versa)
 * @todo refactor remove the Depricated API code
 */

#ifndef EHS_FB_TYPES_H
#define EHS_FB_TYPES_H

#include "ehs_types.h"

//#include "hal_process.h" //needed for reference to EhsTPThread_execute() for Function blocks

#define EHS_FUNCTION_SIZE 32 /**< Maximum length of a function name */
/* This is duplicated from globals.h */
/*********************************************************************************************/
/* Function block definitions */

/**
 * Defines a pointer to a standard identify function (which appears within a function block).
 *
 * @param[in] params Points to the text containing the parameters block
 * @param[out] bytesRequired Used by the identify function to determine how many bytes the context area requires.
 * @param[out] priority I think this is no longer used
 */
typedef void (*EhsIdentifyFuncType)(const ehs_char* params, ehs_uint32* bytesRequired, ehs_uint16* priority);

struct EhsFunctionInstanceDataStruct;

/**
 * Defines a pointer to a standard initialise function appearing within a function block.
 *
 * @param[in] params Points to the text containing the parameters block
 * @param[in] context Points to an area of memory used to store the context for the current instance of the function block
 * @param[in] pCallbackTable Points to the first callback function for this function block
 * @return true if successful
 */
typedef ehs_bool (*EhsInitFuncType)(const ehs_char* params, void* context, struct EhsFunctionInstanceDataStruct* pCallbackTable);

/**
 * Defines a pointer to a standard initialise function appearing within a function block.
 *
 * @param[in] params Points to the text containing the parameters block
 * @param[in] context Points to an area of memory used to store the context for the current instance of the function block
 * @return true if successful
 */
typedef ehs_bool (*EhsDestroyFuncType)(void* context);

/**
 * Defines a pointer to a standard generic function appearing within a function block.
 *
 * @param[in,out] context Points to an area of memory used to store the context for the current instance of the function block.
 * It's a pointer to a pointer to allow the context area to be reallocated if necessary.
 */
typedef void (*EhsRunFuncType)(struct EhsFunctionInstanceDataStruct* context);

/**
 * Defines a pointer to a generic function to be used within a separate thread
 *
 * @param[in,out] context Points to an area of memory used to store the context for the current instance of the function block.
 * It's a point to a point to allow the context area to be reallocated if necessary.
 * @return The return type is target-specific. Where a return value is required, the EHS_THREAD_END macro will supply
 * an appropriate return value.
 */

typedef EhsThreadFuncReturnType (*EhsThreadFuncType)(struct EhsFunctionInstanceDataStruct* context);

/**
 * Macro for defining a function block identity function. Use of macros ensures ease of ability to change parameter lists for all
 * function blocks should the need arise
 */
#define EHS_FB_IDENTIFY_FUNCTION(x) void EHS_FB_IDENTIFY_NAME(x) (const ehs_char* params, ehs_uint32* bytesRequired, ehs_uint16* priority)
#define EHS_FB_IDENTIFY_FUNCTION_API2(x,vers) void EHS_FB_IDENTIFY_NAME_API2(x,vers) (const ehs_char* params, ehs_uint32* bytesRequired, ehs_uint16* priority)
#define EHS_FB_IDENTIFY_PARAMETERS (params) /**< Provides access to the parameters during identification */
#define EHS_FB_IDENTIFY_MEMORY (*bytesRequired) /**< Provides access to the memory requirements during identification */
/*lint -e961 ## Required to provide API to allow 3rd parties to create function blocks without clashing in namespace */
#define EHS_FB_IDENTIFY_NAME(fb) (EhsIdentify ## fb) /**< Maps the function block name onto a identify function name */
#define EHS_FB_IDENTIFY_NAME_API2(fb,vers) (EhsIdentify ## fb ## vers) /**< Maps the function block name onto a identify function name */
/*lint +e961 */

/**
 * Macro for defining a function block initialise function. Use of macros ensures ease of ability to change parameter lists for all
 * function blocks should the need arise
 *
 */
/*lint -e961 suppress complaint about ##. See definition of EHS_FB_IDENTIFY_NAME */
#define EHS_FB_INIT_NAME(fb) (EhsInit ## fb) /**< Maps the function block name onto an init function name */
#define EHS_FB_INIT_NAME_API2(fb,vers) (EhsInit ## fb ## vers) /**< Maps the function block name onto an init function name */
//#define EHS_FB_MYTEST_NAME_API2(fb,vers) (fb ## vers) /**< Maps the function block name onto an init function name */
//#define EHS_FB_MYTEST_NAME_API2(fb,vers) (#fb #vers) /**< Maps the function block name onto an init function name */
#define EHS_FB_MAKE_NAME_STRING_API2(fb,vers) (EHS_FB_VAR_TO_STRING(fb) EHS_FB_VAR_TO_STRING(vers)) /** converts two values into strings, strings are then automatically concatenated together by compiler */
#define EHS_FB_VAR_TO_STRING(x) #x /** use of # to convert a value to a string */
/*lint +e961 */
#define EHS_FB_INIT_FUNCTION(x) ehs_bool EHS_FB_INIT_NAME(x) (const ehs_char* params, EhsFunctionInstanceDataType* pFIdata, EhsFunctionInstanceDataType* pCallbackTable)
#define EHS_FB_INIT_FUNCTION_API2(x,vers) ehs_bool EHS_FB_INIT_NAME_API2(x,vers) (const ehs_char* params, EhsFunctionInstanceDataType* pFIdata, EhsFunctionInstanceDataType* pCallbackTable)
#define EHS_FB_INIT_CONTEXT (pFIdata->pObjData) /**< Provides access to the context during initialization */
#define EHS_FB_INIT_PARAMETERS (params) /**< Provides access to the parameters during initialization */

/**
 * Macro for defining a function block destructor function. Use of macros ensures ease of ability to change parameter lists for all
 * function blocks should the need arise
 *
 */
#define EHS_FB_DESTROY_NAME(fb) (EhsDestroy ## fb) /**< Maps the function block name onto an init function name */
#define EHS_FB_DESTROY_NAME_API2(fb,vers) (EhsDestroy ## fb ## vers) /**< Maps the function block name onto an init function name */
#define EHS_FB_DESTROY_FUNCTION(x) ehs_bool EHS_FB_DESTROY_NAME(x) (EhsFunctionInstanceDataType* pFIdata)
#define EHS_FB_DESTROY_FUNCTION_API2(x,vers) ehs_bool EHS_FB_DESTROY_NAME_API2(x,vers) (EhsFunctionInstanceDataType* pFIdata)
#define EHS_FB_DESTROY_CONTEXT (pFIdata->pObjData) /**< Provides access to the context during initialization */

/* @todo the following macros should be dumped and the dynamic method used instead. */
#define EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(x) (&(pCallbackTable[-1-(x)])) /**< Provides access to the function instance data for callback functions */

/*lint -e961 suppress complaint about ##. See definition of EHS_FB_IDENTIFY_NAME */
#define EHS_FB_RUN_NAME(fb) (EhsRun ## fb) /**< Maps the function block name onto an run function name */
#define EHS_FB_RUN_NAME_API2(fb,vers) (EhsRun ## fb ## vers) /**< Maps the function block name onto an run function name */
/*lint +e961 */

/*lint -e961 suppress complaint about ##. See definition of EHS_FB_IDENTIFY_NAME */
#define EHS_FB_THREAD_NAME(fb) (EhsFbThread## fb) /**< Maps the thread name onto a function name */
/*line +e961 */

/**
 * Macro for defining a function block general-purpose run function. Use of macros ensures ease of ability to change parameter lists for all
 * function blocks should the need arise
 */
/* Called from EHS */
#define EHS_FB_RUN_FUNCTION(x) void EHS_FB_RUN_NAME(x) (EhsFunctionInstanceDataType* pFIdata)
#define EHS_FB_RUN_FUNCTION_API2(x,vers) void EHS_FB_RUN_NAME_API2(x,vers) (EhsFunctionInstanceDataType* pFIdata)
/*Called from component space*/
//#define EHS_FB_RUN_FUNCTION_RET_ARGS(x,...) EHS_FB_RUN_NAME(x) (EhsFunctionInstanceDataType* pFIdata,##__VA_ARGS__)  //@todo make one macro for ARGS version
#define EHS_FB_RUN_FUNCTION_ARGS(x,...) void EHS_FB_RUN_NAME(x) (EhsFunctionInstanceDataType* pFIdata,##__VA_ARGS__) //@todo the ##__VAR_ARGS__ macros is not C99 compliant


#define EHS_FB_THREAD_FUNCTION(x) EhsThreadFuncReturnType EHS_FB_THREAD_NAME(x) (EhsFunctionInstanceDataType* pFIdata)
#define EHS_FB_THREAD_FUNCTION_ARGS(x,...) EhsThreadFuncReturnType EHS_FB_THREAD_NAME(x) (EhsFunctionInstanceDataType* pFIdata,__VA_ARGS__)
//#define EHS_FB_RUN_FUNCTION_EX(x) void EHS_FB_RUN_NAME(x) (EhsFunctionInstanceDataType* pFIdata)
#define EHS_FB_RUN_CONTEXT (pFIdata->pObjData) //PUBLIC /**< Provides access to the context within the run function, including the ability to assign new context blocks in its place. */
#define EHS_FB_RUN_CONTEXT_REF (pFIdata) // @todo This should be called something else - it is used instead of EHS_FB_RUN_FUNCTION_INSTANCE - it only happens to be the same address as the object data pointer
#define EHS_FB_RUN_FUNCTION_INSTANCE (pFIdata) //PUBLIC
/* Executing threaded functions conforming to EHS_FB_RUN contexts (i.e. to access object data and ports for the calling RUN context)*/
#define EHS_FB_START_THREAD(x,pri) EhsTPThread_execute(EHS_FB_THREAD_NAME(x), (EhsFunctionInstanceDataType*) EHS_FB_RUN_CONTEXT_REF,pri,EHS_THREAD_USE_DEFAULT_STACK_SIZE)
#define EHS_FB_START_THREAD_ARGS(x,pri,...) EhsTPThread_execute(EHS_FB_THREAD_NAME(x), (EhsFunctionInstanceDataType*) EHS_FB_RUN_CONTEXT_REF,pri,EHS_THREAD_USE_DEFAULT_STACK_SIZE,__VA_ARGS__)
/* Executing non threaded functions conforming to EHS_FB_RUN contexts (i.e. to access object data and ports for the calling RUN context)*/
#define EHS_FB_START_RUN_FUNCTION(x) EHS_FB_RUN_NAME(x) ((EhsFunctionInstanceDataType*) EHS_FB_RUN_CONTEXT_REF)
#define EHS_FB_START_RUN_FUNCTION_ARGS(x,...) EHS_FB_RUN_NAME(x) ((EhsFunctionInstanceDataType*) EHS_FB_RUN_CONTEXT_REF,__VA_ARGS__)

/* Required functions that must be called at the start and finish of thread functions
 * This is to ensure threads are completed before an EHS application environment is torn down
 */
void Ehs_FB_ThreadStarted();
void Ehs_FB_ThreadComplete();


/* non-public stuff */

typedef struct
{
#ifndef EHRT1
    const ehs_char szName[EHS_FUNCTION_SIZE]; /**< Function name */
#else
    const ehs_uint8 szName; /**< Function name */
#endif
    EhsRunFuncType fpRunFunc; /**< Pointer to the function corresponding to the name */
} EhsFuncRefType;

/**
 * Component Object & Function registration - used in component_XXXX.c
 */
/*lint -e961 suppress complaint about ##. See definition of EHS_FB_IDENTIFY_NAME */
#define EHS_FB_FUNCTABLE_NAME(fb) (EhsFuncTable ## fb) /**< Creates the name for the table of functions */
/*lint +e961 */
#define EHS_FB_FUNCTIONS_START(fb) EhsFuncRefType EHS_FB_FUNCTABLE_NAME(fb)[] = {
#ifdef EHRT1
#define EHS_FB_FUNCTION_ENTRY(n,i,f) {i,EHS_FB_RUN_NAME(f)},
#define EHS_FB_FUNCTIONS_END {0} };
#else
#define EHS_FB_FUNCTION_ENTRY(n,i,f) {n,EHS_FB_RUN_NAME(f)},
#define EHS_FB_FUNCTIONS_END {{0}} };
#endif

#define EHS_FB_FUNCTIONS(fb) EHS_GLOBAL EhsFuncRefType EHS_FB_FUNCTABLE_NAME(fb)[];


/* API2 macros using versioning */
/*lint -e961 suppress complaint about ##. See definition of EHS_FB_IDENTIFY_NAME */
#define EHS_FB_FUNCTABLE_NAME_API2(fb,vers) (EhsFuncTable ## fb ## vers) /**< Creates the name for the table of functions */
/*lint +e961 */
#define EHS_FB_FUNCTIONS_START_API2(fb,vers) EhsFuncRefType EHS_FB_FUNCTABLE_NAME_API2(fb,vers)[] = {
#ifdef EHRT1
#define EHS_FB_FUNCTION_ENTRY_API2(n,i,f,vers) {i,EHS_FB_RUN_NAME_API2(f,vers)},
#else
#define EHS_FB_FUNCTION_ENTRY_API2(n,i,f,vers) {n,EHS_FB_RUN_NAME_API2(f,vers)},
#endif
#define EHS_FB_FUNCTIONS_END_API2 {0}};

#define EHS_FB_FUNCTIONS_API2(fb,vers) EHS_GLOBAL EhsFuncRefType EHS_FB_FUNCTABLE_NAME_API2(fb,vers)[];

/*********************************************************************************************/
typedef ehs_sint16 EhsFunctionInstanceIndexType; /**< Provides the index of a specific start event. Indexes into EhsFunctionInstanceDataTable */
#define EHS_START_INDEX_INVALID ((EhsTriggerIdType)(-1))
typedef ehs_sint16 EhsTriggerIdType; /**< Used to identify a specific trigger id */
typedef unsigned char debug_type_byte;
//typedef ehs_uint32 debug_type_byte;

/**
 * This type holds the array of start ports that are activated when a single finish port is notified.
 * Each instance of this type corresponds to a specific trigger id.
 */
typedef struct
{
    EhsTriggerIdType nTriggerId; /**< Identifies the Id of this trigger */
    ehs_uint16 nStartPorts; /**< Number of start ports connected to the finish port */
    debug_type_byte tMonitorType; /* bitmask for monitor type for this function - see DEBUG_SET_MASK_ */
    EhsFunctionInstanceIndexType *piComp; /**< Index of each start port */
} EhsTriggerType;


/**
 *
 */

typedef ehs_sint32 EhsDataflowIntType;
#define EHS_DATAFLOW_INT_MAX EHS_SINT32_MAX

typedef ehs_bool EhsDataflowBoolType;
typedef ehs_char* EhsDataflowStringType;
typedef double EhsDataflowFloatType;
typedef void* EhsDataflowUserType;

/*********************************************************************************************/
/* Connection macros used by components */

/**
 * Get input x - generic version - needs to be casted before use
 */
#define EHS_FB_IN(x) EHS_FB_RUN_FUNCTION_INSTANCE->pIn[x]

/**
 * Check whether input x is connected or not
 */
#define EHS_FB_IN_CONNECTED(x) ((EHS_FB_RUN_FUNCTION_INSTANCE->pIn) && (EHS_FB_IN(x) != EhsDataConnectionTable.xDummyIn))

/**
 * Get input x as an integer
 */
#define EHS_FB_IN_I(x) *(EhsDataflowIntType*)EHS_FB_IN(x)

/**
 * Get input x as a float
 */
#define EHS_FB_IN_F(x) *(EhsDataflowFloatType*)EHS_FB_IN(x)

/**
 * Get input x as a bool
 */
#define EHS_FB_IN_B(x) *(ehs_bool*)EHS_FB_IN(x)

/**
 * Get input x as a string
 */
#define EHS_FB_IN_S(x) (char*)EHS_FB_IN(x)

/**
 * Get input x as a user data
 */
#define EHS_FB_IN_U(x) *(EhsDataflowUserType*)EHS_FB_IN(x)

/**
 * Check whether output x is connected or not
 */
#define EHS_FB_OUT_CONNECTED(x) ((EHS_FB_RUN_FUNCTION_INSTANCE->pOut) && (EHS_FB_OUT(x) != EhsDataConnectionTable.xDummy))

/**
 * Get output x - generic version
 */
#define EHS_FB_OUT(x) (EHS_FB_RUN_FUNCTION_INSTANCE->pOut[x])

/**
 * Get output x as a bool
 */
#define EHS_FB_OUT_B(x) *(ehs_bool*)EHS_FB_OUT(x)

/**
 *  Get output x as an integer
 */
#define EHS_FB_OUT_I(x) *(EhsDataflowIntType*)EHS_FB_OUT(x)

/**
 * Get output x as a float
 */
#define EHS_FB_OUT_F(x) *(EhsDataflowFloatType*)EHS_FB_OUT(x)

/**
 * Get output x as a string
 */
#define EHS_FB_OUT_S(x) (char*)EHS_FB_OUT(x)

/**
 * Get output x as a user
 */
#define EHS_FB_OUT_U(x) *(EhsDataflowUserType*)EHS_FB_OUT(x)

/**
 * Assert event x
 */
#define EHS_FB_FINISH(x) EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,(x))

/*********************************************************************************************/
/******************************************************/
/* MACROS  	used in components                    */
/******************************************************/

/*The following should NOT be in the proper 'API header' for use by function c modules */
/* useful (side effect free!) #define macros for the legacy API...*/
/* useful starting #defines to work with...*/

#define NCAPSA_pMain (structFuncArg*)(&EHS_FB_RUN_CONTEXT)

/* quantities first...*/
// Deprecated todo remove : #define NCAPSA_nNumIns EHS_FB_RUN_FUNCTION_INSTANCE->nNumInputs
// Deprecated todo remove : #define NCAPSA_nNumOuts EHS_FB_RUN_FUNCTION_INSTANCE->nNumOutputs

#define NCAPSA_nNumComps(x) EHS_FB_RUN_FUNCTION_INSTANCE->nNumCompletes[x]

/* todo the following NCAPSA macros should be removed*/
/* useful debugging location #defines...*/
#define NCAPSA_pIn(x) EHS_FB_IN(x)
#define NCAPSA_pOut(x) EHS_FB_RUN_FUNCTION_INSTANCE->pOut[x]
/* todo remove these: Legacy API Only uses the hardwired numbers */
#define NCAPSA_pComp1(x) EHS_FB_RUN_FUNCTION_INSTANCE->pComp[0][x]
#define NCAPSA_pComp2(x) EHS_FB_RUN_FUNCTION_INSTANCE->pComp[1][x]
#define NCAPSA_pComp3(x) EHS_FB_RUN_FUNCTION_INSTANCE->pComp[2][x]
#define NCAPSA_pComp4(x) EHS_FB_RUN_FUNCTION_INSTANCE->pComp[3][x]
#define NCAPSA_pComp5(x) EHS_FB_RUN_FUNCTION_INSTANCE->pComp[4][x]
#define NCAPSA_pComp6(x) EHS_FB_RUN_FUNCTION_INSTANCE->pComp[5][x]
#define NCAPSA_pComp7(x) EHS_FB_RUN_FUNCTION_INSTANCE->pComp[6][x]
#define NCAPSA_pComp8(x) EHS_FB_RUN_FUNCTION_INSTANCE->pComp[7][x]

/* data accessor macros - inputs...*/
#define NCAPSA_nIn(x) EHS_FB_IN_I(x)
#define NCAPSA_dIn(x) EHS_FB_IN_F(x)
#define NCAPSA_bIn(x) EHS_FB_IN_B(x)
#define NCAPSA_szIn(x) EHS_FB_IN_S(x)
/* data accessor macros - outputs...*/
#define NCAPSA_nOut(x) EHS_FB_OUT_I(x)
#define NCAPSA_dOut(x) EHS_FB_OUT_F(x)
#define NCAPSA_bOut(x) EHS_FB_OUT_B(x)
#define NCAPSA_szOut(x) EHS_FB_OUT_S(x)
/* data accessor macros  - completes...*/
/* todo Legacy API Only uses the hardwired numbers */
#define NCAPSA_bComp1(x) *(ehs_bool*)NCAPSA_pComp1(x)
#define NCAPSA_bComp2(x) *(ehs_bool*)NCAPSA_pComp2(x)
#define NCAPSA_bComp3(x) *(ehs_bool*)NCAPSA_pComp3(x)
#define NCAPSA_bComp4(x) *(ehs_bool*)NCAPSA_pComp4(x)
#define NCAPSA_bComp5(x) *(ehs_bool*)NCAPSA_pComp5(x)
#define NCAPSA_bComp6(x) *(ehs_bool*)NCAPSA_pComp6(x)
#define NCAPSA_bComp7(x) *(ehs_bool*)NCAPSA_pComp7(x)
#define NCAPSA_bComp8(x) *(ehs_bool*)NCAPSA_pComp8(x)
#define NCAPSA_pLocState (((ehs_uint8*)(pByte[0])))

/* data accessor macros - data type ids and data table element locations
 * char cDataType[MAX_NUM_OUTPUTS];
	int nLocation[MAX_NUM_OUTPUTS];
 */
#define NCAPSA_cDataType(x) EHS_FB_RUN_FUNCTION_INSTANCE->cDataType[x]
#define NCAPSA_nLocation(x) EHS_FB_RUN_FUNCTION_INSTANCE->nLocation[x]

#include "fid.h"

#endif /* EHS_TYPES_H */

