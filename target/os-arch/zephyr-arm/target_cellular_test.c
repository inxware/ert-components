/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_cellular_test.c
 * TEST_FUNC harness for the eRT cellular (LTE-M / NB-IoT) HAL.
 *
 *   ./configure zephyr_arm-nrf9151_thingy91x-cellular
 *   make TEST_FUNC=test_cellular zephyr_cmake_gen
 *   make TEST_FUNC=test_cellular zephyr_build_docker
 *
 * Runs instead of EhsMain() (see the EHS_TEST_FUNC_OVERRIDE block in
 * target_main.c) so the modem can be exercised with no SODL, no filesystem and
 * no application in the way. Everything it touches is the PUBLIC API in
 * target/Component-HAL/cellular/cellular.h — the shared state machine in
 * Common/Components/networking/inx-cellular_link.c and this OS-arch's backend
 * in target_cellular.c. It deliberately does NOT call lte_lc directly; doing
 * that would test Nordic's library rather than the eRT layer on top of it.
 *
 * WHAT IS DELIBERATELY *NOT* TESTED, AND WHY
 * ------------------------------------------
 * Three parts of the API are destructive or irreversible and are reported as
 * SKIPPED rather than exercised. This is not laziness; an automated test that
 * touched them would eventually damage hardware or a customer's SIM:
 *
 *   EhsSim_PinEnter()      A SIM PIN allows three attempts and then requires a
 *                          PUK. A test that guesses, retries, or runs twice
 *                          against a PIN-locked SIM permanently locks it. The
 *                          test reads the PIN *state* and stops there.
 *   EhsSim_Select()        Cycles the modem (CFUN=0/1) and, on a board where
 *                          the slot is hardware-determined, cannot work anyway.
 *                          Reported via the capability bit instead.
 *   EhsSim_SoftProvision() Writes a SIM profile to flash. Needs a real
 *                          provider blob and is not reversible in-place.
 *
 * The PSM/eDRX steps DO make live requests, because those are negotiated with
 * the network and are harmless — the network simply grants or refuses.
 *
 * EXPECTED RUN TIME
 * -----------------
 * Dominated by the attach. LTE-M on a known-good SIM is usually 5-60 s; NB-IoT
 * is minutes, and a cold search on an unknown PLMN can legitimately take tens
 * of minutes. The test's own budget is derived from the SM's per-RAT deadlines
 * so it cannot declare failure while the SM is still legitimately working.
 *
 * @author: inx limited
 */

/* Module-scoped logger ID. Must precede every include: several headers pull in
 * hal_logger.h, which latches EHSL_MODULE_ID to UNDEFINED if it is not set yet. */
#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include <zephyr/kernel.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "globals.h"
#include "hal_logger.h"

#ifdef EHS_TEST_FUNC_OVERRIDE

#include "cellular.h"
#include "targetos_init.h"

/* ==========================================================================
 *                     >>>  TEST CONFIGURATION — EDIT  <<<
 * ==========================================================================
 *
 * APN: leave empty for a network-assigned APN. That is correct for most modern
 * networks and specifically for the pre-activated Onomondo / Wireless Logic
 * SIMs shipped with the Thingy:91 X kit. Only set it if your operator requires
 * a named APN.
 *
 * RAT: EHS_CELL_RAT_LTEM is the sane default for bring-up — its attach is far
 * quicker than NB-IoT, so a fault shows up in seconds rather than minutes.
 * Re-run with EHS_CELL_RAT_NBIOT once LTE-M passes; that comparison is the
 * evidence base for the SM's per-RAT timeouts.
 *
 * PSM/eDRX: requested, then the GRANTED values are reported. A network is free
 * to refuse or to grant something different from the ask, and reporting only
 * the request would hide exactly the thing you need to know.
 */
#define EHS_CELL_TEST_APN         ""      /* "" = network-assigned            */
#define EHS_CELL_TEST_RAT         EHS_CELL_RAT_LTEM
#define EHS_CELL_TEST_RAT_PREF    EHS_CELL_RAT_PREF_AUTO
#define EHS_CELL_TEST_PSM         1       /* 1 = request PSM                  */
#define EHS_CELL_TEST_PSM_TAU_S   3600    /* requested periodic-TAU, seconds  */
#define EHS_CELL_TEST_PSM_ACT_S   60      /* requested active time, seconds   */
#define EHS_CELL_TEST_EDRX        1       /* 1 = request eDRX                 */

/* Connectivity test peer, used for both the ping and the HTTP fetch.
 *
 * NOTE this is a PUBLIC address, so the test proves end-to-end internet
 * reachability over the cellular APN - not just attach. Two things outside our
 * control can make it "fail" while the stack is perfectly healthy:
 *   - many hosts do not answer ICMP echo at all, and a good number of cellular
 *     APNs block ICMP outright;
 *   - not every host serves plain HTTP on port 80.
 * The two phases are therefore reported separately, and a ping failure does
 * NOT stop the HTTP phase from running. */
#define EHS_CELL_TEST_PEER_IP     "192.178.223.100"
#define EHS_CELL_TEST_HTTP_PORT   80
#define EHS_CELL_TEST_HTTP_PATH   "/"
#define EHS_CELL_TEST_PING_COUNT  4
#define EHS_CELL_TEST_PING_TMO_MS 5000
#define EHS_CELL_TEST_HTTP_TMO_MS 15000
/* Bytes of the HTTP response to print. Enough for the status line and the
 * headers; the body is counted but not dumped, because a metered link is no
 * place to print a whole page to a 115200 baud console. */
#define EHS_CELL_TEST_HTTP_PRINT  512

/* ========================================================================== */

/* The console on this board is USB-CDC via the nRF5340 bridge and takes a few
 * seconds to enumerate on the host; anything printed before then is lost. Same
 * reasoning (and same value) as the delay in target_main.c's EHS thread. */
#define CELL_TEST_CONSOLE_SETTLE_MS 6000

#define CELL_TEST_POLL_MS         500
#define CELL_TEST_STATUS_EVERY_MS 15000

/* Modem library up + SIM readable. Derived from the SM's own deadlines so the
 * test cannot time out a phase the SM is still legitimately inside. */
#define CELL_TEST_MODEM_TIMEOUT_MS \
	((ehs_uint32)(CELL_SM_TIMEOUT_MODEM_INIT_MS + CELL_SM_TIMEOUT_SIM_MS + 5000UL))

/* Attach budget. The SM already bounds the search per RAT; the test allows one
 * full attach plus the PDN phase plus a margin. Using the NB-IoT figure
 * unconditionally is deliberate — timing out early on a slow-but-working
 * network is the failure mode that wastes the most time to diagnose. */
#define CELL_TEST_ATTACH_TIMEOUT_MS \
	((ehs_uint32)(CELL_SM_TIMEOUT_ATTACH_NBIOT_MS + CELL_SM_TIMEOUT_PDN_MS + 30000UL))

#define TEST_LOG(fmt, ...) do { \
	printf("CELL-TEST: " fmt "\n", ##__VA_ARGS__); \
	fflush(stdout); \
} while (0)

/* Result tally, printed as a summary at the end. */
static ehs_uint16 s_pass;
static ehs_uint16 s_fail;
static ehs_uint16 s_skip;

#define T_PASS(fmt, ...) do { s_pass++; TEST_LOG("  [PASS] " fmt, ##__VA_ARGS__); } while (0)
#define T_FAIL(fmt, ...) do { s_fail++; TEST_LOG("  [FAIL] " fmt, ##__VA_ARGS__); } while (0)
#define T_SKIP(fmt, ...) do { s_skip++; TEST_LOG("  [SKIP] " fmt, ##__VA_ARGS__); } while (0)
#define T_INFO(fmt, ...) do {           TEST_LOG("         " fmt, ##__VA_ARGS__); } while (0)

/*****************************************************************************/
/* Helpers                                                                   */

/** Render a signal value, honouring the invalid sentinel. */
static const ehs_char *sig_str(ehs_char *buf, ehs_uint16 len, ehs_sint16 v, const ehs_char *unit)
{
	if (v == EHS_CELL_VALUE_INVALID) {
		(void)snprintf(buf, len, "n/a");
	} else {
		(void)snprintf(buf, len, "%d %s", (int)v, unit);
	}
	return buf;
}

static const ehs_char *sim_state_name(EhsSimState s)
{
	switch (s) {
	case EHS_SIM_STATE_ABSENT:       return "ABSENT";
	case EHS_SIM_STATE_PIN_REQUIRED: return "PIN_REQUIRED";
	case EHS_SIM_STATE_PUK_REQUIRED: return "PUK_REQUIRED";
	case EHS_SIM_STATE_READY:        return "READY";
	case EHS_SIM_STATE_FAILED:       return "FAILED";
	default:                         return "UNKNOWN";
	}
}

static const ehs_char *sim_slot_name(EhsSimSlot s)
{
	switch (s) {
	case EHS_SIM_SLOT_PHYSICAL: return "PHYSICAL (4FF socket / eSIM behind the switch)";
	case EHS_SIM_SLOT_EMBEDDED: return "EMBEDDED (soldered eUICC)";
	case EHS_SIM_SLOT_SOFT:     return "SOFT (SoftSIM)";
	case EHS_SIM_SLOT_ISIM:     return "ISIM";
	default:                    return "?";
	}
}

static const ehs_char *reg_name(EhsCellRegStatus r)
{
	switch (r) {
	case EhsCellReg_NotRegistered:     return "not registered";
	case EhsCellReg_RegisteredHome:    return "registered (home)";
	case EhsCellReg_Searching:         return "searching";
	case EhsCellReg_Denied:            return "REGISTRATION DENIED";
	case EhsCellReg_RegisteredRoaming: return "registered (ROAMING)";
	case EhsCellReg_UiccFail:          return "UICC FAILURE";
	default:                           return "unknown";
	}
}

static const ehs_char *rat_name(EhsCellRat r)
{
	if ((r & (EHS_CELL_RAT_LTEM | EHS_CELL_RAT_NBIOT)) ==
	    (EHS_CELL_RAT_LTEM | EHS_CELL_RAT_NBIOT)) {
		return "LTE-M + NB-IoT";
	}
	if (r & EHS_CELL_RAT_LTEM)  { return "LTE-M";  }
	if (r & EHS_CELL_RAT_NBIOT) { return "NB-IoT"; }
	return "none";
}

static const ehs_char *energy_name(EhsCellEnergyEstimate e)
{
	switch (e) {
	case EhsCellEnergy_Excessive: return "EXCESSIVE (do not transmit if avoidable)";
	case EhsCellEnergy_Increased: return "increased";
	case EhsCellEnergy_Normal:    return "normal";
	case EhsCellEnergy_Reduced:   return "reduced";
	case EhsCellEnergy_Efficient: return "efficient (good time to send)";
	default:                      return "unknown";
	}
}

/**
 * Poll Cell_GetState() until it reaches a wanted state or a terminal one,
 * printing every transition on the way.
 *
 * Terminal states short-circuit the wait: SimPinRequired, SimFailed, RegDenied
 * and Failed are by design never retried by the SM, so continuing to wait for
 * the full budget would just burn minutes to reach the same conclusion.
 *
 * @return the state reached, or the last state seen on timeout
 */
static EhsCellState cell_wait_state(EhsCellState wanted, ehs_uint32 timeout_ms)
{
	EhsCellState last = Cell_GetState();
	ehs_uint32 waited = 0;

	TEST_LOG("  state: %s", Cell_GetStateName(last));

	while (last != wanted) {
		EhsCellState now;

		if ((last == EhsCell_Failed) || (last == EhsCell_SimFailed) ||
		    (last == EhsCell_SimPinRequired) || (last == EhsCell_RegDenied)) {
			TEST_LOG("  terminal state %s reached after %u ms - not waiting further",
			         Cell_GetStateName(last), (unsigned)waited);
			break;
		}
		if (waited >= timeout_ms) {
			TEST_LOG("  TIMEOUT after %u ms in state %s",
			         (unsigned)waited, Cell_GetStateName(last));
			break;
		}
		k_msleep(CELL_TEST_POLL_MS);
		waited += CELL_TEST_POLL_MS;

		now = Cell_GetState();
		if (now != last) {
			last = now;
			TEST_LOG("  state: %s (+%u ms)", Cell_GetStateName(last), (unsigned)waited);
		}
	}
	return last;
}

/*****************************************************************************/
/* Test phases                                                               */

static ehs_uint32 s_caps;

static void phase_capabilities(void)
{
	TEST_LOG("[1/11] capabilities");

	s_caps = Cell_GetCapabilities();
	if (s_caps == 0u) {
		T_FAIL("Cell_GetCapabilities() == 0 - the stubbed backend is linked.");
		T_INFO("Check EHS_CELLULAR_SUPPORT=zephyr_nrf91 in the platform config.");
		return;
	}
	T_PASS("Cell_GetCapabilities() = 0x%08X", (unsigned)s_caps);

#define CAP(bit, name) \
	T_INFO("  %-22s %s", name, (s_caps & (ehs_uint32)(bit)) ? "yes" : "no")
	CAP(EHS_CELL_CAP_LTEM,            "LTE-M");
	CAP(EHS_CELL_CAP_NBIOT,           "NB-IoT");
	CAP(EHS_CELL_CAP_PSM,             "PSM");
	CAP(EHS_CELL_CAP_EDRX,            "eDRX");
	CAP(EHS_CELL_CAP_RAI,             "RAI");
	CAP(EHS_CELL_CAP_CONN_EVAL,       "link evaluation");
	CAP(EHS_CELL_CAP_ENERGY_ESTIMATE, "energy estimate");
	CAP(EHS_CELL_CAP_CE_LEVEL,        "CE level");
	CAP(EHS_CELL_CAP_NEIGHBOUR_MEAS,  "neighbour cells");
	CAP(EHS_CELL_CAP_DATA_COUNTERS,   "data counters");
	CAP(EHS_CELL_CAP_SIM_PIN,         "SIM PIN");
	CAP(EHS_CELL_CAP_SIM_SELECT,      "SIM slot select");
	CAP(EHS_CELL_CAP_SOFTSIM,         "SoftSIM");
	CAP(EHS_CELL_CAP_EUICC_PROFILES,  "eUICC profiles");
	CAP(EHS_CELL_CAP_TLS_OFFLOAD,     "TLS offload");
#undef CAP
}

static void phase_sim(void)
{
	ehs_char buf[EHS_CELL_ICCID_BUFF_MAX];
	EhsSimState st;
	EhsSimSlot slot;
	ehs_uint32 slots;
	ehs_sint32 attempts;

	TEST_LOG("[3/11] SIM");

	st = EhsSim_GetState();
	switch (st) {
	case EHS_SIM_STATE_READY:
		T_PASS("EhsSim_GetState() = READY");
		break;
	case EHS_SIM_STATE_ABSENT:
		T_FAIL("EhsSim_GetState() = ABSENT - no SIM detected.");
		T_INFO("Insert a nano-SIM (4FF) in socket J4, contacts down, notch as");
		T_INFO("moulded. The board must be power-cycled after inserting: the");
		T_INFO("UICC interface is only probed at modem init.");
		break;
	case EHS_SIM_STATE_PIN_REQUIRED:
		T_FAIL("EhsSim_GetState() = PIN_REQUIRED");
		T_INFO("This test does NOT enter the PIN - three wrong attempts require a");
		T_INFO("PUK and would permanently lock the SIM. Disable the PIN on the SIM");
		T_INFO("using a phone, or supply it via EhsCellConfigType.sim_pin in your");
		T_INFO("own code, having verified it is correct.");
		break;
	default:
		T_FAIL("EhsSim_GetState() = %s", sim_state_name(st));
		break;
	}

	slot = EhsSim_GetActiveSlot();
	T_INFO("active slot: %s", sim_slot_name(slot));

	/* Two different questions - see the note on EhsSimSlot in cellular.h.
	 * "Available" is what exists; "selectable" is what software can switch to. */
	slots = EhsSim_GetAvailableSlots();
	T_INFO("SIM types available  : 0x%02X  %s%s%s", (unsigned)slots,
	       (slots & (1u << EHS_SIM_SLOT_PHYSICAL)) ? "SIM "     : "",
	       (slots & (1u << EHS_SIM_SLOT_SOFT))     ? "SoftSIM " : "",
	       (slots & (1u << EHS_SIM_SLOT_EMBEDDED)) ? "eSIM "    : "");
	{
		ehs_uint32 sel = EhsSim_GetSelectableSlots();

		T_INFO("SIM types selectable : 0x%02X  %s%s%s", (unsigned)sel,
		       (sel & (1u << EHS_SIM_SLOT_PHYSICAL)) ? "SIM "     : "",
		       (sel & (1u << EHS_SIM_SLOT_SOFT))     ? "SoftSIM " : "",
		       (sel & (1u << EHS_SIM_SLOT_EMBEDDED)) ? "eSIM "    : "");
		if ((slots & ~sel) != 0u) {
			T_INFO("  (a type that is available but NOT selectable is chosen by");
			T_INFO("   hardware - on this board the 4FF socket and the eSIM");
			T_INFO("   footprint share one UICC interface behind a card-detect");
			T_INFO("   autoswitch, so inserting a card is what picks it.)");
		}
	}
	T_SKIP("EhsSim_Select() - disruptive (drops the link and re-attaches); not run.");

	attempts = EhsSim_PinAttemptsRemaining();
	T_INFO("PIN attempts remaining: %s",
	       (attempts < 0) ? "unknown (modem does not report it)" : "see value below");
	if (attempts >= 0) {
		T_INFO("  %d", (int)attempts);
	}
	T_SKIP("EhsSim_PinEnter() / PinEnable / PinChange - can permanently lock the SIM.");

	buf[0] = '\0';
	if (EhsSim_GetIccid(buf, (ehs_uint16)sizeof(buf)) && (buf[0] != '\0')) {
		T_PASS("EhsSim_GetIccid() = %s", buf);
	} else {
		T_FAIL("EhsSim_GetIccid() returned nothing");
	}

	buf[0] = '\0';
	if (EhsSim_GetImsi(buf, (ehs_uint16)sizeof(buf)) && (buf[0] != '\0')) {
		T_PASS("EhsSim_GetImsi() = %s", buf);
	} else {
		T_FAIL("EhsSim_GetImsi() returned nothing");
	}

	if (s_caps & (ehs_uint32)EHS_CELL_CAP_SOFTSIM) {
		T_INFO("SoftSIM provisioned: %s",
		       EhsSim_SoftIsProvisioned() ? "yes" : "no (module not built in)");
	}
	T_SKIP("EhsSim_SoftProvision() - writes a profile to flash; needs a real blob.");
}

static void phase_identity(void)
{
	ehs_char buf[EHS_CELL_FWVER_BUFF_MAX];

	TEST_LOG("[4/11] modem identity");

	buf[0] = '\0';
	if (Cell_GetImei(buf, (ehs_uint16)sizeof(buf)) && (buf[0] != '\0')) {
		T_PASS("Cell_GetImei() = %s", buf);
	} else {
		T_FAIL("Cell_GetImei() returned nothing");
	}

	buf[0] = '\0';
	if (Cell_GetModemFwVersion(buf, (ehs_uint16)sizeof(buf)) && (buf[0] != '\0')) {
		T_PASS("Cell_GetModemFwVersion() = %s", buf);
	} else {
		T_FAIL("Cell_GetModemFwVersion() returned nothing");
	}
}

static void phase_rat(void)
{
	EhsCellRat rat = EHS_CELL_RAT_NONE;
	EhsCellRatPref pref = EHS_CELL_RAT_PREF_AUTO;

	TEST_LOG("[5/11] radio access technology");

	if (Cell_GetRat(&rat, &pref)) {
		T_PASS("Cell_GetRat() enabled = %s (pref %d)", rat_name(rat), (int)pref);
	} else {
		T_FAIL("Cell_GetRat() failed");
	}

	/* The ENABLED set and the ACTIVE mode are different questions — this is
	 * the distinction the API exists to preserve. Before attach the active
	 * mode is legitimately 'none'. */
	T_INFO("Cell_GetActiveRat() = %s (expected 'none' before attach)",
	       rat_name(Cell_GetActiveRat()));

	if ((s_caps & (ehs_uint32)EHS_CELL_CAP_BAND_MASK) == 0u) {
		T_SKIP("Cell_SetBandMask() - not implemented on this backend.");
	}
}

static void phase_signal_and_cell(void)
{
	EhsCellSignalType sig;
	EhsCellLinkEvalType eval;
	EhsCellInfoType info;
	ehs_char a[16], b[16], c[16];
	ehs_char op[EHS_CELL_OPERATOR_BUFF_MAX];

	TEST_LOG("[7/11] signal, link evaluation and serving cell");

	memset(&sig, 0, sizeof(sig));
	if (Cell_GetSignal(&sig)) {
		T_PASS("Cell_GetSignal() RSRP %s  RSRQ %s  SNR %s",
		       sig_str(a, sizeof(a), sig.rsrp_dbm, "dBm"),
		       sig_str(b, sizeof(b), sig.rsrq_db, "dB"),
		       sig_str(c, sizeof(c), sig.snr_db, "dB"));
		/* Rough sanity band for a working LTE link. Outside it, the link may
		 * still work but is worth flagging. */
		if ((sig.rsrp_dbm != EHS_CELL_VALUE_INVALID) &&
		    ((sig.rsrp_dbm > -50) || (sig.rsrp_dbm < -130))) {
			T_INFO("  NOTE: RSRP outside the usual -50..-130 dBm range. The Thingy:91 X");
			T_INFO("  LTE antenna is INTERNAL (600-2200 MHz, with an active front end),");
			T_INFO("  so there is nothing to plug in - move the board away from metal");
			T_INFO("  and mains adapters, and try near a window.");
		}
	} else {
		T_FAIL("Cell_GetSignal() failed");
	}

	if (s_caps & (ehs_uint32)EHS_CELL_CAP_CONN_EVAL) {
		memset(&eval, 0, sizeof(eval));
		if (Cell_EvalLink(&eval)) {
			T_PASS("Cell_EvalLink() ok");
			T_INFO("  energy estimate : %s", energy_name(eval.energy));
			T_INFO("  CE level        : %s", sig_str(a, sizeof(a), eval.ce_level, ""));
			T_INFO("  TX power        : %s", sig_str(b, sizeof(b), eval.tx_power_dbm, "dBm"));
			T_INFO("  TX/RX reps      : %s / %s",
			       sig_str(a, sizeof(a), eval.tx_repetitions, ""),
			       sig_str(c, sizeof(c), eval.rx_repetitions, ""));
			T_INFO("  DL pathloss     : %s", sig_str(b, sizeof(b), eval.dl_pathloss_db, "dB"));
			T_INFO("  band            : %s", sig_str(c, sizeof(c), eval.band, ""));
		} else {
			/* Not a failure: conn_eval is only valid when registered and idle. */
			T_INFO("Cell_EvalLink() unavailable right now (valid only when");
			T_INFO("  registered and RRC-idle - retry from the monitor loop).");
		}
	} else {
		T_SKIP("Cell_EvalLink() - EHS_CELL_CAP_CONN_EVAL not advertised.");
	}

	memset(&info, 0, sizeof(info));
	if (Cell_GetCellInfo(&info)) {
		T_PASS("Cell_GetCellInfo() MCC %d MNC %d cell 0x%08X TAC %u EARFCN %u",
		       (int)info.mcc, (int)info.mnc, (unsigned)info.cell_id,
		       (unsigned)info.tac, (unsigned)info.earfcn);
		T_INFO("  band: %s", sig_str(a, sizeof(a), info.band, ""));
	} else {
		T_FAIL("Cell_GetCellInfo() - no serving cell recorded");
	}

	op[0] = '\0';
	if (Cell_GetOperator(op, (ehs_uint16)sizeof(op)) && (op[0] != '\0')) {
		T_PASS("Cell_GetOperator() = %s", op);
	} else {
		T_FAIL("Cell_GetOperator() returned nothing");
	}
}

static void phase_power_saving(void)
{
	EhsCellPsmType psm;
	EhsCellEdrxType edrx;

	TEST_LOG("[8/11] power saving (request vs GRANTED)");

	if (s_caps & (ehs_uint32)EHS_CELL_CAP_PSM) {
#if EHS_CELL_TEST_PSM
		if (Cell_PsmRequest(EHS_TRUE, EHS_CELL_TEST_PSM_TAU_S, EHS_CELL_TEST_PSM_ACT_S)) {
			T_PASS("Cell_PsmRequest(TAU %d s, active %d s) accepted by the modem",
			       (int)EHS_CELL_TEST_PSM_TAU_S, (int)EHS_CELL_TEST_PSM_ACT_S);
		} else {
			T_FAIL("Cell_PsmRequest() rejected");
		}
		/* The grant arrives asynchronously in a network signalling message. */
		k_msleep(5000);
#endif
		memset(&psm, 0, sizeof(psm));
		if (Cell_PsmGetGranted(&psm)) {
			if (psm.granted) {
				T_PASS("PSM GRANTED: TAU %d s, active %d s",
				       (int)psm.tau_s, (int)psm.active_s);
				T_INFO("  Note the granted values may differ from the request -");
				T_INFO("  that is normal and is why the API reports both.");
			} else {
				T_INFO("PSM requested but NOT granted by this network. Common;");
				T_INFO("  not a fault. The device simply will not enter PSM here.");
			}
		} else {
			T_FAIL("Cell_PsmGetGranted() failed");
		}
	} else {
		T_SKIP("PSM - capability not advertised.");
	}

	if (s_caps & (ehs_uint32)EHS_CELL_CAP_EDRX) {
#if EHS_CELL_TEST_EDRX
		if (Cell_EdrxRequest(EHS_TRUE, EHS_CELL_TEST_RAT, 0)) {
			T_PASS("Cell_EdrxRequest() accepted by the modem");
		} else {
			T_FAIL("Cell_EdrxRequest() rejected");
		}
		k_msleep(5000);
#endif
		memset(&edrx, 0, sizeof(edrx));
		if (Cell_EdrxGetGranted(&edrx)) {
			if (edrx.granted) {
				T_PASS("eDRX GRANTED: cycle %d ms, PTW %d ms",
				       (int)edrx.cycle_ms, (int)edrx.ptw_ms);
			} else {
				T_INFO("eDRX requested but NOT granted by this network.");
			}
		} else {
			T_FAIL("Cell_EdrxGetGranted() failed");
		}
	} else {
		T_SKIP("eDRX - capability not advertised.");
	}

	if ((s_caps & (ehs_uint32)EHS_CELL_CAP_RAI) == 0u) {
		T_SKIP("Cell_RaiSet() - not implemented on this backend.");
	}
}

static void phase_counters(void)
{
	ehs_uint32 tx = 0, rx = 0;

	TEST_LOG("[9/11] data counters and power state");

	if (s_caps & (ehs_uint32)EHS_CELL_CAP_DATA_COUNTERS) {
		if (Cell_GetDataCounters(&tx, &rx)) {
			T_PASS("Cell_GetDataCounters() tx %u B, rx %u B",
			       (unsigned)tx, (unsigned)rx);
			T_INFO("  Resolution is 1 KB - the modem reports kilobytes.");
		} else {
			T_INFO("Cell_GetDataCounters() unavailable (needs");
			T_INFO("  modem_info_connectivity_stats_init() first - call");
			T_INFO("  Cell_ResetDataCounters() once after attach).");
			if (Cell_ResetDataCounters()) {
				T_PASS("Cell_ResetDataCounters() enabled statistics collection");
			}
		}
	} else {
		T_SKIP("data counters - capability not advertised.");
	}

	{
		EhsCellPowerState ps = Cell_GetPowerState();
		static const ehs_char *const names[] = {
			"OFF", "ON", "AIRPLANE", "UICC_ONLY"
		};
		if (ps < EHS_CELL_POWER_MAX) {
			T_PASS("Cell_GetPowerState() = %s", names[ps]);
		} else {
			T_FAIL("Cell_GetPowerState() returned %d", (int)ps);
		}
	}
	/* Cell_SetPowerState is exercised only at the very end (see the monitor
	 * loop note) — flipping the radio off mid-test would invalidate every
	 * measurement after it. */
	T_SKIP("Cell_SetPowerState() - would drop the link; not run mid-test.");
}


/*****************************************************************************/
/* Phase 10 - ICMP ping                                                      */
/*
 * On nRF91 there is no ICMP socket: sockets are offloaded to the modem and it
 * offers a RAW PACKET socket instead, on which the application builds the whole
 * IPv4 frame itself. That is what NCS's own modem_shell ping does
 * (nrf/samples/cellular/modem_shell/src/ping/icmp_ping.c) and this follows the
 * same construction - 20-byte IPv4 header, 8-byte ICMP header, one's-complement
 * checksums over each.
 *
 * The source address must be filled in explicitly, since nothing below us knows
 * it; we take it from Cell_GetIp().
 */

#define PING_IPV4_HDR_LEN 20
#define PING_ICMP_HDR_LEN 8
#define PING_PAYLOAD_LEN  32
#define PING_FRAME_LEN    (PING_IPV4_HDR_LEN + PING_ICMP_HDR_LEN + PING_PAYLOAD_LEN)
#define PING_ICMP_ECHO_REQ 8
#define PING_ICMP_ECHO_REP 0
#define PING_PROTO_ICMP    1

/* Static, not on the stack: the test runs on the EHS thread and a 128-byte
 * frame plus a receive buffer is not worth risking there. */
static ehs_uint8 s_ping_tx[PING_FRAME_LEN];
static ehs_uint8 s_ping_rx[256];

/** One's-complement checksum, as RFC 1071. */
static ehs_uint16 ping_checksum(const ehs_uint8 *buf, int len)
{
	ehs_uint32 sum = 0;
	int i;

	for (i = 0; i + 1 < len; i += 2) {
		sum += (ehs_uint32)((buf[i] << 8) | buf[i + 1]);
	}
	if (i < len) {
		sum += (ehs_uint32)(buf[i] << 8);
	}
	while (sum > 0xFFFFu) {
		sum = (sum & 0xFFFFu) + (sum >> 16);
	}
	return (ehs_uint16)(~sum);
}

static void ping_put16(ehs_uint8 *p, ehs_uint16 v)
{
	p[0] = (ehs_uint8)(v >> 8);
	p[1] = (ehs_uint8)(v & 0xFF);
}

static void phase_ping(void)
{
	int fd = -1;
	struct timeval tv;
	struct in_addr src, dst;
	ehs_uint16 seq;
	int sent = 0, recvd = 0;
	ehs_sint64 rtt_min = 0, rtt_max = 0, rtt_sum = 0;

	TEST_LOG("[10/11] ICMP ping %s", EHS_CELL_TEST_PEER_IP);

	if (inet_pton(AF_INET, Cell_GetIp(), &src) != 1) {
		T_FAIL("cannot parse our own IP '%s' - cannot build an IP header", Cell_GetIp());
		return;
	}
	if (inet_pton(AF_INET, EHS_CELL_TEST_PEER_IP, &dst) != 1) {
		T_FAIL("cannot parse peer address " EHS_CELL_TEST_PEER_IP);
		return;
	}

	/* AF_PACKET/SOCK_RAW, not SOCK_RAW/IPPROTO_ICMP: the nRF91 modem exposes a
	 * raw packet socket and expects the full IPv4 frame. */
	fd = socket(AF_PACKET, SOCK_RAW, 0);
	if (fd < 0) {
		T_FAIL("raw socket() failed, errno %d", errno);
		T_INFO("A raw packet socket is how ping is done on this modem. If this");
		T_INFO("is EAFNOSUPPORT the offload layer did not accept AF_PACKET.");
		return;
	}

	tv.tv_sec  = EHS_CELL_TEST_PING_TMO_MS / 1000;
	tv.tv_usec = (EHS_CELL_TEST_PING_TMO_MS % 1000) * 1000;
	(void)setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	(void)setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

	for (seq = 1; seq <= (ehs_uint16)EHS_CELL_TEST_PING_COUNT; seq++) {
		ehs_sint64 t0, rtt;
		int n, i;

		memset(s_ping_tx, 0, sizeof(s_ping_tx));

		/* ---- IPv4 header ---- */
		s_ping_tx[0] = (ehs_uint8)((4u << 4) | (PING_IPV4_HDR_LEN / 4u));
		ping_put16(&s_ping_tx[2], (ehs_uint16)PING_FRAME_LEN);   /* total length */
		s_ping_tx[8] = 64;                                       /* TTL          */
		s_ping_tx[9] = PING_PROTO_ICMP;
		memcpy(&s_ping_tx[12], &src.s_addr, 4);
		memcpy(&s_ping_tx[16], &dst.s_addr, 4);
		ping_put16(&s_ping_tx[10], ping_checksum(s_ping_tx, PING_IPV4_HDR_LEN));

		/* ---- ICMP echo request ---- */
		{
			ehs_uint8 *icmp = &s_ping_tx[PING_IPV4_HDR_LEN];

			icmp[0] = PING_ICMP_ECHO_REQ;
			ping_put16(&icmp[4], 0xE175);      /* identifier */
			ping_put16(&icmp[6], seq);
			for (i = 0; i < PING_PAYLOAD_LEN; i++) {
				icmp[PING_ICMP_HDR_LEN + i] = (ehs_uint8)('0' + ((i + seq) % 10));
			}
			ping_put16(&icmp[2],
			           ping_checksum(icmp, PING_ICMP_HDR_LEN + PING_PAYLOAD_LEN));
		}

		t0 = (ehs_sint64)k_uptime_get();
		if (send(fd, s_ping_tx, PING_FRAME_LEN, 0) < 0) {
			T_INFO("  seq %u: send failed, errno %d", (unsigned)seq, errno);
			continue;
		}
		sent++;

		n = (int)recv(fd, s_ping_rx, sizeof(s_ping_rx), 0);
		rtt = (ehs_sint64)k_uptime_get() - t0;

		if (n < (PING_IPV4_HDR_LEN + PING_ICMP_HDR_LEN)) {
			T_INFO("  seq %u: no reply within %d ms", (unsigned)seq,
			       (int)EHS_CELL_TEST_PING_TMO_MS);
			continue;
		}
		if ((s_ping_rx[9] != PING_PROTO_ICMP) ||
		    (s_ping_rx[PING_IPV4_HDR_LEN] != PING_ICMP_ECHO_REP)) {
			T_INFO("  seq %u: reply was not an ICMP echo reply (proto %u type %u)",
			       (unsigned)seq, (unsigned)s_ping_rx[9],
			       (unsigned)s_ping_rx[PING_IPV4_HDR_LEN]);
			continue;
		}

		recvd++;
		rtt_sum += rtt;
		if ((recvd == 1) || (rtt < rtt_min)) { rtt_min = rtt; }
		if (rtt > rtt_max) { rtt_max = rtt; }
		T_INFO("  seq %u: reply from %s, %d bytes, %d ms",
		       (unsigned)seq, EHS_CELL_TEST_PEER_IP, n, (int)rtt);
	}

	(void)close(fd);

	if (recvd > 0) {
		T_PASS("ping %d/%d replies, rtt min/avg/max %d/%d/%d ms",
		       recvd, sent, (int)rtt_min, (int)(rtt_sum / recvd), (int)rtt_max);
	} else {
		/* Deliberately not a hard failure of the cellular stack: see the note
		 * on EHS_CELL_TEST_PEER_IP. The HTTP phase is the real reachability
		 * test and runs regardless. */
		T_FAIL("ping: no replies from " EHS_CELL_TEST_PEER_IP);
		T_INFO("This does NOT on its own mean the link is broken. Many hosts do");
		T_INFO("not answer ICMP, and some cellular APNs block it entirely. If");
		T_INFO("the HTTP phase below succeeds, the link is fine and this is the");
		T_INFO("peer or the APN, not the modem.");
	}
}

/*****************************************************************************/
/* Phase 11 - plain HTTP GET                                                 */

static ehs_char s_http_rx[1024];

static void phase_http(void)
{
	int fd = -1;
	struct sockaddr_in peer;
	struct timeval tv;
	ehs_char req[160];
	int n, req_len;
	ehs_uint32 total = 0;
	ehs_bool printed_head = EHS_FALSE;

	TEST_LOG("[11/11] HTTP GET http://%s:%d%s",
	         EHS_CELL_TEST_PEER_IP, (int)EHS_CELL_TEST_HTTP_PORT, EHS_CELL_TEST_HTTP_PATH);

	memset(&peer, 0, sizeof(peer));
	peer.sin_family = AF_INET;
	peer.sin_port   = htons((ehs_uint16)EHS_CELL_TEST_HTTP_PORT);
	if (inet_pton(AF_INET, EHS_CELL_TEST_PEER_IP, &peer.sin_addr) != 1) {
		T_FAIL("cannot parse peer address " EHS_CELL_TEST_PEER_IP);
		return;
	}

	/* Plain POSIX sockets. With CONFIG_NET_SOCKETS_OFFLOAD these go straight to
	 * the modem's own IP stack - which is exactly what the eRT mqtt_* /
	 * inx-netsocket / url_get blocks use, so this phase doubles as proof that
	 * those will work over cellular. */
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		T_FAIL("socket() failed, errno %d", errno);
		return;
	}

	tv.tv_sec  = EHS_CELL_TEST_HTTP_TMO_MS / 1000;
	tv.tv_usec = 0;
	(void)setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	(void)setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

	if (connect(fd, (struct sockaddr *)&peer, sizeof(peer)) < 0) {
		T_FAIL("connect() to %s:%d failed, errno %d",
		       EHS_CELL_TEST_PEER_IP, (int)EHS_CELL_TEST_HTTP_PORT, errno);
		T_INFO("errno 110/116 is a timeout - the host may not listen on port 80,");
		T_INFO("or the APN may not permit outbound TCP to it.");
		(void)close(fd);
		return;
	}
	T_PASS("TCP connected to %s:%d", EHS_CELL_TEST_PEER_IP, (int)EHS_CELL_TEST_HTTP_PORT);

	/* HTTP/1.1 requires Host. Connection: close so the server ends the body for
	 * us and we do not have to parse Content-Length or chunked encoding. */
	req_len = snprintf(req, sizeof(req),
	                   "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: eRT-cell-test\r\n"
	                   "Connection: close\r\n\r\n",
	                   EHS_CELL_TEST_HTTP_PATH, EHS_CELL_TEST_PEER_IP);
	if ((req_len <= 0) || (send(fd, req, (size_t)req_len, 0) != req_len)) {
		T_FAIL("send() of the request failed, errno %d", errno);
		(void)close(fd);
		return;
	}

	TEST_LOG("  --- response ---");
	for (;;) {
		n = (int)recv(fd, s_http_rx, sizeof(s_http_rx) - 1u, 0);
		if (n < 0) {
			if (total == 0u) {
				T_FAIL("recv() failed with no data, errno %d", errno);
			} else {
				T_INFO("  (recv ended, errno %d)", errno);
			}
			break;
		}
		if (n == 0) {
			break;   /* server closed - expected with Connection: close */
		}
		s_http_rx[n] = '\0';

		/* Print only the head of the response: a metered link and a 115200
		 * console are both poor places to dump a whole page. */
		if (!printed_head) {
			if (total < (ehs_uint32)EHS_CELL_TEST_HTTP_PRINT) {
				printf("%s", s_http_rx);
				fflush(stdout);
			} else {
				printed_head = EHS_TRUE;
			}
		}
		total += (ehs_uint32)n;
	}
	printf("\n");
	fflush(stdout);
	TEST_LOG("  --- end of response ---");

	(void)close(fd);

	if (total > 0u) {
		T_PASS("HTTP: received %u bytes", (unsigned)total);
		if (strncmp(s_http_rx, "HTTP/", 5) == 0 || total > 12u) {
			T_INFO("End-to-end IP connectivity over cellular is CONFIRMED.");
		}
	} else {
		T_FAIL("HTTP: connected but received nothing");
	}
}

/*****************************************************************************/
/* Entry point                                                               */

/**
 * TEST_FUNC entry point. Never returns — MCU test functions loop forever so the
 * board stays in a known state for inspection.
 */
void test_cellular(void)
{
	EhsCellConfigType cfg;
	EhsCellState st;

#ifdef EHS_TEST_FUNC_NO_ERT_INIT
	/* ERT_INIT=none skipped target_main.c's EhsTOsSys_init(). The shared
	 * cellular state machine runs on an eRT thread and takes eRT mutexes, so
	 * bring that one layer up even in "bare metal" mode. */
	EhsTOsSys_init();
#endif

	k_msleep(CELL_TEST_CONSOLE_SETTLE_MS);

	s_pass = s_fail = s_skip = 0;

	TEST_LOG("==================================================");
	TEST_LOG(" eRT cellular HAL test (Zephyr / nRF9151)");
	TEST_LOG(" RAT requested : %s", rat_name(EHS_CELL_TEST_RAT));
	TEST_LOG(" APN           : %s",
	         (sizeof(EHS_CELL_TEST_APN) > 1) ? EHS_CELL_TEST_APN : "(network-assigned)");
	TEST_LOG("==================================================");
	TEST_LOG("NOTE: an LTE-M attach is typically 5-60 s; NB-IoT is minutes and a");
	TEST_LOG("      cold search can legitimately take tens of minutes. Be patient -");
	TEST_LOG("      every state change is printed as it happens.");
	TEST_LOG("");

	phase_capabilities();
	if (s_caps == 0u) {
		goto summary;
	}

	/* ---- Connect: this also starts the modem and the SM thread ----------- */
	TEST_LOG("[2/11] modem bring-up + attach");
	memset(&cfg, 0, sizeof(cfg));
	cfg.rat           = EHS_CELL_TEST_RAT;
	cfg.preference    = EHS_CELL_TEST_RAT_PREF;
	cfg.apn           = EHS_CELL_TEST_APN;
	cfg.request_psm   = EHS_CELL_TEST_PSM ? EHS_TRUE : EHS_FALSE;
	cfg.psm_tau_s     = EHS_CELL_TEST_PSM_TAU_S;
	cfg.psm_active_s  = EHS_CELL_TEST_PSM_ACT_S;
	cfg.request_edrx  = EHS_CELL_TEST_EDRX ? EHS_TRUE : EHS_FALSE;

	if (!Cell_Connect(&cfg)) {
		T_FAIL("Cell_Connect() refused - the SM thread did not start.");
		T_INFO("Usually means no free EHS dynamic thread slot: raise");
		T_INFO("EHS_ZEPHYR_MAX_DYNAMIC_THREADS in the platform config.mk.");
		goto summary;
	}
	T_PASS("Cell_Connect() accepted");

	/* Wait for the modem library to come up and the SIM to be readable. */
	{
		ehs_uint32 waited = 0;

		while (!isCellularInitialised()) {
			st = Cell_GetState();
			if ((st == EhsCell_Failed) || (st == EhsCell_SimFailed)) {
				T_FAIL("modem bring-up failed: %s (%s)",
				       Cell_GetStateName(st),
				       Cell_GetFailReasonText(Cell_GetLastFailReason()));
				goto summary;
			}
			if (waited >= CELL_TEST_MODEM_TIMEOUT_MS) {
				T_FAIL("modem not up after %u ms", (unsigned)waited);
				T_INFO("Look above for nrf_modem_lib_init errors. A -1 usually means");
				T_INFO("the modem firmware is missing or incompatible - reflash the");
				T_INFO("modem firmware with nrfutil (see the setup notes).");
				goto summary;
			}
			k_msleep(CELL_TEST_POLL_MS);
			waited += CELL_TEST_POLL_MS;
		}
		T_PASS("modem library up after %u ms", (unsigned)waited);
	}

	phase_sim();
	phase_identity();
	phase_rat();

	/* ---- Attach ---------------------------------------------------------- */
	TEST_LOG("[6/11] attaching (budget %u s)",
	         (unsigned)(CELL_TEST_ATTACH_TIMEOUT_MS / 1000U));
	st = cell_wait_state(EhsCell_Online, CELL_TEST_ATTACH_TIMEOUT_MS);

	if (st != EhsCell_Online) {
		T_FAIL("not online - final state %s, reason %s",
		       Cell_GetStateName(st),
		       Cell_GetFailReasonText(Cell_GetLastFailReason()));
		T_INFO("registration status: %s", reg_name(Cell_GetRegStatus()));
		switch (st) {
		case EhsCell_SimPinRequired:
			T_INFO("The SIM is PIN-locked. Disable the PIN or supply it in cfg.sim_pin.");
			break;
		case EhsCell_SimFailed:
			T_INFO("UICC fault. Reseat the SIM; check it is a working, activated card.");
			break;
		case EhsCell_RegDenied:
			T_INFO("The network refused the subscription. Wrong APN, an inactive SIM,");
			T_INFO("or no roaming agreement. This is terminal by design - the SM will");
			T_INFO("not retry, because retrying denied registration burns battery and");
			T_INFO("can trip the modem's own reset-loop protection.");
			break;
		case EhsCell_NoCoverage:
		case EhsCell_Searching:
			T_INFO("Still searching. The LTE antenna is internal on this board, so");
			T_INFO("position matters: move away from metal, try near a window. Then");
			T_INFO("confirm the operator actually covers your chosen RAT here - NB-IoT");
			T_INFO("in particular is not deployed everywhere. Try RAT = both.");
			break;
		default:
			break;
		}
		goto summary;
	}

	T_PASS("ONLINE");
	T_INFO("  IP            : %s", Cell_GetIp());
	T_INFO("  registration  : %s", reg_name(Cell_GetRegStatus()));
	T_INFO("  roaming       : %s", Cell_IsRoaming() ? "YES (may have cost implications)" : "no");
	T_INFO("  active RAT    : %s", rat_name(Cell_GetActiveRat()));

	phase_signal_and_cell();
	phase_power_saving();
	phase_counters();

	/* Data-plane tests. These need an IP, so they come last - and they are what
	 * actually proves the link carries traffic rather than merely attaching. */
	phase_ping();
	phase_http();

summary:
	TEST_LOG("");
	TEST_LOG("==================================================");
	TEST_LOG(" SUMMARY:  %u passed, %u failed, %u skipped",
	         (unsigned)s_pass, (unsigned)s_fail, (unsigned)s_skip);
	TEST_LOG("   (skipped = destructive/irreversible or not supported -");
	TEST_LOG("    see the file header for why each is skipped)");
	TEST_LOG("==================================================");

	/* ---- Monitor --------------------------------------------------------- */
	if (Cell_GetState() == EhsCell_Online || Cell_GetState() == EhsCell_PsmSleep) {
		TEST_LOG("monitoring - reporting every state change.");
		TEST_LOG("PsmSleep is NOT a disconnect: the modem is still registered and");
		TEST_LOG("data resumes on wake. Seeing it here means PSM is working.");
	}
	{
		EhsCellState last = Cell_GetState();
		ehs_uint32 since = 0;

		for (;;) {
			EhsCellState now;

			k_msleep(CELL_TEST_POLL_MS);
			since += CELL_TEST_POLL_MS;

			now = Cell_GetState();
			if (now != last) {
				last = now;
				TEST_LOG("state -> %s", Cell_GetStateName(now));
				if ((now != EhsCell_Online) && (now != EhsCell_PsmSleep)) {
					TEST_LOG("  reason: %s",
					         Cell_GetFailReasonText(Cell_GetLastFailReason()));
				}
				since = CELL_TEST_STATUS_EVERY_MS;   /* force a status line */
			}

			if (since >= CELL_TEST_STATUS_EVERY_MS) {
				EhsCellSignalType sig;
				ehs_char a[16];

				since = 0;
				memset(&sig, 0, sizeof(sig));
				(void)Cell_GetSignal(&sig);
				TEST_LOG("status: %-13s ip=%-15s rsrp=%s %s",
				         Cell_GetStateName(now),
				         (Cell_GetIp()[0] != '\0') ? Cell_GetIp() : "-",
				         sig_str(a, sizeof(a), sig.rsrp_dbm, "dBm"),
				         isCellularSleeping() ? "[PSM]" : "");
			}
		}
	}
}

#else /* !EHS_TEST_FUNC_OVERRIDE */

/* ISO C forbids an empty translation unit. This file is in OBJECTS
 * unconditionally (alongside target_cellular.c) so that a TEST_FUNC build needs
 * no makefile change; keep it legal when the test is compiled out. */
typedef int ehs_cellular_test_translation_unit_not_empty;

#endif /* EHS_TEST_FUNC_OVERRIDE */
