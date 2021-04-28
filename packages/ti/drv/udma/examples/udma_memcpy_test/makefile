#
# This file is the makefile for building UDMA memcpy test app for TI RTOS
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

ifeq ($(SMP), enable)
APP_NAME = udma_memcpy_smp_testapp_$(BUILD_OS_TYPE)
else
APP_NAME = udma_memcpy_testapp_$(BUILD_OS_TYPE)
endif

include makefile.mk
