#
# This file is the makefile for building safertos.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

MODULE_NAME = safertos

ifeq ($(ISA),$(filter $(ISA), r5f))
SAFERTOS_ISA_EXT=199_TI_CR5
SAFERTOS_COMPILER_EXT=024_Clang
endif
ifeq ($(ISA),$(filter $(ISA), c66))
SAFERTOS_ISA_EXT=201_C66x
SAFERTOS_COMPILER_EXT=005_TI_CGT
endif

SRCDIR = ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/api/PrivWrapperStd
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/kernel
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT)
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT)/$(SAFERTOS_COMPILER_EXT)
ifeq ($(ISA),$(filter $(ISA), r5f))
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/api/${SAFERTOS_ISA_EXT}
endif

INCDIR = ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/api/$(SAFERTOS_ISA_EXT)
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/api/PrivWrapperStd
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/config
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/kernel/include_api
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/kernel/include_prv
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT)
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT)/$(SAFERTOS_COMPILER_EXT)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += \
    timers.c \
    evtmplx.c \
    mutex.c \
    streambuffer.c \
    queue.c \
    task.c \
    eventgroups.c \
    semaphore.c \
    list.c \
    apiSemaphoreWrapper.c \
    apiTimersWrapper.c \
    apiMutexWrapper.c \
    apiWrapper.c \
    apiEventGroupsWrapper.c \
    apiStreamBufferWrapper.c \
    apiEvtMplxWrapper.c \
    portable.c

#ISA specific C files
ifeq ($(ISA),$(filter $(ISA), r5f))
SRCS_COMMON += \
    mpuARM.c \
    portmpu.c \
    apiMPU.c
endif

#ISA specific assembly files
ifeq ($(ISA),$(filter $(ISA), c66))
SRCS_ASM_COMMON := \
	portasm.asm
endif

ifeq ($(ISA),$(filter $(ISA), r5f))
SRCS_S_COMMON := \
    portasm.S
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

PACKAGE_SRCS_COMMON = safertos.mak safertos_component.mk
PACKAGE_SRCS_COMMON += ../test/safertos/task_switch

#include posix support files if present
-include safertos_posix.mak

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
