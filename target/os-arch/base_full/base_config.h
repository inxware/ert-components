/** @file base_config.h
 * Compatibility shim. The real content now lives in ert_components_config.h.
 *
 * Why the rename: EHS-kernel and ert-components both have a
 * target/os-arch/base_full/ directory, so "../base_full/base_config.h" was a
 * plausible thing to write in either repo and resolved quietly into whichever one
 * the include path happened to reach first. That is not hypothetical - 13 kernel
 * platforms shipped for an unknown length of time including THIS file instead of
 * the kernel's own kernel_config.h, and nothing failed until the two headers'
 * contents diverged. A name that states its owner cannot be included from the
 * wrong repo by accident.
 *
 * Why this shim exists rather than a sweep: 191 of the 208 platform configs
 * include base_config.h, and the published + community regression covers at most
 * 58 platforms. Renaming every includer would mean editing ~130 configs that no
 * CI run exercises. So the new name is authoritative, this shim keeps every
 * existing config working, and includers migrate opportunistically when touched
 * for other reasons. Nothing is on a deadline.
 *
 * Why there is no unconditional #warning here: it would break the build. The nxp
 * targets compile with bare -Werror (target/os-arch/nxp-redlib-freertos-arm/
 * target.mk), which promotes #warning to an error, so a deprecation warning would
 * turn 191 working platforms into failures. Build -DEHS_WARN_DEPRECATED_HEADERS to
 * opt in and list the remaining includers, or just grep for them - which costs
 * nothing and cannot break a build:
 *
 *     grep -rl base_config.h target/platform/(star)/target_config.h | wc -l
 *
 * If you are writing a NEW platform config, include ert_components_config.h
 * directly. The kernel's lint (EHS-kernel target/envbuildscripts/
 * lint_config_includes.sh, rule A) rejects new kernel-side uses of the old name,
 * so the population can only shrink. Delete this shim when nothing includes it.
 *
 * See EHS-kernel docs/plan-header-boundary-split.md.
 */

#ifdef EHS_WARN_DEPRECATED_HEADERS
#warning "base_config.h is deprecated - include ert_components_config.h instead (same directory)"
#endif

#include "ert_components_config.h"
