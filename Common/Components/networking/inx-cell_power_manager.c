/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file inx-cell_power_manager.c
 * cell_power_manager function block — request the cellular power-saving modes
 * and report what the network actually granted.
 *
 * REQUEST AND GRANT ARE DIFFERENT THINGS, and that is the whole point of this
 * block. PSM and eDRX parameters are negotiated: the device asks, the network
 * grants something — often different from the ask, sometimes nothing at all.
 * An API or a block that only exposed the request would hide exactly the fact
 * a battery-powered deployment needs to know, so Apply and Read are separate
 * ports and the outputs are all granted values.
 *
 * @author: inx limited
 */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-cell_power_manager.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal-api.h"
#include "hal_logger.h"
#include "cellular.h"

//ICB STATE VAR MACRO START -- DO NOT ALTER
typedef struct inx_cell_power_manager_state
{
	ehs_bool   psmEnable;
	ehs_sint32 tauSeconds;
	ehs_sint32 activeSeconds;
	ehs_bool   edrxEnable;
	ehs_sint32 edrxCycleMs;
	ehs_bool   asRai;
	ehs_bool   cpRai;
} inx_cell_power_manager_state_type;
//ICB STATE VAR MACRO END -- DO NOT ALTER

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(cell_power_manager)
EHS_FB_FUNCTION_ENTRY("do_apply", 0x01, cell_power_manager_do_apply)
EHS_FB_FUNCTION_ENTRY("do_read", 0x02, cell_power_manager_do_read)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_cell_power_manager_ARG_do_apply_apply_ok 1
#define INX_cell_power_manager_ARG_do_apply_apply_fail 2
#define INX_cell_power_manager_ARG_do_read_read_ok 1
#define INX_cell_power_manager_ARG_do_read_psm_granted 1
#define INX_cell_power_manager_ARG_do_read_granted_tau 2
#define INX_cell_power_manager_ARG_do_read_granted_active 3
#define INX_cell_power_manager_ARG_do_read_edrx_granted 4
#define INX_cell_power_manager_ARG_do_read_granted_edrx_ms 5
#define INX_cell_power_manager_ARG_do_read_granted_ptw_ms 6
#define INX_cell_power_manager_ARG_do_read_rai_ok 7
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
#define INX_FB_cell_power_manager_psmEnable 1
#define INX_FB_cell_power_manager_tauSeconds 3600
#define INX_FB_cell_power_manager_activeSeconds 60
#define INX_FB_cell_power_manager_edrxEnable 0
#define INX_FB_cell_power_manager_edrxCycleMs 0
#define INX_FB_cell_power_manager_asRai 0
#define INX_FB_cell_power_manager_cpRai 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER

/* Set by the last do_apply so do_read can report whether RAI was accepted. */
static ehs_bool s_rai_ok = EHS_FALSE;

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_IDENTIFY_FUNCTION(cell_power_manager)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_cell_power_manager_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_INIT_FUNCTION(cell_power_manager)
{
	inx_cell_power_manager_state_type *st =
		(inx_cell_power_manager_state_type *)EHS_FB_INIT_CONTEXT;

	st->psmEnable     = INX_FB_cell_power_manager_psmEnable;
	st->tauSeconds    = INX_FB_cell_power_manager_tauSeconds;
	st->activeSeconds = INX_FB_cell_power_manager_activeSeconds;
	st->edrxEnable    = INX_FB_cell_power_manager_edrxEnable;
	st->edrxCycleMs   = INX_FB_cell_power_manager_edrxCycleMs;
	st->asRai         = INX_FB_cell_power_manager_asRai;
	st->cpRai         = INX_FB_cell_power_manager_cpRai;

	/* The four ehs_bool fields are 1 byte - %d would write 4. */
	EhsSscanf(EHS_FB_INIT_PARAMETERS, "%hhu %d %d %hhu %d %hhu %hhu",
	          &(st->psmEnable), &(st->tauSeconds), &(st->activeSeconds),
	          &(st->edrxEnable), &(st->edrxCycleMs), &(st->asRai), &(st->cpRai));
	return EHS_TRUE;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(cell_power_manager)
{
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_apply MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(cell_power_manager_do_apply)
{
	inx_cell_power_manager_state_type *st =
		(inx_cell_power_manager_state_type *)EHS_FB_RUN_CONTEXT;
	const ehs_uint32 caps = Cell_GetCapabilities();
	ehs_bool all_ok = EHS_TRUE;

	if (caps & (ehs_uint32)EHS_CELL_CAP_PSM) {
		if (!Cell_PsmRequest(st->psmEnable, st->tauSeconds, st->activeSeconds)) {
			EHSH_LOG_WARNING("cell_power_manager: PSM request rejected by the modem");
			all_ok = EHS_FALSE;
		}
	} else if (st->psmEnable) {
		EHSH_LOG_WARNING("cell_power_manager: PSM requested but not supported here");
		all_ok = EHS_FALSE;
	} else {
		/* Not asked for and not supported - nothing to report. */
	}

	if (caps & (ehs_uint32)EHS_CELL_CAP_EDRX) {
		if (!Cell_EdrxRequest(st->edrxEnable, Cell_GetActiveRat(), st->edrxCycleMs)) {
			EHSH_LOG_WARNING("cell_power_manager: eDRX request rejected by the modem");
			all_ok = EHS_FALSE;
		}
	} else if (st->edrxEnable) {
		EHSH_LOG_WARNING("cell_power_manager: eDRX requested but not supported here");
		all_ok = EHS_FALSE;
	} else {
		/* Not asked for and not supported. */
	}

	s_rai_ok = EHS_FALSE;
	if (st->asRai || st->cpRai) {
		if (caps & (ehs_uint32)EHS_CELL_CAP_RAI) {
			s_rai_ok = Cell_RaiSet(st->asRai, st->cpRai);
			if (!s_rai_ok) {
				all_ok = EHS_FALSE;
			}
		} else {
			EHSH_LOG_WARNING("cell_power_manager: RAI requested but not supported here");
			all_ok = EHS_FALSE;
		}
	}

	/* The REQUEST has been made. Whether the network grants it, and with what
	 * values, is only knowable from do_read some seconds later - the grant
	 * arrives in a network signalling message, not in the reply to the ask. */
	if (all_ok) {
		EHS_FB_FINISH(INX_cell_power_manager_ARG_do_apply_apply_ok);
	} else {
		EHS_FB_FINISH(INX_cell_power_manager_ARG_do_apply_apply_fail);
	}
}//ICB FUNCTION do_apply MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_read MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(cell_power_manager_do_read)
{
	EhsCellPsmType  psm;
	EhsCellEdrxType edrx;

	memset(&psm, 0, sizeof(psm));
	memset(&edrx, 0, sizeof(edrx));
	(void)Cell_PsmGetGranted(&psm);
	(void)Cell_EdrxGetGranted(&edrx);

	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_power_manager_ARG_do_read_psm_granted)) {
		EHS_FB_OUT_B_API2(INX_cell_power_manager_ARG_do_read_psm_granted) = psm.granted;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_power_manager_ARG_do_read_granted_tau)) {
		EHS_FB_OUT_I_API2(INX_cell_power_manager_ARG_do_read_granted_tau) = psm.tau_s;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_power_manager_ARG_do_read_granted_active)) {
		EHS_FB_OUT_I_API2(INX_cell_power_manager_ARG_do_read_granted_active) = psm.active_s;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_power_manager_ARG_do_read_edrx_granted)) {
		EHS_FB_OUT_B_API2(INX_cell_power_manager_ARG_do_read_edrx_granted) = edrx.granted;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_power_manager_ARG_do_read_granted_edrx_ms)) {
		EHS_FB_OUT_I_API2(INX_cell_power_manager_ARG_do_read_granted_edrx_ms) = edrx.cycle_ms;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_power_manager_ARG_do_read_granted_ptw_ms)) {
		EHS_FB_OUT_I_API2(INX_cell_power_manager_ARG_do_read_granted_ptw_ms) = edrx.ptw_ms;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_power_manager_ARG_do_read_rai_ok)) {
		EHS_FB_OUT_B_API2(INX_cell_power_manager_ARG_do_read_rai_ok) = s_rai_ok;
	}

	EHS_FB_FINISH(INX_cell_power_manager_ARG_do_read_read_ok);
}//ICB FUNCTION do_read MACRO END -- DO NOT ALTER THIS LINE
