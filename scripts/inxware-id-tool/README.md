# inxware-id-tool — eRT Component ID and Hash Scripts

Tools for generating, updating, and validating hash-based identifiers for eRT function blocks.

All scripts are in `scripts/inxware-id-tool/` and should be run from the **repo root**.

---

## Scripts

### `inxtool.py` — main entry point

Generate a hash for a class name, or batch-update ID defines across source files.

```bash
# Generate NameHash_CRC16 for a new function block class name:
python3 scripts/inxware-id-tool/inxtool.py -genHash "<class_name>" -hash 16CRC

# Batch-update EHS_FB_ID_* defines in .c/.h files (dry run):
python3 scripts/inxware-id-tool/inxtool.py -d <dir> -m updateid -hash 16CRC -e c,h

# Apply changes:
python3 scripts/inxware-id-tool/inxtool.py -d <dir> -m updateid -hash 16CRC -e c,h -apply
```

**Options:**

| Flag | Description |
|---|---|
| `-genHash <str>` | Print the hash of the given string and exit |
| `-hash <type>` | Hash algorithm: `16CRC` (CRC-16/Modbus) or `32CRC` (CRC-32) |
| `-d <dir>` | Root directory to walk for batch processing |
| `-m <mode>` | Processing mode (see table below) |
| `-e <exts>` | Comma-separated file extensions, default `c,h` |
| `-apply` | Write changes to files (default is dry run) |
| `-keep` | Emit `#if ERT_SODL_VERSION` guards around old and new values |
| `-rf <file>` | Write processing report to file instead of stdout |

**Modes (`-m`):**

| Mode | What it matches and replaces |
|---|---|
| `updateid` | `#define EHS_FB... "string"` — replaces string with CRC hash |
| `updatefooid` | `EHS_FB_FUNCTION_ENTRY("name", ...)` — replaces name string with hash |
| `updatefoonewid` | Same as above but generates a `FUNCTION_NAME_ID_NAME` alias |
| `updateinxwareid` | `#define INXWARE_FB... "string"` — replaces string with CRC hash |
| `update_hash_only` | Updates existing `0xXXXX` values in place (reads adjacent string line for input) |

### `hash.py` — hash algorithm implementations

Not called directly. Provides `Hash16CRC` (CRC-16/Modbus) and `Hash32CRC` (Python `binascii.crc32`) used by `inxtool.py`.

### `argumentparser.py` — CLI argument parser

Not called directly. Parses the command-line arguments for `inxtool.py`.

### `configurations.py` — default settings

Not called directly. Holds the default configuration values (mode, extensions, hash type, etc.) used by `inxtool.py`.

### `converter.py` — file-walking and processing orchestrator

Not called directly. Reads files, calls `processline.py` for each line, and writes results back.

### `processline.py` — line-level processing logic

Not called directly. Contains the `ProcessLine` subclasses that match and transform individual lines for each mode.

### `check_cdf_hashes.py` — NameHash_CRC16 regression checker

Verifies that every `.cdf` file's `<NameHash_CRC16>` matches the CRC-16/Modbus of its `<Class>` name, and that any corresponding `INXWARE_FB_ID_*` or `EHS_FB_ID_*` macro in a `.h` file matches too. Exits non-zero if any mismatch is found — suitable for CI.

```bash
python3 scripts/inxware-id-tool/check_cdf_hashes.py                  # checks Common/Components
python3 scripts/inxware-id-tool/check_cdf_hashes.py <other/path>     # checks a different root
```

### `cdf_validate.py` — CDF vs C function ID validator

Checks that function IDs declared in `.cdf` files match those in the corresponding C implementation (`EHS_FB_FUNCTIONS_START`/`EHS_FB_FUNCTION_ENTRY` blocks).

```bash
python3 scripts/inxware-id-tool/cdf_validate.py
# Runs against ./Common/Components by default
```

---

## Generating a `NameHash_CRC16` for a new block

```bash
python3 scripts/inxware-id-tool/inxtool.py -genHash "<class_name>" -hash 16CRC
```

Output is 4 uppercase hex digits (e.g. `F512`). Prepend `0x` and write to **two places**:

1. **CDF** `<Hashes>` element:
   ```xml
   <Hashes>
       <NameHash_CRC16>0xF512</NameHash_CRC16>
       <FbApiDescriptorHash_CRC32>00000000</FbApiDescriptorHash_CRC32>
       <FbApiDescriptorHash/>
   </Hashes>
   ```

2. **`.h` header** macro:
   ```c
   #define INXWARE_FB_ID_my_block_class  0xF512
   ```

**Verified values:**

| Class name | NameHash_CRC16 |
|---|---|
| `adc_config` | `0x566F` |
| `accel_gyro` | `0xF2AA` |
| `lorawan` | `0xC89A` |
| `rtc` | `0x32C7` |
| `led` | `0xA6EA` |
| `ml_image_inference` | `0xF512` |

## `FbApiDescriptorHash_CRC32` — always `00000000`

This field is not computed by any script in this directory and is marked `@TODO` in Lucid IDE source. Existing CDFs with non-zero values had them set by older IDE versions. Leave it as `00000000` for any manually created or edited CDF.
