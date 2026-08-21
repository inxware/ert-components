# Boot2Qt display configuration — Raspberry Pi 5 + official 7" MIPI DSI panel

This target's binary also runs on **Boot2Qt** images (`b2qt-raspberrypi-armv8`). Driving an
MIPI/DSI panel instead of HDMI needs two **target-side** files on the device's SD card.
Neither is set by the build — `config.mk` and the Docker image are unaffected.

Verified on: Pi 5 Model B Rev 1.0, Boot2Qt, kernel 6.6.63-v8, official 7" Touch Display
rev 1 (800x480). A Pi 4 drives DSI from the SoC rather than RP1, so its DRM device and
`by-path` name will differ.

## Change 1 — `/boot/config.txt`

```
display_auto_detect=1
dtparam=i2c_csi_dsi0=on
dtparam=i2c_csi_dsi1=on
```

`display_auto_detect=1` makes the firmware probe the MIPI connector over I2C and load the
matching panel overlay. Without it the DSI hardware is never instantiated — no DRM
connector at all — and `dtoverlay=vc4-kms-v3d` gives HDMI only. It is preferred over a
hard-coded `dtoverlay=vc4-kms-dsi-7inch,dsi0` because it also works out *which* connector
the panel is in. This is the line Raspberry Pi OS ships and the Boot2Qt image lacks.

The `i2c_csi_dsi` params are optional/diagnostic: they expose `i2c-10` / `i2c-11` so a panel
can be identified with `i2cdetect -y -r 10` if auto-detect fails to recognise it (rev 1
display = ATTINY88 at `0x45`, FT5406 touch at `0x38`).

Needs a reboot. Afterwards the panel is a **third DRM card**, `card2-DSI-1`, at 800x480.
Touch needs no extra work — `edt_ft5x06` binds on `i2c-10:0x38` and libinput finds it.

Backup: `/boot/config.txt.bak-preDSI`

## Change 2 — `/etc/kms.conf`

```json
{
  "hwcursor": false,
  "device": "/dev/dri/by-path/platform-1f00118000.dsi-card"
}
```

This is what actually moves the app onto the panel. It previously named `/dev/dri/card0`,
the HDMI card — which gives the "text on the panel, graphics on HDMI" symptom, because the
kernel console follows `fbcon=map:10` onto the panel while Qt renders to HDMI.

Use the `by-path` name, not `card2` — DRM card numbers can shift between boots.

Back to HDMI: `cp /etc/kms.conf.hdmi-bak /etc/kms.conf`, no reboot needed.

## Change 3 — `/etc/default/qt` (hide the mouse pointer)

```
QT_QPA_EGLFS_HIDECURSOR=1
```

Removes the pointer on a touch-only device. Note `"hwcursor": false` in `kms.conf`
does **not** do this — it only disables the hardware cursor *plane*, after which Qt
draws a software cursor instead.

There is usually no mouse attached: both touchscreens expose legacy `mouse0`/`mouse1`
handlers next to their `event*` nodes, which is enough for Qt to draw a cursor.

This is an **eglfs** setting, so it covers the launcher and any app running
direct-to-KMS. A Wayland client's cursor is drawn by the compositor and is a separate
layer.

Backup: `/etc/default/qt.bak-precursor`

## Rendering technologies

No X11, no compositor — Qt owns the display directly. (Launching from the Boot2Qt
launcher instead makes the app a Wayland client inside it; see *Adding an app + icon*
below.)

| Layer          | Technology                  | Notes                                          |
|----------------|-----------------------------|------------------------------------------------|
| Qt platform    | `eglfs`                     | `QT_QPA_PLATFORM=eglfs`, Qt drives KMS itself   |
| Device backend | `eglfs_kms`                 | `libqeglfs-kms-integration.so`                  |
| Buffer mgmt    | **GBM**                     | allocates scanout buffers                       |
| Mode setting   | **DRM/KMS**, legacy path    | atomic disabled via `QT_QPA_EGLFS_KMS_ATOMIC=0` |
| 3D API         | **OpenGL ES via EGL**       | Mesa                                            |
| GPU driver     | **v3d**                     | render-only node `renderD128`                   |
| Scanout        | **drm-rp1-dsi** / `vc4-drm` | DSI panel / HDMI                                |
| Render↔scanout | Mesa **kmsro**              | bridges v3d onto a display-only KMS device      |
| Pixel format   | `XR24` (XRGB8888)           | `QT_QPA_EGLFS_FORCE888=1`                       |
| Input          | **libinput**                | evdev underneath                                |
| Launcher       | `appcontroller`             | Boot2Qt; sources `/etc/default/qt`              |

Pi 5 splits rendering and scanout across separate DRM devices — this is the part that
catches people out:

| Node                   | Driver        | Role                                |
|------------------------|---------------|-------------------------------------|
| `card0`                | `vc4-drm`     | HDMI x2 (BCM2712) — scanout only    |
| `card1` / `renderD128` | `v3d`         | GPU — render only, no display       |
| `card2`                | `drm-rp1-dsi` | MIPI DSI panel (RP1) — scanout only |

Neither display device can render and the render device cannot scan out; Mesa's kmsro path
stitches them together. That is why pointing `kms.conf` at a display-only device still gets
hardware-accelerated GL.

## Verifying

```bash
cat /sys/class/drm/card2-DSI-1/status          # -> connected
grep ft5x06 /proc/bus/input/devices            # -> 10-0038 generic ft5x06

QT_LOGGING_RULES="qt.qpa.*=true" /usr/bin/appcontroller /opt/ehs/bin/ehs.exe 2>&1 \
  | grep -iE "Using DRM device|Selected mode|Adding QPlatformScreen"
# -> ...dsi-card... / Selected mode 0 : 800 x 480 @ 60 hz for output "DSI1"
```

## Adding an app + icon to the Boot2Qt launcher

The Boot2Qt "dashboard" is `/usr/bin/qtlauncher`, run by `demolauncher.service`. It
scans **`/usr/share/examples`** for `*/demo.xml` and renders one tile per descriptor.

Ready-made files for this target are in [`boot2qt-launcher/`](boot2qt-launcher/) next
to this document. Install:

```bash
ssh root@<device> 'mkdir -p /usr/share/examples/ert'
scp boot2qt-launcher/demo.xml boot2qt-launcher/run_ert_wayland.sh \
    boot2qt-launcher/preview.png root@<device>:/usr/share/examples/ert/
ssh root@<device> 'chmod +x /usr/share/examples/ert/run_ert_wayland.sh; \
                   systemctl restart demolauncher.service'
```

(`make_preview.sh` stays on the build host — it is not needed on the device.)

Confirm the tile registered — the total goes up by one:

```bash
journalctl -u demolauncher.service -n 5 | grep Indexer   # -> Indexer: all done... total: 9
```

### The descriptor

```xml
<application title="inxware eRT" priority="10" scalable="1" icon="preview.png"
             binary="/usr/share/examples/ert/run_ert_wayland.sh">
    <description>Shown in the tile's info popup.</description>
</application>
```

| Attribute  | Meaning                                                             |
|------------|---------------------------------------------------------------------|
| `title`    | Tile caption                                                        |
| `priority` | Sort order in the grid; higher sorts earlier                        |
| `scalable` | `1` = compositor configures the surface to the full output size     |
| `icon`     | Resolved relative to the descriptor's own directory                 |
| `binary`   | Absolute, or relative to `/usr/share/examples`                      |

### Two traps

**1. The launcher is a Wayland compositor.** Tiles run as Wayland *clients* rendering
into a surface inside it — not on eglfs. The image-wide `QT_QPA_PLATFORM=eglfs` from
`/etc/default/qt` has to be overridden to `wayland`, or the app takes DRM master and
fights the launcher for the display.

**2. `appcontroller` stops the running app.** Without `--launch` it stops whatever
Boot2Qt app is running — which, from a tile, is the launcher itself. It dies with
`Could not queue DRM page flip on screen DSI1 (Permission denied)` and takes the
compositor socket with it, so the app then fails with `Failed to create wl_display
(Connection refused)`. This is why `run_ehs.sh` **cannot** be reused for a tile: it
calls plain `appcontroller`. `run_ert_wayland.sh` replicates the environment it sets
(`INXWAREROOT`, `DEVMANCOREDIR`, `SYSDATA`, cwd) and execs `ehs.exe` directly.

The stop-first behaviour applies to *every* `appcontroller` call on the device, not
just tiles.

`appcontroller` applies `/etc/default/qt` as **defaults only** — it never overwrites a
variable already set — so exporting `QT_QPA_PLATFORM` in a wrapper is sufficient.

### Verifying a launched tile

```bash
pidof ehs.exe && pidof qtlauncher        # both alive = no DRM master fight
ls -l /proc/$(pidof ehs.exe | cut -d' ' -f1)/fd | grep dri
# -> renderD128 only.  card0/card2 would mean it grabbed KMS and will kill the launcher.
```

### The icon

`preview.png` is 1191x687 on `#003D46` — the stock tile geometry, and a background
sampled from it that sits in the launcher's palette (`ViewSettings.backgroundColor`
`#00414a`). Regenerate it from the brand SVG rather than editing the PNG:

```bash
cd boot2qt-launcher && ./make_preview.sh preview.png
```

It renders the off-white inxware **wordmark** per
`Marketing/Claude/assets/inx-branding/branding_kit_sheet.md` — off-white `#f8f8f8` on
dark, horizontal lockup, and clear space comfortably above the kit's 9% minimum.
Override the asset location with `BRAND=<dir>` if the marketing repo is checked out
elsewhere.

No icon+wordmark combined lockup is used: the kit records that one does not exist yet
and that its ratio/gap needs design sign-off, so building one here would be off-brand.

### Alternative — boot straight to eRT instead

To skip the launcher entirely, `appcontroller --make-default` creates `/usr/bin/b2qt`.
`demolauncher.service` and `startupscreen.service` are gated on
`ConditionPathExists=!/usr/bin/b2qt` and `b2qt.service` on the same path existing, so
that one file flips which UI boots. That route keeps eglfs and full KMS ownership —
faster than compositing inside the launcher. `--remove-default` reverts it.

## Gotchas

- **Both touchscreens register** if an HDMI touch monitor is also attached — events arrive
  from both.
- **`fbcon=map:10`** in `/boot/cmdline.txt` puts the kernel console on the panel; remove it
  to keep the panel clear of boot text.
- **Legacy Pi 0-4 keys in `config.txt` are ignored on a Pi 5** (`start_x`, `gpu_mem_*`,
  `max_framebuffers`) — misleading when debugging.
- **The rev 1 panel is 15-pin DSI**; a Pi 5 has 22-pin connectors, so the Pi 5 adapter cable
  is required.
- **`i2c-13`/`i2c-14` are HDMI DDC buses**, not MIPI — an EDID read there is the monitor's.
