/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file console.h
 * Defines console interface to EHS.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_CONSOLE
#define EHS_CONSOLE

#include "ehs_types.h"

/* We normally change all file lengths to 8.3 file formats for all targets: */
#define EHS_CONSOLE_MAX_FILENAME_LENGTH 32

EHS_GLOBAL Ehs_ConsoleCommand_Type EhsProcessCommands(void);

#endif /* EHS_CONSOLE */
