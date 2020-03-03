# File: edma_component.mk
#       This file is component include makefile of EDMA driver library.
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
ifeq ($(edma_component_make_include), )

drvedma_SOCLIST          = tpr12
drvedma_tpr12_CORELIST   = $(DEFAULT_tpr12_CORELIST)

############################
# edma package
# List of components included under edma lib
# The components included here are built and will be part of edma lib
############################
edma_LIB_LIST = edma

############################
# edma examples
# List of examples under edma (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
edma_EXAMPLE_LIST =

#
# EDMA Modules
#

# EDMA library
edma_COMP_LIST = edma
edma_RELPATH = ti/drv/edma
edma_PATH = $(PDK_EDMA_COMP_PATH)
edma_LIBNAME = edma
edma_LIBPATH = $(PDK_EDMA_COMP_PATH)/lib
edma_MAKEFILE = -fsrc/makefile
export edma_MAKEFILE
export edma_LIBNAME
export edma_LIBPATH
edma_BOARD_DEPENDENCY = no
edma_CORE_DEPENDENCY = yes
export edma_COMP_LIST
export edma_BOARD_DEPENDENCY
export edma_CORE_DEPENDENCY
edma_PKG_LIST = edma
edma_INCLUDE = $(edma_PATH)
edma_SOCLIST = $(drvedma_SOCLIST)
export edma_SOCLIST
edma_$(SOC)_CORELIST = $(drvedma_$(SOC)_CORELIST)
export edma_$(SOC)_CORELIST

#
# EDMA Examples
#

export edma_LIB_LIST
export edma_EXAMPLE_LIST
export drvedma_LIB_LIST = $(edma_LIB_LIST)
export drvedma_EXAMPLE_LIST = $(edma_EXAMPLE_LIST)

EDMA_CFLAGS =
export EDMA_CFLAGS

edma_component_make_include := 1
endif
