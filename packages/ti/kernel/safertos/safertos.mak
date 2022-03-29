#
# This file is the makefile for building safertos.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

MODULE_NAME = safertos

BUILD_OS_TYPE = safertos

ifeq ($(ISA),$(filter $(ISA), r5f))
SAFERTOS_ISA_EXT=199_TI_CR5
SAFERTOS_COMPILER_EXT=024_Clang
endif
ifeq ($(ISA),$(filter $(ISA), c66))
SAFERTOS_ISA_EXT=201_C66x
SAFERTOS_COMPILER_EXT=005_TI_CGT
endif
ifeq ($(ISA),$(filter $(ISA), c7x))
SAFERTOS_ISA_EXT=230_C7x
SAFERTOS_COMPILER_EXT=005_TI_CGT
endif

SRCDIR = ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/api/PrivWrapperStd
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/kernel
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT)
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT)/$(SAFERTOS_COMPILER_EXT)
ifeq ($(ISA),$(filter $(ISA), r5f))
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/api/${SAFERTOS_ISA_EXT}
endif
ifeq ($(ISA),$(filter $(ISA), c7x))
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/demo_projects/SafeRTOS_TDA4VM_C7x_Demo/TI_c7x_Support
endif

INCDIR = ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/api/$(SAFERTOS_ISA_EXT)
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/api/PrivWrapperStd
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/config
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/kernel/include_api
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/kernel/include_prv
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT)
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT)/$(SAFERTOS_COMPILER_EXT)
ifeq ($(ISA),$(filter $(ISA), c7x))
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/SafeRTOS/api/NoWrapper
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/demo_projects/SafeRTOS_TDA4VM_C7x_Demo/TI_c7x_Support
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/source_code_and_projects/demo_projects/SafeRTOS_TDA4VM_C7x_Demo
endif

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
    eventgroups.c \
    semaphore.c \
    list.c

# SafeRTOS task and portable does not compile in CPP build. Skip it for CPP build
ifneq ($(CPLUSPLUS_BUILD), yes)
SRCS_COMMON += \
    task.c \
    portable.c
endif

#ISA specific C files
ifeq ($(ISA),$(filter $(ISA), r5f c66))
SRCS_COMMON += \
    apiSemaphoreWrapper.c \
    apiTimersWrapper.c \
    apiMutexWrapper.c \
    apiWrapper.c \
    apiEventGroupsWrapper.c \
    apiStreamBufferWrapper.c \
    apiEvtMplxWrapper.c 
endif
ifeq ($(ISA),$(filter $(ISA), r5f))
SRCS_COMMON += \
    mpuARM.c \
    portmpu.c \
    apiMPU.c
endif
ifeq ($(ISA),$(filter $(ISA), c7x))
# Following does not compile in CPP build. Skip it for CPP build
ifneq ($(CPLUSPLUS_BUILD), yes)
SRCS_COMMON += \
    boot.c \
    Exception.c \
    Hwi.c \
    Startup.c \
    c7x_module_config.c \
    Mmu.c \
    TimestampProvider.c \
    IntrinsicsSupport.c \
    Cache.c \
    Mmu_table.c 
endif
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

ifeq ($(ISA),$(filter $(ISA), c7x))
SRCS_ASM_COMMON := \
    portasm.asm \
    Mmu_asm.asm \
    Clobber_asm.asm \
    Cache_asm.asm \
    Hwi_asm.asm \
    Exception_asm.asm \
    Hwi_asm_switch.asm 
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
ifeq ($(ISA),$(filter $(ISA), c7x))
CFLAGS_LOCAL_COMMON += -DHwi_bootToNonSecure__D=true
CFLAGS_LOCAL_COMMON += -DException_vectors__D
endif

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
