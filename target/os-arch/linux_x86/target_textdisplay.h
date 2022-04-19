/*
 * target_textdisplay.h
 *
 *  Created on: 08-Sep-2008
 *      Author: pbeaumont
 */

#ifndef TARGET_TEXTDISPLAYX_H_
#define TARGET_TEXTDISPLAYX_H_
#include "target_types.h"
/*Declare global functions for working with textdisplay*/
EHS_GLOBAL void EhsInitTextdisplay();
EHS_GLOBAL void EhsWriteTextdisplay(ehs_sint32 row, ehs_sint32 col, char* text);
EHS_GLOBAL void EhsClearTextdisplay();
#endif /* TARGET_TEXTDISPLAYX_H_ */
