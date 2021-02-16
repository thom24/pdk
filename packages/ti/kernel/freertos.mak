#
# This file is the makefile for building mailbox.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

MODULE_NAME = freertos

SRCDIR = freertos/dpl/$(ISA) 
SRCDIR += freertos/portable/TI_CGT/$(ISA)
SRCDIR += ${FREERTOS_KERNEL_INSTALL_PATH}/FreeRTOS-Kernel/ \
          ${FREERTOS_KERNEL_INSTALL_PATH}/FreeRTOS-Kernel/portable/MemMang \
          ${FREERTOS_LABS_INSTALL_PATH}/FreeRTOS-Plus-POSIX/FreeRTOS-Plus-POSIX/source
	
INCDIR = freertos/config/$(SOC)/$(ISA) dpl
INCDIR += ${FREERTOS_KERNEL_INSTALL_PATH}/FreeRTOS-Kernel/include \
          freertos/portable/TI_CGT/$(ISA) \
          ${FREERTOS_LABS_INSTALL_PATH}/FreeRTOS-Plus-POSIX/include \
          ${FREERTOS_LABS_INSTALL_PATH}/FreeRTOS-Plus-POSIX/include/private \
          ${FREERTOS_LABS_INSTALL_PATH}/FreeRTOS-Plus-POSIX/FreeRTOS-Plus-POSIX/include \
          ${FREERTOS_LABS_INSTALL_PATH}/FreeRTOS-Plus-POSIX/FreeRTOS-Plus-POSIX/include/portable

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += \
    Hwi.c \
    tasks.c \
    timers.c \
    queue.c \
    list.c \
    heap_3.c \
    FreeRTOS_POSIX_clock.c \
    FreeRTOS_POSIX_mqueue.c \
    FreeRTOS_POSIX_pthread_barrier.c \
    FreeRTOS_POSIX_pthread_cond.c \
    FreeRTOS_POSIX_pthread_mutex.c \
    FreeRTOS_POSIX_pthread.c \
    FreeRTOS_POSIX_sched.c \
    FreeRTOS_POSIX_semaphore.c \
    FreeRTOS_POSIX_timer.c \
    FreeRTOS_POSIX_unistd.c \
    FreeRTOS_POSIX_utils.c \
    port.c

SRCS_ASM_COMMON := \
    Hwi_asm_switch.asm \
    Hwi_disp_always.asm \
    Hwi_intcIsrDispatch.asm \
    TaskSupport_asm.asm

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

PACKAGE_SRCS_COMMON = freertos.mak freertos_component.mk makefile
PACKAGE_SRCS_COMMON += freertos
PACKAGE_SRCS_COMMON += test

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
