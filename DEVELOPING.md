# Development

The following notes aim at helping developers who want to work upon and expand the eRT Components codebase.

- [Code style guidelines](#code-style-guidelines)
- [Platform porting](#platform-porting)
- [Component development](#component-development)
- [Testing guidelines](#testing-guidelines)
- [Debugging tools](#debugging-tools)
- [Development examples](#development-examples)
- [Attribution](#attribution)

## Code style guidelines

### C coding style

   ```c
   // Functions: lowercase with underscores
   int component_initialize(component_t* comp);

   // Types: lowercase with _t suffix
   typedef struct {
       int value;
       char name[32];
   } my_component_t;

   // Constants: uppercase with underscores
   #define MAX_BUFFER_SIZE 1024
   #define DEFAULT_TIMEOUT_MS 5000

   // HAL functions: `hal_` prefix
   ehs_result_t hal_gpio_write(int pin, int value);
   ```

   All eRT C source code should use K&R style braces and indent with four spaces.

   Where it is the source code for a (imported) third-party component, the existing coding style should be preserved to minimise the work of merges of future upstream revisions.

### Comments 

Say what the code is for, not how it works unless this is necessary

Write a comment when the intent is not obvious from the code. Say what the
function or block is for, what the units are, what the caller must guarantee,
which invariant must hold. Then stop.

```c
/* Bad - a history lesson */
/* i must be uint32: it was uint8, which wrapped 255->0 and spun forever on
 * any format over 256 characters, which is how the empty-format hang got in. */
ehs_uint32 i;

/* Good - if a longer explaination is necessary*/
ehs_uint32 i; /* i indexes the format string characters, which can be a full row */
```
## Platform porting

### Porting new hardware

1. **Create platform configuration**
   ```bash
   mkdir target/platform/my_new_platform/
   cd target/platform/my_new_platform/
   
   # Copy from similar platform
   cp ../linux_x86_64_clang/config.mk .
   cp ../linux_x86_64_clang/target_config.h .
   
   # Edit for your platform
   nano config.mk
   nano target_config.h
   ```

2. **Implement HAL layer**
   ```bash
   mkdir target/os-arch/my_os-my_arch/
   cd target/os-arch/my_os-my_arch/
   
   # Required files:
   nano target.mk           # Build configuration
   nano toolchain.mk        # Compiler settings
   nano target_main.c       # Application entry point
   nano target_time.c       # Timer implementation
   nano target_process.c    # Threading/mutex
   nano target_file.c       # File system
   # ... other HAL implementations
   ```

3. **Test and validate**
   ```bash
   ./configure my_new_platform
   make prepdeps
   make all
   make targetenv_run_tests
   ```

For detailed porting instructions, see [Porting Guide](docs/ert-porting-guide.md).

## Component development

### Component categories

| Category       | Purpose          | Examples                                |
|--------------- |----------------- |---------------------------------------- |
| **core**       | Basic operations | Operators, buffers, timers, file I/O    |
| **gui**        | User interface   | Displays, buttons, text boxes, graphics |
| **networking** | Communication    | HTTP, MQTT, TCP/UDP, WebSocket          |
| **media**      | Audio/video      | Codecs, players, streaming              |
| **ml**         | Machine learning | TensorFlow Lite, inference engines      |
| **mv**         | Machine vision   | Image processing, computer vision       |

### Component architecture

Each eRT component consists of:

- **CDF file** (.cdf): XML component description
- **Implementation** (.c/.h): C/C++ source code
- **Bitmap** (.bmp): Visual icon for IDE
- **Interface file** (.idf.ini): IDE integration settings

### Component source file layout

```
Common/Components/<category>/
├── <component_name>.c        # Implementation
├── <component_name>.h        # Public object interface
├── <component_name>.cdf      # Component XML descriptor used in Lucid IDE
├── <component_name>/tests/   # Lucid Applications used for unit testing the component
└── <component_name>/help/    # HTML documentation (used in Lucid)
```

### Creating new components

1. **Choose component category**
   ```bash
   ls Common/Components/
   # core/ gui/ networking/ media/ ml/ mv/
   ```

2. **Create component files**
   ```bash
   cd Common/Components/core/
   
   # Create component description
   nano my_component.cdf
   
   # Create implementation
   nano my_component.c
   nano my_component.h
   
   # Create icon (24x24 bitmap)
   # my_component.bmp
   
   # Create IDE settings
   nano my_component.idf.ini
   ```

### Creating HALs

The function block code that is common to all platforms should be implemented within the **`./Common/Components/`** directories under their relevant toolbox directories.

Code and functions that are common to multiple functions blocks and also 100% platform independent should be included within the **`./Common/HAL/`** directories. Functions should be prefixed with **`EhsH`**

Code and functions that works across more than one operating system-architecture combination and makes calls to APIs not defined within `./Common` (usually hardare or OS specific libraries) should be developed within the **`./target/Component-HAL/`** and prefixed with  **`Ehs<HAL Name>_`**. The HAL Name is the name of the abstraction function and defines a common API that any code in the `./Common/` directories or `target` directories can call. This code may interface directly to third party library APIs, OSs or hardware specific spefic code.

Code that only works for a specific operating system-architecture (os-arch) should be implemented within **`./target/os-arch/<os-arch>/`**. directories. The functions should be prefixed **`EhsT`**. 

During new hardware or OS integration it can be helpful to run only minimal functions of the ert runtimes. Skipping full system intialisation and/or starting an alternative main function instead of the EHS event handler can be achieved when running `make all` or `make all_docker` by setting the following environment variables:
```make
  TEST_FUNC=<some function>    # to run an alternative entrypoint instead of ehs_main() event handler.
  ERT_INIT=none                # to also skip the normal eRT initialisation functions before running TEST_FUNC.
```

3. **Register component**
   ```bash
   # Add to category makefile
   echo "my_component.c" >> components.mk
   
   # Update dependencies
   make depend
   
   # Update IDE tools
   make toolsenv_update
   ```

## Testing guidelines

### Automated unit testing
   Host tests. They compile the real source with the host `cc`, so they need no
   target tree and no hardware, and they return non-zero on failure.

   ```bash
   # Run every host unit test
   make unit_tests

   # The same, under AddressSanitizer + UndefinedBehaviorSanitizer
   make unit_tests_asan

   # Run one suite on its own
   cd UnitTest/hal_string && make
   ```
   `make unit_tests` picks up any `UnitTest/*/Makefile` that has a `run`
   target, so a new suite needs no wiring beyond its own directory. See
   [`UnitTest/README.md`](UnitTest/README.md) for what each suite covers and
   which ones are currently orphaned.

   The tests are host-native and Linux-only, but they are **not** detached from
   the eRT configuration: `PLATFORM=` selects a real `target_config.h`, so
   `make PLATFORM=nxp_arm` exercises the `base_small` size constants
   (`EHS_STRING_LENGTH_MAX` 256) rather than `base_full`'s (2048). What they do
   not inherit is `TARGET.cfg`, the `config.mk` feature flags and the target
   toolchain — so code behind a configuration `#ifdef` is untested unless the
   suite defines the flag itself. The full picture, and the refactoring this is
   expected to go through, is in [`docs/unit-testing.md`](docs/unit-testing.md).

### Automated integration testing

   ```bash
   # CI regression tests
   ./SystemTests/CI/regression_test-published-only.sh

   # Platform-specific tests
   ./SystemTests/CI/projects/[platform]/test_*.sh
   ```

### Manual (exploratory) testing

   ```bash
   # Test basic functionality
   ./configure -run
   # Should start without errors
   # Check basic i/o, networking, etc.

   # Test debugging
   ./configure -debug
   # Should provide detailed logging
   ```

## Debugging tools

eRT builds can be debugged most easily on linux targets using GDB, but can also be debugged on MCUs that support debugger connections (e.g. JTAG)

### gdb on Linux targets

You will need to install gdb on the linux target first.

   ```bash
   # install gdb (debian/raspbian/ubuntu)
   apt install gdb
   # Debug with gdb
   ./configure -debug
   # Automatically starts gdb session

   # For embedded targets
   # For embedded target supporting debuggers the following host scripts are available.
   ./scripts/build-deploy/[platform]/debug_*.sh
   ```

### Logging system

The logging verbosity of eRT can be configured in the build at the module level for verbosity. The following logging functions are used within the source code:

   ```c
   // In component code
   #include "hal_logger.h"

   hal_log_info("Component initialized");
   hal_log_error("Failed to read sensor: %d", error_code);
   hal_log_debug("Processing data: %f", sensor_value);
   ```

**todo** The following runtime log level selection may not actually work currently!

   ```bash
   # Control log levels
   EHS_LOG_LEVEL=DEBUG
   EHS_LOG_CATEGORIES="network,gpio"
   ./bin/ehs.exe
   ```

### Static analysis

   ```bash
   # cppcheck over the whole tree, honouring suppressions.txt and inline
   # suppressions. Results go to stdout.
   make static_analysis
   ```

### Stack usage

   Function blocks run on task stacks of a few kB on MCU targets, so a run
   function holding a buffer sized from a string-table constant can be the
   entire budget on its own. `EHS_DATA_TABLE_STRING_DEFAULT_LENGTH` is 32 KB on
   `base_full` and 256 bytes on `base_small` — never size a stack buffer from
   it.

   ```bash
   # Largest per-function frames, largest first
   make stack_report

   # Narrow it, and fail rather than just report
   make stack_report STACK_SRCS="Common/Components/core/stringfn.c" STACK_THRESHOLD=8192
   ```
   Frame sizes are ABI-dependent. The default is a host build, which is fine
   for comparing one revision against another; for the figure that matters on a
   device, build the real target and read the `.su` files the cross-compiler
   leaves behind:

   ```bash
   ./scripts/software-utilities/stack_report.sh --prebuilt
   ```

### Sanitisers

   The host unit tests build under AddressSanitizer and
   UndefinedBehaviorSanitizer with `make unit_tests_asan`. Bounds tests should
   use exact-sized heap allocations so the sanitiser catches an overrun that an
   assertion would miss.

## Development examples

### Creating a custom component

1. **Define component interface (CDF file)**
   ```xml
   <!-- Common/Components/custom/my_sensor.cdf -->
   <component name="my_sensor" category="sensors">
     <outputs>
       <pin name="temperature" type="real"/>
       <pin name="humidity" type="real"/>
     </outputs>
     <events>
       <event name="data_ready"/>
     </events>
   </component>
   ```

2. **Implement component logic**
   ```c
   // Common/Components/custom/my_sensor.c
   #include "my_sensor.h"
   
   void my_sensor_init(my_sensor_t* component) {
       // Initialize sensor hardware
   }
   
   void my_sensor_update(my_sensor_t* component) {
       // Read sensor data
       component->temperature = read_temperature();
       component->humidity = read_humidity();
       // Trigger data ready event
       ehs_trigger_event(component, EVENT_DATA_READY);
   }
   ```

3. **Register and build**
   ```bash
   # Add to component makefile
   echo "my_sensor.c" >> Common/Components/custom/components.mk
   
   # Update development tools
   make toolsenv_update
   
   # Component now available in Lucid IDE
   ```

### Hardware abstraction example

Below is a trivial example of how to implement some of the Hardware Abstraction Layer (HAL) for a new platform:

   ```c
   // target/os-arch/my_platform/target_gpio.c
   #include "ehs_hal_gpio.h"

   ehs_result_t hal_gpio_write(int pin, int value) {
       // Platform-specific GPIO implementation
       platform_gpio_set(pin, value);
       return EHS_SUCCESS;
   }

   int hal_gpio_read(int pin) {
       return platform_gpio_get(pin);
   }
   ```

## Attribution

*Copyright © 2008–2025 inx Limited. The community **eRT Components** release is open‑source under **LGPLv3**. The **EHS Kernel** is proprietary and licensed separately as described in [LICENSE.md](LICENSE.md).*
