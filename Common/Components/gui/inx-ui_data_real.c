#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-ui_data_real.h"
#include "callback_queue.h"
#include "hal_string.h"
#include "hal_logger.h"
#include "hal_ui_binding.h"

#define EHSL_MODULE_ID EHSH_LOG_MODULE_GRAPHICS

typedef struct inx_ui_data_real_state
{
    EhsCallbackQueueEntryType xChangedCbEntry;
    EhsCallbackQueueType changed_cb_queue;
    ehs_float  value_out;
    EhsUI_subscription_handle ui_handle;
    ehs_char Object[EHS_STRING_LENGTH_MAX];
    ehs_char Property[EHS_STRING_LENGTH_MAX];
} inx_ui_data_real_state_type;

EHS_FB_FUNCTIONS_START(ui_data_real)
EHS_FB_FUNCTION_ENTRY("set", 0x01, ui_data_real_set)
EHS_FB_FUNCTION_ENTRY("changed_cb", 0x02, ui_data_real_changed_cb)
EHS_FB_FUNCTIONS_END

#define INX_ui_data_real_ARG_set_value                   1
#define INX_ui_data_real_ARG_set_done                    1
#define INX_ui_data_real_ARG_changed_cb_changed                 1
#define INX_ui_data_real_ARG_changed_cb_value_out               1

EHS_FB_IDENTIFY_FUNCTION(ui_data_real)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_ui_data_real_state_type);
}

EHS_FB_INIT_FUNCTION(ui_data_real)
{
    inx_ui_data_real_state_type *state = (inx_ui_data_real_state_type *)EHS_FB_INIT_CONTEXT;
    const ehs_char *pParams = EHS_FB_INIT_PARAMETERS;
    (void)state; (void)pParams;

    pParams = EhsGetWordFromString(state->Object, pParams, sizeof(state->Object));
    pParams = EhsGetWordFromString(state->Property, pParams, sizeof(state->Property));

    /* FB state comes from EhsHMem_Alloc and is not zeroed — the queue head
     * must be NULL before the first register or the list runs into garbage. */
    state->changed_cb_queue = NULL;
    EhsCallbackQueue_register(&state->changed_cb_queue,
                              EHS_FB_RUN_NAME(ui_data_real_changed_cb),
                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1),
                              &(state->xChangedCbEntry));
    state->ui_handle = EhsUI_register_property_changed_real(state->Object, state->Property,
                                                            &state->changed_cb_queue,
                                                            &state->value_out);
    return EHS_TRUE;
}

EHS_FB_DESTROY_FUNCTION(ui_data_real)
{
    inx_ui_data_real_state_type *state = (inx_ui_data_real_state_type *)EHS_FB_DESTROY_CONTEXT;
    EhsUI_unregister(state->ui_handle);
    state->ui_handle = EHS_UI_HANDLE_INVALID;
    EhsCallbackQueue_clear(&state->changed_cb_queue);
    return EHS_TRUE;
}

EHS_FB_RUN_FUNCTION(ui_data_real_set)
{
    inx_ui_data_real_state_type *state = (inx_ui_data_real_state_type *)EHS_FB_RUN_CONTEXT;
    (void)state;
    ehs_float value = EHS_FB_IN_F_API2(INX_ui_data_real_ARG_set_value);
    EhsUI_set_property_real(state->Object, state->Property, value);
    EHS_FB_FINISH(INX_ui_data_real_ARG_set_done);
}
EHS_FB_RUN_FUNCTION(ui_data_real_changed_cb)
{
    inx_ui_data_real_state_type *state = (inx_ui_data_real_state_type *)EHS_FB_RUN_CONTEXT;
    if (EHS_FB_OUT_CONNECTED_API2(INX_ui_data_real_ARG_changed_cb_value_out))
        EHS_FB_OUT_F_API2(INX_ui_data_real_ARG_changed_cb_value_out) = state->value_out;
    EHS_FB_FINISH(INX_ui_data_real_ARG_changed_cb_changed);
}
