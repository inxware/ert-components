#!/bin/bash
# inx limited 2022
# Creates a Windows Installer .exe

echo "**************************************************************************************"
echo "**  Making Windows Installer EXE - _* option - not standalone **"
echo "**************************************************************************************"

export SPECIFIC_TARGET="$1"
export ERT_PACKAGE_NAME="$2"
export ERT_NSIS_EXE_NAME="$3"

export EHS_ROOT=`pwd` # assuming we're in the ehs project root
pushd "${EHS_ROOT}/.."
export REPOSITORY_ROOT=`pwd`
popd
if [ "$SPECIFIC_TARGET" = "" ]; then
    echo "TARGET is not specified."
    exit 1
fi
if [ "$ERT_PACKAGE_NAME" = "" ]; then
    echo "ERT package name is not specified."
    exit 1
fi
pushd ../TARGET_TREES || exit 1
TARGET_TREES="$(pwd)"
popd
if ! [ -d "$TARGET_TREES" ]; then
    echo "TARGET_TREES directory is not available!"
    exit 1
fi
export TARGET_PATH="${TARGET_TREES}/ehs_env-${SPECIFIC_TARGET}"

NSIS_WORKING_BASE="${TARGET_TREES}/ehs_nsis-${SPECIFIC_TARGET}"
mkdir -p "$NSIS_WORKING_BASE"

INSTALLER_EXE="${NSIS_WORKING_BASE}/inxware-ert-installer.exe"
if [ -f "${INSTALLER_EXE}" ]; then
  rm "${INSTALLER_EXE}"
fi

INSTALLER_NSI_SOURCE="${EHS_ROOT}/scripts/build-deploy/packagers/nsis/inxware-ert-installer.nsi"
INSTALLER_NSI_DEST="$NSIS_WORKING_BASE/inxware-ert-installer.nsi"
if [ ! -f "$INSTALLER_NSI_SOURCE" ]; then
    echo "Failed to find required file: $INSTALLER_NSI_SOURCE"
    exit 1
fi
cp -fv "$INSTALLER_NSI_SOURCE" "$INSTALLER_NSI_DEST"

if [ "$ERT_NSIS_EXE_NAME" = "" ]; then
  export ERT_NSIS_EXE_NAME="eRT"
fi

makensis -DERT_TARGET="${SPECIFIC_TARGET}" -DERT_PACKAGE_NAME="${ERT_PACKAGE_NAME}" -DERT_NSIS_EXE_NAME="${ERT_NSIS_EXE_NAME}" -V1 "$INSTALLER_NSI_DEST"

if [ -f "${INSTALLER_EXE}" ]; then
  echo "Successfully generated Windows installer: ${INSTALLER_EXE}"
else
  echo "Failed to generate Windows installer"
  exit 1
fi
