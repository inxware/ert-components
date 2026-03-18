# Sfera Labs HAL

This directory contains the eRT HAL implementation for Sfera Labs hardware
(Strato Pi, Iono Pi, and compatible boards).  The HAL reads and writes sysfs
nodes exposed by the Sfera Labs kernel module.

---

## Sfera Labs Kernel Module

The kernel module must be installed on the target Raspberry Pi before running
an eRT application that uses this HAL.  Sfera Labs distributes all drivers as
source-only modules — there are no pre-built `.deb` or dkms packages.

### Product / Repository Matrix

| Product | Kernel module repository |
|---|---|
| Strato Pi (CM1 / CM3 / CM4) | https://github.com/sfera-labs/strato-pi-kernel-module |
| Strato Pi Max (CM4 / CM5) | https://github.com/sfera-labs/strato-pi-max-kernel-module |
| Iono Pi | https://github.com/sfera-labs/iono-pi-kernel-module |
| Iono Pi Max | https://github.com/sfera-labs/iono-pi-max-kernel-module |
| Exo Sense Pi | https://github.com/sfera-labs/exo-sense-pi-kernel-module |

Sfera Labs GitHub organisation: https://github.com/sfera-labs
Sfera Labs website: https://www.sferalabs.cc/

---

## Installation (Raspberry Pi OS / Debian)

The instructions below apply to all modules.  Replace `<MODULE_REPO>` with the
repository name for your board (e.g. `strato-pi-kernel-module`) and `<MODULE>`
with the module/overlay name (e.g. `stratopi`).

### 1. Update the system

```bash
sudo apt update && sudo apt upgrade
```

### 2. RPi 4 / CM4 only — 32-bit OS workaround

Skip this step if you are running a 64-bit OS (the default since Raspberry Pi OS Bookworm).

Add the following line to `/boot/firmware/config.txt` (or `/boot/config.txt` on older images) and reboot before continuing:

```
arm_64bit=0
```

### 3. Install build dependencies

**Raspberry Pi OS / Debian:**
```bash
sudo apt install git linux-headers-$(uname -r)
```

**Ubuntu on Raspberry Pi:**
```bash
sudo apt install git linux-headers-raspi make gcc
```

### 4. Clone, build, and install the kernel module

```bash
git clone --depth 1 https://github.com/sfera-labs/<MODULE_REPO>.git
cd <MODULE_REPO>
make clean
make
sudo make install
```

### 5. Compile and install the Device Tree overlay

**Raspberry Pi OS / Debian** (overlay goes to `/boot/overlays/`):
```bash
dtc -@ -Hepapr -I dts -O dtb -o <MODULE>.dtbo <MODULE>.dts
sudo cp <MODULE>.dtbo /boot/overlays/
```

**Ubuntu** (overlay goes to `/boot/firmware/overlays/`):
```bash
dtc -@ -Hepapr -I dts -O dtb -o <MODULE>.dtbo <MODULE>.dts
sudo cp <MODULE>.dtbo /boot/firmware/overlays/
```

### 6. Enable the overlay in the boot config

Add to `/boot/firmware/config.txt` (or `/boot/config.txt`):
```
dtoverlay=<MODULE>
```

**Strato Pi Max XL with CM5 only** — also add:
```
dtoverlay=sdio-pi5
```

### 7. Optional: allow non-root access via udev

```bash
sudo groupadd <MODULE>
sudo cp 99-<MODULE>.rules /etc/udev/rules.d/
sudo usermod -a -G <MODULE> $USER
```

### 8. Reboot

```bash
sudo reboot
```

After reboot, the sysfs interface is available under `/sys/class/<MODULE>/`.

---

## RPi 4 vs RPi 5 Differences

| | RPi 4 / CM4 | RPi 5 / CM5 |
|---|---|---|
| Supported product | Strato Pi, Strato Pi Max, Iono Pi, etc. | Strato Pi Max (CM5 variant) |
| 32-bit OS workaround | Required for 32-bit OS only | Not applicable (64-bit only) |
| DTS file (Strato Pi Max) | `stratopimax-cm4.dts` | `stratopimax-cm5.dts` |
| Extra overlay (Strato Pi Max XL) | Not required | `dtoverlay=sdio-pi5` |
| Build process | Identical | Identical |

---

## Ubuntu-specific Notes

See the Sfera Labs knowledge base for Ubuntu-specific build notes:
https://github.com/sfera-labs/knowledge-base/blob/main/raspberrypi/kernel-modules-ubuntu.md

The main differences from Raspberry Pi OS are:
- Use `linux-headers-raspi` instead of `linux-headers-$(uname -r)`
- Device Tree overlay path is `/boot/firmware/overlays/` not `/boot/overlays/`
