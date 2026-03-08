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

### Location
Each component gets a `docs/help.html` in a subdirectory alongside its `.cdf`:
```
Common/Components/<category>/<block_name>/docs/help.html
Common/Components/<category>/<block_name>.cdf
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

## Build note for cross-compilation targets

Always use `make all_docker` — `make all` fails on cross-compilation targets (e.g. arm64 built on x86_64) because the host lacks the sysroot headers (`bits/libc-header-start.h` etc.).
