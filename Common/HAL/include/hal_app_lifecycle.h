/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_app_lifecycle.h
 *
 * Platform-agnostic per-app lifecycle hooks for ert-components.
 *
 * These functions exist to give each target (and each feature-support
 * component-HAL within a target) a defined point to tear down per-app
 * state so the next app load starts from a clean slate — equivalent to
 * first-boot conditions.
 *
 * The motivating problem: a "reload app" operation should leave no
 * surviving HAL-side runtime objects from the previous app. Without an
 * explicit teardown, accumulated state (Qt engines, QML root mappers,
 * subscription tables, etc.) leaks across loads and produces subtle
 * bugs (mis-routed events, dangling pointers, lockups in callback
 * queues) that disappear only on a full process restart.
 *
 * Layered implementation:
 *
 *   1. This header declares the common API the kernel / loader calls.
 *
 *   2. A per-os-arch dispatcher (target/os-arch/<arch>/...) implements
 *      the entry point and conditionally calls into feature-specific
 *      teardowns that are active for the current target's component-HAL
 *      configuration (e.g. Qt UI teardown when EHS_GUI_SUPPORT_MODE_B_QT
 *      is enabled).
 *
 *   3. Each feature-support component-HAL exposes its own teardown
 *      function (e.g. EhsTV_qtAppTeardown). The os-arch dispatcher is
 *      the only place that knows which of these to call.
 *
 * Targets that have no UI / HAL state needing per-app teardown still
 * implement the entry point — as a no-op. That keeps the kernel's view
 * uniform across platforms.
 */

#ifndef EHS_HAL_APP_LIFECYCLE_H
#define EHS_HAL_APP_LIFECYCLE_H

#ifdef __cplusplus
extern "C" {
#endif

/** Tear down all per-app HAL state for the current target.
 *
 * Called on every app load (boot and reload alike) so the path is
 * idempotent: the next ertqt_load_app / equivalent has a clean,
 * first-boot-like environment to set up against.
 *
 * Implementations live per os-arch — see
 * target/os-arch/<arch>/target_app_lifecycle.c. Each one is responsible
 * for invoking the active feature-support teardowns for that target
 * (Qt, network, etc.) under their respective build-time guards.
 *
 * MUST be safe to call from a clean state (no engine alive, no
 * subscriptions, first boot) — implementations are no-ops in that case.
 */
void EhsApp_teardown(void);

#ifdef __cplusplus
}
#endif

#endif /* EHS_HAL_APP_LIFECYCLE_H */
