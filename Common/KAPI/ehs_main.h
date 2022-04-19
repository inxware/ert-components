/** @file ehs_main.h
 * In this file, all of the hardware abstraction layer functions provided for EHS are given.
 * 
 * @author: inx limited
 * @version: $Revision: 5649 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_MAIN_H
#define EHS_MAIN_H

#include "ehs_types.h"

/**
 * EHS Entry point. Initialise EHS, then start running EHS & handling console
 * input.
 * This can return running state change requests
 */
EHS_GLOBAL void EhsMain( Ehs_ConsoleCommand_Type (*target_loop_iteration)(void*),void * target_env_blob);

/**
 * Perform one step of main EHS loop. This can return running state change requests
 */
EHS_GLOBAL Ehs_ConsoleCommand_Type EhsMainLoop( Ehs_ConsoleCommand_Type (*target_loop_iteration)(void*),void * target_env_blob );

Ehs_ConsoleCommand_Type EhsCheckInternalCommands(Ehs_ConsoleCommand_Type Command);
ehs_bool SetupApplication();
#endif /* EHS_MAIN_H */
