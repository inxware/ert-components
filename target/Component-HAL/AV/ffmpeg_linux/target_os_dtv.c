/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#include "target_os_dtv.h"

#include "target_dtv.h"

#include "hal-api.h"

void* EhsTDPlayback_init(EhsFbPvrPlayClass *pPvrPlay)
{
    //Initialise the structure for our target dependent dtv
    ehs_char full_plugin_path[EHS_SYS_MAXPATHLENGTH];
    struct EhsTDPlaybackStruct* pPlayback =(struct EhsTDPlaybackStruct*) EhsHMem_tempAlloc(sizeof(struct EhsTDPlaybackStruct));
    if (!pPlayback)
    {
        EHSH_LOG_ERROR("Failed to allocate memory for dtv\n");
        return NULL;
    }
    // @TODO - init lvgl ffmpeg
    
    pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED; //@todo move to the holding class
    pPlayback->nPlaySpeed = 0; //@todo move to the holding class
    pPlayback->mediaFormat=EHS_FALSE;
    pPlayback->nVolume=50; //@todo move to the holding class //set a defailt value. persistent values are set by the player manager

    return pPlayback;
}

EHS_GLOBAL void EhsTDPlayback_setWindow(EhsFbPvrPlayClass* pPvrPlay)
{

}

EHS_GLOBAL void EhsTDPlayback_closeWindow(EhsFbPvrPlayClass* pPvrPlay)
{
   
}

EHS_GLOBAL void EhsTDPlayback_updateZorder(EhsFbPvrPlayClass* pPvr)
{
    
}
