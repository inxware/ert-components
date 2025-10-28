/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * sfc.h
 *
 * function prototypes for sfc.c - for state flow control? Not really sure.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_SFC_H
#define EHS_SFC_H

#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBar "SFCBar"
#define EHS_FB_ID_SFCBar 0x8999

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBar);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBar);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarPreviousStateComplete);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarConditionUpdate);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBar)


/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBarGroup1 "SFCBarGroup1"
#define EHS_FB_ID_SFCBarGroup1 0xB01F

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup1)

/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBarGroup2 "SFCBarGroup2"
#define EHS_FB_ID_SFCBarGroup2 0xB15F

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup2);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup2)

/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBarGroup3 "SFCBarGroup3"
#define EHS_FB_ID_SFCBarGroup3 0x719E

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup3);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup3);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup3)

/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBarGroup4 "SFCBarGroup4"
#define EHS_FB_ID_SFCBarGroup4 0xB3DF

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup4);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup4);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup4)

/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBarGroup8 "SFCBarGroup8"
#define EHS_FB_ID_SFCBarGroup8 0xB6DF

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup8);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup8);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup8);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup8)

/******************************************************************************/
/* Declare SFC */
#define EHS_FB_NAME_SFCBarGroup12 "SFCBarGroup12"
#define EHS_FB_ID_SFCBarGroup12 0x1D70

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup12);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup12);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup12);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup12)

/* common functions for SFCBarGroup */
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(Run_CL);

/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBarGroup1Event "SFCBarGroup1Event"
#define EHS_FB_ID_SFCBarGroup1Event 0xDD58

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup1Event);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup1Event);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup1Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup1Event)

/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBarGroup2Event "SFCBarGroup2Event"
#define EHS_FB_ID_SFCBarGroup2Event 0xEE58

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup2Event);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup2Event);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup2Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup2Event)

/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBarGroup3Event "SFCBarGroup3Event"
#define EHS_FB_ID_SFCBarGroup3Event 0x3F59

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup3Event);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup3Event);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup3Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup3Event)

/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBarGroup4Event "SFCBarGroup4Event"
#define EHS_FB_ID_SFCBarGroup4Event 0x8858

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup4Event);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup4Event);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup4Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup4Event)

/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBarGroup8Event "SFCBarGroup8Event"
#define EHS_FB_ID_SFCBarGroup8Event 0x4458

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup8Event);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup8Event);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup8Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup8Event)

/******************************************************************************/
/* Declare SFC */

#define EHS_FB_NAME_SFCBarGroup12Event "SFCBarGroup12Event"
#define EHS_FB_ID_SFCBarGroup12Event 0x78AC

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup12Event);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(SFCBarGroup12Event);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(SFCBarGroup12Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup12Event)

EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(Run_CLEvent);

#endif
