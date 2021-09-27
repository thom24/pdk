
SRCDIR = . src/qnx
INCDIR = . src/qnx

SRCS_COMMON += HwiP_qnx.c SwiP_qnx.c Utils_qnx.c SemaphoreP_qnx.c RegisterIntr_qnx.c \
               TaskP_qnx.c QueueP_qnx.c DelayP_qnx.c CacheP_qnx.c DebugP_qnx.c \
               MutexP_qnx.c

ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200))
  SRCDIR      += soc/$(SOC)
  SRCS_COMMON +=
endif

ifeq ($(CORE),mpu1_0)
  SRCDIR += arch/core/a53
  SRCS_COMMON +=
endif

PACKAGE_SRCS_COMMON = makefile HwiP.h SwiP.h MuxIntcP.h osal.h osal_component.mk SemaphoreP.h CacheP.h \
                      TimerP.h RegisterIntr.h EventCombinerP.h QueueP.h TaskP.h EventP.h DebugP.h MutexP.h \
                      arch soc src/qnx src/src_common_qnx.mk \
                      build/makefile_qnx.mk
