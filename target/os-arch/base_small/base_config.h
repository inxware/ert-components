/** @file base_config.h
 * Compatibility shim. The real content now lives in ert_components_config.h.
 *
 * See the fuller explanation in ../base_full/base_config.h: both repos have a
 * target/os-arch/base_small/ directory, so the generic name could resolve into
 * whichever repo the include path reached first, and 13 kernel platforms had in
 * fact been reading ert-components' sizes rather than their own.
 *
 * No unconditional #warning here on purpose - the nxp targets build with bare
 * -Werror, which would turn a deprecation warning into a build failure. Build
 * -DEHS_WARN_DEPRECATED_HEADERS to opt in, or grep for the remaining includers.
 *
 * New platform configs should include ert_components_config.h directly. Existing
 * ones keep working through this shim and migrate when touched for other reasons.
 * Delete this shim when nothing includes it.
 *
 * See EHS-kernel docs/plan-header-boundary-split.md.
 */

#ifdef EHS_WARN_DEPRECATED_HEADERS
#warning "base_config.h is deprecated - include ert_components_config.h instead (same directory)"
#endif

#include "ert_components_config.h"
