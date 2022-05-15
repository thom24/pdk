################################################################################
# Configure SafeRTOS Package paths for various SOC/ISA
################################################################################

# Update the following to set custom path to SafeRTOS Package.
# Else the default path will be set to $(SDK_INSTALL_PATH)/safertos_$(SOC)_$(ISA)_$(SAFERTOS_VERSION_$(ISA))
# IMPORTANT:
#   Make sure the paths specified below DO NOT have any spaces in them.
ifeq ($(BOARD),$(filter $(BOARD), j721e_evm ))
  export SAFERTOS_j721e_r5f_INSTALL_PATH = 
  export SAFERTOS_j721e_c66_INSTALL_PATH = 
  export SAFERTOS_j721e_c7x_INSTALL_PATH = 
endif

# This release is validated on below mentioned SafeRTOS Version on said SOC's/ISA's 
ifeq ($(BOARD),$(filter $(BOARD), j721e_evm ))
  SAFERTOS_VERSION_r5f = 009-002-199-024-219-004
  SAFERTOS_VERSION_c66 = 009-002-201-005-219-002
  SAFERTOS_VERSION_c7x = 009-002-230-005-219-005
endif

include $(PDK_INSTALL_PATH)/ti/build/makerules/platform.mk

ifeq ($(ISA),$(filter $(ISA), r5f c66 c7x))
  ifneq ($(SAFERTOS_$(SOC)_$(ISA)_INSTALL_PATH),)
    export SAFERTOS_KERNEL_INSTALL_PATH         = $(SAFERTOS_$(SOC)_$(ISA)_INSTALL_PATH)
    export SAFERTOS_KERNEL_INSTALL_PATH_$(ISA) ?= $(SAFERTOS_$(SOC)_$(ISA)_INSTALL_PATH)
  else
    export SAFERTOS_KERNEL_INSTALL_PATH         = $(SDK_INSTALL_PATH)/safertos_$(SOC)_$(ISA)_$(SAFERTOS_VERSION_$(ISA))
    export SAFERTOS_KERNEL_INSTALL_PATH_$(ISA) ?= $(SDK_INSTALL_PATH)/safertos_$(SOC)_$(ISA)_$(SAFERTOS_VERSION_$(ISA))
  endif
endif

# ISA based directory extensions used in SafeRTOS Package 
SAFERTOS_ISA_EXT_r5f = 199_TI_CR5
SAFERTOS_ISA_EXT_c66 = 201_C66x
SAFERTOS_ISA_EXT_c7x = 230_C7x
# Compiler based directory extensions used in SafeRTOS Package 
SAFERTOS_COMPILER_EXT_r5f = 024_Clang
SAFERTOS_COMPILER_EXT_c66 = 005_TI_CGT
SAFERTOS_COMPILER_EXT_c7x = 005_TI_CGT