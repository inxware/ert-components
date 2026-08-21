/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file inx-sgp32_download.c
 * sgp32_download function block — download a NEW subscription profile onto an
 * eSIM over GSMA SGP.32 remote SIM provisioning, and configure the eIM address
 * it runs against.
 *
 * THE ONLY eSIM OPERATION THAT NEEDS A SERVER. That is why it is its own block:
 * managing profiles already on the chip (esim_profile_mgr) is a purely local
 * ES10c conversation, and burying one networked function among five offline
 * ones made the block's contract a lie for anyone provisioning in a factory.
 *
 * WHY A PROFILE CANNOT BE FETCHED NOW AND INSTALLED LATER. The download is a
 * mutually authenticated session: the eUICC signs a challenge with its own key,
 * the SM-DP+ verifies the EID against the EUM certificate chain, and the session
 * keys come from an ECKA exchange with that specific chip. The resulting Bound
 * Profile Package is encrypted so only that eUICC can decrypt it, and the
 * decryption keys live in eUICC session state that a reboot destroys. So there
 * is no staging a package to a file, and no preparing one device's profile and
 * applying it to another. GSMA SGP.41/.42 (In-Factory Profile Provisioning) is
 * the work item that addresses the factory case; SGP.42 is not published, so
 * there is nothing to implement against yet.
 *
 * The session needs IP reachability to the eIM / SM-DP+, but NOT necessarily
 * over cellular — a factory line can carry it over Ethernet, Wi-Fi or a test
 * fixture, which is what breaks the "need a network to get a SIM" circularity.
 *
 * Gated on EHS_CELL_CAP_EUICC_DOWNLOAD, separately from the local lifecycle's
 * EHS_CELL_CAP_EUICC_PROFILES.
 *
 * @author: inx limited
 */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-sgp32_download.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal-api.h"
#include "hal_logger.h"
#include "hal_string.h"
#include "cellular.h"

//ICB STATE VAR MACRO START -- DO NOT ALTER
typedef struct inx_sgp32_download_state
{
	ehs_uint8 unused;   /* no parameters and no per-instance state */
} inx_sgp32_download_state_type;
//ICB STATE VAR MACRO END -- DO NOT ALTER

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(sgp32_download)
EHS_FB_FUNCTION_ENTRY("do_set_eim", 0x01, sgp32_download_do_set_eim)
EHS_FB_FUNCTION_ENTRY("do_download", 0x02, sgp32_download_do_download)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_sgp32_download_ARG_do_set_eim_eim_address 1
#define INX_sgp32_download_ARG_do_set_eim_set_eim_ok 1
#define INX_sgp32_download_ARG_do_set_eim_set_eim_fail 2
#define INX_sgp32_download_ARG_do_download_activation_code 1
#define INX_sgp32_download_ARG_do_download_dl_ok 1
#define INX_sgp32_download_ARG_do_download_dl_fail 2
#define INX_sgp32_download_ARG_do_download_dl_err_code 1
#define INX_sgp32_download_ARG_do_download_dl_iccid 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

static void sgp32_failed(const ehs_char *what)
{
	EHSH_LOG_WARNING("sgp32_download: %s failed (%s)", what,
	                 Cell_GetFailReasonText(Cell_GetLastFailReason()));
}

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_IDENTIFY_FUNCTION(sgp32_download)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_sgp32_download_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_INIT_FUNCTION(sgp32_download)
{
	inx_sgp32_download_state_type *st =
		(inx_sgp32_download_state_type *)EHS_FB_INIT_CONTEXT;

	st->unused = 0u;
	return EHS_TRUE;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(sgp32_download)
{
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_set_eim MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(sgp32_download_do_set_eim)
{
	const ehs_char *addr = NULL;

	if (EHS_FB_IN_CONNECTED_API2(INX_sgp32_download_ARG_do_set_eim_eim_address)) {
		addr = EHS_FB_IN_S_API2(INX_sgp32_download_ARG_do_set_eim_eim_address);
	}

	/* The eIM address is the one piece of configuration SGP.32 always needs,
	 * whether the IPA runs on the device (IPAd) or on the eUICC (IPAe). Set it
	 * before attempting a download. */
	if (EhsSim_ProfileSetEim(addr)) {
		EHS_FB_FINISH(INX_sgp32_download_ARG_do_set_eim_set_eim_ok);
	} else {
		sgp32_failed("eIM configuration");
		EHS_FB_FINISH(INX_sgp32_download_ARG_do_set_eim_set_eim_fail);
	}
}//ICB FUNCTION do_set_eim MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_download MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(sgp32_download_do_download)
{
	const ehs_char *code = NULL;
	ehs_char iccid[EHS_CELL_ICCID_BUFF_MAX];
	ehs_bool ok;

	iccid[0] = '\0';

	if (EHS_FB_IN_CONNECTED_API2(INX_sgp32_download_ARG_do_download_activation_code)) {
		code = EHS_FB_IN_S_API2(INX_sgp32_download_ARG_do_download_activation_code);
	}

	ok = EhsSim_ProfileDownload(code, iccid, (ehs_uint16)sizeof(iccid));

	/* The ICCID is the whole point of this output: a downloaded profile is
	 * installed DISABLED, so the app's next step is always
	 * esim_profile_mgr.Enable, and without this it would have to list profiles
	 * and diff against a snapshot taken before the download to work out which
	 * one it just got. Empty on failure rather than stale. */
	if (EHS_FB_OUT_CONNECTED_API2(INX_sgp32_download_ARG_do_download_dl_iccid)) {
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_sgp32_download_ARG_do_download_dl_iccid), iccid);
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_sgp32_download_ARG_do_download_dl_err_code)) {
		EHS_FB_OUT_I_API2(INX_sgp32_download_ARG_do_download_dl_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	/* A downloaded profile is installed but NOT enabled. Use the eSIM Profiles
	 * block to enable it — deliberately a separate, local step, so that a
	 * download cannot silently drop the link that carried it. */
	if (ok) {
		EHS_FB_FINISH(INX_sgp32_download_ARG_do_download_dl_ok);
	} else {
		sgp32_failed("profile download");
		EHS_FB_FINISH(INX_sgp32_download_ARG_do_download_dl_fail);
	}
}//ICB FUNCTION do_download MACRO END -- DO NOT ALTER THIS LINE
