/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#ifndef _COMPONENT_HAL_CELLULAR_H_
#define _COMPONENT_HAL_CELLULAR_H_

/* ============================================================================
 *              Cellular (LTE-M / NB-IoT) Subsystem — the contract
 * ============================================================================
 *
 * This header is the canonical, cross-target definition of the cellular WAN
 * subsystem: connection state machine, capability model, and the primitives a
 * target HAL must implement.  It is deliberately free of any vendor type — no
 * `enum lte_lc_*`, no `struct cellular_*`, no AT strings.  A backend that needs
 * one of those converts at its own boundary, exactly as the Wi-Fi contract in
 * wifi/wifi_station.h keeps `esp_err_t` out of the shared layer.
 *
 * Backends expected to satisfy this contract:
 *   nRF91 (NCS lte_lc + modem_info)  target/os-arch/zephyr-arm/target_cellular.c
 *   Zephyr generic modem_cellular    (drivers/cellular.h ops)   — not yet written
 *   UART AT modem (Quectel/u-blox/…) (3GPP TS 27.007)           — not yet written
 *   stubbed                          cellular/cellular_stubbed.c
 *
 *
 * WHY THIS IS NOT wifi_station.h WITH DIFFERENT NAMES
 * ---------------------------------------------------
 * The two subsystems look similar and are not.  Four differences drive most of
 * the design below, and getting any of them wrong produces a subsystem that
 * "works" on the bench and fails in the field:
 *
 *  1. TIMESCALE.  Wi-Fi association completes in seconds; an NB-IoT cold attach
 *     on an unknown PLMN can legitimately take tens of minutes.  The Wi-Fi
 *     timeouts (WIFI_SM_TIMEOUT_ASSOC_MS = 30 s) would declare failure while
 *     the modem is doing exactly what it should.  See CELL_SM_TIMEOUT_* below.
 *
 *  2. TERMINAL FAILURES ARE REAL.  Wi-Fi retries a bad password forever (its
 *     header admits this as a known gap).  Cellular has three failure modes
 *     that MUST NOT auto-retry:
 *        - SIM_PIN_REQUIRED : a SIM PIN has three attempts and then needs a
 *                             PUK.  A retry loop bricks a customer's SIM in
 *                             three boots.  This is the single most destructive
 *                             thing this subsystem can do.
 *        - SIM_FAILED       : UICC fault; retrying cannot help.
 *        - REG_DENIED       : the network refused the subscription.  Hammering
 *                             it burns battery and can trip the modem's own
 *                             reset-loop protection (which then refuses to
 *                             attach for ~30 minutes).
 *
 *  3. "ASLEEP" IS NOT "DISCONNECTED".  A modem in PSM has no radio activity for
 *     minutes to hours but is still registered, and data resumes on wake.  A
 *     naive is-connected check that reports offline here makes every
 *     power-optimised deployment look broken.  EhsCell_Online covers PSM;
 *     isCellularSleeping() distinguishes it when a caller genuinely cares.
 *
 *  4. NO SCAN-THEN-ASSOCIATE.  Registration is modem-driven.  There is a manual
 *     operator scan in 3GPP (+COPS=?) but it costs minutes and is almost never
 *     what an application wants, so it is capability-gated and not part of the
 *     connect path.
 *
 *
 * STATE MACHINE  (EhsCellState)
 * -----------------------------
 *
 *   IDLE
 *     | Cell_Connect()
 *     v
 *   MODEM_INIT ------------------------------> FAILED
 *     | modem library up, firmware ready        (init error)
 *     v   [CELL_SM_TIMEOUT_MODEM_INIT_MS]
 *   SIM_WAIT --+--> SIM_PIN_REQUIRED   *** TERMINAL — never auto-retry ***
 *     |        +--> SIM_FAILED         *** TERMINAL ***
 *     | UICC ready
 *     v   [CELL_SM_TIMEOUT_SIM_MS]
 *   RADIO_ON
 *     | CFUN=1 accepted
 *     v
 *   SEARCHING -+--> REG_DENIED         *** TERMINAL-ish — long backoff only ***
 *     |         +--> NO_COVERAGE --(backoff)--> SEARCHING
 *     | registered home OR roaming
 *     v   [CELL_SM_TIMEOUT_ATTACH_LTEM_MS / _NBIOT_MS, per active RAT]
 *   REGISTERED
 *     | PDN active, IP assigned
 *     v   [CELL_SM_TIMEOUT_PDN_MS]
 *   ONLINE <-----------------> PSM_SLEEP   (still registered; not a disconnect)
 *     | link lost / detach
 *     v
 *   RECONNECTING --> SEARCHING   (EXPONENTIAL backoff, not Wi-Fi's fast retry)
 *
 * KEY RULE, inherited from the Wi-Fi contract and just as binding here: every
 * transitory state has exactly one active timer.  States that persist
 * indefinitely (IDLE, ONLINE, PSM_SLEEP, and the terminal states) arm none.
 *
 *
 * PORTING CONTRACT (new cellular target)
 * --------------------------------------
 *  1. Implement the doCellular* primitives declared at the bottom of this file.
 *     Every one of them must be NON-BLOCKING: an attach can take minutes and
 *     the shared SM thread cannot be parked for that long.  Enqueue work to a
 *     backend-owned worker thread and return.
 *  2. Implement the isCellular* accessors.
 *  3. Translate native events into cbSource transitions via
 *     EhsCellSetCBSource().  Do NOT implement a parallel state machine — the
 *     shared SM in Common/Components/networking/inx-cellular_link.c owns all
 *     the logic.
 *  4. Report capabilities honestly from doCellularGetCapabilities().  A
 *     capability bit that is set but unimplemented is worse than one that is
 *     clear: callers branch on these, and a silent no-op reads as "the network
 *     said no".
 *  5. Never call EhsConsolePrintf (kernel-only) or printf.  Diagnostics go to
 *     EHSH_LOG_* with EHSL_MODULE_ID = EHSH_LOG_MODULE_HAL_NETWORK.
 *
 * See docs/ert-cellular-plan.md for the full design rationale, the vendor API
 * survey behind the capability split, and the measured memory budget.
 * ============================================================================
 */

#include "globals.h"

#ifdef EHS_NETWORK_CELLULAR_SUPPORT

/* ── Buffer sizes ─────────────────────────────────────────────────────────
 * Sized from the 3GPP maxima, not from any one vendor's SDK:
 *   IMEI  15 digits, ICCID 19-20 digits, IMSI 15 digits, operator name up to
 *   64 chars (long alphanumeric PLMN name), APN up to 63 octets, IPv6 string
 *   up to 45 chars + NUL.  All include the terminator. */
#define EHS_CELL_IMEI_BUFF_MAX      16u
#define EHS_CELL_ICCID_BUFF_MAX     24u
#define EHS_CELL_EID_BUFF_MAX       33u   /* EID is 32 decimal digits + NUL */
#define EHS_CELL_IMSI_BUFF_MAX      16u
#define EHS_CELL_OPERATOR_BUFF_MAX  65u
#define EHS_CELL_APN_BUFF_MAX       64u
#define EHS_CELL_IPADDR_BUFF_MAX    46u
#define EHS_CELL_PIN_BUFF_MAX       9u    /* 4-8 digits + NUL              */
#define EHS_CELL_FWVER_BUFF_MAX     41u   /* matches the nRF91 modem fw string */
#define EHS_CELL_USER_BUFF_MAX      33u
#define EHS_CELL_PASS_BUFF_MAX      33u

/* Sentinel for "this reading is not available".  Deliberately not 0 — an
 * RSRP of 0 dBm is not a plausible reading but it IS a plausible-looking one,
 * and a diagnostic block that prints 0 for "unknown" is worse than useless.
 * Backends must write this rather than leaving a field untouched. */
#define EHS_CELL_VALUE_INVALID      ((ehs_sint16)0x7FFF)
#define EHS_CELL_CELLID_INVALID     ((ehs_uint32)0xFFFFFFFFu)


/* ── Connection state-machine timeouts (milliseconds) ─────────────────────
 * Part of the porting contract: a backend that needs its own driver-level
 * deadline must DERIVE it from these so the two cannot silently drift.
 *
 * These are ~10-100x the Wi-Fi equivalents and that is not a mistake.  Figures
 * are from the LTE-M/NB-IoT attach timings in docs/ert-cellular-plan.md § 2.6;
 * for reference the (now archived) ubxlib used a 30-minute default for NB-IoT
 * network search. */
#define CELL_SM_TIMEOUT_MODEM_INIT_MS   10000UL   /* lib init -> fw ready      */
#define CELL_SM_TIMEOUT_SIM_MS          10000UL   /* UICC power -> state known */
#define CELL_SM_TIMEOUT_ATTACH_LTEM_MS  180000UL  /* SEARCHING -> REGISTERED   */
#define CELL_SM_TIMEOUT_ATTACH_NBIOT_MS 600000UL  /* ditto, NB-IoT is slower   */
#define CELL_SM_TIMEOUT_PDN_MS          60000UL   /* REGISTERED -> got IP      */

/* Reconnect backoff.  Exponential from INITIAL, doubling, capped at MAX.
 * Wi-Fi's "N fast retries then a slow period" is wrong here: a failed attach
 * means poor coverage or a network problem, and retrying hard costs battery
 * and risks the modem's reset-loop protection. */
#define CELL_SM_BACKOFF_INITIAL_MS      60000UL
#define CELL_SM_BACKOFF_MAX_MS          3600000UL

/* Compile-time sanity: the attach timers must exceed the modem-init and SIM
 * timers they follow, or the SM can time out a phase it never entered. */
#if (CELL_SM_TIMEOUT_ATTACH_LTEM_MS <= CELL_SM_TIMEOUT_MODEM_INIT_MS) || \
    (CELL_SM_TIMEOUT_ATTACH_NBIOT_MS <= CELL_SM_TIMEOUT_ATTACH_LTEM_MS)
#error "CELL_SM_TIMEOUT_* are inconsistent: attach must outlast init, and NB-IoT must outlast LTE-M."
#endif


/* ── Capabilities ─────────────────────────────────────────────────────────
 * Query with Cell_GetCapabilities() BEFORE using anything gated.  The vendor
 * survey in docs/ert-cellular-plan.md § 4.3 is why this is not optional: the
 * common denominator across nRF91 / Zephyr modem_cellular / raw AT is much
 * smaller than any one of them, and a function block that silently no-ops on a
 * target reads to the user as "the network refused". */
typedef enum {
	EHS_CELL_CAP_LTEM            = (1u << 0),  /* LTE-M / Cat-M1 radio        */
	EHS_CELL_CAP_NBIOT           = (1u << 1),  /* NB-IoT / Cat-NB1/NB2 radio  */
	EHS_CELL_CAP_PSM             = (1u << 2),  /* Power Saving Mode           */
	EHS_CELL_CAP_EDRX            = (1u << 3),  /* extended DRX                */
	EHS_CELL_CAP_RAI             = (1u << 4),  /* Release Assistance          */
	EHS_CELL_CAP_CONN_EVAL       = (1u << 5),  /* pre-TX link evaluation      */
	EHS_CELL_CAP_ENERGY_ESTIMATE = (1u << 6),  /* energy estimate in the eval */
	EHS_CELL_CAP_CE_LEVEL        = (1u << 7),  /* coverage enhancement level  */
	EHS_CELL_CAP_NEIGHBOUR_MEAS  = (1u << 8),  /* neighbour-cell measurement  */
	EHS_CELL_CAP_OPERATOR_SCAN   = (1u << 9),  /* manual PLMN scan (+COPS=?)  */
	EHS_CELL_CAP_BAND_MASK       = (1u << 10), /* band lock / mask            */
	EHS_CELL_CAP_DATA_COUNTERS   = (1u << 11), /* TX/RX byte counters         */
	EHS_CELL_CAP_SIM_PIN         = (1u << 12), /* PIN entry / enable / change */
	/* At least ONE SIM type is runtime-selectable in software. NOT "every pair
	 * is selectable" — query EhsSim_GetSelectableSlots() for which. On nRF91
	 * the UICC interface and SoftSIM are switchable via %CSUS even though
	 * physical-vs-embedded is decided by a hardware autoswitch. */
	EHS_CELL_CAP_SIM_SELECT      = (1u << 13),
	EHS_CELL_CAP_SOFTSIM         = (1u << 14), /* SoftSIM (no hardware UICC)  */
	EHS_CELL_CAP_EUICC_PROFILES  = (1u << 15), /* LOCAL eUICC profile mgmt    */
	EHS_CELL_CAP_GNSS_SHARED     = (1u << 16), /* GNSS shares this modem      */
	EHS_CELL_CAP_TLS_OFFLOAD     = (1u << 17), /* modem-side credential store */
	EHS_CELL_CAP_EUICC_DOWNLOAD  = (1u << 18), /* RSP download stack present  */
	EHS_CELL_CAP_EUICC_FACTORY_INSTALL = (1u << 19) /* local BPP install path */
} EhsCellCapability;


/* ── Radio access technology ──────────────────────────────────────────────
 * A BITMASK, because "enable both and let the modem choose" is a distinct and
 * common configuration.  Note that the set of ENABLED RATs and the CURRENTLY
 * ACTIVE one are different questions — Cell_GetRat() answers the first,
 * Cell_GetActiveRat() the second, and conflating them is a classic bug. */
typedef enum {
	EHS_CELL_RAT_NONE  = 0u,
	EHS_CELL_RAT_LTEM  = (1u << 0),
	EHS_CELL_RAT_NBIOT = (1u << 1)
} EhsCellRat;

/* Which RAT the modem should prefer when both are enabled.  PLMN_PRIO variants
 * ask the modem to honour the SIM's PLMN priority list ahead of the RAT
 * preference. */
typedef enum {
	EHS_CELL_RAT_PREF_AUTO = 0,
	EHS_CELL_RAT_PREF_LTEM,
	EHS_CELL_RAT_PREF_NBIOT,
	EHS_CELL_RAT_PREF_LTEM_PLMN_PRIO,
	EHS_CELL_RAT_PREF_NBIOT_PLMN_PRIO,
	EHS_CELL_RAT_PREF_MAX
} EhsCellRatPref;


/* ── Radio power state ────────────────────────────────────────────────────*/
typedef enum {
	EHS_CELL_POWER_OFF = 0,   /* modem fully powered down (CFUN=0)          */
	EHS_CELL_POWER_ON,        /* normal operation        (CFUN=1)           */
	EHS_CELL_POWER_AIRPLANE,  /* radio off, UICC off     (CFUN=4)           */
	EHS_CELL_POWER_UICC_ONLY, /* radio off, UICC powered (CFUN=44)          */
	EHS_CELL_POWER_MAX
} EhsCellPowerState;


/* ── Connection state machine ─────────────────────────────────────────────
 * See the diagram at the top of this file.  Backends may keep private internal
 * flags but must surface behaviour through these states and must not invent
 * new ones without extending this enum. */
typedef enum {
	EhsCell_Idle = 0,          /* stopped / not configured                   */
	EhsCell_ModemInit,         /* modem library / firmware coming up         */
	EhsCell_SimWait,           /* waiting for UICC to report ready           */
	EhsCell_SimPinRequired,    /* TERMINAL — needs Cell/EhsSim_PinEnter()    */
	EhsCell_SimFailed,         /* TERMINAL — UICC fault or absent            */
	EhsCell_RadioOn,           /* CFUN=1 accepted, not yet searching         */
	EhsCell_Searching,         /* network search / registration in flight    */
	EhsCell_NoCoverage,        /* searched, found nothing — backoff & retry  */
	EhsCell_RegDenied,         /* TERMINAL-ish — subscription refused        */
	EhsCell_Registered,        /* attached; PDN/IP not yet up                */
	EhsCell_Online,            /* fully up, has an IP                        */
	EhsCell_PsmSleep,          /* registered but in PSM — NOT disconnected   */
	EhsCell_Reconnecting,      /* link lost; exponential backoff             */
	EhsCell_Disconnecting,     /* user-requested teardown in progress        */
	EhsCell_Failed,            /* terminal — manual recovery required        */
	EhsCell_MAX
} EhsCellState;

/* 3GPP TS 27.007 +CEREG numbering, used verbatim.  All three surveyed stacks
 * (NCS lte_lc, Zephyr cellular.h, raw AT) are this enum wearing different
 * names, so standardising on the spec's own numbering costs nothing and makes
 * every backend's conversion a straight cast-with-validation. */
typedef enum {
	EhsCellReg_NotRegistered      = 0,
	EhsCellReg_RegisteredHome     = 1,
	EhsCellReg_Searching          = 2,
	EhsCellReg_Denied             = 3,
	EhsCellReg_Unknown            = 4,
	EhsCellReg_RegisteredRoaming  = 5,
	EhsCellReg_UiccFail           = 90
} EhsCellRegStatus;

/* Why the last attempt failed.  This is the gap the Wi-Fi contract documents
 * as unfixed ("AUTH_FAIL is not distinguished from a transient drop"); here it
 * is a first-class output because the SM's retry policy depends on it. */
typedef enum {
	EhsCellFail_None = 0,
	EhsCellFail_ModemInit,        /* modem library or firmware failed to start */
	EhsCellFail_SimAbsent,
	EhsCellFail_SimPinRequired,
	EhsCellFail_SimPukRequired,
	EhsCellFail_SimFailure,       /* UICC fault                                */
	EhsCellFail_RegistrationDenied,
	EhsCellFail_NoCoverage,
	EhsCellFail_AttachTimeout,
	EhsCellFail_PdnTimeout,
	EhsCellFail_PdnRejected,      /* APN wrong / not permitted                 */
	EhsCellFail_ModemResetLoop,   /* modem's own reset-loop protection tripped */
	EhsCellFail_Overheated,
	EhsCellFail_NotSupported,
	EhsCellFail_InvalidArg,
	EhsCellFail_Internal,
	EhsCellFail_MAX
} EhsCellFailReason;


/* ── Configuration passed to Cell_Connect() ───────────────────────────────
 * Any pointer may be NULL / any string empty, meaning "leave at the modem or
 * platform default".  In particular an empty APN is normal and correct on most
 * modern networks, which supply it during attach. */
typedef struct EhsCellConfig {
	EhsCellRat      rat;            /* bitmask; EHS_CELL_RAT_NONE = leave as-is */
	EhsCellRatPref  preference;
	const ehs_char *apn;            /* NULL/"" = network-assigned               */
	const ehs_char *apn_user;       /* PAP/CHAP username, usually NULL          */
	const ehs_char *apn_pass;
	const ehs_char *sim_pin;        /* NULL unless the SIM is PIN-locked.       */
	ehs_bool        request_psm;
	ehs_sint32      psm_tau_s;      /* requested periodic-TAU, seconds          */
	ehs_sint32      psm_active_s;   /* requested active time, seconds           */
	ehs_bool        request_edrx;
	ehs_sint32      edrx_cycle_ms;  /* requested eDRX cycle                     */
} EhsCellConfigType;


/* ── Signal quality ───────────────────────────────────────────────────────
 * Fields not supported by a backend are set to EHS_CELL_VALUE_INVALID. */
typedef struct EhsCellSignal {
	ehs_sint16 rsrp_dbm;   /* reference signal received power  */
	ehs_sint16 rsrq_db;    /* reference signal received quality */
	ehs_sint16 snr_db;
	ehs_sint16 rssi_dbm;
} EhsCellSignalType;

/* Pre-transmit link evaluation — the standout cellular-specific capability and
 * the reason EHS_CELL_CAP_CONN_EVAL exists.  It answers "is sending right now
 * worth the battery?" BEFORE any data is queued, which is a first-class NB-IoT
 * design pattern with no Wi-Fi equivalent.  Gated: only backends advertising
 * EHS_CELL_CAP_CONN_EVAL populate it. */
typedef enum {
	EhsCellEnergy_Unknown = 0,
	EhsCellEnergy_Excessive,
	EhsCellEnergy_Increased,
	EhsCellEnergy_Normal,
	EhsCellEnergy_Reduced,
	EhsCellEnergy_Efficient
} EhsCellEnergyEstimate;

typedef struct EhsCellLinkEval {
	EhsCellSignalType      signal;
	EhsCellEnergyEstimate  energy;
	ehs_sint16 ce_level;       /* coverage enhancement 0-3, INVALID if unknown */
	ehs_sint16 tx_power_dbm;
	ehs_sint16 tx_repetitions;
	ehs_sint16 rx_repetitions;
	ehs_sint16 dl_pathloss_db;
	ehs_sint16 band;
} EhsCellLinkEvalType;

/* Serving-cell identity. */
typedef struct EhsCellInfo {
	ehs_sint32 mcc;
	ehs_sint32 mnc;
	ehs_uint32 cell_id;         /* EHS_CELL_CELLID_INVALID if unknown */
	ehs_uint32 tac;
	ehs_uint32 earfcn;
	ehs_sint16 phys_cell_id;
	ehs_sint16 band;
} EhsCellInfoType;

/* Granted (not requested) power-saving parameters.  The distinction matters:
 * you ask, the network grants, and the grant is frequently different from the
 * ask.  An API that only exposes the request is useless in the field. */
typedef struct EhsCellPsm {
	ehs_bool   granted;
	ehs_sint32 tau_s;
	ehs_sint32 active_s;
} EhsCellPsmType;

typedef struct EhsCellEdrx {
	ehs_bool   granted;
	EhsCellRat rat;
	ehs_sint32 cycle_ms;
	ehs_sint32 ptw_ms;      /* paging time window */
} EhsCellEdrxType;


/* ── SIM ──────────────────────────────────────────────────────────────────
 * Four genuinely different things get called "SIM"; conflating them causes
 * real design errors, so they are distinct here:
 *
 *   PHYSICAL : removable card in a socket (4FF/nano etc.)
 *   EMBEDDED : soldered eUICC (MFF2); profiles arrive by remote provisioning
 *   SOFT     : SoftSIM — a pure software UICC application, NO hardware at all
 *   ISIM     : UICC integrated into the SoC die
 *
 * The sim_manager function block presents the first three as "SIM", "SoftSIM"
 * and "eSIM" respectively, and supports switching between them at RUNTIME.
 *
 * IMPORTANT — "exists" and "is runtime-selectable" are DIFFERENT QUESTIONS, and
 * conflating them under-reports what the hardware can do:
 *
 *   EhsSim_GetAvailableSlots()   which types exist on this board
 *   EhsSim_GetSelectableSlots()  which of those software can switch to
 *
 * On the Thingy:91 X the nano-SIM socket and the eSIM footprint share one UICC
 * interface, routed by an analog switch driven by card-detect: inserting a card
 * wins, removing it falls back to the eSIM. Software cannot choose between
 * those two. But it CAN switch between that UICC interface and SoftSIM
 * (AT%CSUS), so selection is genuinely supported — just not across every pair.
 *
 * EhsSim_GetActiveSlot() is always answerable regardless of either. */
typedef enum {
	EHS_SIM_SLOT_PHYSICAL = 0,
	EHS_SIM_SLOT_EMBEDDED,
	EHS_SIM_SLOT_SOFT,
	EHS_SIM_SLOT_ISIM,
	EHS_SIM_SLOT_MAX
} EhsSimSlot;

typedef enum {
	EHS_SIM_STATE_ABSENT = 0,
	EHS_SIM_STATE_PIN_REQUIRED,
	EHS_SIM_STATE_PUK_REQUIRED,   /* TERMINAL — needs operator intervention */
	EHS_SIM_STATE_READY,
	EHS_SIM_STATE_FAILED,
	EHS_SIM_STATE_UNKNOWN,
	EHS_SIM_STATE_MAX
} EhsSimState;

/* eUICC profile descriptor (SGP.22 / SGP.32).  Declared so the API shape is
 * settled; no backend implements it yet and every one should clear
 * EHS_CELL_CAP_EUICC_PROFILES until one does. */
typedef struct EhsSimProfile {
	ehs_char iccid[EHS_CELL_ICCID_BUFF_MAX];
	ehs_char name[33];
	ehs_bool enabled;
} EhsSimProfileType;


/* ── Callback sources ─────────────────────────────────────────────────────
 * "What should the shared SM thread do next."  Set by backend event handlers,
 * consumed by cellular_link_thread() in inx-cellular_link.c.  This is the
 * mechanism that drives state transitions, not a parallel state machine. */
enum eCellCallbackSource {
	eCellCallbackSource_None = 0,
	eCellCallbackSource_Connect,
	eCellCallbackSource_ModemReady,
	eCellCallbackSource_SimReady,
	eCellCallbackSource_SimPin,
	eCellCallbackSource_SimFail,
	eCellCallbackSource_SimSwitch,   /* runtime SIM-type change requested */
	eCellCallbackSource_RegStatus,
	eCellCallbackSource_GotIp,
	eCellCallbackSource_Psm,
	eCellCallbackSource_LinkLost,
	eCellCallbackSource_Disconnect,
	eCellCallbackSource_Internal,
	eCellCallbackSource_MAX
};


/* ============================================================================
 *                        UNIFIED API — preferred surface
 * ============================================================================
 * Callers (function blocks, console, applications) use these.  All are
 * non-blocking; connection progress is observed by polling Cell_GetState().
 */

/* Bitmask of EhsCellCapability. */
ehs_uint32        Cell_GetCapabilities(void);

/* Request connect.  Returns immediately; poll Cell_GetState() for progress.
 * cfg may be NULL to reuse whatever was last configured. */
ehs_bool          Cell_Connect(const EhsCellConfigType *cfg);
ehs_bool          Cell_Disconnect(void);

ehs_bool          Cell_SetPowerState(EhsCellPowerState state);
EhsCellPowerState Cell_GetPowerState(void);

EhsCellState      Cell_GetState(void);
const ehs_char   *Cell_GetStateName(EhsCellState state);
EhsCellRegStatus  Cell_GetRegStatus(void);
ehs_bool          Cell_IsRoaming(void);

/* "" when not online. */
const ehs_char   *Cell_GetIp(void);

EhsCellFailReason Cell_GetLastFailReason(void);
const ehs_char   *Cell_GetFailReasonText(EhsCellFailReason reason);

/* RAT selection.  Disruptive: applying a new RAT drops any active connection.
 * Cell_GetRat() reports the ENABLED set; Cell_GetActiveRat() reports what the
 * modem is currently camped on — these are different questions. */
ehs_bool          Cell_SetRat(EhsCellRat rat, EhsCellRatPref preference);
ehs_bool          Cell_GetRat(EhsCellRat *rat, EhsCellRatPref *preference);
EhsCellRat        Cell_GetActiveRat(void);
ehs_bool          Cell_SetBandMask(ehs_uint64 mask);   /* CAP_BAND_MASK */

ehs_bool          Cell_GetSignal(EhsCellSignalType *out);
ehs_bool          Cell_EvalLink(EhsCellLinkEvalType *out);   /* CAP_CONN_EVAL */
ehs_bool          Cell_GetCellInfo(EhsCellInfoType *out);

ehs_bool          Cell_GetImei(ehs_char *buf, ehs_uint16 buf_size);
ehs_bool          Cell_GetOperator(ehs_char *buf, ehs_uint16 buf_size);
ehs_bool          Cell_GetModemFwVersion(ehs_char *buf, ehs_uint16 buf_size);

ehs_bool          Cell_GetDataCounters(ehs_uint32 *tx_bytes, ehs_uint32 *rx_bytes);
ehs_bool          Cell_ResetDataCounters(void);

/* Power saving.  Request vs granted — see EhsCellPsmType. */
ehs_bool          Cell_PsmRequest(ehs_bool enable, ehs_sint32 tau_s, ehs_sint32 active_s);
ehs_bool          Cell_PsmGetGranted(EhsCellPsmType *out);
ehs_bool          Cell_EdrxRequest(ehs_bool enable, EhsCellRat rat, ehs_sint32 cycle_ms);
ehs_bool          Cell_EdrxGetGranted(EhsCellEdrxType *out);
ehs_bool          Cell_RaiSet(ehs_bool as_rai, ehs_bool cp_rai);


/* ============================================================================
 *                              SIM sub-API
 * ============================================================================ */

EhsSimState  EhsSim_GetState(void);
EhsSimSlot   EhsSim_GetActiveSlot(void);        /* always answerable */

/* Which types EXIST on this hardware. Bitmask of (1u << EhsSimSlot). */
ehs_uint32   EhsSim_GetAvailableSlots(void);

/* Which of those software can SWITCH TO at runtime — a subset of the above.
 * A type can be available but not selectable (an eUICC behind a hardware
 * autoswitch, say). Callers offering a SIM-type choice must use this, not
 * GetAvailableSlots, or they will offer a switch that cannot happen. */
ehs_uint32   EhsSim_GetSelectableSlots(void);

/* Switch SIM type at runtime.
 *
 * DISRUPTIVE and asynchronous. %CSUS (or the equivalent) only takes effect
 * across a modem re-initialisation, so this drops any active connection. The
 * shared state machine is told, tears the link down cleanly and re-attaches on
 * the new SIM — a caller does not have to orchestrate that, and must not try.
 * Poll Cell_GetState() as for any other connect.
 *
 * Returns EHS_FALSE with EhsCellFail_NotSupported if the type is not in
 * EhsSim_GetSelectableSlots(). Selecting the already-active type succeeds and
 * does nothing. */
ehs_bool     EhsSim_Select(EhsSimSlot slot);    /* CAP_SIM_SELECT */

ehs_bool     EhsSim_GetIccid(ehs_char *buf, ehs_uint16 buf_size);
ehs_bool     EhsSim_GetImsi(ehs_char *buf, ehs_uint16 buf_size);

/* *** READ THIS BEFORE CALLING EhsSim_PinEnter() ***
 * A SIM PIN has three attempts and then requires a PUK.  Neither the SM nor any
 * function block may retry a REJECTED PIN automatically — doing so permanently
 * locks a customer's SIM in three boots.  Callers must check
 * EhsSim_PinAttemptsRemaining() first and must stop after one rejection. */
ehs_bool     EhsSim_PinEnter(const ehs_char *pin);            /* CAP_SIM_PIN */
ehs_sint32   EhsSim_PinAttemptsRemaining(void);               /* -1 = unknown */
ehs_bool     EhsSim_PinEnable(ehs_bool enable, const ehs_char *pin);
ehs_bool     EhsSim_PinChange(const ehs_char *old_pin, const ehs_char *new_pin);

/* SoftSIM.  Provider-agnostic: the blob is whatever the chosen provider's
 * tooling emits, and the backend hands it to the provider module unmodified. */
ehs_bool     EhsSim_SoftIsProvisioned(void);                  /* CAP_SOFTSIM */
ehs_bool     EhsSim_SoftProvision(const ehs_uint8 *blob, ehs_uint32 len);

/* ── eUICC profile management ─────────────────────────────────────────────
 * API shape only; see EhsSimProfileType. No backend implements these yet and
 * none advertises either capability. The plan's § 6.1.2 covers the IPAd-vs-IPAe
 * question that decides how large the device-side work is.
 *
 * TWO CAPABILITIES, DELIBERATELY, because they are two different interfaces
 * and a real device can have one without the other:
 *
 *   EHS_CELL_CAP_EUICC_PROFILES  LOCAL lifecycle. GSMA ES10c — plain APDU
 *                                exchanges with the chip. Needs NO network of
 *                                any kind, and is common to SGP.22 and SGP.32
 *                                rather than being specific to either.
 *   EHS_CELL_CAP_EUICC_DOWNLOAD  REMOTE provisioning. ES9+ to an SM-DP+, or
 *                                ESipa to an eIM under SGP.32. Needs IP
 *                                reachability to that server — which does not
 *                                have to be the cellular link.
 *
 * A device whose profiles were loaded in the factory (in-factory provisioning,
 * or a pre-personalised part) has the first and not the second. An eUICC that
 * runs the IPA on-chip (IPAe) may also present the first only. One combined bit
 * could not express either case.
 *
 * Backs two function blocks on the same split: esim_profile_mgr (local) and
 * sgp32_download (remote). */

/* --- LOCAL: ES10c lifecycle. CAP_EUICC_PROFILES. No network. --- */
ehs_sint32   EhsSim_ProfileCount(void);       /* -1 = cannot be determined  */
ehs_bool     EhsSim_ProfileGet(ehs_sint32 index, EhsSimProfileType *out);
ehs_bool     EhsSim_ProfileEnable(const ehs_char *iccid);
ehs_bool     EhsSim_ProfileDisable(const ehs_char *iccid);
ehs_bool     EhsSim_ProfileDelete(const ehs_char *iccid);

/* --- REMOTE: RSP. CAP_EUICC_DOWNLOAD. Needs a reachable server. --- */

/* Address of the eIM (eSIM IoT Remote Manager) to provision against. The one
 * piece of configuration SGP.32 always needs, whichever of IPAd/IPAe applies. */
ehs_bool     EhsSim_ProfileSetEim(const ehs_char *eim_address);
/* iccid_out receives the ICCID of the profile that was installed, so the caller
 * can then enable it via EhsSim_ProfileEnable() without having to list profiles
 * and diff against a snapshot. A downloaded profile is installed DISABLED, so
 * that follow-up step is always required. Set to "" on failure. */
ehs_bool     EhsSim_ProfileDownload(const ehs_char *activation_code,
                                    ehs_char *iccid_out, ehs_uint16 iccid_buf_size);

/* --- FACTORY: local install of a pre-built package. CAP_EUICC_FACTORY_INSTALL.
 *
 * For provisioning on a production line with no device-side network. The
 * factory's equipment obtains a Bound Profile Package already bound to this
 * eUICC's EID (hence GetEid below, which has no SGP.32 equivalent — in a
 * download the eUICC presents its own EID inside the authenticated session and
 * the application never sees it) and streams it in through this device.
 *
 * STREAMED, NOT BUFFERED, and the shape is the protocol's, not a convenience:
 * ES10b delivers a BPP to the eUICC as an ordered sequence of segments, so no
 * layer ever needs the whole package. Consequences that callers must respect:
 *
 *   * There is NO offset. The eUICC sequences by the STORE DATA block number
 *     and reassembles by concatenation; segments must arrive in order and
 *     exactly once. An offset parameter would advertise random access the chip
 *     does not have.
 *   * A rejected segment is NOT retryable. SCP03 MAC chaining means the chain
 *     is already broken, so the only recovery is Abort and start again.
 *   * Do NOT add an application checksum over the data. Every segment carries
 *     an SCP03t C-MAC verified inside the eUICC, which no caller — not this
 *     device, not the factory equipment — holds the keys to forge. Corruption
 *     anywhere in the path fails the MAC. A CRC here would catch nothing and
 *     imply a guarantee it does not provide.
 *
 * The package is never written to storage; see the note on the function block.
 */
ehs_bool     EhsSim_EuiccGetEid(ehs_char *buf, ehs_uint16 buf_size);
/* Largest segment, in BYTES, that the eUICC will accept in one push. -1 when it
 * cannot be determined. NOTE this is only one of the two limits that matter -
 * the caller must also fit the segment through whatever transport carries it,
 * which on a function-block path is typically the smaller of the two. */
ehs_sint32   EhsSim_FactoryInstallMaxSegment(void);
ehs_bool     EhsSim_FactoryInstallBegin(void);
ehs_bool     EhsSim_FactoryInstallPushSegment(const ehs_uint8 *seg, ehs_uint32 len);
ehs_bool     EhsSim_FactoryInstallCommit(ehs_char *iccid_out, ehs_uint16 iccid_buf_size);
/* Unconditional: discards session state and wipes buffers. Safe to call when no
 * install is in progress, which is what makes it usable as a timeout action. */
void         EhsSim_FactoryInstallAbort(void);


/* ============================================================================
 *                      TARGET HAL CONTRACT — implement these
 * ============================================================================
 * EVERY doCellular* MUST RETURN PROMPTLY.  Attach takes minutes; the shared SM
 * thread cannot be parked.  Enqueue and return.
 */

ehs_uint32   doCellularGetCapabilities(void);

ehs_bool     doCellularInit(void);
void         doCellularDestroy(void);

/* Start the modem stack (library init, firmware bring-up). */
ehs_bool     doCellularStart(void);

/* Apply cfg and begin the attach.  Non-blocking. */
ehs_bool     doCellularConnect(const EhsCellConfigType *cfg);
ehs_bool     doCellularDisconnect(void);

ehs_bool     doCellularSetPowerState(EhsCellPowerState state);
ehs_bool     doCellularGetPowerState(EhsCellPowerState *out);

ehs_bool     doCellularSetRat(EhsCellRat rat, EhsCellRatPref preference);
ehs_bool     doCellularGetRat(EhsCellRat *rat, EhsCellRatPref *preference);
ehs_bool     doCellularGetActiveRat(EhsCellRat *out);
ehs_bool     doCellularSetBandMask(ehs_uint64 mask);

ehs_bool     doCellularGetRegStatus(EhsCellRegStatus *out);
ehs_bool     doCellularGetIp(ehs_char *buf, ehs_uint16 buf_size);
ehs_bool     doCellularGetSignal(EhsCellSignalType *out);
ehs_bool     doCellularEvalLink(EhsCellLinkEvalType *out);
ehs_bool     doCellularGetCellInfo(EhsCellInfoType *out);

ehs_bool     doCellularGetImei(ehs_char *buf, ehs_uint16 buf_size);
ehs_bool     doCellularGetOperator(ehs_char *buf, ehs_uint16 buf_size);
ehs_bool     doCellularGetModemFwVersion(ehs_char *buf, ehs_uint16 buf_size);
ehs_bool     doCellularGetDataCounters(ehs_uint32 *tx_bytes, ehs_uint32 *rx_bytes);
ehs_bool     doCellularResetDataCounters(void);

ehs_bool     doCellularPsmRequest(ehs_bool enable, ehs_sint32 tau_s, ehs_sint32 active_s);
ehs_bool     doCellularPsmGetGranted(EhsCellPsmType *out);
ehs_bool     doCellularEdrxRequest(ehs_bool enable, EhsCellRat rat, ehs_sint32 cycle_ms);
ehs_bool     doCellularEdrxGetGranted(EhsCellEdrxType *out);
ehs_bool     doCellularRaiSet(ehs_bool as_rai, ehs_bool cp_rai);

/* SIM primitives */
ehs_bool     doCellularSimGetState(EhsSimState *out);
ehs_bool     doCellularSimGetActiveSlot(EhsSimSlot *out);
ehs_uint32   doCellularSimGetAvailableSlots(void);
ehs_uint32   doCellularSimGetSelectableSlots(void);
ehs_bool     doCellularSimSelect(EhsSimSlot slot);
ehs_bool     doCellularSimGetIccid(ehs_char *buf, ehs_uint16 buf_size);
ehs_bool     doCellularSimGetImsi(ehs_char *buf, ehs_uint16 buf_size);
ehs_bool     doCellularSimPinEnter(const ehs_char *pin);
ehs_sint32   doCellularSimPinAttemptsRemaining(void);
ehs_bool     doCellularSimPinEnable(ehs_bool enable, const ehs_char *pin);
ehs_bool     doCellularSimPinChange(const ehs_char *old_pin, const ehs_char *new_pin);
ehs_bool     doCellularSimSoftIsProvisioned(void);
ehs_bool     doCellularSimSoftProvision(const ehs_uint8 *blob, ehs_uint32 len);

/* eUICC. Every one of these must exist in every backend even where there is no
 * eUICC - the shared layer gates on capability and then delegates, exactly as
 * it does for SoftSIM, so that a backend CAN implement them. Previously only
 * SetEim was in this contract and the other five were hard-coded stubs in the
 * shared layer, which meant a target with a real eUICC had no way to implement
 * local profile management at all. */
ehs_sint32   doCellularSimProfileCount(void);
ehs_bool     doCellularSimProfileGet(ehs_sint32 index, EhsSimProfileType *out);
ehs_bool     doCellularSimProfileEnable(const ehs_char *iccid);
ehs_bool     doCellularSimProfileDisable(const ehs_char *iccid);
ehs_bool     doCellularSimProfileDelete(const ehs_char *iccid);
ehs_bool     doCellularSimProfileSetEim(const ehs_char *eim_address);
ehs_bool     doCellularSimProfileDownload(const ehs_char *activation_code,
                                          ehs_char *iccid_out,
                                          ehs_uint16 iccid_buf_size);

/* Factory install. See the caller-facing notes on EhsSim_FactoryInstall*. */
ehs_bool     doCellularSimEuiccGetEid(ehs_char *buf, ehs_uint16 buf_size);
ehs_sint32   doCellularSimFactoryInstallMaxSegment(void);
ehs_bool     doCellularSimFactoryInstallBegin(void);
ehs_bool     doCellularSimFactoryInstallPushSegment(const ehs_uint8 *seg, ehs_uint32 len);
ehs_bool     doCellularSimFactoryInstallCommit(ehs_char *iccid_out, ehs_uint16 iccid_buf_size);
void         doCellularSimFactoryInstallAbort(void);

/* Boolean accessors */
ehs_bool     isCellularInitialised(void);
ehs_bool     isCellularRegistered(void);
ehs_bool     isCellularOnline(void);
ehs_bool     isCellularSleeping(void);   /* in PSM — still registered */


/* ============================================================================
 *                    SM plumbing — shared layer owns these
 * ============================================================================
 * Implemented in Common/Components/networking/inx-cellular_link.c.  Backends
 * CALL these from their event handlers; they must not reimplement them.
 */

/* Current SM state, for backends that need to make an event decision. */
EhsCellState getCellularConnectState(void);
void         setCellularConnectState(EhsCellState state);

void                     EhsCellSetCBSource(enum eCellCallbackSource source);
enum eCellCallbackSource EhsCellGetCBSource(void);

/* Record why the last attempt failed.  Backends call this on the way to a
 * terminal state so the reason survives for Cell_GetLastFailReason(). */
void         EhsCellSetFailReason(EhsCellFailReason reason);

/* Start the shared SM thread.  Idempotent. */
void         EhsStartCellularThread(void);

/* Fired by the shared layer when the link drops after having been online, so
 * function blocks can raise a Disconnected event.  Implemented in the shared
 * layer — do NOT reimplement in target code. */
void         Common_Cellular_onDisconnected(EhsCellFailReason reason);


/* ── NVS keys ─────────────────────────────────────────────────────────────
 * Namespaced alongside EHS_NVS_WIFI_NAMESPACE.  Note that on Zephyr targets
 * EHS_NVS_SUPPORT is still 'stubbed', so nothing persists yet and the platform
 * config's build-time seeds apply on every boot. */
#define EHS_NVS_CELL_NAMESPACE          "cellular"
#define EHS_NVS_CELL_KEY_APN            "apn"
#define EHS_NVS_CELL_KEY_APN_USER       "apn_user"
#define EHS_NVS_CELL_KEY_APN_PASS       "apn_pass"
#define EHS_NVS_CELL_KEY_RAT            "rat"
#define EHS_NVS_CELL_KEY_RAT_PREF       "rat_pref"
#define EHS_NVS_CELL_KEY_ONSTARTUP      "startup"
#define EHS_NVS_CELL_KEY_PSM_ENABLE     "psm_en"
#define EHS_NVS_CELL_KEY_PSM_TAU        "psm_tau"
#define EHS_NVS_CELL_KEY_PSM_ACTIVE     "psm_act"
#define EHS_NVS_CELL_KEY_SIM_SLOT       "sim_slot"

#endif /* EHS_NETWORK_CELLULAR_SUPPORT */

#endif /* _COMPONENT_HAL_CELLULAR_H_ */
