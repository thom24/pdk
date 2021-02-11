# File: adcbuf_component.mk
#       This file is component include makefile of adcbuf driver library.
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
ifeq ($(adcbuf_component_make_include), )

drvadcbuf_SOCLIST            = awr294x
drvadcbuf_awr294x_CORELIST   = mcu1_0 c66xdsp_1
drvadcbuf_BOARDLIST          = awr294x_evm
drvadcbuf_awr294x_BOARDLIST  = awr294x_evm

############################
# adcbuf package
# List of components included under adcbuf lib
# The components included here are built and will be part of adcbuf lib
############################
adcbuf_LIB_LIST = adcbuf

############################
# adcbuf examples
# List of examples under adcbuf (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
adcbuf_EXAMPLE_LIST =

#
# adcbuf Modules
#

# adcbuf library
export adcbuf_COMP_LIST = adcbuf
adcbuf_RELPATH = ti/drv/adcbuf
adcbuf_PATH = $(PDK_ADCBUF_COMP_PATH)
export adcbuf_LIBNAME = adcbuf
export adcbuf_LIBPATH = $(PDK_ADCBUF_COMP_PATH)/lib
export adcbuf_MAKEFILE = -fsrc/makefile
export adcbuf_BOARD_DEPENDENCY = no
export adcbuf_CORE_DEPENDENCY = yes
adcbuf_PKG_LIST = adcbuf
adcbuf_INCLUDE = $(adcbuf_PATH)
export adcbuf_SOCLIST = $(drvadcbuf_SOCLIST)
export adcbuf_$(SOC)_CORELIST = $(drvadcbuf_$(SOC)_CORELIST)

#
# adcbuf Examples
#

# adcbuf test app
export adcbuf_testapp_COMP_LIST = adcbuf_testapp
adcbuf_testapp_RELPATH = ti/drv/adcbuf/example/
adcbuf_testapp_PATH = $(PDK_ADCBUF_COMP_PATH)/example
export adcbuf_testapp_BOARD_DEPENDENCY = yes
export adcbuf_testapp_CORE_DEPENDENCY = yes
export adcbuf_testapp_XDC_CONFIGURO = yes
adcbuf_testapp_PKG_LIST = adcbuf_testapp
adcbuf_testapp_INCLUDE = $(adcbuf_testapp_PATH)
export adcbuf_testapp_BOARDLIST = $(drvadcbuf_awr294x_BOARDLIST)
export adcbuf_testapp_$(SOC)_CORELIST = $(drvadcbuf_$(SOC)_CORELIST)
adcbuf_EXAMPLE_LIST += adcbuf_testapp

export adcbuf_LIB_LIST
export adcbuf_EXAMPLE_LIST

ADCBUF_CFLAGS =
export ADCBUF_CFLAGS

adcbuf_component_make_include := 1
endif
