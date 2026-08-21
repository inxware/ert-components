/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file cellular_stubbed.c
 * No-op cellular backend.
 *
 * Selected when a platform sets EHS_NETWORK_CELLULAR_SUPPORT but has no real
 * modem backend (EHS_CELLULAR_SUPPORT=stubbed), and used as the reference for
 * what a new port has to provide.
 *
 * Two rules this file exists to enforce:
 *
 *  1. **Every entry point matches the real prototype exactly.** A stub whose
 *     signatures drift from cellular.h compiles fine while nothing calls it and
 *     then explodes with "conflicting types for ..." the moment something does.
 *
 *  2. **Capabilities are reported as zero, and every call fails cleanly.** The
 *     temptation with a stub is to return EHS_TRUE so callers "work". That is
 *     exactly wrong here: a caller that believes it is attached will sit in
 *     EhsCell_Searching forever. Failing immediately and advertising no
 *     capabilities is the honest answer, and Cell_GetLastFailReason() reports
 *     EhsCellFail_NotSupported so the reason is visible rather than mysterious.
 *
 * @author: inx limited
 */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include "globals.h"

#ifdef EHS_NETWORK_CELLULAR_SUPPORT

#include "cellular.h"

/*****************************************************************************/
/* Capabilities                                                              */

ehs_uint32 doCellularGetCapabilities(void)
{
	return 0u;   /* nothing is supported — see rule 2 in the file header */
}

/*****************************************************************************/
/* Lifecycle                                                                 */

ehs_bool doCellularInit(void)
{
	return EHS_FALSE;
}

void doCellularDestroy(void)
{
}

ehs_bool doCellularStart(void)
{
	return EHS_FALSE;
}

ehs_bool doCellularConnect(const EhsCellConfigType *cfg)
{
	(void)cfg;
	EhsCellSetFailReason(EhsCellFail_NotSupported);
	return EHS_FALSE;
}

ehs_bool doCellularDisconnect(void)
{
	return EHS_FALSE;
}

/*****************************************************************************/
/* Power / RAT                                                               */

ehs_bool doCellularSetPowerState(EhsCellPowerState state)
{
	(void)state;
	return EHS_FALSE;
}

ehs_bool doCellularGetPowerState(EhsCellPowerState *out)
{
	if (out != NULL) {
		*out = EHS_CELL_POWER_OFF;
	}
	return EHS_FALSE;
}

ehs_bool doCellularSetRat(EhsCellRat rat, EhsCellRatPref preference)
{
	(void)rat;
	(void)preference;
	return EHS_FALSE;
}

ehs_bool doCellularGetRat(EhsCellRat *rat, EhsCellRatPref *preference)
{
	if (rat != NULL) {
		*rat = EHS_CELL_RAT_NONE;
	}
	if (preference != NULL) {
		*preference = EHS_CELL_RAT_PREF_AUTO;
	}
	return EHS_FALSE;
}

ehs_bool doCellularGetActiveRat(EhsCellRat *out)
{
	if (out != NULL) {
		*out = EHS_CELL_RAT_NONE;
	}
	return EHS_FALSE;
}

ehs_bool doCellularSetBandMask(ehs_uint64 mask)
{
	(void)mask;
	return EHS_FALSE;
}

/*****************************************************************************/
/* Status / diagnostics                                                      */

ehs_bool doCellularGetRegStatus(EhsCellRegStatus *out)
{
	if (out != NULL) {
		*out = EhsCellReg_NotRegistered;
	}
	return EHS_FALSE;
}

ehs_bool doCellularGetIp(ehs_char *buf, ehs_uint16 buf_size)
{
	if ((buf != NULL) && (buf_size > 0u)) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
}

ehs_bool doCellularGetSignal(EhsCellSignalType *out)
{
	if (out != NULL) {
		out->rsrp_dbm = EHS_CELL_VALUE_INVALID;
		out->rsrq_db  = EHS_CELL_VALUE_INVALID;
		out->snr_db   = EHS_CELL_VALUE_INVALID;
		out->rssi_dbm = EHS_CELL_VALUE_INVALID;
	}
	return EHS_FALSE;
}

ehs_bool doCellularEvalLink(EhsCellLinkEvalType *out)
{
	if (out != NULL) {
		out->signal.rsrp_dbm = EHS_CELL_VALUE_INVALID;
		out->signal.rsrq_db  = EHS_CELL_VALUE_INVALID;
		out->signal.snr_db   = EHS_CELL_VALUE_INVALID;
		out->signal.rssi_dbm = EHS_CELL_VALUE_INVALID;
		out->energy          = EhsCellEnergy_Unknown;
		out->ce_level        = EHS_CELL_VALUE_INVALID;
		out->tx_power_dbm    = EHS_CELL_VALUE_INVALID;
		out->tx_repetitions  = EHS_CELL_VALUE_INVALID;
		out->rx_repetitions  = EHS_CELL_VALUE_INVALID;
		out->dl_pathloss_db  = EHS_CELL_VALUE_INVALID;
		out->band            = EHS_CELL_VALUE_INVALID;
	}
	return EHS_FALSE;
}

ehs_bool doCellularGetCellInfo(EhsCellInfoType *out)
{
	if (out != NULL) {
		out->mcc          = 0;
		out->mnc          = 0;
		out->cell_id      = EHS_CELL_CELLID_INVALID;
		out->tac          = 0u;
		out->earfcn       = 0u;
		out->phys_cell_id = EHS_CELL_VALUE_INVALID;
		out->band         = EHS_CELL_VALUE_INVALID;
	}
	return EHS_FALSE;
}

ehs_bool doCellularGetImei(ehs_char *buf, ehs_uint16 buf_size)
{
	if ((buf != NULL) && (buf_size > 0u)) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
}

ehs_bool doCellularGetOperator(ehs_char *buf, ehs_uint16 buf_size)
{
	if ((buf != NULL) && (buf_size > 0u)) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
}

ehs_bool doCellularGetModemFwVersion(ehs_char *buf, ehs_uint16 buf_size)
{
	if ((buf != NULL) && (buf_size > 0u)) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
}

ehs_bool doCellularGetDataCounters(ehs_uint32 *tx_bytes, ehs_uint32 *rx_bytes)
{
	if (tx_bytes != NULL) {
		*tx_bytes = 0u;
	}
	if (rx_bytes != NULL) {
		*rx_bytes = 0u;
	}
	return EHS_FALSE;
}

ehs_bool doCellularResetDataCounters(void)
{
	return EHS_FALSE;
}

/*****************************************************************************/
/* Power saving                                                              */

ehs_bool doCellularPsmRequest(ehs_bool enable, ehs_sint32 tau_s, ehs_sint32 active_s)
{
	(void)enable;
	(void)tau_s;
	(void)active_s;
	return EHS_FALSE;
}

ehs_bool doCellularPsmGetGranted(EhsCellPsmType *out)
{
	if (out != NULL) {
		out->granted  = EHS_FALSE;
		out->tau_s    = 0;
		out->active_s = 0;
	}
	return EHS_FALSE;
}

ehs_bool doCellularEdrxRequest(ehs_bool enable, EhsCellRat rat, ehs_sint32 cycle_ms)
{
	(void)enable;
	(void)rat;
	(void)cycle_ms;
	return EHS_FALSE;
}

ehs_bool doCellularEdrxGetGranted(EhsCellEdrxType *out)
{
	if (out != NULL) {
		out->granted  = EHS_FALSE;
		out->rat      = EHS_CELL_RAT_NONE;
		out->cycle_ms = 0;
		out->ptw_ms   = 0;
	}
	return EHS_FALSE;
}

ehs_bool doCellularRaiSet(ehs_bool as_rai, ehs_bool cp_rai)
{
	(void)as_rai;
	(void)cp_rai;
	return EHS_FALSE;
}

/*****************************************************************************/
/* SIM                                                                       */

ehs_bool doCellularSimGetState(EhsSimState *out)
{
	if (out != NULL) {
		*out = EHS_SIM_STATE_UNKNOWN;
	}
	return EHS_FALSE;
}

ehs_bool doCellularSimGetActiveSlot(EhsSimSlot *out)
{
	if (out != NULL) {
		*out = EHS_SIM_SLOT_PHYSICAL;
	}
	return EHS_FALSE;
}

ehs_uint32 doCellularSimGetAvailableSlots(void)
{
	return 0u;
}

ehs_uint32 doCellularSimGetSelectableSlots(void)
{
	return 0u;
}

ehs_bool doCellularSimSelect(EhsSimSlot slot)
{
	(void)slot;
	return EHS_FALSE;
}

ehs_bool doCellularSimGetIccid(ehs_char *buf, ehs_uint16 buf_size)
{
	if ((buf != NULL) && (buf_size > 0u)) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
}

ehs_bool doCellularSimGetImsi(ehs_char *buf, ehs_uint16 buf_size)
{
	if ((buf != NULL) && (buf_size > 0u)) {
		buf[0] = '\0';
	}
	return EHS_FALSE;
}

ehs_bool doCellularSimPinEnter(const ehs_char *pin)
{
	(void)pin;
	return EHS_FALSE;
}

ehs_sint32 doCellularSimPinAttemptsRemaining(void)
{
	return -1;   /* unknown — callers must treat this as "do not auto-retry" */
}

ehs_bool doCellularSimPinEnable(ehs_bool enable, const ehs_char *pin)
{
	(void)enable;
	(void)pin;
	return EHS_FALSE;
}

ehs_bool doCellularSimPinChange(const ehs_char *old_pin, const ehs_char *new_pin)
{
	(void)old_pin;
	(void)new_pin;
	return EHS_FALSE;
}

ehs_bool doCellularSimSoftIsProvisioned(void)
{
	return EHS_FALSE;
}

ehs_bool doCellularSimSoftProvision(const ehs_uint8 *blob, ehs_uint32 len)
{
	(void)blob;
	(void)len;
	return EHS_FALSE;
}

/* eUICC. This backend advertises neither EHS_CELL_CAP_EUICC_PROFILES nor
 * EHS_CELL_CAP_EUICC_DOWNLOAD, so the shared layer refuses every one of these
 * before it gets here. They exist to satisfy the contract and to keep the link
 * resolvable on a target that has no eUICC. */
ehs_sint32 doCellularSimProfileCount(void)
{
	return -1;   /* "cannot be determined", never 0 */
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
	return EHS_FALSE;
}

ehs_bool isCellularRegistered(void)
{
	return EHS_FALSE;
}

ehs_bool isCellularOnline(void)
{
	return EHS_FALSE;
}

ehs_bool isCellularSleeping(void)
{
	return EHS_FALSE;
}

#endif /* EHS_NETWORK_CELLULAR_SUPPORT */
