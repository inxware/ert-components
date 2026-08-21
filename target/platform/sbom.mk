#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------

# sbom.mk — Software Bill of Materials
#
# Generates three outputs when 'make sbom' is run:
#
# 1. Forward SBOM — SPDX 2.3 tag-value format
#    sbom/$(TARGET)/SBOM.spdx
#    Lists what THIS platform depends on.  Suitable for supply-chain tooling.
#
# 2. Forward SBOM summary — human-readable markdown
#    sbom/$(TARGET)/SBOM_SUMMARY.md
#    Condensed table of dependencies with presence check on disk.
#
# 3. Reverse dependents record — appended to each dependency root
#    <dep-root>/DEPENDENTS.md
#    Records WHICH platforms consume each dependency.  Run 'make sbom' for
#    every supported platform to build a complete cross-platform usage matrix.
#    Format: pipe-separated markdown table (also valid pipe-delimited CSV).
#
# Accuracy note: dependencies are derived from the same make variables used
# in the real build (matching buildenv_checkpaths.sh).  Whether a specific
# file within a root is accessed at compile time is not tracked — path
# presence in the build configuration is the indicator.
#
# This file is conditionally included from the root Makefile when the 'sbom'
# goal is requested (see Makefile ifeq block).

# =========================================================================
# Build metadata
# =========================================================================

_SBOM_TS       := $(shell date -u "+%Y-%m-%d %H:%M UTC")
_SBOM_TS_SPDX  := $(shell date -u "+%Y-%m-%dT%H:%M:%SZ")
_SBOM_PLAT     := $(TARGET)
_SBOM_VER      := $(shell git describe --tags --always 2>/dev/null || echo "unknown")
_SBOM_IMG      := $(shell cat "$(EHS_PLATFORM_PATH)/Dockerimagename" 2>/dev/null \
                      | tr -d '[:space:]' || echo "none")

# Versions of sibling repos (best-effort; empty string if repo absent)
_SBOM_VER_CONTRIB  := $(shell git -C "$(EHS_COMPONENT_SUPPORT_BASE)" \
                          describe --tags --always 2>/dev/null || echo "unknown")
_SBOM_VER_BUILDSUP := $(shell git -C "$(EHS_CORE_SUPPORT_BASE)" \
                          describe --tags --always 2>/dev/null || echo "unknown")

# =========================================================================
# Dependency roots  (mirrors buildenv_checkpaths.sh logic)
# =========================================================================

# 1. ert-contrib-middleware package build root
#    = ../ert-contrib-middleware/target_libs/<arch_variant>/build/
_SBOM_CONTRIB_ROOT  := $(EHS_COMPONENT_SUPPORT_BUILD)

# 2. ert-build-support target-libs root for this arch
#    Contains kernel/ (EHS kernel .a) and build/ (sysroot headers + libs)
_SBOM_BUILDSUP_ROOT := $(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)

# 3. Toolchain root — only when a named (non-HOST) toolchain is used
_SBOM_TOOLCHAIN_ROOT :=
ifneq ($(TOOLCHAIN_PATH),HOST)
ifdef TOOLCHAIN_PATH
_SBOM_TOOLCHAIN_ROOT := $(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)
endif
endif

# Combined list; only roots that exist on disk are written to
_SBOM_ALL_ROOTS := $(_SBOM_CONTRIB_ROOT) $(_SBOM_BUILDSUP_ROOT) $(_SBOM_TOOLCHAIN_ROOT)
_SBOM_ROOTS     := $(strip $(foreach _d,$(_SBOM_ALL_ROOTS),$(if $(wildcard $(_d)),$(_d))))

# =========================================================================
# Output paths (forward SBOM)
# Default layout: Releases/SBOM/<ert-components-version>/<platform>/
#
# Override SBOM_OUT_DIR on the make command line to redirect output for
# CI / routine regression use (avoids polluting Releases/ with per-run data):
#
#   make sbom SBOM_OUT_DIR=/path/to/ci/results/platform/sbom \
#             SBOM_SKIP_DEPENDENTS=1
#
# update_release_report.sh (and run_regression.sh --release-report) use the
# default path so that versioned artefacts accumulate under Releases/SBOM/.
# =========================================================================

SBOM_OUT_DIR     ?= $(EHS_ROOT_PATH)/Releases/SBOM/$(_SBOM_VER)/$(_SBOM_PLAT)
_SBOM_OUT_DIR    := $(SBOM_OUT_DIR)
_SBOM_SPDX       := $(_SBOM_OUT_DIR)/SBOM.spdx
_SBOM_SUMMARY    := $(_SBOM_OUT_DIR)/SBOM_SUMMARY.md

# Set SBOM_SKIP_DEPENDENTS=1 to suppress writing DEPENDENTS.md files.
# Appropriate for CI/routine builds; release reports should leave this unset.
SBOM_SKIP_DEPENDENTS ?=

# =========================================================================
# DEPENDENTS.md table geometry (reverse record written to each dep root)
# =========================================================================
# Pipe-separated columns — valid GitHub-Flavored Markdown table and
# importable as pipe-delimited CSV (set '|' as field delimiter).
#
#   Col 1  Platform          50 chars
#   Col 2  Date/Time (UTC)   22 chars
#   Col 3  Version           20 chars
#   Col 4  Docker Image      50 chars

_SBOM_DEP_HDR := | %-50s | %-22s | %-20s | %-50s |\n
_SBOM_DEP_SEP := |:%-50s-|:%-22s-|:%-20s-|:%-50s-|\n
_SBOM_DEP_ROW := | %-50s | %-22s | %-20s | %-50s |\n

# =========================================================================
# Target
# =========================================================================

.PHONY: sbom
sbom:
ifndef TARGET
	@$(call missing_target)
else
	@echo ""
	@echo "SBOM: Generating bill of materials for platform '$(TARGET)'"
	@echo "      Version  : $(_SBOM_VER)"
	@echo "      Image    : $(_SBOM_IMG)"
	@echo "      Timestamp: $(_SBOM_TS)"
	@echo ""

	@mkdir -p "$(_SBOM_OUT_DIR)"

	@# -----------------------------------------------------------------
	@# 1. Forward SBOM — SPDX 2.3 tag-value
	@# -----------------------------------------------------------------
	@echo "--- Writing forward SBOM (SPDX 2.3): $(_SBOM_SPDX)"
	@{ \
	  echo "SPDXVersion: SPDX-2.3"; \
	  echo "DataLicense: CC0-1.0"; \
	  echo "SPDXID: SPDXRef-DOCUMENT"; \
	  echo "DocumentName: ert-components-$(_SBOM_PLAT)"; \
	  echo "DocumentNamespace: https://inxware.com/sbom/ert-components/$(_SBOM_PLAT)-$(_SBOM_TS_SPDX)"; \
	  echo ""; \
	  echo "##-------------------------------------------------------------------------"; \
	  echo "## Root package: ert-components"; \
	  echo "##-------------------------------------------------------------------------"; \
	  echo ""; \
	  echo "PackageName: ert-components"; \
	  echo "SPDXID: SPDXRef-ert-components"; \
	  echo "PackageVersion: $(_SBOM_VER)"; \
	  echo "PackageDownloadLocation: NOASSERTION"; \
	  echo "FilesAnalyzed: false"; \
	  echo "PackageComment: inxware eRT components — platform $(_SBOM_PLAT)"; \
	  echo "ExternalRef: OTHER docker-image $(_SBOM_IMG)"; \
	  echo ""; \
	  echo "Relationship: SPDXRef-DOCUMENT DESCRIBES SPDXRef-ert-components"; \
	  echo ""; \
	  echo "##-------------------------------------------------------------------------"; \
	  echo "## Dependency: ert-contrib-middleware"; \
	  echo "##-------------------------------------------------------------------------"; \
	  echo ""; \
	  echo "PackageName: ert-contrib-middleware"; \
	  echo "SPDXID: SPDXRef-contrib-middleware"; \
	  echo "PackageVersion: $(_SBOM_VER_CONTRIB)"; \
	  echo "PackageDownloadLocation: NOASSERTION"; \
	  echo "FilesAnalyzed: false"; \
	  echo "PackageComment: Contributed middleware libraries — $(COMPONENT_BASE_TECHNOLOGIES)"; \
	  echo "ExternalRef: OTHER local-path $(EHS_COMPONENT_SUPPORT_BUILD)"; \
	  if [ -d "$(EHS_COMPONENT_SUPPORT_BUILD)" ]; then \
	    echo "PackageChecksum: SHA1: NOASSERTION"; \
	  else \
	    echo "PackageComment: WARNING — path not found on disk"; \
	  fi; \
	  echo ""; \
	  echo "Relationship: SPDXRef-ert-components DEPENDS_ON SPDXRef-contrib-middleware"; \
	  echo ""; \
	  echo "##-------------------------------------------------------------------------"; \
	  echo "## Dependency: ert-build-support (kernel + target libs)"; \
	  echo "##-------------------------------------------------------------------------"; \
	  echo ""; \
	  echo "PackageName: ert-build-support"; \
	  echo "SPDXID: SPDXRef-build-support"; \
	  echo "PackageVersion: $(_SBOM_VER_BUILDSUP)"; \
	  echo "PackageDownloadLocation: NOASSERTION"; \
	  echo "FilesAnalyzed: false"; \
	  echo "PackageComment: Build support kernel and target libraries — $(EHS_GNU_OS_ARCH)"; \
	  echo "ExternalRef: OTHER local-path $(_SBOM_BUILDSUP_ROOT)"; \
	  if [ -d "$(_SBOM_BUILDSUP_ROOT)" ]; then \
	    echo "PackageChecksum: SHA1: NOASSERTION"; \
	  else \
	    echo "PackageComment: WARNING — path not found on disk"; \
	  fi; \
	  echo ""; \
	  echo "Relationship: SPDXRef-ert-components DEPENDS_ON SPDXRef-build-support"; \
	  if [ "$(TOOLCHAIN_PATH)" != "HOST" ] && [ -n "$(TOOLCHAIN_PATH)" ]; then \
	    echo ""; \
	    echo "##-------------------------------------------------------------------------"; \
	    echo "## Dependency: toolchain"; \
	    echo "##-------------------------------------------------------------------------"; \
	    echo ""; \
	    echo "PackageName: toolchain-$(TOOLCHAIN_PATH)"; \
	    echo "SPDXID: SPDXRef-toolchain"; \
	    echo "PackageVersion: unknown"; \
	    echo "PackageDownloadLocation: NOASSERTION"; \
	    echo "FilesAnalyzed: false"; \
	    echo "PackageComment: Cross-compilation toolchain"; \
	    echo "ExternalRef: OTHER local-path $(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)"; \
	    echo ""; \
	    echo "Relationship: SPDXRef-ert-components DEPENDS_ON SPDXRef-toolchain"; \
	  else \
	    echo ""; \
	    echo "## Toolchain: HOST ($(EHS_TOOLCHAIN_TYPE) — system compiler, no external toolchain package)"; \
	  fi; \
	} > "$(_SBOM_SPDX)"
	@echo "    -> $(_SBOM_SPDX)"

	@# -----------------------------------------------------------------
	@# 2. Forward SBOM summary — markdown
	@# -----------------------------------------------------------------
	@echo "--- Writing SBOM summary: $(_SBOM_SUMMARY)"
	@{ \
	  echo "# SBOM Summary: $(_SBOM_PLAT)"; \
	  echo ""; \
	  echo "| Field          | Value                                                              |"; \
	  echo "|:---------------|:-------------------------------------------------------------------|"; \
	  printf "| %-14s | %-66s |\n" "Generated"    "$(_SBOM_TS)"; \
	  printf "| %-14s | %-66s |\n" "Platform"     "$(_SBOM_PLAT)"; \
	  printf "| %-14s | %-66s |\n" "Version"      "$(_SBOM_VER)"; \
	  printf "| %-14s | %-66s |\n" "Docker Image" "$(_SBOM_IMG)"; \
	  printf "| %-14s | %-66s |\n" "Toolchain"    "$(EHS_TOOLCHAIN_TYPE) / $(TOOLCHAIN_PATH)"; \
	  printf "| %-14s | %-66s |\n" "Arch"         "$(EHS_GNU_OS_ARCH)"; \
	  echo ""; \
	  echo "## Dependencies"; \
	  echo ""; \
	  echo "| Package                    | Version       | Path                                                                  | On Disk |"; \
	  echo "|:---------------------------|:--------------|:----------------------------------------------------------------------|:--------|"; \
	  _contrib_ok="NO"; \
	  [ -d "$(EHS_COMPONENT_SUPPORT_BUILD)" ] && _contrib_ok="YES"; \
	  printf "| %-26s | %-13s | %-69s | %-7s |\n" \
	    "ert-contrib-middleware" "$(_SBOM_VER_CONTRIB)" \
	    "$(EHS_COMPONENT_SUPPORT_BUILD)" "$$_contrib_ok"; \
	  _bsup_ok="NO"; \
	  [ -d "$(_SBOM_BUILDSUP_ROOT)" ] && _bsup_ok="YES"; \
	  printf "| %-26s | %-13s | %-69s | %-7s |\n" \
	    "ert-build-support" "$(_SBOM_VER_BUILDSUP)" \
	    "$(_SBOM_BUILDSUP_ROOT)" "$$_bsup_ok"; \
	  if [ "$(TOOLCHAIN_PATH)" != "HOST" ] && [ -n "$(TOOLCHAIN_PATH)" ]; then \
	    _tc_path="$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)"; \
	    _tc_ok="NO"; \
	    [ -d "$$_tc_path" ] && _tc_ok="YES"; \
	    printf "| %-26s | %-13s | %-69s | %-7s |\n" \
	      "toolchain" "n/a" "$$_tc_path" "$$_tc_ok"; \
	  else \
	    printf "| %-26s | %-13s | %-69s | %-7s |\n" \
	      "toolchain (HOST)" "n/a" "$(EHS_TOOLCHAIN_TYPE) system compiler" "HOST"; \
	  fi; \
	  echo ""; \
	  echo "> **Note:** _On Disk_ reflects path presence at SBOM generation time."; \
	  echo "> SPDX detail: \`$(notdir $(_SBOM_SPDX))\` in the same directory."; \
	} > "$(_SBOM_SUMMARY)"
	@echo "    -> $(_SBOM_SUMMARY)"

	@# -----------------------------------------------------------------
	@# 3. Reverse dependents record — append to DEPENDENTS.md in each root
	@#    Skipped when SBOM_SKIP_DEPENDENTS=1 (CI / routine regression use)
	@# -----------------------------------------------------------------
	@if [ -n "$(SBOM_SKIP_DEPENDENTS)" ]; then \
	    echo "--- DEPENDENTS.md update skipped (SBOM_SKIP_DEPENDENTS set)"; \
	else \
	    echo "--- Updating DEPENDENTS.md in dependency roots"; \
	    if [ -z "$(_SBOM_ROOTS)" ]; then \
	        echo "    WARNING: No dependency roots found on disk — skipping DEPENDENTS.md update."; \
	        echo "             Ensure ert-build-support and ert-contrib-middleware are checked out."; \
	    else \
	        for _root in $(_SBOM_ROOTS); do \
	            _f="$$_root/DEPENDENTS.md"; \
	            if [ ! -f "$$_f" ]; then \
	                printf "$(_SBOM_DEP_HDR)" \
	                    "Platform" "Date/Time (UTC)" "Version" "Docker Image" > "$$_f"; \
	                printf "$(_SBOM_DEP_SEP)" \
	                    "--------------------------------------------------" \
	                    "----------------------" \
	                    "--------------------" \
	                    "--------------------------------------------------" >> "$$_f"; \
	            fi; \
	            printf "$(_SBOM_DEP_ROW)" \
	                "$(_SBOM_PLAT)" "$(_SBOM_TS)" "$(_SBOM_VER)" "$(_SBOM_IMG)" >> "$$_f"; \
	            echo "    -> $$_f"; \
	        done; \
	    fi; \
	fi

	@echo ""
	@echo "Done."
	@echo "  Forward SBOM : $(_SBOM_OUT_DIR)/"
	@if [ -n "$(SBOM_SKIP_DEPENDENTS)" ]; then \
	    echo "  Dep roots    : DEPENDENTS.md not written (SBOM_SKIP_DEPENDENTS set)"; \
	else \
	    echo "  Dep roots    : $(words $(_SBOM_ROOTS)) DEPENDENTS.md file(s) updated"; \
	fi
	@echo ""
	@echo "Tip: Run './Releases/update_release_report.sh' to generate SBOMs for all"
	@echo "     published platforms, rebuild DEPENDENTS.md from scratch, and produce"
	@echo "     the feature compliance matrix under Releases/SBOM/$(_SBOM_VER)/FEATURES/."
	@echo "     Or run 'make sbom' per platform to append to existing DEPENDENTS.md files."
	@echo "     SBOM.spdx can be consumed by SPDX-aware supply-chain tools."
	@echo "     DEPENDENTS.md files use '|' as field delimiter — importable as CSV."
	@echo ""
endif
