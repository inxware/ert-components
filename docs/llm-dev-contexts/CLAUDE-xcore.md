# XMOS xcore.ai Target — Claude Developer Context

This file contains operational detail for building, debugging, and porting XMOS xcore.ai
FreeRTOS targets in ert-components. For human-readable background (vendor / SDK / image
table) see `docs/ert-porting-guide.md` line 378.

> **CDF port argument numbers:** canonical spec in `../ert-porting-guide.md` § "Port Argument Numbers" — each port type has its own independent 1..N range; `argument="0"` is the trigger.

The xcore target is **work in progress**: Phase 1 (the `.a` archive) currently builds clean,
Phase 2 (the `.xe` link) does not yet produce firmware. See "Phase 2 status" below.

---

## Two-phase build workflow

```bash
./configure xcore_freertos-xcore-base    # writes TARGET.cfg
make all_docker                           # Phase 1 — produces ehs_<TARGET>.a
make targetenv_xmos_docker                # Phase 2 — produces ehs.xe (incomplete)
```

Phase 1 compiles every eRT translation unit with the XMOS `xcc` compiler in the proprietary
XTC Tools docker image, then archives the objects into `ehs_<TARGET>.a` with `xmosar`. No
SDK libs are linked at this stage — the SDK is consumed *only* as headers, by pointing
`INC_DIRS` directly at `ert-contrib-middleware/contrib/xmos-sdk/fwk_*` source trees ("Option
B" — see `target/os-arch/xcore_freertos-xcore/target.mk:31-46`).

Phase 2 invokes `cmake` against `ert-contrib-middleware/contrib/xmos-sdk/xcore-ehs-app/`,
which is meant to link the Phase 1 archive together with `fwk_rtos`/`fwk_core` library
targets and the board `.xn` file to produce the final `.xe`. **This step currently fails at
the cmake-configure stage — see "Phase 2 status".**

Why two phases: `xcc` is not GCC-compatible enough to link a normal eRT build directly;
xcommon_cmake / fwk_rtos provides macros and module aggregates that have to drive the link.
Splitting lets eRT keep its existing per-source compile pipeline while delegating the final
link to native XMOS tooling.

---

## Key make variables (set in `target/platform/<TARGET>/config.mk`)

| Variable | Example | Effect |
|---|---|---|
| `EHS_ARCH` | `xcore` | Architecture identifier — drives directory layout under `target/os-arch/` |
| `EHS_OS` | `xcore_freertos` | OS identifier |
| `TOOLCHAIN_NAME` | `xtc-tools-15.x` | XTC version; matches the docker image tag |
| `COMPONENT_BASE_TECHNOLOGIES` | `xcore_freertos-xcore-xtc-15.x` | Selects `target_libs/<variant>/` — currently unused on xcore (Option B) |
| `XMOS_BOARD_TARGET` | `XCORE-AI-EXPLORER` | xcc `-target=` board name; resolves against `XCC_TARGET_PATH` to find the `.xn` file |
| `XMOS_BSP_CONFIG` *(planned)* | `XCORE-AI-EXPLORER_1V1` | fwk_rtos BSP variant — not yet implemented; see todo #1 |

Standard MCU exclusion flags (set in `target/platform/xcore_freertos-xcore-base/config.mk`):
```makefile
EHS_FILESYSTEM_SUPPORT          = posix
EHS_COMPONENT_NETWORKING_SUPPORT = nocurl
EHS_COMMS_API_SUPPORT           = none      # network HAL not implemented yet
EHS_EXCLUDE_XML_PARSER          = yes
EHS_SKIP_APPLICATION_INFO_GETTER = yes      # dirent.h not supported on xcore
EHS_GUI_SUPPORT                 = none
EHS_AV_SUPPORT                  = none
EHS_ML_SUPPORT                  = none
EHS_DEBUG_TCPIP_CONSOLE         = stubbed   # required when EHS_DEBUGALL=yes; see gotchas
```

---

## Validated Docker image / XTC Tools pairings

| `Dockerimagename` | XTC Tools | Notes |
|---|---|---|
| `inxware/xcore_ubuntu22.04-xtc-tools` | 15.3.1 (15.x) | **Current** — image must be built locally with `make build_docker_local` because XTC Tools are proprietary and cannot be redistributed |

The Docker image is named in `target/platform/<TARGET>/Dockerimagename`. The XTC Tools
tarball is downloaded manually and placed at
`target/platform/xcore_freertos-xcore-base/xtc-tools-linux.tgz` before
`make build_docker_local`. The Dockerfile bakes `XMOS_TOOL_PATH` and `PATH` into the image
but **does not** source `SetEnv` — so `XCC_TARGET_PATH`, `XCC_DEVICE_PATH`,
`XCC_LIBRARY_PATH`, `XCC_*_INCLUDE_PATH`, `XMOS_HOME`, etc. are unset until the build script
sources `SetEnv` itself. See `target/envbuildscripts/targetenv_xmos.sh` stage 2.

---

## SDK source layout (ert-contrib-middleware)

The XMOS SDK is consumed from cloned source repos under
`ert-contrib-middleware/contrib/xmos-sdk/`. Cloning is performed by
`ert-contrib-middleware/inx_build_scripts/build-xcore-freertos-ehs.sh` on the host (no
docker, no XTC Tools needed for the clone itself). Phase 1 references these read-only via
`INC_DIRS`; Phase 2 references them via `XMOS_SANDBOX_DIR`.

| Repo | URL | Phase 1 use | Phase 2 use |
|---|---|---|---|
| `fwk_rtos` | github.com/xmos/fwk_rtos | Headers (FreeRTOS SMP kernel, osal, rtos_support) | `add_subdirectory()` → `rtos::*` targets |
| `fwk_core` | github.com/xmos/fwk_core | Headers (`xcore_utils.h`) | `add_subdirectory()` → `core::*` targets |
| `fwk_io` | github.com/xmos/fwk_io | (not used) | (probably not used; review at link time) |
| `xcommon_cmake` | github.com/xmos/xcommon_cmake | (not used) | xcc toolchain + macros |
| `lib_xcore_math` / `lib_i2c` / `lib_uart` / `lib_i2s` / `lib_qspi_fast_read` | github.com/xmos/lib_* | Cloned but unused at present | TBD — link if a peripheral HAL needs them |

Two `fwk_rtos` git submodules use SSH URLs and are cloned via HTTPS workarounds in
`build-xcore-freertos-ehs.sh`:

| Path | Source |
|---|---|
| `fwk_rtos/modules/FreeRTOS/FreeRTOS-SMP-Kernel` | github.com/FreeRTOS/FreeRTOS-Kernel (branch: `smp`) |
| `fwk_rtos/modules/FreeRTOS/FreeRTOS-Plus-TCP` | github.com/xmos/FreeRTOS-Plus-TCP |

---

## fwk_rtos library target map (for Phase 2)

`fwk_rtos` is a CMake project (`project(framework_rtos)`), **not** an xcommon-style module
with `lib_build_info.cmake`. Phase 2's CMakeLists.txt needs to consume it via
`add_subdirectory(${XMOS_SANDBOX_DIR}/fwk_rtos)` and link the alias targets directly.

Target tree (defined in `fwk_rtos/modules/CMakeLists.txt`):

```
rtos::iot                      ← rtos::freertos + rtos::wifi + http + json + mqtt + sntpd + tls
rtos::freertos                 ← drivers::general + osal + rtos_support + sw_services::general
                                 + FreeRTOS::FreeRTOS_SMP + framework_core_multitile_support
rtos::wifi                     ← drivers::wifi + FreeRTOS-Plus-TCP + sw_services::dhcp
                                 + sw_services::wifi_manager
rtos::drivers::general         ← clock_control + gpio + i2c + intertile + l2_cache + qspi_io
                                 + rpc + spi + swmem + uart
rtos::drivers::audio           ← i2s + mic_array
rtos::drivers::all             ← drivers::general + drivers::audio + trace + usb + wifi
rtos::drivers::<peripheral>    ← individual driver, e.g. rtos::drivers::gpio
rtos::FreeRTOS::FreeRTOS_SMP   ← SMP kernel + xcore.ai port (port.xc, portasm.S, heap_4.c)
rtos::osal                     ← OSAL FreeRTOS adaptor
rtos::rtos_support             ← rtos_printf, RTOS support macros
```

`fwk_core` (defined in `fwk_core/modules/CMakeLists.txt`):

```
core::all
core::general
framework_core_multitile_support   (linked transitively by rtos::freertos)
```

`fwk_io` (defined in `fwk_io/modules/CMakeLists.txt`):

```
io::all
io::general
io::audio
```

For an eRT app the minimum link set is: `rtos::freertos` + the Phase 1 `ehs_lib` archive.
Add board init via `framework_rtos_board_support_config_<XMOS_BSP_CONFIG>` (defined in
`fwk_rtos/modules/bsp_config/CMakeLists.txt`) — see "Phase 2 status" for the BSP decision.

---

## Phase 2 status (as of 2026-04-29)

`make targetenv_xmos_docker` succeeds through `cmake` toolchain detection but fails at the
configure step with:

```
include could not find requested file:
    .../fwk_rtos/fwk_rtos/lib_build_info.cmake
```

**Why.** `xcore-ehs-app/CMakeLists.txt:47` declares `APP_DEPENDENT_MODULES fwk_rtos fwk_io
fwk_core` and `xcommon_cmake`'s `XMOS_REGISTER_DEPS` looks up
`<XMOS_SANDBOX_DIR>/<dep>/<dep>/lib_build_info.cmake` — which does not exist for any of the
fwk_* repos because they are CMake projects, not xcommon modules. The fix is to drop
`XMOS_REGISTER_APP()` and use plain `add_subdirectory()` + `add_executable()` instead (see
todo #4 below).

**Architectural decision still open: BSP or no BSP.**
- *Use BSP* — link `framework_rtos_board_support_config_XCORE-AI-EXPLORER_1V1` (or `_2V0`).
  Adds `platform_init.c`, `platform_start.c`, `aic3204.c`, `app_pll_ctrl.c`,
  `driver_instances.c` to the link. Pulls in audio/usb drivers eRT doesn't use (bloat,
  no functional harm). Requires a one-line shim in `target/os-arch/xcore_freertos-xcore/target_main.c` to call
  `platform_init(); platform_start();` before starting the FreeRTOS scheduler.
- *Skip BSP* — cleaner link footprint, but xcore tile clocks and I/O ports may not come up
  → silent runtime hang. Would need a minimal hand-rolled clock/PLL init in `target_main.c`.

Picking the BSP variant (`_1V1` vs `_2V0`) requires knowing the silicon revision of the
XCORE-AI-EXPLORER PCB in use. This is a *separate* concept from `XMOS_BOARD_TARGET` — the
latter selects the xcc `.xn` file (silicon + memory layout); the BSP variant selects board
init code (PLL frequency, codec wiring, GPIO instances).

---

## Known landmines (won't surface until first link attempt)

### `framework_core_multitile_support` and single-tile `main()`
`rtos::freertos` transitively links `framework_core_multitile_support`, which typically
expects the application's `main` to be an `.xc` file declaring `par { on tile[0]: …; on
tile[1]: …; }`. eRT's `target/os-arch/xcore_freertos-xcore/target_main.c:65` is plain C
`int main(void)`. `target_main.c:14` claims `xcc` treats this as the tile-0 entry, but the
multitile aggregate may still reference symbols only present in an XC main. May either link
clean (tile[1] empty) or fail with undefined references — won't know until first link.

### `FreeRTOSConfig.h` reachability
Phase 1 finds `FreeRTOSConfig.h` via `INC_DIRS += target/os-arch/xcore_freertos-xcore/`
(`target.mk:27`). Phase 2's executable target needs the same directory on its include path
so the FreeRTOS kernel sources injected by `rtos::FreeRTOS::FreeRTOS_SMP` can find it.
Symptom if missed: `FreeRTOSConfig.h: No such file` from inside the kernel sources.

### `XMOS_BOARD_TARGET` propagation
The platform `config.mk` sets `XMOS_BOARD_TARGET` as a make variable. It does **not**
currently flow through `targetenv_xmos_docker.sh` into the docker run as an env var, so
`targetenv_xmos.sh` falls back to a hard-coded default and emits
`WARNING: XMOS_BOARD_TARGET not set`. The fallback value happens to match the platform
config's value, so this is benign today but will silently mismatch if a new platform sets a
different board.

---

## Phase 2 todo list

| # | Where | Item |
|---|---|---|
| 1 | ert-components: `target/platform/xcore_freertos-xcore-base/config.mk` | Add `XMOS_BSP_CONFIG ?= XCORE-AI-EXPLORER_1V1` (or document explicitly as `none` to skip BSP). New variable — `XMOS_BOARD_TARGET` covers the xcc `-target=` name only. |
| 2 | ert-components: `Makefile` / `target/envbuildscripts/all_docker.sh` / `targetenv_xmos_docker.sh` | Pass `XMOS_BOARD_TARGET` and `XMOS_BSP_CONFIG` into the docker run via `-e`. Mirrors the `EHS_TARGETENV_PREBUILD_*` passthrough convention in `target_buildenv_run_command.sh:46-52`. |
| 3 | ert-components: `target/envbuildscripts/targetenv_xmos.sh` | Drop the `WARNING: XMOS_BOARD_TARGET not set` fallback once #2 lands; read `XMOS_BSP_CONFIG` from env and forward as `-DXMOS_BSP_CONFIG=…` to cmake. |
| 4 | **ert-contrib-middleware**: `contrib/xmos-sdk/xcore-ehs-app/CMakeLists.txt` | Rewrite: drop `APP_DEPENDENT_MODULES` and `XMOS_REGISTER_APP()`. Replace with `add_subdirectory(${XMOS_SANDBOX_DIR}/fwk_core ${CMAKE_BINARY_DIR}/fwk_core)` and `…/fwk_rtos` (in that order). Then `add_executable(ehs_xe src/main_stub.c)` + `target_link_libraries(ehs_xe PRIVATE rtos::freertos ehs_lib)`. |
| 5 | **ert-contrib-middleware**: same CMakeLists.txt | Conditionally link `framework_rtos_board_support_config_${XMOS_BSP_CONFIG}` when `XMOS_BSP_CONFIG` is set and != `none`. Architectural decision per "Phase 2 status". |
| 6 | **ert-contrib-middleware**: same CMakeLists.txt | `target_include_directories(ehs_xe PRIVATE ${EHS_ROOT}/target/os-arch/xcore_freertos-xcore)` so `FreeRTOSConfig.h` is reachable to kernel sources injected by `rtos::FreeRTOS::FreeRTOS_SMP`. |
| 7 | **ert-contrib-middleware**: same CMakeLists.txt | Pass `-target=${XMOS_BOARD_TARGET}` via both `target_compile_options` and `target_link_options`. xcommon would have done this in `XMOS_REGISTER_APP`; now ours. |
| 8 | ert-components: `target/os-arch/xcore_freertos-xcore/target_main.c` | If using BSP (#5): add `platform_init(); platform_start();` shim before the `xTaskCreate(EhsMain, …)` call. If not using BSP: hand-rolled clock/PLL init or accept silent runtime hang. |
| 9 | (verify) | First link attempt — watch for missing symbols from `framework_core_multitile_support`. |
| 10 | (verify) | First boot on hardware — confirm clocks/ports come up, FreeRTOS scheduler runs, `EhsMain` task executes. |
| 11 | ert-components: `target/Component-HAL/comms/freertos_plus_tcp/` | Future — fill in real `target_tcp.c`, `targetcomms_init.c`, `comms.mk` against `FreeRTOS_Sockets.h`; remove the transitional INC_DIRS entry in `target/os-arch/xcore_freertos-xcore/target.mk` and flip `EHS_COMMS_API_SUPPORT=freertos_plus_tcp` in the platform config. Not blocking the .xe build. |

Items #4–#7 land in `ert-contrib-middleware` (`contrib/xmos-sdk/xcore-ehs-app/` is currently
untracked there — fresh work, not part of contrib's stable contract). Everything else is in
ert-components.

---

## Known gotchas and past build failures

### `EHS_DEBUGALL=yes` auto-promotes `EHS_DEBUG_TCPIP_CONSOLE` to `yes`
`Common/Ehs/ehs.mk:44-49` promotes `EHS_DEBUG_TCPIP_CONSOLE=yes` whenever `EHS_DEBUGALL` is
set and `EHS_DEBUG_TCPIP_CONSOLE` is not explicitly `stubbed` or `target_specific`. That
pulls in `console_server.c` (which needs BSD socket types) and the `EhsSvcTcp_*` call sites
in `hal.c`. Both fail to compile on xcore because there's no comms HAL yet.

Fix: the platform `config.mk` must set `EHS_DEBUG_TCPIP_CONSOLE=stubbed` explicitly when
`EHS_DEBUGALL=yes`. Already done in
`target/platform/xcore_freertos-xcore-base/config.mk:42-47`.

### `target_tcp.h: file not found` even with `EHS_COMMS_API_SUPPORT=none`
`Common/HAL/include/hal_network.h:19` includes `target_tcp.h` *unconditionally*. When the
platform sets `EHS_COMMS_API_SUPPORT=none`, `Common/Ehs/ehs.mk:71-78` skips the comms HAL
include path, leaving no `target_tcp.h` reachable.

Fix: a stub header lives at `target/Component-HAL/comms/freertos_plus_tcp/target_tcp.h`,
and the os-arch `target.mk` adds that directory to `INC_DIRS` unconditionally. This is
transitional — the long-term home is a real `freertos_plus_tcp` Component-HAL (todo #11).

### `Ehs_UserUtime` conflicting types
`hal_file.h:279-290` and `hal_file.c:720-758` switch the prototype between `struct utimbuf
*` and `void *` based on `EHS_TARGET_FILE_SKIP_STAT`. The XMOS libc / fwk_rtos POSIX layer
does not provide `<utime.h>`, so without `EHS_TARGET_FILE_SKIP_STAT` defined the compiler
forms `struct utimbuf` as an incomplete type and the header/`.c` declarations diverge.

Fix: `target/os-arch/xcore_freertos-xcore/target_file.h` defines
`EHS_TARGET_FILE_SKIP_STAT yes`, matching the esp32 / zephyr precedent — both sides take
the `void *` stub branch.

### `xcc: unknown target 'XCORE-AI-EXPLORER'`
The Dockerfile bakes `XMOS_TOOL_PATH` and `PATH` only. xcc additionally needs
`XCC_TARGET_PATH` (search path for `.xn` files). Without it `xcc -target=XCORE-AI-EXPLORER`
fails even though the `.xn` file is present at `$XMOS_TOOL_PATH/targets/XCORE-AI-EXPLORER/`.

Fix: `targetenv_xmos.sh` stage 2 sources `${XMOS_TOOL_PATH}/SetEnv` (which is bash and
relies on `$PWD`, hence the `pushd`/`popd` wrap) before invoking cmake.

### `xcommon.cmake` reads `$ENV{XMOS_CMAKE_PATH}`, not the CMake variable
`xcore-ehs-app/CMakeLists.txt:24` uses `${XMOS_CMAKE_PATH}` (CMake variable, populated by
`-D`) but `xcommon.cmake:16` itself uses `$ENV{XMOS_CMAKE_PATH}` (env var) to locate
`xcore_xs.cmake`. Setting only the `-D` form gets past CMakeLists.txt but breaks inside
xcommon — symptom is `include could not find requested file: /xcore_xs.cmake` (just `/`
plus the filename), followed later by `No sources present to determine architecture`.

Fix: `targetenv_xmos.sh` stage 5 `export`s `XMOS_CMAKE_PATH` *and* passes it as `-D`.

### "No sources present to determine architecture" with an app source already at the root
`xcommon.cmake:495` calls `glob_srcs("APP" ${CMAKE_CURRENT_SOURCE_DIR} src)` which globs
`<source_dir>/src/*.{c,xc,cpp,S}`. Sources at the source-dir root are invisible to the
glob.

Fix: `xcore-ehs-app/main_stub.c` lives at `xcore-ehs-app/src/main_stub.c`. (Earlier
revisions had it at the root.)

---

## Adding a new xcore board

1. `./configure -new xcore_freertos-xcore-<board>`
2. Copy `config.mk` from `target/platform/xcore_freertos-xcore-base/`
3. Set `XMOS_BOARD_TARGET` to a name resolvable in `XCC_TARGET_PATH` (run
   `ls $XMOS_TOOL_PATH/targets/` inside the docker image to list options).
4. Set `XMOS_BSP_CONFIG` to the matching fwk_rtos BSP variant (see
   `fwk_rtos/modules/bsp_config/` for available variants), or `none` to skip BSP.
5. `Dockerimagename` and `Dockerfile` can be inherited (or copied if XTC version differs).
6. `make build_docker_local && make all_docker && make targetenv_xmos_docker`
7. Output (when Phase 2 is fixed): `../TARGET_TREES/ehs_env-<TARGET>/bin/ehs.xe`
