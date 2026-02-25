#!/usr/bin/env python3
"""
extract-md-images.py

Extract base64-encoded images embedded in a Markdown file and write them as
individual image files (PNG, JPEG, SVG, etc.).

Handles reference-style image definitions of the form:

    [imageN]: <data:image/png;base64,....>

Usage:
    python3 extract-md-images.py <markdown-file> [output-dir]

Arguments:
    markdown-file   Path to the source .md file.
    output-dir      Directory to write extracted images into.
                    Defaults to the same directory as the markdown file.

Output filenames are derived from the reference key (e.g. [image1] →
image1.png).  An optional name-map dictionary near the top of this script
can be edited to give more descriptive names.
"""

import re
import base64
import os
import sys


# ---------------------------------------------------------------------------
# Optional: map reference keys to descriptive stem names.
# Add entries here when extracting from a specific document.
# Keys not listed here fall back to the raw reference key as the filename stem.
# ---------------------------------------------------------------------------
NAME_MAP: dict[str, str] = {
    # inxware-edge-ml.md
    "image1": "edge-ml-yolo8-pose-hailo-model-explorer",
    "image2": "edge-ml-ml-data-formats-workflow",
    "image3": "edge-ml-tflite-runtime-diagram",
    "image4": "edge-ml-cmsis-inference-diagram",
    "image5": "edge-ml-nxp-eiq-diagram",
    "image6": "edge-ml-gguf-llm-format-diagram",
}

# Regex: matches  [key]: <data:image/<fmt>;base64,<data>>
IMAGE_REF_RE = re.compile(
    r'\[(\w+)\]:\s*<data:image/(png|jpeg|svg\+xml|gif|webp);base64,([A-Za-z0-9+/=\s]+)>'
)


def ext_for_fmt(fmt: str) -> str:
    mapping = {"jpeg": "jpg", "svg+xml": "svg"}
    return mapping.get(fmt, fmt)


def extract(md_path: str, out_dir: str) -> None:
    os.makedirs(out_dir, exist_ok=True)

    with open(md_path, "r", encoding="utf-8") as fh:
        content = fh.read()

    matches = list(IMAGE_REF_RE.finditer(content))
    if not matches:
        print("No embedded base64 images found.")
        return

    for m in matches:
        key = m.group(1)
        fmt = m.group(2)
        b64data = m.group(3).replace("\n", "").replace("\r", "").strip()

        ext = ext_for_fmt(fmt)
        stem = NAME_MAP.get(key, key)
        out_path = os.path.join(out_dir, f"{stem}.{ext}")

        raw = base64.b64decode(b64data)
        with open(out_path, "wb") as fh:
            fh.write(raw)

        print(f"{key} -> {os.path.basename(out_path)} ({len(raw):,} bytes)")

    print(f"\nExtracted {len(matches)} image(s) to: {out_dir}")


def main() -> None:
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    md_path = sys.argv[1]
    if not os.path.isfile(md_path):
        print(f"Error: file not found: {md_path}")
        sys.exit(1)

    out_dir = sys.argv[2] if len(sys.argv) >= 3 else os.path.dirname(os.path.abspath(md_path))

    extract(md_path, out_dir)


if __name__ == "__main__":
    main()
