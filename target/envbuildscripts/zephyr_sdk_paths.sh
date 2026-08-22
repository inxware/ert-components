#---------------------------------------------------------------
# Copyright (C) 2026, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# zephyr_sdk_paths.sh — resolve the shared Zephyr/NCS SDK source location.
#
# SCOPE — sourced, not executed. Defines EHS_ZEPHYR_SDK_ID, EHS_ZEPHYR_SDK_DIR
# and WEST_WORKSPACE for both zephyr_prepdeps.sh and zephyr_build.sh, so the
# two agree on one definition.
#
# Requires ERT_ZEPHYR_VERSION (and optionally ERT_ZEPHYR_MANIFEST) to be set by
# the caller, which reads them from the platform config or make_vars.env.
#
# ── One tree per (manifest, version), shared by every target on that SDK ─────
#
# The west workspace is the SDK *source*. It is byte-identical for every target
# built from the same manifest and version, and Zephyr never writes into it: the
# merged .config, devicetree_generated.h, every .o and the linked .elf all go to
# the per-target build directory passed as 'west build -d'. So one tree serves
# many boards, which is how Zephyr's own CI builds hundreds of configurations.
#
# The key is deliberately ARCH-FREE. The source compiles per board, so a single
# zephyr-v4.1.0 tree serves both cortex-m4 (RAK4631) and cortex-m33 (nRF5340 DK);
# only compiled artefacts — contrib target_libs and the EHS kernel archive — are
# ABI-keyed. Three targets on two SDKs need two source trees, not three.
#
# ── Why ../TARGET_SRC_STAGING and not TARGET_TREES ──────────────────────────
#
# TARGET_TREES is the post-build product-assembly area: one directory per target
# holding what that build produced. SDK source is neither per-target nor a
# product — it is a pre-build input shared across targets, so it gets its own
# sibling directory rather than being wedged into a per-target tree. That is also
# what stopped the old layout from being shareable at all: the workspace lived at
# TARGET_TREES/ehs_env-<target>/zephyr-staging/west-workspace, so its path
# encoded a single target and every target fetched its own ~7 GB copy.
#
# Override EHS_ZEPHYR_SRC_STAGING to relocate the whole store, or set ZEPHYR_BASE
# to bypass this entirely and use an existing Zephyr installation.
#
# NCS trees carry redistribution-restricted nrfxlib blobs, so this store must
# stay on local disk or a private registry — never git, never a public image.

# Directory holding every shared SDK source tree, one subdirectory per SDK.
: "${EHS_ZEPHYR_SRC_STAGING:=${PWD}/../TARGET_SRC_STAGING}"

# SDK identity: manifest short name + version. An empty manifest means the
# upstream Zephyr manifest, which is what west init uses by default.
if [ -z "${ERT_ZEPHYR_MANIFEST}" ]; then
    _ehs_zephyr_manifest_id="zephyr"
else
    # https://github.com/nrfconnect/sdk-nrf(.git) -> sdk-nrf
    _ehs_zephyr_manifest_id="$(basename "${ERT_ZEPHYR_MANIFEST}" .git)"
fi

EHS_ZEPHYR_SDK_ID="${_ehs_zephyr_manifest_id}-${ERT_ZEPHYR_VERSION}"
EHS_ZEPHYR_SDK_DIR="${EHS_ZEPHYR_SRC_STAGING}/${EHS_ZEPHYR_SDK_ID}"

# The west workspace root. zephyr/ beneath it is ZEPHYR_BASE.
WEST_WORKSPACE="${EHS_ZEPHYR_SDK_DIR}"

export EHS_ZEPHYR_SRC_STAGING EHS_ZEPHYR_SDK_ID EHS_ZEPHYR_SDK_DIR WEST_WORKSPACE
