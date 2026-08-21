/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_cellular.c
 * nRF91-series cellular HAL — the target half of the cross-target contract in
 * target/Component-HAL/cellular/cellular.h.
 *
 * Built on NCS's lte_lc (LTE link control) + modem_info, with raw AT via
 * nrf_modem_at_printf() for the handful of things neither wraps (APN, %CSUS
 * SIM selection, %XSIM status, +CPIN). The shared connect state machine lives
 * in Common/Components/networking/inx-cellular_link.c and is identical on
 * every port; this file only (a) implements the doCellular* primitives and
 * (b) translates lte_lc events into cbSource transitions.
 *
 *   eRT contract                  NCS
 *   ────────────────────────────  ──────────────────────────────────────────
 *   doCellularStart               nrf_modem_lib_init + modem_info_init
 *   doCellularConnect             lte_lc_system_mode_set + lte_lc_connect_async
 *   doCellularDisconnect          lte_lc_offline
 *   doCellularSetPowerState       lte_lc_func_mode_set
 *   doCellularGetRegStatus        lte_lc_nw_reg_status_get
 *   doCellularGetSignal           modem_info_get_rsrp / _snr
 *   doCellularEvalLink            lte_lc_conn_eval_params_get
 *   doCellularGetIp               modem_info_string_get(MODEM_INFO_IP_ADDRESS)
 *   doCellularSimSelect           AT%CSUS (+ CFUN cycle — see below)
 *
 * WHY A DEDICATED WORKER THREAD
 * -----------------------------
 * Same reasoning as target_wifi.c, different mechanism. Most lte_lc calls are
 * synchronous AT round-trips to the modem: lte_lc_system_mode_set() and
 * lte_lc_func_mode_set() block the caller until the modem answers, which under
 * a busy modem is tens to hundreds of milliseconds, and the CFUN cycle used
 * for a SIM-slot change is seconds. The shared SM thread must stay responsive
 * (it services the attach watchdog), and cellular.h makes every doCellular*
 * "must return promptly" a contract requirement. So the do* entry points
 * enqueue a command and return; this file's worker performs the modem calls.
 *
 * The lte_lc event handler runs on lte_lc's own workqueue context. Like the
 * Wi-Fi net_mgmt handlers it only touches flags and the SM's cbSource — no
 * blocking work, no printf, no allocation.
 *
 * A NOTE ON THE lte_lc EVENT UNION
 * --------------------------------
 * struct lte_lc_evt's union members are individually #if'd on
 * CONFIG_LTE_LC_*_MODULE in NCS v2.9.0's lte_lc.h. Reading evt->psm_cfg
 * without CONFIG_LTE_LC_PSM_MODULE is a compile error, not a runtime no-op, so
 * every optional member below carries the matching guard. Kconfig for the
 * modules we rely on is set in cellular_nrf91.mk.
 *
 * @author: inx limited
 */

/* Module-scoped logger ID. Must precede every include: several headers pull in
 * hal_logger.h, which latches EHSL_MODULE_ID to UNDEFINED if it is not set. */
#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include <zephyr/kernel.h>

#include <modem/nrf_modem_lib.h>
#include <modem/lte_lc.h>
#include <nrf_modem_at.h>

#if defined(CONFIG_MODEM_INFO)
#include <modem/modem_info.h>
#endif

#include <stdio.h>
#include <string.h>

#include "globals.h"

#ifdef EHS_NETWORK_CELLULAR_SUPPORT

#include "hal.h"
#include "hal_logger.h"
#include "hal_string.h"
#include "cellular.h"

/* Worker stack. lte_lc calls are AT round-trips through nrf_modem, which is
 * far lighter on stack than the wpa_supplicant chain the Wi-Fi worker has to
 * carry (5200 B). 2048 covers the deepest path here (modem_info's response
 * parsing with a MODEM_INFO_MAX_RESPONSE_SIZE scratch buffer), measured with
 * CONFIG_INIT_STACKS + k_thread_stack_space_get() during bring-up. */
#ifndef EHS_ZEPHYR_CELL_THREAD_STACK_SIZE
#define EHS_ZEPHYR_CELL_THREAD_STACK_SIZE 2048
#endif
#define EHS_ZEPHYR_CELL_THREAD_PRIORITY K_PRIO_PREEMPT(7)

#define EHS_ZEPHYR_CELL_CMDQ_DEPTH 8

/* AT response scratch. The longest reply we parse is +CGDCONT/%XSIM class —
 * well under this. Deliberately not MODEM_INFO_MAX_RESPONSE_SIZE-sized: this
 * lives on the worker stack. */
#define EHS_ZEPHYR_CELL_AT_BUF 64

/*****************************************************************************/
/* Worker command queue                                                      */

typedef enum {
	Cell_Cmd_Start = 0,
	Cell_Cmd_Connect,
	Cell_Cmd_Disconnect,
	Cell_Cmd_PowerState,
	Cell_Cmd_SetRat,
	Cell_Cmd_CheckIp,
	Cell_Cmd_SimSelect,
	Cell_Cmd_PinEnter
} eCellCmd;

typedef struct {
	ehs_uint8  id;
	ehs_uint8  u8;
	ehs_uint8  u8b;
	ehs_char   str[EHS_CELL_PIN_BUFF_MAX];
} sCellCmd;

K_THREAD_STACK_DEFINE(s_cell_worker_stack, EHS_ZEPHYR_CELL_THREAD_STACK_SIZE);
static struct k_thread s_cell_worker_thread;
static ehs_bool        s_cell_worker_started = EHS_FALSE;

K_MSGQ_DEFINE(s_cell_cmdq, sizeof(sCellCmd), EHS_ZEPHYR_CELL_CMDQ_DEPTH, 4);

/*****************************************************************************/
/* State                                                                     */

static volatile ehs_bool s_modem_up      = EHS_FALSE;  /* nrf_modem_lib_init done */
static volatile ehs_bool s_registered    = EHS_FALSE;
static volatile ehs_bool s_online        = EHS_FALSE;
static volatile ehs_bool s_sleeping      = EHS_FALSE;  /* PSM */
static volatile ehs_bool s_handler_bound = EHS_FALSE;

static volatile enum lte_lc_nw_reg_status s_reg = LTE_LC_NW_REG_NOT_REGISTERED;
static volatile enum lte_lc_lte_mode      s_active_mode = LTE_LC_LTE_MODE_NONE;

static EhsCellConfigType s_cfg;
static ehs_char s_cfg_apn[EHS_CELL_APN_BUFF_MAX] = {0};
static ehs_bool s_cfg_valid = EHS_FALSE;

static ehs_char s_ip[EHS_CELL_IPADDR_BUFF_MAX] = {0};

/* Serving cell, updated from LTE_LC_EVT_CELL_UPDATE. */
static volatile ehs_sint32 s_cell_mcc = 0;
static volatile ehs_sint32 s_cell_mnc = 0;
static volatile ehs_uint32 s_cell_id  = EHS_CELL_CELLID_INVALID;
static volatile ehs_uint32 s_cell_tac = 0;
/* "Not known or not detectable" as reported by AT+CESQ / AT%CONEVAL for the
 * RSRQ index. Same value as NCS's LTE_LC_CELL_RSRQ_INVALID and modem_info's
 * private CELL_RSRP_INVALID, neither of which is exported for this use. */
#define CELL_RSRQ_IDX_INVALID 255

/* %CONEVAL snr index. 127 = not known, matching NCS's SNR_UNAVAILABLE.
 *
 * The offset is 25 because lte_lc.h's own table for this field says index 1 is
 * "-24 dB <= SNR < -23 dB" (1 - 25 = -24) and index 49 is "24 dB <= SNR"
 * (49 - 25 = 24). NOTE that modem_info.h defines SNR_OFFSET_VAL as 24 for the
 * AT%XSNRSQ path used by modem_info_get_snr(), so the two SNR sources in this
 * file can differ by 1 dB. That is deliberate: each follows the documentation
 * of the command it actually reads. Do not "harmonise" them without checking
 * the Nordic AT command reference for both %CONEVAL and %XSNRSQ. */
#define CELL_SNR_IDX_INVALID  127
#define CELL_SNR_IDX_OFFSET   25

/* No s_cell_rsrp / s_cell_rsrq / s_cell_earfcn here on purpose. They used to be
 * captured from LTE_LC_EVT_CELL_UPDATE, which never carries them (see the note
 * in the handler), so they were dead state that read as valid measurements.
 * Signal strength is read live in doCellularGetSignal(). */

#if defined(CONFIG_LTE_LC_PSM_MODULE)
static volatile ehs_bool   s_psm_granted  = EHS_FALSE;
static volatile ehs_sint32 s_psm_tau      = 0;
static volatile ehs_sint32 s_psm_active   = 0;
#endif
#if defined(CONFIG_LTE_LC_EDRX_MODULE)
static volatile ehs_bool   s_edrx_granted = EHS_FALSE;
static volatile ehs_sint32 s_edrx_cycle_ms = 0;
static volatile ehs_sint32 s_edrx_ptw_ms   = 0;
#endif

static ehs_bool cell_post(eCellCmd id, ehs_uint8 a, ehs_uint8 b, const ehs_char *str);

/*****************************************************************************/
/* Modem fault handler                                                       */

#if defined(CONFIG_NRF_MODEM_LIB_ON_FAULT_APPLICATION_SPECIFIC)
/**
 * Called by nrf_modem when the modem core faults.
 *
 * CONFIG_NRF_MODEM_LIB_ON_FAULT_APPLICATION_SPECIFIC=y makes this symbol
 * MANDATORY — the library references it from its init parameters, so omitting
 * it is a link error ("undefined reference to nrf_modem_fault_handler"), which
 * is how this function came to be written. The alternative Kconfig values
 * silently reset or halt the whole system on a modem fault; we want neither
 * without saying so first.
 *
 * Runs in an interrupt context, so it does what such a handler must: records
 * the reason, tells the state machine, and returns. Recovery (a modem
 * re-init) is the SM's business on its own thread — attempting it here would
 * block an ISR on AT round-trips.
 *
 * A modem fault is not the same as losing coverage: the modem core itself has
 * stopped, and nothing will work again until it is re-initialised. Surfacing
 * it as a distinct failure reason is the difference between "no signal here"
 * and "this unit needs attention".
 */
void nrf_modem_fault_handler(struct nrf_modem_fault_info *fault_info)
{
	s_modem_up   = EHS_FALSE;
	s_registered = EHS_FALSE;
	s_online     = EHS_FALSE;
	s_sleeping   = EHS_FALSE;

	EhsCellSetFailReason(EhsCellFail_Internal);
	EhsCellSetCBSource(eCellCallbackSource_LinkLost);

	/* Deliberately printk, not EHSH_LOG_*: this is ISR context and the eRT
	 * logger formats onto the caller's stack. The fault reason and PC are the
	 * only two things Nordic support will ask for, so they must survive even
	 * if nothing else does. */
	if (fault_info != NULL) {
		printk("MODEM FAULT: reason 0x%08X pc 0x%08X\n",
		       (unsigned int)fault_info->reason,
		       (unsigned int)fault_info->program_counter);
	} else {
		printk("MODEM FAULT: (no info)\n");
	}
}
#endif /* CONFIG_NRF_MODEM_LIB_ON_FAULT_APPLICATION_SPECIFIC */

/*****************************************************************************/
/* Enum translation                                                          */

static enum lte_lc_system_mode rat_to_lte_lc(EhsCellRat rat)
{
	const ehs_bool m = ((rat & EHS_CELL_RAT_LTEM) != 0u) ? EHS_TRUE : EHS_FALSE;
	const ehs_bool n = ((rat & EHS_CELL_RAT_NBIOT) != 0u) ? EHS_TRUE : EHS_FALSE;

	if (m && n) {
		return LTE_LC_SYSTEM_MODE_LTEM_NBIOT;
	}
	if (n) {
		return LTE_LC_SYSTEM_MODE_NBIOT;
	}
	return LTE_LC_SYSTEM_MODE_LTEM;   /* default and the m-only case */
}

static EhsCellRat lte_lc_to_rat(enum lte_lc_system_mode mode)
{
	switch (mode) {
	case LTE_LC_SYSTEM_MODE_LTEM:
	case LTE_LC_SYSTEM_MODE_LTEM_GPS:
		return EHS_CELL_RAT_LTEM;
	case LTE_LC_SYSTEM_MODE_NBIOT:
	case LTE_LC_SYSTEM_MODE_NBIOT_GPS:
		return EHS_CELL_RAT_NBIOT;
	case LTE_LC_SYSTEM_MODE_LTEM_NBIOT:
	case LTE_LC_SYSTEM_MODE_LTEM_NBIOT_GPS:
		return (EhsCellRat)(EHS_CELL_RAT_LTEM | EHS_CELL_RAT_NBIOT);
	default:
		return EHS_CELL_RAT_NONE;
	}
}

static enum lte_lc_system_mode_preference pref_to_lte_lc(EhsCellRatPref p)
{
	switch (p) {
	case EHS_CELL_RAT_PREF_LTEM:            return LTE_LC_SYSTEM_MODE_PREFER_LTEM;
	case EHS_CELL_RAT_PREF_NBIOT:           return LTE_LC_SYSTEM_MODE_PREFER_NBIOT;
	case EHS_CELL_RAT_PREF_LTEM_PLMN_PRIO:  return LTE_LC_SYSTEM_MODE_PREFER_LTEM_PLMN_PRIO;
	case EHS_CELL_RAT_PREF_NBIOT_PLMN_PRIO: return LTE_LC_SYSTEM_MODE_PREFER_NBIOT_PLMN_PRIO;
	case EHS_CELL_RAT_PREF_AUTO:
	default:                                return LTE_LC_SYSTEM_MODE_PREFER_AUTO;
	}
}

static EhsCellRatPref pref_from_lte_lc(enum lte_lc_system_mode_preference p)
{
	switch (p) {
	case LTE_LC_SYSTEM_MODE_PREFER_LTEM:            return EHS_CELL_RAT_PREF_LTEM;
	case LTE_LC_SYSTEM_MODE_PREFER_NBIOT:           return EHS_CELL_RAT_PREF_NBIOT;
	case LTE_LC_SYSTEM_MODE_PREFER_LTEM_PLMN_PRIO:  return EHS_CELL_RAT_PREF_LTEM_PLMN_PRIO;
	case LTE_LC_SYSTEM_MODE_PREFER_NBIOT_PLMN_PRIO: return EHS_CELL_RAT_PREF_NBIOT_PLMN_PRIO;
	case LTE_LC_SYSTEM_MODE_PREFER_AUTO:
	default:                                        return EHS_CELL_RAT_PREF_AUTO;
	}
}

/* lte_lc uses the 3GPP +CEREG numbering, and so does EhsCellRegStatus — the
 * mapping is identity. It is written out rather than cast so that a future
 * divergence in either enum is a compile-time switch warning, not a silent
 * mis-report. */
static EhsCellRegStatus reg_from_lte_lc(enum lte_lc_nw_reg_status r)
{
	switch (r) {
	case LTE_LC_NW_REG_NOT_REGISTERED:      return EhsCellReg_NotRegistered;
	case LTE_LC_NW_REG_REGISTERED_HOME:     return EhsCellReg_RegisteredHome;
	case LTE_LC_NW_REG_SEARCHING:           return EhsCellReg_Searching;
	case LTE_LC_NW_REG_REGISTRATION_DENIED: return EhsCellReg_Denied;
	case LTE_LC_NW_REG_REGISTERED_ROAMING:  return EhsCellReg_RegisteredRoaming;
	case LTE_LC_NW_REG_UICC_FAIL:           return EhsCellReg_UiccFail;
	case LTE_LC_NW_REG_UNKNOWN:
	default:                                return EhsCellReg_Unknown;
	}
}

/*****************************************************************************/
/* lte_lc event handler — runs on lte_lc's workqueue. Flags and cbSource only. */

static void cell_lte_handler(const struct lte_lc_evt *const evt)
{
	if (evt == NULL) {
		return;
	}

	switch (evt->type) {
	case LTE_LC_EVT_NW_REG_STATUS:
		s_reg = evt->nw_reg_status;

		switch (evt->nw_reg_status) {
		case LTE_LC_NW_REG_REGISTERED_HOME:
		case LTE_LC_NW_REG_REGISTERED_ROAMING:
			s_registered = EHS_TRUE;
			EhsCellSetCBSource(eCellCallbackSource_RegStatus);
			/* The default PDN comes up as part of attach on nRF91, but the
			 * address is not readable from this context — ask the worker. */
			(void)cell_post(Cell_Cmd_CheckIp, 0u, 0u, NULL);
			break;

		case LTE_LC_NW_REG_UICC_FAIL:
			s_registered = EHS_FALSE;
			s_online     = EHS_FALSE;
			EhsCellSetFailReason(EhsCellFail_SimFailure);
			EhsCellSetCBSource(eCellCallbackSource_SimFail);
			break;

		case LTE_LC_NW_REG_REGISTRATION_DENIED:
			s_registered = EHS_FALSE;
			s_online     = EHS_FALSE;
			EhsCellSetFailReason(EhsCellFail_RegistrationDenied);
			EhsCellSetCBSource(eCellCallbackSource_RegStatus);
			break;

		default:
			if (s_registered) {
				s_registered = EHS_FALSE;
				s_online     = EHS_FALSE;
				EhsCellSetCBSource(eCellCallbackSource_LinkLost);
			} else {
				EhsCellSetCBSource(eCellCallbackSource_RegStatus);
			}
			break;
		}
		break;

	case LTE_LC_EVT_LTE_MODE_UPDATE:
		/* The modem can switch between LTE-M and NB-IoT under us when both are
		 * enabled. Recorded so the SM picks the right attach deadline. */
		s_active_mode = evt->lte_mode;
		break;

	case LTE_LC_EVT_CELL_UPDATE:
		/* mcc/mnc are copied for completeness but are ALWAYS 0 here: this event
		 * is built from +CEREG (see NCS lte_link_control/modules/cereg.c, which
		 * contains no reference to mcc or mnc at all), and +CEREG reports
		 * <tac>,<ci>,<AcT> without the PLMN. The other lte_lc paths that do fill
		 * these - ncellmeas, coneval, rai - are not this one. The real values
		 * are derived from AT+COPS? in doCellularGetCellInfo(). */
		/* ONLY id AND tac ARE REAL HERE. NCS builds this event in
		 * lte_link_control/modules/cereg.c: parse_cereg() memsets the whole
		 * lte_lc_cell and then assigns exactly two fields, id and tac, because
		 * +CEREG reports <tac>,<ci>,<AcT> and nothing else. mcc, mnc, earfcn,
		 * rsrp and rsrq stay at the memset zero on this path - the lte_lc
		 * modules that do fill them (ncellmeas, coneval, rai) raise different
		 * events.
		 *
		 * Reading them here produced two silent wrong answers: MCC/MNC 0/0, and
		 * an RSRQ of 0 that never matched the INVALID sentinel (255) and so was
		 * published as a plausible "0 dB". Both are now sourced where the data
		 * actually lives - doCellularGetCellInfo() derives MCC/MNC from AT+COPS?
		 * and doCellularGetSignal() reads AT+CESQ. Do not reinstate them. */
		s_cell_id     = (ehs_uint32)evt->cell.id;
		s_cell_tac    = (ehs_uint32)evt->cell.tac;
		break;

#if defined(CONFIG_LTE_LC_PSM_MODULE)
	case LTE_LC_EVT_PSM_UPDATE:
		/* GRANTED values, not what we asked for — see EhsCellPsmType. A
		 * negative TAU means the network refused PSM. */
		s_psm_tau     = (ehs_sint32)evt->psm_cfg.tau;
		s_psm_active  = (ehs_sint32)evt->psm_cfg.active_time;
		s_psm_granted = (evt->psm_cfg.tau >= 0) ? EHS_TRUE : EHS_FALSE;
		break;
#endif

#if defined(CONFIG_LTE_LC_EDRX_MODULE)
	case LTE_LC_EVT_EDRX_UPDATE:
		s_edrx_cycle_ms = (ehs_sint32)(evt->edrx_cfg.edrx * 1000.0f);
		s_edrx_ptw_ms   = (ehs_sint32)(evt->edrx_cfg.ptw * 1000.0f);
		s_edrx_granted  = (evt->edrx_cfg.edrx > 0.0f) ? EHS_TRUE : EHS_FALSE;
		break;
#endif

#if defined(CONFIG_LTE_LC_MODEM_SLEEP_MODULE)
	case LTE_LC_EVT_MODEM_SLEEP_ENTER:
		/* Registered but asleep. NOT a disconnect — the SM models this as
		 * EhsCell_PsmSleep and must not try to reconnect. */
		s_sleeping = EHS_TRUE;
		EhsCellSetCBSource(eCellCallbackSource_Psm);
		break;

	case LTE_LC_EVT_MODEM_SLEEP_EXIT:
		s_sleeping = EHS_FALSE;
		EhsCellSetCBSource(eCellCallbackSource_Psm);
		break;
#endif

	case LTE_LC_EVT_MODEM_EVENT:
		switch (evt->modem_evt) {
		case LTE_LC_MODEM_EVT_RESET_LOOP:
			/* The modem has decided we are power-cycling it too often and
			 * will refuse to attach for ~30 minutes. Surfacing this matters:
			 * silently swallowing it produces a device that looks dead for
			 * half an hour with no explanation. */
			EhsCellSetFailReason(EhsCellFail_ModemResetLoop);
			break;
		case LTE_LC_MODEM_EVT_OVERHEATED:
			EhsCellSetFailReason(EhsCellFail_Overheated);
			break;
		case LTE_LC_MODEM_EVT_NO_IMEI:
			EhsCellSetFailReason(EhsCellFail_Internal);
			break;
		case LTE_LC_MODEM_EVT_SEARCH_DONE:
			/* Search finished with no registration — the SM's NoCoverage
			 * path handles the backoff. */
			if (!s_registered) {
				EhsCellSetFailReason(EhsCellFail_NoCoverage);
			}
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
}

/*****************************************************************************/
/* Worker                                                                    */

static void worker_start(void)
{
	int err;

	if (s_modem_up) {
		EhsCellSetCBSource(eCellCallbackSource_ModemReady);
		return;
	}

	err = nrf_modem_lib_init();
	if (err != 0) {
		EHSH_LOG_ERROR("Cellular: nrf_modem_lib_init failed (%d)", err);
		EhsCellSetFailReason(EhsCellFail_ModemInit);
		return;
	}

#if defined(CONFIG_MODEM_INFO)
	err = modem_info_init();
	if (err != 0) {
		/* Not fatal: identity/diagnostics degrade, the link still works. */
		EHSH_LOG_WARNING("Cellular: modem_info_init failed (%d)", err);
	}
#endif

	if (!s_handler_bound) {
		lte_lc_register_handler(cell_lte_handler);
		s_handler_bound = EHS_TRUE;
	}

	/* Modem events carry RESET_LOOP / OVERHEATED / NO_IMEI, all of which are
	 * things we want to report rather than discover as an unexplained
	 * failure to attach. */
	(void)lte_lc_modem_events_enable();

	/* POWER THE UICC BEFORE ANYONE ASKS ABOUT THE SIM.
	 *
	 * nrf_modem_lib_init() leaves the modem at CFUN=0, which means the UICC is
	 * UNPOWERED. %XSIM then answers 0 and every SIM query reports ABSENT —
	 * whether or not a card is actually in the socket. First hardware run hit
	 * exactly that: IMEI and modem firmware read back fine (so the modem was
	 * demonstrably working) while the SIM read as absent with a card fitted.
	 *
	 * CFUN=41 activates the UICC without bringing the radio up, so the SIM is
	 * readable during SimWait and a missing card is diagnosed in seconds
	 * instead of after a full attach attempt. Not fatal if it fails: the
	 * subsequent CFUN=1 in lte_lc_connect_async() powers the UICC anyway, so
	 * the only loss is the early diagnosis. */
	err = lte_lc_func_mode_set(LTE_LC_FUNC_MODE_ACTIVATE_UICC);
	if (err != 0) {
		EHSH_LOG_WARNING("Cellular: could not pre-activate the UICC (%d); SIM "
		                 "state will not be readable until attach", err);
	}

	s_modem_up = EHS_TRUE;
	EHSH_LOG_INFO("Cellular: modem library up");

	/* ModemReady ONLY. Do not also assert SimReady here — whether a SIM is
	 * present is something to be read, not assumed, and the shared SM polls
	 * doCellularSimGetState() throughout SimWait to find out. */
	EhsCellSetCBSource(eCellCallbackSource_ModemReady);
}

static void worker_apply_apn(void)
{
	int err;

	if (!s_cfg_valid || (s_cfg_apn[0] == '\0')) {
		return;   /* network-assigned APN — normal and correct on most networks */
	}

	/* Configure the default PDN context. Raw AT rather than the pdn library:
	 * this is the only thing we would use it for, and it saves the whole
	 * CONFIG_PDN subsystem on a part where flash is shared with the app. */
	err = nrf_modem_at_printf("AT+CGDCONT=0,\"IP\",\"%s\"", s_cfg_apn);
	if (err != 0) {
		EHSH_LOG_WARNING("Cellular: setting APN '%s' failed (%d)", s_cfg_apn, err);
	} else {
		EHSH_LOG_INFO("Cellular: APN set to '%s'", s_cfg_apn);
	}
}

static void worker_connect(void)
{
	int err;

	if (!s_modem_up) {
		EhsCellSetFailReason(EhsCellFail_ModemInit);
		return;
	}

	if (s_cfg_valid && (s_cfg.rat != EHS_CELL_RAT_NONE)) {
		err = lte_lc_system_mode_set(rat_to_lte_lc(s_cfg.rat),
		                             pref_to_lte_lc(s_cfg.preference));
		if (err != 0) {
			EHSH_LOG_WARNING("Cellular: system mode set failed (%d)", err);
		}
	}

	/* APN must be configured while the radio is off, before attach. */
	worker_apply_apn();

#if defined(CONFIG_LTE_LC_PSM_MODULE)
	if (s_cfg_valid && s_cfg.request_psm) {
		(void)lte_lc_psm_param_set_seconds((int)s_cfg.psm_tau_s,
		                                   (int)s_cfg.psm_active_s);
		(void)lte_lc_psm_req(true);
	}
#endif
#if defined(CONFIG_LTE_LC_EDRX_MODULE)
	if (s_cfg_valid && s_cfg.request_edrx) {
		(void)lte_lc_edrx_req(true);
	}
#endif

	err = lte_lc_connect_async(cell_lte_handler);
	if (err != 0) {
		EHSH_LOG_ERROR("Cellular: lte_lc_connect_async failed (%d)", err);
		EhsCellSetFailReason(EhsCellFail_Internal);
		return;
	}
	EHSH_LOG_INFO("Cellular: attach started");
}

static void worker_check_ip(void)
{
#if defined(CONFIG_MODEM_INFO)
	ehs_char buf[EHS_CELL_IPADDR_BUFF_MAX];
	int len;

	buf[0] = '\0';
	len = modem_info_string_get(MODEM_INFO_IP_ADDRESS, buf, sizeof(buf));
	if ((len > 0) && (buf[0] != '\0')) {
		(void)EhsStrncpy_s(s_ip, sizeof(s_ip), buf, sizeof(s_ip) - 1u);
		s_online = EHS_TRUE;
		EhsCellSetCBSource(eCellCallbackSource_GotIp);
		return;
	}
	/* No address yet. The SM's Registered-state PDN deadline covers the case
	 * where one never arrives; nothing to do here but wait for the next
	 * registration event. */
#else
	/* Without modem_info there is no way to read the assigned address. Treat
	 * registration as online — the link works, only the reported IP is
	 * unavailable. */
	s_ip[0] = '\0';
	s_online = EHS_TRUE;
	EhsCellSetCBSource(eCellCallbackSource_GotIp);
#endif
}

static void worker_disconnect(void)
{
	int err = lte_lc_offline();

	if (err != 0) {
		EHSH_LOG_WARNING("Cellular: lte_lc_offline failed (%d)", err);
	}
	s_registered = EHS_FALSE;
	s_online     = EHS_FALSE;
	s_sleeping   = EHS_FALSE;
	s_ip[0]      = '\0';
}

static void worker_power_state(EhsCellPowerState state)
{
	enum lte_lc_func_mode mode;
	int err;

	switch (state) {
	case EHS_CELL_POWER_OFF:       mode = LTE_LC_FUNC_MODE_POWER_OFF;      break;
	case EHS_CELL_POWER_ON:        mode = LTE_LC_FUNC_MODE_NORMAL;         break;
	case EHS_CELL_POWER_AIRPLANE:  mode = LTE_LC_FUNC_MODE_OFFLINE;        break;
	case EHS_CELL_POWER_UICC_ONLY: mode = LTE_LC_FUNC_MODE_OFFLINE_UICC_ON; break;
	default:                       return;
	}

	err = lte_lc_func_mode_set(mode);
	if (err != 0) {
		EHSH_LOG_WARNING("Cellular: func mode %d failed (%d)", (int)mode, err);
	}
}

static void worker_set_rat(EhsCellRat rat, EhsCellRatPref pref)
{
	int err = lte_lc_system_mode_set(rat_to_lte_lc(rat), pref_to_lte_lc(pref));

	if (err != 0) {
		EHSH_LOG_WARNING("Cellular: system mode set failed (%d)", err);
	}
}

static void worker_sim_select(EhsSimSlot slot)
{
	/* %CSUS only takes effect across a modem re-initialisation, so the radio
	 * has to be cycled around it. Setting it live and expecting it to apply is
	 * the standard first mistake with this command. */
	int csus;
	int err;

	switch (slot) {
	case EHS_SIM_SLOT_PHYSICAL:  csus = 0; break;   /* the UICC interface */
	case EHS_SIM_SLOT_SOFT:      csus = 2; break;
	case EHS_SIM_SLOT_EMBEDDED:
		/* Reachable only if a caller bypassed doCellularSimSelect's own check.
		 * Selecting %CSUS=0 here would "succeed" while actually landing on
		 * whichever device the card-detect autoswitch has routed - i.e. it
		 * would lie. Refuse instead. */
		EHSH_LOG_WARNING("Cellular: eSIM is not runtime-selectable on this board");
		return;
	default:                     return;
	}

	(void)lte_lc_func_mode_set(LTE_LC_FUNC_MODE_POWER_OFF);
	err = nrf_modem_at_printf("AT%%CSUS=%d", csus);
	if (err != 0) {
		EHSH_LOG_ERROR("Cellular: AT%%CSUS=%d failed (%d)", csus, err);
	}
	(void)lte_lc_func_mode_set(LTE_LC_FUNC_MODE_NORMAL);
}

static void worker_pin_enter(const ehs_char *pin)
{
	int err = nrf_modem_at_printf("AT+CPIN=\"%s\"", pin);

	if (err != 0) {
		/* Do NOT retry here or anywhere up the stack — three wrong PINs need
		 * a PUK. cellular.h and inx-cellular_link.c both enforce this; the
		 * comment is repeated because this is the call site that would do the
		 * damage. */
		EHSH_LOG_ERROR("Cellular: PIN rejected (%d) — NOT retrying", err);
		EhsCellSetFailReason(EhsCellFail_SimPinRequired);
		EhsCellSetCBSource(eCellCallbackSource_SimPin);
		return;
	}
	EhsCellSetCBSource(eCellCallbackSource_SimReady);
}

static void cell_worker_entry(void *p1, void *p2, void *p3)
{
	sCellCmd cmd;

	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	for (;;) {
		if (k_msgq_get(&s_cell_cmdq, &cmd, K_FOREVER) != 0) {
			continue;
		}

		switch ((eCellCmd)cmd.id) {
		case Cell_Cmd_Start:      worker_start();                        break;
		case Cell_Cmd_Connect:    worker_connect();                      break;
		case Cell_Cmd_Disconnect: worker_disconnect();                   break;
		case Cell_Cmd_PowerState: worker_power_state((EhsCellPowerState)cmd.u8); break;
		case Cell_Cmd_SetRat:     worker_set_rat((EhsCellRat)cmd.u8,
		                                         (EhsCellRatPref)cmd.u8b); break;
		case Cell_Cmd_CheckIp:    worker_check_ip();                     break;
		case Cell_Cmd_SimSelect:  worker_sim_select((EhsSimSlot)cmd.u8); break;
		case Cell_Cmd_PinEnter:   worker_pin_enter(cmd.str);             break;
		default:                                                         break;
		}
	}
}

static ehs_bool cell_post(eCellCmd id, ehs_uint8 a, ehs_uint8 b, const ehs_char *str)
{
	sCellCmd cmd;

	memset(&cmd, 0, sizeof(cmd));
	cmd.id  = (ehs_uint8)id;
	cmd.u8  = a;
	cmd.u8b = b;
	if (str != NULL) {
		(void)EhsStrncpy_s(cmd.str, sizeof(cmd.str), str, sizeof(cmd.str) - 1u);
	}

	/* Never block: the SM posts these from its 1 s tick, and a full queue
	 * means a request is already in flight. */
	if (k_msgq_put(&s_cell_cmdq, &cmd, K_NO_WAIT) != 0) {
		EHSH_LOG_WARNING("Cellular: command queue full; dropped cmd %d", (int)id);
		return EHS_FALSE;
	}
	return EHS_TRUE;
}

/*****************************************************************************/
/* Target HAL contract — see target/Component-HAL/cellular/cellular.h        */

ehs_uint32 doCellularGetCapabilities(void)
{
	ehs_uint32 caps = (ehs_uint32)EHS_CELL_CAP_LTEM
	                | (ehs_uint32)EHS_CELL_CAP_NBIOT
	                | (ehs_uint32)EHS_CELL_CAP_GNSS_SHARED
	                | (ehs_uint32)EHS_CELL_CAP_TLS_OFFLOAD
	                | (ehs_uint32)EHS_CELL_CAP_SOFTSIM
	                /* At least one type IS runtime-selectable: %CSUS switches
	                 * between the UICC interface and SoftSIM. Physical-vs-
	                 * embedded is decided by the board's card-detect autoswitch,
	                 * which is why doCellularSimGetSelectableSlots() below is
	                 * narrower than doCellularSimGetAvailableSlots(). */
	                | (ehs_uint32)EHS_CELL_CAP_SIM_SELECT;

#if defined(CONFIG_LTE_LC_PSM_MODULE)
	caps |= (ehs_uint32)EHS_CELL_CAP_PSM;
#endif
#if defined(CONFIG_LTE_LC_EDRX_MODULE)
	caps |= (ehs_uint32)EHS_CELL_CAP_EDRX;
#endif
#if defined(CONFIG_LTE_LC_RAI_MODULE)
	caps |= (ehs_uint32)EHS_CELL_CAP_RAI;
#endif
#if defined(CONFIG_LTE_LC_CONN_EVAL_MODULE)
	caps |= (ehs_uint32)EHS_CELL_CAP_CONN_EVAL
	     |  (ehs_uint32)EHS_CELL_CAP_ENERGY_ESTIMATE
	     |  (ehs_uint32)EHS_CELL_CAP_CE_LEVEL;
#endif
#if defined(CONFIG_LTE_LC_NEIGHBOR_CELL_MEAS_MODULE)
	caps |= (ehs_uint32)EHS_CELL_CAP_NEIGHBOUR_MEAS;
#endif
#if defined(CONFIG_MODEM_INFO)
	caps |= (ehs_uint32)EHS_CELL_CAP_DATA_COUNTERS;
#endif

	/* Deliberately NOT set:
	 *  EHS_CELL_CAP_SIM_PIN — the PIN path exists (doCellularSimPinEnter) but
	 *    has had no hardware validation with a PIN-locked SIM, and advertising
	 *    it would let callers drive it. Enable once tested.
	 *  EHS_CELL_CAP_EUICC_PROFILES — no eUICC populated (footprint U15 is
	 *    unpopulated on stock hardware).
	 *  EHS_CELL_CAP_BAND_MASK / OPERATOR_SCAN — not implemented. */
	return caps;
}

ehs_bool doCellularInit(void)
{
	if (s_cell_worker_started) {
		return EHS_TRUE;   /* idempotent */
	}

	(void)k_thread_create(&s_cell_worker_thread, s_cell_worker_stack,
	                      K_THREAD_STACK_SIZEOF(s_cell_worker_stack),
	                      cell_worker_entry, NULL, NULL, NULL,
	                      EHS_ZEPHYR_CELL_THREAD_PRIORITY, 0, K_NO_WAIT);
	k_thread_name_set(&s_cell_worker_thread, "ehs_cell");
	s_cell_worker_started = EHS_TRUE;
	return EHS_TRUE;
}

void doCellularDestroy(void)
{
	(void)cell_post(Cell_Cmd_Disconnect, 0u, 0u, NULL);
}

ehs_bool doCellularStart(void)
{
	return cell_post(Cell_Cmd_Start, 0u, 0u, NULL);
}

ehs_bool doCellularConnect(const EhsCellConfigType *cfg)
{
	if (cfg != NULL) {
		s_cfg = *cfg;
		s_cfg_apn[0] = '\0';
		if (cfg->apn != NULL) {
			(void)EhsStrncpy_s(s_cfg_apn, sizeof(s_cfg_apn), cfg->apn,
			                   sizeof(s_cfg_apn) - 1u);
		}
		s_cfg.apn   = s_cfg_apn;
		s_cfg_valid = EHS_TRUE;
	}
	return cell_post(Cell_Cmd_Connect, 0u, 0u, NULL);
}

ehs_bool doCellularDisconnect(void)
{
	return cell_post(Cell_Cmd_Disconnect, 0u, 0u, NULL);
}

ehs_bool doCellularSetPowerState(EhsCellPowerState state)
{
	return cell_post(Cell_Cmd_PowerState, (ehs_uint8)state, 0u, NULL);
}

ehs_bool doCellularGetPowerState(EhsCellPowerState *out)
{
	enum lte_lc_func_mode mode;

	if (out == NULL) {
		return EHS_FALSE;
	}
	if (!s_modem_up || (lte_lc_func_mode_get(&mode) != 0)) {
		return EHS_FALSE;
	}

	switch (mode) {
	case LTE_LC_FUNC_MODE_POWER_OFF:       *out = EHS_CELL_POWER_OFF;       break;
	case LTE_LC_FUNC_MODE_NORMAL:          *out = EHS_CELL_POWER_ON;        break;
	case LTE_LC_FUNC_MODE_OFFLINE:         *out = EHS_CELL_POWER_AIRPLANE;  break;
	case LTE_LC_FUNC_MODE_OFFLINE_UICC_ON: *out = EHS_CELL_POWER_UICC_ONLY; break;
	default:                               *out = EHS_CELL_POWER_ON;        break;
	}
	return EHS_TRUE;
}

ehs_bool doCellularSetRat(EhsCellRat rat, EhsCellRatPref preference)
{
	return cell_post(Cell_Cmd_SetRat, (ehs_uint8)rat, (ehs_uint8)preference, NULL);
}

ehs_bool doCellularGetRat(EhsCellRat *rat, EhsCellRatPref *preference)
{
	enum lte_lc_system_mode mode;
	enum lte_lc_system_mode_preference pref;

	if (!s_modem_up || (lte_lc_system_mode_get(&mode, &pref) != 0)) {
		return EHS_FALSE;
	}
	if (rat != NULL) {
		*rat = lte_lc_to_rat(mode);
	}
	if (preference != NULL) {
		*preference = pref_from_lte_lc(pref);
	}
	return EHS_TRUE;
}

ehs_bool doCellularGetActiveRat(EhsCellRat *out)
{
	if (out == NULL) {
		return EHS_FALSE;
	}
	switch (s_active_mode) {
	case LTE_LC_LTE_MODE_LTEM:  *out = EHS_CELL_RAT_LTEM;  return EHS_TRUE;
	case LTE_LC_LTE_MODE_NBIOT: *out = EHS_CELL_RAT_NBIOT; return EHS_TRUE;
	default:                    *out = EHS_CELL_RAT_NONE;  return EHS_FALSE;
	}
}

ehs_bool doCellularSetBandMask(ehs_uint64 mask)
{
	(void)mask;
	return EHS_FALSE;   /* not implemented — capability bit is clear */
}

ehs_bool doCellularGetRegStatus(EhsCellRegStatus *out)
{
	if (out == NULL) {
		return EHS_FALSE;
	}
	*out = reg_from_lte_lc(s_reg);
	return EHS_TRUE;
}

ehs_bool doCellularGetIp(ehs_char *buf, ehs_uint16 buf_size)
{
	if ((buf == NULL) || (buf_size == 0u)) {
		return EHS_FALSE;
	}
	(void)EhsStrncpy_s(buf, buf_size, s_ip, (ehs_uint32)(buf_size - 1u));
	return (s_ip[0] != '\0') ? EHS_TRUE : EHS_FALSE;
}

ehs_bool doCellularGetSignal(EhsCellSignalType *out)
{
	if (out == NULL) {
		return EHS_FALSE;
	}

	/* Everything starts INVALID and is only overwritten by a source that
	 * genuinely measured it. The previous code seeded rsrp/rsrq from the
	 * CELL_UPDATE statics, which are fed by +CEREG - and parse_cereg() memsets
	 * the whole lte_lc_cell and then assigns ONLY id and tac. So rsrq arrived
	 * here as a hard 0 that never matched the INVALID sentinel (255) and was
	 * published as a perfectly plausible "0 dB". */
	out->rsrp_dbm = EHS_CELL_VALUE_INVALID;
	out->rsrq_db  = EHS_CELL_VALUE_INVALID;
	out->snr_db   = EHS_CELL_VALUE_INVALID;
	out->rssi_dbm = EHS_CELL_VALUE_INVALID;

#if defined(CONFIG_MODEM_INFO)
	{
		int v = 0;

		/* Both of these return real units, not indices: modem_info applies
		 * RSRP_IDX_TO_DBM / the SNR offset internally and returns -ENOENT when
		 * the measurement is unavailable. */
		if (modem_info_get_rsrp(&v) == 0) {
			out->rsrp_dbm = (ehs_sint16)v;
		}
		if (modem_info_get_snr(&v) == 0) {
			out->snr_db = (ehs_sint16)v;
		}

		/* NCS has no modem_info_get_rsrq(), so read AT+CESQ directly. Field
		 * order is <rxlev>,<ber>,<rscp>,<ecno>,<rsrq>,<rsrp> - RSRQ is the 5th,
		 * which is why modem_info_get_rsrp() skips five and takes the sixth.
		 * 255 means "not detectable"; the index is converted to dB and rounded
		 * to the nearest whole dB because the port is an integer. */
		{
			int rsrq_idx = 0;

			if ((nrf_modem_at_scanf("AT+CESQ",
			                        "+CESQ: %*d,%*d,%*d,%*d,%d,%*d",
			                        &rsrq_idx) == 1) &&
			    (rsrq_idx != CELL_RSRQ_IDX_INVALID)) {
				float db = RSRQ_IDX_TO_DB(rsrq_idx);

				out->rsrq_db = (ehs_sint16)((db < 0.0f) ? (db - 0.5f) : (db + 0.5f));
			}
		}
	}
#endif
	return (out->rsrp_dbm != EHS_CELL_VALUE_INVALID) ? EHS_TRUE : EHS_FALSE;
}

ehs_bool doCellularEvalLink(EhsCellLinkEvalType *out)
{
#if defined(CONFIG_LTE_LC_CONN_EVAL_MODULE)
	struct lte_lc_conn_eval_params p;

	if (out == NULL) {
		return EHS_FALSE;
	}
	memset(&p, 0, sizeof(p));
	if (lte_lc_conn_eval_params_get(&p) != 0) {
		return EHS_FALSE;
	}

	/* CONVERT - do not assign these raw. lte_lc_conn_eval_params reports rsrp,
	 * rsrq and snr as INDICES, not as dBm/dB (see the tables on those fields in
	 * lte_lc.h), while the fields they land in are named _dbm / _db and are read
	 * as real units by every caller. Assigning raw published an RSRP index of
	 * ~60 as "60 dBm" - a positive number for a signal that is always negative.
	 * The index-to-unit macros are modem_info's; snr carries its own offset. */
	out->signal.rsrp_dbm = (p.rsrp == LTE_LC_CELL_RSRP_INVALID)
	                       ? EHS_CELL_VALUE_INVALID
	                       : (ehs_sint16)RSRP_IDX_TO_DBM((int)p.rsrp);
	if (p.rsrq == (int16_t)CELL_RSRQ_IDX_INVALID) {
		out->signal.rsrq_db = EHS_CELL_VALUE_INVALID;
	} else {
		float db = RSRQ_IDX_TO_DB((int)p.rsrq);

		out->signal.rsrq_db = (ehs_sint16)((db < 0.0f) ? (db - 0.5f) : (db + 0.5f));
	}
	/* SNR index 127 = "not known or not detectable"; otherwise dB = index - 24. */
	out->signal.snr_db   = (p.snr == CELL_SNR_IDX_INVALID)
	                       ? EHS_CELL_VALUE_INVALID
	                       : (ehs_sint16)((int)p.snr - CELL_SNR_IDX_OFFSET);
	out->signal.rssi_dbm = EHS_CELL_VALUE_INVALID;
	out->ce_level        = (p.ce_level == LTE_LC_CE_LEVEL_UNKNOWN)
	                       ? EHS_CELL_VALUE_INVALID : (ehs_sint16)p.ce_level;
	out->tx_power_dbm    = (ehs_sint16)p.tx_power;
	out->tx_repetitions  = (ehs_sint16)p.tx_rep;
	out->rx_repetitions  = (ehs_sint16)p.rx_rep;
	out->dl_pathloss_db  = (ehs_sint16)p.dl_pathloss;
	out->band            = (ehs_sint16)p.band;

	switch (p.energy_estimate) {
	case LTE_LC_ENERGY_CONSUMPTION_EXCESSIVE: out->energy = EhsCellEnergy_Excessive; break;
	case LTE_LC_ENERGY_CONSUMPTION_INCREASED: out->energy = EhsCellEnergy_Increased; break;
	case LTE_LC_ENERGY_CONSUMPTION_NORMAL:    out->energy = EhsCellEnergy_Normal;    break;
	case LTE_LC_ENERGY_CONSUMPTION_REDUCED:   out->energy = EhsCellEnergy_Reduced;   break;
	case LTE_LC_ENERGY_CONSUMPTION_EFFICIENT: out->energy = EhsCellEnergy_Efficient; break;
	default:                                  out->energy = EhsCellEnergy_Unknown;   break;
	}
	return EHS_TRUE;
#else
	(void)out;
	return EHS_FALSE;
#endif
}

/**
 * Split a PLMN string into its MCC and MNC parts.
 *
 * The PLMN is 5 or 6 digits: a 3-digit MCC followed by a 2- or 3-digit MNC.
 * "23410" -> 234 / 10 (O2 UK); "310260" -> 310 / 260 (T-Mobile US).
 *
 * CAVEAT ON LEADING ZEROS. As integers, MNC "01" and "1" are indistinguishable,
 * and 3GPP treats them as different networks. That is a real (if rare) loss,
 * inherent in the ports being integers; a caller that must round-trip the exact
 * MNC should use the operator port, which carries the PLMN string verbatim.
 */
static ehs_bool plmn_split(const ehs_char *plmn, ehs_sint32 *mcc, ehs_sint32 *mnc)
{
	ehs_uint32 len = 0u;
	ehs_uint32 i;
	ehs_sint32 m = 0;
	ehs_sint32 n = 0;

	if ((plmn == NULL) || (mcc == NULL) || (mnc == NULL)) {
		return EHS_FALSE;
	}
	while (plmn[len] != '\0') {
		len++;
	}
	if ((len != 5u) && (len != 6u)) {
		return EHS_FALSE;
	}
	for (i = 0u; i < len; i++) {
		if ((plmn[i] < '0') || (plmn[i] > '9')) {
			return EHS_FALSE;   /* an alphanumeric operator name, not a PLMN */
		}
	}
	for (i = 0u; i < 3u; i++) {
		m = (m * 10) + (ehs_sint32)(plmn[i] - '0');
	}
	for (i = 3u; i < len; i++) {
		n = (n * 10) + (ehs_sint32)(plmn[i] - '0');
	}
	*mcc = m;
	*mnc = n;
	return EHS_TRUE;
}

ehs_bool doCellularGetCellInfo(EhsCellInfoType *out)
{
	if (out == NULL) {
		return EHS_FALSE;
	}
	out->mcc          = s_cell_mcc;
	out->mnc          = s_cell_mnc;
	out->cell_id      = s_cell_id;
	out->tac          = s_cell_tac;
	/* Not available on this backend: +CEREG does not report EARFCN and no port
	 * exposes it, so it is not worth an extra AT%XMONITOR round trip. 0 here
	 * means 'not supplied', consistent with the stubbed backend. */
	out->earfcn       = 0u;
	out->phys_cell_id = EHS_CELL_VALUE_INVALID;
	out->band         = EHS_CELL_VALUE_INVALID;

#if defined(CONFIG_MODEM_INFO)
	{
		ehs_uint8 band = 0u;

		if (modem_info_get_current_band(&band) == 0) {
			out->band = (ehs_sint16)band;
		}
	}

	/* Derive MCC/MNC from the PLMN, because the CELL_UPDATE event does not
	 * carry them (see the note in the handler). AT+COPS? is the same source the
	 * operator name falls back to, and modem_info names this field "mccmnc" for
	 * exactly this reason. Read on demand rather than cached: it costs one AT
	 * command on a function the app triggers deliberately, and it cannot then go
	 * stale across a network re-selection. */
	if (s_modem_up) {
		ehs_char plmn[EHS_CELL_OPERATOR_BUFF_MAX];
		ehs_sint32 mcc = 0;
		ehs_sint32 mnc = 0;

		plmn[0] = '\0';
		if ((modem_info_string_get(MODEM_INFO_OPERATOR, plmn, sizeof(plmn)) > 0) &&
		    plmn_split(plmn, &mcc, &mnc)) {
			out->mcc = mcc;
			out->mnc = mnc;
		} else if (plmn[0] != '\0') {
			/* +COPS answered with an alphanumeric name rather than the numeric
			 * PLMN (format 0/1 rather than 2). Nothing to split. */
			EHSH_LOG_INFO("Cellular: +COPS gave '%s', not a numeric PLMN - "
			              "MCC/MNC unavailable", plmn);
		} else {
			EHSH_LOG_WARNING("Cellular: no PLMN available for MCC/MNC");
		}
	}
#endif
	return (s_cell_id != EHS_CELL_CELLID_INVALID) ? EHS_TRUE : EHS_FALSE;
}

ehs_bool doCellularGetImei(ehs_char *buf, ehs_uint16 buf_size)
{
#if defined(CONFIG_MODEM_INFO)
	int rc;

	if ((buf == NULL) || (buf_size == 0u)) {
		return EHS_FALSE;
	}
	buf[0] = '\0';
	if (!s_modem_up) {
		EHSH_LOG_WARNING("Cellular: IMEI read before modem init");
		return EHS_FALSE;
	}
	rc = modem_info_string_get(MODEM_INFO_IMEI, buf, buf_size);
	if (rc <= 0) {
		/* Logged for CONTRAST as much as for its own sake: IMEI is answered by
		 * the modem itself (AT+CGSN), so if this succeeds in the same call
		 * where ICCID/IMSI fail, modem_info and at_parser are healthy and the
		 * fault is specific to SIM file access. If all three fail together,
		 * suspect the calling context instead (these are blocking AT commands
		 * and need stack for modem_info's own response buffer). */
		EHSH_LOG_WARNING("Cellular: IMEI read failed (%d)", rc);
		buf[0] = '\0';
		return EHS_FALSE;
	}
	return EHS_TRUE;
#else
	if ((buf != NULL) && (buf_size > 0u)) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
#endif
}

ehs_bool doCellularGetOperator(ehs_char *buf, ehs_uint16 buf_size)
{
#if defined(CONFIG_MODEM_INFO)
	if ((buf == NULL) || (buf_size == 0u)) {
		return EHS_FALSE;
	}
	int rc;

	buf[0] = '\0';
	if (!s_modem_up) {
		EHSH_LOG_WARNING("Cellular: operator read before modem init");
		EhsCellSetFailReason(EhsCellFail_ModemInit);
		return EHS_FALSE;
	}
	/* TWO ROUTES, AND THE FIRST LEGITIMATELY FAILS ON MANY NETWORKS.
	 *
	 * Preferred is the human-readable short name from AT%XMONITOR. But NCS
	 * parses it with the scanf conversion "%64[^\"]", which needs at least one
	 * character, so a network that broadcasts an EMPTY name ("") fails the whole
	 * parse. That is common rather than exceptional: many networks send no name,
	 * and MVNO / IoT SIMs (Onomondo, Wireless Logic - what the Thingy:91 X kit
	 * ships with) usually carry no EF_SPN either. NCS itself logs this at debug
	 * level "because it is not always reported", i.e. it treats an absent
	 * operator name as normal.
	 *
	 * So fall back to AT+COPS?, whose modem_info data name is literally
	 * "mccmnc": the numeric PLMN, always present once registered. A port reading
	 * "23410" is far more use than an empty one, and it is the same identity in
	 * numeric form.
	 *
	 * NOTE THE MINIMUM on the first route: modem_info_get_operator() rejects any
	 * buffer smaller than MODEM_INFO_SHORT_OP_NAME_SIZE (65) with -EINVAL and
	 * never issues the AT command at all. EHS_CELL_OPERATOR_BUFF_MAX is 65 to
	 * match - callers must pass that, not a smaller shared scratch size. */
	rc = modem_info_get_operator(buf, (size_t)buf_size);
	if ((rc == 0) && (buf[0] != '\0')) {
		return EHS_TRUE;
	}

	buf[0] = '\0';
	rc = modem_info_string_get(MODEM_INFO_OPERATOR, buf, buf_size);
	if ((rc > 0) && (buf[0] != '\0')) {
		EHSH_LOG_INFO("Cellular: network reports no operator name, "
		              "using numeric PLMN '%s'", buf);
		return EHS_TRUE;
	}

	EHSH_LOG_WARNING("Cellular: operator unavailable by name or PLMN (%d), "
	                 "buf_size %u", rc, (unsigned)buf_size);
	/* Both routes failed. NotSupported rather than a link error: the modem is
	 * up and answering, it simply has no operator identity to give - which is
	 * what the caller needs to distinguish from "the read never happened". */
	EhsCellSetFailReason(EhsCellFail_NotSupported);
	buf[0] = '\0';
	return EHS_FALSE;
#else
	if ((buf != NULL) && (buf_size > 0u)) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
#endif
}

ehs_bool doCellularGetModemFwVersion(ehs_char *buf, ehs_uint16 buf_size)
{
#if defined(CONFIG_MODEM_INFO)
	if ((buf == NULL) || (buf_size == 0u)) {
		return EHS_FALSE;
	}
	buf[0] = '\0';
	return (modem_info_get_fw_version(buf, (size_t)buf_size) == 0)
	       ? EHS_TRUE : EHS_FALSE;
#else
	if ((buf != NULL) && (buf_size > 0u)) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
#endif
}

ehs_bool doCellularGetDataCounters(ehs_uint32 *tx_bytes, ehs_uint32 *rx_bytes)
{
#if defined(CONFIG_MODEM_INFO)
	int tx_kb = 0;
	int rx_kb = 0;

	if (modem_info_get_connectivity_stats(&tx_kb, &rx_kb) != 0) {
		return EHS_FALSE;
	}
	/* The modem reports kilobytes; the contract is bytes. */
	if (tx_bytes != NULL) {
		*tx_bytes = (ehs_uint32)tx_kb * 1024u;
	}
	if (rx_bytes != NULL) {
		*rx_bytes = (ehs_uint32)rx_kb * 1024u;
	}
	return EHS_TRUE;
#else
	(void)tx_bytes;
	(void)rx_bytes;
	return EHS_FALSE;
#endif
}

ehs_bool doCellularResetDataCounters(void)
{
#if defined(CONFIG_MODEM_INFO)
	return (modem_info_connectivity_stats_init() == 0) ? EHS_TRUE : EHS_FALSE;
#else
	return EHS_FALSE;
#endif
}

ehs_bool doCellularPsmRequest(ehs_bool enable, ehs_sint32 tau_s, ehs_sint32 active_s)
{
#if defined(CONFIG_LTE_LC_PSM_MODULE)
	if (enable) {
		(void)lte_lc_psm_param_set_seconds((int)tau_s, (int)active_s);
	}
	return (lte_lc_psm_req(enable ? true : false) == 0) ? EHS_TRUE : EHS_FALSE;
#else
	(void)enable; (void)tau_s; (void)active_s;
	return EHS_FALSE;
#endif
}

ehs_bool doCellularPsmGetGranted(EhsCellPsmType *out)
{
#if defined(CONFIG_LTE_LC_PSM_MODULE)
	if (out == NULL) {
		return EHS_FALSE;
	}
	out->granted  = s_psm_granted;
	out->tau_s    = s_psm_tau;
	out->active_s = s_psm_active;
	return EHS_TRUE;
#else
	if (out != NULL) {
		out->granted = EHS_FALSE;
		out->tau_s = 0;
		out->active_s = 0;
	}
	return EHS_FALSE;
#endif
}

ehs_bool doCellularEdrxRequest(ehs_bool enable, EhsCellRat rat, ehs_sint32 cycle_ms)
{
#if defined(CONFIG_LTE_LC_EDRX_MODULE)
	(void)rat;
	(void)cycle_ms;   /* cycle is set via lte_lc_edrx_param_set's coded string */
	return (lte_lc_edrx_req(enable ? true : false) == 0) ? EHS_TRUE : EHS_FALSE;
#else
	(void)enable; (void)rat; (void)cycle_ms;
	return EHS_FALSE;
#endif
}

ehs_bool doCellularEdrxGetGranted(EhsCellEdrxType *out)
{
#if defined(CONFIG_LTE_LC_EDRX_MODULE)
	if (out == NULL) {
		return EHS_FALSE;
	}
	out->granted  = s_edrx_granted;
	out->cycle_ms = s_edrx_cycle_ms;
	out->ptw_ms   = s_edrx_ptw_ms;
	out->rat      = (s_active_mode == LTE_LC_LTE_MODE_NBIOT)
	                ? EHS_CELL_RAT_NBIOT : EHS_CELL_RAT_LTEM;
	return EHS_TRUE;
#else
	if (out != NULL) {
		out->granted = EHS_FALSE;
		out->rat = EHS_CELL_RAT_NONE;
		out->cycle_ms = 0;
		out->ptw_ms = 0;
	}
	return EHS_FALSE;
#endif
}

ehs_bool doCellularRaiSet(ehs_bool as_rai, ehs_bool cp_rai)
{
	(void)as_rai;
	(void)cp_rai;
	return EHS_FALSE;   /* not implemented — capability bit reflects the module only */
}

/*****************************************************************************/
/* SIM                                                                       */

ehs_bool doCellularSimGetState(EhsSimState *out)
{
	if (out == NULL) {
		return EHS_FALSE;
	}
	if (!s_modem_up) {
		*out = EHS_SIM_STATE_UNKNOWN;
		return EHS_FALSE;
	}
	if (s_reg == LTE_LC_NW_REG_UICC_FAIL) {
		*out = EHS_SIM_STATE_FAILED;
		return EHS_TRUE;
	}

	{
		/* %XSIM: <status>  — 1 means the UICC is initialised and usable. */
		int status = 0;

		if (nrf_modem_at_scanf("AT%XSIM?", "%%XSIM: %d", &status) == 1) {
			*out = (status == 1) ? EHS_SIM_STATE_READY : EHS_SIM_STATE_ABSENT;
			return EHS_TRUE;
		}
	}

	*out = EHS_SIM_STATE_UNKNOWN;
	return EHS_FALSE;
}

ehs_bool doCellularSimGetActiveSlot(EhsSimSlot *out)
{
	int csus = 0;

	if (out == NULL) {
		return EHS_FALSE;
	}
	/* %CSUS distinguishes the UICC interface from SoftSIM. It cannot
	 * distinguish physical from embedded — on this board those share one
	 * interface and the analog switch decides, so PHYSICAL is reported for
	 * both. See the note on EhsSimSlot in cellular.h. */
	if (s_modem_up && (nrf_modem_at_scanf("AT%CSUS?", "%%CSUS: %d", &csus) == 1)) {
		*out = (csus == 2) ? EHS_SIM_SLOT_SOFT : EHS_SIM_SLOT_PHYSICAL;
		return EHS_TRUE;
	}
	*out = EHS_SIM_SLOT_PHYSICAL;
	return EHS_FALSE;
}

ehs_uint32 doCellularSimGetAvailableSlots(void)
{
	/* PHYSICAL always (socket or the eSIM footprint behind the same switch),
	 * SOFT because the nRF91 modem supports SoftSIM — whether a profile is
	 * actually provisioned is a separate question
	 * (doCellularSimSoftIsProvisioned). */
	return (ehs_uint32)((1u << (ehs_uint32)EHS_SIM_SLOT_PHYSICAL) |
	                    (1u << (ehs_uint32)EHS_SIM_SLOT_SOFT));
}

ehs_uint32 doCellularSimGetSelectableSlots(void)
{
	/* %CSUS chooses between the UICC interface (0) and SoftSIM (2), so both of
	 * those are runtime-selectable. EHS_SIM_SLOT_EMBEDDED is NOT: on this board
	 * the eSIM footprint sits behind the same UICC interface as the 4FF socket,
	 * with a card-detect-driven analog switch deciding between them. Reporting
	 * it as selectable would offer the caller a switch that silently cannot
	 * happen. */
	return (ehs_uint32)((1u << (ehs_uint32)EHS_SIM_SLOT_PHYSICAL) |
	                    (1u << (ehs_uint32)EHS_SIM_SLOT_SOFT));
}

ehs_bool doCellularSimSelect(EhsSimSlot slot)
{
	if ((doCellularSimGetSelectableSlots() &
	     (ehs_uint32)(1u << (ehs_uint32)slot)) == 0u) {
		return EHS_FALSE;
	}
	return cell_post(Cell_Cmd_SimSelect, (ehs_uint8)slot, 0u, NULL);
}

ehs_bool doCellularSimGetIccid(ehs_char *buf, ehs_uint16 buf_size)
{
#if defined(CONFIG_MODEM_INFO)
	int rc;

	if ((buf == NULL) || (buf_size == 0u)) {
		return EHS_FALSE;
	}
	buf[0] = '\0';
	/* No modem, no AT. Without this the call goes to nrf_modem_at_cmd before
	 * nrf_modem_lib_init() has run and comes back -EIO, which reads as "the SIM
	 * refused the read" when the real answer is "you asked too early". Same
	 * guard doCellularSimGetActiveSlot() already uses. Trigger SIM reads from
	 * cell_modem_manager's Connected event, not from app start. */
	if (!s_modem_up) {
		EHSH_LOG_WARNING("Cellular: ICCID read before modem init - trigger SIM "
		                 "reads from the Connected event, not app start");
		return EHS_FALSE;
	}
	rc = modem_info_string_get(MODEM_INFO_ICCID, buf, buf_size);
	if (rc <= 0) {
		/* Do not collapse this to a bare false. ICCID is a SIM-RESIDENT read
		 * (AT+CRSM=176,12258,0,0,10 against EF_ICCID), unlike IMEI which the
		 * modem answers from itself - so it can fail on a link that attached
		 * perfectly well, and the caller then has an empty string and no idea
		 * why. Decode of rc:
		 *   -EIO       the modem returned ERROR to the AT command - SIM file
		 *              access refused or unavailable in the current CFUN state
		 *   -EMSGSIZE  buf too small (needs 21+; EHS_CELL_ICCID_BUFF_MAX is 24)
		 *   -ENOTSUP   command answered but parsed to a zero-length value
		 *   other      at_parser error against the +CRSM response */
		EHSH_LOG_WARNING("Cellular: ICCID read failed (%d)", rc);
		buf[0] = '\0';
		return EHS_FALSE;
	}
	return EHS_TRUE;
#else
	if ((buf != NULL) && (buf_size > 0u)) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
#endif
}

ehs_bool doCellularSimGetImsi(ehs_char *buf, ehs_uint16 buf_size)
{
#if defined(CONFIG_MODEM_INFO)
	int rc;

	if ((buf == NULL) || (buf_size == 0u)) {
		return EHS_FALSE;
	}
	buf[0] = '\0';
	if (!s_modem_up) {
		EHSH_LOG_WARNING("Cellular: IMSI read before modem init - trigger SIM "
		                 "reads from the Connected event, not app start");
		return EHS_FALSE;
	}
	rc = modem_info_string_get(MODEM_INFO_IMSI, buf, buf_size);
	if (rc <= 0) {
		/* SIM-resident (AT+CIMI) - see the decode note on ICCID above. Worth
		 * knowing that a PIN-locked SIM answers ERROR here while still being
		 * able to attach later, so -EIO is not automatically a fault. */
		EHSH_LOG_WARNING("Cellular: IMSI read failed (%d)", rc);
		buf[0] = '\0';
		return EHS_FALSE;
	}
	return EHS_TRUE;
#else
	if ((buf != NULL) && (buf_size > 0u)) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
#endif
}

ehs_bool doCellularSimPinEnter(const ehs_char *pin)
{
	if ((pin == NULL) || (pin[0] == '\0')) {
		return EHS_FALSE;
	}
	return cell_post(Cell_Cmd_PinEnter, 0u, 0u, pin);
}

ehs_sint32 doCellularSimPinAttemptsRemaining(void)
{
	/* The nRF91 modem does not expose the PIN retry counter through any
	 * documented AT command. Returning -1 ("unknown") is the safe answer: the
	 * shared layer treats unknown as "do not auto-retry", which is exactly the
	 * behaviour we want when we cannot see how many attempts are left. */
	return -1;
}

ehs_bool doCellularSimPinEnable(ehs_bool enable, const ehs_char *pin)
{
	if ((pin == NULL) || (pin[0] == '\0')) {
		return EHS_FALSE;
	}
	return (nrf_modem_at_printf("AT+CLCK=\"SC\",%d,\"%s\"",
	                            enable ? 1 : 0, pin) == 0) ? EHS_TRUE : EHS_FALSE;
}

ehs_bool doCellularSimPinChange(const ehs_char *old_pin, const ehs_char *new_pin)
{
	if ((old_pin == NULL) || (new_pin == NULL)) {
		return EHS_FALSE;
	}
	return (nrf_modem_at_printf("AT+CPWD=\"SC\",\"%s\",\"%s\"",
	                            old_pin, new_pin) == 0) ? EHS_TRUE : EHS_FALSE;
}

ehs_bool doCellularSimSoftIsProvisioned(void)
{
	/* Requires the SoftSIM provider module, which is not built into this
	 * target yet (it is an out-of-tree west project — see the plan's
	 * ERT_ZEPHYR_EXTRA_MODULES item). */
	return EHS_FALSE;
}

ehs_bool doCellularSimSoftProvision(const ehs_uint8 *blob, ehs_uint32 len)
{
	(void)blob;
	(void)len;
	return EHS_FALSE;
}

/*****************************************************************************/
/* eUICC — no eUICC on stock Thingy:91 X (footprint U15 unpopulated)         */
/*
 * Neither EHS_CELL_CAP_EUICC_PROFILES nor EHS_CELL_CAP_EUICC_DOWNLOAD is
 * advertised, so the shared layer refuses all of these before they reach here.
 * They exist so the contract is complete and so that a board with U15 populated
 * only has to fill them in and set the capability bits.
 *
 * WHEN IMPLEMENTING, note the two halves are independent:
 *   Count/Get/Enable/Disable/Delete  ES10c, local APDU to the chip. On this
 *                                    modem that means AT+CSIM - NCS's own
 *                                    lib/uicc_lwm2m is a working precedent for
 *                                    APDU exchange over that command.
 *   SetEim/Download                  RSP. Needs an IPA (on-chip IPAe, or IPAd
 *                                    here) plus IP reachability to an eIM /
 *                                    SM-DP+. Far larger; prefer IPAe.
 * A board can legitimately implement the first group and not the second.
 */
ehs_sint32 doCellularSimProfileCount(void)
{
	return -1;   /* "cannot be determined", never 0 — 0 means an empty eUICC */
}

ehs_bool doCellularSimProfileGet(ehs_sint32 index, EhsSimProfileType *out)
{
	(void)index;
	(void)out;
	return EHS_FALSE;
}

ehs_bool doCellularSimProfileEnable(const ehs_char *iccid)
{
	(void)iccid;
	return EHS_FALSE;
}

ehs_bool doCellularSimProfileDisable(const ehs_char *iccid)
{
	(void)iccid;
	return EHS_FALSE;
}

ehs_bool doCellularSimProfileDelete(const ehs_char *iccid)
{
	(void)iccid;
	return EHS_FALSE;
}

ehs_bool doCellularSimProfileSetEim(const ehs_char *eim_address)
{
	(void)eim_address;
	return EHS_FALSE;
}

ehs_bool doCellularSimProfileDownload(const ehs_char *activation_code,
                                     ehs_char *iccid_out, ehs_uint16 iccid_buf_size)
{
	(void)activation_code;
	(void)iccid_buf_size;
	if (iccid_out != NULL) {
		iccid_out[0] = '\0';
	}
	return EHS_FALSE;
}

/*****************************************************************************/
/* eUICC factory install                                                     */

ehs_bool doCellularSimEuiccGetEid(ehs_char *buf, ehs_uint16 buf_size)
{
	(void)buf_size;
	if (buf != NULL) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
}

ehs_sint32 doCellularSimFactoryInstallMaxSegment(void)
{
	return -1;   /* cannot be determined - no eUICC */
}

ehs_bool doCellularSimFactoryInstallBegin(void)
{
	return EHS_FALSE;
}

ehs_bool doCellularSimFactoryInstallPushSegment(const ehs_uint8 *seg, ehs_uint32 len)
{
	(void)seg;
	(void)len;
	return EHS_FALSE;
}

ehs_bool doCellularSimFactoryInstallCommit(ehs_char *iccid_out, ehs_uint16 iccid_buf_size)
{
	(void)iccid_buf_size;
	if (iccid_out != NULL) {
		iccid_out[0] = '\0';
	}
	return EHS_FALSE;
}

void doCellularSimFactoryInstallAbort(void)
{
	/* Nothing to discard - no install can have started. */
}

/*****************************************************************************/
/* Boolean accessors                                                         */

ehs_bool isCellularInitialised(void)
{
	return s_modem_up;
}

ehs_bool isCellularRegistered(void)
{
	return s_registered;
}

ehs_bool isCellularOnline(void)
{
	/* PSM counts as online: the modem is registered and data resumes on wake.
	 * Reporting offline here is what makes a correctly power-optimised device
	 * look broken. */
	return (s_online || s_sleeping) ? EHS_TRUE : EHS_FALSE;
}

ehs_bool isCellularSleeping(void)
{
	return s_sleeping;
}

#endif /* EHS_NETWORK_CELLULAR_SUPPORT */
