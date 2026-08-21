# Callback FB-instance lookup by function name/ID — design sketch

> **CDF port argument numbers:** canonical spec in `../ert-porting-guide.md` § "Port Argument Numbers" — each port type has its own independent 1..N range; `argument="0"` is the trigger.

Status: design only, not yet implemented. Kernel + KAPI changes scoped here so
the size of the patch is visible before committing the time. Migration is
opt-in per FB; existing FBs keep working unchanged.

## Why

Callbacks today are addressed by **positional slot index** in
`pCallbackTable[]`, where the slot order is the order Lucid emits
`<InternalPort>` elements from `<Ports>` in the CDF
(`inxware-gui-builder-mfc/LucidApplicationBuilder/SODL.cpp:321`). The FB-init
code (`inx-lorawan.c`, `inx-ota.c:85`, `inx-wifi_station.c:842`, …) hardcodes
`EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1 - n)` against an enum it owns. Any
disagreement between the CDF's `<InternalPort>` order and the FB's enum order
silently dispatches each cb on the wrong FB instance — the LoRaWAN
LoadProhibited at `EXCVADDR=0x18` was exactly this.

Replacement: the FB asks for an instance **by function ID (ERT1) or name
(ERT0)** — the identifier that already appears in both the CDF
(`<Function_ERT1_ID>`) and the FB code (`EHS_FB_FUNCTION_ENTRY("name", id,
…)`). Lookup runs once at FB-init; runtime event dispatch is unchanged.

## Constraints satisfied

- **No persistent memory cost on `EhsFunctionInstanceDataType`.** Struct shape unchanged.
- **No persistent memory cost on FBs that have callbacks.** The new ID/name index is parser-temporary; freed when `EhsKP_parse` returns.
- **No memory cost on FBs without callbacks.** Same as today.
- **FBs that hardwire (single-cb pattern) keep working unchanged.** The new macro is opt-in; the existing positional macro is untouched.
- **Both ERT0 and ERT1 supported.** Single FB call site; `#if ERT_SODL_VERSION` chooses the right helper under the macro.

## Scope of change

Five files, ~150 LoC total.

### 1. `EHS-kernel/Common/Kernel/parse_sodl.h`

Add a transient parallel index alongside `EhsParsedSodlData`. Stays scoped
to the parser temp-alloc lifecycle (`EhsHMem_tempAlloc` at
`parser.c:1881` / `EhsHMem_tempFree` at `parser.c:635` — already paired).

```c
/* New global, paired with EhsParsedSodlData. One entry per callback in the
 * app — sized at parse time. Indexed by the same nCallBackFuncIndex as
 * EhsCallbackInstanceDataTable.pFunctions[], so they live in lockstep.
 *
 * ERT1: 1 byte/callback. ERT0: 1 pointer/callback into the FB's xFunctions
 * table (the name string lives in flash; we only carry the pointer). */
#if ERT_SODL_VERSION > 0
EHS_GLOBAL ehs_uint8 *EhsParsedCallbackIds;
#else
EHS_GLOBAL const ehs_char **EhsParsedCallbackNames;
#endif
```

### 2. `EHS-kernel/Common/Kernel/parser.c`

Allocate at parse start, free at parse end — same pattern as
`EhsParsedSodlData` already follows.

```c
/* Around line 1881 — after dataQty.nFunctionBlocks is known and the existing
 * EhsParsedSodlData alloc. We size by total callbacks across the whole app;
 * dataQty already counts the SODL function records with nFuncTrigID == -1. */
ehs_uint16 nTotalCallbacks = dataQty.nCallbackFunctions; /* new field — see below */
#if ERT_SODL_VERSION > 0
EhsParsedCallbackIds   = (ehs_uint8 *)EhsHMem_tempAlloc(nTotalCallbacks);
#else
EhsParsedCallbackNames = (const ehs_char **)EhsHMem_tempAlloc(
                              sizeof(const ehs_char *) * nTotalCallbacks);
#endif

/* Around line 635 — alongside the existing EhsParsedSodlData free. */
#if ERT_SODL_VERSION > 0
if (EhsParsedCallbackIds) {
    EhsHMem_tempFree(EhsParsedCallbackIds);
    EhsParsedCallbackIds = NULL;
}
#else
if (EhsParsedCallbackNames) {
    EhsHMem_tempFree(EhsParsedCallbackNames);
    EhsParsedCallbackNames = NULL;
}
#endif
```

`dataQty.nCallbackFunctions` is added to the existing pre-pass that
populates `dataQty` (`parser.c:1850` area — already counts function blocks
and total functions; one extra `if (nFuncTrigID == -1) nCallbackFunctions++`).

### 3. `EHS-kernel/Common/Kernel/parse_sodl.c`

Capture the ID/name when the callback slot is allocated. The existing site
at line 1062-1063:

```c
pFunctionInstance = &EhsCallbackInstanceDataTable.pFunctions[
                          EhsCallbackInstanceDataTable.nSize];
EhsCallbackInstanceDataTable.nSize++;
```

becomes:

```c
const ehs_uint16 cbSlot = EhsCallbackInstanceDataTable.nSize;
pFunctionInstance = &EhsCallbackInstanceDataTable.pFunctions[cbSlot];
EhsCallbackInstanceDataTable.nSize++;

#if ERT_SODL_VERSION > 0
EhsParsedCallbackIds[cbSlot] = pFuncBlockSODL->pFuncRef[nSODLFunc]->szName;
#else
EhsParsedCallbackNames[cbSlot] =
    pFuncBlockSODL->pFuncRef[nSODLFunc]->szName;
#endif
```

`pFuncRef[nSODLFunc]` is already populated upstream by
`EhsBlockRef_findFunction` — so the byte/pointer we store is exactly what
the FB's `EHS_FB_FUNCTION_ENTRY(...)` registered.

### 4. `EHS-kernel/Common/Kernel/initialise.c`

`InitialiseObj` (line 27/29) currently takes `nCallBackFuncIndex` — the
absolute index into `EhsCallbackInstanceDataTable.pFunctions[]` of the
first callback in this FB's slice. Pass that through to the FB-init
function unchanged; the new lookup helper just uses it as the slice base.

The FB-init signature (`EhsInitFuncType` in `KAPI/ehs_fb_types.h:55`)
doesn't change — `pCallbackTable` is already the slice base. The lookup
helper derives the slice index from the pointer arithmetic
`pCallbackTable - EhsCallbackInstanceDataTable.pFunctions` so it can index
into the parallel ID/name array.

### 5. `EHS-kernel/Common/Kernel/callback_lookup.c` (new file, ~50 LoC)

```c
#include "fid.h"
#include "app_data.h"
#include "blockref_table.h"   /* EhsParsedCallbackIds / EhsParsedCallbackNames */
#include "hal_logger.h"

#if ERT_SODL_VERSION > 0
EhsFunctionInstanceDataType *
EhsCallback_findByFunctionId(EhsFunctionInstanceDataType *pCallbackTable,
                             ehs_uint16 nCallbacksInBlock,
                             ehs_uint8 cFunctionId)
{
    /* Slice base = pCallbackTable as a global table offset. Pointer subtraction
     * over EhsCallbackInstanceDataTable.pFunctions yields the absolute index
     * the parser used to populate EhsParsedCallbackIds[]. */
    const ehs_uint32 base = (ehs_uint32)(pCallbackTable -
                              EhsCallbackInstanceDataTable.pFunctions);
    for (ehs_uint16 i = 0; i < nCallbacksInBlock; i++) {
        if (EhsParsedCallbackIds[base + i] == cFunctionId) {
            return &pCallbackTable[i];
        }
    }
    /* Not found — caller is asking for a function the SODL didn't materialise
     * a callback for. The FB-init macro turns this into EHS_TRUSTLESS_NULL_FATAL. */
    return NULL;
}
#else
EhsFunctionInstanceDataType *
EhsCallback_findByFunctionName(EhsFunctionInstanceDataType *pCallbackTable,
                               ehs_uint16 nCallbacksInBlock,
                               const ehs_char *szName)
{
    const ehs_uint32 base = (ehs_uint32)(pCallbackTable -
                              EhsCallbackInstanceDataTable.pFunctions);
    for (ehs_uint16 i = 0; i < nCallbacksInBlock; i++) {
        if (0 == EhsStricmp(szName, EhsParsedCallbackNames[base + i])) {
            return &pCallbackTable[i];
        }
    }
    return NULL;
}
#endif
```

### 6. `Common/KAPI/ehs_fb_types.h`

```c
/* New macro — call site is identical on ERT0 and ERT1 */
#if ERT_SODL_VERSION > 0
EhsFunctionInstanceDataType *
EhsCallback_findByFunctionId(EhsFunctionInstanceDataType *pCallbackTable,
                             ehs_uint16 nCallbacksInBlock,
                             ehs_uint8 cFunctionId);
#define EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE_BY_FNID(fnId, n_in_block)       \
    EhsCallback_findByFunctionId(pCallbackTable, (n_in_block), (fnId))
#else
EhsFunctionInstanceDataType *
EhsCallback_findByFunctionName(EhsFunctionInstanceDataType *pCallbackTable,
                               ehs_uint16 nCallbacksInBlock,
                               const ehs_char *szName);
#define EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE_BY_FNID(fnIdMacro, n_in_block)  \
    EhsCallback_findByFunctionName(pCallbackTable, (n_in_block), \
                                   _EHS_FB_FNNAME_FROM_ID(fnIdMacro))
#endif

/* Existing positional macro unchanged — preserved for FBs that hardwire */
#define EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(x) (&(pCallbackTable[-1-(x)]))
```

### 7. ICB header generation

For each FB, the ICB tooling (which already emits `INX_<fb>_ARG_<func>_<port>`
and the function-ID enums) gains:

```c
#define INX_lorawan_FNID_connect           0x01
#define INX_lorawan_FNID_send_msg          0x03
#define INX_lorawan_FNID_send_msg_cb       0x07
/* ...one per <Function><ERT1_ID> in the CDF */

#define INX_lorawan_NUM_CALLBACKS          11   /* count of cb functions */
```

ERT0 build also emits string-name macros so the ID-form macro can resolve to
a name at compile time (purely a token-paste convenience, no runtime cost).

### 8. Migration: `inx-lorawan.c`

```c
EhsCallbackQueue_register(
    &xLorawanApiCallbackQueue[E_LORAWAN_API_SEND_MSG],
    EHS_FB_RUN_NAME(lorawan_send_msg_cb),
    EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE_BY_FNID(
        INX_lorawan_FNID_send_msg_cb, INX_lorawan_NUM_CALLBACKS),
    &(inx_lorawan_state->xEntry[E_LORAWAN_API_SEND_MSG])
);
```

The CDF's `<InternalPort>` order becomes a true don't-care after this. The
LoRaWAN-specific reorder we did earlier remains useful (consistency with
e_ehs_lorawan_api_cmd_t makes the file easier to read) but is no longer
load-bearing.

### 9. Migration: `inx-ota.c`, `inx-wifi_station.c`

These have one cb each, so the existing positional `(-1)` macro keeps
working without change. Optionally migrate to the new macro for symmetry —
no functional difference for single-cb FBs.

## Validation

```c
/* In inx-lorawan.c after each EhsCallbackQueue_register call: */
EHS_TRUSTLESS_NULL_FATAL(inx_lorawan_state->xEntry[E_LORAWAN_API_SEND_MSG].pFuncInst,
    "lorawan: send_msg_cb function instance not found at FB-init");
```

Or wrap inside the macro itself — fail at FB-init naming the missing
function. Crashes cleanly via `EhsAppDenyCurrentApp` + `EhsHSysReboot`
before any event runs.

## Cost summary

| | ERT0 | ERT1 |
|---|---|---|
| Persistent memory | unchanged | unchanged |
| Transient memory during parse | `4 bytes × total_callbacks` | `1 byte × total_callbacks` |
| Code size | ~30 LoC for `EhsCallback_findByFunctionName` | ~30 LoC for `EhsCallback_findByFunctionId` |
| FB-init time | one O(N_cb_in_block) `strcmp` per registered cb | one O(N_cb_in_block) byte compare per registered cb |
| Runtime event dispatch | unchanged | unchanged |
| Migration cost | per-FB, opt-in, can stage | per-FB, opt-in, can stage |

For lorawan (11 callbacks) on ERT1: 11 bytes of transient parse-time scratch,
freed when `EhsKP_parse` returns. Negligible.

## Out of scope

- No CDF schema change. `<SlotIndex>` is **not** added — it would just move
  the implicit contract one layer further out and require Lucid to enforce.
- No Lucid change.
- No SODL format change.

## Companion CI check

A separate static checker is added in
`scripts/inxware-id-tool/check_cdf_internalport_consistency.py` to catch
mismatches between an FB's CDF InternalPort references and its C-side
`EHS_FB_FUNCTION_ENTRY` table. That doesn't depend on this kernel change
landing — it's useful immediately and stays useful afterwards as a
defence-in-depth check.
