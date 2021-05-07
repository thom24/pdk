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

# File: safertos_component.mk
#       This file is component include make file of safertos library.
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
ifeq ($(safertos_component_make_include), )

safertos_BOARDLIST       = tpr12_evm awr294x_evm
safertos_SOCLIST         = tpr12 awr294x
safertos_tpr12_CORELIST  = c66xdsp_1
safertos_awr294x_CORELIST  = c66xdsp_1

############################
# safertos package
# List of components included under safertos lib
# The components included here are built and will be part of safertos lib
############################
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
safertos_LIB_LIST = safertos
endif


############################
# safertos examples
# List of examples under safertos
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
safertos_EXAMPLE_LIST =

ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
ifeq ($(SOC),$(filter $(SOC), $(safertos_SOCLIST)))
safertos_EXAMPLE_LIST =  safertos_test_task_switch
endif
endif

#
# safertos Modules
#

# safertos LIB
safertos_COMP_LIST = safertos
safertos_RELPATH = ti/kernel/safertos
safertos_PATH = $(PDK_SAFERTOS_COMP_PATH)
safertos_LIBNAME = ti.kernel.safertos
export safertos_LIBNAME
safertos_LIBPATH = $(safertos_PATH)/lib
export safertos_LIBPATH
safertos_OBJPATH = $(safertos_RELPATH)
export safertos_OBJPATH
safertos_MAKEFILE = -f safertos.mak
export safertos_MAKEFILE
safertos_BOARD_DEPENDENCY = no
safertos_CORE_DEPENDENCY = yes
safertos_SOC_DEPENDENCY = yes
export safertos_COMP_LIST
export safertos_BOARD_DEPENDENCY
export safertos_CORE_DEPENDENCY
export safertos_SOC_DEPENDENCY
safertos_PKG_LIST = safertos
export safertos_PKG_LIST
safertos_INCLUDE =  ${SAFERTOS_KERNEL_INSTALL_PATH}/SafeRTOS/include
safertos_INCLUDE += $(SAFERTOS_KERNEL_INSTALL_PATH)/portable/TI_CGT/$(ISA)
safertos_INCLUDE += $(SAFERTOS_KERNEL_INSTALL_PATH)/config/$(SOC)/$(ISA)
export safertos_SOCLIST
export safertos_$(SOC)_CORELIST



#
# safertos Examples
#
# safertos rtos baremetal test app

# safertos rtos task switch test app
safertos_test_task_switch_COMP_LIST = safertos_test_task_switch
safertos_test_task_switch_RELPATH = test/safertos/task_switch
safertos_test_task_switch_PATH = $(SAFERTOS_KERNEL_INSTALL_PATH)/test/safertos/task_switch
safertos_test_task_switch_BOARD_DEPENDENCY = yes
safertos_test_task_switch_CORE_DEPENDENCY = no
safertos_test_task_switch_XDC_CONFIGURO = no
safertos_test_task_switch_MAKEFILE = -f makefile BAREMETAL=yes
export safertos_test_task_switch_COMP_LIST
export safertos_test_task_switch_BOARD_DEPENDENCY
export safertos_test_task_switch_CORE_DEPENDENCY
export safertos_test_task_switch_XDC_CONFIGURO
export safertos_test_task_switch_MAKEFILE
safertos_test_task_switch_PKG_LIST = safertos_test_task_switch
export safertos_test_task_switch_PKG_LIST
safertos_test_task_switch_INCLUDE = $(safertos_test_task_switch_PATH)
safertos_test_task_switch_BOARDLIST = $(safertos_BOARDLIST)
export safertos_test_task_switch_BOARDLIST
safertos_test_task_switch_$(SOC)_CORELIST = $(safertos_$(SOC)_CORELIST)
export safertos_test_task_switch_$(SOC)_CORELIST

export safertos_LIB_LIST
export safertos_EXAMPLE_LIST

safertos_component_make_include := 1
endif