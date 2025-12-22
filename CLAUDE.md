# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is the **ert-components** repository, which contains the core components needed for the inxware eRT (event-based RunTime) system. eRT is designed to run no-code applications on embedded devices and various computing systems including servers, edge compute, and desktop platforms.

## Build System

The project uses a sophisticated Make-based build system with Docker support for cross-platform compilation.

### Key Commands

**Configuration:**
- `./configure` - List available build targets
- `./configure [target]` - Configure for specific platform (e.g., `linux_x86_64_clang`)
- `./configure -edit` - Edit current target configuration
- `./configure -run` - Run the target on current host
- `./configure -debug` - Debug the target with GDB

**Building:**
- `make help` - Show all available build targets and options
- `make prepdeps` - Install dependencies and checkout required repositories
- `make all` - Build the eRT binary (ehs_[TARGET].exe)
- `make all_docker` - Build using Docker environment
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

These are automatically cloned by `make prepdeps` and require ~40GB of disk space.

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
- **Hashes** - CRC checksums for component identification
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

2. **Glue Layer** (`Common/Components/[category]/inx-[component]_hal_glue.c`)
   - Platform-independent bridge between component and HAL
   - Translates component state to HAL configuration
   - Handles callbacks from HAL to component InternalPorts
   - Stores callback data in component state before triggering internal port functions

3. **HAL Layer** (`target/Component-HAL/[subsystem]/[implementation]/`)
   - Platform-specific hardware/SDK integration
   - Multiple implementations per subsystem (e.g., `nimble`, `stubbed`)
   - Provides uniform API regardless of underlying platform

### Creating HAL-Dependent Components

#### 1. Define HAL Support Variable

In `target/platform/[target]/config.mk`, set the implementation type:

```makefile
# BLE Support - specify implementation
EHS_NETWORK_BLE_SUPPORT=nimble     # For ESP32 with NimBLE
# EHS_NETWORK_BLE_SUPPORT=stubbed  # For platforms without BLE
```

**Naming Convention:**
- Use `EHS_NETWORK_[SUBSYSTEM]_SUPPORT` for networking features
- Use `EHS_PERIPHERALS_[SUBSYSTEM]_SUPPORT` for peripherals
- Use `EHS_[SUBSYSTEM]_SUPPORT` for other features
- Value should be the implementation name (e.g., `nimble`, `stubbed`, `esp32`, `linux`)

#### 2. Create HAL Directory Structure

```
target/Component-HAL/[subsystem]/
├── [subsystem].mk                    # Main HAL makefile
├── [implementation1]/                # First implementation (e.g., nimble)
│   ├── target_[subsystem].mk        # Implementation-specific makefile
│   ├── [subsystem]_[impl].c         # Implementation source
│   └── [subsystem]_[impl].h         # Implementation header
└── stubbed/                          # Stub for unsupported platforms
    ├── target_[subsystem].mk        # Stubbed makefile
    ├── [subsystem]_stubbed.c        # Stub returning errors
    └── [subsystem]_stubbed.h        # Stub header
```

#### 3. Create Main HAL Makefile

`target/Component-HAL/[subsystem]/[subsystem].mk`:

```makefile
# [Subsystem] HAL Makefile

# Determine which implementation to use based on platform variable
ifdef EHS_NETWORK_[SUBSYSTEM]_SUPPORT
ifneq ($(EHS_NETWORK_[SUBSYSTEM]_SUPPORT),none)

    # Define paths based on implementation type
    EHS_COMMON_[SUBSYSTEM]_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/[subsystem]
    EHS_TARGET_[SUBSYSTEM]_HAL_PATH=$(EHS_COMMON_[SUBSYSTEM]_HAL_PATH)/$(EHS_NETWORK_[SUBSYSTEM]_SUPPORT)

    # Include implementation-specific makefile
    include $(EHS_TARGET_[SUBSYSTEM]_HAL_PATH)/target_[subsystem].mk

    # Add include paths
    INC_DIRS += $(EHS_TARGET_[SUBSYSTEM]_HAL_PATH)

    # Add to VPATH so make can find sources
    VPATH += $(EHS_TARGET_[SUBSYSTEM]_HAL_PATH)

endif
endif
```

**Key Points:**
- Use conditional includes (`include $(EHS_TARGET_..._HAL_PATH)/target_[subsystem].mk`)
- Let each implementation's makefile specify its own `OBJECTS +=` entries
- This keeps implementation-specific build logic in implementation directories
- Avoids complex conditionals in the main HAL makefile

#### 4. Create Implementation Makefiles

`target/Component-HAL/[subsystem]/[implementation]/target_[subsystem].mk`:

```makefile
# [Implementation] [Subsystem] HAL Makefile

# Add implementation-specific source files
OBJECTS += [subsystem]_[implementation].$(OBJ)

# Add glue layer (shared across implementations)
OBJECTS += inx-[subsystem]_hal_glue.$(OBJ)

# Add any implementation-specific includes or defines
# INC_DIRS += $(EHS_TARGET_[SUBSYSTEM]_HAL_PATH)/lib
# DEFS += [IMPLEMENTATION]_SPECIFIC_FLAG
```

#### 5. Integrate into Component-HAL

In `target/Component-HAL/component-hal.mk`, add a section:

```makefile
########################################################################################################
## [Subsystem Description]
########################################################################################################
ifdef EHS_NETWORK_[SUBSYSTEM]_SUPPORT
ifneq ($(EHS_NETWORK_[SUBSYSTEM]_SUPPORT),none)
DEFS += EHS_NETWORK_[SUBSYSTEM]_SUPPORT
include $(EHS_TARGET_COMPONENT_HAL_PATH)/[subsystem]/[subsystem].mk
endif
endif
```

#### 6. Add Component to Networking/Component Makefile

In `Common/Components/networking/components.mk` (or appropriate category):

```makefile
# [Subsystem] support
ifdef EHS_NETWORK_[SUBSYSTEM]_SUPPORT
ifneq ($(EHS_NETWORK_[SUBSYSTEM]_SUPPORT),none)
ifneq ($(EHS_NETWORK_[SUBSYSTEM]_SUPPORT),)
	DEFS += EHS_NETWORK_[SUBSYSTEM]_SUPPORT
	OBJECTS += inx-[component].$(OBJ)
	# Add HAL include path for glue layer
	INC_DIRS += $(EHS_TARGET_COMPONENT_HAL_PATH)/[subsystem]/$(EHS_NETWORK_[SUBSYSTEM]_SUPPORT)
endif
endif
endif
```

### HAL Interface Best Practices

#### Data Types
- **Always use ehs_ types in Common/ code:** `ehs_uint8`, `ehs_uint16`, `ehs_uint32`, `ehs_bool`, `ehs_char`
- **Use ehs_ types in HAL headers:** Ensures cross-platform compatibility
- **Platform-specific types only in .c files:** OK to use SDK types internally, but interface must use ehs_ types

#### Callback Pattern
HAL implementations often need to trigger component InternalPorts from interrupts or threads:

1. **HAL receives async event** (ISR, thread callback)
2. **HAL calls glue layer callback** with event data
3. **Glue layer stores data in component state**
   ```c
   state->cb_char_idx = char_idx;
   state->cb_data_len = length;
   memcpy(state->cb_data, data, length);
   ```
4. **Glue layer calls InternalPort function**
   ```c
   EhsRunble_service_on_client_write(pFIdata);
   ```
5. **InternalPort function populates outputs from state**
   ```c
   EHS_FB_OUT_I_API2(port_idx) = state->cb_char_idx;
   ```
6. **InternalPort triggers finish event**
   ```c
   EHS_FB_FINISH(event_idx);
   ```

#### Stubbed Implementations
Always provide a stubbed implementation for platforms without hardware support:

- All functions return `-1` (error)
- Read functions set output length to `0`
- Deinit function does nothing
- Allows components to compile on all platforms
- Enables cross-platform build checks

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

**Port-to-Function Mapping:**
- Each Port must reference a Function via `<Function_ERT1_ID>` element
- The `argument` attribute on Function element specifies port evaluation order
- Multiple ports can reference the same Function (grouped as inputs/outputs for that operation)