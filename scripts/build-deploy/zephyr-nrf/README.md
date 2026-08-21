# Zephyr / nRF flashing and debugging tools

Host-native tools for flashing and debugging Zephyr / nRF Connect SDK targets
(Thingy:91 X, nRF9151/nRF5340 DKs). Like the ESP32 flashing tools, these run on
the developer PC (real USB / J-Link access) — **not** inside the build Docker
image. On WSL, USB needs `usbipd` (see the esp32 README).

## Install the CLI

```bash
./install_nrfutil.sh            # nrfutil → /usr/local/bin (sudo if needed)
# or, user-local without sudo:
INSTALL_DIR=~/.local/bin ./install_nrfutil.sh
```

Installs Nordic **nRF Util** + its `device` command (program / recover / list).
No VS Code / nRF Connect extension required — that GUI just wraps these CLIs.
The external-probe path additionally needs [SEGGER J-Link](https://www.segger.com/downloads/jlink/).

## Two ways to flash the Thingy:91 X

The Thingy:91 X has **no onboard debugger**. Choose based on what you have and
what image you built.

### A. USB / MCUboot — the normal "getting started" path (no probe)

Flash over the USB-C cable using the pre-installed MCUboot bootloader.

- **Requires a signed `dfu_application.zip`** — produced by the MCUboot/sysbuild
  build (`ERT_ZEPHYR_SYSBUILD = y`, already set for this target). It lands in
  `../../../../TARGET_TREES/ehs_env-zephyr_arm-nrf9151_thingy91x-wifi/bin/dfu_application.zip`.
- Power on (SW1 → ON). Do **not** open a terminal on the USB/UART0 port during
  the update — MCUboot uses it.
- Modem-firmware updates are **not** supported over USB/MCUboot (probe only).

```bash
nrfutil device program \
    --firmware .../bin/dfu_application.zip \
    --traits mcuboot --x-family nrf91 --core Application
```

> **Signing-key caveat:** our DFU zip is signed with MCUboot's **default debug
> key**. Nordic dev boards typically ship with that same key, so serial-recovery
> DFU should accept it. If the device *rejects* the image (its MCUboot uses a
> different key), flash `merged.hex` once via SWD (path B) to install our
> MCUboot + app, after which USB DFU with our key works.

### B. External SWD probe / J-Link — Nordic's recommended dev path

Takes the **raw `zephyr.hex`** we build today (no MCUboot needed).

- Connect a debug probe (standalone SEGGER J-Link, or an nRF DK's onboard
  J-Link-OB) to the 10-pin SWD header **P8**.
- Set **SW2 → nRF91** (selects the nRF9151 as the SWD target), **SW1 → ON**.
- Use app/sector programming (below). **Do not** full-`--recover` / erase-all —
  that can wipe the factory modem firmware and provisioning.

```bash
nrfutil device program \
    --firmware ../../../../TARGET_TREES/ehs_env-zephyr_arm-nrf9151_thingy91x-wifi/bin/zephyr.hex \
    --serial-number <JLINK_SN> \
    --traits jlink --x-family nrf91 --core Application
nrfutil device reset --serial-number <JLINK_SN>
```

`nrfutil device list` shows attached devices and their serial numbers.

## Serial console (logs)

The eRT app logs over the USB-CDC serial port. Any terminal works:

```bash
tio /dev/ttyACM0            # or minicom / putty, 115200 8N1
```

## On the companion chips

Flashing the nRF9151 app core does **not** touch the pre-flashed nRF9151 modem
firmware or the nRF5340 connectivity-bridge firmware (different cores/chips).
Custom BLE means building/flashing a separate `thingy91x/nrf5340/cpuapp` target
with **SW2 → nRF53**. See repo-root `CLAUDE.md` discussion for details.

## Wrapper scripts

Run from the ert-components repo **root** (they read `TARGET.cfg`), mirroring the
esp32 scripts:

| Script | Use |
|--------|-----|
| `nrf_flash.sh` | `--usb` (default) → MCUboot DFU of `dfu_application.zip`; `--swd` → J-Link flash of `merged.hex`. `--serial <SN>` / env `NRF_SERIAL`, `NRF_XFAMILY`, `NRF_CORE`. |
| `nrf_monitor.sh` | serial console on the USB-CDC port (`minicom`, or `--use-screen`); `115200` 8N1. |
| `nrf_debug.sh` | *(planned)* `west debug` / JLinkGDBServer. |

`nrf_flash.sh --usb` performs a **real** flash — not a dry run.

### USB permissions (errno 13 / "failed to open device")

`nrfutil` needs permission to open the USB device. If flashing fails with
`errno 13`, install Nordic's udev rules and/or join the device groups:

```bash
sudo usermod -aG dialout,plugdev "$USER"      # then log out / back in
# Nordic udev rules:
git clone https://github.com/NordicSemiconductor/nrf-udev
cd nrf-udev && sudo ./install.sh
```

(A one-off `sudo ./nrf_flash.sh …` also works, but udev is the clean fix.)
