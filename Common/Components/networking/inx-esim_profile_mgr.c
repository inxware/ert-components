/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file inx-esim_profile_mgr.c
 * esim_profile_mgr function block — local lifecycle of the profiles already on
 * an eSIM: list, read, enable, disable, delete.
 *
 * EVERYTHING HERE IS LOCAL. These map onto GSMA ES10c, which is an APDU
 * conversation with the eUICC itself, so none of them needs a network of any
 * kind — not cellular, not IP. That is the whole reason this block is separate
 * from sgp32_download: an app or a factory line can use it with no connectivity
 * at all, and the block's contract says so rather than hiding one networked
 * function among five offline ones.
 *
 * ES10c IS NOT SGP.32. Local profile management is common to SGP.22 and SGP.32
 * alike. Only downloading a NEW profile is specific to a remote-provisioning
 * profile, which is why the eIM address and the download live next door.
 *
 * Gated on EHS_CELL_CAP_EUICC_PROFILES, deliberately a different capability
 * from EHS_CELL_CAP_EUICC_DOWNLOAD: a device whose profiles were installed
 * during manufacture has this one and not the other.
 *
 * @author: inx limited
 */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-esim_profile_mgr.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal-api.h"
#include "hal_logger.h"
#include "hal_string.h"
#include "cellular.h"

//ICB STATE VAR MACRO START -- DO NOT ALTER
typedef struct inx_esim_profile_mgr_state
{
	ehs_uint8 unused;   /* no parameters and no per-instance state */
} inx_esim_profile_mgr_state_type;
//ICB STATE VAR MACRO END -- DO NOT ALTER

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(esim_profile_mgr)
EHS_FB_FUNCTION_ENTRY("do_list_profiles", 0x01, esim_profile_mgr_do_list_profiles)
EHS_FB_FUNCTION_ENTRY("do_get_profile", 0x02, esim_profile_mgr_do_get_profile)
EHS_FB_FUNCTION_ENTRY("do_enable_profile", 0x03, esim_profile_mgr_do_enable_profile)
EHS_FB_FUNCTION_ENTRY("do_disable_profile", 0x04, esim_profile_mgr_do_disable_profile)
EHS_FB_FUNCTION_ENTRY("do_delete_profile", 0x05, esim_profile_mgr_do_delete_profile)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_esim_profile_mgr_ARG_do_list_profiles_list_ok 1
#define INX_esim_profile_mgr_ARG_do_list_profiles_list_fail 2
#define INX_esim_profile_mgr_ARG_do_list_profiles_profile_count 1
#define INX_esim_profile_mgr_ARG_do_list_profiles_list_err_code 2
#define INX_esim_profile_mgr_ARG_do_get_profile_profile_index 1
#define INX_esim_profile_mgr_ARG_do_get_profile_get_ok 1
#define INX_esim_profile_mgr_ARG_do_get_profile_get_fail 2
#define INX_esim_profile_mgr_ARG_do_get_profile_profile_iccid 1
#define INX_esim_profile_mgr_ARG_do_get_profile_profile_name 2
#define INX_esim_profile_mgr_ARG_do_get_profile_profile_enabled 3
#define INX_esim_profile_mgr_ARG_do_get_profile_get_err_code 4
#define INX_esim_profile_mgr_ARG_do_enable_profile_enable_iccid 1
#define INX_esim_profile_mgr_ARG_do_enable_profile_enable_ok 1
#define INX_esim_profile_mgr_ARG_do_enable_profile_enable_fail 2
#define INX_esim_profile_mgr_ARG_do_enable_profile_enable_err_code 1
#define INX_esim_profile_mgr_ARG_do_disable_profile_disable_iccid 1
#define INX_esim_profile_mgr_ARG_do_disable_profile_disable_ok 1
#define INX_esim_profile_mgr_ARG_do_disable_profile_disable_fail 2
#define INX_esim_profile_mgr_ARG_do_disable_profile_disable_err_code 1
#define INX_esim_profile_mgr_ARG_do_delete_profile_delete_iccid 1
#define INX_esim_profile_mgr_ARG_do_delete_profile_delete_ok 1
#define INX_esim_profile_mgr_ARG_do_delete_profile_delete_fail 2
#define INX_esim_profile_mgr_ARG_do_delete_profile_delete_err_code 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

/** Shared by every failure path: log once, in the caller's words. */
static void esim_unsupported(const ehs_char *what)
{
	EHSH_LOG_WARNING("esim_profile_mgr: %s failed (%s)", what,
	                 Cell_GetFailReasonText(Cell_GetLastFailReason()));
}

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_IDENTIFY_FUNCTION(esim_profile_mgr)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_esim_profile_mgr_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_INIT_FUNCTION(esim_profile_mgr)
{
	inx_esim_profile_mgr_state_type *st =
		(inx_esim_profile_mgr_state_type *)EHS_FB_INIT_CONTEXT;

	st->unused = 0u;
	return EHS_TRUE;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(esim_profile_mgr)
{
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_list_profiles MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(esim_profile_mgr_do_list_profiles)
{
	const ehs_sint32 count = EhsSim_ProfileCount();

	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_list_profiles_profile_count)) {
		/* -1 means "cannot be determined", NOT "zero profiles". A caller that
		 * saw 0 would conclude the eUICC was empty rather than absent. */
		EHS_FB_OUT_I_API2(INX_esim_profile_mgr_ARG_do_list_profiles_profile_count) = count;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_list_profiles_list_err_code)) {
		EHS_FB_OUT_I_API2(INX_esim_profile_mgr_ARG_do_list_profiles_list_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	if (count >= 0) {
		EHS_FB_FINISH(INX_esim_profile_mgr_ARG_do_list_profiles_list_ok);
	} else {
		esim_unsupported("profile listing");
		EHS_FB_FINISH(INX_esim_profile_mgr_ARG_do_list_profiles_list_fail);
	}
}//ICB FUNCTION do_list_profiles MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_get_profile MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(esim_profile_mgr_do_get_profile)
{
	EhsSimProfileType profile;
	ehs_sint32 index = 0;
	ehs_bool ok;

	memset(&profile, 0, sizeof(profile));

	if (EHS_FB_IN_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_get_profile_profile_index)) {
		index = EHS_FB_IN_I_API2(INX_esim_profile_mgr_ARG_do_get_profile_profile_index);
	}

	ok = EhsSim_ProfileGet(index, &profile);

	/* Outputs are written either way. On failure they hold the zeroed struct,
	 * which is an empty ICCID rather than a stale one from a previous read. */
	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_get_profile_profile_iccid)) {
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_esim_profile_mgr_ARG_do_get_profile_profile_iccid),
		          profile.iccid);
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_get_profile_profile_name)) {
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_esim_profile_mgr_ARG_do_get_profile_profile_name),
		          profile.name);
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_get_profile_profile_enabled)) {
		EHS_FB_OUT_B_API2(INX_esim_profile_mgr_ARG_do_get_profile_profile_enabled) =
			profile.enabled;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_get_profile_get_err_code)) {
		EHS_FB_OUT_I_API2(INX_esim_profile_mgr_ARG_do_get_profile_get_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	if (ok) {
		EHS_FB_FINISH(INX_esim_profile_mgr_ARG_do_get_profile_get_ok);
	} else {
		esim_unsupported("profile read");
		EHS_FB_FINISH(INX_esim_profile_mgr_ARG_do_get_profile_get_fail);
	}
}//ICB FUNCTION do_get_profile MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_enable_profile MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(esim_profile_mgr_do_enable_profile)
{
	const ehs_char *iccid = NULL;
	ehs_bool ok;

	if (EHS_FB_IN_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_enable_profile_enable_iccid)) {
		iccid = EHS_FB_IN_S_API2(INX_esim_profile_mgr_ARG_do_enable_profile_enable_iccid);
	}

	/* Enabling triggers a REFRESH: the UICC restarts and any live cellular link
	 * drops, then re-attaches on the newly enabled subscription. Harmless on a
	 * factory line; in the field the app has to expect the reconnect. */
	ok = EhsSim_ProfileEnable(iccid);

	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_enable_profile_enable_err_code)) {
		EHS_FB_OUT_I_API2(INX_esim_profile_mgr_ARG_do_enable_profile_enable_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	if (ok) {
		EHS_FB_FINISH(INX_esim_profile_mgr_ARG_do_enable_profile_enable_ok);
	} else {
		esim_unsupported("profile enable");
		EHS_FB_FINISH(INX_esim_profile_mgr_ARG_do_enable_profile_enable_fail);
	}
}//ICB FUNCTION do_enable_profile MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_disable_profile MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(esim_profile_mgr_do_disable_profile)
{
	const ehs_char *iccid = NULL;
	ehs_bool ok;

	if (EHS_FB_IN_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_disable_profile_disable_iccid)) {
		iccid = EHS_FB_IN_S_API2(INX_esim_profile_mgr_ARG_do_disable_profile_disable_iccid);
	}

	/* Leaves the eUICC with NO profile enabled, and therefore the device with no
	 * subscription at all until something enables one. Switching subscription is
	 * do_enable_profile on the new ICCID — that disables the old one implicitly.
	 * This exists because ES10c defines it, not because it is the usual move. */
	ok = EhsSim_ProfileDisable(iccid);

	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_disable_profile_disable_err_code)) {
		EHS_FB_OUT_I_API2(INX_esim_profile_mgr_ARG_do_disable_profile_disable_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	if (ok) {
		EHS_FB_FINISH(INX_esim_profile_mgr_ARG_do_disable_profile_disable_ok);
	} else {
		esim_unsupported("profile disable");
		EHS_FB_FINISH(INX_esim_profile_mgr_ARG_do_disable_profile_disable_fail);
	}
}//ICB FUNCTION do_disable_profile MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_delete_profile MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(esim_profile_mgr_do_delete_profile)
{
	const ehs_char *iccid = NULL;
	ehs_bool ok;

	if (EHS_FB_IN_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_delete_profile_delete_iccid)) {
		iccid = EHS_FB_IN_S_API2(INX_esim_profile_mgr_ARG_do_delete_profile_delete_iccid);
	}

	/* IRREVERSIBLE. A deleted profile has to be downloaded again, which needs
	 * the SGP.32 Download block, a reachable server, and in most cases a fresh
	 * activation code from the operator. */
	ok = EhsSim_ProfileDelete(iccid);

	if (EHS_FB_OUT_CONNECTED_API2(INX_esim_profile_mgr_ARG_do_delete_profile_delete_err_code)) {
		EHS_FB_OUT_I_API2(INX_esim_profile_mgr_ARG_do_delete_profile_delete_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	if (ok) {
		EHS_FB_FINISH(INX_esim_profile_mgr_ARG_do_delete_profile_delete_ok);
	} else {
		esim_unsupported("profile delete");
		EHS_FB_FINISH(INX_esim_profile_mgr_ARG_do_delete_profile_delete_fail);
	}
}//ICB FUNCTION do_delete_profile MACRO END -- DO NOT ALTER THIS LINE
