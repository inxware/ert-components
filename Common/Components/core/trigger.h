/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * trigger.h
 *
 * function prototypes for triggerFunctions.c
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TRIGGER_FUNCTIONS_H
#define EHS_TRIGGER_FUNCTIONS_H


#include "ehs_fb_types.h"
/******************************************************************************/
//@todo - check if this component is no longer used, if so can be removed
/* Declare EventLatchingAnd */
#define EHS_FB_NAME_TRIGGER_LATCHING_AND_NAME "EventLatchingAnd"
#define EHS_FB_ID_TRIGGER_LATCHING_AND_NAME 0x7604

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventLatchingAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventLatchingAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventLatchingAnd1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventLatchingAnd2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventLatchingAndReset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventLatchingAnd)

/******************************************************************************/
/* Declare Start */
#define EHS_FB_NAME_Start "Start"
#define EHS_FB_ID_Start 0xA5EA

#define EHS_FB_START_VERS 0
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(Start);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(Start);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Start);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(Start)

/******************************************************************************/
/* Declare EventBooleanControl */

#define EHS_FB_NAME_EventBooleanControl "EventBooleanControl"
#define EHS_FB_ID_EventBooleanControl 0xA177

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventBooleanControl);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventBooleanControl);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventBooleanControl);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventBooleanControl)

/******************************************************************************/
/* Declare EventTwoInputLatchingAnd */

#define EHS_FB_NAME_EventTwoInputLatchingAnd "EventTwoInputLatchingAnd"
#define EHS_FB_ID_EventTwoInputLatchingAnd 0xD5A5

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventTwoInputLatchingAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventTwoInputLatchingAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventTwoInputLatchingAnd1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventTwoInputLatchingAnd2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventTwoInputLatchingAndReset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventTwoInputLatchingAnd)


/******************************************************************************/
/* Declare EventThreeInputLatchingAnd */
#define EHS_FB_NAME_EventThreeInputLatchingAnd "EventThreeInputLatchingAnd"
#define EHS_FB_ID_EventThreeInputLatchingAnd 0x0185

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventThreeInputLatchingAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventThreeInputLatchingAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventThreeInputLatchingAnd1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventThreeInputLatchingAnd2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventThreeInputLatchingAnd3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventThreeInputLatchingAndReset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventThreeInputLatchingAnd)

/******************************************************************************/
/* Declare EventFourInputLatchingAnd */
#define EHS_FB_NAME_EventFourInputLatchingAnd "EventFourInputLatchingAnd"
#define EHS_FB_ID_EventFourInputLatchingAnd 0x6574

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventFourInputLatchingAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventFourInputLatchingAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventFourInputLatchingAnd1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventFourInputLatchingAnd2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventFourInputLatchingAnd3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventFourInputLatchingAnd4);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventFourInputLatchingAndReset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventFourInputLatchingAnd)

/******************************************************************************/
/* Declare EventOneInputManRstAnd */

#define EHS_FB_NAME_EventOneInputManRstAnd "EventOneInputManRstAnd"
#define EHS_FB_ID_EventOneInputManRstAnd 0xA002

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventOneInputManRstAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventOneInputManRstAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventOneInputManRstAnd1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventOneInputManRstAndReset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventOneInputManRstAnd)

/******************************************************************************/
/* Declare EventTwoInputManRstAnd */

#define EHS_FB_NAME_EventTwoInputManRstAnd "EventTwoInputManRstAnd"
#define EHS_FB_ID_EventTwoInputManRstAnd 0xCA39

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventTwoInputManRstAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventTwoInputManRstAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventTwoInputManRstAnd1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventTwoInputManRstAnd2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventTwoInputManRstAndReset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventTwoInputManRstAnd)


/******************************************************************************/
/* Declare EventThreeInputManRstAnd */

#define EHS_FB_NAME_EventThreeInputManRstAnd "EventThreeInputManRstAnd"
#define EHS_FB_ID_EventThreeInputManRstAnd 0xFA45

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventThreeInputManRstAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventThreeInputManRstAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventThreeInputManRstAnd1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventThreeInputManRstAnd2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventThreeInputManRstAnd3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventThreeInputManRstAndReset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventThreeInputManRstAnd)

/******************************************************************************/
/* Declare EventFourInputManRstAnd */
#define EHS_FB_NAME_EventFourInputManRstAnd "EventFourInputManRstAnd"
#define EHS_FB_ID_EventFourInputManRstAnd 0xB4F8

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventFourInputManRstAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventFourInputManRstAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventFourInputManRstAnd1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventFourInputManRstAnd2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventFourInputManRstAnd3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventFourInputManRstAnd4);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventFourInputManRstAndReset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventFourInputManRstAnd)

/******************************************************************************/
/* Declare EventSelfResetTwoInputLatchingAnd */

#define EHS_FB_NAME_EventSelfResetTwoInputLatchingAnd "EventSelfResetTwoInputLatchingAnd"
#define EHS_FB_ID_EventSelfResetTwoInputLatchingAnd 0x2A9C

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventSelfResetTwoInputLatchingAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventSelfResetTwoInputLatchingAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventSelfResetTwoInputLatchingAnd1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventSelfResetTwoInputLatchingAnd2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventSelfResetTwoInputLatchingAnd)

/******************************************************************************/
/* Declare EventSelfResetThreeInputLatchingAnd */

#define EHS_FB_NAME_EventSelfResetThreeInputLatchingAnd "EventSelfResetThreeInputLatchingAnd"
#define EHS_FB_ID_EventSelfResetThreeInputLatchingAnd 0x11D7

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventSelfResetThreeInputLatchingAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventSelfResetThreeInputLatchingAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventSelfResetThreeInputLatchingAnd1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventSelfResetThreeInputLatchingAnd2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventSelfResetThreeInputLatchingAnd3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventSelfResetThreeInputLatchingAnd)

/******************************************************************************/
/* Declare EventSelfResetFourInputLatchingAnd */

#define EHS_FB_NAME_EventSelfResetFourInputLatchingAnd "EventSelfResetFourInputLatchingAnd"
#define EHS_FB_ID_EventSelfResetFourInputLatchingAnd 0x774B

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventSelfResetFourInputLatchingAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventSelfResetFourInputLatchingAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventSelfResetFourInputLatchingAnd1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventSelfResetFourInputLatchingAnd2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventSelfResetFourInputLatchingAnd3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventSelfResetFourInputLatchingAnd4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventSelfResetFourInputLatchingAnd)

/******************************************************************************/
/* Declare EventTwoInputOr */

#define EHS_FB_NAME_EventTwoInputOr "EventTwoInputOr"
#define EHS_FB_ID_EventTwoInputOr 0x0A91

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventTwoInputOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventTwoInputOr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventInputOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventTwoInputOr)

/******************************************************************************/
/* Declare EventThreeInputOr */

#define EHS_FB_NAME_EventThreeInputOr "EventThreeInputOr"
#define EHS_FB_ID_EventThreeInputOr 0xBA97

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventThreeInputOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventThreeInputOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventThreeInputOr)

/******************************************************************************/
/* Declare EventFourInputOr */

#define EHS_FB_NAME_EventFourInputOr "EventFourInputOr"
#define EHS_FB_ID_EventFourInputOr 0x549D

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventFourInputOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventFourInputOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventFourInputOr)

/******************************************************************************/
/* Declare EventTwoInputBooleanOr */

#define EHS_FB_NAME_TRIGGER_TWO_INPUT_BOOLEAN_OR_NAME "EventTwoInputBooleanOr"
#define EHS_FB_ID_TRIGGER_TWO_INPUT_BOOLEAN_OR_NAME 0x3008

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventTwoInputBooleanOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventTwoInputBooleanOr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventTwoInputBooleanOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventTwoInputBooleanOr)

/******************************************************************************/
/* Declare EventThreeInputBooleanOr */
#define EHS_FB_NAME_TRIGGER_THREE_INPUT_BOOLEAN_OR_NAME "EventThreeInputBooleanOr"
#define EHS_FB_ID_TRIGGER_THREE_INPUT_BOOLEAN_OR_NAME 0x0074

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventThreeInputBooleanOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventThreeInputBooleanOr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventThreeInputBooleanOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventThreeInputBooleanOr)

/******************************************************************************/
/* Declare EventFourInputBooleanOr */

#define EHS_FB_NAME_TRIGGER_FOUR_INPUT_BOOLEAN_OR_NAME "EventFourInputBooleanOr"
#define EHS_FB_ID_TRIGGER_FOUR_INPUT_BOOLEAN_OR_NAME 0x4EC9
#
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventFourInputBooleanOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventFourInputBooleanOr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventFourInputBooleanOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventFourInputBooleanOr)

/******************************************************************************/
/* Declare EventToggleBoolean */

#define EHS_FB_NAME_EventToggleBoolean "EventToggleBoolean"
#define EHS_FB_ID_EventToggleBoolean 0x659F

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventToggleBoolean);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventToggleBoolean);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventToggleBoolean);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventToggleBoolean)

/******************************************************************************/
/* Declare EventPositiveEdgeDetect */

#define EHS_FB_NAME_EventPositiveEdgeDetect "EventPositiveEdgeDetect"
#define EHS_FB_ID_EventPositiveEdgeDetect 0x0039

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventPositiveEdgeDetect);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventPositiveEdgeDetect);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventPositiveEdgeDetect);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventPositiveEdgeDetect)

/******************************************************************************/
/* Declare EventNegativeEdgeDetect */

#define EHS_FB_NAME_EventNegativeEdgeDetect "EventNegativeEdgeDetect"
#define EHS_FB_ID_EventNegativeEdgeDetect 0x857E

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventNegativeEdgeDetect);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventNegativeEdgeDetect);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventNegativeEdgeDetect);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventNegativeEdgeDetect)

/******************************************************************************/
/* Declare EventEdgeDetect */

#define EHS_FB_NAME_EventEdgeDetect "EventEdgeDetect"
#define EHS_FB_ID_EventEdgeDetect 0x3C33

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventEdgeDetect);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventEdgeDetect);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventEdgeDetect);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventEdgeDetect)

/******************************************************************************/
/* Declare EventSetReset */

#define EHS_FB_NAME_EventSetReset "EventSetReset"
#define EHS_FB_ID_EventSetReset 0x0166

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventSetReset);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventSetReset);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventSetReset_Set);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventSetReset_Reset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventSetReset)

/******************************************************************************/
/* Declare EventOr */
#define EHS_FB_NAME_TRIGGER_OR_NAME "EventOr"
#define EHS_FB_ID_TRIGGER_OR_NAME 0x7DBB

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventOr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventOr)

/******************************************************************************/
/* Declare EventCounter */

#define EHS_FB_NAME_EventCounter "EventCounter"
#define EHS_FB_ID_EventCounter 0xB649

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventCounter);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventCounter);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounter);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounter_Reset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventCounter)


#endif /* EHS_TRIGGER_FUNCTIONS_H */
