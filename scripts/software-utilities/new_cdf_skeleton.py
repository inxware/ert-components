#!/usr/bin/env python3
"""
new_cdf_skeleton.py — Generate a validator-clean CDF from a small JSON spec.

The point of this script is determinism. Authoring a CDF by hand requires
remembering ~15 distinct rules: geometry from cdf_geometry.py, NameHash_CRC16
from inxtool.py, port argument-number contiguity (check_cdf_function_args.py),
InternalPort X/Y/CName pairing (check_cdf_internalport_consistency.py), title
text placement, the F-vs-R DataType trap, ERT1_ID assignment, etc.

This script encodes all of those rules. The author provides a class name,
parameters, and the per-function port list; everything else is computed.

USAGE
=====

    python3 scripts/software-utilities/new_cdf_skeleton.py \\
        --spec specs/my_block.json \\
        --out  Common/Components/<category>/my_block.cdf

    python3 scripts/software-utilities/new_cdf_skeleton.py \\
        --spec specs/my_block.json   # prints to stdout, no file written

    python3 scripts/software-utilities/new_cdf_skeleton.py --print-example
        # prints a fully-annotated example spec to stdout

JSON SPEC FORMAT
================

    {
      "class":              "my_block",              # required, snake_case
      "label":              "My Block",              # required, IDE display
      "short_description":  "One-line summary",
      "long_description":   "Longer paragraph.",
      "menu":               ["Top", "Mid", "Leaf"],  # IDE menu path
      "block_type":         "IO",                    # IO | Data_Processor | ...
      "module":             "usercomponents",        # optional
      "mandatory_first_start": false,                # default false; set true
                                                     # to mark the first
                                                     # StartPort with
                                                     # <MandatoryFlag>1</...>

      "parameters": [
        { "name": "Object",   "type": "ST", "default": "root",
          "description": "...", "min": 0, "max": 0 },
        { "name": "Property", "type": "ST", "default": "value",
          "description": "...", "min": 0, "max": 0 }
      ],

      "functions": [
        {
          "name": "set",                             # lowercase, no class prefix
          "internal": false,                         # true => StartPort is a
                                                     # hidden InternalPort
                                                     # (HAL-fired callback)
          "ports": [
            { "role": "start",  "cname": "set" },           # arg=0
            { "role": "input",  "cname": "value", "dtype": "I" },
            { "role": "finish", "cname": "done" }
            # all non-start ports default to arg=1 unless specified
          ]
        },
        {
          "name": "changed_cb",
          "internal": true,
          "ports": [
            { "role": "finish", "cname": "changed" },
            { "role": "output", "cname": "value_out", "dtype": "I" }
          ]
        }
      ]
    }

PORT ROLES
==========

    start       Start of a normal function (left, X=0, visible).
    internal    Start of a callback function (hidden, X=-1, Y=-1).
                Implicit when function "internal": true is set — do not list.
    input       Data input (left side, X=0).
    output      Data output (right side, X=Width-5).
    finish      Event output / finish port (right side, X=Width-5).

PORT FIELDS
===========

    cname       Required for all ports. The C identifier.
    description Optional. Defaults to cname with underscores replaced by spaces.
    dtype       Required for input/output: one of "I", "F", "B", "S".
                Forbidden for start/finish/internal ports.
    arg         Optional override of the auto-assigned argument number.
                Auto: start/internal → 0; all others → 1 (unless multiple
                FinishPorts in one function — then assigned sequentially 1,2,3
                per FinishPort).

WHAT THE SCRIPT COMPUTES AUTOMATICALLY
======================================

    NameHash_CRC16             via scripts/inxware-id-tool/hash.Hash16CRC
    Block geometry             via the same logic as cdf_geometry.py:
                               Width tier from max label length;
                               Height = last visible port Y + 28;
                               LocationY = -15; TextX=10, TextY=5.
    Right-side port X          Width - 5
    Left-side port X           0
    Hidden port X/Y            -1 / -1
    Per-port Y coordinate      Auto-laid-out 10/20/35/45-style with a 15-unit
                               gap between functions. Internal-only functions
                               do not consume visible Y space.
    Function ERT1_IDs          1, 2, 3, ... in spec order
    Argument numbers           Per the rule in PORT FIELDS above
    FbApiDescriptorHash_CRC32  "00000000" (per CLAUDE-function-blocks.md SOP)

AFTER GENERATING
================

The four CDF validators must still be run; this script writes valid output but
the validators are the source of truth:

    python3 scripts/software-utilities/cdf_geometry.py            validate <cdf>
    python3 scripts/inxware-id-tool/check_cdf_hashes.py                    <dir>
    python3 scripts/inxware-id-tool/check_cdf_function_args.py             <cdf>
    python3 scripts/inxware-id-tool/check_cdf_internalport_consistency.py  <cdf>

The last one needs a matching `.c` file to do useful work — author the C
skeleton (see CLAUDE-function-blocks.md § Function Block C Implementation)
before running it.
"""

import argparse
import datetime
import json
import os
import sys
from typing import Optional

# Import the canonical CRC16 implementation from the existing tooling.
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
INXTOOL_DIR = os.path.normpath(os.path.join(SCRIPT_DIR, "..", "inxware-id-tool"))
sys.path.insert(0, INXTOOL_DIR)
from hash import Hash16CRC  # noqa: E402

# ---------------------------------------------------------------------------
# Geometry rules — same constants as cdf_geometry.py.
# ---------------------------------------------------------------------------

LOCATION_Y = -15
BOTTOM_CLEARANCE = 13            # Height = last_port_Y + (15 + 13) = last_port_Y + 28
HEIGHT_PADDING = 28
RIGHT_INSET = 5                  # right-side port X = Width - 5
TEXT_X = 10
TEXT_Y = 5
TEXT_SCALE = 1.25

WIDTH_TIERS = [
    (30,   2),    # (width, max_label_chars_fitting_comfortably)
    (65,   9),
    (75,  11),
    (95,  14),
    (105, 16),
    (115, 20),
]
WIDTH_MAX_TIER = 115

FUNCTION_GAP = 15                # gap between last port of one function and
                                 # first port of the next
PORT_STRIDE = 10                 # vertical stride between ports inside a
                                 # function

DTYPE_PORT_ALLOWED = {"I", "F", "B", "S"}   # F not R — see CLAUDE-function-blocks.md
PARAM_DTYPE_ALLOWED = {"I", "F", "B", "S", "ST", "WT"}
BLOCK_TYPE_ALLOWED = {
    "IO", "Data_Processor", "Data", "Event_Processor",
    "And_Event", "Or_Event", "Sub_System", "Sub_System_IO",
}

# ---------------------------------------------------------------------------
# Spec validation
# ---------------------------------------------------------------------------


def _fail(msg: str) -> None:
    print(f"new_cdf_skeleton.py: error: {msg}", file=sys.stderr)
    sys.exit(1)


def validate_spec(spec: dict) -> None:
    required_top = ["class", "label", "functions"]
    for k in required_top:
        if k not in spec:
            _fail(f"spec missing required key '{k}'")

    cls = spec["class"]
    if not isinstance(cls, str) or not cls or not cls.replace("_", "").isalnum():
        _fail(f"'class' must be a snake_case identifier, got: {cls!r}")

    bt = spec.get("block_type", "IO")
    if bt not in BLOCK_TYPE_ALLOWED:
        _fail(
            f"block_type '{bt}' not in allowed set "
            f"{sorted(BLOCK_TYPE_ALLOWED)} — see CLAUDE-function-blocks.md § Block Type"
        )

    for i, p in enumerate(spec.get("parameters", [])):
        for k in ("name", "type", "default"):
            if k not in p:
                _fail(f"parameter[{i}] missing required key '{k}'")
        if p["type"] not in PARAM_DTYPE_ALLOWED:
            _fail(
                f"parameter '{p['name']}' has DataType {p['type']!r}; "
                f"allowed: {sorted(PARAM_DTYPE_ALLOWED)}"
            )

    fn_names = set()
    for i, fn in enumerate(spec["functions"]):
        if "name" not in fn or "ports" not in fn:
            _fail(f"function[{i}] missing 'name' or 'ports'")
        if fn["name"] in fn_names:
            _fail(f"duplicate function name {fn['name']!r}")
        fn_names.add(fn["name"])

        seen_start = False
        for j, port in enumerate(fn["ports"]):
            if "role" not in port or "cname" not in port:
                _fail(f"function {fn['name']!r} port[{j}] missing 'role' or 'cname'")
            role = port["role"]
            if role not in ("start", "input", "output", "finish"):
                _fail(
                    f"function {fn['name']!r} port {port['cname']!r}: "
                    f"role {role!r} not in start/input/output/finish "
                    f"(internal-start ports come from function 'internal': true)"
                )
            if role == "start":
                if fn.get("internal"):
                    _fail(
                        f"function {fn['name']!r}: 'internal': true means the "
                        f"StartPort is implicit — remove the role='start' entry"
                    )
                if seen_start:
                    _fail(f"function {fn['name']!r}: multiple 'start' ports")
                seen_start = True
            if role in ("input", "output"):
                dt = port.get("dtype")
                if dt not in DTYPE_PORT_ALLOWED:
                    _fail(
                        f"function {fn['name']!r} port {port['cname']!r}: "
                        f"dtype {dt!r} must be one of {sorted(DTYPE_PORT_ALLOWED)} "
                        f"(use 'F' for floats, not 'R')"
                    )
            elif "dtype" in port:
                _fail(
                    f"function {fn['name']!r} port {port['cname']!r}: "
                    f"role={role} must not carry 'dtype'"
                )

        if not fn.get("internal") and not seen_start:
            _fail(
                f"function {fn['name']!r} has no StartPort and is not "
                f"marked 'internal': true"
            )


# ---------------------------------------------------------------------------
# Geometry computation
# ---------------------------------------------------------------------------


def suggest_width(max_label_len: int) -> int:
    for w, cap in WIDTH_TIERS:
        if max_label_len <= cap:
            return w
    return WIDTH_MAX_TIER


def assign_args_for_function(fn: dict) -> None:
    """Mutate ports to set port['arg'] if the spec didn't.

    Auto-assignment rule (matches the SOP):
      * start / internal-start → arg=0
      * All other ports default to arg=1. When a function has multiple
        FinishPorts, each gets the next sequential arg (1, 2, 3, ...). Input
        and output ports that the spec lists *between* two FinishPorts inherit
        the arg of the FinishPort they precede (this matches the
        success-path / error-path grouping pattern in ml_image_inference.cdf).
    """
    finish_idx = 0
    pending_arg = 1
    seen_finish_in_pending = False
    for port in fn["ports"]:
        if "arg" in port:
            continue
        role = port["role"]
        if role == "start":
            port["arg"] = 0
            continue
        if role == "finish":
            finish_idx += 1
            port["arg"] = finish_idx
            pending_arg = finish_idx + 1
            seen_finish_in_pending = True
        else:
            # input or output — sits with the next not-yet-seen finish, or
            # with the previous finish if there's only one outcome path.
            if seen_finish_in_pending:
                # second outcome path's data ports follow its FinishPort
                port["arg"] = finish_idx
            else:
                # before the first FinishPort: pair with arg=1 (success path)
                port["arg"] = max(1, pending_arg)


def assign_geometry(spec: dict) -> tuple[dict, int, int]:
    """Returns (per_port_xy, width, height).

    per_port_xy is keyed by (function_index, port_index) → (x, y, side).
    side ∈ {'left', 'right', 'hidden'}.
    """
    # Compute width from the longest visible label (CName + Description).
    max_label = 0
    for fn in spec["functions"]:
        for port in fn["ports"]:
            cname = port.get("cname", "")
            desc = port.get("description") or cname.replace("_", " ")
            max_label = max(max_label, len(cname), len(desc))
    # Also consider the block's display label.
    max_label = max(max_label, len(spec.get("label", "")) + 2)
    width = suggest_width(max_label)
    right_x = width - RIGHT_INSET

    per_port = {}
    cursor_y = 10                       # first port of first function at Y=10
    last_visible_y = 0

    for fi, fn in enumerate(spec["functions"]):
        function_visible = not fn.get("internal", False)

        if function_visible:
            base_y = cursor_y
        else:
            # Internal-only function: it still contributes a hidden StartPort
            # and may have visible FinishPort/OutputPorts that the HAL
            # callback writes to. Visible ports of an internal function get
            # the same Y treatment as a normal function, but there is no
            # left-side StartPort to align with.
            base_y = cursor_y

        # Walk ports — first decide each port's side, then assign Y.
        left_ys: list[int] = []
        right_ys: list[int] = []
        function_visible_used = False

        for pi, port in enumerate(fn["ports"]):
            role = port["role"]
            if role == "start":
                side = "left"
            elif role == "input":
                side = "left"
            elif role in ("finish", "output"):
                side = "right"
            else:
                side = "hidden"

            if side == "hidden":
                per_port[(fi, pi)] = (-1, -1, "hidden")
                continue

            if side == "left":
                if not left_ys:
                    y = base_y
                else:
                    y = left_ys[-1] + PORT_STRIDE
                left_ys.append(y)
                x = 0
            else:  # right
                if not right_ys:
                    # Pair the first FinishPort with the StartPort's Y if
                    # there is one; otherwise sit at base_y.
                    y = left_ys[0] if left_ys else base_y
                else:
                    y = right_ys[-1] + PORT_STRIDE
                right_ys.append(y)
                x = right_x

            per_port[(fi, pi)] = (x, y, side)
            function_visible_used = True
            last_visible_y = max(last_visible_y, y)

        # Hidden InternalPort StartPort, if function is internal.
        if fn.get("internal"):
            per_port[(fi, "internal_start")] = (-1, -1, "hidden")

        if function_visible_used:
            cursor_y = last_visible_y + FUNCTION_GAP

    height = (last_visible_y or 10) + HEIGHT_PADDING
    return per_port, width, height


# ---------------------------------------------------------------------------
# CDF emission
# ---------------------------------------------------------------------------


def crc16_hex(name: str) -> str:
    """CRC16/Modbus of the class name, as '0xXXXX'."""
    h = Hash16CRC("16CRC", bytearray(name, "utf-8"), False)
    return "0x" + h.getHash().upper()


def _xml_escape(s: str) -> str:
    return (s.replace("&", "&amp;")
             .replace("<", "&lt;")
             .replace(">", "&gt;")
             .replace('"', "&quot;"))


def emit_cdf(spec: dict) -> str:
    cls = spec["class"]
    label = spec["label"]
    block_type = spec.get("block_type", "IO")
    module = spec.get("module", "usercomponents")
    short_desc = spec.get("short_description", "")
    long_desc = spec.get("long_description", "")
    menu = spec.get("menu", [])
    parameters = spec.get("parameters", [])
    functions = spec["functions"]
    mandatory_first_start = spec.get("mandatory_first_start", False)

    # Assign per-function ERT1_IDs in spec order.
    for i, fn in enumerate(functions):
        fn["_id"] = i + 1
        assign_args_for_function(fn)

    per_port, width, height = assign_geometry(spec)

    now = datetime.datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%SZ")
    name_hash = crc16_hex(cls)

    # ---- Menu ------------------------------------------------------------
    # The outer <Menu>...</Menu> wrapper comes from the top-level template;
    # this function emits the *contents* of that wrapper, which is the first
    # menu level's text followed by a nested <Menu> for each subsequent level.
    def render_menu_inner(parts: list[str], indent_level: int) -> str:
        if not parts:
            return ""
        if len(parts) == 1:
            return _xml_escape(parts[0])
        pad = " " * (4 * indent_level)
        return (
            _xml_escape(parts[0])
            + "\n" + pad + "<Menu>"
            + render_menu_inner(parts[1:], indent_level + 1)
            + "</Menu>"
        )

    menu_inner = render_menu_inner(menu, indent_level=4)

    # ---- Parameters ------------------------------------------------------
    if parameters:
        params_xml = "<Parameters>\n"
        for i, p in enumerate(parameters):
            params_xml += f"""    <Parameter>
        <Name>{_xml_escape(p['name'])}</Name>
        <DataType>{p['type']}</DataType>
        <DefaultValue>{_xml_escape(str(p['default']))}</DefaultValue>
        <MinValue>{p.get('min', 0)}</MinValue>
        <MaxValue>{p.get('max', 0)}</MaxValue>
        <Description>{_xml_escape(p.get('description', p['name']))}</Description>
        <ListPlacement>{i + 1}</ListPlacement>
        <ArgPlacement>{i + 1}</ArgPlacement>
    </Parameter>
"""
        params_xml += "</Parameters>"
    else:
        params_xml = "<Parameters/>"

    # ---- Functions section ----------------------------------------------
    funcs_xml = "<Functions>\n"
    for fn in functions:
        funcs_xml += f"""    <Function>
        <name>{fn['name']}</name>
        <ID>
            <ERT1_ID>{fn['_id']}</ERT1_ID>
        </ID>
    </Function>
"""
    funcs_xml += "</Functions>"

    # ---- Ports -----------------------------------------------------------
    ports_xml = "<Ports>\n"
    first_start_emitted = False

    for fi, fn in enumerate(functions):
        fn_id = fn["_id"]
        for pi, port in enumerate(fn["ports"]):
            x, y, side = per_port[(fi, pi)]
            role = port["role"]
            cname = port["cname"]
            desc = port.get("description") or cname.replace("_", " ")
            arg = port["arg"]

            if role == "start":
                port_type = "StartPort"
                dt_xml = ""
                mandatory_xml = ""
                if mandatory_first_start and not first_start_emitted:
                    mandatory_xml = "        <MandatoryFlag>1</MandatoryFlag>\n"
                first_start_emitted = True
                fn_attrs = (f'        <Function argument="{arg}">\n'
                            f'            <Function_ERT1_ID>{fn_id}</Function_ERT1_ID>\n'
                            f'            <AtomicFlag>0</AtomicFlag>\n'
                            f'        </Function>\n')
                ports_xml += f"""    <Port>
        <Description>{_xml_escape(desc)}</Description>
        <PortType>{port_type}</PortType>
        <XCoordinate>{x}</XCoordinate>
        <YCoordinate>{y}</YCoordinate>
{mandatory_xml}        <CName>{cname}</CName>
{fn_attrs}    </Port>
"""
            elif role in ("input", "output"):
                port_type = "InputPort" if role == "input" else "OutputPort"
                dt = port["dtype"]
                ports_xml += f"""    <Port>
        <DataType>{dt}</DataType>
        <Description>{_xml_escape(desc)}</Description>
        <PortType>{port_type}</PortType>
        <XCoordinate>{x}</XCoordinate>
        <YCoordinate>{y}</YCoordinate>
        <CName>{cname}</CName>
        <Function argument="{arg}">
            <Function_ERT1_ID>{fn_id}</Function_ERT1_ID>
        </Function>
    </Port>
"""
            elif role == "finish":
                ports_xml += f"""    <Port>
        <Description>{_xml_escape(desc)}</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>{x}</XCoordinate>
        <YCoordinate>{y}</YCoordinate>
        <Wcet>0</Wcet>
        <CName>{cname}</CName>
        <Function argument="{arg}">
            <Function_ERT1_ID>{fn_id}</Function_ERT1_ID>
        </Function>
    </Port>
"""
        # InternalPort StartPort if this is a callback function.
        if fn.get("internal"):
            cb_cname = f"{fn['name']}_start"
            ports_xml += f"""    <Port>
        <Description>{cb_cname}</Description>
        <PortType>InternalPort</PortType>
        <XCoordinate>-1</XCoordinate>
        <YCoordinate>-1</YCoordinate>
        <CName>{cb_cname}</CName>
        <Function argument="0">
            <Function_ERT1_ID>{fn_id}</Function_ERT1_ID>
        </Function>
    </Port>
"""
    ports_xml += "</Ports>"

    # ---- Glue it all together -------------------------------------------
    indent = lambda block, n=4: "\n".join(  # noqa: E731
        (" " * n + line) if line else line for line in block.splitlines()
    )

    cdf = f"""<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>{module}</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>{now}</CreationDate>
        <UpdatedDate>{now}</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>{_xml_escape(short_desc)}</ShortDescription>
        <LongDescription>{_xml_escape(long_desc)}</LongDescription>
        <UserName/>
        <Menu>{menu_inner}</Menu>
    </Description>
    <Block>
        <Type>{block_type}</Type>
        <Width>{width}</Width>
        <Height>{height}</Height>
        <Text>{_xml_escape(label)}</Text>
        <TextX>{TEXT_X}</TextX>
        <TextY>{TEXT_Y}</TextY>
        <TextScale>{TEXT_SCALE}</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>{LOCATION_Y}</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>{cls}</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>{name_hash}</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>00000000</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    {indent(params_xml, 4).lstrip()}
    {indent(funcs_xml, 4).lstrip()}
    {indent(ports_xml, 4).lstrip()}
</Component>
"""
    return cdf


# ---------------------------------------------------------------------------
# C-side emission — inx-<class>.h and inx-<class>.c
# ---------------------------------------------------------------------------
# These are the canonical templates from CLAUDE-function-blocks.md
# "Canonical C skeleton". The dtype → macro / type table is encoded once
# here; do not duplicate it in calling code.

_DTYPE_TO_CTYPE = {
    "I": "ehs_sint32",
    "F": "ehs_float",
    "B": "ehs_bool",
    "S": "ehs_char",            # arrays of EHS_STRING_LENGTH_MAX
}

_DTYPE_TO_IN_MACRO = {
    "I": "EHS_FB_IN_I_API2",
    "F": "EHS_FB_IN_F_API2",
    "B": "EHS_FB_IN_B_API2",
    "S": "EHS_FB_IN_S_API2",
}

_DTYPE_TO_OUT_MACRO = {
    "I": "EHS_FB_OUT_I_API2",
    "F": "EHS_FB_OUT_F_API2",
    "B": "EHS_FB_OUT_B_API2",
    "S": "EHS_FB_OUT_S_API2",
}

_DTYPE_PRINTF_FMT = {"I": "%d", "F": "%f", "B": "%d", "S": "%s"}

# Parameter type → state-struct field type + INIT-time parse function.
#   ST and WT are stored as strings (char buffers).
#   I parses to ehs_sint32; F parses to ehs_float; B parses as ehs_sint32 then
#   coerces to ehs_bool.
_PARAM_TO_FIELD = {
    "S":  ("ehs_char",  "string"),
    "ST": ("ehs_char",  "string"),
    "WT": ("ehs_char",  "string"),
    "I":  ("ehs_sint32", "int"),
    "F":  ("ehs_float",  "float"),
    "B":  ("ehs_sint32", "int"),    # parsed as int, used as bool
}


def _c_ident(s: str) -> str:
    """Map a parameter Name like "Widget-Group" → "Widget_Group" (valid C ident)."""
    out = []
    for ch in s:
        out.append(ch if ch.isalnum() or ch == "_" else "_")
    if out and out[0].isdigit():
        out.insert(0, "_")
    return "".join(out)


def _camel(s: str) -> str:
    return "".join(p[:1].upper() + p[1:] for p in s.split("_"))


def emit_header(spec: dict, name_hash: str) -> str:
    cls = spec["class"]
    functions = spec["functions"]
    label = spec.get("label", cls)
    short = spec.get("short_description", "")

    decls = []
    decls.append(f"EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION({cls});")
    decls.append(f"EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION({cls});")
    decls.append(f"EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION({cls});")
    for fn in functions:
        decls.append(
            f"EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION({cls}_{fn['name']});"
        )

    # HAL contract — extern declarations for module-level queues and per-output
    # globals that the HAL writes into before firing the queue.
    extern_block_lines = []
    for fn in functions:
        if not fn.get("internal"):
            continue
        extern_block_lines.append(
            f"extern EhsCallbackQueueType x_{cls}_{fn['name']}_queue;"
        )
        for port in fn["ports"]:
            if port["role"] != "output":
                continue
            dt = port["dtype"]
            ctype = _DTYPE_TO_CTYPE[dt]
            if dt == "S":
                extern_block_lines.append(
                    f"extern {ctype} g_{cls}_{fn['name']}_{port['cname']}"
                    f"[EHS_STRING_LENGTH_MAX];"
                )
            else:
                extern_block_lines.append(
                    f"extern {ctype} g_{cls}_{fn['name']}_{port['cname']};"
                )
    if extern_block_lines:
        extern_block = (
            "\n/* HAL contract — HAL writes g_* values then calls\n"
            " * EhsCallbackQueue_execute(&x_*_queue) to dispatch the callback. */\n"
            + "\n".join(extern_block_lines)
            + "\n"
        )
    else:
        extern_block = ""

    func_list = "\n *   ".join(
        f"{fn['name']:<16} {'callback' if fn.get('internal') else 'function'}"
        for fn in functions
    )

    return f"""#ifndef INXWARE_{cls}
#define INXWARE_{cls}
#include "inx-component.h"

/* {_xml_escape(short) or label}
 *
 * Functions:
 *   {func_list}
 */
#define INXWARE_FB_ID_{cls}   {name_hash}
#define INXWARE_FB_NAME_{cls} "{cls}"

{chr(10).join(decls)}
{extern_block}
EHS_FB_FUNCTIONS({cls})

#endif /* INXWARE_{cls} */
"""


def emit_c(spec: dict) -> str:
    cls = spec["class"]
    functions = spec["functions"]
    parameters = spec.get("parameters", [])
    log_module = spec.get("log_module", "UNDEFINED")

    # ---- Per-instance state struct ---------------------------------------
    state_fields = []
    for fn in functions:
        if fn.get("internal"):
            state_fields.append(
                f"    EhsCallbackQueueEntryType x{_camel(fn['name'])}Entry;"
            )
    for p in parameters:
        ctype, kind = _PARAM_TO_FIELD[p["type"]]
        field = _c_ident(p["name"])
        if kind == "string":
            state_fields.append(f"    {ctype} {field}[EHS_STRING_LENGTH_MAX];")
        else:
            state_fields.append(f"    {ctype} {field};")

    state_struct = (
        f"typedef struct inx_{cls}_state\n{{\n"
        + ("\n".join(state_fields) if state_fields else "    int _unused;")
        + f"\n}} inx_{cls}_state_type;"
    )

    # ---- Module-level HAL contract (queue + globals) ---------------------
    module_globals = []
    for fn in functions:
        if not fn.get("internal"):
            continue
        module_globals.append(
            f"EhsCallbackQueueType x_{cls}_{fn['name']}_queue;"
        )
        for port in fn["ports"]:
            if port["role"] != "output":
                continue
            dt = port["dtype"]
            ctype = _DTYPE_TO_CTYPE[dt]
            if dt == "S":
                module_globals.append(
                    f"{ctype} g_{cls}_{fn['name']}_{port['cname']}"
                    f"[EHS_STRING_LENGTH_MAX];"
                )
            else:
                module_globals.append(
                    f"{ctype} g_{cls}_{fn['name']}_{port['cname']};"
                )
    module_globals_block = (
        "/* HAL contract — see header for usage. */\n"
        + "\n".join(module_globals)
        + "\n"
    ) if module_globals else ""

    # ---- Function table --------------------------------------------------
    fn_entries = "\n".join(
        f'EHS_FB_FUNCTION_ENTRY("{fn["name"]}", 0x{fn["_id"]:02x}, {cls}_{fn["name"]})'
        for fn in functions
    )

    # ---- Friendly-label macros (INX_<class>_ARG_<fn>_<port>) ------------
    label_lines = []
    for fn in functions:
        for port in fn["ports"]:
            if port["role"] in ("input", "output", "finish"):
                label_lines.append(
                    f"#define INX_{cls}_ARG_{fn['name']}_{port['cname']:<24}{port['arg']}"
                )
    labels_block = "\n".join(label_lines)

    # ---- INIT: parameter parsing -----------------------------------------
    parse_lines = []
    for p in parameters:
        field = _c_ident(p["name"])
        kind = _PARAM_TO_FIELD[p["type"]][1]
        if kind == "string":
            parse_lines.append(
                f"    pParams = EhsGetWordFromString(state->{field}, pParams, "
                f"sizeof(state->{field}));"
            )
        elif kind == "int":
            parse_lines.append(
                f"    pParams = EhsGetSint32FromString(&state->{field}, pParams);"
            )
        elif kind == "float":
            parse_lines.append(
                f"    pParams = EhsGetReal32FromString(&state->{field}, pParams);"
            )

    # ---- INIT: callback registration -------------------------------------
    cb_register_lines = []
    cb_index = -1
    for fn in functions:
        if not fn.get("internal"):
            continue
        cb_register_lines.append(
            f"    EhsCallbackQueue_register(&x_{cls}_{fn['name']}_queue,\n"
            f"                              EHS_FB_RUN_NAME({cls}_{fn['name']}),\n"
            f"                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE({cb_index}),\n"
            f"                              &(state->x{_camel(fn['name'])}Entry));"
        )
        cb_index -= 1

    # ---- RUN functions ---------------------------------------------------
    run_funcs = []
    for fn in functions:
        run_funcs.append(_emit_run_function(cls, fn))

    # ---- Header --------------------------------------------------------
    includes = [
        '#include "inx-parameters.h"',
        '#include "inx-component.h"',
        f'#include "inx-{cls}.h"',
    ]
    has_internal = any(fn.get("internal") for fn in functions)
    has_string = any(
        (p["type"] in ("S", "ST", "WT")) or
        any(port.get("dtype") == "S" for port in fn["ports"])
        for fn in functions
        for p in parameters
    ) or any(port.get("dtype") == "S" for fn in functions for port in fn["ports"])
    if has_internal:
        includes.append('#include "callback_queue.h"')
    if parameters or has_string:
        includes.append('#include "hal_string.h"')
    includes.append('#include "hal_logger.h"')

    return f"""{chr(10).join(includes)}

#define EHSL_MODULE_ID EHSH_LOG_MODULE_{log_module}

{state_struct}

{module_globals_block}
EHS_FB_FUNCTIONS_START({cls})
{fn_entries}
EHS_FB_FUNCTIONS_END

{labels_block}

EHS_FB_IDENTIFY_FUNCTION({cls})
{{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_{cls}_state_type);
}}

EHS_FB_INIT_FUNCTION({cls})
{{
    inx_{cls}_state_type *state = (inx_{cls}_state_type *)EHS_FB_INIT_CONTEXT;
    const ehs_char *pParams = EHS_FB_INIT_PARAMETERS;
    (void)state; (void)pParams;

{chr(10).join(parse_lines)}

{chr(10).join(cb_register_lines)}
    return EHS_TRUE;
}}

EHS_FB_DESTROY_FUNCTION({cls})
{{
    return EHS_TRUE;
}}

{chr(10).join(run_funcs)}
"""


def _emit_run_function(cls: str, fn: dict) -> str:
    """Emit a single EHS_FB_RUN_FUNCTION body for one function."""
    name = fn["name"]
    if fn.get("internal"):
        # Internal/callback: write HAL-supplied globals into output ports, fire finish.
        body_lines = []
        finish_arg_label = None
        for port in fn["ports"]:
            cname = port["cname"]
            arg_label = f"INX_{cls}_ARG_{name}_{cname}"
            if port["role"] == "output":
                dt = port["dtype"]
                if dt == "S":
                    body_lines.append(
                        f"    if (EHS_FB_OUT_CONNECTED_API2({arg_label}))\n"
                        f"        EhsStrncpy((ehs_char *){_DTYPE_TO_OUT_MACRO[dt]}({arg_label}),\n"
                        f"                   g_{cls}_{name}_{cname}, EHS_STRING_LENGTH_MAX);"
                    )
                else:
                    body_lines.append(
                        f"    if (EHS_FB_OUT_CONNECTED_API2({arg_label}))\n"
                        f"        {_DTYPE_TO_OUT_MACRO[dt]}({arg_label}) = g_{cls}_{name}_{cname};"
                    )
            elif port["role"] == "finish" and finish_arg_label is None:
                finish_arg_label = arg_label
        if finish_arg_label is None:
            # Should never happen — a callback function must have a finish port.
            finish_arg_label = "/* no finish port? */"
        body_lines.append(f"    EHS_FB_FINISH({finish_arg_label});")
        body = "\n".join(body_lines)
        return f"""EHS_FB_RUN_FUNCTION({cls}_{name})
{{
{body}
}}"""

    # Non-internal: read inputs, log, fire finish.
    body_lines = []
    log_args = []
    log_fmt_parts = [f"{cls} {name}"]
    finish_arg_label = None
    input_reads = []
    for port in fn["ports"]:
        cname = port["cname"]
        arg_label = f"INX_{cls}_ARG_{name}_{cname}"
        if port["role"] == "input":
            dt = port["dtype"]
            ctype = _DTYPE_TO_CTYPE[dt]
            if dt == "S":
                input_reads.append(
                    f"    const ehs_char *{cname} = {_DTYPE_TO_IN_MACRO[dt]}({arg_label});"
                )
                log_args.append(f"{cname} ? {cname} : \"(null)\"")
            else:
                input_reads.append(
                    f"    {ctype} {cname} = {_DTYPE_TO_IN_MACRO[dt]}({arg_label});"
                )
                if dt == "F":
                    log_args.append(f"(double){cname}")
                elif dt == "B":
                    log_args.append(f"(int){cname}")
                else:
                    log_args.append(f"(int){cname}")
            log_fmt_parts.append(f"{cname}={_DTYPE_PRINTF_FMT[dt]}")
        elif port["role"] == "finish" and finish_arg_label is None:
            finish_arg_label = arg_label
    if finish_arg_label is None:
        finish_arg_label = "/* no finish port? */"

    body_lines.extend(input_reads)
    # Use state-> for param logging if available
    log_fmt = " ".join(log_fmt_parts)
    if log_args:
        body_lines.append(
            f'    EHSH_LOG_INFO("{log_fmt}", {", ".join(log_args)});'
        )
    else:
        body_lines.append(f'    EHSH_LOG_INFO("{log_fmt}");')
    body_lines.append(f"    EHS_FB_FINISH({finish_arg_label});")

    body = "\n".join(body_lines)
    return f"""EHS_FB_RUN_FUNCTION({cls}_{name})
{{
    inx_{cls}_state_type *state = (inx_{cls}_state_type *)EHS_FB_RUN_CONTEXT;
    (void)state;
{body}
}}"""


# ---------------------------------------------------------------------------
# Example spec for `--print-example`
# ---------------------------------------------------------------------------


EXAMPLE_SPEC = {
    "class": "my_block",
    "label": "My Block",
    "short_description": "Short summary visible in the IDE",
    "long_description": "Longer description shown in tooltips.",
    "menu": ["Peripherals", "Custom", "My Block"],
    "block_type": "IO",
    "module": "usercomponents",
    "log_module": "UNDEFINED",
    "mandatory_first_start": False,
    "parameters": [
        {
            "name": "Object",
            "type": "ST",
            "default": "root",
            "description": "Name of the UI object to bind to.",
        },
        {
            "name": "Property",
            "type": "ST",
            "default": "value",
            "description": "Name of the property to read and write.",
        },
    ],
    "functions": [
        {
            "name": "set",
            "ports": [
                {"role": "start",  "cname": "set"},
                {"role": "input",  "cname": "value", "dtype": "I"},
                {"role": "finish", "cname": "done"},
            ],
        },
        {
            "name": "changed_cb",
            "internal": True,
            "ports": [
                {"role": "finish", "cname": "changed"},
                {"role": "output", "cname": "value_out", "dtype": "I"},
            ],
        },
    ],
}


def main() -> int:
    ap = argparse.ArgumentParser(
        description="Generate a validator-clean CDF from a JSON spec.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    ap.add_argument("--spec", help="path to JSON spec file")
    ap.add_argument("--out", help="output CDF path (default: stdout)")
    ap.add_argument("--out-c", help="also emit the inx-<class>.c here")
    ap.add_argument("--out-h", help="also emit the inx-<class>.h here")
    ap.add_argument("--out-dir",
                    help="emit all three files (cdf, .c, .h) into this directory; "
                         "filenames are derived from the spec's class name")
    ap.add_argument("--print-example", action="store_true",
                    help="print a fully-annotated example spec to stdout and exit")
    args = ap.parse_args()

    if args.print_example:
        print(json.dumps(EXAMPLE_SPEC, indent=2))
        return 0

    if not args.spec:
        ap.error("--spec is required unless --print-example is given")

    with open(args.spec, "r", encoding="utf-8") as f:
        spec = json.load(f)

    validate_spec(spec)
    cdf = emit_cdf(spec)        # also mutates spec to assign _id, arg, etc.
    name_hash = crc16_hex(spec["class"])
    header = emit_header(spec, name_hash)
    source = emit_c(spec)

    out_cdf = args.out
    out_c = args.out_c
    out_h = args.out_h
    if args.out_dir:
        cls = spec["class"]
        out_cdf = out_cdf or os.path.join(args.out_dir, f"{cls}.cdf")
        out_c = out_c or os.path.join(args.out_dir, f"inx-{cls}.c")
        out_h = out_h or os.path.join(args.out_dir, f"inx-{cls}.h")

    def _write(path: str, content: str) -> None:
        os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
        with open(path, "w", encoding="utf-8") as f:
            f.write(content)
        print(f"wrote {path}")

    if out_cdf:
        _write(out_cdf, cdf)
    elif not (out_c or out_h):
        sys.stdout.write(cdf)
    if out_c:
        _write(out_c, source)
    if out_h:
        _write(out_h, header)
    return 0


if __name__ == "__main__":
    sys.exit(main())
