#
# Copyright (c) 2016 - 2020, Texas Instruments Incorporated
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

# File: canfd_component.mk
#       This file is component include make file of CANFD library.
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
#                             board and the compiled obj/lib has to be kept
#                             under <board> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no board dependent code and hence
#                             the obj/libs are not kept under <board> dir.
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
ifeq ($(canfd_component_make_include), )

drvcanfd_BOARDLIST       = tpr12_evm awr294x_evm
drvcanfd_SOCLIST         = tpr12 awr294x
drvcanfd_tpr12_CORELIST   = mcu1_0
drvcanfd_awr294x_CORELIST   = mcu1_0

############################
# canfd package
# List of components included under canfd lib
# The components included here are built and will be part of canfd lib
############################
canfd_LIB_LIST = canfd
drvcanfd_LIB_LIST = $(canfd_LIB_LIST)


############################
# canfd examples
# List of examples under canfd
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
canfd_EXAMPLE_LIST = 

ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
canfd_EXAMPLE_LIST = canfd_test canfd_freertos_test
endif

drvcanfd_EXAMPLE_LIST = $(canfd_EXAMPLE_LIST)

#
# CANFD Modules
#

# CANFD LIB
canfd_COMP_LIST = canfd
canfd_RELPATH = ti/drv/canfd
canfd_PATH = $(PDK_CANFD_COMP_PATH)
canfd_LIBNAME = ti.drv.canfd
export canfd_LIBNAME
canfd_LIBPATH = $(canfd_PATH)/lib
export canfd_LIBPATH
canfd_OBJPATH = $(canfd_RELPATH)/canfd
export canfd_OBJPATH
canfd_MAKEFILE = -f src/makefile
export canfd_MAKEFILE
canfd_BOARD_DEPENDENCY = no
canfd_CORE_DEPENDENCY = no
canfd_SOC_DEPENDENCY = yes
export canfd_COMP_LIST
export canfd_BOARD_DEPENDENCY
export canfd_CORE_DEPENDENCY
export canfd_SOC_DEPENDENCY
canfd_PKG_LIST = canfd
export canfd_PKG_LIST
canfd_INCLUDE = $(canfd_PATH)
canfd_SOCLIST = $(drvcanfd_SOCLIST)
export canfd_SOCLIST
canfd_$(SOC)_CORELIST = $(drvcanfd_$(SOC)_CORELIST)
export canfd_$(SOC)_CORELIST



#
# CANFD Examples
#
# CANFD rtos baremetal test app

# CANFD rtos test app
canfd_test_COMP_LIST = canfd_test
canfd_test_RELPATH = ti/drv/canfd/test
canfd_test_PATH = $(PDK_CANFD_COMP_PATH)/test
canfd_test_BOARD_DEPENDENCY = yes
canfd_test_CORE_DEPENDENCY = no
canfd_test_XDC_CONFIGURO = yes
canfd_test_MAKEFILE = -f makefile
export canfd_test_COMP_LIST
export canfd_test_BOARD_DEPENDENCY
export canfd_test_CORE_DEPENDENCY
export canfd_test_XDC_CONFIGURO
export canfd_test_MAKEFILE
canfd_test_PKG_LIST = canfd_test
export canfd_test_PKG_LIST
canfd_test_INCLUDE = $(canfd_test_PATH)
canfd_test_BOARDLIST = $(drvcanfd_BOARDLIST)
export canfd_test_BOARDLIST
canfd_test_$(SOC)_CORELIST = mcu1_0
export canfd_test_$(SOC)_CORELIST

canfd_freertos_test_COMP_LIST = canfd_freertos_test
canfd_freertos_test_RELPATH = ti/drv/canfd/test
canfd_freertos_test_PATH = $(PDK_CANFD_COMP_PATH)/test
canfd_freertos_test_BOARD_DEPENDENCY = yes
canfd_freertos_test_CORE_DEPENDENCY = no
canfd_freertos_test_XDC_CONFIGURO = yes
canfd_freertos_test_MAKEFILE = -f makefile IS_FREERTOS=yes
export canfd_freertos_test_COMP_LIST
export canfd_freertos_test_BOARD_DEPENDENCY
export canfd_freertos_test_CORE_DEPENDENCY
export canfd_freertos_test_XDC_CONFIGURO
export canfd_freertos_test_MAKEFILE
canfd_freertos_test_PKG_LIST = canfd_freertos_test
export canfd_freertos_test_PKG_LIST
canfd_freertos_test_INCLUDE = $(canfd_freertos_test_PATH)
canfd_freertos_test_BOARDLIST = $(drvcanfd_BOARDLIST)
export canfd_freertos_test_BOARDLIST
canfd_freertos_test_$(SOC)_CORELIST = mcu1_0
export canfd_freertos_test_$(SOC)_CORELIST



export drvcanfd_LIB_LIST
export canfd_LIB_LIST
export canfd_EXAMPLE_LIST
export drvcanfd_EXAMPLE_LIST

canfd_component_make_include := 1
endif
