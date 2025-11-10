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

   ```bash
   # Run all unit tests
   make targetenv_run_tests

   # Run specific test suites
   cd UnitTest/
   ./run_function_library_tests.sh
   ./run_component_tests.sh
   ```

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
   # Run static analysis
   make static_analysis

   # Results in build/analysis/
   # Includes: cppcheck, clang-analyzer, etc.
   ```

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
   #include "hal_gpio.h"

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
