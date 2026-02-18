#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2008-2025, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
# <https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------

set -e
export SPECIFIC_TARGET=$1

source ./scripts/build-function-library/colour.sh

TXT_FG=${TXT_FG_BLUE}

echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
echo
echo "${TXT_FG_BRIGHT_WHITE}Building target environment staging tree"
echo
echo "Build parameters: "
echo
echo "${TXT_FG_GREEN}--> GNU OS     :    ${TXT_FG_WHITE}'${EHS_GNU_OS}'"
echo "${TXT_FG_GREEN}--> GNU ARCH   :    ${TXT_FG_WHITE}'${EHS_GNU_ARCH}'"
echo "${TXT_FG_GREEN}--> COMMON OS  :    ${TXT_FG_WHITE}'${EHS_OS}'"
echo "${TXT_FG_GREEN}--> COMMON ARCH:    ${TXT_FG_WHITE}'${EHS_ARCH}'"
echo
echo "${TXT_FG_GREEN}--> SYSTEM_VARIANT: ${TXT_FG_WHITE}'${SYSTEM_VARIANT}'"
echo "${TXT_FG_GREEN}--> PRODUCT_NAME:   ${TXT_FG_WHITE}'${PRODUCT_NAME}'"
echo

EHS_SKIP_BIN_COPY=""
if [ "$2" = "--skip-bin-cpy" ]; then
    EHS_SKIP_BIN_COPY="yes"
fi

########################################################
## Stop disturbances from cron to the environment if we are mapped to /root/ehs
#which crontab &> /dev/null && crontab -r

# export UPLOAD variable to let bash use it for doing uploads (e.g. to debian servers).
export UPLOAD

########################################################
#create File system, including target specific parts and version information
#echo  "running ./target/envbuildscripts/targetenv_directories.sh" $SPECIFIC_TARGET

echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
echo
echo "Copying eRT hosted assets to staging directory..."
./target/envbuildscripts/targetenv_directories.sh $SPECIFIC_TARGET

### EHS ################################################
if [ "${EHS_SKIP_BIN_COPY}" = "" ]; then
    if test -e ehs_${SPECIFIC_TARGET}.${EXE}; then
        echo "Copying EHS to target staging directory..."
        cp -v "ehs_${SPECIFIC_TARGET}.${EXE}" "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/ehs.${EXE}"
        echo
    else
        err "ehs_${SPECIFIC_TARGET}.${EXE} not found!"
        echo "Please build using ${TXT_FG_WHITE}make all${TXT_FG} first or try ${TXT_FG_WHITE}make help${TXT_FG} for more details."
        echo "Exiting..."
        exit 1
    fi
fi

EHS_TOOLCHAIN_PATH_FROM_BASE="ert-build-support/toolchains/${TOOLCHAIN_PATH}"
EHS_TOOLCHAIN_LOCAL_PATH="${LOCAL_BASE}/${EHS_TOOLCHAIN_PATH_FROM_BASE}"
EHS_TOOLCHAIN_SERVER_PATH="${REPOSITORY}/${EHS_TOOLCHAIN_PATH_FROM_BASE}"

echo "Adding Supporting Target Binaries to EHS File System..."
#Add user space support for EHS and components:
mkdir -p "../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib"

# toolchain sysroot shared libs
# note - may not exist for target
#todo this looks a bit of a mess?

if [ "${TOOLCHAIN_PATH}" = "HOST" ] || [ "${TOOLCHAIN_NAME}" = "HOST" ]; then
    echo "Using host system/Docker toolchain — no toolchain libraries to copy."
else
    TC_LIBS="../ert-build-support/toolchains/${TOOLCHAIN_PATH}/sysroot/lib"
    if [ -e "${TC_LIBS}" ]; then
        echo "Copying toolchain libraries..."
        cp -vPR ${TC_LIBS}/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib/
    else
        warn "No toolchain libraries found in '${TC_LIBS}'"
    fi
fi

#Tidy up any accidently copied static libraries of toolchain objects
rm -f "../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib/*.a"
rm -Rf "../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/corelib/gcc"
# additional clib libraries if built separately
if [ ! -n "${EHS_CLIB_OVERRIDE_PATH}" ]; then
    EXTR_PKGS="../ert-build-support/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/target_packages"
    if [ -e "${EXTR_PKGS}" ]; then
        echo "Copying additional core libraries..."
        cp -vPR "${EXTR_PKGS}/"* "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin"
    else
        warn "No additional core libraries found in '${EXTR_PKGS}'"
    fi
else
    warn "Using CLIB override path = '${EHS_CLIB_OVERRIDE_PATH}'"
    CLIB_OVR="../ert-build-support/support_libs/target_libs/${EHS_CLIB_OVERRIDE_PATH}/target_packages"
    if [ -e "../ert-build-support/support_libs/target_libsx/${EHS_CLIB_OVERRIDE_PATH}/target_packages" ]; then
        echo "Copying CLIB override files..."
        cp -vPR "${CLIB_OVR}/"* "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin"
    else
        warn "No additional support libraries found in '${CLIB_OVR}'"
    fi
fi

#and finally the component libraries, we more often do expect to find
CMPT_LIBS="../ert-contrib-middleware/target_libs/${COMPONENT_BASE_TECHNOLOGIES}/target_packages"
if [ -d "${CMPT_LIBS}" ]; then
    echo "Copying component libraries..."
    echo "  from '${CMPT_LIBS}'"
    echo "  to '../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin'"
    cp -PR --remove-destination "${CMPT_LIBS}/"* "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/" || exit 1
else
    warn "No component libraries found in '${CMPT_LIBS}'"
fi

# Create a versionn information file in the taretenv tree (doesn't increment)
echo
echo "Setting the internal version information file for the build"
./target/envbuildscripts/targetenv_create_version_info.sh $SPECIFIC_TARGET

if [ "${EHS_DEFAULT_APP}" = "NONE" ]; then
  echo
  echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
  echo
  echo "Installing the default app"

  if [ -z "${EHS_SKIP_REPO_PULL}" ]; then
    echo "Checking out the latest PRODUCTION branch of your app repo..."
    if [ -d ../apps ]; then
        pushd ../apps || exit 1

        # Ensure we are on the right branch
        #if ! git rev-parse --abbrev-ref HEAD | grep -qx "RELEASE-PRODUCTION"; then
        #    err "../apps is on branch '$(git rev-parse --abbrev-ref HEAD)'," >&2
        #    err "but the build expects RELEASE-PRODUCTION. Please switch branches or set EHS_SKIP_REPO_PULL=1." >&2
        #    exit 1
        #fi

        # Refuse to pull if it would require a merge
        #git fetch origin RELEASE-PRODUCTION || exit 1
        #if ! git merge-base --is-ancestor HEAD origin/RELEASE-PRODUCTION; then
        #    err "local RELEASE-PRODUCTION has diverged from origin/RELEASE-PRODUCTION." >&2
        #    echo "Please run 'git reset --hard origin/RELEASE-PRODUCTION' or push your changes," >&2
        #    echo "or set EHS_SKIP_REPO_PULL=1 to use your local state." >&2
        #    exit 1
        #fi
        git checkout RELEASE-PRODUCTION || exit 1
        #git pull --ff-only origin RELEASE-PRODUCTION || exit 1

        popd || exit 1
    else
        pushd ..
        if [ -f ${LOCAL_BASE}/COMMUNITY_RELEASE ]; then
            # TODO: should the yaml action do this with credentials?
            git clone --depth 1 git@github.com:inxware/apps.git
        else
            git clone git@github.com:inxware/apps.git
        fi
        cd apps/
        git checkout RELEASE-PRODUCTION || exit 1
        popd
    fi
  else
    warn "Not checking out the latest 'PRODUCTION' branch of your app repo - 'EHS_SKIP_REPO_PULL' is set"
  fi
  echo
  echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
  echo
else 
  echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
  echo "${TXT_FG_YELLOW} NOT Installing a default app"
  echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
fi #if [ "${EHS_DEFAULT_APP}" = "NONE" ];

EHS_APP_EXPORT_DIR=export-ert${ERT_SODL_VERSION}

if [ "${EHS_DEFAULT_APP}" = "" ]; then
    APP_SRC="../apps/systemapps/Home/${EHS_APP_EXPORT_DIR}"
    echo "Installing the desktop HOME app (${APP_SRC})"
    if [ ! -d "${APP_SRC}" ]; then
        echo
        err "================================================================"
        err "  Application not found: ${APP_SRC}"
        err ""
        err "  The ../apps/ repository may not be checked out, or the app"
        err "  has not been exported for eRT${ERT_SODL_VERSION}."
        err ""
        err "  Try:  git clone <apps-repo-url> ../apps"
        err "  Or:   make prepdeps"
        err "================================================================"
        exit 1
    fi
    cp -Rf ${APP_SRC}/* ../TARGET_TREES/ehs_env-$TARGET/appdata/default/ || exit 1
else
    if [ "${EHS_DEFAULT_APP}" = "NONE" ]; then
        warn "Not installing a default app (EHS_DEFAULT_APP = "NONE")!"
    else
        APP_SRC="../apps/${EHS_DEFAULT_APP}/${EHS_APP_EXPORT_DIR}"
        echo "Installing platform configured [${APP_SRC}]"
        if [ ! -d "${APP_SRC}" ]; then
            echo
            err "================================================================"
            err "  Application not found: ${APP_SRC}"
            err ""
            err "  EHS_DEFAULT_APP is set to '${EHS_DEFAULT_APP}'"
            err "  but the export directory does not exist."
            err ""
            err "  Check that:"
            err "    1. ../apps/ repository is checked out"
            err "    2. The app '${EHS_DEFAULT_APP}' exists"
            err "    3. It has been exported to ${EHS_APP_EXPORT_DIR}/"
            err "================================================================"
            exit 1
        fi
        cp -Rf ${APP_SRC}/* ../TARGET_TREES/ehs_env-$TARGET/appdata/default/ || exit 1
        echo "Default app copied OK"
    fi
fi

echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
echo
echo "Configuring the IoT server service"
echo
if [ -f ./COMMUNITY_RELEASE ]; then
    echo "No Certificate repo used"
else
    if [ "${EHS_SKIP_REPO_PULL}" = "" ]; then
        if [ -d ../DevmanSecurity/ ]; then
            pushd ../DevmanSecurity/
            warn "About to git pull in '$(pwd)'"
            git pull origin master
            popd
        else
            pushd ..
            echo "WARNING! You need to create a security folder adjascent to ert-components."
            echo "or check one out using something like"
            echo "git clone git@github.com:/<Your Secure Repo>.git"
            #cd DevmanSecurity/ ||
            #git checkout master ||
            popd
        fi
    else
        warn "Not checking out the latest 'PRODUCTION' branch of Devman Security repo - 'EHS_SKIP_REPO_PULL' is set "
    fi
fi
echo
echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
echo

# This function populated the Devman Server information if the patform is using Devman
# We probably want to put this in it's own utility script rather than having in the general targetenv script in the future.

function SetDevmanServer() {
    local DEVMAN_SERVER_DOMAIN=$1
    local DEVMAN_SERVER_DOMAIN_DEFAULT=$2

    if [ "${DEVMAN_SERVER_DOMAIN}" != "" ]; then
        # devman cert destination directory
        DEVMAN_CERT_DIR=certs
        TGT_CERT_DIR="../TARGET_TREES/ehs_env-$TARGET/devman/core/$DEVMAN_CERT_DIR"

        if [ "$DEVMAN_SERVER_DOMAIN_DEFAULT" != "default" ]; then
            DEVMAN_CERT_DIR="certs/$DEVMAN_SERVER_DOMAIN"
        fi

        if [ "${DEVMAN_SERVER_PROTOCOL}" = "mqtts" ]; then
            echo "Looking for certificates in  ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker"
        else
            echo "Looking for certificates in  ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs"
        fi

        #todo we should be able to remove the following if targetenv_directories has done it's job
        mkdir -p "${TGT_CERT_DIR}"
        echo "Target certificate directory ${TGT_CERT_DIR}/"

        # We will first always copy the single CA cert for Devman to the staging directory because this may be used by the downloader instead of a full bundle
        CA_CERT="../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca.crt"
        if [ "${DEVMAN_SERVER_PROTOCOL}" = "https" ] || [ "${DEVMAN_SERVER_PROTOCOL}" = "http" ]; then
            # devman-only-ca.crt - is only use by the android supervisor, and only needed when DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE is set and overwrites devman-ca.crt (see below)
            if [ "${BUILD_WITH_ANDROID_SUPERVISOR}" != "" -a "${BUILD_WITH_ANDROID_SUPERVISOR}" != "no" ]; then
                echo "Copying single Devman pem format client key to Devman directory"
                if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca.crt ]; then
                    cp -vf ${CA_CERT} ${TGT_CERT_DIR}/devman-only-ca.crt
                else
                    warn "You have specified DEVMAN_SERVER_PROTOCOL = https, but a CA cert is not found in '${CA_CERT}'"
                fi
            fi
        elif [ "${DEVMAN_SERVER_PROTOCOL}" = "mqtts" ]; then
            echo "Copying Devman mqtt broker CA to Devman directory"
            CA_CERT_FILE=../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/ca.crt
            if [ -f "${CA_CERT_FILE}" ]; then
                cp -f ${CA_CERT_FILE} ${TGT_CERT_DIR}/ca.crt
            else
                warn "You have specified DEVMAN_SERVER_PROTOCOL = mqtts, but a CA cert is not found at '${CA_CERT_FILE}'"
            fi
        else
            warn "You have configured a non-secure http protocol for Devman"
        fi

        # We now decide wht the default CA certs should be for the eRT runtime as sometimes we don't have a full bundle).
        # e.g. 32bit version of linux curl needs a full CA bundle it seems
        if [ "${DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE}" != "" -a "${DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE}" != "none"  ]; then
            echo "Copying pem format client key to Devman directory"
            BUNDLE="../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-ca-bundle.crt"
            if [ -f ${BUNDLE} ]; then
                cp -f ${BUNDLE} ${TGT_CERT_DIR}/devman-ca.crt
                # todo2023 This fCkd shizzle for some hardwired thing I can't find in the build that makes unity EHS always look for it, what ever the source code says.
                # I'm pretty sure we can get rid of this now
                cp -f ${BUNDLE} ${TGT_CERT_DIR}/cacert.pem
            else
                err "You have specified DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE, but a bundle does not exist"
                echo "Looking in '${BUNDLE}"
                exit 1
            fi
        else
            if [ "${DEVMAN_SERVER_PROTOCOL}" = "https" ] || [ "${DEVMAN_SERVER_PROTOCOL}" = "http" ]; then
                echo "Copying pem format client key to Devman directory"
                if [ -f ${CA_CERT} ]; then
                    cp -f ${CA_CERT} ${TGT_CERT_DIR}/devman-ca.crt
                else
                    warn "You have specified DEVMAN_SERVER_PROTOCOL = http(s), but but a CA cert is not found at '${CA_CERT}'"
                fi
            elif [ "${DEVMAN_SERVER_PROTOCOL}" = "mqtts" ]; then
                echo "Copying Devman mqtt broker clinet cert and key to Devman directory"
                CLIENT_CERT_FILE=../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/client.crt
                if [ -f "${CLIENT_CERT_FILE}" ]; then
                    cp -f ${CLIENT_CERT_FILE} ${TGT_CERT_DIR}/client.crt
                else
                    warn "You have specified DEVMAN_SERVER_PROTOCOL = mqtts, but a CLIENT cert is not found at '${CLIENT_CERT_FILE}'"
                fi
                CLIENT_KEY_FILE=../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/client.key
                if [ -f "${CLIENT_KEY_FILE}" ]; then
                    cp -f ${CLIENT_KEY_FILE} ${TGT_CERT_DIR}/client.key
                else
                    warn "You have specified DEVMAN_SERVER_PROTOCOL = mqtts, but a CLIENT key is not found at '${CLIENT_KEY_FILE}'"
                fi
            else
                warn "You have configured a non-secure http protocol for Devman"
            fi
        fi

        if [ "${DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED}" != "" -a "${DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED}" != "no"  ]; then
            if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-client-crt-key.pem ]; then
                echo "Copying pem format client key to Devman config directory..."
                cp  -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-client-crt-key.pem ${TGT_CERT_DIR}/
            else
                if [ "${DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED}" != "" -a "${DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED}" != "no"  ]; then
                    err "No client certificte in PEM format is available for ${DEVMAN_SERVER_DOMAIN} and you have set DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED"
                    exit 1
                else
                    warn "No client certificte in PEM format is available for ${DEVMAN_SERVER_DOMAIN}"
                fi
            fi
        fi

        if [ "${BUILD_WITH_ANDROID_SUPERVISOR}" != "" -a "${BUILD_WITH_ANDROID_SUPERVISOR}" != "no" ]; then
            if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-client-crt-key.p12 ]; then
                echo "Copying pem format client key to Devman config directory..."
                cp  -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/certs/client/devman-client-crt-key.p12 ${TGT_CERT_DIR}/
            else
                warn "No client certificte in P12 format client cert is available for ${DEVMAN_SERVER_DOMAIN} (Required only for the Android supervisor)"
            fi
        fi

        if [ "${DEVMAN_SERVER_DOMAIN_DEFAULT}" = "default" ]; then
            # only needs to be set for the default domain
            echo "Updating the IoT Server URL..."
            mkdir -p ../TARGET_TREES/ehs_env-$TARGET/devman/core/config
            if [ "${DEVMAN_SERVER_PROTOCOL}" = "" ]; then
                warn "'DEVMAN_SERVER_PROTOCOL' is not set. Defaulting to https"
                DEVMAN_SERVER_PROTOCOL="https"
            fi

            #configure the devman URLs:
            if [ "${DEVMAN_SERVER_PROTOCOL}" = "mqtts" ]; then
                echo "Setting MQTT (${DEVMAN_SERVER_DOMAIN}) devman URLs ====> DEVMANURL.000"
                echo "${DEVMAN_SERVER_DOMAIN}" > ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/DEVMANURL.000
                # check if mqtt devman clinet user name and password files are present
                if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/uname ]; then
                    cp -v ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/uname ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/uname.000
                    echo "Devman MQTT clinet username file copied"
                fi

                if [ -f ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/passw ]; then
                    cp -v ../DevmanSecurity/${DEVMAN_SERVER_DOMAIN}/mqtt_broker/passw ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/passw.000
                    echo "Devman MQTT clinet password file copied"
                fi
            else
                echo "Setting (${DEVMAN_SERVER_PROTOCOL}://${DEVMAN_SERVER_DOMAIN}) devman URLs ====> DEVMANURL.000"
                test -d ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/ || mkdir -p ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/
                echo "${DEVMAN_SERVER_PROTOCOL}://${DEVMAN_SERVER_DOMAIN}" > ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/DEVMANURL.000
            fi
        fi
    else
        warn "'DEVMAN_SERVER_DOMAIN' is not set. Not configuring an IoT support server in targetenv"
    fi
} # SetDevmanServer()

# check the default certificate domain
if [ "$DEVMAN_SERVER_DOMAIN" != "" ]; then
    echo "Setting 'DEVMAN_SERVER_DOMAIN' (default)"
    SetDevmanServer ${DEVMAN_SERVER_DOMAIN} "default"
    # check the second certificate domain
    if [ "$DEVMAN_SERVER_DOMAIN_1" != "" ]; then
        echo "Setting 'DEVMAN_SERVER_DOMAIN_1'"
        SetDevmanServer ${DEVMAN_SERVER_DOMAIN_1} ""
    else
        echo "Skip 'DEVMAN_SERVER_DOMAIN_1'..."
    fi

    # check the third certificate domain
    if [ "$DEVMAN_SERVER_DOMAIN_2" != "" ]; then
        echo "Setting 'DEVMAN_SERVER_DOMAIN_2'"
        SetDevmanServer ${DEVMAN_SERVER_DOMAIN_2} ""
    else
        echo "Skip 'DEVMAN_SERVER_DOMAIN_2'..."
    fi
else
    warn "'DEVMAN_SERVER_DOMAIN' is not set. Not configuring an IoT support server in targetenv"
fi

echo
echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
echo

echo "Looking for platform hacks script..."
# Some devices have varying OSs features that need to be modified:
# Targetenv hacks are selected specifically with $INXWARE_TARGETENV_HACKS as there are other unrelated
# factors defined by $SPECIFIC_TARGET used elsewhere too which may are may not be useful here.
# TODO we should probably support a list of hack files in the INXWARE_TARGETENV_HACKS variable
if  [ -e "./target/envbuildscripts/platform-hacks/targetenv_hacks_${INXWARE_TARGETENV_HACKS}.sh" ]; then
    echo "Applying Hack: /target/envbuildscripts/platform-hacks/targetenv_hacks_${INXWARE_TARGETENV_HACKS}.sh $INXWARE_TARGETENV_HACKS"
    ./target/envbuildscripts/platform-hacks/targetenv_hacks_${INXWARE_TARGETENV_HACKS}.sh $SPECIFIC_TARGET
else
    if [ "$INXWARE_TARGETENV_HACKS" != "" ]; then
        err "No Hack file found for /target/envbuildscripts/platform-hacks/targetenv_hacks_${INXWARE_TARGETENV_HACKS}.sh"
        exit 1
    fi
fi

if [ -d ../apps ]; then
    pushd ../apps

    if [ -f ../ert-components/COMMUNITY_RELEASE ]; then
        git checkout main || :
    else
        git checkout master || :
    fi
fi

echo
echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
echo
echo "Completed make targetenv - staging directory ready at '../TARGET_TREES/ehs_env-$SPECIFIC_TARGET'"
echo
heading "Finished building target environment staging tree"
echo
echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
echo
exit 0
