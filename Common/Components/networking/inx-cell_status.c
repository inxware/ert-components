/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file inx-cell_status.c
 * cell_status function block — cellular signal quality, pre-transmit link
 * evaluation, and metered-link byte counters.
 *
 * Signal and data usage are merged into one block because both answer "how is
 * the link doing right now", both are pure reads, and on a metered link you
 * almost always want them together: bytes sent, and whether conditions
 * justified sending them.
 *
 * UNAVAILABLE READINGS ARE NOT ZERO. Every numeric output that the modem
 * cannot supply is emitted as EHS_CELL_VALUE_INVALID (32767), never 0. An RSRP
 * of 0 dBm is not plausible but it looks plausible, and a diagnostic block that
 * prints 0 for "unknown" is worse than one that prints nothing.
 *
 * @author: inx limited
 */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-cell_status.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal-api.h"
#include "hal_logger.h"
#include "cellular.h"

//ICB STATE VAR MACRO START -- DO NOT ALTER
typedef struct inx_cell_status_state
{
	ehs_sint32 mode;   /* 0 = signal only, 1 = full link evaluation */
} inx_cell_status_state_type;
//ICB STATE VAR MACRO END -- DO NOT ALTER

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(cell_status)
EHS_FB_FUNCTION_ENTRY("do_read", 0x01, cell_status_do_read)
EHS_FB_FUNCTION_ENTRY("do_reset_counters", 0x02, cell_status_do_reset_counters)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_cell_status_ARG_do_read_read_ok 1
#define INX_cell_status_ARG_do_read_rsrp 1
#define INX_cell_status_ARG_do_read_rsrq 2
#define INX_cell_status_ARG_do_read_snr 3
#define INX_cell_status_ARG_do_read_ce_level 4
#define INX_cell_status_ARG_do_read_tx_power 5
#define INX_cell_status_ARG_do_read_tx_reps 6
#define INX_cell_status_ARG_do_read_rx_reps 7
#define INX_cell_status_ARG_do_read_dl_pathloss 8
#define INX_cell_status_ARG_do_read_energy_estimate 9
#define INX_cell_status_ARG_do_read_eval_ok 10
#define INX_cell_status_ARG_do_read_tx_bytes 11
#define INX_cell_status_ARG_do_read_rx_bytes 12
#define INX_cell_status_ARG_do_reset_counters_reset_ok 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
#define INX_FB_cell_status_mode 1
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_IDENTIFY_FUNCTION(cell_status)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_cell_status_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_INIT_FUNCTION(cell_status)
{
	inx_cell_status_state_type *st = (inx_cell_status_state_type *)EHS_FB_INIT_CONTEXT;

	st->mode = INX_FB_cell_status_mode;
	EhsSscanf(EHS_FB_INIT_PARAMETERS, "%d", &(st->mode));
	return EHS_TRUE;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(cell_status)
{
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_read MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(cell_status_do_read)
{
	inx_cell_status_state_type *st = (inx_cell_status_state_type *)EHS_FB_RUN_CONTEXT;
	EhsCellSignalType   sig;
	EhsCellLinkEvalType eval;
	ehs_bool eval_ok = EHS_FALSE;
	ehs_uint32 tx = 0u, rx = 0u;

	memset(&sig, 0, sizeof(sig));
	memset(&eval, 0, sizeof(eval));

	(void)Cell_GetSignal(&sig);

	/* Full evaluation only if asked for AND supported. Cell_EvalLink already
	 * refuses when the capability is absent; it also legitimately fails while
	 * unregistered or with the radio active, which is not an error. */
	if (st->mode != 0) {
		eval_ok = Cell_EvalLink(&eval);
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_rsrp)) {
		EHS_FB_OUT_I_API2(INX_cell_status_ARG_do_read_rsrp) = (ehs_sint32)sig.rsrp_dbm;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_rsrq)) {
		EHS_FB_OUT_I_API2(INX_cell_status_ARG_do_read_rsrq) = (ehs_sint32)sig.rsrq_db;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_snr)) {
		EHS_FB_OUT_I_API2(INX_cell_status_ARG_do_read_snr) = (ehs_sint32)sig.snr_db;
	}

	/* Evaluation-only fields. When the evaluation did not run they must read
	 * INVALID, not stale or zero. */
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_ce_level)) {
		EHS_FB_OUT_I_API2(INX_cell_status_ARG_do_read_ce_level) =
			eval_ok ? (ehs_sint32)eval.ce_level : (ehs_sint32)EHS_CELL_VALUE_INVALID;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_tx_power)) {
		EHS_FB_OUT_I_API2(INX_cell_status_ARG_do_read_tx_power) =
			eval_ok ? (ehs_sint32)eval.tx_power_dbm : (ehs_sint32)EHS_CELL_VALUE_INVALID;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_tx_reps)) {
		EHS_FB_OUT_I_API2(INX_cell_status_ARG_do_read_tx_reps) =
			eval_ok ? (ehs_sint32)eval.tx_repetitions : (ehs_sint32)EHS_CELL_VALUE_INVALID;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_rx_reps)) {
		EHS_FB_OUT_I_API2(INX_cell_status_ARG_do_read_rx_reps) =
			eval_ok ? (ehs_sint32)eval.rx_repetitions : (ehs_sint32)EHS_CELL_VALUE_INVALID;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_dl_pathloss)) {
		EHS_FB_OUT_I_API2(INX_cell_status_ARG_do_read_dl_pathloss) =
			eval_ok ? (ehs_sint32)eval.dl_pathloss_db : (ehs_sint32)EHS_CELL_VALUE_INVALID;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_energy_estimate)) {
		EHS_FB_OUT_I_API2(INX_cell_status_ARG_do_read_energy_estimate) =
			eval_ok ? (ehs_sint32)eval.energy : (ehs_sint32)EhsCellEnergy_Unknown;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_eval_ok)) {
		EHS_FB_OUT_B_API2(INX_cell_status_ARG_do_read_eval_ok) = eval_ok;
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_tx_bytes) ||
	    EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_rx_bytes)) {
		(void)Cell_GetDataCounters(&tx, &rx);
		if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_tx_bytes)) {
			EHS_FB_OUT_I_API2(INX_cell_status_ARG_do_read_tx_bytes) = (ehs_sint32)tx;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_cell_status_ARG_do_read_rx_bytes)) {
			EHS_FB_OUT_I_API2(INX_cell_status_ARG_do_read_rx_bytes) = (ehs_sint32)rx;
		}
	}

	EHS_FB_FINISH(INX_cell_status_ARG_do_read_read_ok);
}//ICB FUNCTION do_read MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_reset_counters MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(cell_status_do_reset_counters)
{
	(void)Cell_ResetDataCounters();
	EHS_FB_FINISH(INX_cell_status_ARG_do_reset_counters_reset_ok);
}//ICB FUNCTION do_reset_counters MACRO END -- DO NOT ALTER THIS LINE
