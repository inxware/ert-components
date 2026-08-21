/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file core_components.c
 *
 * Implementation of the PVR functions for the DTV toolkit
 *
 * @author: inx limited
 *
 */

#include "target.h"

#include "dtv_pvr.h"
#include "globals.h"
#include "setCompletes.h"

EHS_FB_FUNCTIONS_START(DtvPvrList)
//EHS_FB_FUNCTION_ENTRY(EHS_FB_NAME_DTV_PVR_LIST_RUN_NAME, DtvPvrList)
EHS_FB_FUNCTIONS_END

/**
 * Define the identify function. Records the current index of the PVR recorded programme we
 * are looking at.
 */
EHS_FB_IDENTIFY_FUNCTION(DtvPvrList)
{
}

/**
 * Point to the first programme on our list
 */
EHS_FB_INIT_FUNCTION(DtvPvrList)
{
}



/**
 * Get the current programme from the list of available programmes.
 * Uses the TAP_Hdd_FindFirst()/TAP_Hdd_FindNext() functions.
 * @attention This Function is written with the assumption that only one
 * instance will be in use at any time. The effects of violating this
 * assumption are not guaranteed.
 */
EHS_FB_RUN_FUNCTION(DtvPvrList)
{
    TYPE_File fileInfo;
    static ehs_bool Restart = TRUE; /* whether to use FindFirst or FindNext */
    int fileCount;

    if (Restart)
    {
        TAP_Hdd_ChangeDir("/DataFiles");
        fileCount = TAP_Hdd_FindFirst(&fileInfo);
        if (fileCount > 0)
        {
            Restart = FALSE;
        }
    }
    else
    {
        if (!TAP_Hdd_FindNext(&fileInfo))
        {
            Restart = TRUE;
        }
    }

    /* if we haven't hit the end of the list, check that the file ends in ".rec".
     * if it doesn't, try to get another one.
     */
    while (!Restart)
    {
        if (!strcmp(&(fileInfo.name[strlen(fileInfo.name)-4]),".rec"))
        {
            break;
        }
        else
        {
            if (!TAP_Hdd_FindNext(&fileInfo))
            {
                Restart = TRUE;
            }
        }
    }

    NCAPSA_bOut(0) = Restart?EHS_FALSE:EHS_TRUE;
    if (!Restart)
    {
        strcpy(NCAPSA_szOut(1),fileInfo.name );
    }

    EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_FUNCTION_INSTANCE,1);
    return;
}

EHS_FB_FUNCTIONS_START(DtvPvrPlay)
EHS_FB_FUNCTION_ENTRY("run", 0x00, DtvPvrPlay)
EHS_FB_FUNCTIONS_END

/**
 * Define the identify function.
 */
EHS_FB_IDENTIFY_FUNCTION(DtvPvrPlay)
{
}

/**
 * Initialise play back (nothing to do here).
 */
EHS_FB_INIT_FUNCTION(DtvPvrPlay)
{
}

/**
 * Play a specified programme
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay)
{
    TAP_Hdd_ChangeDir("/DataFiles");
    TAP_Hdd_PlayTs(NCAPSA_szIn(0));

    EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_FUNCTION_INSTANCE,1);
    return;
}

