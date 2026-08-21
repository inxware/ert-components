# Function Block Authoring Guide

Reference for creating and modifying eRT function blocks (CDF + C implementation).

> **See also:** [`CLAUDE-general.md`](CLAUDE-general.md) for the full CDF XML schema reference (port types, data types, key elements), component file structure, `EHS_FB_*` macro reference, and the component creation workflow.

---

## Use the skeleton generator — don't hand-author blocks

**`scripts/software-utilities/new_cdf_skeleton.py`** turns a small JSON spec into a validator-clean CDF **and** the matching `inx-<class>.c` / `inx-<class>.h`. It auto-computes geometry, `NameHash_CRC16`, port wiring, InternalPort pairing, ERT1_IDs, friendly-label macros, parameter parsing, callback-queue registration, and HAL-contract extern declarations. The sections below document the rules the script encodes — read them when you need to understand or extend the output, not when authoring routine blocks.

```bash
# CDF only (writes to stdout if --out omitted):
python3 scripts/software-utilities/new_cdf_skeleton.py \
    --spec specs/my_block.json \
    --out  Common/Components/<cat>/my_block.cdf

# All three files at once (drops cdf, inx-<class>.c, inx-<class>.h into one dir):
python3 scripts/software-utilities/new_cdf_skeleton.py \
    --spec    specs/my_block.json \
    --out-dir Common/Components/<cat>/

# Print a fully-annotated example spec:
python3 scripts/software-utilities/new_cdf_skeleton.py --print-example
```

A minimal spec is shown in [§ Canonical CDF skeleton](#canonical-cdf-skeleton). Specs should be committed under `Common/Components/<cat>/_specs/<class>.json` so a future maintainer can regenerate the block by editing the spec rather than the generated output.

Hand-edit the generated output only for things the spec doesn't cover (rich logging that references state, real HAL calls, custom non-mechanical behaviour). The four CDF validators must still be run after any hand edit:

```bash
python3 scripts/software-utilities/cdf_geometry.py            validate <cdf>
python3 scripts/inxware-id-tool/check_cdf_hashes.py                    <dir>
python3 scripts/inxware-id-tool/check_cdf_function_args.py             <cdf>
python3 scripts/inxware-id-tool/check_cdf_internalport_consistency.py  <cdf>
```

---

## Canonical CDF skeleton

Every CDF has the same top-level structure. Element order matters for some downstream tools — keep this order.

```xml
<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>YYYY-MM-DDTHH:MM:SSZ</CreationDate>
        <UpdatedDate>YYYY-MM-DDTHH:MM:SSZ</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>One-line summary shown in the IDE</ShortDescription>
        <LongDescription>Multi-sentence description.</LongDescription>
        <UserName/>
        <Menu>Top
            <Menu>Mid
                <Menu>Leaf</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>                <!-- see § Block Type -->
        <Width>65</Width>              <!-- from cdf_geometry.py suggest -->
        <Height>73</Height>
        <Text>Display Label</Text>
        <TextX>10</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>my_block</Class>        <!-- must equal CDF filename basename -->
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x1234</NameHash_CRC16>   <!-- from inxtool.py -->
        <FbApiDescriptorHash_CRC32>00000000</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>...</Parameter>     <!-- or self-close <Parameters/> if none -->
    </Parameters>
    <Functions>
        <Function>
            <name>foo</name>           <!-- lowercase, matches C run-func suffix -->
            <ID><ERT1_ID>1</ERT1_ID></ID>
        </Function>
    </Functions>
    <Ports>
        <Port>...</Port>
    </Ports>
</Component>
```

Filename, `<Class>`, docs subdirectory name, and the C source class prefix must all be identical (see [§ Function Block Help Files](#function-block-help-files)).

### Minimal JSON spec for `new_cdf_skeleton.py`

```json
{
    "class": "my_block",
    "category": "peripherals",
    "label": "My Block",
    "short_description": "Brief summary",
    "long_description": "Full description sentence.",
    "menu": ["Peripherals", "Custom", "My Block"],
    "block_type": "IO",
    "parameters": [
        {"name": "Object",   "type": "ST", "default": "root"},
        {"name": "Property", "type": "ST", "default": "value"}
    ],
    "functions": [
        {
            "name": "set",
            "ports": [
                {"role": "start",  "cname": "set"},
                {"role": "input",  "cname": "value", "dtype": "I"},
                {"role": "finish", "cname": "done"}
            ]
        },
        {
            "name": "changed_cb",
            "internal": true,
            "ports": [
                {"role": "finish", "cname": "changed"},
                {"role": "output", "cname": "value_out", "dtype": "I"}
            ]
        }
    ]
}
```

Everything else — geometry, ERT1_IDs, NameHash, port X/Y coordinates, InternalPort pairing, `<MandatoryFlag>` on the first StartPort — is computed by the script.

---

## DataType reference

The CDF schema (`Common/Components/cdf-schema.xsd`) is incomplete — it lists only `I/R/B/S`, but the codebase uses more. **Use the values in the tables below; the schema is the wrong source of truth.**

### Port DataType (`<Port><DataType>`)

| Value | Meaning | Tree-wide uses | Notes |
|-------|---------|----------------|-------|
| `I` | Integer (signed 32-bit) | 730 | Most common |
| `S` | String | 518 | NUL-terminated `char*` buffer |
| `F` | Float | 264 | **Use `F` — not `R`** despite the schema's claim |
| `B` | Boolean | 251 | `ehs_bool` |
| `R` | (legacy float alias) | 4 | **Do not use in new CDFs** — replace with `F` |
| `U` | Unsigned | 2 | Rare; reserved for unsigned-integer ports |

StartPort, FinishPort, and InternalPort do **not** have a `<DataType>` element — they are pure event ports.

### Parameter DataType (`<Parameter><DataType>`)

| Value | Meaning | Tree-wide uses | When to use |
|-------|---------|----------------|-------------|
| `I` | Integer | 218 | Numeric counts, GPIO indices, timeouts |
| `S` | String (generic) | 188 | Free-form text, paths, IPs (multi-line OK) |
| `B` | Boolean | 100 | 0/1 flags, often with `MinValue=0 MaxValue=1` |
| `F` | Float | 29 | Calibration constants, thresholds |
| `ST` | Short Text | 33 | Short identifier-like strings (object names, group names, signal names) — IDE renders inline |
| `WT` | Widget Tag | 23 | **Only** for blocks that create a widget — Lucid uses this to populate the widget-tag table. Do not use for blocks that merely reference a widget. |

`MinValue` / `MaxValue` are interpreted per type (numeric range for `I`/`F`/`B`; ignored or set to `0` for strings).

---

## Block Type

The `<Block><Type>` element classifies the block for the IDE's palette and for static analysis.

| Type | Tree-wide uses | When to use |
|------|----------------|-------------|
| `Data_Processor` | 189 | Pure data transforms with no IO and no events — e.g. arithmetic, type coercion, multiplexers |
| `IO` | 95 | Anything that talks to hardware, OS, network, UI, or fires events from external sources |
| `Data` | 42 | Constant blocks, value sources |
| `Event_Processor` | 12 | Event routing/gating that doesn't carry data |
| `And_Event` / `Or_Event` | 20 | Boolean combinators of event ports |
| `Sub_System` / `Sub_System_IO` | 11 | Encapsulation containers (`xfinish` etc.) |

For peripheral or UI blocks: use `IO`. For arithmetic / format-conversion / mux blocks: use `Data_Processor`.

---

## Functions naming and `<Functions>` ↔ C run-function pairing

The `<name>` element inside each `<Function>` must be a **lowercase descriptive name matching the C run-function name without the block-class prefix**. The kernel resolves callbacks by string lookup against `EHS_FB_FUNCTION_ENTRY` rows.

For a CDF `<Class>foo</Class>` with `<Function><name>read_status</name></Function>`:

| Artefact | Required value |
|----------|----------------|
| CDF `<Functions><Function><name>` | `read_status` |
| C `EHS_FB_FUNCTION_ENTRY` string | `"read_status"` |
| C run-function symbol | `EHS_FB_RUN_FUNCTION(foo_read_status)` |
| Friendly-label macros | `INX_foo_ARG_read_status_<port_cname>` |

Snake_case only. Never PascalCase. Never camelCase.

---

## InternalPort naming convention

InternalPorts are HAL-fired async callbacks (see [§ InternalPort async callback pattern](#internalport-async-callback-pattern)). Their `<name>` and `<CName>` follow a strict pattern so the cross-validators (`check_cdf_internalport_consistency.py`) can pair them with C-side `EHS_FB_FUNCTION_ENTRY` rows.

| Artefact | Convention | Example |
|----------|-----------|---------|
| `<Function><name>` | `<event>_cb` | `recv_cb`, `changed_cb` |
| InternalPort `<CName>` | `<event>_cb_start` (suffix `_start`) | `recv_cb_start`, `changed_cb_start` |
| InternalPort `<Description>` | Same as the `CName` (or a short phrase) | `changed_cb_start` |
| `<XCoordinate>` / `<YCoordinate>` | Both `-1` (hidden) | — |
| `<Function argument="0">` | Always `argument="0"` (it is the function's StartPort, just hidden) | — |

The InternalPort is the function's StartPort, just invisible — it's fired by `EhsCallbackQueue_execute` from HAL code rather than by the kernel from another FB's FinishPort.

---

## Block registration — choosing the right `EHS_BLOCKREF_ENTRY` macro

Every FB must be registered in the category's `*_components.c` table (e.g. `gui_components.c`). Three macro forms exist:

| Macro | Use when | Resolves to |
|-------|----------|-------------|
| `EHS_BLOCKREF_ENTRY(name_str, id, class)` | Default — block has its own `<Class>`-named `EHS_FB_INIT_FUNCTION` / `EHS_FB_IDENTIFY_FUNCTION` and no destroy function | destroy=NULL; identify/init resolved by class name |
| `EHS_BLOCKREF_ENTRY_WITH_DESTROY(name_str, id, class)` | Block has a destroy function (`EHS_FB_DESTROY_FUNCTION(class)`) — needed for any block that allocates target resources (file handles, sockets, library widgets, etc.) | destroy=`EHS_FB_DESTROY_NAME(class)` |
| `EHS_BLOCKREF_ENTRY_EX(name_str, id, class, init, identify)` | Block shares another class's identify/init functions (e.g. `gui_text_int2` reuses `gui_widget` — four FBs, one shared IDENTIFY/INIT) | Explicit identify/init function names |

There are also `*_EX_WITH_DESTROY` and `*_API2` variants — only use those if you have an explicit reason. The vast majority of new blocks use `EHS_BLOCKREF_ENTRY` (222 of 371 tree-wide).

`name_str` is the same string the FB's `<Class>` element holds — use the `EHS_FB_NAME_<class>` macro (declared in the FB's header alongside `EHS_FB_ID_<class>` / `INXWARE_FB_ID_<class>`).

---

## `MandatoryFlag` on StartPorts

`<MandatoryFlag>1</MandatoryFlag>` on a StartPort marks it as required-to-be-wired in the IDE — Lucid will flag an unwired mandatory StartPort as a design error. Conventions in the tree:

- Only the **primary trigger** StartPort of a block gets `MandatoryFlag` (the first function's StartPort — `create` for widgets, `start` for peripherals).
- Optional / secondary StartPorts (close, hide, send, etc.) **never** carry `MandatoryFlag`.
- Most blocks have **no** `MandatoryFlag` at all (118 occurrences across ~370 blocks).

For new blocks: omit `MandatoryFlag` unless there is exactly one entry-point function and leaving it unwired would brick the block at runtime.

---

## `.idf.ini` files — legacy, do not author

`.idf.ini` is the pre-CDF format used by older Lucid versions. Of ~370 blocks, 238 have both a `.cdf` and a matching `.idf.ini`; 134 are CDF-only. **Do not create new `.idf.ini` files** — the CDF is the canonical format and nothing in the build reads `.idf.ini` for blocks that have a CDF. Leave existing `.idf.ini` files in place; do not edit them when changing the CDF.

---

## C file organisation — one file per class, unless they share state

Default: each FB class gets its own `inx-<class>.c` and `inx-<class>.h` pair (e.g. `inx-uart.c`/`inx-uart.h`, `inx-lorawan.c`/`inx-lorawan.h`).

**Exception** — when N FBs share a state struct and share their `EHS_FB_IDENTIFY_FUNCTION` / `EHS_FB_INIT_FUNCTION` (registered via `EHS_BLOCKREF_ENTRY_EX` pointing at the shared identify/init names), put them in one file. `inx-gui_widget.c` is the canonical example: `gui_text_int2`, `gui_text_real2`, `gui_text_bool2`, `gui_text_string2`, and `gui_widget` all share `EhsWidget` state and one identify/init pair.

Don't share a file just because the blocks happen to be in the same category — file boundaries follow shared-state / shared-init, not topical grouping.

---

## Canonical C skeleton

The C side has the same goal as the CDF side: **every choice is mechanical and lookup-able**. This section is the source of truth for the include list, the helper macros, and the `.h`/`.c` shape. The generator (`new_cdf_skeleton.py --out-dir DIR`) emits exactly this shape from a JSON spec.

### Required includes (and where the symbols live)

| Header | Provides | Notes |
|--------|----------|-------|
| `inx-parameters.h` | EHS parameter-parsing macros, `EhsParameter*` types | First include in every FB `.c` |
| `inx-component.h` | All `EHS_FB_*` KAPI macros, `EHS_FB_IDENTIFY/INIT/DESTROY/RUN_FUNCTION`, port read/write macros (`EHS_FB_IN_*_API2`, `EHS_FB_OUT_*_API2`, `EHS_FB_FINISH`). Pulls in `globals.h` (which provides `EHS_STRING_LENGTH_MAX`, `ehs_sint32`, `ehs_bool`, `ehs_float`, `ehs_char`). | Second include — every FB needs this |
| `inx-<class>.h` | This FB's own declarations | Third include |
| `callback_queue.h` | `EhsCallbackQueueType`, `EhsCallbackQueueEntryType`, `EhsCallbackQueue_register`, `EhsCallbackQueue_execute` | Required when the FB has any InternalPort callbacks |
| `hal_string.h` | `EhsGetWordFromString`, `EhsGetSint32FromString`, `EhsGetReal32FromString`, string helpers. `EhsStrncpy` (no `_s`) is a per-target macro from `target/os-arch/*/target_string.h` — pulled in via this header on every target. | Required when parsing params or copying strings |
| `hal_logger.h` | `EHSH_LOG_INFO/WARNING/ERROR/ENTER/EXIT` and the `EHSH_LOG_MODULE_*` enum | Required when logging — and you almost always want to log |

### Log module IDs

Defined in `Common/HAL/include/hal_logger.h`. Pick the one closest to your subsystem; `_UNDEFINED` is the catch-all default for category-less code, but for an FB you should declare a real module:

```
EHSH_LOG_MODULE_UNDEFINED       — default, only use if nothing else fits
EHSH_LOG_MODULE_KERNEL
EHSH_LOG_MODULE_GRAPHICS        — all GUI / UI / widget blocks
EHSH_LOG_MODULE_LOGGER
EHSH_LOG_MODULE_HAL_MEMORY
EHSH_LOG_MODULE_HAL_PROCESS
EHSH_LOG_MODULE_HAL_STRING
EHSH_LOG_MODULE_TGT_VIEWPORT
EHSH_LOG_MODULE_HAL_NETWORK     — networking, MQTT, modbus, etc.
EHSH_LOG_MODULE_HAL_DEVMANMON
EHSH_LOG_MODULE_HAL_FILE
```

Set it via `#define EHSL_MODULE_ID EHSH_LOG_MODULE_<X>` immediately **after** the `hal_logger.h` include and before any `EHSH_LOG_*` call site. The macro must be a literal define — not a function, not an expression.

If you need a module ID that doesn't yet exist, add it to the enum in `hal_logger.h` and update this table.

### `hal_ui_binding.h` — UI bridge for the `ui_data_*` / `ui_event` FBs

The 5 UI binding blocks (`ui_data_int`, `ui_data_real`, `ui_data_bool`, `ui_data_string`, `ui_event`) are platform-independent in `Common/Components/gui/` and call a target-supplied HAL declared in **`Common/HAL/include/hal_ui_binding.h`**. The Qt implementation is `target/Component-HAL/graphics/qt/hal_ui_binding_qt.c`, built when a Qt graphics target is active.

| FB | INIT calls (subscribe) | RUN calls (push) |
|----|------------------------|------------------|
| `ui_data_int`    | `EhsUI_register_property_changed_int(obj, prop, &queue, &g_value)`    | `EhsUI_set_property_int(obj, prop, v)`    |
| `ui_data_real`   | `EhsUI_register_property_changed_real(obj, prop, &queue, &g_value)`   | `EhsUI_set_property_real(obj, prop, v)`   |
| `ui_data_bool`   | `EhsUI_register_property_changed_bool(obj, prop, &queue, &g_value)`   | `EhsUI_set_property_bool(obj, prop, v)`   |
| `ui_data_string` | `EhsUI_register_property_changed_string(obj, prop, &queue, g_buffer)` | `EhsUI_set_property_string(obj, prop, s)` |
| `ui_event`       | `EhsUI_register_signal(obj, sig, &queue)`                             | `EhsUI_emit_signal(obj, sig)`             |

The build is gated on `EHS_UI_BINDING_SUPPORT` (set by `target/Component-HAL/graphics/qt/graphics.mk`). Other GUI targets (GTK Mode A, LVGL Mode B) can opt in by providing their own `hal_ui_binding_*.c` and defining `EHS_UI_BINDING_SUPPORT` in their graphics.mk — the FBs themselves don't change.

**Generic `ertqt_*` helpers (Qt only):** `ertqt_bind_signal(h, "any_signal", cb, ud)` and `ertqt_invoke_signal(h, "any_signal")` were added alongside the typed binders (`ertqt_bind_clicked`, `ertqt_bind_value_changed`, etc.). Use the generic form when the signal name comes from runtime data (e.g. a parameter); use the typed binders when you know the signal at compile time.

### `EHS_TARGET_FP_SUPPORT` — guard float/real blocks

Targets that don't have float ABI support (some Cortex-M0 variants, certain MCU configs) define `EHS_TARGET_FP_SUPPORT` to `0` or leave it undefined. **Any FB that exposes `<DataType>F</DataType>` ports or uses `ehs_float` in its state must be guarded** so it isn't built on those targets.

The guard goes in two places — never in the `.c`/`.h` of the block itself:

```makefile
# In Common/Components/<cat>/components.mk
ifdef EHS_TARGET_FP_SUPPORT
OBJECTS += inx-my_float_block.$(OBJ)
endif
```

```c
// In the category's *_components.c block-reference table
#ifdef EHS_TARGET_FP_SUPPORT
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_my_float_block,
                                    INXWARE_FB_ID_my_float_block,
                                    my_float_block),
#endif
```

The block's own `.c`/`.h` is *not* `#ifdef`'d — the file simply isn't compiled or registered on non-FP targets. That keeps the source clean.

### Canonical `inx-<class>.h` template

```c
#ifndef INXWARE_<class>
#define INXWARE_<class>
#include "inx-component.h"

/* One-line description of what this block does.
 *   <function1>  — what it triggers
 *   <function2>  — what it triggers
 */
#define INXWARE_FB_ID_<class>   0xXXXX     /* from inxtool.py -genHash */
#define INXWARE_FB_NAME_<class> "<class>"

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(<class>);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(<class>);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(<class>);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(<class>_<function1>);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(<class>_<function2>);

/* HAL contract (only when an InternalPort callback exists) — HAL writes to the
 * `g_*` globals then calls EhsCallbackQueue_execute on the matching queue to
 * dispatch the callback on the EHS thread. */
extern EhsCallbackQueueType x_<class>_<callback_fn>_queue;
extern <type> g_<class>_<callback_fn>_<output_cname>;   /* one per callback output */

EHS_FB_FUNCTIONS(<class>)

#endif /* INXWARE_<class> */
```

The trailing `EHS_FB_FUNCTIONS(<class>)` macro (no semicolon) declares the function-table reference symbol used by `EHS_FB_FUNCTIONS_START/END` in the `.c`.

### Canonical `inx-<class>.c` template

```c
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-<class>.h"
#include "callback_queue.h"          /* only if any internal function exists */
#include "hal_string.h"              /* for EhsGetWordFromString / EhsStrncpy */
#include "hal_logger.h"

#define EHSL_MODULE_ID EHSH_LOG_MODULE_<X>   /* see § Log module IDs */

/* ----------------------------------------------------------------------- */
/* Per-instance state                                                      */
/* One EhsCallbackQueueEntryType per internal-function callback.           */
typedef struct inx_<class>_state
{
    EhsCallbackQueueEntryType x<Callback1>Entry;     /* per internal function */
    ehs_char <Param1>[EHS_STRING_LENGTH_MAX];        /* string params */
    ehs_char <Param2>[EHS_STRING_LENGTH_MAX];
    ehs_sint32 <Param3>;                             /* int params */
} inx_<class>_state_type;

/* ----------------------------------------------------------------------- */
/* HAL contract — module-level globals.                                    */
/* The HAL writes to g_* then calls EhsCallbackQueue_execute on the queue. */
static EhsCallbackQueueType x_<class>_<callback_fn>_queue;
static <type>               g_<class>_<callback_fn>_<output_cname>;

/* ----------------------------------------------------------------------- */
/* Function table — must list every function in the CDF's <Functions>.    */
/* The string and the 8-bit ID must match the CDF's <name> / <ERT1_ID>.    */
EHS_FB_FUNCTIONS_START(<class>)
EHS_FB_FUNCTION_ENTRY("<function1>", 0x01, <class>_<function1>)
EHS_FB_FUNCTION_ENTRY("<function2>", 0x02, <class>_<function2>)
EHS_FB_FUNCTIONS_END

/* ----------------------------------------------------------------------- */
/* Friendly-label macros — one per (function, port). The value MUST equal  */
/* the argument= attribute on that <Port>'s <Function> in the CDF.         */
#define INX_<class>_ARG_<function1>_<port_cname>  1
#define INX_<class>_ARG_<function2>_<port_cname>  1
/* ... */

/* ----------------------------------------------------------------------- */
EHS_FB_IDENTIFY_FUNCTION(<class>)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_<class>_state_type);
}

EHS_FB_INIT_FUNCTION(<class>)
{
    inx_<class>_state_type *state = (inx_<class>_state_type *)EHS_FB_INIT_CONTEXT;
    const ehs_char *pParams = EHS_FB_INIT_PARAMETERS;

    /* Parse parameters in CDF <ArgPlacement> order. ALWAYS use EhsGet*; never
     * sscanf / strtol / atoi. The parser is position-based — chain the calls. */
    pParams = EhsGetWordFromString(state-><Param1>, pParams, sizeof(state-><Param1>));
    pParams = EhsGetWordFromString(state-><Param2>, pParams, sizeof(state-><Param2>));
    pParams = EhsGetSint32FromString(&state-><Param3>, pParams);

    /* Register one callback per internal function. The -1, -2, ... indexes
     * the InternalPorts in CDF document order. */
    EhsCallbackQueue_register(&x_<class>_<callback_fn>_queue,
                              EHS_FB_RUN_NAME(<class>_<callback_fn>),
                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1),
                              &(state->x<Callback1>Entry));
    return EHS_TRUE;
}

EHS_FB_DESTROY_FUNCTION(<class>)
{
    /* Free anything allocated in INIT. The state struct itself is owned by
     * the kernel pool — do not free it. Return EHS_TRUE on clean shutdown. */
    return EHS_TRUE;
}

/* ----------------------------------------------------------------------- */
/* Non-internal function: read inputs → do work → fire FinishPort.         */
EHS_FB_RUN_FUNCTION(<class>_<function1>)
{
    inx_<class>_state_type *state = (inx_<class>_state_type *)EHS_FB_RUN_CONTEXT;
    ehs_sint32 value = EHS_FB_IN_I_API2(INX_<class>_ARG_<function1>_<input_cname>);
    /* HAL call goes here, e.g. EhsTarget_<X>_set(state-><Param1>, value); */
    EHS_FB_FINISH(INX_<class>_ARG_<function1>_<done_cname>);
}

/* Internal-function run: copy HAL-supplied globals → output ports → finish. */
EHS_FB_RUN_FUNCTION(<class>_<callback_fn>)
{
    if (EHS_FB_OUT_CONNECTED_API2(INX_<class>_ARG_<callback_fn>_<output_cname>))
        EHS_FB_OUT_I_API2(INX_<class>_ARG_<callback_fn>_<output_cname>)
            = g_<class>_<callback_fn>_<output_cname>;
    EHS_FB_FINISH(INX_<class>_ARG_<callback_fn>_<finish_cname>);
}
```

### Port read/write macro table

| dtype | Read input | Write output | C type | Notes |
|-------|-----------|--------------|--------|-------|
| `I` | `EHS_FB_IN_I_API2(arg)` | `EHS_FB_OUT_I_API2(arg) = v` | `ehs_sint32` | rvalue read; lvalue assign |
| `F` | `EHS_FB_IN_F_API2(arg)` | `EHS_FB_OUT_F_API2(arg) = v` | `ehs_float` | requires `EHS_TARGET_FP_SUPPORT` at build time |
| `B` | `EHS_FB_IN_B_API2(arg)` | `EHS_FB_OUT_B_API2(arg) = v` | `ehs_bool` | rvalue/lvalue |
| `S` | `EHS_FB_IN_S_API2(arg)` returns `const ehs_char*` | `EHS_FB_OUT_S_API2(arg)` returns `char*` buffer — write with `EhsStrncpy` | `ehs_char[EHS_STRING_LENGTH_MAX]` | string output is a pre-allocated buffer, not a pointer assignment |

Connectivity check before every write: `if (EHS_FB_OUT_CONNECTED_API2(arg)) { … }`. Optional but matches the convention.

### `EHS_FB_FINISH` vs `EHS_FB_FINISH_API2`

Both fire the FinishPort at the given argument number. They are aliases in current builds — use `EHS_FB_FINISH(INX_<class>_ARG_<function>_<finish_cname>)`. The friendly-label macro must equal the CDF `argument=` value.

There is no `EHS_FB_FINISH_PORT("name")` or string-based form.

---

## CDF Port Argument Number Rules

> **Canonical spec:** `docs/ert-porting-guide.md` § "Port Argument Numbers". The section below is the detailed reference; the porting guide is the single authoritative summary for cross-repo use.

Confirmed against `ml_image_inference.cdf` and `ble_service.cdf`. The `argument=` attribute on a port's `<Function>` element controls logical grouping and determines which API2 macro slot accesses the port.

### Hard rules

1. **argument=0** — always the StartPort (function trigger). One per function, no other ports share it.

2. **At most ONE `<PortType>OutputPort</PortType>` per argument number per function** (regardless of data type).
   - `EHS_FB_OUT_I_API2(n)` addresses the OutputPort/I at argument n.
   - `EHS_FB_OUT_S_API2(n)` addresses the OutputPort/S at argument n.
   - Two OutputPorts at the same argument number — even with different data types (I vs S) — is ambiguous and broken.

3. **FinishPorts must have unique argument numbers** per function. `EHS_FB_FINISH(n)` fires exactly the FinishPort at argument n.

4. **InputPorts and FinishPorts MAY share an argument number with one OutputPort** — they use distinct macros (`EHS_FB_IN_*` vs `EHS_FB_OUT_*` vs `EHS_FB_FINISH`) so there is no ambiguity.

5. **Writing to an OutputPort at arg=X before firing a FinishPort at arg=Y is valid** — the argument number does not control when writing occurs, only which API2 slot the port occupies.

### Established pattern (confirmed from `ml_image_inference.cdf`)

| argument | Typical contents |
|---|---|
| 0 | StartPort (trigger) |
| 1 | Success FinishPort + InputPort(s) + OutputPort/I (errno) |
| 2 | Error FinishPort + OutputPort/S (json/string result) |

The OutputPort/S string result always goes at argument=2 alongside the error FinishPort. This pattern is used by `ml_image_inference`, `tf_lite_frame`, and `tflm`.

### Friendly label macros

`#define INX_<FB>_ARG_<function>_<port> N` in the `.c` file must exactly match the `argument=` value in the CDF.

Example for a function with the standard pattern:
```c
#define INX_MyBlock_ARG_do_work_input_data    1  // InputPort/I at arg=1
#define INX_MyBlock_ARG_do_work_work_errno    1  // OutputPort/I at arg=1
#define INX_MyBlock_ARG_do_work_done          1  // FinishPort (success) at arg=1
#define INX_MyBlock_ARG_do_work_error         2  // FinishPort (error) at arg=2
#define INX_MyBlock_ARG_do_work_result        2  // OutputPort/S at arg=2
```

### Mandatory after-authoring check: `check_cdf_function_args.py`

**Whenever you generate or modify a CDF, run this validator on the file before considering the change complete.** It enforces the rules above by checking the in-file invariants the SODL writer silently relies on:

- Per `(function, port-type)` the `argument=` values form contiguous `1..N` with no holes and no duplicates. A hole produces a default-typed (`B`/`0`) entry in the SODL output stream; a duplicate causes one entry to overwrite the other under `SetAtGrow`. Both modes corrupt the binary silently and only show as runtime misbehaviour.
- Function 8-bit IDs (`<ID><ERT1_ID>`) are unique across distinct function names within the block.

```bash
# Single CDF
python3 scripts/inxware-id-tool/check_cdf_function_args.py path/to/block.cdf

# All CDFs in a category
python3 scripts/inxware-id-tool/check_cdf_function_args.py Common/Components/networking
```

Exits 0 if clean, 1 on any violation. Errors must be fixed before the CDF is considered complete — do not commit a CDF that this tool flags.

### Run all CDF validators when authoring or modifying a function block

There are four CI-grade CDF validators. Each catches a different failure mode the SODL parser/runtime cannot diagnose at load time. **Whenever you create a new FB or modify an existing one (CDF, `.c`, or both), run every one of them on the touched file before considering the change complete.** Together they prevent the runtime crashes that "just compile and flash" wouldn't catch — the LoRaWAN `LoadProhibited` at `EXCVADDR=0x18` was caused by exactly the kind of CDF↔FB drift these scripts detect.

| Script | Catches | When to run |
|---|---|---|
| `scripts/software-utilities/cdf_geometry.py validate` | Block geometry — Width/Height vs port Y range, X coordinates inside the border, title placement. Wrong geometry means clipped ports or text in Lucid. | Always — when you author/edit any CDF. |
| `scripts/inxware-id-tool/check_cdf_hashes.py` | Block-ID (`NameHash_CRC16`) uniqueness across the whole tree, and CDF↔header agreement (`INXWARE_FB_ID_<class>`). | Always — when you add an FB, rename a `<Class>`, or change a `NameHash_CRC16`. |
| `scripts/inxware-id-tool/check_cdf_function_args.py` | Per-CDF in-file invariants the SODL writer relies on — function 8-bit ID uniqueness within the block, and per `(function, port-type)` `argument=` contiguity. | Always — when you add/remove/edit a `<Port>` or `<Function>`. |
| `scripts/inxware-id-tool/check_cdf_internalport_consistency.py` | CDF↔C-code consistency for callback wiring — InternalPorts referencing functions that exist in `EHS_FB_FUNCTION_ENTRY`, no dead C functions, no duplicate InternalPort references, and no duplicate IDs in the FB's `EHS_FB_FUNCTION_ENTRY` table. | Always — when you add or remove a callback (`InternalPort`) or change an `EHS_FB_FUNCTION_ENTRY` line. |

```bash
# Validate one block end-to-end (CDF + .c) — the full set:
python3 scripts/software-utilities/cdf_geometry.py            validate Common/Components/<cat>/<block>.cdf
python3 scripts/inxware-id-tool/check_cdf_hashes.py                    Common/Components/<cat>/<block>.cdf
python3 scripts/inxware-id-tool/check_cdf_function_args.py             Common/Components/<cat>/<block>.cdf
python3 scripts/inxware-id-tool/check_cdf_internalport_consistency.py  Common/Components/<cat>/<block>.cdf

# Or validate a whole category (or the entire tree by omitting the path):
python3 scripts/software-utilities/cdf_geometry.py            validate Common/Components/<cat>/*.cdf
python3 scripts/inxware-id-tool/check_cdf_hashes.py                    Common/Components/<cat>
python3 scripts/inxware-id-tool/check_cdf_function_args.py             Common/Components/<cat>
python3 scripts/inxware-id-tool/check_cdf_internalport_consistency.py  Common/Components/<cat>
```

Each script exits 0 on clean, 1 on any violation. Treat any violation as a release-blocker — none of these are "lint-style" warnings; every diagnostic corresponds to a class of silent runtime corruption that has historically caused either crashes or wrong-port dispatch on shipped devices.

`SystemTests/CI/validate_all.sh` runs `check_cdf_hashes.py` automatically as a pre-flight before any platform build. Adding the other three to the same gate is the simplest way to prevent CDF authoring slips from reaching a build.

---

## CDF Block Geometry (Width, Height, port X coordinates)

These rules ensure the block outline fits all ports with no gaps or clipping, and the title label has room. Confirmed against `adc_read_single.cdf` (canonical reference) and corrected against `watchdog`, `ups`, `rs485_config`, and `led`.

### How LocationY affects the block rectangle

`<LocationY>-15</LocationY>` is standard for all IO blocks. It shifts the block rectangle so that:

- **Effective block top** = `LocationY` = **−15**
- **Effective block bottom** = `LocationY + Height` = **Height − 15**

This means Height is NOT simply the visual span of the ports. A block with `LocationY=-15` and `Height=60` has its rectangle from Y=−15 to Y=45.

**Missing `<LocationY>-15</LocationY>` causes the title to be clipped at the top** because the block rectangle starts at Y=0 and the title (TextY=5, text body extends slightly above that) is cut off.

### Height formula

```
Height = last_port_Y + 28
```

Derivation (from `adc_read_single.cdf`):
- `LocationY = -15`, so effective bottom = `Height − 15`
- Required bottom clearance below last port = **13 units** (confirmed from reference block)
- Therefore: `Height − 15 = last_port_Y + 13` → `Height = last_port_Y + 28`

| Block | Last port Y | Height |
|---|---|---|
| adc_read_single (reference) | 32 | 60 (32+28) |
| watchdog | 70 | 98 (70+28) |
| ups | 55 | 83 (55+28) |
| rs485_config | 45 | 73 (45+28) |
| led | 75 | 103 (75+28) |

**When adding a port**, increase Height by 10 if the new port's Y is 10 more than the previous last port (i.e. Height stays `last_port_Y + 28`).

### Port X coordinate rules

- **Right-side ports** (FinishPort, OutputPort) at `XCoordinate = Width − 5`. Example: Width=75 → X=70. Do **not** use X=Width — that places the connector outside the border.
- **Left-side ports** (StartPort, InputPort) at `XCoordinate = 0`.
- **Hidden ports** (InternalPort) at `XCoordinate = -1`, `YCoordinate = -1`.

### Title placement

- `<TextX>10</TextX>`, `<TextY>5</TextY>` — title at top-left. Works because `LocationY=-15` puts the block top 15 units above Y=0, well above the title body.
- **Do not use a large TextY** (e.g. TextY=70) — the title will overlap body ports.
- First port at `Y=12` gives 7 units of clearance below the title baseline.

### Block section template

```xml
<Block>
    <Type>IO</Type>
    <Width>75</Width>
    <Height><!-- last_port_Y + 28 --></Height>
    <Text>Block Label</Text>
    <TextX>10</TextX>
    <TextY>5</TextY>
    <TextScale>1.25</TextScale>
    <TextVertical>0</TextVertical>
    <LocationX>0</LocationX>
    <LocationY>-15</LocationY>
</Block>
```

### Port X coordinate summary

| Side | Port types | XCoordinate |
|---|---|---|
| Left | StartPort, InputPort | `0` |
| Right | FinishPort, OutputPort | `Width − 5` (e.g. `70` for Width=75) |
| Hidden | InternalPort | `-1` |

### Using `cdf_geometry.py` — mandatory geometry tool

**Always use `scripts/software-utilities/cdf_geometry.py` when authoring or modifying CDF files.** It implements the rules above so geometry is consistent and errors are caught before they reach the IDE.

#### Before writing the CDF — get geometry values

Run `suggest` with the planned port labels and the Y coordinate of the lowest port:

```bash
python3 scripts/software-utilities/cdf_geometry.py suggest \
    --labels "enable" "disable" "kick" "error id" "expired" \
    --last-y 70
```

Output is a ready-to-paste `<Block>` geometry snippet with Width, Height, TextX/Y, LocationX/Y, and the right-side port XCoordinate. Copy these values into the CDF.

#### After writing the CDF — validate

```bash
python3 scripts/software-utilities/cdf_geometry.py validate path/to/block.cdf
```

Exits 0 if OK, 1 if errors. Errors must be fixed before the CDF is considered complete. Warnings are advisory (e.g. width may be slightly tight for the label length).

#### Validating all CDFs in a category

```bash
python3 scripts/software-utilities/cdf_geometry.py validate Common/Components/peripherals/*.cdf
```

### Width tier selection

The `suggest` command picks the smallest standard tier that fits the longest label. Bump to the next tier for complex or protocol-heavy blocks:

| Tier | Fits labels up to | Typical use |
|---|---|---|
| 30 | 2 chars | Logic, operators, mux/demux (no meaningful labels) |
| 65 | 9 chars | Simple utilities, buffers, file ops |
| 75 | 11 chars | Standard peripheral blocks (ADC, GPIO, watchdog, UPS) |
| 95 | 14 chars | Config blocks, UART, NV storage |
| 105 | 16 chars | Complex blocks: MQTT, RTC, GUI widgets |
| 115 | 20+ chars | Protocol stacks: Modbus, BLE |

---

## CDF Port Y Coordinate Layout

Y coordinates control the visual position of ports on the block. The conventions below are confirmed from `tf_lite_frame.cdf` and `ml_image_inference.cdf`.

### Rules

- **StartPort (left) and success FinishPort (right) share the same Y** — they are the trigger/done pair and should appear horizontally aligned.
- **Ports within one function are spaced 10 units apart.**
- **Data inputs (InputPorts) cluster just below the StartPort** on the left side.
- **Data outputs for the success path cluster just below the success FinishPort** on the right side.
- **Error FinishPort comes below the success data**, with no extra gap within the function.
- **Data outputs for the error path follow the error FinishPort** immediately (10 units below).
- **Gap of ~15 units between the last port of one function and the StartPort of the next** — this creates the visual separation between functions on the block.

### Example layout (from `tf_lite_frame.cdf`)

```
Y=12   [load_model StartPort]       [load_ok FinishPort]        Y=12
Y=22   [model_file_path InputPort]  [load_error FinishPort]     Y=22
                                    [load_errno OutputPort/I]   Y=32
                                    [model_info OutputPort/S]   Y=42

       ← ~15 unit gap between functions →

Y=57   [do_inference StartPort]     [done_inference FinishPort] Y=55
Y=67   [frame_id InputPort]         [output OutputPort/S]       Y=65
                                    [inference_error FinishPort]Y=75
                                    [inference_errno OutputPort/I]Y=85
```

Input ports (left, X=0) and their paired output/finish ports (right, X=95) stay close to the same Y range — keeping related ports visually grouped across the block.

---

## Function Block Help Files

### Location and naming — CRITICAL for Lucid

**The CDF filename, the docs subdirectory name, and the `<Class>` element inside the CDF must all be identical.** Lucid uses the `<Class>` value to locate the help file via the directory name — a deploy script handles the final `help.html` filename when publishing to Lucid.

Each component gets a `docs/help.html` in a subdirectory alongside its `.cdf`:
```
Common/Components/<category>/<block_name>/docs/help.html
Common/Components/<category>/<block_name>.cdf
```

The **directory** name must match `<Class>`. For a block with `<Class>ml_svm_inference</Class>`:
```
Common/Components/ml/ml_svm_inference.cdf               ← CDF filename = Class name
Common/Components/ml/ml_svm_inference/docs/help.html    ← directory name = Class name
Common/Components/ml/ml_svm_inference/docs/ml_svm_inference.md
```

If the CDF filename, docs directory, or `<Class>` diverge, Lucid cannot find the help file. When renaming a block, rename all three: the `.cdf` file, the docs directory, and `<Class>` inside the CDF.

The markdown doc is regenerated with:
```bash
python3 scripts/software-utilities/cdf_to_ascii.py \
    Common/Components/<cat>/<Class>.cdf \
    > Common/Components/<cat>/<Class>/docs/<Class>.md
```

### Template
Base template: `docs/helpTemplate.html`

CSS is loaded via 4 relative `<link>` tags pointing to `iabStyle.css` (levels `./`, `../`, `../../`, `../../../`).

### Structure
- `<div class="page_container">` wrapping everything
- `<div class="toc_container">` for table of contents
- Parameters `<table class="table">` with columns: Name | Default | Min | Max | Description
- Ports `<table class="table">` with columns: Name | Type | Description

### Port section order
Always: **In Events → Data In → Out Events → Data Out**

### Port CSS classes
| Class | Used for |
|---|---|
| `portsEvent` | StartPort / FinishPort |
| `portsStr` | String (S) data ports |
| `portsInt` | Integer (I) data ports |
| `portsBool` | Boolean (B) data ports |
| `portsReal` | Float/Real (F) data ports |

---

## Adding a port to an existing function block

Four files must change in sync. Using `model_info OutputPort/S` on `load_model` as the canonical example:

### 1. CDF file — add the `<Port>` element

Place it after the last existing port at the same arg level. Set `argument=` to match the arg number (here `2`, same as the error FinishPort). Increment `<YCoordinate>` by 10 from the previous port on the same side. Recalculate `<Height>` as `new_last_port_Y + 28` (see Block Geometry section).

```xml
<Port>
    <DataType>S</DataType>
    <Description>model info</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>85</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <CName>model_info</CName>
    <Function argument="2">
        <Function_ERT1_ID>1</Function_ERT1_ID>
    </Function>
</Port>
```

### 2. C file — add a friendly label macro

Inside the `//ICB FRIENDLY LABELS` block:
```c
#define INX_<FB>_ARG_<function>_model_info 2   // must match argument= in CDF
```

### 3. C file — call the output in the run function

Inside the relevant `EHS_FB_RUN_FUNCTION`, write the output before firing the success FinishPort:
```c
if (EHS_FB_OUT_CONNECTED_API2(INX_<FB>_ARG_<function>_model_info)) {
    EhsML_GetModelInfoJson(
        &state->ml_ctx,
        szCanonicalFilePath,
        EHS_FB_OUT_S_API2(INX_<FB>_ARG_<function>_model_info),
        EHS_STRING_LENGTH_MAX);
}
```

### 4. Regenerate the markdown doc

```bash
name=ml_image_inference   # or whatever the component is
python3 scripts/software-utilities/cdf_to_ascii.py \
    Common/Components/ml/${name}.cdf \
    > Common/Components/ml/${name}/docs/${name}.md
```

**Rules to remember:**
- An `OutputPort/S` at `argument=2` is valid alongside the error `FinishPort` at `argument=2` — they use different macros (`EHS_FB_OUT_S_API2` vs `EHS_FB_FINISH`) so there is no conflict.
- Only ONE `OutputPort` of each data type per argument number per function.
- The CDF `<Height>` must be recalculated as `last_port_Y + 28` whenever a port is added (see Block Geometry section for derivation).

---

## Function Block C Implementation

### Parameter parsing in `EHS_FB_INIT_FUNCTION`

**Always use `EhsGet*` functions to parse the parameter string.** Do not call `sscanf`, `strtol`, `atoi`, custom tokenisers, or any other parsing approach. The `EhsGet*` family is the only sanctioned way to read CDF parameters — it handles the tokenisation contract the kernel and SODL writer rely on.

| Value type    | Function                                             |
|---------------|------------------------------------------------------|
| String / word | `pParams = EhsGetWordFromString(buf, pParams, size)` |
| Signed int    | `pParams = EhsGetSint32FromString(&val, pParams)`    |
| Real / float  | `pParams = EhsGetReal32FromString(&val, pParams)`    |

Each call advances `pParams` past the consumed token and returns the new position. Always chain them in the exact order the parameters appear in the CDF (matching `<ArgPlacement>` sequence) — the parser is position-based.

If a parameter needs further interpretation after reading (e.g. parsing a UUID string into a binary struct), read it as a word first with `EhsGetWordFromString`, then convert it with whatever helper is appropriate. Never write a custom tokeniser that peeks at the raw `pParams` string directly.

Deviating from `EhsGet*` is occasionally justified (e.g. a tightly constrained embedded parse of a known fixed-format field) but must be deliberate and documented with a comment explaining why — it is not the default.

### Required include

Every function block `.c` file must include `inx-component.h` as its first inx header:

```c
#include "inx-component.h"
#include "<block_name>.h"
#include "hal_<block_name>.h"
```

`inx-component.h` provides all `EHS_FB_*` KAPI macros. Do **not** use `globals.h` as the primary include in function block `.c` files — it lacks the KAPI macros.

### KAPI macro reference

| Operation | Macro | Notes |
|---|---|---|
| Fire a FinishPort | `EHS_FB_FINISH_API2(arg_num)` | `arg_num` = the `argument=` value in the CDF |
| Write integer output | `EHS_FB_OUT_I_API2(arg) = value` | **lvalue** — use assignment syntax |
| Write boolean output | `EHS_FB_OUT_B_API2(arg) = value` | **lvalue** — use assignment syntax |
| Write float output | `EHS_FB_OUT_F_API2(arg) = value` | **lvalue** — use assignment syntax |
| Write string output | `EHS_FB_OUT_S_API2(arg)` | returns `char *` buffer — write into it |
| Read integer input | `EHS_FB_IN_I_API2(arg)` | rvalue |
| Read boolean input | `EHS_FB_IN_B_API2(arg)` | rvalue |
| Read float input | `EHS_FB_IN_F_API2(arg)` | rvalue |
| Check port connected | `EHS_FB_IN_CONNECTED_API2(arg)` / `EHS_FB_OUT_CONNECTED_API2(arg)` | |

**Critical**: `EHS_FB_OUT_I_API2(arg)` is a pointer-dereference **lvalue**, not a function call.
Always assign: `EHS_FB_OUT_I_API2(arg) = value;`  — never `EHS_FB_OUT_I_API2(arg, value)`.

There is no `EHS_FB_FINISH_PORT("name")` macro. Always use `EHS_FB_FINISH_API2(arg_num)` with the numeric argument number from the CDF `<Function argument="N">` attribute.

### InternalPort async callback pattern

InternalPorts (at `XCoordinate=-1`, `YCoordinate=-1` in the CDF) are callback functions fired asynchronously by the HAL. They use the `EhsCallbackQueue` mechanism — the same pattern as UART's `recv_cb` in `inx-uart.c`.

**Module-level (in the `.c` file):**
```c
static EhsCallbackQueueType xMyEventQueue;
```

**Per-instance state (in `hal_<block>.h`, inside the state struct):**
```c
EhsCallbackQueueEntryType xMyEventEntry;
EhsCallbackQueueType     *pMyEventQueue;   /* pointer set at init so HAL can fire it */
```

**`EHS_FB_INIT_FUNCTION`:**
```c
state->pMyEventQueue = &xMyEventQueue;
EhsCallbackQueue_register(&xMyEventQueue,
                           EHS_FB_RUN_NAME(block_my_event_cb),
                           EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1),  /* -1 = 1st InternalPort, -2 = 2nd */
                           &(state->xMyEventEntry));
```

**HAL implementation fires the callback:**
```c
EhsCallbackQueue_execute(state->pMyEventQueue);
```

**The callback run function** only needs to fire its FinishPort:
```c
EHS_FB_RUN_FUNCTION(block_my_event_cb)
{
    EHS_FB_FINISH_API2(1);
}
```

The `hal_<block>.h` must include `"callback_queue.h"` for the `EhsCallbackQueue*` types.

---

## CDF `<Functions>` naming convention

The `<name>` element inside each `<Function>` in the `<Functions>` section must be a **lowercase descriptive name matching the C function name without the block prefix**. This is the established convention (see `uart.cdf`: `start`, `close`, `recv_cb`, `send`, `flush`).

**Correct:**
```xml
<Function>
    <name>read_status</name>
    <ID>
        <ERT1_ID>1</ERT1_ID>
    </ID>
</Function>
```

**Wrong** (agent-generated PascalCase — do not use):
```xml
<Function>
    <name>ReadStatusFunc</name>
    <ID><ERT1_ID>1</ERT1_ID></ID>
</Function>
```

---

## Function Block ID Generation

Every function block has two hash-based identifiers stored in the CDF `<Hashes>` block and mirrored in the `.h` header file.

### `NameHash_CRC16` — the block's unique runtime ID

This is a **CRC-16/Modbus** hash of the `<Class>` name string (exact byte sequence, UTF-8, no null terminator).

**To compute the hash for a new block, use the `inxtool.py` script** (from repo root):
```bash
python3 scripts/inxware-id-tool/inxtool.py -genHash "<class_name>" -hash 16CRC
```

The tool prints the 4-hex-digit result (e.g. `F512`). Prepend `0x` when writing to the CDF (`0xF512`).

The result is written to **two places**:

1. **CDF** `<Hashes>` block:
```xml
<Hashes>
    <NameHash_CRC16>0xF512</NameHash_CRC16>
    <FbApiDescriptorHash_CRC32>00000000</FbApiDescriptorHash_CRC32>
    <FbApiDescriptorHash/>
</Hashes>
```

2. **Header file** — `INXWARE_FB_ID_<Class>` macro (the value from the tool, prefixed with `0x`):
```c
#define INXWARE_FB_ID_my_block_class  0xF512
```

Verified values: `adc_config`→`0x566F`, `accel_gyro`→`0xF2AA`, `lorawan`→`0xC89A`, `rtc`→`0x32C7`, `Unsigned2Int`→`0x4F75`, `led`→`0xA6EA`, `ml_image_inference`→`0xF512`.

> **Note:** The scripts were previously in a subdirectory called `inxtool_oldscript/`. That name was a historical artefact — there was no newer replacement. The scripts have been moved up to `scripts/inxware-id-tool/` directly.

### `FbApiDescriptorHash_CRC32` — not used, leave as `00000000`

The Lucid IDE source (`LucidConstants.h`) marks both `FbApiDescriptorHash_CRC32` and `FbApiDescriptorHash` as `@TODO - this is not done yet`. Existing CDFs that show non-zero values (e.g. `5659f300` for `uart`) had these set by older versions of Lucid IDE; the algorithm is not exposed in the open-source tools.

**Always leave as `00000000` for manually-created or modified CDFs.** The `inxtool.py` script does not compute this field and neither should you.

---

## Platform support matrix

To see which function blocks are available on which platforms (and which HAL variant is selected), use `scripts/software-utilities/fb_platform_matrix.py`:

```bash
# Most useful view — HAL-selectable blocks only, markdown:
python3 scripts/software-utilities/fb_platform_matrix.py \
    --format md --no-always --no-never

# Sfera Labs platforms only:
python3 scripts/software-utilities/fb_platform_matrix.py \
    --format md --platform '*sferalabs*' --no-always --no-never

# All platforms, CSV (pipe to file for spreadsheet):
python3 scripts/software-utilities/fb_platform_matrix.py > matrix.csv

# Single block family:
python3 scripts/software-utilities/fb_platform_matrix.py \
    --format md --component 'accel_gyro'
```

The script collapses primitive-family variants (type and count variants such as `MultiplexFourInputInt` / `MultiplexEightInputBool`) into a single representative row. Core toolbox blocks that are always built show as `always`; platform-specific blocks show the HAL variant name (`sferalabs`, `stubbed`, `arduino`, …) or `—` if not built.

---

## Build note for cross-compilation targets

Always use `make all_docker` — `make all` fails on cross-compilation targets (e.g. arm64 built on x86_64) because the host lacks the sysroot headers (`bits/libc-header-start.h` etc.).
