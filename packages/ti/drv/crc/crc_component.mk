# File: crc_component.mk
#       This file is component include makefile of CRC driver library.
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
ifeq ($(crc_component_make_include), )

drvcrc_SOCLIST          = tpr12
drvcrc_tpr12_CORELIST   = $(DEFAULT_tpr12_CORELIST)

############################
# crc package
# List of components included under crc lib
# The components included here are built and will be part of crc lib
############################
crc_LIB_LIST = crc

############################
# crc examples
# List of examples under crc (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
crc_EXAMPLE_LIST =

#
# CRC Modules
#

# CRC library
crc_COMP_LIST = crc
crc_RELPATH = ti/drv/crc
crc_PATH = $(PDK_CRC_COMP_PATH)
crc_LIBNAME = crc
crc_LIBPATH = $(PDK_CRC_COMP_PATH)/lib
crc_MAKEFILE = -fsrc/makefile
export crc_MAKEFILE
export crc_LIBNAME
export crc_LIBPATH
crc_BOARD_DEPENDENCY = no
crc_CORE_DEPENDENCY = no
export crc_COMP_LIST
export crc_BOARD_DEPENDENCY
export crc_CORE_DEPENDENCY
crc_PKG_LIST = crc
crc_INCLUDE = $(crc_PATH)
crc_SOCLIST = $(drvcrc_SOCLIST)
export crc_SOCLIST
crc_$(SOC)_CORELIST = $(drvcrc_$(SOC)_CORELIST)
export crc_$(SOC)_CORELIST

#
# CRC Examples
#

export crc_LIB_LIST
export crc_EXAMPLE_LIST
export drvcrc_LIB_LIST = $(crc_LIB_LIST)
export drvcrc_EXAMPLE_LIST = $(crc_EXAMPLE_LIST)

CRC_CFLAGS =
export CRC_CFLAGS

crc_component_make_include := 1
endif
