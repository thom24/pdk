#!/bin/bash
#
# Copyright (c) 2018-2020, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Usage : sysfw_migrate.sh <release tag> [OPTIONS]

################################################################################
export RM=rm
export MV=mv
export MKDIR=mkdir
export MAKE=gcc
export ECHO=echo
export CHMOD=chmod
export COPY=cp
export CAT=cat

################################################################################
# Parse CLI arguments
for i in "$@"; do
case $i in
    -sk|--skip-checkout) # Skips the SYSFW checkout step
	SKIP_CHECKOUT=YES
	shift
	;;
    -sb|--skip-build) # Skips the sciclient_ccs_init build step
	SKIP_BUILD=YES
	shift
	;;
    -sc|--skip-commit) # Skips the PDK commit step
	SKIP_COMMIT=YES
	shift
	;;
    -*) # Invalid flag
	$ECHO "!!!WARNING!!! - IGNORING INVALID FLAG: $1"
	shift
	;;
    *)  # Any positional arg will overwrite previous positional arg
	if [ "$RELEASE_TAG" != "" ]; then
	    $ECHO "!!!WARNING!!! - OVERWRITING \$RELEASE_TAG: $RELEASE_TAG->$1"
	fi
	RELEASE_TAG="$1"
	shift
	;;
esac
done

# Check CLI positional args (must always include a SYSFW release tag)
if [ "$RELEASE_TAG" == "" ]; then
    $ECHO "Usage : sysfw_migrate.sh <release tag> [OPTIONS]"
    exit 1
fi

################################################################################
# Specify paths relative to script
export SCRIPT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}")" && pwd )
export SCI_CLIENT_DIR=$(cd "$SCRIPT_DIR/.." && pwd )
export ROOTDIR=$(cd "$SCI_CLIENT_DIR/../../.." && pwd )

################################################################################
# Checkout SYSFW release and prepare it for use with PDK
if [ "$SKIP_CHECKOUT" != "YES" ]; then
    $ECHO "Reset PDK branch and rebase onto master"
    git reset --hard HEAD
    git fetch origin; git rebase origin/master

    $ECHO "Cloning the system-firmware-releases"
    cd $SCI_CLIENT_DIR/soc/
    git clone ssh://git@bitbucket.itg.ti.com/sysfw/system-firmware-releases.git --branch $RELEASE_TAG
    if [ $? -ne 0 ]; then
	$ECHO
	$ECHO "!!!ABORT!!! - SYSFW TAG NOT FOUND: $RELEASE_TAG"
	exit 1
    fi

    $ECHO "Remove old SYSFW dir, and replace it with the newly cloned repo"
    $RM -fr sysfw
    $MV system-firmware-releases sysfw
    cd sysfw

    $ECHO "Removing files not required from SYSFW release.."
    $RM -fr .git
    $RM -fr src
    $RM -fr log
    $RM -fr binaries/system-firmware-design-documentation
    $RM -fr binaries/system-firmware-full-documentation
    $RM binaries/*.elf
    $RM binaries/*.png
    $RM binaries/*.svg
    $RM binaries/*.cmm
    $RM binaries/t32-qt-lsf
    $RM binaries/*hs.bin
    $RM -fr binaries/am6
    $RM -fr binaries/am6_sr2
    $RM -fr binaries/j721e
    $RM -fr binaries/memory
    $RM -fr scripts
    $RM .gitignore
    $RM .gitmodules
    $RM Makefile
    $RM README.md
    $RM binaries/SYSFW_Software_FMEA_Form.xls

    $ECHO "Modifying SR2 headers such that they don't cause any collisions with the SR1 headers"

    # Include guards need to be modified to remove collision with SR1
    sed -i 's/SOC_AM6_CLOCKS_H/&_SR2/' include/am65x_sr2/g2tisci_clocks.h
    sed -i 's/SOC_TISCI_DEVICES_H/&_SR2/' include/am65x_sr2/tisci_devices.h
    sed -i 's/TISCI_BOARDCFG_CONSTRAINTS_H/&_SR2/' include/am65x_sr2/tisci_boardcfg_constraints.h
    sed -i 's/TISCI_HOSTS_H/&_SR2/' include/am65x_sr2/tisci_hosts.h
    sed -i 's/AM6_TISCI_SEC_PROXY_H/&_SR2/' include/am65x_sr2/tisci_sec_proxy.h
    sed -i 's/TISCI_RESASG_TYPES_H/&_SR2/' include/am65x_sr2/tisci_resasg_types.h

    # RM cfg count needs to be modified to remove collision with SR1
    sed -i 's/TISCI_RESASG_UTYPE_CNT/&_SR2/' include/am65x_sr2/tisci_resasg_types.h

    # TIDL team requires pointer notation instead of array notation (when using VS)
    vs_str='\/\* Windows Visual Studio build has issues with  payload\[\], changing it only for visual studio build \*\/\n'
    vs_str+='#ifdef _MSC_VER\n'
    vs_str+='    uint32_t    *payload;\n'
    vs_str+='#else\n'
    vs_str+='    uint8_t    payload[];\n'
    vs_str+='#endif'

    sed -i "/payload\[\]/c\\$vs_str" include/tisci/tisci_protocol.h
fi

################################################################################
# Build sciclient_ccs_init for use with launch.js
if [ "$SKIP_BUILD" != "YES" ]; then
    cd $ROOTDIR/ti/build

    # J721e
    make -j -s sciclient_ccs_init_clean BOARD=j721e_evm
    make -j -s sciclient_ccs_init BOARD=j721e_evm
    $COPY $ROOTDIR/ti/binary/sciclient_ccs_init/bin/j721e/sciclient_ccs_init_mcu1_0_release.xer5f $SCI_CLIENT_DIR/tools/ccsLoadDmsc/j721e/

    # AM65xx
    make -j -s sciclient_ccs_init_clean BOARD=am65xx_evm
    make -j -s sciclient_ccs_init BOARD=am65xx_evm
    $COPY $ROOTDIR/ti/binary/sciclient_ccs_init/bin/am65xx/sciclient_ccs_init_mcu1_0_release.xer5f $SCI_CLIENT_DIR/tools/ccsLoadDmsc/am65xx/
    
    cd -
fi

################################################################################
# Commit changes to PDK
if [ "$SKIP_COMMIT" != "YES" ]; then
    $ECHO "Commit changes to PDK"
    cd $SCRIPT_DIR
    git add $SCI_CLIENT_DIR/soc/sysfw
    git commit -m "Migrating to SYSFW version $RELEASE_TAG"
fi

################################################################################
$ECHO "Done."
