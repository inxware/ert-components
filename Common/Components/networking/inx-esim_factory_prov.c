/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file inx-esim_factory_prov.c
 * esim_factory_prov function block — install a pre-built eSIM profile package
 * on a production line, with no network connection on the device.
 *
 * The factory's equipment obtains a Bound Profile Package already bound to this
 * eUICC (hence do_read_eid, which has no SGP.32 counterpart — in a download the
 * eUICC presents its own EID inside the authenticated session and the
 * application never sees it) and streams it in through this block.
 *
 * WHY THIS IS NOT A SPLIT OF sgp32_download
 * -----------------------------------------
 * It is a different operation, not the second half of one. RSP download and
 * install are a single authenticated session with the chip — a package cannot
 * be fetched now and installed later. Here the package was fetched by someone
 * else entirely, so the data flow is reversed: a reference goes OUT (the EID)
 * before payload comes IN, and there is no eIM.
 *
 * SHAPE IS THE PROTOCOL'S, NOT A CONVENIENCE
 * ------------------------------------------
 * ES10b delivers a BPP as an ordered sequence of segments, so nothing needs the
 * whole package at once. Hence begin / push / commit rather than one call:
 *
 *  * NO OFFSET. The eUICC sequences by STORE DATA block number and reassembles
 *    by concatenation. Segments must arrive in order and exactly once; there is
 *    no addressing and no seeking, so an offset port would advertise random
 *    access the chip does not have.
 *  * NO SINGLE-SEGMENT RETRY. SCP03 MAC chaining means a rejected segment has
 *    already broken the chain. Abort and restart the package — which is why
 *    do_abort_install is a first-class function rather than an afterthought.
 *  * NO APPLICATION CHECKSUM. Each segment carries an SCP03t C-MAC verified
 *    inside the eUICC, and nothing in this path — not the equipment, not this
 *    device — holds the keys to forge one. Corruption introduced anywhere fails
 *    that check. A CRC here would catch nothing extra and would imply a
 *    guarantee it does not provide.
 *
 * SECRET HANDLING
 * ---------------
 * Segments are decoded, handed to the eUICC and wiped. Nothing is written to
 * the filesystem: a package on external SPI-NOR would outlive provisioning,
 * survive an unlink (LittleFS frees the entry, not the blocks) and be captured
 * by any filesystem image or DFU bundle.
 *
 * Honest limit: this block cannot wipe what it does not own. The decoded copy
 * here is erased, but the hex text also exists in the SODL data-connection slot
 * feeding segData and in whatever upstream block received it. eRT has no secure
 * string type. The exposure is bounded — a BPP is encrypted to its target eUICC
 * (SCP03t C-ENC) and its session keys are destroyed when the process ends, so a
 * captured package yields neither subscription keys nor a replay — but "never
 * written to storage" is the accurate claim, not "exists in exactly one place".
 *
 * @author: inx limited
 */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-esim_factory_prov.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal-api.h"
#include "hal_logger.h"
#include "hal_string.h"
#include "cellular.h"

/* Bytes accepted per push, DERIVED FROM THE PORT that carries them: hex costs
 * two characters per byte, plus the NUL.
 *
 * PORT CAPACITY AND PROTOCOL BLOCK SIZE ARE NOT THE SAME THING, and conflating
 * them is a trap. A STORE DATA block is up to 255 bytes — a property of the
 * eUICC transport — but EHS_STRING_LENGTH_MAX is a per-platform figure and is
 * far smaller than the generic default on MCU targets: base_small sets it to
 * 256, so a hex-encoded 255-byte block (510 characters) does not remotely fit.
 * Deriving the byte count here means this block is correct on every target
 * instead of on the one whose numbers happened to be checked.
 *
 * Consequence for the HAL: a push is NOT necessarily one STORE DATA block. The
 * backend owns assembling conformant blocks from the pushed bytes, because only
 * it knows the eUICC's framing rules. This block's job is to move bytes in
 * order without reinterpreting them. */
#define ESIM_SEG_MAX_BYTES ((ehs_uint32)((EHS_STRING_LENGTH_MAX - 1u) / 2u))

/* A floor rather than a ceiling: below this the per-push overhead makes a
 * multi-kilobyte package impractical, and it almost certainly means a platform
 * shrank its string length without realising what depended on it. */
#if (((EHS_STRING_LENGTH_MAX - 1u) / 2u) < 64u)
#error "EHS_STRING_LENGTH_MAX is too small to stream an eSIM profile package - \
a hex segment needs 2 chars per byte; raise it for this platform"
#endif

//ICB STATE VAR MACRO START -- DO NOT ALTER
typedef struct inx_esim_factory_prov_state
{
	ehs_uint8 unused;   /* no parameters; install state lives in the HAL */
} inx_esim_factory_prov_state_type;
//ICB STATE VAR MACRO END -- DO NOT ALTER

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(esim_factory_prov)
EHS_FB_FUNCTION_ENTRY("do_read_eid", 0x01, esim_factory_prov_do_read_eid)
EHS_FB_FUNCTION_ENTRY("do_begin_install", 0x02, esim_factory_prov_do_begin_install)
EHS_FB_FUNCTION_ENTRY("do_push_segment", 0x03, esim_factory_prov_do_push_segment)
EHS_FB_FUNCTION_ENTRY("do_commit_install", 0x04, esim_factory_prov_do_commit_install)
EHS_FB_FUNCTION_ENTRY("do_abort_install", 0x05, esim_factory_prov_do_abort_install)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_esim_factory_prov_ARG_do_read_eid_read_ok 1
#define INX_esim_factory_prov_ARG_do_read_eid_read_fail 2
#define INX_esim_factory_prov_ARG_do_read_eid_eid 1
#define INX_esim_factory_prov_ARG_do_read_eid_read_err_code 2
#define INX_esim_factory_prov_ARG_do_read_eid_max_seg_len 3
#define INX_esim_factory_prov_ARG_do_begin_install_begin_ok 1
#define INX_esim_factory_prov_ARG_do_begin_install_begin_fail 2
#define INX_esim_factory_prov_ARG_do_begin_install_begin_err_code 1
#define INX_esim_factory_prov_ARG_do_push_segment_seg_data 1
#define INX_esim_factory_prov_ARG_do_push_segment_seg_len 2
#define INX_esim_factory_prov_ARG_do_push_segment_push_ok 1
#define INX_esim_factory_prov_ARG_do_push_segment_push_fail 2
#define INX_esim_factory_prov_ARG_do_push_segment_push_err_code 1
#define INX_esim_factory_prov_ARG_do_commit_install_commit_ok 1
#define INX_esim_factory_prov_ARG_do_commit_install_commit_fail 2
#define INX_esim_factory_prov_ARG_do_commit_install_inst_iccid 1
#define INX_esim_factory_prov_ARG_do_commit_install_commit_err_code 2
#define INX_esim_factory_prov_ARG_do_abort_install_abort_ok 1
#define INX_esim_factory_prov_ARG_do_abort_install_abort_err_code 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

/** Log a failure with the HAL's reason text. NEVER logs segment data. */
static void factory_failed(const ehs_char *what)
{
	EHSH_LOG_WARNING("esim_factory_prov: %s failed (%s)", what,
	                 Cell_GetFailReasonText(Cell_GetLastFailReason()));
}

/** One hex digit to its value, or -1. */
static ehs_sint32 hex_nibble(ehs_char c)
{
	if ((c >= '0') && (c <= '9')) { return (ehs_sint32)(c - '0'); }
	if ((c >= 'a') && (c <= 'f')) { return (ehs_sint32)(c - 'a') + 10; }
	if ((c >= 'A') && (c <= 'F')) { return (ehs_sint32)(c - 'A') + 10; }
	return -1;
}

/**
 * Decode hex text into out[]. Returns the byte count, or 0 on any malformation.
 *
 * Hex rather than raw bytes because a string port is NUL-terminated: a raw
 * segment would be truncated at its first zero byte, and a BPP is binary, so
 * that would happen almost immediately and silently.
 *
 * Strict: an odd digit count or a non-hex character is rejected outright rather
 * than partially decoded. A half-decoded segment would fail the eUICC's MAC
 * check anyway, but it would fail as "bad package" rather than "bad input",
 * sending the line operator after the wrong problem.
 */
static ehs_uint32 hex_decode(const ehs_char *in, ehs_uint8 *out, ehs_uint32 out_max)
{
	ehs_uint32 n = 0u;
	ehs_uint32 i = 0u;

	while (in[i] != '\0') {
		ehs_sint32 hi;
		ehs_sint32 lo;

		if (in[i + 1u] == '\0') {
			return 0u;            /* odd number of digits */
		}
		hi = hex_nibble(in[i]);
		lo = hex_nibble(in[i + 1u]);
		if ((hi < 0) || (lo < 0)) {
			return 0u;            /* not hex */
		}
		if (n >= out_max) {
			return 0u;            /* longer than one transport block */
		}
		out[n] = (ehs_uint8)(((ehs_uint32)hi << 4) | (ehs_uint32)lo);
		n++;
		i += 2u;
	}
	return n;
}

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_IDENTIFY_FUNCTION(esim_factory_prov)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_esim_factory_prov_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_INIT_FUNCTION(esim_factory_prov)
{
	inx_esim_factory_prov_state_type *st =
		(inx_esim_factory_prov_state_type *)EHS_FB_INIT_CONTEXT;

	st->unused = 0u;
	return EHS_TRUE;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(esim_factory_prov)
{
	/* An app torn down mid-install must not leave the eUICC holding a partial
	 * package or this device holding segment data. Abort is unconditional and
	 * safe when no install is running. */
	EhsSim_FactoryInstallAbort();
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_read_eid MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(esim_factory_prov_do_read_eid)
{
	ehs_char eid[EHS_CELL_EID_BUFF_MAX];
	ehs_bool ok;

	eid[0] = '\0';
	ok = EhsSim_EuiccGetEid(eid, (ehs_uint16)sizeof(eid));

	/* The EID is not secret — it identifies the chip, and the factory needs it
	 * in the clear to request the right package. */
	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_factory_prov_ARG_do_read_eid_eid)) {
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_esim_factory_prov_ARG_do_read_eid_eid), eid);
	}
	/* THE USABLE CHUNK IS THE SMALLER OF TWO INDEPENDENT LIMITS, and reporting
	 * either one alone would be wrong:
	 *
	 *   the eUICC's    what the chip will accept in one segment (SGP.22 puts
	 *                  the floor at 1020 bytes, so it is rarely the binding one)
	 *   this device's  what survives the port carrying it — ESIM_SEG_MAX_BYTES,
	 *                  derived from EHS_STRING_LENGTH_MAX and the 2-chars-per-
	 *                  byte hex encoding, so only 127 bytes on an MCU target
	 *
	 * Publishing the chip's figure would have the factory send segments this
	 * device silently truncates. The device limit is the binding one in
	 * practice, but both are honoured so the answer stays correct if either
	 * moves. -1 from the HAL means "unknown", in which case the device limit
	 * stands alone. */
	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_factory_prov_ARG_do_read_eid_max_seg_len)) {
		ehs_sint32 usable = (ehs_sint32)ESIM_SEG_MAX_BYTES;

		if (ok) {
			const ehs_sint32 euicc_max = EhsSim_FactoryInstallMaxSegment();

			if ((euicc_max > 0) && (euicc_max < usable)) {
				usable = euicc_max;
			}
		} else {
			/* No usable path at all. 0 rather than a plausible size, so a
			 * caller cannot split against a number that will never work. */
			usable = 0;
		}
		EHS_FB_OUT_I_API2(INX_esim_factory_prov_ARG_do_read_eid_max_seg_len) = usable;
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_factory_prov_ARG_do_read_eid_read_err_code)) {
		EHS_FB_OUT_I_API2(INX_esim_factory_prov_ARG_do_read_eid_read_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	if (ok) {
		EHS_FB_FINISH(INX_esim_factory_prov_ARG_do_read_eid_read_ok);
	} else {
		factory_failed("EID read");
		EHS_FB_FINISH(INX_esim_factory_prov_ARG_do_read_eid_read_fail);
	}
}//ICB FUNCTION do_read_eid MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_begin_install MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(esim_factory_prov_do_begin_install)
{
	const ehs_bool ok = EhsSim_FactoryInstallBegin();

	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_factory_prov_ARG_do_begin_install_begin_err_code)) {
		EHS_FB_OUT_I_API2(INX_esim_factory_prov_ARG_do_begin_install_begin_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	if (ok) {
		EHS_FB_FINISH(INX_esim_factory_prov_ARG_do_begin_install_begin_ok);
	} else {
		factory_failed("install begin");
		EHS_FB_FINISH(INX_esim_factory_prov_ARG_do_begin_install_begin_fail);
	}
}//ICB FUNCTION do_begin_install MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_push_segment MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(esim_factory_prov_do_push_segment)
{
	ehs_uint8 seg[ESIM_SEG_MAX_BYTES];
	const ehs_char *hex = NULL;
	ehs_sint32 declared = -1;
	ehs_uint32 len = 0u;
	ehs_bool ok = EHS_FALSE;

	if (EHS_FB_IN_CONNECTED_API2(INX_esim_factory_prov_ARG_do_push_segment_seg_data)) {
		hex = EHS_FB_IN_S_API2(INX_esim_factory_prov_ARG_do_push_segment_seg_data);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_esim_factory_prov_ARG_do_push_segment_seg_len)) {
		declared = EHS_FB_IN_I_API2(INX_esim_factory_prov_ARG_do_push_segment_seg_len);
	}

	if ((hex != NULL) && (hex[0] != '\0')) {
		len = hex_decode(hex, seg, (ehs_uint32)sizeof(seg));

		/* DECLARED LENGTH IS A FRAMING CHECK, NOT A CHECKSUM. It catches the one
		 * failure the cryptographic layer diagnoses badly: a string port is
		 * fixed-width, so an over-long segment is TRUNCATED SILENTLY, and the
		 * eUICC would then reject it as a MAC failure — sending the line
		 * operator after a corrupt package when the real fault is a caller
		 * splitting on the wrong size. Comparing against what the caller said it
		 * sent names that immediately. Optional: unconnected means "trust the
		 * decoded length". */
		if ((len != 0u) && (declared >= 0) && ((ehs_uint32)declared != len)) {
			EHSH_LOG_ERROR("esim_factory_prov: segment length mismatch - caller "
			               "declared %d bytes, decoded %u. The segment was very "
			               "likely truncated: this target accepts at most %u "
			               "bytes per push (see maxSegLen on ReadEID).",
			               (int)declared, (unsigned)len,
			               (unsigned)ESIM_SEG_MAX_BYTES);
			len = 0u;
		}

		if (len == 0u) {
			/* Malformed input, not a package problem. Say which, because on a
			 * production line these send you to completely different places. */
			EHSH_LOG_ERROR("esim_factory_prov: segment is not valid hex, or is "
			               "longer than %u bytes", (unsigned)ESIM_SEG_MAX_BYTES);
		} else {
			ok = EhsSim_FactoryInstallPushSegment(seg, len);
			if (!ok) {
				factory_failed("segment push");
			}
		}
	} else {
		EHSH_LOG_WARNING("esim_factory_prov: no segment data supplied");
	}

	/* Wipe before returning, whatever happened. The decoded segment has no
	 * reason to outlive this call. */
	EhsMemset(seg, 0, sizeof(seg));
	len = 0u;

	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_factory_prov_ARG_do_push_segment_push_err_code)) {
		EHS_FB_OUT_I_API2(INX_esim_factory_prov_ARG_do_push_segment_push_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	/* A rejected segment cannot be re-sent — the MAC chain is broken. The app
	 * must treat push_fail as "abort and restart the package", not "retry". */
	if (ok) {
		EHS_FB_FINISH(INX_esim_factory_prov_ARG_do_push_segment_push_ok);
	} else {
		EHS_FB_FINISH(INX_esim_factory_prov_ARG_do_push_segment_push_fail);
	}
}//ICB FUNCTION do_push_segment MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_commit_install MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(esim_factory_prov_do_commit_install)
{
	ehs_char iccid[EHS_CELL_ICCID_BUFF_MAX];
	ehs_bool ok;

	iccid[0] = '\0';
	ok = EhsSim_FactoryInstallCommit(iccid, (ehs_uint16)sizeof(iccid));

	/* As with a download, the profile is installed DISABLED. instICCID is what
	 * the eSIM Profiles block's Enable input needs. */
	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_factory_prov_ARG_do_commit_install_inst_iccid)) {
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_esim_factory_prov_ARG_do_commit_install_inst_iccid), iccid);
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_factory_prov_ARG_do_commit_install_commit_err_code)) {
		EHS_FB_OUT_I_API2(INX_esim_factory_prov_ARG_do_commit_install_commit_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	if (ok) {
		EHS_FB_FINISH(INX_esim_factory_prov_ARG_do_commit_install_commit_ok);
	} else {
		factory_failed("install commit");
		/* Leave nothing half-installed behind a failed commit. */
		EhsSim_FactoryInstallAbort();
		EHS_FB_FINISH(INX_esim_factory_prov_ARG_do_commit_install_commit_fail);
	}
}//ICB FUNCTION do_commit_install MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_abort_install MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(esim_factory_prov_do_abort_install)
{
	/* No fail port, deliberately. Abort is the error path and the timeout
	 * action; a cleanup that can itself fail is a cleanup callers will skip.
	 * Safe when no install is in progress. */
	EhsSim_FactoryInstallAbort();

	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_factory_prov_ARG_do_abort_install_abort_err_code)) {
		EHS_FB_OUT_I_API2(INX_esim_factory_prov_ARG_do_abort_install_abort_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	EHS_FB_FINISH(INX_esim_factory_prov_ARG_do_abort_install_abort_ok);
}//ICB FUNCTION do_abort_install MACRO END -- DO NOT ALTER THIS LINE
