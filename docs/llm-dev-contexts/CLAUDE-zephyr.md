# Zephyr RTOS — status & signpost

**Substantive documentation lives in [`docs/ert-porting-guide.md` § Zephyr RTOS](../ert-porting-guide.md#zephyr-rtos).**
That section is the canonical reference for the two-step build workflow, the `config.mk`
variables, the Docker image / Zephyr version pairing, the DTS alias contract, the generated
staging files, known build gotchas, and the new-board checklist. Update it — not this file —
when any of that changes.

This file holds only transitory Zephyr status and TODOs.

---

## Open items

- **On-device kernel validation (pending).** The real eRT kernel is cross-compiled, linked, and
  invoked (`EhsInit()` → `EhsMain()` on a dedicated Zephyr thread) for `zephyr_arm-nrf5340_nrf5340dk`
  and `zephyr_arm-nrf52840_rak4631`, but the scheduler has **not yet been confirmed running on real
  hardware**. A hardware bring-up test on a new device is planned. Update the status row in the
  porting guide (§ *Current status and known limitations*) once validated.

- **`zephyr_arm-nrf9151_thingy91x-wifi` (Nordic Thingy:91 X) — first nRF Connect SDK (NCS) target, bring-up.**
  Builds the nRF9151 Cortex-M33 core from NCS (not upstream Zephyr) via the new `ERT_ZEPHYR_MANIFEST` knob
  and the `sdk-nrf-toolchain` image (which bakes west + the arm-zephyr-eabi toolchain into PATH — no wrapper
  needed; its `bash -c` entrypoint just needs the container command as a single string, handled in
  `zephyr_build_docker.sh`). Board `thingy91x/nrf9151`; reuses the existing `arm-zephyr-cortexm33` kernel archive.
  **Build-verified end-to-end on NCS v3.3.0 (Zephyr 4.3.99), both single-image and MCUboot/sysbuild:**
  - `--no-sysbuild` (bare): `zephyr.hex`/`.elf`/`.bin`, FLASH ~45% (207 KB/448 KB), RAM ~40%.
  - `ERT_ZEPHYR_SYSBUILD=y` (MCUboot + b0 + signed app): emits **`merged.hex`** (full SWD flash) + **`dfu_application.zip`**
    (USB/MCUboot DFU, no probe); app FLASH ~24% of an 851 KB slot, MCUboot ~85% of 32 KB. The sysbuild
    `EHS_ROOT`-not-forwarded issue is fixed via an exported-env fallback in the generated app CMakeLists (porting-guide gotcha).
  **On-device boot CONFIRMED** (USB/MCUboot DFU flash worked): Zephyr `main()` + the separate EHS kernel thread run,
  `EhsInit()` completes, `EhsMain()` runs and its console debug is visible on the UART — it loops on
  `Couldn't open SODL file t.sdl … Parsing DONE. FAILED!` with Zephyr `fs: mount point not found`. So the ONLY thing
  missing is an app + a filesystem to load it from; `target_file.c`'s POSIX `open()` is already correctly routed to
  Zephyr fs. Console notes: one-shot boot messages are lost in the USB-CDC enumeration window (repeating markers survive —
  hence the `M` heartbeat + a 3 s pre-marker delay in `target_main.c`); and each console line currently prints twice
  (cosmetic double-emit, tidy later). USB-DFU key caveat: our zip is signed with MCUboot's default debug key — if the
  on-device MCUboot rejects it, SWD-flash `merged.hex` once, then USB DFU works (`scripts/build-deploy/zephyr-nrf/README.md`).
  See porting guide § *Building for the nRF Connect SDK (NCS)*.

- **IN PROGRESS — LittleFS + first app (blinky), to get an app booting.** The kernel loads its default SODL from
  `<install-root>/appdata/temp/t.sdl` (`EHS_DEFAULT_SODL_NAME`); nothing is mounted, so it fails. Plan (no eRT/kernel
  changes — the file HAL + `EHS_FILESYSTEM_SUPPORT=posix` are already right): mount a **LittleFS on the external flash**
  (the nRF9151 owns it — `jedec,spi-nor` on `&spi3`, `nordic,pm-ext-flash=&flash_ext`)
  at the eRT install-root path; author a **blinky** SODL (`elapsed_timer` → `gpio_out` → `led0`/red — the nRF9151 owns the
  RGB LED per `thingy91x_nrf9151_common.dtsi`, aliases `led0=red_led`), name it `t.sdl`, pre-load via a flashed LittleFS
  image. Enable `CONFIG_FILE_SYSTEM`+`CONFIG_FILE_SYSTEM_LITTLEFS`+`CONFIG_SPI_NOR`. Mount path/base-dir + esp32s3
  FS-template being traced. Iteration once mounted = rewrite the SODL on the FS (lighter than full DFU).

- **ROADMAP — TCP/IP debugger console (Lucid over network) for fast app iteration.** Lets Lucid push apps + stream debug
  without reflashing; needs IP, and **WiFi is the right local-dev transport** (not cellular). WiFi on the nRF9151 is
  **SDK-supported, not a multi-chip hack**: the nRF7002 is wired to the nRF9151 (`thingy91x_common.dtsi` `nrf70: wifi@1
  compatible="nordic,nrf7002-spi"`, enabled in the 9151 dtsi + board `nrf70_support.c`); the `nrf_wifi` firmware blob is
  host-loaded from `nrfxlib`. Real caveat = **RAM/flash squeeze** on 1 MB/256 KB (community "just barely fits" a WiFi
  station on the nRF9151). Four layers: (1) Zephyr net + `WIFI_NRF70` up — **DONE**; (2) eRT WiFi HAL nRF70/Zephyr
  backend for `wifi_station` — **DONE**, see below; (3) eRT sockets HAL on Zephyr (`bsdsockets` reused) — **DONE**;
  (4) `EHS_DEBUG_TCPIP_CONSOLE=yes` → `console_server.c` + TCP → Lucid — **DONE, build-verified only**.
  Full write-up in the porting guide § *The Lucid TCP console on Zephyr (nRF9151)*: enabled via
  `EHS_DEBUG_TCPIP_CONSOLE=yes` + `EHS_COMMS_API_SUPPORT=bsdsockets`, listening on TCP 11425. Cost **+4,720 B flash,
  +2,616 B RAM**; the console server thread is free (existing 8×4096 EHS thread pool). Four non-obvious fixes were
  needed — `linux/netlink.h`, the os-arch `target_tcp.h` shadowing the backend header via a shared include guard,
  the console ring buffers never being allocated on this target (NULL deref, invisible at build time), and the
  desktop-sized 124 KB default TCP buffers. **Not yet exercised against Lucid on hardware.**

- **`SECURE FAULT @ 0x0` on every KILL APP — FIXED, cause was `k_timer_init()` on a running timer.**
  `EhsTgtTimer_reset()` called `k_timer_init()`, which re-runs `z_init_timeout()` and zeroes the timer's
  timeout dnode *while Zephyr's global timeout list still links to it*; the next `z_add_timeout()` anywhere
  in the system then stored through a NULL `prev`. Reset now uses `k_timer_stop()`, with `k_timer_init()`
  behind a one-shot flag. Details + the addr2line recipe in the porting guide § *`k_timer_init()` is
  construction, not reset*. **Lesson for next time: resolve the fault PC with `arm-zephyr-eabi-addr2line -i`
  against `zephyr.elf` BEFORE theorising** — two rounds were spent on plausible-but-wrong stale-pointer
  theories that the address would have ruled out in one step.

- **KERNEL CHANGE (Aug 2026) — `libehs_ehrt1.a` for cortex-M33 was rebuilt.** A `SECURE FAULT /
  Attribution unit violation @ 0x0` on every `KILL APP` (i.e. whenever Lucid pushes a new app) was a stale
  static in `EHS-kernel/Common/Kernel/app_data.c`: `EhsDestroyObjList`'s nodes are `readonlyAlloc`'d into
  `APP_SODL`, but the head was only cleared on the successful-teardown path, while `EhsApplicationInit()` and
  `EhsApplicationReset()` both flush that pool. A failed parse therefore left it dangling and the next
  teardown called a zeroed `funcPtr`. Fixed by nulling the head at both flush sites, NULL-guarding
  `EhsIterateDestructorList`, and checking the allocation before writing in `EhsAddToDestructorList`. Worked
  example now in `../EHS-kernel/docs/memory-pools.md`. **Anyone pulling this repo needs the matching kernel
  archive** — rebuild with `cd ../EHS-kernel && ./configure zephyr_arm_cortexm33_ehrt1 && make all_docker`.
  Affects every cortex-M33 Zephyr target, strictly a safety improvement.

- **Fault dumps now name the thread.** `EhsHThread_execute` was accepting `_szThreadname` and discarding it,
  so every EHS dynamic thread showed as `(unknown)`. Now passed to `k_thread_name_set` with
  `CONFIG_THREAD_NAME=y` / `CONFIG_THREAD_MAX_NAME_LEN=16` (~368 B). This is what identified the fault above
  as `ehs_main` rather than the Wi-Fi or console thread, after a round of guessing without it.

- **RAM accounting on this target: "free" == the application heap, not slack.**
  `CONFIG_COMMON_LIBC_MALLOC_ARENA_SIZE=-1` means all remaining system RAM becomes the malloc arena, and
  `EhsTMem_alloc` is plain `malloc` — so `hal_mem.c`'s kernel pools, SODL parsing, FB instances and the console
  queues all come out of exactly the bytes the linker reports unused. The Lucid console took that from ~9.4 KB to
  ~6.8 KB (less ~512 B for the queues). Any future static-RAM growth is a direct subtraction from what the running
  application can allocate; watch for kernel/SODL allocation failures rather than link failures.

- **`wifi_station` HAL backend for Zephyr/nRF70 — build-verified, NOT yet validated on hardware.**
  `target/os-arch/zephyr-arm/target_wifi.c` implements the cross-target contract in
  `target/Component-HAL/wifi/wifi_station.h` against `net_mgmt` + the `wifi_ready` library, modelled on the ESP32-S3
  backend. Design notes (worker thread, wifi_ready-as-STA_START, no scan-abort, HAL-driven DHCP start) are in the
  porting guide § *WiFi Subsystem* → *Zephyr / nRF70 backend notes*. Enabled by `EHS_NETWORK_WIFI_SUPPORT = yes` +
  `EHS_COMPONENT_NETWORKING_SUPPORT = no-curl` alongside the existing `EHS_WIFI_SUPPORT = zephyr_nrf70`.
  The old bring-up test (`wifi_connect_test.c`) is now off by default behind `EHS_ZEPHYR_WIFI_CONNECT_TEST=yes`.
  **To exercise it:** `target/os-arch/zephyr-arm/target_wifi_test.c`, run via
  `make TEST_FUNC=test_wifi_station zephyr_cmake_gen && make TEST_FUNC=test_wifi_station zephyr_build_docker`.
  Credentials are two `#define`s at the top of that file (placeholders as shipped). This also added the first
  TEST_FUNC hooks to the Zephyr `target_main.c` (both `ERT_INIT=none` bare-metal and the default full-init mode)
  — Zephyr had none before. Note `zephyr_build_docker` does *not* depend on `zephyr_cmake_gen`, so `TEST_FUNC`
  has to be on the **generate** step to have any effect.
  **First hardware run (Aug 2026) found and fixed the RPU-init failure.** `CONFIG_NRF70_RX_NUM_BUFS=2`
  (a previous "bare-minimum viability probe") divided to 0 buffers per RX queue — the driver splits the value
  across 3 queues with integer division and Kconfig has no range check — so `nrf_wifi_fmac_init_rx()` returned
  failure with no reason logged and the whole RPU/supplicant stack cascaded into "RPU context not initialized".
  Now 6 (Nordic's own low-memory figure) via `ERT_ZEPHYR_NRF70_RX_NUM_BUFS` in `wifi_nrf70.mk`, with a
  build-time guard that hard-errors on <3 or a non-multiple of 3. Full write-up in the porting guide
  § *Two nRF70 traps that cost a bring-up cycle* — which also documents the second finding, that
  `net_if_up()` and `wifi_ready` both report success even when RPU bring-up failed (upstream NCS v2.9.0
  bug: `nrf_wifi_if_start_zep` leaks a 0 `ret` from its mutex lock onto the error path). The HAL now
  detects that via a scan the driver refuses while unassociated.
  **Second hardware run got past RPU init and hit the heap**, exactly where the first fix's note predicted.
  `<err> wifi_supplicant: Failed to initialize supplicant control interface`, then no wifi_ready and a 30 s
  timeout. Cause: `struct spair` embeds `buf[CONFIG_NET_SOCKETPAIR_BUFFER_SIZE]` and is k_malloc'd per
  ENDPOINT, so a socketpair costs 2x that; Zephyr defaults it to 4096 when the supplicant is on, and the
  supplicant opens two pairs (control + monitor) = ~16.8 KB of heap. With the RX buffers now genuinely
  allocated there was no longer room for the second pair. Fixed with
  `CONFIG_NET_SOCKETPAIR_BUFFER_SIZE=1024` (2x the supplicant's own 512-byte MAX_CMD_SIZE/MAX_RESPONSE_SIZE;
  scan results do not use this path), recovering ~12.4 KB. Third trap in the porting-guide section.
  **Open on this:**
  - **The heap has no build-time protection and is the standing suspect on this board.**
    `CONFIG_HEAP_MEM_POOL_IGNORE_MIN=y` hides the fact that subsystems declare they need 213 KB
    (NRF70 150000 + SOCKETPAIR 32000 + HOSTAP 30000 + MQUEUE 1024) and we supply 60000. Re-check
    `grep HEAP_MEM_POOL_ADD_SIZE <build>/app/zephyr/.config` after any Kconfig change. `ERT_ZEPHYR_NRF70_HEAP_EXTRA`
    is the knob, but +10 KB overflows RAM by ~800 B, so new heap consumers have to be paid for by cutting
    another. Measured dead end: removing `CONFIG_LOG_MODE_IMMEDIATE` costs ~2 KB MORE (deferred mode
    allocates a ring buffer).
  - Both fixes are build-verified only — everything past supplicant init (associate / DHCP / reconnect /
    scan-list) is still unrun on device.
  - **RAM is at 96% of the 216 KB app region** (FLASH 95%). The scan buffers
    (`EHS_ZEPHYR_MAX_AP_RECORDS`, 2 × 20 records) and the 5200-byte worker stack are the obvious levers if
    something else needs space; `CONFIG_HEAP_MEM_POOL_SIZE=60000` and `CONFIG_NRF70_RX_NUM_BUFS` are the bigger ones.
  - Credentials don't persist: `EHS_NVS_SUPPORT` is `stubbed` on Zephyr, so `target_main.c` seeds
    `CONFIG_WIFI_CREDENTIALS_STATIC_*` at every boot. A Zephyr NVS backend under `target/Component-HAL/nvs/`
    would replace that; the seeding block in `target_main.c` should then be deleted.
  - Enterprise (802.1X) returns `WifiStation_NotImplemented`.
  - There is no serial console on this target (`EHS_SERIAL_CONSOLE_SUPPORT` unset), so the console `wifi`
    commands aren't available — drive it from the `wifi_station` FB or the seeded credentials.

- **TODO — share the LittleFS-image build step (don't duplicate esp32's).** esp32 packs the SODL app tree into a
  LittleFS image via `target/envbuildscripts/targetenv_littlefs.sh` (`littlefs-python create … --image bin/app_data.bin appdata`);
  it warns/no-ops for non-esp32 targets. The `littlefs-python create` core is target-agnostic — factor a shared FS-image
  step parameterised by block-size / fs-size / partition / mount subdir, reused by esp32 + Zephyr, instead of a Zephyr copy.
  Modest refactor; not blocking (Zephyr can call `littlefs-python create` directly for now). Also revisit provisioning:
  the FS partition currently auto-formats blank on first boot — options to load `t.sdl` are (a) flash a prebuilt LittleFS
  image to the partition, (b) embed the SODL and copy-to-FS on first boot (esp32 does this too), or (c) live upload once
  the serial/TCP console input path is wired.

- **TODO — honour the project `pm_static.yml` (external-flash LittleFS partition).** The board's
  `thingy91x_nrf9151_pm_static_no_tfm.yml` takes precedence in sysbuild, so our app-dir `pm_static.yml` (1 MB on external
  flash) is ignored; PM dynamically placed `littlefs_storage` as **24 KB in internal flash** (`0xfa000–0x100000`) instead.
  Works for a small blinky SODL, but to get the intended 1 MB external-flash FS, deliver the static file the way NCS
  sysbuild expects (board-qualified name / `APPLICATION_CONFIG_DIR` / `PM_STATIC_YML_FILE`) — needs verification.

- **TODO — a thin `targetenv_zephyr` packager (eventually).** Signing + `merged.hex` + `dfu_application.zip` are inherent
  outputs of the sysbuild `west build` (CMake-master fuses compile+sign+package), so they are *not* a separable
  compile-then-package step like ESP32's `targetenv_esp32`. A future `targetenv_zephyr`/`zephyr_package` target would sit
  *on top* — DFU/OTA artefact selection, version stamping, OTA upload (cf. `esp32_upload_to_OTA_server.sh`) — not the
  signing itself. Low priority until an OTA/update story is needed.

- **TODO — share the SDK source tree across targets instead of fetching ~7 GB per target.**
  Today each Zephyr/NCS target runs `west init/update` into its own `ehs_env-<target>/zephyr-staging/west-workspace`
  (~7 GB), even when the `(manifest, version)` is identical — pure duplication, and the direct cause of the
  disk-full wall hit during the Thingy:91 X bring-up. The SDK *source* tree is byte-identical for all targets
  sharing an SDK; only the per-target *build dir* (and out-of-tree *app dir*) differ.
  **Rationale it's safe:** Zephyr keeps source read-only and writes all config/board/product state into `-d <build_dir>`
  (see porting guide § *Is one shared tree safe for many targets? Yes*). So one shared checkout + per-target build dirs
  is correct; the only constraints are one-workspace-per-`(manifest,version)`, per-target `-d`, and no `west update`
  mid-build.
  **Plan:** fetch/host the SDK source once per `(manifest, version)`, point targets at it via `ZEPHYR_BASE`
  (`zephyr_build.sh` already skips init/update when `ZEPHYR_BASE` is set — the code path exists). Options ranked in
  porting guide § *SDK source hosting — entities and options* (shared on-disk checkout = zero-infra; contrib-prepared
  or baked private image = house-pattern, private registry required because NCS `nrfxlib` blobs can't be published).
  **Phase split — DONE.** The `west init/update` SDK fetch used to live inside `zephyr_build.sh`, conflated with the
  compile, in the *same* Docker container that then went on to compile — which fails outright on hosts that
  sandbox Docker's default bridge network off the internet (the `Cloning manifest repository from … sdk-nrf`
  failure). It is now its own step, `target/envbuildscripts/zephyr_prepdeps.sh` launched via
  `zephyr_prepdeps_docker.sh` / `make zephyr_prepdeps`, wired in as a `make` prerequisite of both `zephyr_build` and
  `zephyr_build_docker`. It still runs inside Docker — `west` is deliberately never required on the host, the image
  already has it — but that container is launched with **`--network host`** (`EHS_DOCKER_EXTRA_RUN_ARGS`, read by
  `target_buildenv_run_command.sh`), which uses the host's own network stack directly and works around the common
  case of the bridge network specifically being blocked while the host's own traffic isn't. `zephyr_build.sh` no
  longer touches the network at all — it errors out pointing back at `zephyr_prepdeps` if the workspace isn't there,
  and its own container keeps normal (non-host) network isolation since it never needs the network. If
  `--network host` still can't reach the internet (genuinely offline/air-gapped), `zephyr_prepdeps.sh` checks
  connectivity to the west manifest up front and fails with an explicit, actionable error instead of hanging or
  producing a confusing partial-clone failure — there's no further automatic workaround for that case. It also
  self-heals a failed/interrupted fetch: it gates completion on a stamp file written only after `west update`
  succeeds (not on `.west/` existing, which `west init` creates before any real transfer happens) and wipes whatever
  was partially cloned on any failure so a retry always starts clean. **Still per-target** — this only fixed *where*
  the fetch runs and *that it can't silently look done when it isn't*, not the *duplication*. The cross-target
  sharing below (a single workspace reused by every target on the same SDK) is still open and unscheduled.
  **Open decision — where the shared source lives.** `TARGET_TREES/` is really the *post-build product-assembly*
  area, though Android/Unity already stage-and-build copied-out projects inside it (`targetenv_unity_export.sh` →
  `TARGET_TREES/ehs_env-<target>/…`). Choices: (a) a new sibling `../TARGET_SRC_STAGING/` for *pre-build* source
  staging — semantically cleanest, but a new top-level convention that ideally should also absorb the Unity/Android
  copy-outs to avoid a half-applied split; (b) a shared, non-per-target subtree under the existing root, e.g.
  `TARGET_TREES/sdk-src/zephyr-<manifest>-<version>/` — less churn, no new convention. Either way the artefact is
  keyed by `(manifest, version)` and consumed via `ZEPHYR_BASE`. **Direction is committed** (porting guide
  § *Planned direction (committed)*); only the method + location are open, and implementation is not yet scheduled.
  **Keying — note it is NOT the `target_libs` key.** The SDK *source* is arch-independent (the same tree compiles
  for cortex-m33 and cortex-m4), so it is keyed purely by SDK identity `(manifest, version)` — e.g. `zephyr-v4.1.0`,
  `ncs-v3.3.0` — with **no** OS-ARCH in the path. That is orthogonal to `ert-contrib-middleware/target_libs/<OS-ARCH+variant>`
  and the EHS kernel archive, which *are* per-ABI because they hold *compiled* `.a` files. Worked example: nRF5340 DK
  (m33) and RAK4631 (m4) both use upstream zephyr v4.1.0 → **one** shared source tree, **two** ABI lib sets; the
  Thingy:91 X (m33, NCS v3.3.0) shares its ABI with the nRF5340 but needs its **own** source tree. So 3 targets →
  2 source trees + 2 ABI lib sets, different groupings. The per-target compiled output stays in the per-target build dir.

- **TODO — Nordic (`LicenseRef-Nordic-5-Clause`) compliance guards. Audited Aug 2026: compliant today, one unguarded path.**
  The licence permits use only in connection with Nordic Semiconductor ICs, so the risk is Nordic-licensed code
  reaching a **non-Nordic** build. The audit found **nothing Nordic vendored**: 0 files matching `LicenseRef-Nordic`
  across `ert-components`, `ert-build-support` and `ert-contrib-middleware`, and no `libmodem`/`nrfxlib`/`nrf_modem*`
  in-tree (the two `Nordic Semiconductor` text hits are vendor-list prose in `docs/inxware-edge-ml.md` and
  `docs/ert-porting-guide.md`). Nordic code enters **only at build time**, via the west workspace, gated by
  `ERT_ZEPHYR_MANIFEST`: empty ⇒ upstream Zephyr (Apache-2.0), `https://github.com/nrfconnect/sdk-nrf` ⇒ NCS.
  Only `zephyr_arm-nrf9151_thingy91x-base` sets it (inherited by `-cellular` / `-wifi`); `nrf52840`,
  `nrf52840_rak4631` and `nrf5340_nrf5340dk` use upstream Zephyr and pull no NCS at all. So every NCS use is on
  nRF silicon. Also verified already-correct: the nRF Kconfig fragments are included by **name derivation**
  (`cellular_$(EHS_CELLULAR_SUPPORT:zephyr_%=%).mk` → `cellular_nrf91.mk`, same for `wifi_nrf70.mk`), so only a
  backend literally named `zephyr_nrf91` / `zephyr_nrf70` can pull them in. Outstanding work, in priority order:
  1. **Guard the NCS-manifest ⇒ Nordic-SoC implication — this is the actual violation path.** Nothing stops a new
     non-Nordic Zephyr ARM target inheriting NCS by copying `thingy91x-base/config.mk` or pointing
     `ERT_ZEPHYR_BASE_PLATFORM` at it; NCS would then be compiled for non-Nordic silicon, silently. Add a
     `$(error …)` in `target/os-arch/zephyr-arm/target.mk` when `ERT_ZEPHYR_MANIFEST` is non-empty and the SoC is
     not Nordic. **A board-name regex is fragile** — `ERT_ZEPHYR_BOARD` is plain `rak4631` for the nRF52840 RAK
     module — so prefer an explicit `ERT_SOC_VENDOR = nordic` in the platform config. The check is one-directional
     (NCS ⇒ Nordic board, not the converse), so the upstream-Zephyr nRF targets are unaffected.
  2. **Gate the nRF-specific OS-port objects on the backend, not the subsystem.** `target.mk` adds
     `target_cellular.$(OBJ)` / `target_cellular_test.$(OBJ)` whenever `EHS_CELLULAR_SUPPORT` is merely not
     none/stubbed, and `target_wifi*.$(OBJ)` likewise — yet those files are nRF-only (`nrf_modem_at.h`,
     `modem/lte_lc.h`, `net_mgmt` + nRF70). Should be `ifeq ($(EHS_CELLULAR_SUPPORT),zephyr_nrf91)`, mirroring what
     `target/Component-HAL/cellular/cellular.mk` already does for the stubbed backend. A wrong backend name today
     gives a build break rather than a licence leak, so this is shape not breach — but fix it with (1).
  3. **Rename to match content:** `target_cellular.c` → `target_cellular_nrf91.c`, `target_wifi.c` →
     `target_wifi_nrf70.c` (plus the `_test` harnesses). The sibling `.mk` files already carry the SoC in the name;
     only the `.c` files claim a generality they do not have, under a generically-named `os-arch/zephyr-arm/`.
  4. **Write `docs/licensing.md`** recording the position: our source is LGPLv3; NCS is `LicenseRef-Nordic-5-Clause`
     and used only for nRF targets; other Zephyr targets are Apache-2.0 upstream Zephyr; nothing Nordic is vendored
     or redistributed (`libmodem.a` / nrfxlib stay in the west workspace and are linked only into nRF91 images).
  5. **Make it a standing CI check** in `SystemTests/CI/validate_all.sh`: fail if any repo file contains
     `LicenseRef-Nordic`, and fail if a platform sets `ERT_ZEPHYR_MANIFEST` without a Nordic SoC marker. Two greps,
     and it turns a point-in-time audit into a guarantee.
  **Explicitly NOT covered by this audit:** whether any of our code is a derivative work *by transcription* — an AT
  command sequence or struct layout copied out of NCS source rather than taken from public documentation. The
  cellular backend calls the NCS API rather than reproducing it, which is ordinary API use, but a defensible legal
  position is a different exercise from a grep and would need a human review.

- **Legacy `zephyr_arm-nrf52840` (base) not migrated.** It predates the two-step CMake-master
  workflow (old `ZEPHYR_BUILD_DIR` style, no `EHS_BUILD_MONOLITHIC_KERNEL`) and so still links the
  `ehs_kernel_stub.c` no-op kernel. Migrate it to the workflow used by the two maintained targets,
  or retire it.
