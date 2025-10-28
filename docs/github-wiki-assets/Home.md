# What is inxware?

**inxware** is a development environment for embedded systems that seperates embedded software from application development.

The platform enables engineers and developers to create sophisticated embedded applications **without the formidable systems integration drag** of conventional embedded programming.

>This repository is for those wishing to modify, extend or port the platform to their requiremnts. It is not needed for developing on supported hardware produts.


inxware is built around the **eRT (event-based RunTime)** system that defines a simple uniform event-based API for software components and simplifies all types of data and control interconnection between components.

The **ert-components** repository contains the core components needed for the inxware eRT system - a high-performance, event-driven runtime designed to run no-code applications on everything from $1 microcontrollers to industrial servers. eRT bridges the gap between ease of use and professional capability, delivering near C/C++ performance while maintaining the simplicity of visual development.

## Key Features

- **Universal Hardware Compatibility**: Runs on microcontrollers, embedded linux, servers including Arduino boards, ESP32, Raspberry Pi, Android, Linux, Windows, or any custom hardware.
- **Component-Based Architecture**: Reusable functional blocks organized into specialized toolboxes (core, networking, GUI, media, ML, machine vision)
- **Event-Driven Performance**: Native event processing approaches the speed of compiled C/C++
- **Consolidates Vendor Toolchains & SDKs**: Cross-compilation for all targets using Docker environments and GNU Make
- **Hardware Abstraction**: Uniform component APIs across all platforms with varying capabilities

![inxware Technology Stack](./illustrations/inxware-ert-tech-stack.svg)

The platform consists of three main elements:
- **Lucid IDE**: Visual development environment for creating no-code applications
- **eRT Runtime**: High-performance execution engine and **Component Library** (this repository)
- **Component Builder**: An eclipse plugin that combies graphical function block definition and C/C++ code management.

Whether you're building IoT devices, industrial automation, digital signage, or embedded AI applications, inxware provides the tools to rapid prototype, develop, and deploy sophisticated solutions without the traditional complexity of embedded programming.

## Getting Started

### Prerequisites

Building eRT requires:
- **Ubuntu 22+** (recommended) or compatible Linux system
- **Git & Git-LFS** for repository management  
- **Docker** for containerized cross-compilation
- **GNU Make** and build-essential tools

These can be installed with
```bash
sudo apt install build-essentials git docker
```

### Start Building

1. **Clone the Repository**
   ```bash
   git clone git@github.com:inxware/ert-components.git
   cd ert-components
   ```

2. **Configure Your Target Platform**
   ```bash
   ./configure                    # List available targets
   ./configure linux_x86_64_clang # then choose one
   ```

3. **Install Dependencies**
   ```bash
   make prepdeps    # Downloads a few GB of SDK dependencies
   ```

4. **Build the inxware Runtime**
   ```bash
   make all_docker  # Build using Docker environment
   make targetenv   # Create runtime environment
   ```

5. **Test Your Build**
   ```bash
   ./configure -run # runs the../TARGET_TREES/ehs_env-[your-target]/bin/ehs.exe staging build
   ```
For detailed build instructions, see [eRT Build Guide](./ert-build-guide.md).

### Supported Platforms

eRT supports numerous platforms out of the box:

| Platform | Architecture | Status | Use Cases |
|----------|-------------|--------|-----------|
| **Linux** | x86, x86_64, ARM, ARM64 | ✅ Working | Servers, edge computing, Raspberry Pi |
| **ESP32** | Xtensa (esp32,S3,C2)     | ✅ Community | IoT devices, sensors, wireless |
| **Arduino** | Various MCUs           | ✅ Community | Industrial, prototypes, Education |
| **NXP Kenetis** | ARM Cortex-M       | ✅ Community | Industrial/Atomotive|
| **STM32** | ARM Cortex-M            | ✅ Ask inx | Consumer/Industrial/Atomotives |
| **Android** | ARM, ARM64           | ✅ Community | Mobile apps, industrial tablets |
| **Windows** | x86,x86_64          | ✅ Community | Desktop applications & simulators |
| **UNITY3D** | Win/Android/Linux | ✅ Ask inx | Digital Signage, VR, Audio          |

New platforms and legacy platforms are also available. Ask us!

## Features & Technologies
 ☑️ Some Systems
 ✅ All Systems

| Feature          | Contrib Tech    | Linux | MCUs | Windows|
|------------------|-----------------|-------|------|--------|
| GPIO/PWM         | Arduino, wiringPi, SDK | ✅    | ✅   |   🔍   |
| Machine Learning | TF-lite         | ✅    | 🚧   | ✅     |
| Vachine Vision   | OpenCV, GST     | ✅    | 🧪   | ✅     |
| GFX/UI           | GTK, LVGL, FB   | ✅    | ☑️   | ✅     |
| TCPIP/HTTP/MQTT  | LWIP, Linux, Curl, ...    | ✅    | ✅   | ✅     |
| WiFi             | WPASup, ESP32,  | ☑️    | ☑️   | ❌     |
| Zigbee           | ESP             | 🚧    | ✅   | 🚧     |
| Thread/Matter    | WiFi, Zigbee,      | ☑️    | ☑️   | 🚧     |
| LoRa/LoRaWAN     | MODEM (RAK, E5, STM)     | ✅    | ✅   | ✅     |

☑️ Some Systems ✅ All Systems 🚧 WIP 🧪 Experimental 🔍 emulated ❌ No Support


Use the `./configure` command to explore which platforms are available in your ert-components repo.

## Source Tree Fundamentals
The ert-components repository contains the central build system for device software as well as containing source code for all inxware components. Components are arranged in the source tree in directories relating to differently themed toolboxes. All components have a platform independent API and business logic layer and may be supported by hardware-specific code and bindings. 

- **`Common/Components/`** - Platform independent implementation
  - `core/` - Essential operators, buffers, converters
  - `networking/` - HTTP, MQTT, sockets, protocols  
  - `gui/` - Graphics and UI components
  - `media/` - Audio/video processing
  - `ml/` - Machine learning algorithms
  - `mv/` - Machine vision and image processing
- **`target/Component-HAL/`** - Abstraction for binding to target-specific libraries and platforms.
- **`target/os-arch/`** - OS and architecture specific code  
- **`target/platform/`** - Finest-grained product and hardare specific configuration (e.g. "boards")

ert-compoents also contains embedded systems utilities like flashing tools, product generation scripts and a command line-based CI system for unit and system build regression testing.

## Porting

### Platform Porting Guide

eRT's modular architecture makes porting to new platforms straightforward. The system separates platform-specific code from cross-platform components to amximise re-usability and consistenct across different target types.

The modular approach allows for features to be disabled or stubbed to allow incremental builds of platforms and reduced feature-sert versions to support resource or SDK capability constraints.

#### Porting Process Overview

1. **Platform Configuration** - Define build settings and capabilities
2. **OS-Architecture Support** - Implement system-level abstractions  
3. **HAL Implementation** - Create hardware abstraction layer
4. **Component Integration** - Port platform-specific components
5. **Testing & Validation** - Ensure functionality and performance

#### Key Porting Concepts

- **Cross-Platform Core**: All code in `./Common/` MUST run on every target
- **Target-Specific Code**: Platform implementations in `./target/`
- **Docker Environments**: Containerized builds ensure consistency (`Dockerfile/Dockerhub`)
- **Component APIs**: Uniform interfaces with platform-specific capabilities (use `iCB`)
- **Hardware Abstraction**: Transferable hardware independence structure through a single HAL API layer. 

For comprehensive porting information, see [eRT Porting Guide](./ert-porting-guide.md).

## Adding Your Tech
- If you have a new software component or type of peripheral you will most likely integrate this as a new Function Block **Compoent**.

- If you have a new implementation of an existing function block component or Feature Toolbox then you will most likely want to implement a new directory `./target/Component-HAL/<Feature>/<Implementation>` to support your implementation. 

### Component Development

eRT's component-based architecture makes it easy to integrate new technologies and extend platform capabilities. Components are the building blocks that users combine in the Lucid IDE to create applications.

#### Component Architecture

Each component consists of:
- **`.cdf` file** - XML component description for the Lucid IDE
- **C/C++ implementation** - Business logic and hardware interfaces
- **`help.html`** - Documentation displayed in Lucid
- **`tests/` directory** - Unit tests and example applications
- **Visual representation** - Bitmap files for IDE display

#### Creating New Components

1. **Define Interface** - Specify inputs, outputs, and parameters
2. **Implement Logic** - Write C/C++ following eRT patterns
3. **Create Documentation** - Help files and examples
4. **Add Visual Assets** - Icons and representations
5. **Integrate & Test** - Add to appropriate toolbox

#### Component Categories

Components are organized into thematic **toolboxes**:

- **Core** - Essential operators, buffers, control structures
- **Networking** - Communication protocols and interfaces
- **GUI** - User interface and graphics components  
- **Peripherals** - GPIO, sensors, actuators
- **Media** - Audio/video processing capabilities
- **Machine Vision** - Image processing and computer vision
- **Machine Learning** - AI algorithms and inference

#### Integration Process

New technologies can be integrated at multiple levels:

1. **Component Level** - Add new functional blocks to existing toolboxes
2. **HAL Level** - Implement hardware abstractions for new devices
3. **Platform Level** - Port eRT to entirely new platforms
4. **Middleware Level** - Integrate third-party libraries and SDKs

#### Hands On
You will probbly need to consult the [detailed documentation](./ert-porting-guide.md) before developing a new component, but a brief is provided here.
Component development is supported with the following elements of inxware:

- **inxware Component Builder (iCB)** - Eclipse plugin for visual component creation
- **Build System** - GNU Make with Docker containerization
- **Testing Framework** - Automated regression testing
- **Documentation** - Integrated help system
### Component Tools
![Component Creation with iCB](./illustrations/component-creation-with-icb.png)

#### Component Versioning and Signing

Community developers typically work within their private `user` toolbox directory. Components for standard toolboxes are managed by inx limited and are managed alongside CDF file deployments to the Lucid IDE. All components are cryptographically signed to ensure API compatibility with devices and applications developed with the Lucid IDE.

### Extending the Component-HAL
If you are supporting inxware features with new technologies you will begin by adding a new directory into `./ert-components/target/Component-HAL/<Relevant Feature>/<Your Variant`
You will add a `feature.mk` file to this to build it and extend the conditional include in the parent `feature.mk` file to select it when the relevnt `EHS_<FEATURE>_SUPPORT` is set to use your implementation.
> If your imiplementation already has a build system for your target and/or you do not want to include the source in the ert-component repository you can instead build it outside the repo (e.g. in the `ert-contrib-middleware` repos and place the headers and libraries in the relevant `./target_libs/$OS-$ARCH-$MIDDLEWARE>)/build/` directory. You then only need to add some binding code in `./ert-components/target/Component-HAL/` to call your libraries. See [detailed porting documentation](./ert-porting-guide.md) for more details of this. You should also build a Dockerfile and build a container for building the target that can then be used to build ert-with the same host SDK.

## Docker Environments

There are a number of utilities for building, deployng and debugging with Docker container images.
If you are creating new components and not porting to a new environment you wont need to work directly with any Docker/container management commands as the correct Dockerimage is run for you when using make commands like `make all_docker` 

### Build and publish a Dockerfile to Dockerhub

```bash
make publish_docker_image
```

Builds the Dockerfile `.target/platform/<your platform>/Dockefile` and publishes it with the name in `.target/platform/<your platform>/Dockerimagename`

### Switch Interactive CLI to Docker container Environment

```bash
make target_buildenv
```

This is useful during development if you need to debug the build environment. in this environment you don't need to use the `_socker` suffix to build commands e.g. `make all` instead of `make all_docker`

---

**Ready to get started?** Choose your path:
- 📥 **Building Applications**: Download prebuilt runtimes from [AppLand](https://appland.inxware.io)
- 🔧 **Platform Development**: Follow the [Getting Started](./ert-porting-guide.md)
- 🛠️ **Porting to New Hardware**: Check the [Porting Guide](./ert-porting-guide.md)
- 💡 **Component Development**: Explore [Adding Your Technology](./ert-porting-guide.md)

For questions, support, or to learn more about inxware, visit creeate a ticket, discussion or ping us a line:
- [github issues](https://github.com/inxware/ert-components/issues) 
- [github discussions](https://github.com/inxware/ert-components/discussions)  
- [inxware.io](https://www.inxware.io) - Let's have a chat!