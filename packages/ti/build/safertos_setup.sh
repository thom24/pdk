#!/bin/bash
#
# Utility script to setup SDK for SafeRTOS Builds
#
# Prerequisites:-
# - Make sure you have installed the SafeRTOS Packages from WHIS
#
# Note:-
# - Make sure the paths provided DO NOT have any spaces in them

# Example usage
# ./safertos-setup.sh --j721e_r5f_path=path/to/whis/safertos/j721e/r5f/pacakge --j721e_c66_path=path/to/whis/safertos/j721e/c66/pacakge --j721e_c7x_path=path/to/whis/safertos/j721e/c7x/pacakge
# ./safertos-setup.sh --j721e_r5f_path=path/to/whis/safertos/j721e/r5f/pacakge 
# ./safertos-setup.sh --j721e_c66_path=path/to/whis/safertos/j721e/c66/pacakge 
# ./safertos-setup.sh --j721e_c7x_path=path/to/whis/safertos/j721e/c7x/pacakge


#Environmental variables
pdk_build_path=`pwd`

#Get user input
while [ $# -gt 0 ]; do
  case "$1" in
    --j721e_r5f_path=*)
      j721e_r5f_path="${1#*=}"
      ;;
    --j721e_c66_path=*)
      j721e_c66_path="${1#*=}"
      ;;
    --j721e_c7x_path=*)
      j721e_c7x_path="${1#*=}"
      ;;    
    -h|--help)
      echo Usage: $0 [options]
      echo
      echo Options:
      echo "--j721e_r5f_path       Path to J721E(TDA4VM) R5F SafeRTOS Package from WHIS"
      echo "--j721e_c66_path       Path to J721E(TDA4VM) C66 SafeRTOS Package from WHIS"
      echo "--j721e_c7x_path       Path to J721E(TDA4VM) C7x SafeRTOS Package from WHIS"
      exit 0
      ;;
    *)
      printf "Error: Invalid argument $1!!\n"
  esac
  shift
done

printf "\n pdk_build_path = $pdk_build_path \n"

if [ ! "${j721e_r5f_path}" == "" ]; then
  printf "\n j721e_r5f_path = $j721e_r5f_path \n\n"
  if [ !  -d ${j721e_r5f_path} ] ; then
    printf "\n Error: Invalid j721e_r5f_path!!\n\n"
  else
    #Update R5F Install Path
    sed -i -e "s|SAFERTOS_j721e_r5f_INSTALL_PATH =.*|SAFERTOS_j721e_r5f_INSTALL_PATH = ${j721e_r5f_path}|g" safertos_package_path.mk
    #Build safertos lib for all R5F cores
    make safertos -sj8 CORE=mcu1_0 BUILD_PROFILE=release BOARD=j721e_evm
    make safertos -sj8 CORE=mcu1_1 BUILD_PROFILE=release BOARD=j721e_evm
    make safertos -sj8 CORE=mcu2_0 BUILD_PROFILE=release BOARD=j721e_evm
    make safertos -sj8 CORE=mcu2_1 BUILD_PROFILE=release BOARD=j721e_evm
    make safertos -sj8 CORE=mcu3_0 BUILD_PROFILE=release BOARD=j721e_evm
    make safertos -sj8 CORE=mcu3_1 BUILD_PROFILE=release BOARD=j721e_evm
    #Build osal_safertos for R5F
    make osal_safertos -sj8 CORE=mcu1_0 BUILD_PROFILE=release BOARD=j721e_evm
    #Build sciserver_testapp_safertos and check-in
    make sciserver_testapp_safertos -sj8 CORE=mcu1_0 BUILD_PROFILE=release BOARD=j721e_evm
    cp ${pdk_build_path}/../binary/sciserver_testapp_safertos/bin/j721e/sciserver_testapp_safertos_mcu1_0_release.xer5f ${pdk_build_path}/../drv/sciclient/tools/ccsLoadDmsc/j721e/
    cp ${pdk_build_path}/../binary/sciserver_testapp_safertos/bin/j721e/sciserver_testapp_safertos_mcu1_0_release.rprc  ${pdk_build_path}/../drv/sciclient/tools/ccsLoadDmsc/j721e/
    printf "\n Successfully completed setup SafeRTOS Builds for J721E R5F\n\n"
  fi
fi
if [ ! "${j721e_c66_path}" == "" ]; then
  printf "\n j721e_c66_path = $j721e_c66_path \n\n"
  if [ !  -d ${j721e_c66_path} ] ; then
    printf "\n Error: Invalid j721e_c66_path!!\n\n"
  else
    #Update C66 Install Path
    sed -i -e "s|SAFERTOS_j721e_c66_INSTALL_PATH =.*|SAFERTOS_j721e_c66_INSTALL_PATH = ${j721e_c66_path}|g" safertos_package_path.mk
    #Build safertos lib for all c66 cores
    make safertos -sj8 CORE=c66xdsp_1 BUILD_PROFILE=release;
    make safertos -sj8 CORE=c66xdsp_2 BUILD_PROFILE=release;
    #Build osal_safertos for c66
    make osal_safertos -sj8 CORE=c66xdsp_1 BUILD_PROFILE=release;
    printf "\n Successfully completed setup SafeRTOS Builds for J721E C66\n\n"
  fi
fi
if [ ! "${j721e_c7x_path}" == "" ]; then
  printf "\n j721e_c7x_path = $j721e_c7x_path \n\n"
  if [ !  -d ${j721e_c7x_path} ] ; then
    printf "\n Error: Invalid j721e_c7x_path!!\n\n"
  else
    #Update C7x install Path
    sed -i -e "s|SAFERTOS_j721e_c7x_INSTALL_PATH =.*|SAFERTOS_j721e_c7x_INSTALL_PATH = ${j721e_c7x_path}|g" safertos_package_path.mk
    #Build safertos lib for all c7x cores
    make safertos -sj8 CORE=c7x_1 BUILD_PROFILE=release;
    #Build osal_safertos for c7x
    make osal_safertos -sj8 CORE=c7x_1 BUILD_PROFILE=release;
    printf "\n Successfully completed setup SafeRTOS Builds for J721E C7x\n\n"
  fi
fi

exit
