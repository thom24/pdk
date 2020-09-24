# File: diag_ex_component.mk
#       This file is component include make file of Software Diagnostic Example
#       Applications.
# List of variables set in this file and their purpose:
# <mod>_RELPATH        - This is the relative path of the module, typically from
#                        top-level directory of the package
# <mod>_PATH           - This is the absolute path of the module. It derives from
#                        absolute path of the top-level directory (set in env.mk)
#                        and relative path set above
# <mod>_INCLUDE        - This is the path that has interface header files of the
#                        module. This can be multiple directories (space separated)
# <mod>_PKG_LIST       - Names of the modules (and sub-modules) that are a part
#                        part of this module, including itself.
# <mod>_BOARD_DEPENDENCY - "yes": means the code for this module depends on
#                             platform and the compiled obj/lib has to be kept
#                             under <platform> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no platform dependent code and hence
#                             the obj/libs are not kept under <platform> dir.
# <mod>_CORE_DEPENDENCY     - "yes": means the code for this module depends on
#                             core and the compiled obj/lib has to be kept
#                             under <core> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no core dependent code and hence
#                             the obj/libs are not kept under <core> dir.
# <mod>_APP_STAGE_FILES     - List of source files that belongs to the module
#                             <mod>, but that needs to be compiled at application
#                             build stage (in the context of the app). This is
#                             primarily for link time configurations or if the
#                             source file is dependent on options/defines that are
#                             application dependent. This can be left blank or
#                             not defined at all, in which case, it means there
#                             no source files in the module <mod> that are required
#                             to be compiled in the application build stage.
#
ifeq ($(diag_ex_component_make_include), )

############################
# Software Diagnostics Example Applications
# List of components included under Diagnostic Example applications
# The components included here are built and will be part of Diagnostic Example apps
############################
diag_ex_LIB_LIST =

diag_ex_SOCLIST         = j721e
diag_ex_BOARDLIST       = j721e_evm
diag_ex_j721e_CORELIST  = mcu1_0
diag_ex_j721e_APPCORELIST = mcu1_0
diag_ex_DISABLE_PARALLEL_MAKE = yes

############################
# Software diagnostic examples package
# List of software diagnostics included under diagnostic examples
# The components included here are built and will be part of software diagnostic examples
############################
diag_ex_LIB_LIST =

############################
# Software Diagnostic examples
# List of examples under diag_ex (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
diag_ex_EXAMPLE_LIST =

# Diagnostic Example Application have no locally built library components

diag_ex_COMP_LIST = diag_ex

#
# Software Diagnostic Application Examples
#

# ESM example
diag_ex_esm_example_app_COMP_LIST = diag_ex_esm_example_app
diag_ex_esm_example_app_RELPATH = ti/diag/examples/esm_example_app
diag_ex_esm_example_app_PATH = $(PDK_DIAG_EX_COMP_PATH)/esm_example_app
diag_ex_esm_example_app_MAKEFILE = -f makefile
diag_ex_esm_example_app_BOARD_DEPENDENCY = yes
diag_ex_esm_example_app_CORE_DEPENDENCY = yes
export diag_ex_esm_example_app_COMP_LIST
export diag_ex_esm_example_app_BOARD_DEPENDENCY
export diag_ex_esm_example_app_CORE_DEPENDENCY
export diag_ex_esm_example_app_MAKEFILE
diag_ex_esm_example_app_PKG_LIST = diag_ex_esm_example_app
diag_ex_esm_example_app_INCLUDE = $(diag_ex_esm_example_app_PATH)
diag_ex_esm_example_app_BOARDLIST = $(diag_ex_BOARDLIST)
export diag_ex_esm_example_app_BOARDLIST
diag_ex_esm_example_app_$(SOC)_CORELIST = $(diag_ex_$(SOC)_CORELIST)
export diag_ex_esm_example_app_$(SOC)_CORELIST
export diag_ex_esm_example_app_SBL_APPIMAGEGEN = yes

diag_ex_EXAMPLE_LIST += diag_ex_esm_example_app

# ECC example
diag_ex_ecc_example_app_COMP_LIST = diag_ex_ecc_example_app
diag_ex_ecc_example_app_RELPATH = ti/diag/examples/ecc_example_app
diag_ex_ecc_example_app_PATH = $(PDK_DIAG_EX_COMP_PATH)/ecc_example_app
diag_ex_ecc_example_app_MAKEFILE = -f makefile
diag_ex_ecc_example_app_BOARD_DEPENDENCY = yes
diag_ex_ecc_example_app_CORE_DEPENDENCY = yes
export diag_ex_ecc_example_app_COMP_LIST
export diag_ex_ecc_example_app_BOARD_DEPENDENCY
export diag_ex_ecc_example_app_CORE_DEPENDENCY
export diag_ex_ecc_example_app_MAKEFILE
diag_ex_ecc_example_app_PKG_LIST = diag_ex_ecc_example_app
diag_ex_ecc_example_app_INCLUDE = $(diag_ex_ecc_example_app_PATH)
diag_ex_ecc_example_app_BOARDLIST = $(diag_ex_BOARDLIST)
export diag_ex_ecc_example_app_BOARDLIST
diag_ex_ecc_example_app_$(SOC)_CORELIST = $(diag_ex_$(SOC)_CORELIST)
export diag_ex_ecc_example_app_$(SOC)_CORELIST
export diag_ex_ecc_example_app_SBL_APPIMAGEGEN = yes

diag_ex_EXAMPLE_LIST += diag_ex_ecc_example_app

export diag_ex_LIB_LIST
export diag_ex_EXAMPLE_LIST

DIAG_EX_CFLAGS =

export DIAG_EX_CFLAGS

diag_ex_component_make_include := 1
endif
