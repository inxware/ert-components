#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-ui_event.h"
#include "callback_queue.h"
#include "hal_string.h"
#include "hal_logger.h"
#include "hal_ui_binding.h"

#define EHSL_MODULE_ID EHSH_LOG_MODULE_GRAPHICS

typedef struct inx_ui_event_state
{
    EhsCallbackQueueEntryType xReceivedCbEntry;
    EhsCallbackQueueType received_cb_queue;
    EhsUI_subscription_handle ui_handle;
    ehs_char Object[EHS_STRING_LENGTH_MAX];
    ehs_char Signal[EHS_STRING_LENGTH_MAX];
} inx_ui_event_state_type;

EHS_FB_FUNCTIONS_START(ui_event)
EHS_FB_FUNCTION_ENTRY("emit", 0x01, ui_event_emit)
EHS_FB_FUNCTION_ENTRY("received_cb", 0x02, ui_event_received_cb)
EHS_FB_FUNCTIONS_END

#define INX_ui_event_ARG_emit_done                    1
#define INX_ui_event_ARG_received_cb_received                1

EHS_FB_IDENTIFY_FUNCTION(ui_event)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_ui_event_state_type);
}

EHS_FB_INIT_FUNCTION(ui_event)
{
    inx_ui_event_state_type *state = (inx_ui_event_state_type *)EHS_FB_INIT_CONTEXT;
    const ehs_char *pParams = EHS_FB_INIT_PARAMETERS;
    (void)state; (void)pParams;

    pParams = EhsGetWordFromString(state->Object, pParams, sizeof(state->Object));
    pParams = EhsGetWordFromString(state->Signal, pParams, sizeof(state->Signal));

    /* FB state comes from EhsHMem_Alloc and is not zeroed — the queue head
     * must be NULL before the first register or the list runs into garbage. */
    state->received_cb_queue = NULL;
    EhsCallbackQueue_register(&state->received_cb_queue,
                              EHS_FB_RUN_NAME(ui_event_received_cb),
                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1),
                              &(state->xReceivedCbEntry));
    state->ui_handle = EhsUI_register_signal(state->Object, state->Signal,
                                             &state->received_cb_queue);
    return EHS_TRUE;
}

EHS_FB_DESTROY_FUNCTION(ui_event)
{
    inx_ui_event_state_type *state = (inx_ui_event_state_type *)EHS_FB_DESTROY_CONTEXT;
    EhsUI_unregister(state->ui_handle);
    state->ui_handle = EHS_UI_HANDLE_INVALID;
    EhsCallbackQueue_clear(&state->received_cb_queue);
    return EHS_TRUE;
}

EHS_FB_RUN_FUNCTION(ui_event_emit)
{
    inx_ui_event_state_type *state = (inx_ui_event_state_type *)EHS_FB_RUN_CONTEXT;
    EhsUI_emit_signal(state->Object, state->Signal);
    EHS_FB_FINISH(INX_ui_event_ARG_emit_done);
}
EHS_FB_RUN_FUNCTION(ui_event_received_cb)
{
    EHS_FB_FINISH(INX_ui_event_ARG_received_cb_received);
}
