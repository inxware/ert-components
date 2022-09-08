/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file ehs_main.h
 * In this file, all of the hardware abstraction layer functions provided for EHS are given.
 *
 * @author: inx limited
 *
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
