/** @file blockref_table.h
 * Contains the declaration of the function block reference table for EHS
 * 
 * @author: inx limited
 * @version: $Revision: 4748 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_BLOCKREF_TABLE_H
#define EHS_BLOCKREF_TABLE_H

/*****************************************************************************/
/* Included files */

//#include "ehs_types.h"
#include "globals.h"
//#include "parse_sodl.h"

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
#define EHS_BLOCKREF_ENTRY(name,x) {(name), (0), (EhsInitFuncType)EHS_FB_INIT_NAME(x), (EhsDestroyFuncType)NULL, (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME(x), EHS_FB_FUNCTABLE_NAME(x)}
#define EHS_BLOCKREF_ENTRY_API2(x,vers) {(EHS_FB_VAR_TO_STRING(x)), (vers),(EhsInitFuncType)EHS_FB_INIT_NAME_API2(x,vers), (EhsDestroyFuncType)NULL, (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME_API2(x,vers),  EHS_FB_FUNCTABLE_NAME_API2(x,vers)}
#define EHS_BLOCKREF_ENTRY_WITH_DESTROY(name,x) {(name), (0),(EhsInitFuncType)EHS_FB_INIT_NAME(x), (EhsDestroyFuncType)EHS_FB_DESTROY_NAME(x), (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME(x),  EHS_FB_FUNCTABLE_NAME(x)}
#define EHS_BLOCKREF_ENTRY_WITH_DESTROY_API2(x,vers) {(EHS_FB_VAR_TO_STRING(x)), (vers),(EhsInitFuncType)EHS_FB_INIT_NAME_API2(x,vers), (EhsDestroyFuncType)EHS_FB_DESTROY_NAME_API2(x,vers), (EhsIdentifyFuncType)EHS_FB_IDENTIFY_NAME_API2(x,vers),  EHS_FB_FUNCTABLE_NAME_API2(x,vers)}

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
#define EHS_BLOCKREF_ENTRY_EX(name, x, init, ident) {name, (0),(EhsInitFuncType)init,(EhsDestroyFuncType)NULL,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME(x)}
#define EHS_BLOCKREF_ENTRY_EX_API2(name, x, init, ident, vers) {name, (vers),(EhsInitFuncType)init,(EhsDestroyFuncType)NULL,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME_API2(x,vers)}
#define EHS_BLOCKREF_ENTRY_EX_WITH_DESTROY(name, x, init, destroy, ident) {name, (0),(EhsInitFuncType)init,(EhsDestroyFuncType)destroy,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME(x)}
#define EHS_BLOCKREF_ENTRY_EX_WITH_DESTROY_API2(name, x, init, destroy, ident, vers) {name, (vers),(EhsInitFuncType)init,(EhsDestroyFuncType)destroy,(EhsIdentifyFuncType)ident,EHS_FB_FUNCTABLE_NAME_API2(x,vers)}


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
EHS_GLOBAL EhsFuncRefType* EhsBlockRef_findFunction(EhsBlockRefType* block, const ehs_uint16 szName);
#endif



#endif /* EHS_BLOCKREF_TABLE_H */

