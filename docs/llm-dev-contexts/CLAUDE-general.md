# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Prompt Logging

At the start of each conversation, log user prompts to `../claude-prompts-ert-components/prompts.log`. For each substantive user prompt (not just ".", "yes", "no", or similar short confirmations), append an entry in this format:

```
=== YYYY-MM-DD HH:MM ===
<prompt text>

```

Create the log file if it doesn't exist. This helps track work history across sessions.

## Project Overview

This is the **ert-components** repository, which contains the core components needed for the inxware eRT (event-based RunTime) system. eRT is designed to run no-code applications on embedded devices and various computing systems including servers, edge compute, and desktop platforms.

## Build System

The project uses a sophisticated Make-based build system with Docker support for cross-platform compilation.

### Key Commands

**Configuration:**
- `./configure` - List available build targets
- `./configure [target]` - Configure for specific platform (e.g., `linux_x86_64_clang`)
- `./configure -run` - Run the target on current host
- `./configure -debug` - Debug the target with GDB

**Building:**
- `make help` - Show all available build targets and options
- `make prepdeps` - **One-time initialisation** for a new target or fresh checkout. Clones/updates
  `ert-build-support` (toolchains) and `ert-contrib-middleware` (pre-built libraries), installs
  Docker and other host tools if missing, and prints any target-specific prerequisites (e.g. for
  XMOS targets it checks whether the XTC Tools archive has been downloaded and gives step-by-step
  instructions if not). Re-run when switching to a new target platform. Do not run on every build.
- `make build_docker_local` - Build the platform's Docker image locally from its `Dockerfile`.
  Required once before `make all_docker` for targets whose Docker image contains proprietary
  toolchain binaries that cannot be pushed to a public registry (e.g. XMOS xcore targets).
- `make all` - Build the eRT binary directly on the host. Only works if the host has all required
  dependencies installed; use `make all_docker` for cross-compilation targets.
- `make all_docker` - Build inside the platform's Docker container. Reads `Dockerimagename` from
  the platform directory; pulls from DockerHub or uses a locally built image. The reliable option
  for all cross-compilation targets.
- `make clean` - Clean build artifacts

**Runtime Environment:**
- `make targetenv` - Create runtime file structure in staging directory
- `make targetenv_version` - Create new version and tag commit
- `make targetenv_package` - Create target-specific package
- `make targetenv_deb` - Create Debian package (Linux targets)
- `make targetenv_apk` - Create Android APK
- `make targetenv_esp32` - Build ESP32 firmware image

**Testing:**
- `make targetenv_run_tests` - Run regression tests
- `make static_analysis` - Run static analysis suite

**Development:**
- `make depend` - Update source dependencies (WARNING: modifies deps.mk files)
- `make toolsenv_update` - Update development tools with new component descriptions

### Build Dependencies

The build system requires several adjacent repositories:
- `ert-build-support` - Binary toolchains and build dependencies
- `ert-contrib-middleware` - Pre-built 3rd party dependencies
- `apps` - is optional and contains Lucid app projects that might be pre-installed in the build.
These are automatically cloned by `make prepdeps` and require ~40GB of disk space.

### Running the Build Product

After building, use `./configure -run` to create the runtime environment and launch the application in one step:

```bash
make all              # Build on host (may not work on all systems)
# OR
make all_docker       # Build in Docker (always works once Docker env is set up)

./configure -run      # Create targetenv and run the application
```

`./configure -run` is the preferred way to run. It handles `make targetenv` and launching `ehs.exe` from the correct working directory automatically. Do NOT attempt to run `ehs.exe` directly without `./configure -run`.

### Runtime Environment (TARGET_TREES)

The runtime environment is a staging directory located at `../TARGET_TREES/ehs_env-<TARGET>/` relative to the `ert-components` directory. The `<TARGET>` name comes from `TARGET.cfg` in the project root (e.g., `linux_x86_64_qt_debian12-no-certs`).

**Deriving the runtime path:**
1. Read `TARGET.cfg` to get the target name
2. The runtime tree is at `../TARGET_TREES/ehs_env-<TARGET>/`

**Runtime directory structure:**
```
../TARGET_TREES/ehs_env-<TARGET>/
├── bin/                    # Executable and runtime scripts
│   ├── ehs.exe             # The built eRT binary
│   ├── app.qml             # Qt QML UI definition (Qt targets only)
│   ├── corelib/            # Core library files
│   └── *.sh                # Runtime control scripts
├── appdata/default/        # Application data files
│   ├── app.qml             # QML UI file (Qt targets)
│   ├── *.gui               # Widget layout files (parsed by guiparams.c)
│   ├── *.bdf               # Font files
│   ├── *.sdl               # SODL application definition
│   └── *.png               # Image assets
├── sysdata/                # System data
├── userdata/               # User data
└── install/                # Installation files
```

**Build utilities for investigating the runtime environment:**
- `./configure -run` — Build and run the application
- `./configure -debug` — Run under GDB with source path substitution
- `./configure -pushd` — Open a shell inside the runtime environment directory (type `exit` to return)
- `./configure -pushd-config` — Open a shell in the platform config directory

**Important runtime files for debugging:**
- `appdata/default/*.gui` — Widget parameter files. These contain the widget names, positions, colours, and other properties parsed (which may be ignored for QT apps) `appdata/default/*.sdl` — The SODL application file defining the component graph.
- `bin/app.qml` — Copy of the QML file used at runtime.

**Docker Image Management:**
- `make publish_docker_image` - Build the Docker image for the current target and push it to the inxware Docker Hub organisation. Run this after any change to the target's `Dockerfile`. Everyone using the repo then gets the updated image automatically on next `make all_docker`.
- `make build_docker_local` - Build the Docker image locally without pushing to the registry. Use this to test Dockerfile changes before publishing.
- `make target_buildenv` - Start an interactive shell inside the Docker container for the current target. Useful for debugging build environment issues.

The Docker image name for each target is defined in `target/platform/<TARGET>/Dockerimagename`.

**Third-party SDKs in Docker images (HailoRT, TensorRT, etc.):**
Some targets depend on vendor SDKs that cannot be redistributed in the repo. The pattern is:
1. Place the SDK package/headers in `temp/<SDK>/` in the repo root (gitignored via `temp/<SDK>/.gitignore`).
2. The `Dockerfile` `COPY`s from `temp/<SDK>/` at build time.
3. Run `make publish_docker_image` — the resulting image has the SDK baked in.
4. All other developers use the published image and never need the SDK locally.
Only the person running `publish_docker_image` needs the vendor SDK files.

**Additional Useful Make Targets:**
- `make chkconfig` - Show current platform configuration dependencies
- `make sbom` - Generate Software Bill of Materials for the current platform (see below)

**Software Bill of Materials (SBOM):**
`make sbom` produces three outputs for the current `TARGET`:

1. **`sbom/$(TARGET)/SBOM.spdx`** — Forward SBOM in SPDX 2.3 tag-value format listing all external
   dependencies (ert-contrib-middleware, ert-build-support, toolchain). Suitable for supply-chain tooling.
2. **`sbom/$(TARGET)/SBOM_SUMMARY.md`** — Human-readable markdown table of dependencies with
   on-disk presence check. Quick way to verify a build environment has all required repos.
3. **`<dep-root>/DEPENDENTS.md`** — Reverse index appended to each dependency root in the sibling
   repos (`../ert-build-support`, `../ert-contrib-middleware`). Records which platforms consume
   that dependency. Run `make sbom` across all platforms to build a complete usage matrix.

The `sbom/` output directory is gitignored in ert-components. The `DEPENDENTS.md` files live in and
should be committed to the sibling repos. The SBOM logic is in `target/platform/sbom.mk` and is
conditionally included by the root `Makefile` only when `make sbom` is invoked.

## Architecture

### Directory Structure

- **Common/** - Core eRT components and libraries
  - **Components/** - Reusable functional components organized by category
    - **core/** - Essential components (operators, buffers, converters, etc.)
    - **gui/** - Graphics and UI components  
    - **networking/** - Network communication components
    - **media/** - Audio/video processing components
    - **ml/** - Machine learning components
    - **mv/** - Machine vision components
  - **HAL/** - Hardware Abstraction Layer
  - **KAPI/** - Kernel API headers and interfaces
  - **Ehs/** - Event handling system core

- **target/** - Platform-specific build configurations
  - **platform/** - Target configuration files (config.mk per platform)
  - **os-arch/** - OS and architecture specific code
  - **envtree/** - Runtime environment templates
  - **Component-HAL/** - Hardware abstraction components

### Component System

eRT uses a component-based architecture where functionality is provided through:
- **CDF files** (Component Description Files) - XML-based component interface definitions
- **C/C++ implementations** - Actual component logic with state management and port handlers
- **Documentation** - Markdown files with ASCII diagrams (generated from CDF)

**Note:** Bitmap files are no longer used for visual representation.

Components are categorized into functional groups (core, networking, gui, etc.) and can be combined to create applications.

#### CDF File Format

CDF (Component Description File) files are XML-based descriptions located in `Common/Components/`. They define:

**Schema Location:** `Common/Components/cdf-schema.xsd`

**Key Elements:**
- **Module** - Component module name (typically "usercomponents")
- **CDFInfo** - Version and timestamp metadata
- **Description** - Component description and menu categorization
- **Block** - Visual representation (type, dimensions, text, position)
- **FBID** - Functional block identifier (ERT1_ID, Class)
- **Hashes** - CRC checksums for component identification. See [Function Block ID Generation](#function-block-id-generation) below for how to compute these.
- **Parameters** - Configurable values with types, defaults, min/max ranges
- **Functions** - Named operations the component performs
- **Ports** - Connection points for data and events

**Data Types:**
- `I` - Integer (32-bit signed)
- `R` - Real/Float
- `B` - Boolean
- `S` - String

**Port Types:**
- `InputPort` - Data inputs (left side of block)
- `OutputPort` - Data outputs (right side of block)
- `StartPort` - Event triggers to start function execution (left side)
- `FinishPort` - Event completions when function finishes (right side)
- `InternalPort` - Internal event handlers callable from ISR/threads
  - **Convention:** InternalPorts use coordinates `<XCoordinate>-1</XCoordinate><YCoordinate>-1</YCoordinate>`
  - These ports are not shown visually in the block diagram
  - Used for callbacks from hardware/stack layers (e.g., BLE connection events, interrupts)

**Block Types:**
- `Data_Processor` - Data processing components
- `Event_Processor` - Event handling components
- `IO` - Input/output peripheral components
- `Computation` - Mathematical/computational components
- `Communication` - Network communication components

**Generating CDF Files:**
New CDF files can be generated from natural language specifications by following the XML schema structure. The schema validates component structure and ensures compatibility with the eRT IDE and runtime system.

> **See also:** [`CLAUDE-function-blocks.md`](CLAUDE-function-blocks.md) for precise rules on port `argument=` numbering (the one-OutputPort-per-argument rule, the confirmed arg=1/arg=2 pattern, help file conventions, and worked examples).

#### Component Implementation Structure

Each component consists of three files in `Common/Components/[category]/`:

1. **`inx-[component].cdf`** - XML component definition (interface specification)
2. **`inx-[component].h`** - C header with function declarations
3. **`inx-[component].c`** - C implementation with:
   - State structure (`inx_[component]_state_type`) containing parameters and internal state
   - Function table mapping function names to ERT1_IDs
   - Friendly label macros for port access (`INX_[component]_ARG_[function]__[DI|DO|EO]`)
   - Lifecycle functions: `IDENTIFY`, `INIT`, `DESTROY`
   - Run functions: One per CDF Function element
   - Port access via API2 macros: `EHS_FB_IN_I_API2()`, `EHS_FB_OUT_S_API2()`, etc.
   - Event triggering via `EHS_FB_FINISH(port_label)`

**Component Documentation:**
- Location: `Common/Components/[category]/[component]/docs/[component].md`
- Generated using: `python3 scripts/software-utilities/cdf_to_ascii.py [cdf_file] > [output.md]`
- Contains ASCII block diagram showing ports, parameters, and port-to-function mappings

**Example C Implementation Pattern:**
```c
// State structure with parameters
typedef struct {
    ehs_char* param_name;
    ehs_sint32 param_value;
    // Internal state variables
} inx_component_state_type;

// Function table
EHS_FB_FUNCTIONS_START(component)
EHS_FB_FUNCTION_ENTRY("init", 0x1, component_init)
EHS_FB_FUNCTION_ENTRY("process", 0x2, component_process)
EHS_FB_FUNCTIONS_END

// Run function implementation
EHS_FB_RUN_FUNCTION(component_process)
{
    inx_component_state_type* state = (inx_component_state_type*)EHS_FB_RUN_CONTEXT;

    // Read input port
    if (EHS_FB_IN_CONNECTED_API2(INX_component_ARG_process__DI))
    {
        ehs_sint32 input = EHS_FB_IN_I_API2(INX_component_ARG_process__DI);
        // Process input...
    }

    // Write output port
    if (EHS_FB_OUT_CONNECTED_API2(INX_component_ARG_process__DO))
    {
        EHS_FB_OUT_I_API2(INX_component_ARG_process__DO) = result;
    }

    // Trigger finish event
    EHS_FB_FINISH(INX_component_ARG_process__EO);
}
```

### Build Targets

The system supports numerous build targets including:
- Linux (x86, ARM, x86_64) with various GUI toolkits
- Android (ARM, ARM64) 
- Windows (x86)
- ESP32/ESP32-S3 (FreeRTOS)
- Arduino platforms
- Bare metal embedded systems

Each target has its own configuration in `target/platform/[target_name]/`.

## Development Workflow

### Creating New Components

1. **Design:** Define component interface (functions, ports, parameters)
2. **Generate CDF:** Create XML component definition following schema
3. **Generate Documentation:** Run `python3 scripts/software-utilities/cdf_to_ascii.py Component.cdf > docs/component.md`
4. **Create Header:** Define function declarations in `inx-component.h`
5. **Implement:** Write component logic in `inx-component.c`
6. **Test Compilation:** Run `make Common/Components/[category]/inx-component.o`
7. **Integration Test:** Build full runtime and test with target application

### Building and Testing

1. **Setup:** Run `./configure [target]` and `make prepdeps`
2. **Build:** Use `make all` or `make all_docker`
3. **Test:** Create runtime with `make targetenv`, then `make targetenv_run_tests`
4. **Package:** Use target-specific packaging commands (`make targetenv_deb`, etc.)

### Development Tools

**`scripts/software-utilities/cdf_to_ascii.py`** - CDF visualization and documentation generator
- Generates ASCII block diagrams from CDF files
- Shows port layout, parameters, and function mappings
- Handles InternalPorts (coordinates -1,-1) by listing them below the block diagram
- Usage: `python3 cdf_to_ascii.py <cdf_file> [> output.md]`

## Hardware Abstraction Layer (HAL) Components

The eRT HAL system provides platform-independent component interfaces with platform-specific implementations. This enables components to work across different hardware platforms (ESP32, Linux, Windows, etc.) without code changes.

### HAL Architecture Layers

HAL components use a three-layer architecture:

1. **Component Layer** (`Common/Components/`)
   - Platform-independent function block implementation
   - Handles ports, events, and state management
   - Uses EHS API macros for port access

2. **Common HAL API Layer** (`Common/HAL/[category]/inx_[name].c`)
   - API headers in `Common/HAL/include/inx_[name].h`
   - Platform-independent bridge between component and target HAL
   - Translates component state to HAL configuration
   - Handles callbacks from target HAL to component InternalPorts
   - May contain utility/processing logic with no target-specific implementation

3. **Target HAL Layer** (`target/Component-HAL/[subsystem]/[implementation]/`)
   - Platform-specific hardware/SDK integration
   - Multiple implementations per subsystem (e.g., `nimble`, `sferalabs`, `stubbed`)
   - All implementations expose the same C function API

### Target HAL Makefile Pattern

This is the standard pattern used across all subsystems. It must be followed consistently.

#### component-hal.mk — top-level entry point

`target/Component-HAL/component-hal.mk` contains one entry per subsystem:

```makefile
ifdef EHS_PERIPHERALS_[SUBSYSTEM]_SUPPORT
ifneq ($(EHS_PERIPHERALS_[SUBSYSTEM]_SUPPORT),none)
include $(EHS_TARGET_COMPONENT_HAL_PATH)/[subsystem]/[subsystem]_common.mk
endif
endif
```

The variable value (`stubbed`, `sferalabs`, `nimble`, etc.) is **not inspected here**. All selection logic lives in the subsystem's own common makefile.

#### [subsystem]_common.mk — path construction and shared build rules

`target/Component-HAL/[subsystem]/[subsystem]_common.mk`:

```makefile
EHS_COMMON_[SUBSYSTEM]_HAL_PATH = $(EHS_TARGET_COMPONENT_HAL_PATH)/[subsystem]
EHS_TARGET_[SUBSYSTEM]_HAL_PATH = $(EHS_COMMON_[SUBSYSTEM]_HAL_PATH)/$(EHS_PERIPHERALS_[SUBSYSTEM]_SUPPORT)

INC_DIRS += $(EHS_COMMON_[SUBSYSTEM]_HAL_PATH)
INC_DIRS += $(EHS_TARGET_[SUBSYSTEM]_HAL_PATH)
VPATH    += $(EHS_TARGET_[SUBSYSTEM]_HAL_PATH)

OBJECTS  += target_[subsystem].$(OBJ)

# Include implementation-specific extras (extra libs, extra includes, DEFS).
include $(EHS_TARGET_[SUBSYSTEM]_HAL_PATH)/target_[subsystem].mk
```

The variable value drives the path directly — no `ifeq` ladder needed to find the right directory.

#### target/Component-HAL/[subsystem]/[implementation]/target_[subsystem].mk — per-implementation extras

Each implementation directory contains a `target_[subsystem].mk` that adds only what is unique to that implementation:

```makefile
# Real hardware implementation — add vendor library
LIB += somevendorlib

# OR: add a shared board-definitions include path
INC_DIRS += $(EHS_TARGET_COMPONENT_HAL_PATH)/shared_board_defs

# OR: for stubbed — nothing needed
# (file exists but is empty or has a comment only)
```

**The `stubbed` implementation is a peer of all other implementations** — it lives in `[subsystem]/stubbed/`, has its own `target_[subsystem].mk` (which may be empty), and provides a `target_[subsystem].c` that implements every function as a no-op returning an error code. It should never be given special treatment in `component-hal.mk` or `[subsystem]_common.mk`. There is no reason to skip compiling stubs from a higher level (they are small, and keeping the build consistent across platforms is more valuable than the trivial space saving). This allows hardware apps to be run on windows machines for instance without bailing because of missed function blocks.

#### Full directory layout

```
target/Component-HAL/[subsystem]/
├── [subsystem]_common.mk             # Path construction + shared OBJECTS/VPATH/INC
├── [implementation1]/                # e.g., nimble, sferalabs, esp32_idf
│   ├── target_[subsystem].mk        # Implementation extras (libs, extra INC_DIRS)
│   ├── target_[subsystem].c         # Implementation source
│   └── target_[subsystem].h         # Implementation header
└── stubbed/                          # No-op implementation — a peer, not special
    ├── target_[subsystem].mk        # Usually empty
    ├── target_[subsystem].c         # All functions return error codes
    └── target_[subsystem].h         # Same header as other implementations
```

### Creating a New HAL Subsystem

#### 1. Define the support variable in platform config

`target/platform/[target]/config.mk`:

```makefile
EHS_PERIPHERALS_[SUBSYSTEM]_SUPPORT=myimpl   # or: stubbed, none
```

**Naming conventions:**
- `EHS_PERIPHERALS_[SUBSYSTEM]_SUPPORT` for I/O peripherals
- `EHS_NETWORK_[SUBSYSTEM]_SUPPORT` for networking features
- `EHS_[SUBSYSTEM]_SUPPORT` for other categories
- Value = implementation directory name (`myimpl`, `stubbed`, `esp32_idf`, ...)
- `none` means the feature is excluded from the build entirely

#### 2. Add entry to component-hal.mk

```makefile
########################################################################################################
## [Subsystem]
########################################################################################################
ifdef EHS_PERIPHERALS_[SUBSYSTEM]_SUPPORT
ifneq ($(EHS_PERIPHERALS_[SUBSYSTEM]_SUPPORT),none)
include $(EHS_TARGET_COMPONENT_HAL_PATH)/[subsystem]/[subsystem]_common.mk
endif
endif
```

#### 3. Create [subsystem]_common.mk

Follow the template above. The `OBJECTS +=` line for the main source file goes here, not in the per-implementation `.mk`.

#### 4. Create the stubbed implementation first

`target_[subsystem].c` — every public function returns an error code (e.g. `-1` or a subsystem-specific `ENOTSUPPORTED` enum value). `target_[subsystem].h` — full API declaration. `target_[subsystem].mk` — empty (or a single comment).

The stubbed implementation serves as the canonical API reference and ensures every platform can compile even without hardware.

#### 5. Create real implementations

Each implementation adds only what differs from the API contract: vendor library linkage, vendor-specific includes, hardware register initialisation. The `.mk` carries only the build differences; the `.c` carries only the functional differences.

### os-arch Default Configuration Convention

#### Where defaults belong

The `EHS_X_SUPPORT` variables that control both HAL compilation (`component-hal.mk`) and function block compilation (`components.mk`) **must be set before `component-hal.mk` runs**. The build ordering in `platform.mk` is:

1. `target/platform/<TARGET>/config.mk` — hardware-specific values (real implementation names)
2. `target/os-arch/<OS-ARCH>/config.mk` — architecture-wide defaults (stubbed, none, or common defaults)
3. `target/Component-HAL/component-hal.mk` — compiles HAL based on the variable values
4. `Common/Components/.../components.mk` — compiles function blocks based on the same variables

Defaults and stubbed directives must be set at step 2 (os-arch level) or earlier. Setting them in `components.mk` (step 4) is **too late** for `component-hal.mk` to see them — this causes linker errors because the HAL stubs are not compiled but the function blocks reference HAL symbols.

#### `?=` is the preferred style for os-arch defaults

Use `VAR ?= value` in os-arch `config.mk` files. This is equivalent to `ifndef VAR / VAR=value / endif` but cleaner:

```makefile
# Preferred
EHS_WATCHDOG_SUPPORT ?= stubbed

# Old style — avoid for new additions
ifneq ($(EHS_WATCHDOG_SUPPORT),none)
ifndef EHS_WATCHDOG_SUPPORT
    EHS_WATCHDOG_SUPPORT=stubbed
endif
endif
```

Note: the `ifneq (...,none)` wrapper in the old style provides an opt-out mechanism. With `?=`, a platform that genuinely wants to exclude a feature should set `EHS_X_SUPPORT=none` in its platform `config.mk` before os-arch runs — but since platform `config.mk` runs before os-arch, the `?=` in os-arch will not overwrite a `none` value set by the platform. The opt-out still works correctly.

> **TODO**: Consider converting all existing `ifndef VAR / VAR=val / endif` patterns in os-arch `config.mk` files to `VAR ?= val` for consistency. Caveat: the old style sometimes wraps additional `ifneq` guards — review each before converting. Policy: `?=` should be standard for all new additions to os-arch config files.

#### When adding a new HAL subsystem

After creating the HAL and stub files, add `?=` defaults to **every** os-arch `config.mk` that should stub the feature by default. Do not add defaults to `component-hal.mk` itself — that file should contain only `ifdef/include` logic, not defaults.

### HAL Interface Best Practices

#### Data Types
- **Always use `ehs_` types in `Common/` code:** `ehs_uint8`, `ehs_uint16`, `ehs_uint32`, `ehs_bool`, `ehs_char`
- **Use `ehs_` types in HAL headers:** ensures cross-platform compatibility
- **Platform-specific types only in `.c` files:** OK internally, but the interface must use `ehs_` types

#### Callback Pattern
HAL implementations often need to trigger component InternalPorts from interrupts or threads:

1. HAL receives async event (ISR, thread callback)
2. HAL calls glue layer callback with event data
3. Glue layer stores data in component state:
   ```c
   state->cb_data_len = length;
   memcpy(state->cb_data, data, length);
   ```
4. Glue layer fires InternalPort function: `EhsMyComponent_onEvent(pFIdata);`
5. InternalPort populates output ports from state, then calls `EHS_FB_FINISH(event_idx)`

### Example: BLE Service Component

See the BLE service component for a complete HAL implementation example:
- **Component:** `Common/Components/networking/inx-ble_service.c`
- **Glue Layer:** `Common/Components/networking/inx-ble_service_hal_glue.c`
- **NimBLE HAL:** `target/Component-HAL/ble/nimble/ble_service_nimble.c`
- **Stubbed HAL:** `target/Component-HAL/ble/stubbed/ble_service_stubbed.c`
- **Build Integration:** `target/Component-HAL/ble/ble.mk`

## Important Notes

- The project requires a proprietary event handling kernel library
- Docker is used extensively for reproducible cross-platform builds
- Version information is managed in `Releases/version_strings`
- Component definitions can be updated in development tools with `make toolsenv_update`
- Always run `make prepdeps` when switching to a new target platform
- **Docker environment variables** — Docker containers do not inherit the host Make or shell environment. Any variable that the build needs inside the container must be listed in `INX_ERTCOMPONENTS_BUILDENV` in `target/envbuildscripts/target_buildenv_run_command.sh`. Variables set with `ENV` in the platform `Dockerfile` are baked into the image and do not need to be listed there.
- **Bitmap files are deprecated** - No longer used for component visual representation
- Components now use markdown documentation with ASCII diagrams generated from CDF files

### CDF Schema Conventions (Updated)

**Function Naming:**
- Use imperative verbs for StartPort functions (e.g., `init`, `read_char`, `notify`)
- Use past tense or noun forms for InternalPort callbacks (e.g., `on_connect`, `client_wrote`)
- Function names should be concise and describe the action performed

**Port Placement:**
- Left side: StartPort (event triggers), InputPort (data inputs), InternalPort (if visualized)
- Right side: FinishPort (event completions), OutputPort (data outputs)
- Y-coordinate spacing: Use gaps (60-80 units) between functional groups for readability
- InternalPorts: Always use coordinates `(-1, -1)` to exclude from visual diagram

**Function IDs:**
- ERT1_ID values are sequential starting from 1
- Each Function element must have unique ID within component
- IDs map to function table entries in C implementation

---

## Function Block ID Generation

Every function block has two hash-based identifiers stored in the CDF `<Hashes>` block and mirrored in the `.h` header file.

### `NameHash_CRC16` — the block's unique runtime ID

This is a **CRC-16/Modbus** hash of the `<Class>` name string. Use `inxtool.py` to compute it:

```bash
python3 scripts/inxware-id-tool/inxtool.py -genHash "<class_name>" -hash 16CRC
```

The tool prints 4 uppercase hex digits (e.g. `F512`). Prepend `0x` for the CDF and `.h` macro.

The result goes in **two places**:
1. CDF `<Hashes>/<NameHash_CRC16>` element
2. `.h` file as `#define INXWARE_FB_ID_<class>  0xF512`

Verified values: `adc_config`→`0x566F`, `accel_gyro`→`0xF2AA`, `lorawan`→`0xC89A`, `rtc`→`0x32C7`, `Unsigned2Int`→`0x4F75`, `led`→`0xA6EA`, `ml_image_inference`→`0xF512`.

See `CLAUDE-function-blocks.md` § *Function Block ID Generation* for full detail including the `FbApiDescriptorHash_CRC32` note.

### `FbApiDescriptorHash_CRC32` — leave as `00000000`

The Lucid IDE source marks this as `@TODO - this is not done yet`. Leave it as `00000000` for any manually-created or modified CDF. Do not attempt to compute it — the algorithm is not exposed in the open-source tools.

---

**Port-to-Function Mapping:**
- Each Port must reference a Function via `<Function_ERT1_ID>` element
- The `argument` attribute on Function element specifies port evaluation order
- Multiple ports can reference the same Function (grouped as inputs/outputs for that operation)
