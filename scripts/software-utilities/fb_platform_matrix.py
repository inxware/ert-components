#!/usr/bin/env python3
"""
fb_platform_matrix.py — function-block × platform support matrix.

Reads all CDF files and platform config.mk files and generates a table
showing which HAL variant (or "—") each function block has on each platform.

Usage:
    python3 fb_platform_matrix.py [options]

Options:
    --repo PATH         Repository root (default: auto-detect from script location)
    --format {md,csv,html}
                        Output format (default: csv)
    --platform GLOB     Filter platform names, e.g. '*sferalabs*' or 'linux_arm64*'
    --component GLOB    Filter component class names, e.g. '*accel*' or 'watchdog'
    --no-always         Omit components that are always built (no support variable)
    --no-never          Omit components that are never built on any shown platform

Examples:
    # All components vs all platforms (CSV, pipe to file):
    python3 fb_platform_matrix.py > matrix.csv

    # Sfera Labs peripheral blocks only, markdown:
    python3 fb_platform_matrix.py --format md --platform '*sferalabs*' --component '*'

    # Just the accel/gyro and watchdog rows:
    python3 fb_platform_matrix.py --component '*accel*' --component 'watchdog'

Normally invoked by LLMs or engineers to understand per-platform component availability.
"""

import argparse
import csv
import fnmatch
import glob
import os
import re
import sys
import xml.etree.ElementTree as ET
from pathlib import Path


# ---------------------------------------------------------------------------
# Repo layout
# ---------------------------------------------------------------------------

def find_repo_root(start: Path) -> Path:
    """Walk up from start until we find a directory containing 'Common' and 'target'."""
    p = start.resolve()
    for candidate in [p] + list(p.parents):
        if (candidate / "Common").is_dir() and (candidate / "target").is_dir():
            return candidate
    raise RuntimeError(f"Cannot find repo root from {start}")


# ---------------------------------------------------------------------------
# CDF parsing
# ---------------------------------------------------------------------------

def parse_cdfs(repo: Path) -> list[dict]:
    """Return list of {class, short_desc, menu, cdf_path} for every CDF found."""
    results = []
    for cdf_path in sorted(repo.glob("Common/Components/**/*.cdf")):
        try:
            tree = ET.parse(cdf_path)
            root = tree.getroot()
        except ET.ParseError:
            continue
        cls = root.findtext("FBID/Class", "").strip()
        if not cls:
            continue
        short_desc = root.findtext("Description/ShortDescription", "").strip()
        # Build menu path: outer Menu text + inner Menu text
        desc_el = root.find("Description")
        menu = ""
        if desc_el is not None:
            menu_el = desc_el.find("Menu")
            if menu_el is not None:
                parts = []
                if menu_el.text and menu_el.text.strip():
                    parts.append(menu_el.text.strip())
                for sub in menu_el:
                    if sub.text and sub.text.strip():
                        parts.append(sub.text.strip())
                menu = " / ".join(parts)
        results.append({
            "class": cls,
            "short_desc": short_desc,
            "menu": menu,
            "cdf_path": str(cdf_path.relative_to(repo)),
        })
    return results


# ---------------------------------------------------------------------------
# components.mk parsing — map class name → support variable
# ---------------------------------------------------------------------------

def _extract_support_var(line: str) -> str | None:
    """Return 'EHS_FOO_SUPPORT' if line is 'ifdef EHS_FOO_SUPPORT', else None."""
    m = re.match(r'^\s*ifdef\s+(EHS_\w+)', line)
    if m:
        return m.group(1)
    return None


def _extract_obj_class(line: str) -> str | None:
    """Return the class name if line is 'OBJECTS += [inx-]foo.$(OBJ)', else None.
    Handles both the modern 'inx-<class>' prefix and older bare '<class>' names."""
    m = re.search(r'OBJECTS\s*\+=\s*(?:inx-)?(\S+)\.\$\(OBJ\)', line)
    if m:
        return m.group(1)
    return None


# ---------------------------------------------------------------------------
# Family-key normalisation for core / toolbox components
# ---------------------------------------------------------------------------

_RE_READONLY = re.compile(r'_?(?:ReadOnly|WriteOnly)', re.IGNORECASE)
_RE_COUNT_IO = re.compile(
    r'(?:One|Two|Three|Four|Five|Six|Seven|Eight|Sixteen)(?:Input|Output)?'
)
_RE_TO_TYPE  = re.compile(
    r'To(?:Bool|Int|Real|String|Float)(?=[A-Z_]|$)', re.IGNORECASE
)
# underscore-delimited type suffix:  _Bool1  _int  _str  _Event  etc.
# 'str' is accepted as short-form of 'String' in snake_case names.
_RE_US_TYPE  = re.compile(
    r'_(?:Bool|Int|Real|String|Str|Float|Event)\d*(?=_|$)', re.IGNORECASE
)
# trailing CamelCase type word at end of name:  ...Bool  ...Int2  etc.
_RE_END_TYPE = re.compile(
    r'(?<=[a-zA-Z])(?:Bool|Int|Real|String|Float|Event)\d*$'
)
_RE_TAIL_NUM = re.compile(r'_?\d+$')
_RE_CLEAN_US = re.compile(r'_+')


def _family_key(cls: str) -> str:
    """Collapse type-variant and count-variant class names to a family name.

    Strips, in order:
      - ReadOnly / WriteOnly access-mode qualifiers
      - Count words (Two/Three/Four/Eight…) with optional Input/Output
      - ToType converter fragments (ToBool, ToInt, …)
      - Underscore-delimited type suffixes (_Bool, _Int, _Real, _String, _Float, _Event)
      - Trailing CamelCase type word at end of name
      - Trailing digits and _N suffixes
    """
    s = cls
    s = _RE_READONLY.sub('', s)
    s = _RE_COUNT_IO.sub('', s)
    s = _RE_TO_TYPE.sub('', s)
    s = _RE_US_TYPE.sub('', s)
    s = _RE_END_TYPE.sub('', s)
    s = _RE_TAIL_NUM.sub('', s)
    s = _RE_CLEAN_US.sub('_', s).strip('_')
    return s if s else cls


# When the components.mk object-name lookup fails (class name doesn't match object
# file name), fall back to the top-level subdirectory of the CDF file to determine
# whether the block is always built or conditionally compiled.
_DIR_SUPPORT_FALLBACK: dict[str, str] = {
    "core":       "ALWAYS",
    "nocode":     "ALWAYS",   # xstart/xinput/xoutput/xfinish etc.
    "deprecated": "ALWAYS",   # still compiled, just superseded
    "gui":        "EHS_GUI_SUPPORT",
    "AV":         "EHS_AV_SUPPORT",
    "media":      "EHS_MEDIA_SUPPORT",
}


def parse_components_mks(repo: Path) -> dict[str, str]:
    """
    Parse all Common/Components/**/components.mk files.
    Returns {class_name: support_variable} where support_variable may be
    'ALWAYS' if the object is unconditionally included.
    """
    mapping: dict[str, str] = {}

    for mk_path in sorted(repo.glob("Common/Components/**/components.mk")):
        lines = mk_path.read_text(errors="replace").splitlines()
        # Stack of (var_name | None) — None for ifeq/ifneq/else branches we don't track
        stack: list[str | None] = []

        for line in lines:
            stripped = line.strip()

            # ifdef / ifeq / ifneq → push
            if stripped.startswith("ifdef "):
                var = _extract_support_var(stripped)
                stack.append(var)
            elif stripped.startswith("ifeq ") or stripped.startswith("ifneq "):
                stack.append(None)
            elif stripped == "else":
                pass  # don't change stack, just let it run
            elif stripped == "endif":
                if stack:
                    stack.pop()
            else:
                cls = _extract_obj_class(stripped)
                if cls:
                    # Find innermost EHS_* variable on the stack
                    var = next(
                        (v for v in reversed(stack) if v and "EHS_" in v),
                        "ALWAYS"
                    )
                    if cls not in mapping:
                        mapping[cls] = var

    return mapping


# ---------------------------------------------------------------------------
# Platform config parsing
# ---------------------------------------------------------------------------

def find_platforms(repo: Path) -> list[str]:
    """Return sorted list of platform names (directory names under target/platform/)."""
    platform_dir = repo / "target" / "platform"
    return sorted(
        p.name for p in platform_dir.iterdir()
        if p.is_dir() and (p / "config.mk").exists()
    )


def _platform_osarch(platform: str) -> str | None:
    """Map a platform directory name to its OS-architecture column key.

    Returns None for template / meta directories that should be excluded
    from the matrix (e.g. OS_ARCH_PLATFORM-TEMPLATES).

    Examples:
      linux_arm64_lvgl_raspberrypi_debian12  → linux_arm64
      linux_x86_64_clang_gg_debian11         → linux_x86_64
      linux_android_arm64                    → linux_android_arm64
      arduino-mbed-nano_base                 → arduino
      esp32_freertos-xtensa-base             → esp32_freertos
      esp32s3_freertos-xtensa-waveshare-…    → esp32s3_freertos
      win_x86_gtk_gst                        → win_x86
      nxp_arm_inx_hri_ehs_debug             → nxp_arm
      zephyr_arm-nrf52840                    → zephyr_arm
      xcore_freertos-xcore-base              → xcore_freertos
    """
    if platform.upper().startswith("OS_ARCH"):
        return None

    # Normalise hyphens to underscores for uniform splitting
    parts = platform.lower().replace("-", "_").split("_")
    first = parts[0]

    if first == "linux":
        second = parts[1] if len(parts) > 1 else ""
        if second.startswith("android"):
            return "linux_android_arm64" if "arm64" in platform.lower() else "linux_android_arm"
        if second == "x86" and len(parts) > 2 and parts[2] == "64":
            return "linux_x86_64"
        return f"linux_{second}"

    if first == "arduino":
        return "arduino"

    if first.startswith("esp32s3"):
        return "esp32s3_freertos"

    if first.startswith("esp32"):
        return "esp32_freertos"

    if first == "win":
        second = parts[1] if len(parts) > 1 else ""
        return f"win_{second}"

    if first == "nxp":
        second = parts[1] if len(parts) > 1 else ""
        return f"nxp_{second}"

    if first == "zephyr":
        second = parts[1] if len(parts) > 1 else ""
        return f"zephyr_{second}"

    if first == "xcore":
        return "xcore_freertos"

    # Fallback: first two parts
    return "_".join(parts[:2]) if len(parts) >= 2 else first


def _parse_config_file(path: Path, result: dict[str, str]) -> None:
    """Parse one config.mk file into result, respecting ?= (setdefault) semantics."""
    try:
        text = path.read_text(errors="replace")
    except FileNotFoundError:
        return
    for line in text.splitlines():
        line = line.strip()
        if line.startswith("#"):
            continue
        m = re.match(r'^(\w+)\s*(\??)(?::?=)\s*(.+)$', line)
        if m:
            var, conditional, val = m.group(1), m.group(2), m.group(3).strip()
            if conditional == "?":
                result.setdefault(var, val)   # ?= : only set if not already defined
            else:
                result[var] = val             # = / := : unconditional


def parse_platform_config(repo: Path, platform: str) -> dict[str, str]:
    """Return the effective {VAR: value} for a platform, replicating make's include order:
      1. target/platform/<platform>/config.mk  (platform-specific, loaded first → wins)
      2. target/os-arch/<EHS_OS>-<EHS_ARCH>/config.mk  (os-arch defaults, ?= only)
    """
    result: dict[str, str] = {}

    # Step 1 — platform config (unconditional assignments take effect immediately)
    _parse_config_file(repo / "target" / "platform" / platform / "config.mk", result)

    # Step 2 — os-arch defaults (uses ?= so platform values are not overwritten)
    os_  = result.get("EHS_OS", "")
    arch = result.get("EHS_ARCH", "")
    if os_ and arch:
        osarch_cfg = repo / "target" / "os-arch" / f"{os_}-{arch}" / "config.mk"
        _parse_config_file(osarch_cfg, result)
    return result


# ---------------------------------------------------------------------------
# Cell value
# ---------------------------------------------------------------------------

NONE_VALUES = {"none", "0", "false", "no", ""}

PRESENT = "✅"
ABSENT  = "—"

def cell_value(support_var: str, platform_vars: dict[str, str]) -> str:
    """Return PRESENT (✅) if the component is supported, ABSENT (—) if not.

    platform_vars is the merged variable dict for an OS-arch group: a variable
    is present if any platform in that group sets it to a non-none value.
    """
    if support_var == "ALWAYS":
        return PRESENT
    val = platform_vars.get(support_var, "").strip()
    if not val or val.lower() in NONE_VALUES:
        return ABSENT
    return PRESENT


# ---------------------------------------------------------------------------
# Output formatters
# ---------------------------------------------------------------------------

def render_csv(components: list[dict], osarch_list: list[str],
               osarch_platform_vars: dict[str, dict[str, str]]) -> str:
    import io
    buf = io.StringIO()
    writer = csv.writer(buf, lineterminator='\n')
    writer.writerow(["Class", "Menu"] + osarch_list)
    for comp in components:
        var = comp.get("_var", "?")
        row = [comp["class"], comp["menu"]]
        for oa in osarch_list:
            row.append(cell_value(var, osarch_platform_vars.get(oa, {})))
        writer.writerow(row)
    return buf.getvalue()


def render_markdown(components: list[dict], osarch_list: list[str],
                    osarch_platform_vars: dict[str, dict[str, str]]) -> str:
    col_w = [max(len(oa), 3) for oa in osarch_list]

    header = "| Component                 |"
    sep    = "|---------------------------|"
    for oa, w in zip(osarch_list, col_w):
        header += f" {oa:<{w}} |"
        sep    += "-" * (w + 2) + "|"
    lines = [header, sep]

    for comp in components:
        var = comp.get("_var", "?")
        row = f"| {comp['class']:<25} |"
        for oa, w in zip(osarch_list, col_w):
            v = cell_value(var, osarch_platform_vars.get(oa, {}))
            row += f" {v:<{w}} |"
        lines.append(row)

    return "\n".join(lines) + "\n"


def render_html(components: list[dict], osarch_list: list[str],
                osarch_platform_vars: dict[str, dict[str, str]]) -> str:
    def esc(s: str) -> str:
        return s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")

    rows = ["<table border='1' cellpadding='4' cellspacing='0'>", "<thead><tr>",
            "<th>Class</th><th>Menu</th>"]
    for oa in osarch_list:
        rows.append(f"<th>{esc(oa)}</th>")
    rows += ["</tr></thead>", "<tbody>"]

    for comp in components:
        var = comp.get("_var", "?")
        rows.append("<tr>")
        rows.append(f"<td>{esc(comp['class'])}</td><td>{esc(comp['menu'])}</td>")
        for oa in osarch_list:
            v = cell_value(var, osarch_platform_vars.get(oa, {}))
            colour = " style='background:#d4edda'" if v == PRESENT else ""
            rows.append(f"<td{colour}>{esc(v)}</td>")
        rows.append("</tr>")

    rows += ["</tbody></table>"]
    return "\n".join(rows) + "\n"


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main() -> None:
    parser = argparse.ArgumentParser(
        description="Generate function-block × platform support matrix.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    parser.add_argument("--repo", default=None,
                        help="Repository root path (auto-detected by default)")
    parser.add_argument("--format", action="append", dest="formats",
                        choices=["csv", "md", "html", "all"],
                        help="Output format: csv, md, html, or all. "
                             "Repeatable (e.g. --format csv --format md). "
                             "'all' produces csv+md+html. "
                             "Single format → stdout; multiple → matrix.<ext> files. "
                             "(default: csv)")
    parser.add_argument("--platform", action="append", metavar="GLOB", dest="platform_globs",
                        help="Filter platforms by glob pattern (repeatable)")
    parser.add_argument("--component", action="append", metavar="GLOB", dest="comp_globs",
                        help="Filter components by class name glob (repeatable)")
    parser.add_argument("--no-always", action="store_true",
                        help="Omit components that are always built (no support variable)")
    parser.add_argument("--no-never", action="store_true",
                        help="Omit components with no support on any shown platform")
    args = parser.parse_args()

    # Locate repo
    if args.repo:
        repo = Path(args.repo).resolve()
    else:
        repo = find_repo_root(Path(__file__).parent)

    # Discover CDFs and build support variable mapping
    all_components = parse_cdfs(repo)
    support_map = parse_components_mks(repo)

    # Collapse primitive-family variants for components without an EHS_*_SUPPORT
    # variable (core toolbox, always-built blocks).  Many CDF classes are just
    # Bool/Int/Real/String or Two/Four/Eight variants of the same functional block;
    # keep one representative per family.  HAL-selectable components (EHS_* var)
    # are kept as individual rows.
    # Resolve support var for every component, falling back to the CDF's
    # top-level subdirectory when the object-name lookup fails.
    def _resolve_var(comp: dict) -> str:
        var = support_map.get(comp["class"], "?")
        if var == "?":
            cdf_parts = Path(comp.get("cdf_path", "")).parts
            subdir = cdf_parts[2] if len(cdf_parts) >= 3 else ""
            var = _DIR_SUPPORT_FALLBACK.get(subdir, "?")
        return var

    def _var_rank(var: str) -> int:
        """Higher rank = preferred when merging family variants."""
        if var.startswith("EHS_"): return 2   # most specific
        if var == "ALWAYS":        return 1
        return 0                               # "?" — least informative

    # Collapse type/count variants into one representative row per family.
    # When the same family key appears with different support vars (e.g. a block
    # in core/ as ALWAYS and the same family in gui/ as EHS_GUI_SUPPORT), keep
    # the most specific var. Insertion order is preserved for output ordering.
    family_vars:  dict[str, str]  = {}   # fkey → best var so far
    family_comps: dict[str, dict] = {}   # fkey → representative comp
    for comp in all_components:
        var  = _resolve_var(comp)
        fkey = _family_key(comp["class"])
        if fkey not in family_vars or _var_rank(var) > _var_rank(family_vars[fkey]):
            family_vars[fkey]  = var
            family_comps[fkey] = comp

    all_components = [
        {**family_comps[fkey], "class": fkey, "_var": family_vars[fkey]}
        for fkey in family_comps
    ]

    # Apply component filter (on family keys, so both 'ICalc*' and 'ICalc' match)
    if args.comp_globs:
        all_components = [
            c for c in all_components
            if any(fnmatch.fnmatch(c["class"], g) for g in args.comp_globs)
        ]

    # Discover platforms and group by OS-architecture
    all_platforms = find_platforms(repo)
    if args.platform_globs:
        all_platforms = [
            p for p in all_platforms
            if any(fnmatch.fnmatch(p, g) for g in args.platform_globs)
        ]

    # Parse each platform config
    platform_vars: dict[str, dict[str, str]] = {
        p: parse_platform_config(repo, p) for p in all_platforms
    }

    # Build ordered list of OS-arch columns and map each to its platform(s).
    # A variable is "present" for an OS-arch if ANY platform in that group sets it.
    osarch_list: list[str] = []
    osarch_to_platforms: dict[str, list[str]] = {}
    for p in all_platforms:
        oa = _platform_osarch(p)
        if oa is None:
            continue
        if oa not in osarch_to_platforms:
            osarch_list.append(oa)
            osarch_to_platforms[oa] = []
        osarch_to_platforms[oa].append(p)

    osarch_platform_vars: dict[str, dict[str, str]] = {}
    for oa, plats in osarch_to_platforms.items():
        merged: dict[str, str] = {}
        for p in plats:
            for var, val in platform_vars.get(p, {}).items():
                if var not in merged and val.strip().lower() not in NONE_VALUES:
                    merged[var] = val
        osarch_platform_vars[oa] = merged

    # Optional filters
    if args.no_always:
        all_components = [c for c in all_components if c["_var"] != "ALWAYS"]

    if args.no_never:
        def has_support(comp: dict) -> bool:
            return any(
                cell_value(comp["_var"], osarch_platform_vars.get(oa, {})) == PRESENT
                for oa in osarch_list
            )
        all_components = [c for c in all_components if has_support(c)]

    # Resolve requested formats
    requested = args.formats or ["csv"]
    if "all" in requested:
        requested = ["csv", "md", "html"]
    # Deduplicate while preserving order
    seen_fmts: set[str] = set()
    formats: list[str] = [f for f in requested if not (f in seen_fmts or seen_fmts.add(f))]  # type: ignore[func-returns-value]

    _renderers = {
        "csv":  render_csv,
        "md":   render_markdown,
        "html": render_html,
    }

    if len(formats) == 1:
        # Single format → stdout (pipe-friendly)
        sys.stdout.write(_renderers[formats[0]](all_components, osarch_list, osarch_platform_vars))
    else:
        # Multiple formats → write to matrix.<ext> files
        for fmt in formats:
            out_path = Path(f"matrix.{fmt}")
            out_path.write_text(_renderers[fmt](all_components, osarch_list, osarch_platform_vars))
            print(f"Written: {out_path}", file=sys.stderr)


if __name__ == "__main__":
    main()
