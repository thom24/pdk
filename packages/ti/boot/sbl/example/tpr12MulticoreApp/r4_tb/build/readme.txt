###############################################################################
# Build Instructions
###############################################################################

-------------------------------------------------------------------------------
- Tool Installation
-------------------------------------------------------------------------------

Please ensure that the following tools are installed:-

1. TI-CGT-ARM Tools Version 18.12.2 LTS

-------------------------------------------------------------------------------
- Unix Build Variables
-------------------------------------------------------------------------------

Ensure that the following build variables are configured. The values are
illustrative and will need to be updated for each individual setup:

#
# This should be set to the R5 Toolchain
#
export R4_CODEGEN_INSTALL_PATH=~/ti/ti-cgt-arm_18.12.2.LTS

#
# This is the directory where the source code is located.
#
export R4_TB_PATH=~/work/mmwave_boot_test/r4_tb

#
# Once the build environment is configured. Use the following script in the
# build/unix directory to ensure that the build setup is complete
#
source ./checkenv.sh

-------------------------------------------------------------------------------
- Windows Build Variables
-------------------------------------------------------------------------------

On Windows it is recommended that the following additional tools are installed

- XDC Tools
  Download and install any XDC Tools from TI. The XDC Tools also provide a
  Unix like build environment. Ensure that the XDCTools is in the PATH.
  XDCTools uses *gmake*. This option might be deprecated in future releases.

Ensure that the following build variables are configured. The values are
illustrative and will need to be updated for each individual setup:

#
# Use XDC for make tools
#
set XDC_INSTALL_PATH=C:/ti/xdctools_3_51_01_18_core

#
# This should be set to the R5 Toolchain
#
set R4_CODEGEN_INSTALL_PATH=E:/ti/ti-cgt-arm_18.12.2.LTS

#
# This is the directory where the source code is located.
#
set R4_TB_PATH=E:/work/mmw/r5-boot-rom/

./checkenv.bat

-------------------------------------------------------------------------------
- Building the code
-------------------------------------------------------------------------------

make all   -> This will build everything.
make clean -> This will clean everything

*Note*: If using the XDCTools on Windows then make might need to be replaced
by gmake.
