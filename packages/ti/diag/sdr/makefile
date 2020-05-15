#
# SDR
#
# Software Diagnostics Reference Library Makefile
#
# Copyright (C) 2019-2020 Texas Instruments Incorporated - http://www.ti.com/
# ALL RIGHTS RESERVED
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

COMP = drvsdr

lib_$(COMP)_BUILD_DEPENDENCY =

$(COMP)_DOXYGEN_SUPPORT = no

include $(PDK_INSTALL_PATH)/ti/build/comp_top.mk
