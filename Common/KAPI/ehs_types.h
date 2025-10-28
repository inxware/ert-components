/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file ehs_types.h
 * In this file, all of the standard type definitions required by EHS are given.
 * Any target-specific type definitions are given in target_types.h, which comes from
 * the target folder.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TYPES_H
#define EHS_TYPES_H
#include "ehs_defs.h"
#include "globals.h"

/*********************************************************************************************/
/**
 * These are the actual valid states for EHS. Empty indicates that no application is loaded,
 * Ready indicates that an application is loaded, but EHS is not currently executing it,
 * Running indicates that an application is loaded and EHS is running it.
 */
typedef enum EhsStateEnum {EHSKE_STATE_EMPTY, EHSKE_STATE_READY, EHSKE_STATE_RUNNING} EhsKEStateType;

/* These are the requested actions available for EHS.
 * Used primarily by the tcpip console (hence name), but also used by other entities
 * function blocks, HAL, devman to control device state
 * */
typedef enum
{
    EHS_RELOAD_EHS_FROM_FILE,			/* stop and reload from a file */
    EHS_RELOAD_EHS_FROM_FILE_DONTSTART, /* used only for debug mode */
    EHS_EXIT_EHS,						/* exit EHS completely */
    EHS_CONTINUE,						/* Carry on - nothing has happened! */
    //EHS_RUN,							/* todo this doesn't seem to be used anywhere */
    EHS_PAUSE							/* pause for something - not determined what */
} Ehs_ConsoleCommand_Type; /* todo refactor - rename to Ehs_ExternalCommandType */

/*********************************************************************************************/
/* Connection type definitions */
typedef enum {EHS_TYPE_ID_INT = 'I', EHS_TYPE_ID_BOL = 'B', EHS_TYPE_ID_DBL = 'F', EHS_TYPE_ID_STR = 'S', EHS_TYPE_ID_USR = 'U', EHS_TYPE_ID_UNDEF = 0xff} EhsDataTypeEnum;

/* Used for passing eRT target config info outside of ert-component e.g. kernel lib */
typedef struct EhsTargetConfigInfoType {
    const ehs_uint32 defaultStringDataLen;
} EhsTargetConfigInfoType;


#endif /* EHS_TYPES_H */

