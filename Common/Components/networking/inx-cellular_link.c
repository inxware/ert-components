/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file inx-cellular_link.c
 * Shared cellular connect state machine and the Cell_* / EhsSim_* unified API.
 *
 * Platform-independent by construction: this file contains no NCS, no AT, no
 * vendor type. It drives the doCellular* primitives declared in
 * target/Component-HAL/cellular/cellular.h and reacts to the cbSource
 * transitions a backend posts from its native event source. That mirrors the
 * Wi-Fi split (Common/Components/networking/inx-wifi_station.c +
 * target/os-arch/<os>/target_wifi.c) and is documented as the porting contract at
 * the top of cellular.h.
 *
 * THREE THINGS THIS FILE EXISTS TO GET RIGHT
 * ------------------------------------------
 *  1. TERMINAL STATES STAY TERMINAL. SimPinRequired, SimFailed and RegDenied
 *     never retry on their own. A PIN retry loop permanently locks a customer's
 *     SIM after three boots; a denied-registration retry loop burns battery and
 *     can trip the modem's own reset-loop protection. Recovery from these is a
 *     deliberate caller action (Cell_Connect / EhsSim_PinEnter), never a timer.
 *
 *  2. PSM IS NOT A DISCONNECT. EhsCell_PsmSleep means "registered, radio
 *     asleep, data resumes on wake". isCellularOnline() covers it and no
 *     reconnect is attempted, because attempting one is what makes a correctly
 *     power-optimised device look broken.
 *
 *  3. BACKOFF IS EXPONENTIAL. Wi-Fi does N fast retries then a slow period.
 *     Here a failed attach means poor coverage or a network problem, so the
 *     delay doubles from CELL_SM_BACKOFF_INITIAL_MS to CELL_SM_BACKOFF_MAX_MS.
 *
 * The one-timer-per-transitory-state rule from the Wi-Fi contract is carried
 * over verbatim: exactly one deadline is armed at a time (s_phase_armed), and
 * states that persist indefinitely arm none.
 *
 * @author: inx limited
 */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include "globals.h"

#ifdef EHS_NETWORK_CELLULAR_SUPPORT

#include <string.h>

#include "hal.h"
#include "hal_logger.h"
#include "hal_string.h"
#include "cellular.h"

/* Shared-SM tick. One second matches the Wi-Fi SM and is far finer than any
 * cellular deadline here (the shortest is 10 s), so it costs nothing and keeps
 * state transitions responsive to backend events. */
#define EHS_CELLULAR_THREADLOOP_SLEEP_S 1u

#ifndef EHS_CELLULAR_THREAD_STACK_SIZE
#define EHS_CELLULAR_THREAD_STACK_SIZE 4096
#endif
#ifndef EHS_CELLULAR_THREAD_PRIORITY
#define EHS_CELLULAR_THREAD_PRIORITY 5
#endif

/*****************************************************************************/
/* State                                                                     */

static volatile EhsCellState             s_state      = EhsCell_Idle;
static volatile EhsCellFailReason        s_fail       = EhsCellFail_None;

/* Pending backend events, as a BITMASK — one bit per eCellCallbackSource.
 *
 * This was a single slot, and that was a real bug: a backend that posts two
 * events back-to-back (target_cellular.c's worker_start posts ModemReady then
 * SimReady) overwrote the first before the SM's 1 s tick could read it. The SM
 * then received SimReady while still in ModemInit, whose handler ignores it,
 * so it sat in ModemInit until the 10 s deadline and reported
 * "Failed / ModemInit" on a modem that had initialised perfectly. Observed on
 * hardware, first run.
 *
 * A bitmask rather than a queue because these events are level-ish facts
 * ("the SIM is ready"), not a stream — coalescing a duplicate is correct, and
 * losing one to a queue-full condition would not be. Drained in the fixed
 * priority order by cb_take() rather than arrival order, which is
 * deterministic and avoids depending on backend posting order.
 *
 * The read-modify-write to clear a bit is not atomic. The window is a few
 * instructions and the SM re-polls SIM and registration state every tick
 * regardless, so a lost event costs at most one second, never correctness. */
static volatile ehs_uint32               s_cb_pending = 0u;
static volatile ehs_bool                 s_thread_run = EHS_FALSE;
static ehs_bool                          s_thread_started = EHS_FALSE;

/* Cached copy of the last configuration handed to Cell_Connect(). The pointers
 * in EhsCellConfigType belong to the caller and may be transient, so the
 * strings are copied here rather than aliased. */
static EhsCellConfigType s_cfg;
static ehs_char s_cfg_apn[EHS_CELL_APN_BUFF_MAX]   = {0};
static ehs_char s_cfg_user[EHS_CELL_USER_BUFF_MAX] = {0};
static ehs_char s_cfg_pass[EHS_CELL_PASS_BUFF_MAX] = {0};
static ehs_char s_cfg_pin[EHS_CELL_PIN_BUFF_MAX]   = {0};
static ehs_bool s_cfg_valid = EHS_FALSE;

static ehs_char s_ip[EHS_CELL_IPADDR_BUFF_MAX] = {0};

/* Exactly one armed deadline at a time — see the one-timer rule above. */
static EhsTickType s_phase_tick    = 0;
static ehs_uint32  s_phase_ms      = 0u;
static ehs_bool    s_phase_armed   = EHS_FALSE;

/* Exponential reconnect backoff. */
static EhsTickType s_backoff_tick  = 0;
static ehs_uint32  s_backoff_ms    = CELL_SM_BACKOFF_INITIAL_MS;
static ehs_bool    s_backoff_armed = EHS_FALSE;

/* Set when the link has been Online at least once, so a drop is reported as a
 * disconnection rather than as a first attach that never completed. */
static ehs_bool s_was_online = EHS_FALSE;

/* Target of a pending runtime SIM-type switch (EhsSim_Select). */
static volatile EhsSimSlot s_pending_slot = EHS_SIM_SLOT_MAX;

static const ehs_char *const s_state_names[EhsCell_MAX] = {
	"Idle", "ModemInit", "SimWait", "SimPinRequired", "SimFailed",
	"RadioOn", "Searching", "NoCoverage", "RegDenied", "Registered",
	"Online", "PsmSleep", "Reconnecting", "Disconnecting", "Failed"
};

static const ehs_char *const s_fail_names[EhsCellFail_MAX] = {
	"None", "ModemInit", "SimAbsent", "SimPinRequired", "SimPukRequired",
	"SimFailure", "RegistrationDenied", "NoCoverage", "AttachTimeout",
	"PdnTimeout", "PdnRejected", "ModemResetLoop", "Overheated",
	"NotSupported", "InvalidArg", "Internal"
};

/*****************************************************************************/
/* Small helpers                                                             */

static void phase_arm(ehs_uint32 timeout_ms)
{
	s_phase_tick  = EHS_CURRENT_TIME;
	s_phase_ms    = timeout_ms;
	s_phase_armed = EHS_TRUE;
}

static void phase_disarm(void)
{
	s_phase_armed = EHS_FALSE;
}

static void backoff_arm(void)
{
	s_backoff_tick  = EHS_CURRENT_TIME;
	s_backoff_armed = EHS_TRUE;
}

static void backoff_reset(void)
{
	s_backoff_ms    = CELL_SM_BACKOFF_INITIAL_MS;
	s_backoff_armed = EHS_FALSE;
}

static void backoff_advance(void)
{
	if (s_backoff_ms < (CELL_SM_BACKOFF_MAX_MS / 2u)) {
		s_backoff_ms *= 2u;
	} else {
		s_backoff_ms = CELL_SM_BACKOFF_MAX_MS;
	}
}

/* Attach deadline depends on the RAT actually in use: NB-IoT searches are
 * legitimately several times longer than LTE-M. When both are enabled, or the
 * active RAT is not yet known, the longer bound applies — timing out early is
 * the failure mode that matters. */
static ehs_uint32 attach_timeout_ms(void)
{
	EhsCellRat active = EHS_CELL_RAT_NONE;

	if (doCellularGetActiveRat(&active) && (active == EHS_CELL_RAT_LTEM)) {
		return (ehs_uint32)CELL_SM_TIMEOUT_ATTACH_LTEM_MS;
	}
	return (ehs_uint32)CELL_SM_TIMEOUT_ATTACH_NBIOT_MS;
}

static void enter_state(EhsCellState next)
{
	if (s_state == next) {
		return;
	}
	EHSH_LOG_INFO("Cellular: %s -> %s",
	              Cell_GetStateName(s_state), Cell_GetStateName(next));
	s_state = next;
}

static void enter_terminal(EhsCellState next, EhsCellFailReason reason)
{
	phase_disarm();
	s_backoff_armed = EHS_FALSE;
	EhsCellSetFailReason(reason);
	enter_state(next);
}

/*****************************************************************************/
/* SM plumbing called by backends                                            */

EhsCellState getCellularConnectState(void)
{
	return s_state;
}

void setCellularConnectState(EhsCellState state)
{
	if (state < EhsCell_MAX) {
		s_state = state;
	}
}

void EhsCellSetCBSource(enum eCellCallbackSource source)
{
	if ((source > eCellCallbackSource_None) && (source < eCellCallbackSource_MAX)) {
		s_cb_pending |= (ehs_uint32)(1u << (ehs_uint32)source);
	}
}

/* Highest-priority pending event, without consuming it. Backends use this to
 * avoid clobbering a queued request with a lower-priority transition. */
enum eCellCallbackSource EhsCellGetCBSource(void)
{
	ehs_uint32 i;

	for (i = (ehs_uint32)eCellCallbackSource_None + 1u;
	     i < (ehs_uint32)eCellCallbackSource_MAX; i++) {
		if (s_cb_pending & (1u << i)) {
			return (enum eCellCallbackSource)i;
		}
	}
	return eCellCallbackSource_None;
}

/** Consume one pending event, or eCellCallbackSource_None if there are none. */
static enum eCellCallbackSource cb_take(void)
{
	ehs_uint32 i;

	for (i = (ehs_uint32)eCellCallbackSource_None + 1u;
	     i < (ehs_uint32)eCellCallbackSource_MAX; i++) {
		const ehs_uint32 bit = (1u << i);

		if (s_cb_pending & bit) {
			s_cb_pending &= ~bit;
			return (enum eCellCallbackSource)i;
		}
	}
	return eCellCallbackSource_None;
}

void EhsCellSetFailReason(EhsCellFailReason reason)
{
	if (reason < EhsCellFail_MAX) {
		s_fail = reason;
	}
}

/* Weak-by-convention hook: the cellular_link function block will override the
 * body when it lands. Until then a link drop is logged and nothing else, which
 * is correct — there is no FB instance to notify yet. */
void Common_Cellular_onDisconnected(EhsCellFailReason reason)
{
	EHSH_LOG_WARNING("Cellular: link lost (%s)", Cell_GetFailReasonText(reason));
}

/*****************************************************************************/
/* The state machine                                                         */

/**
 * Poll the SIM while in SimWait.
 *
 * The SM does not rely on a backend event for this. A UICC takes a moment to
 * initialise after it is powered, so "SIM ready" is a condition to be observed
 * rather than an edge to be signalled, and a backend that guesses (posting
 * SimReady optimistically before the card has answered) produces exactly the
 * wrong answer. Polling here means any backend that can answer
 * doCellularSimGetState() correctly works, whether or not it has an event.
 */
static void sm_poll_sim(void)
{
	EhsSimState sim = EHS_SIM_STATE_UNKNOWN;

	if (s_state != EhsCell_SimWait) {
		return;
	}
	if (!doCellularSimGetState(&sim)) {
		return;   /* backend cannot answer yet — the SimWait deadline covers us */
	}

	switch (sim) {
	case EHS_SIM_STATE_READY:
		EhsCellSetCBSource(eCellCallbackSource_SimReady);
		break;
	case EHS_SIM_STATE_PIN_REQUIRED:
		EhsCellSetCBSource(eCellCallbackSource_SimPin);
		break;
	case EHS_SIM_STATE_PUK_REQUIRED:
		EHSH_LOG_ERROR("Cellular: SIM requires a PUK - manual recovery only");
		enter_terminal(EhsCell_SimFailed, EhsCellFail_SimPukRequired);
		break;
	case EHS_SIM_STATE_FAILED:
		EhsCellSetCBSource(eCellCallbackSource_SimFail);
		break;
	case EHS_SIM_STATE_ABSENT:
	case EHS_SIM_STATE_UNKNOWN:
	default:
		/* Keep waiting: a card can take a moment to come up after the UICC is
		 * powered. The SimWait deadline converts a persistent absence into a
		 * terminal SimFailed/SimAbsent. */
		break;
	}
}

static void sm_service_watchdog(void)
{
	EhsTickType now;

	if (!s_phase_armed) {
		return;
	}

	now = EHS_CURRENT_TIME;
	if ((now - s_phase_tick) <= EhsTgtTimer_msToTick(s_phase_ms)) {
		return;
	}

	phase_disarm();

	switch (s_state) {
	case EhsCell_ModemInit:
		/* If the modem itself came up (isCellularInitialised) but we are still
		 * here, the backend never posted ModemReady — a plumbing fault, not a
		 * modem fault. Say which, because the two need completely different
		 * investigation. */
		if (isCellularInitialised()) {
			EHSH_LOG_ERROR("Cellular: modem is up but the backend never posted "
			               "ModemReady - event plumbing fault in the HAL backend");
		} else {
			EHSH_LOG_ERROR("Cellular: modem init timed out after %u ms", s_phase_ms);
		}
		enter_terminal(EhsCell_Failed, EhsCellFail_ModemInit);
		break;

	case EhsCell_SimWait:
		EHSH_LOG_ERROR("Cellular: no usable SIM after %u ms - card absent, not "
		               "seated, or not activated", s_phase_ms);
		enter_terminal(EhsCell_SimFailed, EhsCellFail_SimAbsent);
		break;

	case EhsCell_Searching:
		/* Not terminal: no coverage is a transient condition on a moving or
		 * marginally-sited device. Back off and search again. */
		EHSH_LOG_WARNING("Cellular: attach timed out after %u ms", s_phase_ms);
		EhsCellSetFailReason(EhsCellFail_AttachTimeout);
		enter_state(EhsCell_NoCoverage);
		backoff_arm();
		break;

	case EhsCell_Registered:
		EHSH_LOG_WARNING("Cellular: no IP within %u ms of registering", s_phase_ms);
		EhsCellSetFailReason(EhsCellFail_PdnTimeout);
		enter_state(EhsCell_Reconnecting);
		backoff_arm();
		break;

	default:
		break;
	}
}

static void sm_service_backoff(void)
{
	EhsTickType now;

	if (!s_backoff_armed) {
		return;
	}
	if ((s_state != EhsCell_NoCoverage) && (s_state != EhsCell_Reconnecting)) {
		s_backoff_armed = EHS_FALSE;
		return;
	}

	now = EHS_CURRENT_TIME;
	if ((now - s_backoff_tick) <= EhsTgtTimer_msToTick(s_backoff_ms)) {
		return;
	}

	s_backoff_armed = EHS_FALSE;
	backoff_advance();

	EHSH_LOG_INFO("Cellular: retrying attach (next backoff %u ms)", s_backoff_ms);
	if (doCellularConnect(s_cfg_valid ? &s_cfg : NULL)) {
		enter_state(EhsCell_Searching);
		phase_arm(attach_timeout_ms());
	} else {
		/* Could not even issue the request — stay in backoff rather than
		 * spinning. The next expiry tries again with a longer delay. */
		backoff_arm();
	}
}

static void sm_handle_event(enum eCellCallbackSource src)
{
	switch (src) {
	case eCellCallbackSource_Connect:
		backoff_reset();
		s_was_online = EHS_FALSE;
		EhsCellSetFailReason(EhsCellFail_None);
		if (!doCellularStart()) {
			enter_terminal(EhsCell_Failed, EhsCellFail_ModemInit);
			break;
		}
		enter_state(EhsCell_ModemInit);
		phase_arm((ehs_uint32)CELL_SM_TIMEOUT_MODEM_INIT_MS);
		break;

	case eCellCallbackSource_ModemReady:
		if (s_state == EhsCell_ModemInit) {
			enter_state(EhsCell_SimWait);
			phase_arm((ehs_uint32)CELL_SM_TIMEOUT_SIM_MS);
		}
		break;

	case eCellCallbackSource_SimReady:
		if ((s_state == EhsCell_SimWait) || (s_state == EhsCell_SimPinRequired)) {
			phase_disarm();
			enter_state(EhsCell_RadioOn);
			if (doCellularConnect(s_cfg_valid ? &s_cfg : NULL)) {
				enter_state(EhsCell_Searching);
				phase_arm(attach_timeout_ms());
			} else {
				enter_terminal(EhsCell_Failed, EhsCellFail_Internal);
			}
		}
		break;

	case eCellCallbackSource_SimPin:
		/* TERMINAL. Do NOT retry: three wrong PINs require a PUK, and an
		 * automatic retry would consume all three across three boots. */
		EHSH_LOG_ERROR("Cellular: SIM PIN required — halting (no auto-retry, "
		               "%d attempts left)", (int)doCellularSimPinAttemptsRemaining());
		enter_terminal(EhsCell_SimPinRequired, EhsCellFail_SimPinRequired);
		break;

	case eCellCallbackSource_SimFail:
		enter_terminal(EhsCell_SimFailed, EhsCellFail_SimFailure);
		break;

	case eCellCallbackSource_RegStatus: {
		EhsCellRegStatus reg = EhsCellReg_Unknown;

		if (!doCellularGetRegStatus(&reg)) {
			break;
		}
		switch (reg) {
		case EhsCellReg_RegisteredHome:
		case EhsCellReg_RegisteredRoaming:
			if ((s_state == EhsCell_Searching) ||
			    (s_state == EhsCell_NoCoverage) ||
			    (s_state == EhsCell_Reconnecting) ||
			    (s_state == EhsCell_RadioOn)) {
				backoff_reset();
				enter_state(EhsCell_Registered);
				phase_arm((ehs_uint32)CELL_SM_TIMEOUT_PDN_MS);
			}
			break;

		case EhsCellReg_Denied:
			/* TERMINAL-ish: the network refused the subscription. Only a
			 * caller-initiated Cell_Connect() restarts this. */
			EHSH_LOG_ERROR("Cellular: registration denied by network");
			enter_terminal(EhsCell_RegDenied, EhsCellFail_RegistrationDenied);
			break;

		case EhsCellReg_UiccFail:
			enter_terminal(EhsCell_SimFailed, EhsCellFail_SimFailure);
			break;

		case EhsCellReg_Searching:
			if (s_state == EhsCell_RadioOn) {
				enter_state(EhsCell_Searching);
				phase_arm(attach_timeout_ms());
			}
			break;

		case EhsCellReg_NotRegistered:
		case EhsCellReg_Unknown:
		default:
			if ((s_state == EhsCell_Online) || (s_state == EhsCell_PsmSleep) ||
			    (s_state == EhsCell_Registered)) {
				phase_disarm();
				if (s_was_online) {
					Common_Cellular_onDisconnected(EhsCellFail_NoCoverage);
				}
				enter_state(EhsCell_Reconnecting);
				backoff_arm();
			}
			break;
		}
		break;
	}

	case eCellCallbackSource_GotIp:
		if ((s_state == EhsCell_Registered) || (s_state == EhsCell_Searching)) {
			phase_disarm();
			backoff_reset();
			(void)doCellularGetIp(s_ip, (ehs_uint16)sizeof(s_ip));
			s_was_online = EHS_TRUE;
			enter_state(EhsCell_Online);
			EHSH_LOG_INFO("Cellular: online, IP %s", s_ip);
		}
		break;

	case eCellCallbackSource_Psm:
		/* Registered but asleep. NOT a disconnect — see rule 2 in the header. */
		if (s_state == EhsCell_Online) {
			enter_state(EhsCell_PsmSleep);
		} else if (s_state == EhsCell_PsmSleep) {
			enter_state(EhsCell_Online);
		} else {
			/* PSM notification in a state where it means nothing. */
		}
		break;

	case eCellCallbackSource_LinkLost:
		if ((s_state == EhsCell_Online) || (s_state == EhsCell_PsmSleep) ||
		    (s_state == EhsCell_Registered)) {
			phase_disarm();
			if (s_was_online) {
				Common_Cellular_onDisconnected(s_fail);
			}
			enter_state(EhsCell_Reconnecting);
			backoff_arm();
		}
		break;

	case eCellCallbackSource_SimSwitch: {
		const EhsSimSlot want = s_pending_slot;

		if (want >= EHS_SIM_SLOT_MAX) {
			break;
		}
		s_pending_slot = EHS_SIM_SLOT_MAX;

		/* A SIM-type change needs a modem re-init, so the link cannot survive
		 * it. Tear down deliberately rather than letting the CFUN cycle look
		 * like an unexplained link loss, then re-enter SimWait so the normal
		 * polling confirms the new SIM before we attach on it. */
		EHSH_LOG_INFO("Cellular: switching SIM type, link will drop");
		phase_disarm();
		backoff_reset();
		if (s_was_online) {
			Common_Cellular_onDisconnected(EhsCellFail_None);
		}
		s_ip[0]      = '\0';
		s_was_online = EHS_FALSE;

		enter_state(EhsCell_Disconnecting);
		(void)doCellularDisconnect();

		if (!doCellularSimSelect(want)) {
			EHSH_LOG_ERROR("Cellular: SIM switch rejected by the backend");
			enter_terminal(EhsCell_SimFailed, EhsCellFail_NotSupported);
			break;
		}

		enter_state(EhsCell_SimWait);
		phase_arm((ehs_uint32)CELL_SM_TIMEOUT_SIM_MS);
		break;
	}

	case eCellCallbackSource_Disconnect:
		phase_disarm();
		s_backoff_armed = EHS_FALSE;
		enter_state(EhsCell_Disconnecting);
		(void)doCellularDisconnect();
		s_ip[0] = '\0';
		s_was_online = EHS_FALSE;
		enter_state(EhsCell_Idle);
		break;

	case eCellCallbackSource_Internal:
	case eCellCallbackSource_None:
	default:
		break;
	}
}

static EhsThreadFuncReturnType cellular_link_thread(void *context)
{
	(void)context;

	EHSH_LOG_INFO("Cellular: state machine thread started");

	while (s_thread_run) {
		enum eCellCallbackSource src;
		ehs_uint16 guard = 0u;

		/* Drain ALL pending events each tick, not just one. Two events posted
		 * between ticks is normal (a backend bringing the modem up posts
		 * ModemReady and then the SIM result), and handling one per second
		 * would make bring-up needlessly slow even once the single-slot
		 * overwrite bug was fixed. The guard bounds the loop against a
		 * pathological producer. */
		while (((src = cb_take()) != eCellCallbackSource_None) &&
		       (guard++ < (ehs_uint16)eCellCallbackSource_MAX)) {
			sm_handle_event(src);
		}

		sm_poll_sim();
		sm_service_watchdog();
		sm_service_backoff();

		EhsSleep(EHS_TIME_s(EHS_CELLULAR_THREADLOOP_SLEEP_S));
	}

	EHSH_LOG_INFO("Cellular: state machine thread stopped");
	return (EhsThreadFuncReturnType)0;
}

void EhsStartCellularThread(void)
{
	if (s_thread_started) {
		return;   /* idempotent — boot path and FB init both call this */
	}

	if (!doCellularInit()) {
		EHSH_LOG_ERROR("Cellular: backend init failed; SM thread not started");
		EhsCellSetFailReason(EhsCellFail_ModemInit);
		return;
	}

	s_thread_run     = EHS_TRUE;
	s_thread_started = EHS_TRUE;

	if (!EhsHThread_execute(cellular_link_thread, NULL,
	                        EHS_CELLULAR_THREAD_PRIORITY,
	                        EHS_CELLULAR_THREAD_STACK_SIZE,
	                        (ehs_char *)"cellular")) {
		EHSH_LOG_ERROR("Cellular: could not start SM thread (no free slots?)");
		s_thread_run     = EHS_FALSE;
		s_thread_started = EHS_FALSE;
	}
}

/*****************************************************************************/
/* Unified API                                                               */

ehs_uint32 Cell_GetCapabilities(void)
{
	return doCellularGetCapabilities();
}

ehs_bool Cell_Connect(const EhsCellConfigType *cfg)
{
	if (cfg != NULL) {
		memset(&s_cfg, 0, sizeof(s_cfg));
		s_cfg.rat           = cfg->rat;
		s_cfg.preference    = cfg->preference;
		s_cfg.request_psm   = cfg->request_psm;
		s_cfg.psm_tau_s     = cfg->psm_tau_s;
		s_cfg.psm_active_s  = cfg->psm_active_s;
		s_cfg.request_edrx  = cfg->request_edrx;
		s_cfg.edrx_cycle_ms = cfg->edrx_cycle_ms;

		/* The caller's strings may be transient — copy, don't alias. */
		s_cfg_apn[0] = s_cfg_user[0] = s_cfg_pass[0] = s_cfg_pin[0] = '\0';
		if (cfg->apn != NULL) {
			(void)EhsStrncpy_s(s_cfg_apn, sizeof(s_cfg_apn), cfg->apn,
			                   sizeof(s_cfg_apn) - 1u);
		}
		if (cfg->apn_user != NULL) {
			(void)EhsStrncpy_s(s_cfg_user, sizeof(s_cfg_user), cfg->apn_user,
			                   sizeof(s_cfg_user) - 1u);
		}
		if (cfg->apn_pass != NULL) {
			(void)EhsStrncpy_s(s_cfg_pass, sizeof(s_cfg_pass), cfg->apn_pass,
			                   sizeof(s_cfg_pass) - 1u);
		}
		if (cfg->sim_pin != NULL) {
			(void)EhsStrncpy_s(s_cfg_pin, sizeof(s_cfg_pin), cfg->sim_pin,
			                   sizeof(s_cfg_pin) - 1u);
		}
		s_cfg.apn      = s_cfg_apn;
		s_cfg.apn_user = s_cfg_user;
		s_cfg.apn_pass = s_cfg_pass;
		s_cfg.sim_pin  = s_cfg_pin;
		s_cfg_valid    = EHS_TRUE;
	}

	EhsStartCellularThread();
	if (!s_thread_started) {
		return EHS_FALSE;
	}

	EhsCellSetCBSource(eCellCallbackSource_Connect);
	return EHS_TRUE;
}

ehs_bool Cell_Disconnect(void)
{
	EhsCellSetCBSource(eCellCallbackSource_Disconnect);
	return EHS_TRUE;
}

ehs_bool Cell_SetPowerState(EhsCellPowerState state)
{
	return doCellularSetPowerState(state);
}

EhsCellPowerState Cell_GetPowerState(void)
{
	EhsCellPowerState state = EHS_CELL_POWER_OFF;

	(void)doCellularGetPowerState(&state);
	return state;
}

EhsCellState Cell_GetState(void)
{
	return s_state;
}

const ehs_char *Cell_GetStateName(EhsCellState state)
{
	if (state >= EhsCell_MAX) {
		return "?";
	}
	return s_state_names[state];
}

EhsCellRegStatus Cell_GetRegStatus(void)
{
	EhsCellRegStatus reg = EhsCellReg_Unknown;

	(void)doCellularGetRegStatus(&reg);
	return reg;
}

ehs_bool Cell_IsRoaming(void)
{
	return (Cell_GetRegStatus() == EhsCellReg_RegisteredRoaming) ? EHS_TRUE : EHS_FALSE;
}

const ehs_char *Cell_GetIp(void)
{
	return s_ip;
}

EhsCellFailReason Cell_GetLastFailReason(void)
{
	return s_fail;
}

const ehs_char *Cell_GetFailReasonText(EhsCellFailReason reason)
{
	if (reason >= EhsCellFail_MAX) {
		return "?";
	}
	return s_fail_names[reason];
}

ehs_bool Cell_SetRat(EhsCellRat rat, EhsCellRatPref preference)
{
	return doCellularSetRat(rat, preference);
}

ehs_bool Cell_GetRat(EhsCellRat *rat, EhsCellRatPref *preference)
{
	return doCellularGetRat(rat, preference);
}

EhsCellRat Cell_GetActiveRat(void)
{
	EhsCellRat rat = EHS_CELL_RAT_NONE;

	(void)doCellularGetActiveRat(&rat);
	return rat;
}

ehs_bool Cell_SetBandMask(ehs_uint64 mask)
{
	return doCellularSetBandMask(mask);
}

ehs_bool Cell_GetSignal(EhsCellSignalType *out)
{
	if (out == NULL) {
		return EHS_FALSE;
	}
	return doCellularGetSignal(out);
}

ehs_bool Cell_EvalLink(EhsCellLinkEvalType *out)
{
	if (out == NULL) {
		return EHS_FALSE;
	}
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_CONN_EVAL) == 0u) {
		return EHS_FALSE;
	}
	return doCellularEvalLink(out);
}

ehs_bool Cell_GetCellInfo(EhsCellInfoType *out)
{
	if (out == NULL) {
		return EHS_FALSE;
	}
	return doCellularGetCellInfo(out);
}

ehs_bool Cell_GetImei(ehs_char *buf, ehs_uint16 buf_size)
{
	return doCellularGetImei(buf, buf_size);
}

ehs_bool Cell_GetOperator(ehs_char *buf, ehs_uint16 buf_size)
{
	return doCellularGetOperator(buf, buf_size);
}

ehs_bool Cell_GetModemFwVersion(ehs_char *buf, ehs_uint16 buf_size)
{
	return doCellularGetModemFwVersion(buf, buf_size);
}

ehs_bool Cell_GetDataCounters(ehs_uint32 *tx_bytes, ehs_uint32 *rx_bytes)
{
	return doCellularGetDataCounters(tx_bytes, rx_bytes);
}

ehs_bool Cell_ResetDataCounters(void)
{
	return doCellularResetDataCounters();
}

ehs_bool Cell_PsmRequest(ehs_bool enable, ehs_sint32 tau_s, ehs_sint32 active_s)
{
	return doCellularPsmRequest(enable, tau_s, active_s);
}

ehs_bool Cell_PsmGetGranted(EhsCellPsmType *out)
{
	if (out == NULL) {
		return EHS_FALSE;
	}
	return doCellularPsmGetGranted(out);
}

ehs_bool Cell_EdrxRequest(ehs_bool enable, EhsCellRat rat, ehs_sint32 cycle_ms)
{
	return doCellularEdrxRequest(enable, rat, cycle_ms);
}

ehs_bool Cell_EdrxGetGranted(EhsCellEdrxType *out)
{
	if (out == NULL) {
		return EHS_FALSE;
	}
	return doCellularEdrxGetGranted(out);
}

ehs_bool Cell_RaiSet(ehs_bool as_rai, ehs_bool cp_rai)
{
	return doCellularRaiSet(as_rai, cp_rai);
}

/*****************************************************************************/
/* SIM sub-API                                                               */

EhsSimState EhsSim_GetState(void)
{
	EhsSimState state = EHS_SIM_STATE_UNKNOWN;

	(void)doCellularSimGetState(&state);
	return state;
}

EhsSimSlot EhsSim_GetActiveSlot(void)
{
	EhsSimSlot slot = EHS_SIM_SLOT_PHYSICAL;

	(void)doCellularSimGetActiveSlot(&slot);
	return slot;
}

ehs_uint32 EhsSim_GetAvailableSlots(void)
{
	return doCellularSimGetAvailableSlots();
}

ehs_uint32 EhsSim_GetSelectableSlots(void)
{
	/* Never wider than what exists: a backend that reported a selectable type
	 * it does not have would offer the caller a switch that cannot happen. */
	return doCellularSimGetSelectableSlots() & doCellularSimGetAvailableSlots();
}

ehs_bool EhsSim_Select(EhsSimSlot slot)
{
	if (slot >= EHS_SIM_SLOT_MAX) {
		EhsCellSetFailReason(EhsCellFail_InvalidArg);
		return EHS_FALSE;
	}
	if (slot == EhsSim_GetActiveSlot()) {
		return EHS_TRUE;   /* already there — a no-op, not an error */
	}
	if ((EhsSim_GetSelectableSlots() & (ehs_uint32)(1u << (ehs_uint32)slot)) == 0u) {
		/* Either the type does not exist here, or it exists but is chosen by
		 * hardware (an eUICC behind a card-detect autoswitch). Say so rather
		 * than silently succeeding and leaving the caller believing it
		 * switched. */
		EHSH_LOG_WARNING("Cellular: SIM type %d is not runtime-selectable "
		                 "(selectable mask 0x%02X)",
		                 (int)slot, (unsigned)EhsSim_GetSelectableSlots());
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}

	/* Hand off to the SM rather than switching behind its back: the CFUN cycle
	 * drops the link, and the SM has to tear down and re-attach in order. */
	s_pending_slot = slot;
	EhsCellSetCBSource(eCellCallbackSource_SimSwitch);
	return EHS_TRUE;
}

ehs_bool EhsSim_GetIccid(ehs_char *buf, ehs_uint16 buf_size)
{
	return doCellularSimGetIccid(buf, buf_size);
}

ehs_bool EhsSim_GetImsi(ehs_char *buf, ehs_uint16 buf_size)
{
	return doCellularSimGetImsi(buf, buf_size);
}

ehs_bool EhsSim_PinEnter(const ehs_char *pin)
{
	if ((pin == NULL) || (pin[0] == '\0')) {
		EhsCellSetFailReason(EhsCellFail_InvalidArg);
		return EHS_FALSE;
	}
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_SIM_PIN) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	/* Refuse outright on the last attempt. A caller that has got this far with
	 * a wrong PIN would otherwise brick the SIM, and no automatic behaviour is
	 * worth that. Recovery requires a PUK and a human. */
	if (doCellularSimPinAttemptsRemaining() == 1) {
		EHSH_LOG_ERROR("Cellular: refusing PIN entry — 1 attempt remains, "
		               "a wrong PIN here requires a PUK to recover");
		EhsCellSetFailReason(EhsCellFail_SimPinRequired);
		return EHS_FALSE;
	}
	return doCellularSimPinEnter(pin);
}

ehs_sint32 EhsSim_PinAttemptsRemaining(void)
{
	return doCellularSimPinAttemptsRemaining();
}

ehs_bool EhsSim_PinEnable(ehs_bool enable, const ehs_char *pin)
{
	return doCellularSimPinEnable(enable, pin);
}

ehs_bool EhsSim_PinChange(const ehs_char *old_pin, const ehs_char *new_pin)
{
	return doCellularSimPinChange(old_pin, new_pin);
}

ehs_bool EhsSim_SoftIsProvisioned(void)
{
	return doCellularSimSoftIsProvisioned();
}

ehs_bool EhsSim_SoftProvision(const ehs_uint8 *blob, ehs_uint32 len)
{
	if ((blob == NULL) || (len == 0u)) {
		EhsCellSetFailReason(EhsCellFail_InvalidArg);
		return EHS_FALSE;
	}
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_SOFTSIM) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	return doCellularSimSoftProvision(blob, len);
}

/* ── eUICC: local lifecycle (CAP_EUICC_PROFILES) ──────────────────────────
 * ES10c operations - APDU exchanges with the chip, no network of any kind.
 * Gate on the capability, then delegate, the same shape EhsSim_SoftProvision
 * uses. These were hard-coded stubs returning NotSupported, which meant a
 * backend with a real eUICC could not implement them however much it wanted
 * to. */
ehs_sint32 EhsSim_ProfileCount(void)
{
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_PROFILES) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		/* -1, NOT 0. Zero would say "an eUICC with no profiles on it", which is
		 * a completely different situation from "no eUICC". */
		return -1;
	}
	return doCellularSimProfileCount();
}

ehs_bool EhsSim_ProfileGet(ehs_sint32 index, EhsSimProfileType *out)
{
	if ((out == NULL) || (index < 0)) {
		EhsCellSetFailReason(EhsCellFail_InvalidArg);
		return EHS_FALSE;
	}
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_PROFILES) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	return doCellularSimProfileGet(index, out);
}

ehs_bool EhsSim_ProfileEnable(const ehs_char *iccid)
{
	if ((iccid == NULL) || (iccid[0] == '\0')) {
		EhsCellSetFailReason(EhsCellFail_InvalidArg);
		return EHS_FALSE;
	}
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_PROFILES) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	/* Enabling a profile triggers a REFRESH: the UICC restarts and any live
	 * link drops. Callers must expect to re-attach afterwards. */
	return doCellularSimProfileEnable(iccid);
}

ehs_bool EhsSim_ProfileDisable(const ehs_char *iccid)
{
	if ((iccid == NULL) || (iccid[0] == '\0')) {
		EhsCellSetFailReason(EhsCellFail_InvalidArg);
		return EHS_FALSE;
	}
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_PROFILES) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	/* Leaves the eUICC with NO profile enabled and therefore no connectivity
	 * until something enables one. Enabling a different profile is the usual
	 * operation; this exists because ES10c defines it. */
	return doCellularSimProfileDisable(iccid);
}

ehs_bool EhsSim_ProfileDelete(const ehs_char *iccid)
{
	if ((iccid == NULL) || (iccid[0] == '\0')) {
		EhsCellSetFailReason(EhsCellFail_InvalidArg);
		return EHS_FALSE;
	}
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_PROFILES) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	/* Irreversible. A deleted profile has to be downloaded again. */
	return doCellularSimProfileDelete(iccid);
}

/* ── eUICC: remote provisioning (CAP_EUICC_DOWNLOAD) ──────────────────────
 * Separate capability from the lifecycle above: a device provisioned in the
 * factory has profiles to manage but no RSP stack to download more. */
ehs_bool EhsSim_ProfileSetEim(const ehs_char *eim_address)
{
	if ((eim_address == NULL) || (eim_address[0] == '\0')) {
		EhsCellSetFailReason(EhsCellFail_InvalidArg);
		return EHS_FALSE;
	}
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_DOWNLOAD) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	return doCellularSimProfileSetEim(eim_address);
}

ehs_bool EhsSim_ProfileDownload(const ehs_char *activation_code,
                               ehs_char *iccid_out, ehs_uint16 iccid_buf_size)
{
	if (iccid_out != NULL) {
		iccid_out[0] = '\0';
	}

	if ((activation_code == NULL) || (activation_code[0] == '\0')) {
		EhsCellSetFailReason(EhsCellFail_InvalidArg);
		return EHS_FALSE;
	}
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_DOWNLOAD) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	/* Needs IP reachability to the SM-DP+/eIM. That does NOT have to be the
	 * cellular link - a factory line can carry the session over Ethernet, Wi-Fi
	 * or a test fixture, which is what breaks the "need a network to get a SIM"
	 * circularity. */
	return doCellularSimProfileDownload(activation_code, iccid_out, iccid_buf_size);
}

/* ── eUICC: factory install (CAP_EUICC_FACTORY_INSTALL) ───────────────────
 * A third capability, separate from both the local lifecycle and RSP download,
 * because it is a third independent thing a board may or may not have: the
 * ability to accept a pre-built package over a local link. A device provisioned
 * this way typically has PROFILES (to enable what it was given) and
 * FACTORY_INSTALL, and no DOWNLOAD at all. */
ehs_bool EhsSim_EuiccGetEid(ehs_char *buf, ehs_uint16 buf_size)
{
	if ((buf == NULL) || (buf_size == 0u)) {
		EhsCellSetFailReason(EhsCellFail_InvalidArg);
		return EHS_FALSE;
	}
	buf[0] = '\0';
	/* Gated on PROFILES, not FACTORY_INSTALL: the EID identifies the chip and is
	 * useful for traceability on any board that has an eUICC at all, whether or
	 * not it can be provisioned locally. */
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_PROFILES) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	return doCellularSimEuiccGetEid(buf, buf_size);
}

ehs_sint32 EhsSim_FactoryInstallMaxSegment(void)
{
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_FACTORY_INSTALL) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return -1;
	}
	return doCellularSimFactoryInstallMaxSegment();
}

ehs_bool EhsSim_FactoryInstallBegin(void)
{
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_FACTORY_INSTALL) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	return doCellularSimFactoryInstallBegin();
}

ehs_bool EhsSim_FactoryInstallPushSegment(const ehs_uint8 *seg, ehs_uint32 len)
{
	if ((seg == NULL) || (len == 0u)) {
		EhsCellSetFailReason(EhsCellFail_InvalidArg);
		return EHS_FALSE;
	}
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_FACTORY_INSTALL) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	return doCellularSimFactoryInstallPushSegment(seg, len);
}

ehs_bool EhsSim_FactoryInstallCommit(ehs_char *iccid_out, ehs_uint16 iccid_buf_size)
{
	if (iccid_out != NULL) {
		iccid_out[0] = '\0';
	}
	if ((Cell_GetCapabilities() & (ehs_uint32)EHS_CELL_CAP_EUICC_FACTORY_INSTALL) == 0u) {
		EhsCellSetFailReason(EhsCellFail_NotSupported);
		return EHS_FALSE;
	}
	return doCellularSimFactoryInstallCommit(iccid_out, iccid_buf_size);
}

void EhsSim_FactoryInstallAbort(void)
{
	/* No capability gate and no return value on purpose. Abort must always be
	 * callable - it is the timeout action and the error path, and a caller that
	 * has to check whether it is allowed to clean up will sometimes not. */
	doCellularSimFactoryInstallAbort();
}

#endif /* EHS_NETWORK_CELLULAR_SUPPORT */
