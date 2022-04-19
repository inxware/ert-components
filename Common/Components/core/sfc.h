/**
 * sfc.h
 *
 * function prototypes for sfc.c - for state flow control? Not really sure.
 *
 * @author: ??
 * @version: $Revision: 278 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef EHS_SFC_H
#define EHS_SFC_H

#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_NAME "SFCBar"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBar);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBar);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarPreviousStateComplete);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarConditionUpdate);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBar)


/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP1_NAME "SFCBarGroup1"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup1)

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP2_NAME "SFCBarGroup2"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup2);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup2)

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP3_NAME "SFCBarGroup3"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup3);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup3)

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP4_NAME "SFCBarGroup4"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup4);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup4);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup4)

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP8_NAME "SFCBarGroup8"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup8);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup8);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup8);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup8)

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP12_NAME "SFCBarGroup12"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup12);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup12);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup12);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup12)

/* common functions for SFCBarGroup */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Run_CL);

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP1EVENT_NAME "SFCBarGroup1Event"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup1Event);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup1Event);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup1Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup1Event)

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP2EVENT_NAME "SFCBarGroup2Event"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup2Event);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup2Event);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup2Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup2Event)

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP3EVENT_NAME "SFCBarGroup3Event"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup3Event);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup3Event);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup3Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup3Event)

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP4EVENT_NAME "SFCBarGroup4Event"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup4Event);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup4Event);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup4Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup4Event)

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP8EVENT_NAME "SFCBarGroup8Event"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup8Event);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup8Event);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup8Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup8Event)

/******************************************************************************/
/* Declare SFC */
#define EHS_SFC_BAR_GROUP12EVENT_NAME "SFCBarGroup12Event"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup12Event);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SFCBarGroup12Event);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SFCBarGroup12Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SFCBarGroup12Event)

EHS_GLOBAL EHS_FB_RUN_FUNCTION(Run_CLEvent);

#endif
