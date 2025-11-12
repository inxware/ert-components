---
name: eRT-Components Bug report
about: Create a report to help us improve
title: ''
labels: ''
assignees: ''

---

## Summary

A clear, concise summary of the problem.

## Environment

- eRT-components version or commit: `vX.Y.Z` or `git rev-parse --short HEAD`
- Host OS and version: e.g. Ubuntu 22.04, Windows 11, macOS 14
- Build method: `make` locally, Docker, CI
- Toolchain and version: e.g. GCC x.y, Clang x.y, ESP-IDF x.y, Arduino Mbed core x.y
- Target platform/board: e.g. ESP32-S3, Linux x86_64, ARM64 Debian 11
- Target CPU architecture: e.g. xtensa, armv7, aarch64, x86_64
- Target OS/RTOS: e.g. FreeRTOS, Linux, bare metal, Android
- Components involved: list the eRT components or subsystems touched
- Lucid IDE version (if relevant): e.g. build date or version string

## Steps to reproduce

1. What you did, including exact commands and configuration
2. Minimal changes from a clean clone
3. Input files or configuration snippets if applicable

```sh
# Example
git clone https://github.com/inxware/ert-components.git
cd ert-components
make prepdeps
make <target> V=1
# run binary / flash device / start container ...
````

## Expected behaviour

What you expected to happen.

## Actual behaviour

What actually happened, including error messages.

## Logs and diagnostics

* Build output excerpts
* Runtime logs or serial output
* Backtrace or crash dump if available
* Any relevant `dmesg` or device logs

<details>
<summary>Logs</summary>

```
Paste logs here
```

</details>

## Reproducibility

* [ ] Always
* [ ] Often
* [ ] Intermittent

Is this a regression from an earlier version? If yes, which version last worked?

## Impact and severity

* [ ] Blocks build
* [ ] Blocks deployment
* [ ] Runtime failure
* [ ] Performance issue
* [ ] Functional bug with workaround

Briefly describe impact on your use case.

## Workarounds tried

Any mitigations or configuration changes you have tried.

## Additional context

Peripherals, network setup, notable environment details, links to a minimal repro repository or patch.

## Pre-submission checklist

* [ ] I searched existing issues and did not find a match.
* [ ] I can reproduce from a clean clone without unrelated changes.
* [ ] I included exact steps, logs, and environment details.
* [ ] I confirmed this happens on the latest `main` or the latest release.
