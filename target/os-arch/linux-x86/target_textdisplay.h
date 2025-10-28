/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* @file target_textdisplay.h
 *
 * @author: inx limited
 *
 */

#ifndef TARGET_TEXTDISPLAYX_H_
#define TARGET_TEXTDISPLAYX_H_
#include "globals.h"
/*Declare global functions for working with textdisplay*/
EHS_GLOBAL void EhsInitTextdisplay();
EHS_GLOBAL void EhsWriteTextdisplay(ehs_sint32 row, ehs_sint32 col, char* text);
EHS_GLOBAL void EhsClearTextdisplay();
#endif /* TARGET_TEXTDISPLAYX_H_ */
