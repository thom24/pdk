#
# This file is the makefile for building safertos.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

MODULE_NAME = safertos

SRCDIR = ${SAFERTOS_KERNEL_INSTALL_PATH}/portable/TI_CGT/$(ISA)
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/SafeRTOS

INCDIR = ${SAFERTOS_KERNEL_INSTALL_PATH}/config/$(SOC)/$(ISA)
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH}/SafeRTOS/include \
          ${SAFERTOS_KERNEL_INSTALL_PATH}/portable/TI_CGT/$(ISA)


# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += \
	eventgroups.c \
	mutex.c \
	runtimestats.c \
	semaphore.c \
    timers.c \
    queue.c \
    list.c \
    port.c

# SafeRTOS task does not compile in CPP build. Skip it for CPP build
ifneq ($(CPLUSPLUS_BUILD), yes)
SRCS_COMMON += \
    task.c
endif

#ISA specific C files
ifeq ($(ISA),$(filter $(ISA), c66))
SRCS_COMMON += \
    port_Hwi.c
endif

ifeq ($(ISA),$(filter $(ISA), r5f))
SRCS_COMMON += \
    queue_register.c \
    mpuArm.c \
    mpuAPI.c \
    portmpu.c
endif

#ISA specific assembly files
ifeq ($(ISA),$(filter $(ISA), c66))
SRCS_ASM_COMMON := \
    port_Hwi_asm_switch.asm \
    port_Hwi_disp_always.asm \
    port_Hwi_intcIsrDispatch.asm \
    port_TaskSupport_asm.asm
endif

ifeq ($(ISA),$(filter $(ISA), r5f))
SRCS_ASM_COMMON := \
    portasm.asm
endif


CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

PACKAGE_SRCS_COMMON = safertos.mak safertos_component.mk

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
