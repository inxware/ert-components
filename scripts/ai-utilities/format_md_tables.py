#!/usr/bin/env python3
"""
format_md_tables.py — Reformat markdown tables with p90-capped column padding.

Column width rule (per CLAUDE.md convention):
    column_width = max(len(header), p90_data_width + BUFFER)

Where:
    p90_data_width  = 90th-percentile cell content length across all data rows
                      (for tables with < 10 data rows, the second-longest cell
                      is used instead to avoid single outliers dominating)
    BUFFER          = added characters on top of p90 (default: 5, range 1–10)

Cells shorter than column_width are padded with trailing spaces.
Cells longer than column_width are left as-is (they overflow the column
boundary — this is intentional; we do not let one outlier stretch every
other row).

Usage:
    python3 format_md_tables.py <file.md> [<file2.md> ...]
    python3 format_md_tables.py <file.md> --buffer 8   # custom buffer
    python3 format_md_tables.py <file.md> --dry-run    # print to stdout only

Multiple files are each reformatted in-place.
"""

import sys
import re
import math
import argparse
from pathlib import Path


# ---------------------------------------------------------------------------
# Core formatting logic
# ---------------------------------------------------------------------------

def cell_content(cell: str) -> str:
    """Return the stripped content of a cell (no surrounding spaces/pipes)."""
    return cell.strip()


def is_table_row(line: str) -> bool:
    return line.lstrip().startswith("|")


def is_separator_row(line: str) -> bool:
    """Return True if every cell contains only dashes (and optional colons)."""
    stripped = line.strip()
    if not stripped.startswith("|"):
        return False
    inner = stripped.strip("|")
    cells = inner.split("|")
    return all(re.fullmatch(r"\s*:?-+:?\s*", c) for c in cells)


def parse_row(line: str) -> list[str]:
    """Split a table row into cell content strings."""
    stripped = line.strip()
    # Remove leading and trailing pipe, then split on |
    if stripped.startswith("|"):
        stripped = stripped[1:]
    if stripped.endswith("|"):
        stripped = stripped[:-1]
    return [c.strip() for c in stripped.split("|")]


def p90_or_second_largest(lengths: list[int]) -> int:
    """
    Return the 90th-percentile length for large samples, or the
    second-largest value for small samples (< 10 rows).
    """
    if not lengths:
        return 0
    lengths_sorted = sorted(lengths)
    n = len(lengths_sorted)
    if n < 10:
        # For small tables avoid letting one outlier dominate
        return lengths_sorted[-2] if n >= 2 else lengths_sorted[-1]
    idx = math.ceil(0.90 * n) - 1
    return lengths_sorted[min(idx, n - 1)]


def compute_column_widths(header_cells: list[str],
                           data_rows: list[list[str]],
                           buffer: int = 5) -> list[int]:
    """
    Compute capped column widths for a table.

    Returns a list of widths, one per column.
    """
    n_cols = len(header_cells)
    widths = []
    for col in range(n_cols):
        header_w = len(header_cells[col])
        data_lengths = []
        for row in data_rows:
            if col < len(row):
                data_lengths.append(len(row[col]))
            else:
                data_lengths.append(0)
        p90 = p90_or_second_largest(data_lengths) if data_lengths else 0
        col_w = max(header_w, p90 + buffer)
        widths.append(col_w)
    return widths


def format_cell(content: str, width: int) -> str:
    """Pad cell content to width (or leave as-is if it overflows)."""
    if len(content) >= width:
        return f" {content} "
    return f" {content}{' ' * (width - len(content))} "


def format_separator(width: int) -> str:
    return f" {'-' * width} "  # note: one space each side keeps alignment


def build_row(cells: list[str], widths: list[int], n_cols: int) -> str:
    parts = []
    for col in range(n_cols):
        content = cells[col] if col < len(cells) else ""
        w = widths[col] if col < len(widths) else len(content)
        parts.append(format_cell(content, w))
    return "|" + "|".join(parts) + "|"


def build_separator(widths: list[int]) -> str:
    parts = [format_separator(w) for w in widths]
    return "|" + "|".join(parts) + "|"


# ---------------------------------------------------------------------------
# Table detection and reformatting
# ---------------------------------------------------------------------------

def reformat_tables(text: str, buffer: int = 5) -> str:
    lines = text.splitlines(keepends=True)
    output = []
    i = 0

    while i < len(lines):
        line = lines[i]

        # Detect start of a table: a pipe row followed by a separator row
        if (is_table_row(line)
                and i + 1 < len(lines)
                and is_separator_row(lines[i + 1])):

            # Collect the full table block
            table_lines = []
            j = i
            while j < len(lines) and is_table_row(lines[j]):
                table_lines.append(lines[j].rstrip("\n").rstrip("\r"))
                j += 1

            # Parse header and data rows (skip separator rows)
            header_cells = parse_row(table_lines[0])
            n_cols = len(header_cells)

            data_rows = []
            for tl in table_lines[2:]:   # skip header [0] and separator [1]
                if not is_separator_row(tl):
                    data_rows.append(parse_row(tl))

            widths = compute_column_widths(header_cells, data_rows, buffer)

            # Emit reformatted table
            nl = "\n"
            output.append(build_row(header_cells, widths, n_cols) + nl)
            output.append(build_separator(widths) + nl)
            for tl in table_lines[2:]:
                if is_separator_row(tl):
                    output.append(build_separator(widths) + nl)
                else:
                    row_cells = parse_row(tl)
                    output.append(build_row(row_cells, widths, n_cols) + nl)

            i = j  # skip past the table block

        else:
            output.append(line)
            i += 1

    return "".join(output)


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Reformat markdown tables with p90-capped column padding."
    )
    parser.add_argument("files", nargs="+", metavar="FILE",
                        help="Markdown file(s) to reformat")
    parser.add_argument("--buffer", type=int, default=5, metavar="N",
                        help="Characters to add on top of p90 width (default: 5, range 1-10)")
    parser.add_argument("--dry-run", action="store_true",
                        help="Print reformatted content to stdout instead of writing")
    args = parser.parse_args()

    buffer = max(1, min(10, args.buffer))

    for path_str in args.files:
        path = Path(path_str)
        if not path.exists():
            print(f"ERROR: {path} not found", file=sys.stderr)
            continue

        original = path.read_text(encoding="utf-8")
        reformatted = reformat_tables(original, buffer=buffer)

        if args.dry_run:
            print(f"# --- {path} ---")
            print(reformatted)
        else:
            if reformatted != original:
                path.write_text(reformatted, encoding="utf-8")
                print(f"Reformatted: {path}")
            else:
                print(f"No changes:  {path}")


if __name__ == "__main__":
    main()
