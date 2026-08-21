/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file inx-cell_modem_manager.c
 * cell_modem_manager function block — bring the cellular link up/down and
 * report connection state plus modem/network identity.
 *
 * The link/identity split that Wi-Fi has (wifi_station vs a separate info
 * block) is deliberately NOT reproduced: every identity field here — operator,
 * MCC/MNC, cell ID, band — is only meaningful while attached, i.e. only when
 * this block already owns the connection state. Two blocks would have meant an
 * app had to know the info block was invalid until the link block reached
 * Online.
 *
 * ASYNC EVENTS. Connected/Failed/Disconnected hang off internal_monitor, whose
 * trigger is an InternalPort — the same arrangement wifi_station uses for its
 * internal_thread.
 *
 * The monitor thread does NOT write those ports itself. It detects the state
 * change, parks the result in the s_report_* statics, and then dispatches
 * through EhsCallbackQueue_execute(), which re-enters internal_monitor with the
 * function instance registered at init time. Only then are the outputs written.
 *
 * That indirection is the whole point, and it is not optional. EHS_FB_FINISH
 * and every EHS_FB_OUT_*_API2 resolve against `pFIdata`, and inside an
 * EHS_FB_THREAD_FUNCTION that is whatever instance called EHS_FB_START_THREAD.
 * do_connect starts the monitor too (so the first transition is not missed), so
 * a thread that wrote the ports directly would address do_connect's argument
 * list roughly half the time: EHS_FB_FINISH(..._connected), argument 1, would
 * fire do_connect's argument 1 — do_connect_OK — and the output writes would
 * land on ports do_connect does not have. Registering the callback binds the
 * dispatch to internal_monitor's OWN instance regardless of who started the
 * thread, which also makes the thread's own pFIdata irrelevant.
 *
 * The queue is registered in the init function and cleared in destroy: the
 * entry lives in FB context (APP_DATA pool) while the monitor thread
 * deliberately outlives an app reload, so leaving it registered would leave the
 * thread dispatching through freed pool memory on its next tick — the stale
 * pointer shape described in ../EHS-kernel/docs/memory-pools.md.
 *
 * @author: inx limited
 */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-cell_modem_manager.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal-api.h"
#include "hal_logger.h"
#include "hal_string.h"
#include "cellular.h"

/* Scratch buffer for do_read_info's three string getters. DERIVED from the
 * consumers, never picked: the dependency direction has to be constant -> code,
 * so that raising any one of the three cannot leave this one short. The #error
 * makes an impossible combination fail the build on every target rather than
 * misbehave on one - this started as a silently empty operator port, which is
 * exactly the failure mode a compile-time check removes. */
#define CELL_READ_INFO_BUFF_MAX EHS_CELL_OPERATOR_BUFF_MAX

#if (EHS_CELL_IMEI_BUFF_MAX     > CELL_READ_INFO_BUFF_MAX) || \
    (EHS_CELL_OPERATOR_BUFF_MAX > CELL_READ_INFO_BUFF_MAX) || \
    (EHS_CELL_FWVER_BUFF_MAX    > CELL_READ_INFO_BUFF_MAX)
#error "CELL_READ_INFO_BUFF_MAX is smaller than one of the getters it serves - \
raise it to the largest of EHS_CELL_IMEI/OPERATOR/FWVER_BUFF_MAX"
#endif

//ICB STATE VAR MACRO START -- DO NOT ALTER
typedef struct inx_cell_modem_manager_state
{
	EhsCallbackQueueEntryType xEntry;       /* internal_monitor callback slot   */
	EhsFunctionInstanceDataType *pFIdata;   /* set by internal_monitor          */
	ehs_bool   onStartup;
	ehs_sint32 rat;
	ehs_sint32 ratPreference;
	ehs_bool   tryReconnect;
	ehs_sint32 backoffInitial;
	ehs_sint32 backoffMax;

	ehs_char   apn[EHS_CELL_APN_BUFF_MAX];
	ehs_char   apn_user[EHS_CELL_USER_BUFF_MAX];
	ehs_char   apn_pass[EHS_CELL_PASS_BUFF_MAX];
} inx_cell_modem_manager_state_type;
//ICB STATE VAR MACRO END -- DO NOT ALTER

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(cell_modem_manager)
EHS_FB_FUNCTION_ENTRY("do_connect", 0x01, cell_modem_manager_do_connect)
EHS_FB_FUNCTION_ENTRY("do_disconnect", 0x02, cell_modem_manager_do_disconnect)
EHS_FB_FUNCTION_ENTRY("internal_monitor", 0x03, cell_modem_manager_internal_monitor)
EHS_FB_FUNCTION_ENTRY("do_set", 0x04, cell_modem_manager_do_set)
EHS_FB_FUNCTION_ENTRY("do_read_info", 0x05, cell_modem_manager_do_read_info)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_cell_modem_manager_ARG_do_connect_do_connect_OK 1
#define INX_cell_modem_manager_ARG_do_disconnect_do_disconnect_OK 1
#define INX_cell_modem_manager_ARG_internal_monitor_connected 1
#define INX_cell_modem_manager_ARG_internal_monitor_connect_failed 2
#define INX_cell_modem_manager_ARG_internal_monitor_disconnected 3
#define INX_cell_modem_manager_ARG_internal_monitor_state 1
#define INX_cell_modem_manager_ARG_internal_monitor_reg_status 2
#define INX_cell_modem_manager_ARG_internal_monitor_ip_address 3
#define INX_cell_modem_manager_ARG_internal_monitor_is_roaming 4
#define INX_cell_modem_manager_ARG_internal_monitor_active_rat 5
#define INX_cell_modem_manager_ARG_internal_monitor_fail_reason 6
#define INX_cell_modem_manager_ARG_do_set_apn 1
#define INX_cell_modem_manager_ARG_do_set_apn_user 2
#define INX_cell_modem_manager_ARG_do_set_apn_pass 3
#define INX_cell_modem_manager_ARG_do_set_set_rat 4
#define INX_cell_modem_manager_ARG_do_set_set_ok 1
#define INX_cell_modem_manager_ARG_do_read_info_read_info_ok 1
#define INX_cell_modem_manager_ARG_do_read_info_imei 1
#define INX_cell_modem_manager_ARG_do_read_info_operator_name 2
#define INX_cell_modem_manager_ARG_do_read_info_mcc 3
#define INX_cell_modem_manager_ARG_do_read_info_mnc 4
#define INX_cell_modem_manager_ARG_do_read_info_cell_id 5
#define INX_cell_modem_manager_ARG_do_read_info_tac 6
#define INX_cell_modem_manager_ARG_do_read_info_band 7
#define INX_cell_modem_manager_ARG_do_read_info_modem_fw 8
#define INX_cell_modem_manager_ARG_do_read_info_err_code 9
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
#define INX_FB_cell_modem_manager_onStartup 1
#define INX_FB_cell_modem_manager_rat 1
#define INX_FB_cell_modem_manager_ratPreference 0
#define INX_FB_cell_modem_manager_tryReconnect 1
#define INX_FB_cell_modem_manager_backoffInitial 60
#define INX_FB_cell_modem_manager_backoffMax 3600
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER

/* One monitor thread per image, not per instance: the cellular link is a
 * single system resource and two monitors would race to report it. */
static volatile ehs_bool s_monitor_running = EHS_FALSE;

/* Dispatch queue for internal_monitor. Registered in the init function against
 * the InternalPort's own function instance, so the monitor thread can reach
 * this block's async ports without borrowing another function's context. */
static EhsCallbackQueueType xCellModemManagerCallbackQueue;

/* Handoff from the monitor thread to internal_monitor. Written by the thread
 * immediately before EhsCallbackQueue_execute() and consumed on the other side
 * of that call - which is a plain synchronous call on the same thread, so no
 * publication barrier is needed between the two. */
static volatile EhsCellState s_report_state    = EhsCell_Idle;
static volatile ehs_sint32   s_report_event    = 0;      /* 0 = update outputs only */
static volatile ehs_bool     s_report_pending  = EHS_FALSE;

/** Map the CDF's rat parameter (0..3) onto the HAL bitmask. */
static EhsCellRat rat_from_param(ehs_sint32 v)
{
	switch (v) {
	case 1:  return EHS_CELL_RAT_LTEM;
	case 2:  return EHS_CELL_RAT_NBIOT;
	case 3:  return (EhsCellRat)(EHS_CELL_RAT_LTEM | EHS_CELL_RAT_NBIOT);
	case 0:
	default: return EHS_CELL_RAT_NONE;   /* leave the modem default alone */
	}
}

static void fill_config(inx_cell_modem_manager_state_type *st, EhsCellConfigType *cfg)
{
	memset(cfg, 0, sizeof(*cfg));
	cfg->rat        = rat_from_param(st->rat);
	cfg->preference = (EhsCellRatPref)st->ratPreference;
	cfg->apn        = st->apn;
	cfg->apn_user   = st->apn_user;
	cfg->apn_pass   = st->apn_pass;
	cfg->sim_pin    = NULL;   /* PIN belongs to sim_manager, deliberately */
}

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_IDENTIFY_FUNCTION(cell_modem_manager)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_cell_modem_manager_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_INIT_FUNCTION(cell_modem_manager)
{
	inx_cell_modem_manager_state_type *st =
		(inx_cell_modem_manager_state_type *)EHS_FB_INIT_CONTEXT;

	memset(st, 0, sizeof(*st));

	/* Bind internal_monitor to its own function instance. -1 is the first (and
	 * only) InternalPort declared in cell_modem_manager.cdf; the index is
	 * positional, so inserting another InternalPort ahead of it in the CDF
	 * would silently dispatch on the wrong instance. */
	EhsCallbackQueue_register(&xCellModemManagerCallbackQueue,
	                          EHS_FB_RUN_NAME(cell_modem_manager_internal_monitor),
	                          EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1),
	                          &(st->xEntry));

	st->onStartup      = INX_FB_cell_modem_manager_onStartup;
	st->rat            = INX_FB_cell_modem_manager_rat;
	st->ratPreference  = INX_FB_cell_modem_manager_ratPreference;
	st->tryReconnect   = INX_FB_cell_modem_manager_tryReconnect;
	st->backoffInitial = INX_FB_cell_modem_manager_backoffInitial;
	st->backoffMax     = INX_FB_cell_modem_manager_backoffMax;

	/* onStartup and tryReconnect are ehs_bool (1 byte) - %d would write 4. */
	EhsSscanf(EHS_FB_INIT_PARAMETERS, "%hhu %d %d %hhu %d %d",
	          &(st->onStartup), &(st->rat), &(st->ratPreference),
	          &(st->tryReconnect), &(st->backoffInitial), &(st->backoffMax));

	return EHS_TRUE;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(cell_modem_manager)
{
	inx_cell_modem_manager_state_type *st =
		(inx_cell_modem_manager_state_type *)EHS_FB_DESTROY_CONTEXT;

	/* CRITICAL - drop the dispatch registration before the context goes away.
	 *
	 * st->xEntry lives in the APP_DATA pool, which an app reload frees whole,
	 * but the monitor thread deliberately SURVIVES that so the link stays up
	 * between apps. Left registered, the thread's next state change would walk
	 * the queue into freed pool memory and call through whatever fpRunFunc and
	 * pFuncInst came back - the stale-pointer-past-a-pool-flush shape in
	 * ../EHS-kernel/docs/memory-pools.md. Clearing leaves an empty queue, which
	 * EhsCallbackQueue_execute() handles as a no-op, so the link keeps running
	 * with no FB to report to - exactly the intended behaviour between apps. */
	EhsCallbackQueue_clear(&xCellModemManagerCallbackQueue);

	if (st != NULL) {
		st->pFIdata = NULL;
	}

	/* Any report the thread staged but did not get to dispatch refers to the FB
	 * that is going away. Drop it so it cannot be delivered to the next app. */
	s_report_pending = EHS_FALSE;

	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

/*****************************************************************************/
/* Monitor thread — owns the async events                                    */

EHS_FB_THREAD_FUNCTION(cell_modem_manager_monitor)
{
	EhsCellState last = Cell_GetState();
	ehs_bool     reported_online = EHS_FALSE;

	/* pFIdata is deliberately UNUSED. It is whichever function instance called
	 * EHS_FB_START_THREAD, which is not necessarily internal_monitor's - see
	 * the note at the top of this file. Ports are written by internal_monitor
	 * on the far side of the callback dispatch below. */
	(void)pFIdata;

	while (s_monitor_running) {
		EhsCellState now = Cell_GetState();

		if (now != last) {
			ehs_sint32 ev = 0;

			last = now;

			/* PsmSleep is NOT a disconnect - the modem is still registered and
			 * data resumes on wake. Firing Disconnected here is what makes a
			 * correctly power-optimised device look broken. */
			if ((now == EhsCell_Online) || (now == EhsCell_PsmSleep)) {
				if (!reported_online) {
					reported_online = EHS_TRUE;
					ev = INX_cell_modem_manager_ARG_internal_monitor_connected;
				}
			} else if ((now == EhsCell_Failed) || (now == EhsCell_SimFailed) ||
			           (now == EhsCell_SimPinRequired) || (now == EhsCell_RegDenied)) {
				/* Terminal states: report once and stop claiming a link. */
				reported_online = EHS_FALSE;
				ev = INX_cell_modem_manager_ARG_internal_monitor_connect_failed;
			} else if (reported_online && (now == EhsCell_Reconnecting)) {
				reported_online = EHS_FALSE;
				ev = INX_cell_modem_manager_ARG_internal_monitor_disconnected;
			} else {
				/* A transitory state - outputs still refresh, no event. */
			}

			s_report_state   = now;
			s_report_event   = ev;
			s_report_pending = EHS_TRUE;

			/* Serialise cb dispatch with other non-EHS-thread FB writers via the
			 * fbIO mutex. EhsCallbackQueue_execute calls internal_monitor
			 * synchronously here on this monitor thread; matching the pattern in
			 * inx-lorawan.c / mqtt_publish.c / mqtt_subscribe.c. */
			EhsTPMutex_lock(EhsTPMutex_fbIO);
			EhsCallbackQueue_execute(&xCellModemManagerCallbackQueue);
			EhsTPMutex_unlock(EhsTPMutex_fbIO);

			/* Cleared whether or not anything consumed it: an empty queue (no
			 * app loaded) must not leave a stale report to be delivered late. */
			s_report_pending = EHS_FALSE;
		}

		EhsSleep(EHS_TIME_s(1));
	}
	return (EhsThreadFuncReturnType)0;
}

//ICB FUNCTION internal_monitor MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(cell_modem_manager_internal_monitor)
{
	inx_cell_modem_manager_state_type *st =
		(inx_cell_modem_manager_state_type *)EHS_FB_RUN_CONTEXT;

	st->pFIdata = EHS_FB_RUN_CONTEXT_REF;

	/* Two entry paths.
	 *
	 * 1. Dispatched by the monitor thread through the callback queue, with a
	 *    staged report to publish. This is the only place the async ports are
	 *    written, and it is reached with THIS function's instance, so
	 *    EHS_FB_OUT_*_API2 and EHS_FB_FINISH resolve against internal_monitor's
	 *    own argument list.
	 * 2. Triggered by the kernel on our InternalPort when the app starts, with
	 *    nothing staged. That call exists to get the monitor thread running. */
	if (s_report_pending) {
		if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_internal_monitor_state)) {
			EHS_FB_OUT_I_API2(INX_cell_modem_manager_ARG_internal_monitor_state) =
				(ehs_sint32)s_report_state;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_internal_monitor_reg_status)) {
			EHS_FB_OUT_I_API2(INX_cell_modem_manager_ARG_internal_monitor_reg_status) =
				(ehs_sint32)Cell_GetRegStatus();
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_internal_monitor_ip_address)) {
			EhsStrcpy(EHS_FB_OUT_S_API2(INX_cell_modem_manager_ARG_internal_monitor_ip_address),
			          (ehs_char *)Cell_GetIp());
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_internal_monitor_is_roaming)) {
			EHS_FB_OUT_B_API2(INX_cell_modem_manager_ARG_internal_monitor_is_roaming) =
				Cell_IsRoaming();
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_internal_monitor_active_rat)) {
			EHS_FB_OUT_I_API2(INX_cell_modem_manager_ARG_internal_monitor_active_rat) =
				(ehs_sint32)Cell_GetActiveRat();
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_internal_monitor_fail_reason)) {
			EHS_FB_OUT_I_API2(INX_cell_modem_manager_ARG_internal_monitor_fail_reason) =
				(ehs_sint32)Cell_GetLastFailReason();
		}

		/* Outputs are refreshed on every transition; only the three reportable
		 * transitions carry a finish event. */
		if (s_report_event != 0) {
			EHS_FB_FINISH(s_report_event);
		}
		return;
	}

	if (s_monitor_running) {
		return;
	}
	s_monitor_running = EHS_TRUE;
	EHS_FB_START_THREAD(cell_modem_manager_monitor, -99);
}//ICB FUNCTION internal_monitor MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_connect MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(cell_modem_manager_do_connect)
{
	inx_cell_modem_manager_state_type *st =
		(inx_cell_modem_manager_state_type *)EHS_FB_RUN_CONTEXT;
	EhsCellConfigType cfg;

	st->pFIdata = EHS_FB_RUN_CONTEXT_REF;

	/* Make sure the monitor is up before the link can change state, or the
	 * first transition is missed and Connected never fires.
	 *
	 * Starting it from here hands the thread do_connect's function instance,
	 * which is harmless ONLY because the thread reports through the callback
	 * queue rather than through its own pFIdata. Do not "simplify" the monitor
	 * to write ports directly - see the note at the top of this file. */
	if (!s_monitor_running) {
		s_monitor_running = EHS_TRUE;
		EHS_FB_START_THREAD(cell_modem_manager_monitor, -99);
	}

	fill_config(st, &cfg);
	if (!Cell_Connect(&cfg)) {
		EHSH_LOG_ERROR("cell_modem_manager: Cell_Connect() refused");
	}

	/* Returns immediately - an attach takes seconds to tens of minutes. The
	 * outcome arrives on Connected / Failed. */
	EHS_FB_FINISH(INX_cell_modem_manager_ARG_do_connect_do_connect_OK);
}//ICB FUNCTION do_connect MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_disconnect MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(cell_modem_manager_do_disconnect)
{
	(void)Cell_Disconnect();
	EHS_FB_FINISH(INX_cell_modem_manager_ARG_do_disconnect_do_disconnect_OK);
}//ICB FUNCTION do_disconnect MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_set MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(cell_modem_manager_do_set)
{
	inx_cell_modem_manager_state_type *st =
		(inx_cell_modem_manager_state_type *)EHS_FB_RUN_CONTEXT;

	if (EHS_FB_IN_CONNECTED_API2(INX_cell_modem_manager_ARG_do_set_apn)) {
		EhsStrncpy_s(st->apn, sizeof(st->apn),
		             EHS_FB_IN_S_API2(INX_cell_modem_manager_ARG_do_set_apn),
		             sizeof(st->apn) - 1u);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_cell_modem_manager_ARG_do_set_apn_user)) {
		EhsStrncpy_s(st->apn_user, sizeof(st->apn_user),
		             EHS_FB_IN_S_API2(INX_cell_modem_manager_ARG_do_set_apn_user),
		             sizeof(st->apn_user) - 1u);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_cell_modem_manager_ARG_do_set_apn_pass)) {
		EhsStrncpy_s(st->apn_pass, sizeof(st->apn_pass),
		             EHS_FB_IN_S_API2(INX_cell_modem_manager_ARG_do_set_apn_pass),
		             sizeof(st->apn_pass) - 1u);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_cell_modem_manager_ARG_do_set_set_rat)) {
		st->rat = EHS_FB_IN_I_API2(INX_cell_modem_manager_ARG_do_set_set_rat);
	}

	/* Settings apply on the next Connect. Changing RAT on a live link would
	 * drop it, which a Set port is not the place to do implicitly. */
	EHS_FB_FINISH(INX_cell_modem_manager_ARG_do_set_set_ok);
}//ICB FUNCTION do_set MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_read_info MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(cell_modem_manager_do_read_info)
{
	EhsCellInfoType info;
	/* ONE scratch buffer shared by three getters, so it must be sized for the
	 * LARGEST of them, not for whichever was written first. Sized off the FW
	 * version (41) it was 24 bytes short of what the operator name needs, and
	 * the operator port came back empty on every read: the nRF91 backend maps
	 * Cell_GetOperator onto modem_info_get_operator(), which rejects any buffer
	 * smaller than 65 with -EINVAL *before issuing the AT command*, so the
	 * failure was total and had nothing to do with network state. */
	ehs_char buf[CELL_READ_INFO_BUFF_MAX];
	ehs_sint32 err = (ehs_sint32)EhsCellFail_None;

	memset(&info, 0, sizeof(info));
	(void)Cell_GetCellInfo(&info);

	/* Each call is given ITS OWN limit rather than sizeof(buf): passing the
	 * whole scratch size would let a getter write past its own documented
	 * maximum, and it hides which constant each field actually depends on.
	 *
	 * Failures are no longer discarded. Each getter reports success, and the
	 * FIRST failure's reason is published on err_code - first rather than last
	 * so that the original fault is reported rather than whatever happened to
	 * fail afterwards. An empty string port with err_code 0 now means "the modem
	 * genuinely has nothing to report"; a non-zero err_code means the read
	 * itself failed, and the two used to be indistinguishable. */
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_do_read_info_imei)) {
		buf[0] = '\0';
		if (!Cell_GetImei(buf, (ehs_uint16)EHS_CELL_IMEI_BUFF_MAX) &&
		    (err == (ehs_sint32)EhsCellFail_None)) {
			err = (ehs_sint32)Cell_GetLastFailReason();
		}
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_cell_modem_manager_ARG_do_read_info_imei), buf);
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_do_read_info_operator_name)) {
		buf[0] = '\0';
		if (!Cell_GetOperator(buf, (ehs_uint16)EHS_CELL_OPERATOR_BUFF_MAX) &&
		    (err == (ehs_sint32)EhsCellFail_None)) {
			err = (ehs_sint32)Cell_GetLastFailReason();
		}
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_cell_modem_manager_ARG_do_read_info_operator_name), buf);
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_do_read_info_modem_fw)) {
		buf[0] = '\0';
		if (!Cell_GetModemFwVersion(buf, (ehs_uint16)EHS_CELL_FWVER_BUFF_MAX) &&
		    (err == (ehs_sint32)EhsCellFail_None)) {
			err = (ehs_sint32)Cell_GetLastFailReason();
		}
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_cell_modem_manager_ARG_do_read_info_modem_fw), buf);
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_do_read_info_mcc)) {
		EHS_FB_OUT_I_API2(INX_cell_modem_manager_ARG_do_read_info_mcc) = info.mcc;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_do_read_info_mnc)) {
		EHS_FB_OUT_I_API2(INX_cell_modem_manager_ARG_do_read_info_mnc) = info.mnc;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_do_read_info_cell_id)) {
		EHS_FB_OUT_I_API2(INX_cell_modem_manager_ARG_do_read_info_cell_id) = (ehs_sint32)info.cell_id;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_do_read_info_tac)) {
		EHS_FB_OUT_I_API2(INX_cell_modem_manager_ARG_do_read_info_tac) = (ehs_sint32)info.tac;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_do_read_info_band)) {
		EHS_FB_OUT_I_API2(INX_cell_modem_manager_ARG_do_read_info_band) = (ehs_sint32)info.band;
	}

	/* BOUNDS CHECK FIRST, and it is not redundant with the CONNECTED test.
	 *
	 * pOut[] is sized from the SODL by nNumOutputs, and EHS_FB_OUT_CONNECTED_API2
	 * does NOT range-check - it only tests pOut != NULL and pOut[x-1] != xDummy.
	 * err_code was added to this block's CDF after apps had already been
	 * exported, so an app built against the previous CDF has nNumOutputs == 8,
	 * and pOut[8] reads off the end of the array. The out-of-bounds slot held
	 * something that was not xDummy, so the connected test passed and the store
	 * went through a junk pointer: USAGE FAULT, "Unaligned memory access", in
	 * this function.
	 *
	 * Any port appended to an existing block needs a guard like this until every
	 * deployed SODL has been re-exported against the new CDF. Without it,
	 * adding an output is a hard-crash upgrade hazard for apps already in the
	 * field, which is far worse than the output simply not appearing. */
	if ((EHS_FB_RUN_FUNCTION_INSTANCE->nNumOutputs >=
	     (ehs_uint8)INX_cell_modem_manager_ARG_do_read_info_err_code) &&
	    EHS_FB_OUT_CONNECTED_API2(INX_cell_modem_manager_ARG_do_read_info_err_code)) {
		EHS_FB_OUT_I_API2(INX_cell_modem_manager_ARG_do_read_info_err_code) = err;
	}

	/* read_info_ok still fires on a partial read: the ports that DID resolve
	 * are valid and useful, and err_code says whether anything was missed. */
	EHS_FB_FINISH(INX_cell_modem_manager_ARG_do_read_info_read_info_ok);
}//ICB FUNCTION do_read_info MACRO END -- DO NOT ALTER THIS LINE
