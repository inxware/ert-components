# Function Block Authoring Guide

Reference for creating and modifying eRT function blocks (CDF + C implementation).

> **See also:** [`CLAUDE-general.md`](CLAUDE-general.md) for the full CDF XML schema reference (port types, data types, key elements), component file structure, `EHS_FB_*` macro reference, and the component creation workflow.

---

## CDF Port Argument Number Rules

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

Place it after the last existing port at the same arg level. Set `argument=` to match the arg number (here `2`, same as the error FinishPort). Increment `<YCoordinate>` by 10 from the previous port on the same side. Increase `<Height>` in `<Block>` by 10.

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
- The CDF `<Height>` must be increased by 10 for each new port added.

---

## Function Block C Implementation

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

## Build note for cross-compilation targets

Always use `make all_docker` — `make all` fails on cross-compilation targets (e.g. arm64 built on x86_64) because the host lacks the sysroot headers (`bits/libc-header-start.h` etc.).
