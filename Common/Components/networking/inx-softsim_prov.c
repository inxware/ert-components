/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file inx-softsim_prov.c
 * softsim_prov function block — write a SoftSIM subscription profile.
 *
 * Kept OUT of sim_manager deliberately. Provisioning writes cryptographic key
 * material to flash, is not reversible in place, and is a commissioning-time
 * action rather than a runtime one. Putting it in the block an application
 * polls every second would make accidental invocation far too easy.
 *
 * SoftSIM is a software-only subscriber identity: no card, no chip. The blob
 * comes from the connectivity provider's tooling and contains the subscription
 * keys, so it is secret material - do not log it, and do not leave it in an
 * app's string constants if that app is distributed.
 *
 * @author: inx limited
 */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-softsim_prov.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal-api.h"
#include "hal_logger.h"
#include "hal_string.h"
#include "cellular.h"

//ICB STATE VAR MACRO START -- DO NOT ALTER
typedef struct inx_softsim_prov_state
{
	ehs_sint32 provider;
} inx_softsim_prov_state_type;
//ICB STATE VAR MACRO END -- DO NOT ALTER

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(softsim_prov)
EHS_FB_FUNCTION_ENTRY("do_provision", 0x01, softsim_prov_do_provision)
EHS_FB_FUNCTION_ENTRY("do_check", 0x02, softsim_prov_do_check)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_softsim_prov_ARG_do_provision_blob 1
#define INX_softsim_prov_ARG_do_provision_prov_ok 1
#define INX_softsim_prov_ARG_do_provision_prov_fail 2
#define INX_softsim_prov_ARG_do_provision_err_code 1
#define INX_softsim_prov_ARG_do_check_check_ok 1
#define INX_softsim_prov_ARG_do_check_provisioned 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
#define INX_FB_softsim_prov_provider 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_IDENTIFY_FUNCTION(softsim_prov)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_softsim_prov_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_INIT_FUNCTION(softsim_prov)
{
	inx_softsim_prov_state_type *st = (inx_softsim_prov_state_type *)EHS_FB_INIT_CONTEXT;

	st->provider = INX_FB_softsim_prov_provider;
	EhsSscanf(EHS_FB_INIT_PARAMETERS, "%d", &(st->provider));
	return EHS_TRUE;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(softsim_prov)
{
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_provision MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(softsim_prov_do_provision)
{
	const ehs_char *blob = NULL;
	ehs_bool ok = EHS_FALSE;

	if (EHS_FB_IN_CONNECTED_API2(INX_softsim_prov_ARG_do_provision_blob)) {
		blob = EHS_FB_IN_S_API2(INX_softsim_prov_ARG_do_provision_blob);
	}

	if ((blob != NULL) && (blob[0] != '\0')) {
		/* Never log the blob itself - it carries the subscription keys. */
		ok = EhsSim_SoftProvision((const ehs_uint8 *)blob,
		                          (ehs_uint32)EhsStrlen((ehs_char *)blob));
		if (!ok) {
			EHSH_LOG_ERROR("softsim_prov: provisioning failed (%s)",
			               Cell_GetFailReasonText(Cell_GetLastFailReason()));
		}
	} else {
		EHSH_LOG_WARNING("softsim_prov: no profile supplied");
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_softsim_prov_ARG_do_provision_err_code)) {
		EHS_FB_OUT_I_API2(INX_softsim_prov_ARG_do_provision_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	if (ok) {
		EHS_FB_FINISH(INX_softsim_prov_ARG_do_provision_prov_ok);
	} else {
		EHS_FB_FINISH(INX_softsim_prov_ARG_do_provision_prov_fail);
	}
}//ICB FUNCTION do_provision MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_check MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(softsim_prov_do_check)
{
	if (EHS_FB_OUT_CONNECTED_API2(INX_softsim_prov_ARG_do_check_provisioned)) {
		EHS_FB_OUT_B_API2(INX_softsim_prov_ARG_do_check_provisioned) =
			EhsSim_SoftIsProvisioned();
	}
	EHS_FB_FINISH(INX_softsim_prov_ARG_do_check_check_ok);
}//ICB FUNCTION do_check MACRO END -- DO NOT ALTER THIS LINE
