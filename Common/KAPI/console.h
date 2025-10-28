/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file console.h
 * Defines console interface to EHS.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_CONSOLE
#define EHS_CONSOLE

#include "globals.h"

/* We normally change all file lengths to 8.3 file formats for all targets: */
#define EHS_CONSOLE_MAX_FILENAME_LENGTH 32

EHS_GLOBAL Ehs_ConsoleCommand_Type EhsProcessCommands(void);

#endif /* EHS_CONSOLE */
