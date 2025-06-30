#ifndef INXWARE_TanInt
#define INXWARE_TanInt
#include "inx-component.h"

/*****************************************************
 * This component performs the tan function on the integer input and presents the result to the data output. Please note that the data input is in degrees or radians in a 1000th set by the Boolean parameter. The output is a thousand times the actual value.

The cosine of an angle is the ratio of the length of the opposite side to the length of the adjacent side, of a right angled triangle.

The mathematical operation is performed when the input event is triggered and the result of the mathematical operation is presented to the data output. The output event is triggered to signal the completion of the mathematical operation.
 *****************************************************/
#define INXWARE_FB_ID_TanInt 0x6551
#define INXWARE_FB_NAME_TanInt "TanInt"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(TanInt); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(TanInt); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(TanInt); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(TanInt_calc); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(TanInt)

#endif /* INXWARE_TanInt */
