# Software Utilities

This directory contains Python scripts for working with eRT components, CDF files, and binary data conversion.

## Table of Contents

- [CDF Component Tools](#cdf-component-tools)
  - [cdf_to_ascii.py](#cdf_to_asciipy)
  - [cdf_to_c_skel.py](#cdf_to_c_skelpy)
  - [cdf_validate.py](#cdf_validatepy)
- [Documentation Tools](#documentation-tools)
  - [move_description_column_final.py](#move_description_column_finalpy)
- [Binary Conversion Tools](#binary-conversion-tools)
  - [sodl_bin_to_c_file.py](#sodl_bin_to_c_filepy)

---

## CDF Component Tools

### cdf_to_ascii.py

**Purpose:** Generates ASCII art diagrams and markdown documentation from Component Description Files (CDF).

**Usage:**
```bash
python3 cdf_to_ascii.py <path_to_cdf_file>
```

**Behavior:**
- Parses CDF XML files to extract component metadata:
  - Component name and description
  - Parameters with types, defaults, ranges, and descriptions
  - Ports (InputPort, OutputPort, StartPort, FinishPort, InternalPort)
  - Functions and their mappings
- Generates ASCII art visualization using box-drawing characters:
  - Component name centered in the box
  - Input ports on the left side
  - Output ports on the right side
  - Start ports (event inputs) on the left
  - Finish ports (event outputs) on the right
- Outputs markdown-formatted documentation including:
  - Component description
  - ASCII diagram
  - Parameter table with all configuration options
  - Port summaries organized by type
- Handles CDF files with or without explicit Block elements (calculates dimensions from port positions as fallback)

**Output Format:**
```markdown
## Component Name

Description text...

### Diagram
```
┌─────────────────┐
│  Component_Name │
├─────────────────┤
│                 │
│                 │
└─────────────────┘
```

### Parameters
| Name | Type | Default | Min | Max | Description |
|------|------|---------|-----|-----|-------------|
...

### Ports
...
```

**Example:**
```bash
# Generate documentation for a single component
python3 cdf_to_ascii.py Common/Components/core/const_i1.cdf > component_doc.md

# Regenerate all component documentation (via Makefile)
make components_gendocs
```

**Integration:**
- Integrated into the Makefile via `components_gendocs` target
- Pattern rule automatically regenerates `.md` files when `.cdf` files change
- Output files are placed in `<component_dir>/docs/<component_name>.md`

---

### cdf_to_c_skel.py

**Purpose:** Generates skeleton C and header files from CDF specifications with proper API2 macros and incremental parsers.

**Usage:**
```bash
python3 cdf_to_c_skel.py <path_to_cdf_file>
```

**Behavior:**

1. **File Existence Handling:**
   - If `inx-<component>.c` and `inx-<component>.h` don't exist: creates them directly
   - If they already exist: creates `inx-<component>_gen.c` and `inx-<component>_gen.h` with `_gen` suffix
   - Offers to run `meld` or `diff` for comparison and manual merging

2. **Header File Generation (`inx-<component>.h`):**
   - Include guard with component name
   - Component description comment block
   - FB_NAME and FB_ID macro definitions
   - Function declarations for:
     - `EHS_FB_IDENTIFY_FUNCTION` - Component identification
     - `EHS_FB_INIT_FUNCTION` - Initialization
     - `EHS_FB_DESTROY_FUNCTION` - Cleanup
     - `EHS_FB_RUN_FUNCTION` - For each StartPort and InternalPort
   - `EHS_FB_FUNCTIONS` macro declaration
   - Special marking for InternalPort functions (callable from ISR/threads)

3. **C File Generation (`inx-<component>.c`):**
   - ICB (Interface Code Builder) macro markers for code generation compatibility
   - State structure with:
     - Actual parameter variables (not commented examples)
     - Space for user-defined state variables
   - Function table mapping event names to C functions
   - Friendly label definitions for all ports (for use in code)
   - IDENTIFY function (reports memory requirements)
   - INIT function with:
     - Incremental parser chain for parameters
     - Uses `EhsGetSint32FromString`, `EhsGetDoubleFromString`, `EhsGetUint8FromString`, `EhsGetStringFromString`
     - Each parser call chains through `pParams` pointer
   - RUN function stubs for each StartPort and InternalPort:
     - API2 macro examples for reading connected input ports
     - Placeholder for implementation logic
     - API2 macro examples for writing output ports
     - Event finish macro examples
     - Helpful notes for functions with no ports
   - DESTROY function stub

4. **Data Type Mapping:**
   - `I` (Integer) → `ehs_sint32` → `EhsGetSint32FromString`
   - `R`/`F` (Real/Float) → `ehs_double` → `EhsGetDoubleFromString`
   - `B` (Boolean) → `ehs_uint8` → `EhsGetUint8FromString`
   - `S` (String) → `ehs_char*` → `EhsGetStringFromString`

5. **Port Type Support:**
   - **StartPort** - Event-triggered functions (visual connections)
   - **InternalPort** - Functions callable from ISR/threads/external sources
   - **InputPort** - Data inputs
   - **OutputPort** - Data outputs
   - **FinishPort** - Event completion signals

6. **Build Configuration:**
   - Offers to add new components to `components.mk` and `components.c` files in the toolbox directory
   - Ensures new components are included in the build system

**API2 Macro Examples Generated:**

Reading inputs:
```c
if (EHS_FB_IN_CONNECTED_API2(INX_component_ARG_function_input_DI))
{
    ehs_sint32 value = EHS_FB_IN_I_API2(INX_component_ARG_function_input_DI);
    // Process input
}
```

Writing outputs:
```c
if (EHS_FB_OUT_CONNECTED_API2(INX_component_ARG_function_output_DO))
{
    EHS_FB_OUT_I_API2(INX_component_ARG_function_output_DO) = result_value;
}
```

Triggering events:
```c
EHS_FB_FINISH(INX_component_ARG_function_done_EO);
```

**Incremental Parser Pattern:**
```c
const char* pParams = EHS_FB_INIT_PARAMETERS;
pParams = EhsGetSint32FromString(&(state->frequency), pParams);
pParams = EhsGetSint32FromString(&(state->channel), pParams);
pParams = EhsGetDoubleFromString(&(state->gain), pParams);
```

**Example:**
```bash
# Generate skeleton for a new component
python3 cdf_to_c_skel.py Common/Components/peripherals/inx-pwm_gen.cdf

# Output:
# Created: Common/Components/peripherals/inx-pwm_gen.h
# Created: Common/Components/peripherals/inx-pwm_gen.c
# Add to components.mk? [y/N]: y
# Updated: Common/Components/peripherals/components.mk
```

**Important Notes:**
- Generated code includes ICB markers (`//ICB ... MACRO START/END`) - do not modify these markers
- Add custom implementation between the markers where indicated
- State variables and initialization code can be added in designated sections
- The skeleton provides a complete, compilable starting point

---

### cdf_validate.py

**Purpose:** Validates consistency between CDF files and C implementation code, ensuring function IDs match.

**Usage:**
```bash
# Validate all components in the default directory
python3 cdf_validate.py

# Compare two CDF directory versions
python3 cdf_validate.py <new_cdf_dir> <old_cdf_dir>
```

**Behavior:**

1. **Function ID Validation Mode (default):**
   - Scans `./Common/Components` for CDF files and C implementations
   - Parses CDF files to extract:
     - Component class names
     - Function names and ERT1_ID mappings
   - Parses C files to find:
     - `EHS_FB_FUNCTIONS_START` blocks
     - `EHS_FB_FUNCTION_ENTRY` mappings
   - Maps CDF class names to FB identify names via header files
   - Compares function ID mappings between CDF and C code
   - Reports discrepancies:
     - `DIFFERENT` - Function IDs don't match between CDF and C
     - `EMPTY` - C code has no function entries for CDF component
     - `Failed to find` - Missing header file or C implementation

2. **CDF Port Comparison Mode:**
   - Compares port configurations between two CDF directory versions
   - Validates port types: InputPort, OutputPort, StartPort, FinishPort, InternalPort
   - Checks port argument indices and IDs
   - Reports changes in port configurations

**Detection Capabilities:**
- Missing C implementations for CDF files
- Function ID mismatches between specification and code
- Missing or extra functions in implementations
- Port configuration changes between versions
- Orphaned CDF files without implementations

**Example Output:**
```
Validating function ID and c code in this directory=./Common/Components
DIFFERENT ( pwm_gen )= {'setup': '1', 'enable': '2'} | cdf( pwm_gen )= {'setup': '2', 'enable': '1'}
Failed to find fb_identify_name for  legacy_component
EMPTY ( new_component ), cdf( new_component )= {'init': '1', 'run': '2'}
```

**Use Cases:**
- Pre-commit validation to ensure CDF and C code stay synchronized
- Detecting regressions when updating component definitions
- Finding components that need implementation updates after CDF changes
- Migration validation when updating component versions

---

## Documentation Tools

### move_description_column_final.py

**Purpose:** Reformats HTML help files for components by moving the description column in parameter tables.

**Usage:**
```bash
python3 move_description_column_final.py
```

**Behavior:**
- Searches for all `help.html` files in `Common/Components/**/docs/` directories
- Uses BeautifulSoup to parse HTML structure
- Finds parameter tables (class `table_params`)
- Moves the description column from the last position (6th column) to the 3rd position
- Reorders both header cells and data cells
- Creates automatic backups before modification (`.bak` files)
- Restores from backup if any error occurs
- Removes backup files after successful modification

**Table Transformation:**

Before:
```
| Name | Type | Default | Min | Max | Description |
```

After:
```
| Name | Type | Description | Default | Min | Max |
```

**Safety Features:**
- Automatic backup creation before modification
- Automatic restoration on error
- Backup cleanup on success
- Error reporting for each file processed

**Example Output:**
```
Found 42 help.html files
Processing Common/Components/core/const_i1/docs/help.html...
Modified Common/Components/core/const_i1/docs/help.html
Processing Common/Components/core/buffer_fifoiiq/docs/help.html...
Modified Common/Components/core/buffer_fifoiiq/docs/help.html
...
Successfully processed 42 out of 42 files
```

**Use Cases:**
- Reformatting documentation after bulk generation
- Standardizing table layouts across all component help files
- Updating documentation format for IDE integration

---

## Binary Conversion Tools

### sodl_bin_to_c_file.py

**Purpose:** Converts binary files to C source files containing byte arrays, suitable for embedding binary data in eRT applications.

**Usage:**
```bash
python3 sodl_bin_to_c_file.py <input_binary_file> <output_c_file> [array_name]
```

**Arguments:**
- `input_binary_file` - Path to the binary file to convert
- `output_c_file` - Path where the C source file will be written
- `array_name` - (Optional) Name for the C array (default: `sodl_binary_data`)

**Behavior:**
- Reads binary file in binary mode
- Converts each byte to hexadecimal format (`0xXX`)
- Generates C source file with:
  - Include for `target_types.h`
  - `const ehs_char` array declaration
  - Formatted hex data (16 bytes per line for readability)
  - Size constant for the array
- Does NOT add null terminator (commented out in code)
- Calculates size as `sizeof(array) - 1`

**Generated C File Format:**
```c
#include "target_types.h"

const ehs_char sodl_binary_data[] = {
    0x7F, 0x45, 0x4C, 0x46, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x3E, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x10, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
    ...
};

const ehs_uint32 sodl_binary_data_size = sizeof(sodl_binary_data) - 1;
```

**Example:**
```bash
# Convert firmware binary to C array
python3 sodl_bin_to_c_file.py firmware.bin firmware_data.c

# With custom array name
python3 sodl_bin_to_c_file.py config.dat embedded_config.c config_data
```

**Use Cases:**
- Embedding firmware images in bootloader code
- Including configuration data directly in compiled binaries
- Storing font data, images, or other resources
- Creating self-contained executables with embedded assets
- SODL (System Object Description Language) binary embedding

**Output:**
```
Successfully wrote binary data to firmware_data.c (null-terminated)
```

**Error Handling:**
- Reports missing input files
- Catches and reports general exceptions
- Provides usage information if arguments are missing

---

## Integration with Build System

Several of these scripts are integrated into the project's Makefile:

**Component Documentation:**
```bash
make components_gendocs  # Regenerate all .md files from CDF files
```

**Automatic Regeneration:**
- The Makefile includes pattern rules to regenerate `.md` files when their corresponding `.cdf` files change
- Ensures documentation stays synchronized with component definitions

---

## Development Workflow

### Creating a New Component

1. **Create CDF file** defining the component interface
   ```bash
   # Create Common/Components/category/inx-my_component.cdf
   ```

2. **Generate documentation** to preview the component
   ```bash
   python3 scripts/software-utilities/cdf_to_ascii.py Common/Components/category/inx-my_component.cdf > preview.md
   ```

3. **Generate skeleton code**
   ```bash
   python3 scripts/software-utilities/cdf_to_c_skel.py Common/Components/category/inx-my_component.cdf
   # Answer 'y' to add to components.mk
   ```

4. **Implement functionality** in the generated C file
   - Add state variables
   - Implement RUN functions
   - Add initialization/cleanup code

5. **Validate** CDF matches implementation
   ```bash
   python3 scripts/software-utilities/cdf_validate.py
   ```

### Updating an Existing Component

1. **Modify CDF file** with new parameters or functions

2. **Generate updated skeleton** with `_gen` suffix
   ```bash
   python3 scripts/software-utilities/cdf_to_c_skel.py Common/Components/category/inx-my_component.cdf
   # Creates inx-my_component_gen.c and inx-my_component_gen.h
   ```

3. **Use meld/diff** to merge changes into existing implementation
   ```bash
   # The script offers to run this automatically
   meld Common/Components/category/inx-my_component.c Common/Components/category/inx-my_component_gen.c
   ```

4. **Validate** the updates
   ```bash
   python3 scripts/software-utilities/cdf_validate.py
   ```

5. **Regenerate documentation**
   ```bash
   make components_gendocs
   ```

---

## Requirements

**Python Dependencies:**
- `xml.etree.ElementTree` (standard library)
- `beautifulsoup4` (for `move_description_column_final.py`)
  ```bash
  pip install beautifulsoup4
  ```

**System Dependencies:**
- `meld` or `diff` (optional, for code comparison in `cdf_to_c_skel.py`)

---

## File Naming Conventions

- **CDF files:** `inx-<component_name>.cdf`
- **Header files:** `inx-<component_name>.h`
- **Implementation files:** `inx-<component_name>.c`
- **Generated files:** `inx-<component_name>_gen.{c,h}` (when originals exist)
- **Documentation:** `<component_name>/docs/<component_name>.md`

---

## Related Documentation

- **CDF Format:** See `Common/Components/cdf-schema.xsd` for XML schema
- **Component Development:** See `docs/ert-porting-guide.md` for full development guide
- **Build System:** See `CLAUDE.md` and `Makefile` for build integration details
