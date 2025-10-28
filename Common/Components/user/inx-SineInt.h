#ifndef INXWARE_SineInt
#define INXWARE_SineInt
#include "inx-component.h"

/*****************************************************
 * This component performs the sin function on the integer input and presents the result to the data output. Please note that the data input is in degrees or radians in a thousand set by the Boolean parameter.

The sine of an angle is the ratio of the length of the opposite side to the length of the hypotenuse, of a right angled triangle.

The mathematical operation is performed when the input event is triggered and the result of the mathematical operation is presented to the data output. The output event is triggered to signal the completion of the mathematical operation.
 *****************************************************/
#define INXWARE_FB_ID_SineInt 0xA860
#define INXWARE_FB_NAME_SineInt "SineInt"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SineInt); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SineInt); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(SineInt); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SineInt_calc); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SineInt)

#endif /* INXWARE_SineInt */
