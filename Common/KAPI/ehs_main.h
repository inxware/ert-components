/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file ehs_main.h
 * In this file, all of the hardware abstraction layer functions provided for EHS are given.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_MAIN_H
#define EHS_MAIN_H

#include "globals.h"

/**
 * @brief Initilised EHS (data tables, kernel console etc.). Call this before looping to intialise data strcuctures and hardware init 
 *       Finally this identifies  and loads boot app. 
 * @note THIS SHOULD ONLY BE CALLED IF EheMain() is not being used as it is called from there.
 */

EHS_GLOBAL ehs_bool EhsInit();

/**
 * @brief EHS Entry point. Initialise EHS, then start running EHS & handling console
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

EHS_GLOBAL void EhsAppLoadingStateMachine( Ehs_ConsoleCommand_Type (*target_loop_iteration)(void*),void * target_env_blob );
EHS_GLOBAL Ehs_ConsoleCommand_Type EhsProcessInAppStateMachine( Ehs_ConsoleCommand_Type _Command );
EHS_GLOBAL Ehs_ConsoleCommand_Type EhsProcessExAppStateMachine( Ehs_ConsoleCommand_Type _Command );
EHS_GLOBAL ehs_bool EhsCheckAppExitLoop( Ehs_ConsoleCommand_Type Command );

#endif /* EHS_MAIN_H */
