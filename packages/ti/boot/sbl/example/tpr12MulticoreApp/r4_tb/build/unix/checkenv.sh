#!/bin/bash
#
# ******************************************************************
# * FILE PURPOSE: Check environment setup
# ******************************************************************
# * FILE NAME: checkenv.sh
# *
# * DESCRIPTION:
# *  Checks the build environment and sets PATH for RBL build.
# ******************************************************************

retval=0

# -------------------------------------------------------------------
# Sanity Check: Ensure that all the required variables are defined
# -------------------------------------------------------------------
if [ -z $R4_TB_PATH ]; then
    echo "Error: R4_TB_PATH [TB Source code path] needs to be defined"
    echo "For example:"
    echo "  export R4_TB_PATH=~/work/r4_tb"
    retval=1
fi
if [ -z $R4_CODEGEN_INSTALL_PATH ]; then
    echo "Error: R4_CODEGEN_INSTALL_PATH [R4 Toochain] needs to be defined"
    echo   "For example:"
    echo   "  export R4_CODEGEN_INSTALL_PATH=/opt/ti/ti-cgt-arm_18.12.2.LTS"
    retval=1
fi

if [ $retval -eq 1 ]; then
    echo  "Error: One of more build variables are not set correctly. Exiting...."
    return 1
fi

echo  "-----------------------------------------------"
echo  "R4 TB Build Environment Configured"
echo  "-----------------------------------------------"
return 0

