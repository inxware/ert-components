/***************************************************************
 * Copyright (C) 2025 inx limited, UK - All Rights Reserved.
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit:
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#define EHS_TARGET_CODE /* Ensure header files include target-internal values */
#include "targetgfx_init.h"
#include "hal_viewport.h"
#include "messages.h"

// extern EhsBlockRefType EhsBlockRefTable_Dtv[];

void EhsTGfxSys_init(void)
{
    EHSH_LOG_INFO("Qt Graphics System Init\n");

    if (!EhsTV_init(&EhsTV))
    {
        EHSH_LOG_ERROR("Qt viewport init failed\n");
        exit(1);
    }
}

void EhsTGfxSys_term(void)
{
    EhsTV_term(&EhsTV);
}

void EhsTGfxApp_init(void)
{
    // Per-application init (if needed)
}

void EhsTGfxApp_term(void)
{
    // Per-application cleanup
}

void EhsTGfxApp_reset(void)
{
    // Per-application reset
}
