# File: cbuff_component.mk
#       This file is component include makefile of CBUFF driver library.
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
ifeq ($(cbuff_component_make_include), )

drvcbuff_SOCLIST              = awr294x
drvcbuff_awr294x_CORELIST     = mcu1_0 c66xdsp_1
drvcbuff_BOARDLIST            = awr294x_evm
drvcbuff_awr294x_BOARDLIST    = awr294x_evm

############################
# cbuff package
# List of components included under cbuff lib
# The components included here are built and will be part of cbuff lib
############################
cbuff_LIB_LIST = cbuff

############################
# cbuff examples
# List of examples under cbuff (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
cbuff_EXAMPLE_LIST =

#
# Cbuff Modules
#

# Cbuff library
export cbuff_COMP_LIST = cbuff
cbuff_RELPATH = ti/drv/cbuff
cbuff_PATH = $(PDK_CBUFF_COMP_PATH)
export cbuff_LIBNAME = cbuff
export cbuff_LIBPATH = $(PDK_CBUFF_COMP_PATH)/lib
export cbuff_MAKEFILE = -fsrc/makefile
export cbuff_BOARD_DEPENDENCY = no
export cbuff_CORE_DEPENDENCY = yes
cbuff_PKG_LIST = cbuff
cbuff_INCLUDE = $(cbuff_PATH)
export cbuff_SOCLIST = $(drvcbuff_SOCLIST)
export cbuff_$(SOC)_CORELIST = $(drvcbuff_$(SOC)_CORELIST)

#
# Cbuff Examples
#

# Cbuff manual test app
export cbuff_manual_testapp_COMP_LIST = cbuff_manual_testapp
cbuff_manual_testapp_RELPATH = ti/drv/cbuff/example/cbuff_manual_testapp
cbuff_manual_testapp_PATH = $(PDK_CBUFF_COMP_PATH)/example/cbuff_manual_testapp
export cbuff_manual_testapp_BOARD_DEPENDENCY = yes
export cbuff_manual_testapp_CORE_DEPENDENCY = yes
export cbuff_manual_testapp_XDC_CONFIGURO = yes
cbuff_manual_testapp_PKG_LIST = cbuff_manual_testapp
cbuff_manual_testapp_INCLUDE = $(cbuff_manual_testapp_PATH)
export cbuff_manual_testapp_BOARDLIST = $(drvcbuff_awr294x_BOARDLIST)
export cbuff_manual_testapp_$(SOC)_CORELIST = $(drvcbuff_$(SOC)_CORELIST)
cbuff_EXAMPLE_LIST += cbuff_manual_testapp

export cbuff_LIB_LIST
export cbuff_EXAMPLE_LIST

CBUFF_CFLAGS =
export CBUFF_CFLAGS

cbuff_component_make_include := 1
endif
