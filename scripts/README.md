# Scripts Directory

This directory contains development utilities, build helpers, and deployment scripts for the eRT runtime system.

## Directory Overview

### build-deploy/
Platform-specific deployment and packaging scripts organized by target platform.

**esp32/** - ESP32 microcontroller deployment tools:
- `esp32_flash.sh` - Flash firmware to ESP32 device
- `esp32_monitor_console.sh` - Monitor serial output from device
- `esp32_build-flash-monitor.sh` - Complete build-flash-monitor workflow
- `esp32_wipe.sh` - Erase flash memory
- `esp32_memory_analysis.sh` - Analyze memory usage
- `create_esptool_python_venv.sh` - Setup Python environment for esptool

**linux-general/** - General Linux deployment:
- `deployViaSsh.sh` - Deploy runtime to remote Linux device via SSH

**packagers/** - Package creation tools:
- `nsis/` - Windows NSIS installer scripts and resources

### build-function-library/
Reusable shell functions for build automation:
- `build-utilities.sh` - Common build helpers (e.g., finding repo root)
- `debian-utilities.sh` - Debian package building functions
- `android-utilities.sh` - Android APK building functions

These are sourced by other scripts to provide consistent build operations across different platforms.

### docker-utilities/
Docker container management tools:
- `run_as_docker.sh` - Execute commands inside Docker build containers

### software-utilities/
Development and validation tools:
- `cdf_validate.py` - Validates component descriptor files (.cdf) for correctness
- `sodl_bin_to_c_file.py` - Converts SODL binary application files to C arrays for embedding

### ai-utilities/
Machine learning model preparation tools:

**apriltag-model-trainer/** - AprilTag detection model training scripts

**hailo-utils/** - Utilities for Hailo AI accelerator model conversion and deployment

**yolo-model-utils/** - YOLO object detection model preparation and conversion

### inxware-id-tool/
Tools for managing inxware device identifiers and component validation:
- `cdf_validate.py` - Symlink to software-utilities validator
- `inxtool_oldscript/` - Legacy tooling (deprecated)

## Notes for Developers

- Most scripts expect to be run from specific working directories. Check the script comments for requirements.
- Build function libraries use `cdBuildTreeRoot()` to locate the repository root automatically.
- ESP32 scripts require a Python virtual environment with esptool installed (see `build-deploy/esp32/README.md`).
- Platform-specific packagers are typically invoked via Makefile targets (e.g., `make targetenv_deb_docker`).
