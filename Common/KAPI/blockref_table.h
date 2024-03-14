/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file blockref_table.h
 * Contains the declaration of the function block reference table for EHS
 *
 * @author: inx limited
 *
 */

#ifndef EHS_BLOCKREF_TABLE_H
#define EHS_BLOCKREF_TABLE_H

/*****************************************************************************/
/* Included files */

//#include "ehs_types.h"
#include "globals.h"
#include "ehs_fb_types.h"
//#include "ehs_types.h"

//#include "hal-api.h"
//#include "target.h"


/*****************************************************************************/
/* declare macros
 * @todo - the public part of this needs to be moved to the inxware API file
 * @todo - should have one version of the following
 * */

typedef const struct
{
#ifdef EHRT1
    const ehs_uint16 szName; /**< Function block name */
#else //#ifdef EHRT1
    const ehs_char *szName; /**< Function block name */
#endif //#else #ifdef EHRT1
    ehs_uint16 nVers; /**< Function block version */
    EhsInitFuncType fpInitFunc; /**< The initialise function for this function block */
    EhsDestroyFuncType fpDestroyFunc; /**< The initialise function for this function block */
    EhsIdentifyFuncType fpIdentifyFunc; /**< The identify function for this function block */
    EhsFuncRefType *xFunctions; /**< The set of functions contained within this function block */
} EhsBlockRefType;

/** Table Entries
 *
 * Creates a macro for defining a single entry in the block reference table.
 *
 * The second version places a null in the Destructor function for Legacy and objects that do not require this.
 *
 * note - changed macros to populate vers field in Blockref struct - old API creates entries with vers=0, new API (API2) has vers passed in as a parameter
 */
//#define EHS_BLOCKREF_ENTRY(name,x) {(name),(EhsInitFuncType)EHS_FB_INIT_NAME(x), (EhsDestroyFuncType)NULL, (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME(x), EHS_FB_FUNCTABLE_NAME(x)}
//#define EHS_BLOCKREF_ENTRY_API2(x,vers) {(EHS_FB_VAR_TO_STRING(x)),(EhsInitFuncType)EHS_FB_INIT_NAME_API2(x,vers), (EhsDestroyFuncType)NULL, (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME_API2(x,vers),  EHS_FB_FUNCTABLE_NAME_API2(x,vers)}
//#define EHS_BLOCKREF_ENTRY_WITH_DESTROY(name,x) {(name),(EhsInitFuncType)EHS_FB_INIT_NAME(x), (EhsDestroyFuncType)EHS_FB_DESTROY_NAME(x), (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME(x),  EHS_FB_FUNCTABLE_NAME(x)}
//#define EHS_BLOCKREF_ENTRY_WITH_DESTROY_API2(x,vers) {(EHS_FB_VAR_TO_STRING(x)),(EhsInitFuncType)EHS_FB_INIT_NAME_API2(x,vers), (EhsDestroyFuncType)EHS_FB_DESTROY_NAME_API2(x,vers), (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME_API2(x,vers),  EHS_FB_FUNCTABLE_NAME_API2(x,vers)}
//TODO2022 ASK aptrick what the (0) do in these functions - has the user components got a different implementation??
#ifdef EHRT1
#define EHS_BLOCKREF_ENTRY(name,id,x) {(id), (0), (EhsInitFuncType)EHS_FB_INIT_NAME(x), (EhsDestroyFuncType)NULL, (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME(x), EHS_FB_FUNCTABLE_NAME(x)}
#define EHS_BLOCKREF_ENTRY_API2(x,id,vers) {(id), (vers),(EhsInitFuncType)EHS_FB_INIT_NAME_API2(x,vers), (EhsDestroyFuncType)NULL, (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME_API2(x,vers),  EHS_FB_FUNCTABLE_NAME_API2(x,vers)}
#define EHS_BLOCKREF_ENTRY_WITH_DESTROY(name,id,x) {(id), (0),(EhsInitFuncType)EHS_FB_INIT_NAME(x), (EhsDestroyFuncType)EHS_FB_DESTROY_NAME(x), (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME(x),  EHS_FB_FUNCTABLE_NAME(x)}
#define EHS_BLOCKREF_ENTRY_WITH_DESTROY_API2(x,id,vers) {(id), (vers),(EhsInitFuncType)EHS_FB_INIT_NAME_API2(x,vers), (EhsDestroyFuncType)EHS_FB_DESTROY_NAME_API2(x,vers), (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME_API2(x,vers),  EHS_FB_FUNCTABLE_NAME_API2(x,vers)}
#else
// The original API needed the function block name as quoted string (arg1) and also C envornment function name - resolved in API2
#define EHS_BLOCKREF_ENTRY(_string_functionBlockName,id,_c_functionBlockName) {(_string_functionBlockName), (0), (EhsInitFuncType)EHS_FB_INIT_NAME(_c_functionBlockName), (EhsDestroyFuncType)NULL, (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME(_c_functionBlockName), EHS_FB_FUNCTABLE_NAME(_c_functionBlockName)}
// API2 avoids duplicating the function name as the 1st and 3rd arguments 
#define EHS_BLOCKREF_ENTRY_API2(_c_functionBlockName,id,vers) {(EHS_FB_VAR_TO_STRING(_c_functionBlockName)), (vers),(EhsInitFuncType)EHS_FB_INIT_NAME_API2(_c_functionBlockName,vers), (EhsDestroyFuncType)NULL, (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME_API2(_c_functionBlockName,vers),  EHS_FB_FUNCTABLE_NAME_API2(_c_functionBlockName,vers)}
#define EHS_BLOCKREF_ENTRY_WITH_DESTROY(_string_functionBlockName,id,_c_functionBlockName) {(_string_functionBlockName), (0),(EhsInitFuncType)EHS_FB_INIT_NAME(_c_functionBlockName), (EhsDestroyFuncType)EHS_FB_DESTROY_NAME(_c_functionBlockName), (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME(_c_functionBlockName),  EHS_FB_FUNCTABLE_NAME(_c_functionBlockName)}
#define EHS_BLOCKREF_ENTRY_WITH_DESTROY_API2(_c_functionBlockName,id,vers) {(EHS_FB_VAR_TO_STRING(_c_functionBlockName)), (vers),(EhsInitFuncType)EHS_FB_INIT_NAME_API2(_c_functionBlockName,vers), (EhsDestroyFuncType)EHS_FB_DESTROY_NAME_API2(_c_functionBlockName,vers), (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME_API2(_c_functionBlockName,vers),  EHS_FB_FUNCTABLE_NAME_API2(_c_functionBlockName,vers)}
#endif
/* IMPORTANT NOTES ON API2 !!!!!!!!
The iCB tool is currently (2022) is using API2 for both Port event and data and also the component's block entry in the xxxcomponents.c files.
However ALL current functionblcks are defined with API1 (which doesn'y support versions and this will mean all C file name would need to be changed to use API2 for block entries)
So for now:
(1) iCB will generate block entries using API version 1.
(2) Decide if API2 for block entries will ever be useful? 
-> One advantage is it only needs the function block name parameters once (andf ot in stringified form too) 
-> One disdvantage is that API2 supports function block versioning, which I think we probably ant to descope as it will be a pain for hashing and we don't really use the deprecated toolbox anyway...
*/

/**
 * Allows a single entry in the block reference table with a different init/identify name.
 * Allows for init/identify to be shared between blocks
 *
 * note - changed macros to populate vers field in Blockref struct - old API creates entries with vers=0, new API (API2) has vers passed in as a parameter
 */
//@todo should flatten all this and not hae the toolkits? tbc
//@TODO ref tables should be compilable from dlls also - for dynamic builds
//#define EHS_BLOCKREF_ENTRY_EX(name, x, init, ident) {name,(EhsInitFuncType)init,(EhsDestroyFuncType)NULL,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME(x)}
//#define EHS_BLOCKREF_ENTRY_EX_WITH_DESTROY(name, x, init, destroy, ident) {name,(EhsInitFuncType)init,(EhsDestroyFuncType)destroy,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME(x)}
#ifdef EHRT1
#define EHS_BLOCKREF_ENTRY_EX(name, id, x, init, ident) {id, (0),(EhsInitFuncType)init,(EhsDestroyFuncType)NULL,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME(x)}
#define EHS_BLOCKREF_ENTRY_EX_API2(name, id, x, init, ident, vers) {id, (vers),(EhsInitFuncType)init,(EhsDestroyFuncType)NULL,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME_API2(x,vers)}
#define EHS_BLOCKREF_ENTRY_EX_WITH_DESTROY(name, id, x, init, destroy, ident) {id, (0),(EhsInitFuncType)init,(EhsDestroyFuncType)destroy,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME(x)}
#define EHS_BLOCKREF_ENTRY_EX_WITH_DESTROY_API2(name, id, x, init, destroy, ident, vers) {id, (vers),(EhsInitFuncType)init,(EhsDestroyFuncType)destroy,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME_API2(x,vers)}
#else
#define EHS_BLOCKREF_ENTRY_EX(name, id, x, init, ident) {name, (0),(EhsInitFuncType)init,(EhsDestroyFuncType)NULL,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME(x)}
#define EHS_BLOCKREF_ENTRY_EX_API2(name, id, x, init, ident, vers) {name, (vers),(EhsInitFuncType)init,(EhsDestroyFuncType)NULL,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME_API2(x,vers)}
#define EHS_BLOCKREF_ENTRY_EX_WITH_DESTROY(name, id, x, init, destroy, ident) {name, (0),(EhsInitFuncType)init,(EhsDestroyFuncType)destroy,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME(x)}
#define EHS_BLOCKREF_ENTRY_EX_WITH_DESTROY_API2(name, id, x, init, destroy, ident, vers) {name, (vers),(EhsInitFuncType)init,(EhsDestroyFuncType)destroy,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME_API2(x,vers)}
#endif

/*****************************************************************************/
/* Declare typedefs and struct definitions */

/**
 * Declares a type representing a single instance of a function block
 */



/*****************************************************************************/
/* Declare globals */

/**
 * Declares a function block reference table
 */

// this just seems to cause problems - removing - typedef EhsBlockRefType * EhsBlockRefTableType;


/**
 * Declares a type containing a set of block reference tables, each corresponding to a toolkit
 */
typedef struct
{
    EhsBlockRefType* pxRefTable[EHS_MAX_TOOLKITS];  /**< Reference to the toolkits */
    ehs_uint16 nNumToolkits; /**< Number of toolkits in the table */
} EhsToolkitTableType;

/**
 * Declares a table containing all of the toolkits
 */
EHS_GLOBAL EhsToolkitTableType EhsToolkitTable;

/**
 * Reset EhsToolkitTable to be empty.
 */
EHS_GLOBAL void EhsToolkitTable_reset(void);

/**
 * Add a new block reference table (aka toolkit) to the list of toolkits that we search.
 * @param pTable Pointer to the table to add
 * @return true if the table was added successfully.
 */
EHS_GLOBAL ehs_bool EhsToolkitTable_addTable(EhsBlockRefType* pTable);

/**
 * Search the set of toolkit tables for a function block with a specific name. Search is case-insensitive.
 *
 * @param bIncorrectVers return param for flagging found name but not vers
 * @param szName Name to search table for
 * @param nVers Vers to search table for
 * @return NULL if no matching name and vers found, or pointer to block reference if it is found.
 */
#ifndef EHRT1
EHS_GLOBAL EhsBlockRefType* EhsBlockRefTable_findObject(ehs_bool * bIncorrectVers, const ehs_char* szName, ehs_uint16 nVers);
#else
EHS_GLOBAL EhsBlockRefType* EhsBlockRefTable_findObject(ehs_bool * bIncorrectVers, const ehs_uint16 szName, ehs_uint16 nVers);
#endif


/**
 * Search an EhsBlockRefType for a function with a specific name. Search is case-insensitive.
 *
 * @param block Function block to search
 * @param szName Function name to search table for
 * @return NULL if name not found, or pointer to EhsFuncRefType reference if it is found.
 */
#ifndef EHRT1
EHS_GLOBAL EhsFuncRefType* EhsBlockRef_findFunction(EhsBlockRefType* block, const ehs_char* szName);
#else
EHS_GLOBAL EhsFuncRefType* EhsBlockRef_findFunction(EhsBlockRefType* block, const ehs_uint8 szName);
#endif



#endif /* EHS_BLOCKREF_TABLE_H */

