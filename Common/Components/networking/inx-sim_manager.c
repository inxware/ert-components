/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file inx-sim_manager.c
 * sim_manager function block — SIM state, identity, PIN, and runtime switching
 * between the three SIM types.
 *
 * *** THE PIN RULE ***
 * A SIM allows three wrong PIN attempts and then requires a PUK from the
 * operator. That makes an automatic retry the single most destructive thing
 * this block could do: a wrong stored PIN retried on every boot permanently
 * locks a customer's SIM in three power cycles. So:
 *
 *   - autoEnterPin defaults to OFF.
 *   - do_enter_pin makes exactly ONE attempt and never retries.
 *   - The shared layer refuses outright when only one attempt remains.
 *
 * Do not "improve" any of that with a retry loop.
 *
 * *** AVAILABLE vs SELECTABLE ***
 * Two outputs describe SIM types and they answer different questions.
 * availableTypes is what exists on the hardware; selectableTypes is what
 * software can switch to. A type can be available but not selectable - where a
 * card socket and an eSIM share one interface, a card-detect autoswitch decides
 * between them and software has no say. A UI must offer selectableTypes, or it
 * offers a switch that silently cannot happen.
 *
 * @author: inx limited
 */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-sim_manager.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal-api.h"
#include "hal_logger.h"
#include "hal_string.h"
#include "cellular.h"

/* Scratch buffer for do_read's two string getters - derived from its consumers,
 * with the invariant enforced at build time rather than trusted. */
#define SIM_READ_BUFF_MAX EHS_CELL_ICCID_BUFF_MAX

#if (EHS_CELL_ICCID_BUFF_MAX > SIM_READ_BUFF_MAX) || \
    (EHS_CELL_IMSI_BUFF_MAX  > SIM_READ_BUFF_MAX)
#error "SIM_READ_BUFF_MAX is smaller than one of the getters it serves - \
raise it to the largest of EHS_CELL_ICCID/IMSI_BUFF_MAX"
#endif

//ICB STATE VAR MACRO START -- DO NOT ALTER
typedef struct inx_sim_manager_state
{
	ehs_bool   autoEnterPin;
	ehs_sint32 preferredType;
} inx_sim_manager_state_type;
//ICB STATE VAR MACRO END -- DO NOT ALTER

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(sim_manager)
EHS_FB_FUNCTION_ENTRY("do_read", 0x01, sim_manager_do_read)
EHS_FB_FUNCTION_ENTRY("do_select_type", 0x02, sim_manager_do_select_type)
EHS_FB_FUNCTION_ENTRY("do_enter_pin", 0x03, sim_manager_do_enter_pin)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_sim_manager_ARG_do_read_read_ok 1
#define INX_sim_manager_ARG_do_read_sim_state 1
#define INX_sim_manager_ARG_do_read_active_type 2
#define INX_sim_manager_ARG_do_read_available_types 3
#define INX_sim_manager_ARG_do_read_selectable_types 4
#define INX_sim_manager_ARG_do_read_iccid 5
#define INX_sim_manager_ARG_do_read_imsi 6
#define INX_sim_manager_ARG_do_read_pin_attempts_left 7
#define INX_sim_manager_ARG_do_read_err_code 8
#define INX_sim_manager_ARG_do_select_type_sim_type 1
#define INX_sim_manager_ARG_do_select_type_select_ok 1
#define INX_sim_manager_ARG_do_select_type_select_fail 2
#define INX_sim_manager_ARG_do_enter_pin_pin 1
#define INX_sim_manager_ARG_do_enter_pin_pin_ok 1
#define INX_sim_manager_ARG_do_enter_pin_pin_fail 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
#define INX_FB_sim_manager_autoEnterPin 0
#define INX_FB_sim_manager_preferredType 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER

/* CDF simType (0 SIM, 1 SoftSIM, 2 eSIM) -> EhsSimSlot. The CDF numbering is
 * user-facing and deliberately ordered by how common each is, so it is NOT the
 * same as the enum's declaration order. */
static EhsSimSlot slot_from_param(ehs_sint32 v)
{
	switch (v) {
	case 1:  return EHS_SIM_SLOT_SOFT;
	case 2:  return EHS_SIM_SLOT_EMBEDDED;
	case 0:
	default: return EHS_SIM_SLOT_PHYSICAL;
	}
}

static ehs_sint32 param_from_slot(EhsSimSlot s)
{
	switch (s) {
	case EHS_SIM_SLOT_SOFT:     return 1;
	case EHS_SIM_SLOT_EMBEDDED: return 2;
	case EHS_SIM_SLOT_PHYSICAL:
	default:                    return 0;
	}
}

/** Re-map a HAL slot bitmask into the CDF's simType numbering. */
static ehs_sint32 mask_to_param(ehs_uint32 hal_mask)
{
	ehs_sint32 out = 0;

	if (hal_mask & (1u << (ehs_uint32)EHS_SIM_SLOT_PHYSICAL)) { out |= (1 << 0); }
	if (hal_mask & (1u << (ehs_uint32)EHS_SIM_SLOT_SOFT))     { out |= (1 << 1); }
	if (hal_mask & (1u << (ehs_uint32)EHS_SIM_SLOT_EMBEDDED)) { out |= (1 << 2); }
	return out;
}

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_IDENTIFY_FUNCTION(sim_manager)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_sim_manager_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_INIT_FUNCTION(sim_manager)
{
	inx_sim_manager_state_type *st = (inx_sim_manager_state_type *)EHS_FB_INIT_CONTEXT;

	st->autoEnterPin  = INX_FB_sim_manager_autoEnterPin;
	st->preferredType = INX_FB_sim_manager_preferredType;

	/* autoEnterPin is ehs_bool (1 byte) - %d would write 4. */
	EhsSscanf(EHS_FB_INIT_PARAMETERS, "%hhu %d",
	          &(st->autoEnterPin), &(st->preferredType));

	/* Deliberately NOT acting on autoEnterPin here. There is nowhere to read a
	 * stored PIN from yet (NVS is stubbed on the Zephyr targets), and entering
	 * one automatically at init is exactly the path that locks a SIM if it is
	 * wrong. When a PIN store exists, guard it on a verified-good flag. */
	return EHS_TRUE;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(sim_manager)
{
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_read MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(sim_manager_do_read)
{
	/* Shared by both string getters, so sized for the larger. Correct today,
	 * guarded at the top of this file so it stays correct - the same shape
	 * silently emptied cell_modem_manager's operator port. */
	ehs_char buf[SIM_READ_BUFF_MAX];

	if (EHS_FB_OUT_CONNECTED_API2(INX_sim_manager_ARG_do_read_sim_state)) {
		EHS_FB_OUT_I_API2(INX_sim_manager_ARG_do_read_sim_state) = (ehs_sint32)EhsSim_GetState();
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_sim_manager_ARG_do_read_active_type)) {
		EHS_FB_OUT_I_API2(INX_sim_manager_ARG_do_read_active_type) =
			param_from_slot(EhsSim_GetActiveSlot());
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_sim_manager_ARG_do_read_available_types)) {
		EHS_FB_OUT_I_API2(INX_sim_manager_ARG_do_read_available_types) =
			mask_to_param(EhsSim_GetAvailableSlots());
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_sim_manager_ARG_do_read_selectable_types)) {
		EHS_FB_OUT_I_API2(INX_sim_manager_ARG_do_read_selectable_types) =
			mask_to_param(EhsSim_GetSelectableSlots());
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_sim_manager_ARG_do_read_iccid)) {
		buf[0] = '\0';
		(void)EhsSim_GetIccid(buf, (ehs_uint16)EHS_CELL_ICCID_BUFF_MAX);
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_sim_manager_ARG_do_read_iccid), buf);
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_sim_manager_ARG_do_read_imsi)) {
		buf[0] = '\0';
		(void)EhsSim_GetImsi(buf, (ehs_uint16)EHS_CELL_IMSI_BUFF_MAX);
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_sim_manager_ARG_do_read_imsi), buf);
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_sim_manager_ARG_do_read_pin_attempts_left)) {
		/* -1 means the modem does not report it. Treat unknown as "do not
		 * risk an attempt", never as "plenty left". */
		EHS_FB_OUT_I_API2(INX_sim_manager_ARG_do_read_pin_attempts_left) =
			EhsSim_PinAttemptsRemaining();
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_sim_manager_ARG_do_read_err_code)) {
		EHS_FB_OUT_I_API2(INX_sim_manager_ARG_do_read_err_code) =
			(ehs_sint32)Cell_GetLastFailReason();
	}

	EHS_FB_FINISH(INX_sim_manager_ARG_do_read_read_ok);
}//ICB FUNCTION do_read MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_select_type MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(sim_manager_do_select_type)
{
	inx_sim_manager_state_type *st = (inx_sim_manager_state_type *)EHS_FB_RUN_CONTEXT;
	ehs_sint32 want = st->preferredType;

	if (EHS_FB_IN_CONNECTED_API2(INX_sim_manager_ARG_do_select_type_sim_type)) {
		want = EHS_FB_IN_I_API2(INX_sim_manager_ARG_do_select_type_sim_type);
	}

	/* EhsSim_Select validates against the SELECTABLE mask and hands the switch
	 * to the connection state machine, which drops the link and re-attaches on
	 * the new SIM. Do not try to orchestrate that here. */
	if (EhsSim_Select(slot_from_param(want))) {
		EHS_FB_FINISH(INX_sim_manager_ARG_do_select_type_select_ok);
	} else {
		EHSH_LOG_WARNING("sim_manager: SIM type %d could not be selected", (int)want);
		EHS_FB_FINISH(INX_sim_manager_ARG_do_select_type_select_fail);
	}
}//ICB FUNCTION do_select_type MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION do_enter_pin MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(sim_manager_do_enter_pin)
{
	const ehs_char *pin = NULL;

	if (EHS_FB_IN_CONNECTED_API2(INX_sim_manager_ARG_do_enter_pin_pin)) {
		pin = EHS_FB_IN_S_API2(INX_sim_manager_ARG_do_enter_pin_pin);
	}

	if ((pin == NULL) || (pin[0] == '\0')) {
		EHSH_LOG_WARNING("sim_manager: no PIN supplied");
		EHS_FB_FINISH(INX_sim_manager_ARG_do_enter_pin_pin_fail);
		return;
	}

	/* EXACTLY ONE ATTEMPT. No retry, here or anywhere above. Three wrong PINs
	 * require a PUK from the operator to recover. */
	if (EhsSim_PinEnter(pin)) {
		EHS_FB_FINISH(INX_sim_manager_ARG_do_enter_pin_pin_ok);
	} else {
		EHSH_LOG_ERROR("sim_manager: PIN rejected - NOT retrying (a further "
		               "wrong attempt can lock the SIM)");
		EHS_FB_FINISH(INX_sim_manager_ARG_do_enter_pin_pin_fail);
	}
}//ICB FUNCTION do_enter_pin MACRO END -- DO NOT ALTER THIS LINE
