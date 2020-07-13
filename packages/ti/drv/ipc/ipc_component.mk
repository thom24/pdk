# File: ipc_component.mk
#       This file is component include make file of IPC driver library.
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
ifeq ($(ipc_component_make_include), )

############################
# ipc package
# List of components included under ipc lib
# The components included here are built and will be part of ipc lib
############################
ipc_LIB_LIST = ipc

drvipc_SOCLIST         = am65xx j721e j7200 am64x
drvipc_BOARDLIST       = am65xx_evm am65xx_idk j721e_sim j721e_qt j721e_evm j7200_evm am64x_evm
drvipc_am65xx_CORELIST = mpu1_0 mcu1_0 mcu1_1
drvipc_am65xx_LASTCORE := $(word $(words $(drvipc_am65xx_CORELIST)), $(drvipc_am65xx_CORELIST))
drvipc_am65xx_BAREMETAL_CORELIST = mcu1_0 mcu1_1
drvipc_am65xx_TIRTOS_CORELIST = $(drvipc_am65xx_CORELIST)
drvipc_j721e_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu3_0 mcu1_1 mcu2_1 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1
drvipc_j721e_APPCORELIST = mpu1_0 mcu1_0 mcu2_0 mcu3_0 mcu1_1 mcu2_1 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1
drvipc_j721e_LASTCORE := $(word $(words $(drvipc_j721e_APPCORELIST)), $(drvipc_j721e_APPCORELIST))
drvipc_j721e_BAREMETAL_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvipc_j721e_TIRTOS_CORELIST = $(drvipc_j721e_CORELIST)
drvipc_j7200_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu1_1 mcu2_1
drvipc_j7200_LASTCORE := $(word $(words $(drvipc_j7200_CORELIST)), $(drvipc_j7200_CORELIST))
drvipc_j7200_BAREMETAL_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvipc_j7200_TIRTOS_CORELIST = $(drvipc_j7200_CORELIST)
drvipc_am64x_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 m4f_0
drvipc_am64x_BAREMETAL_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 m4f_0
drvipc_am64x_TIRTOS_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvipc_am64x_LASTCORE := $(word $(words $(drvipc_am64x_TIRTOS_CORELIST)), $(drvipc_am64x_TIRTOS_CORELIST))
drvipc_DISABLE_PARALLEL_MAKE = yes

############################
# ipc examples
# List of examples under ipc (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
ipc_EXAMPLE_LIST =

# IPC dependent examples which should be built after all examples are completed
ipc_DUP_EXAMPLE_LIST =

#
# IPC Modules
#

# IPC library
ipc_COMP_LIST = ipc
ipc_RELPATH = ti/drv/ipc
ipc_PATH = $(PDK_IPC_COMP_PATH)
ipc_LIBNAME = ipc
ipc_LIBPATH = $(PDK_IPC_COMP_PATH)/lib
ipc_MAKEFILE = -fsrc/makefile
export ipc_MAKEFILE
export ipc_LIBNAME
export ipc_LIBPATH
ipc_BOARD_DEPENDENCY = no
ipc_CORE_DEPENDENCY = yes
export ipc_COMP_LIST
export ipc_BOARD_DEPENDENCY
export ipc_CORE_DEPENDENCY
ipc_PKG_LIST = ipc
ipc_INCLUDE = $(ipc_PATH) $(bios_INCLUDE)
ipc_SOCLIST = $(drvipc_SOCLIST)
export ipc_SOCLIST
ipc_$(SOC)_CORELIST = $(drvipc_$(SOC)_CORELIST)
export ipc_$(SOC)_CORELIST

#
# IPC Baremetal Module
#

# IPC library
ipc_baremetal_COMP_LIST = ipc_baremetal
ipc_baremetal_RELPATH = ti/drv/ipc
ipc_baremetal_PATH = $(PDK_IPC_COMP_PATH)
ipc_baremetal_LIBNAME = ipc_baremetal
ipc_baremetal_LIBPATH = $(PDK_IPC_COMP_PATH)/lib
ipc_baremetal_OBJPATH = $(ipc_baremetal_RELPATH)/ipc_baremetal
export ipc_baremetal_OBJPATH
ipc_baremetal_MAKEFILE = -fsrc/makefile_baremetal
export ipc_baremetal_MAKEFILE
export ipc_baremetal_LIBNAME
export ipc_baremetal_LIBPATH
ipc_baremetal_BOARD_DEPENDENCY = no
ipc_baremetal_CORE_DEPENDENCY = yes
export ipc_baremetal_COMP_LIST
export ipc_baremetal_BOARD_DEPENDENCY
export ipc_baremetal_CORE_DEPENDENCY
ipc_baremetal_PKG_LIST = ipc_baremetal
ipc_baremetal_INCLUDE = $(ipc_baremetal_PATH)
ipc_baremetal_SOCLIST = $(drvipc_SOCLIST)
export ipc_baremetal_SOCLIST
ipc_baremetal_$(SOC)_CORELIST = $(drvipc_$(SOC)_CORELIST)
export ipc_baremetal_$(SOC)_CORELIST
ipc_LIB_LIST += ipc_baremetal

#
# IPC Examples
#

# IPC echo_test
ipc_echo_test_COMP_LIST = ipc_echo_test
ipc_echo_test_RELPATH = ti/drv/ipc/examples/echo_test
ipc_echo_test_PATH = $(PDK_IPC_COMP_PATH)/examples/echo_test
ipc_echo_test_BOARD_DEPENDENCY = yes
ipc_echo_test_CORE_DEPENDENCY = yes
ipc_echo_test_XDC_CONFIGURO = yes
export ipc_echo_test_COMP_LIST
export ipc_echo_test_BOARD_DEPENDENCY
export ipc_echo_test_CORE_DEPENDENCY
export ipc_echo_test_XDC_CONFIGURO
ipc_echo_test_PKG_LIST = ipc_echo_test
ipc_echo_test_INCLUDE = $(ipc_echo_test_PATH)
ipc_echo_test_BOARDLIST = $(drvipc_BOARDLIST)
export ipc_echo_test_BOARDLIST
ipc_echo_test_$(SOC)_CORELIST = $(drvipc_$(SOC)_TIRTOS_CORELIST)
export ipc_echo_test_$(SOC)_CORELIST
ipc_EXAMPLE_LIST += ipc_echo_test

-include $(PDK_IPC_COMP_PATH)/unit_test/ipc_ut_component.mk
ifneq ($(ipc_ut_LIB_LIST),)
  ipc_LIB_LIST += $(ipc_ut_LIB_LIST)
endif
ifneq ($(ipc_ut_EXAMPLE_LIST),)
  ipc_EXAMPLE_LIST += $(ipc_ut_EXAMPLE_LIST)
endif

# IPC echo_test baremetal
ipc_echo_baremetal_test_COMP_LIST = ipc_echo_baremetal_test
ipc_echo_baremetal_test_RELPATH = ti/drv/ipc/examples/echo_test/echo_test_baremetal
ipc_echo_baremetal_test_PATH = $(PDK_IPC_COMP_PATH)/examples/echo_test/echo_test_baremetal
ipc_echo_baremetal_test_MAKEFILE = -fmakefile.baremetal
ipc_echo_baremetal_test_BOARD_DEPENDENCY = yes
ipc_echo_baremetal_test_CORE_DEPENDENCY = yes
ipc_echo_baremetal_test_XDC_CONFIGURO = no
export ipc_echo_baremetal_test_MAKEFILE
export ipc_echo_baremetal_test_COMP_LIST
export ipc_echo_baremetal_test_BOARD_DEPENDENCY
export ipc_echo_baremetal_test_CORE_DEPENDENCY
export ipc_echo_baremetal_test_XDC_CONFIGURO
ipc_echo_baremetal_test_PKG_LIST = ipc_echo_baremetal_test
ipc_echo_baremetal_test_INCLUDE = $(ipc_echo_baremetal_test_PATH)
ipc_echo_baremetal_test_BOARDLIST = $(drvipc_BOARDLIST)
export ipc_echo_baremetal_test_BOARDLIST
ipc_echo_baremetal_test_$(SOC)_CORELIST = $(drvipc_$(SOC)_BAREMETAL_CORELIST)
export ipc_echo_baremetal_test_$(SOC)_CORELIST
ipc_EXAMPLE_LIST += ipc_echo_baremetal_test

# IPC echo_test - use R5F BTCM
ipc_echo_testb_COMP_LIST = ipc_echo_testb
ipc_echo_testb_RELPATH = ti/drv/ipc/examples/echo_test/echo_test_btcm
ipc_echo_testb_PATH = $(PDK_IPC_COMP_PATH)/examples/echo_test/echo_test_btcm
ipc_echo_testb_MAKEFILE = -fmakefile.btcm
ipc_echo_testb_BOARD_DEPENDENCY = yes
ipc_echo_testb_CORE_DEPENDENCY = yes
ipc_echo_testb_XDC_CONFIGURO = yes
export ipc_echo_testb_MAKEFILE
export ipc_echo_testb_COMP_LIST
export ipc_echo_testb_BOARD_DEPENDENCY
export ipc_echo_testb_CORE_DEPENDENCY
export ipc_echo_testb_XDC_CONFIGURO
ipc_echo_testb_PKG_LIST = ipc_echo_testb
ipc_echo_testb_INCLUDE = $(ipc_echo_testb_PATH)
ipc_echo_testb_BOARDLIST = $(drvipc_BOARDLIST)
export ipc_echo_testb_BOARDLIST
ipc_echo_testb_$(SOC)_CORELIST = $(drvipc_$(SOC)_TIRTOS_CORELIST)
export ipc_echo_testb_$(SOC)_CORELIST
ipc_EXAMPLE_LIST += ipc_echo_testb

# IPC ex01_bios_2core_echo_test
ex01_bios_2core_echo_test_COMP_LIST = ex01_bios_2core_echo_test
ex01_bios_2core_echo_test_RELPATH = ti/drv/ipc/examples/ex01_bios_2core_echo_test
ex01_bios_2core_echo_test_PATH = $(PDK_IPC_COMP_PATH)/examples/ex01_bios_2core_echo_test
ex01_bios_2core_echo_test_BOARD_DEPENDENCY = yes
ex01_bios_2core_echo_test_CORE_DEPENDENCY = yes
ex01_bios_2core_echo_test_XDC_CONFIGURO = yes
export ex01_bios_2core_echo_test_COMP_LIST
export ex01_bios_2core_echo_test_BOARD_DEPENDENCY
export ex01_bios_2core_echo_test_CORE_DEPENDENCY
export ex01_bios_2core_echo_test_XDC_CONFIGURO
ex01_bios_2core_echo_test_PKG_LIST = ex01_bios_2core_echo_test
ex01_bios_2core_echo_test_INCLUDE = $(ex01_bios_2core_echo_test_PATH)
ex01_bios_2core_echo_test_BOARDLIST = $(drvipc_BOARDLIST)
export ex01_bios_2core_echo_test_BOARDLIST
ex01_bios_2core_echo_test_$(SOC)_CORELIST = $(drvipc_$(SOC)_TIRTOS_CORELIST)
export ex01_bios_2core_echo_test_$(SOC)_CORELIST
ipc_EXAMPLE_LIST += ex01_bios_2core_echo_test

# IPC ex02_bios_multicore_echo_test
ex02_bios_multicore_echo_test_COMP_LIST = ex02_bios_multicore_echo_test
ex02_bios_multicore_echo_test_RELPATH = ti/drv/ipc/examples/ex02_bios_multicore_echo_test
ex02_bios_multicore_echo_test_PATH = $(PDK_IPC_COMP_PATH)/examples/ex02_bios_multicore_echo_test
ex02_bios_multicore_echo_test_BOARD_DEPENDENCY = yes
ex02_bios_multicore_echo_test_CORE_DEPENDENCY = yes
ex02_bios_multicore_echo_test_XDC_CONFIGURO = yes
export ex02_bios_multicore_echo_test_COMP_LIST
export ex02_bios_multicore_echo_test_BOARD_DEPENDENCY
export ex02_bios_multicore_echo_test_CORE_DEPENDENCY
export ex02_bios_multicore_echo_test_XDC_CONFIGURO
ex02_bios_multicore_echo_test_PKG_LIST = ex02_bios_multicore_echo_test
ex02_bios_multicore_echo_test_INCLUDE = $(ex02_bios_multicore_echo_test_PATH)
ex02_bios_multicore_echo_test_BOARDLIST = $(drvipc_BOARDLIST)
export ex02_bios_multicore_echo_test_BOARDLIST
ex02_bios_multicore_echo_test_$(SOC)_CORELIST = $(drvipc_$(SOC)_TIRTOS_CORELIST)
export ex02_bios_multicore_echo_test_$(SOC)_CORELIST
ipc_EXAMPLE_LIST += ex02_bios_multicore_echo_test

# IPC ex02_baremetal_multicore_echo_test
ex02_baremetal_multicore_echo_test_COMP_LIST = ex02_baremetal_multicore_echo_test
ex02_baremetal_multicore_echo_test_RELPATH = ti/drv/ipc/examples/ex02_baremetal_multicore_echo_test
ex02_baremetal_multicore_echo_test_PATH = $(PDK_IPC_COMP_PATH)/examples/ex02_baremetal_multicore_echo_test
ex02_baremetal_multicore_echo_test_BOARD_DEPENDENCY = yes
ex02_baremetal_multicore_echo_test_CORE_DEPENDENCY = yes
ex02_baremetal_multicore_echo_test_XDC_CONFIGURO = no
export ex02_baremetal_multicore_echo_test_COMP_LIST
export ex02_baremetal_multicore_echo_test_BOARD_DEPENDENCY
export ex02_baremetal_multicore_echo_test_CORE_DEPENDENCY
export ex02_baremetal_multicore_echo_test_XDC_CONFIGURO
ex02_baremetal_multicore_echo_test_PKG_LIST = ex02_baremetal_multicore_echo_test
ex02_baremetal_multicore_echo_test_INCLUDE = $(ex02_baremetal_multicore_echo_test_PATH)
ex02_baremetal_multicore_echo_test_BOARDLIST = $(drvipc_BOARDLIST)
export ex02_baremetal_multicore_echo_test_BOARDLIST
ex02_baremetal_multicore_echo_test_$(SOC)_CORELIST = $(drvipc_$(SOC)_BAREMETAL_CORELIST)
export ex02_baremetal_multicore_echo_test_$(SOC)_CORELIST
ipc_EXAMPLE_LIST += ex02_baremetal_multicore_echo_test

# IPC ex05_bios_multicore_echo_negative_test
ex05_bios_multicore_echo_negative_test_COMP_LIST = ex05_bios_multicore_echo_negative_test
ex05_bios_multicore_echo_negative_test_RELPATH = ti/drv/ipc/examples/ex05_bios_multicore_echo_negative_test
ex05_bios_multicore_echo_negative_test_PATH = $(PDK_IPC_COMP_PATH)/examples/ex05_bios_multicore_echo_negative_test
ex05_bios_multicore_echo_negative_test_BOARD_DEPENDENCY = yes
ex05_bios_multicore_echo_negative_test_CORE_DEPENDENCY = yes
ex05_bios_multicore_echo_negative_test_XDC_CONFIGURO = yes
export ex05_bios_multicore_echo_negative_test_COMP_LIST
export ex05_bios_multicore_echo_negative_test_BOARD_DEPENDENCY
export ex05_bios_multicore_echo_negative_test_CORE_DEPENDENCY
export ex05_bios_multicore_echo_negative_test_XDC_CONFIGURO
ex05_bios_multicore_echo_negative_test_PKG_LIST = ex05_bios_multicore_echo_negative_test
ex05_bios_multicore_echo_negative_test_INCLUDE = $(ex05_bios_multicore_echo_negative_test_PATH)
ex05_bios_multicore_echo_negative_test_BOARDLIST = $(drvipc_BOARDLIST)
export ex05_bios_multicore_echo_negative_test_BOARDLIST
ex05_bios_multicore_echo_negative_test_$(SOC)_CORELIST = $(drvipc_$(SOC)_TIRTOS_CORELIST)
export ex05_bios_multicore_echo_negative_test_$(SOC)_CORELIST
ipc_EXAMPLE_LIST += ex05_bios_multicore_echo_negative_test

# IPC ex02_bios_multicore_echo_testb - use R5F BTCM
ex02_bios_multicore_echo_testb_COMP_LIST = ex02_bios_multicore_echo_testb
ex02_bios_multicore_echo_testb_RELPATH = ti/drv/ipc/examples/ex02_bios_multicore_echo_test/ex02_bios_multicore_echo_testb
ex02_bios_multicore_echo_testb_PATH = $(PDK_IPC_COMP_PATH)/examples/ex02_bios_multicore_echo_test/ex02_bios_multicore_echo_testb
ex02_bios_multicore_echo_testb_MAKEFILE = -fmakefile.btcm
ex02_bios_multicore_echo_testb_BOARD_DEPENDENCY = yes
ex02_bios_multicore_echo_testb_CORE_DEPENDENCY = yes
ex02_bios_multicore_echo_testb_XDC_CONFIGURO = yes
export ex02_bios_multicore_echo_testb_COMP_LIST
export ex02_bios_multicore_echo_testb_BOARD_DEPENDENCY
export ex02_bios_multicore_echo_testb_CORE_DEPENDENCY
export ex02_bios_multicore_echo_testb_XDC_CONFIGURO
ex02_bios_multicore_echo_testb_PKG_LIST = ex02_bios_multicore_echo_testb
ex02_bios_multicore_echo_testb_INCLUDE = $(ex02_bios_multicore_echo_testb_PATH)
ex02_bios_multicore_echo_testb_BOARDLIST = $(drvipc_BOARDLIST)
export ex02_bios_multicore_echo_testb_BOARDLIST
ex02_bios_multicore_echo_testb_$(SOC)_CORELIST = $(drvipc_$(SOC)_TIRTOS_CORELIST)
export ex02_bios_multicore_echo_testb_$(SOC)_CORELIST
ipc_EXAMPLE_LIST += ex02_bios_multicore_echo_testb

# IPC ex03_linux_bios_2core_echo_test
ex03_linux_bios_2core_echo_test_COMP_LIST = ex03_linux_bios_2core_echo_test
ex03_linux_bios_2core_echo_test_RELPATH = ti/drv/ipc/examples/ex03_linux_bios_2core_echo_test
ex03_linux_bios_2core_echo_test_PATH = $(PDK_IPC_COMP_PATH)/examples/ex03_linux_bios_2core_echo_test
ex03_linux_bios_2core_echo_test_BOARD_DEPENDENCY = yes
ex03_linux_bios_2core_echo_test_CORE_DEPENDENCY = yes
ex03_linux_bios_2core_echo_test_XDC_CONFIGURO = yes
export ex03_linux_bios_2core_echo_test_COMP_LIST
export ex03_linux_bios_2core_echo_test_BOARD_DEPENDENCY
export ex03_linux_bios_2core_echo_test_CORE_DEPENDENCY
export ex03_linux_bios_2core_echo_test_XDC_CONFIGURO
ex03_linux_bios_2core_echo_test_PKG_LIST = ex03_linux_bios_2core_echo_test
ex03_linux_bios_2core_echo_test_INCLUDE = $(ex03_linux_bios_2core_echo_test_PATH)
ex03_linux_bios_2core_echo_test_BOARDLIST = $(drvipc_BOARDLIST)
export ex03_linux_bios_2core_echo_test_BOARDLIST
ex03_linux_bios_2core_echo_test_$(SOC)_CORELIST = $(drvipc_$(SOC)_TIRTOS_CORELIST)
export ex03_linux_bios_2core_echo_test_$(SOC)_CORELIST
ipc_EXAMPLE_LIST += ex03_linux_bios_2core_echo_test

# IPC performance_test
ipc_perf_test_COMP_LIST = ipc_perf_test
ipc_perf_test_RELPATH = ti/drv/ipc/examples/ipc_perf_test
ipc_perf_test_PATH = $(PDK_IPC_COMP_PATH)/examples/ipc_perf_test
ipc_perf_test_BOARD_DEPENDENCY = yes
ipc_perf_test_CORE_DEPENDENCY = yes
ipc_perf_test_XDC_CONFIGURO = yes
export ipc_perf_test_COMP_LIST
export ipc_perf_test_BOARD_DEPENDENCY
export ipc_perf_test_CORE_DEPENDENCY
export ipc_perf_test_XDC_CONFIGURO
ipc_perf_test_PKG_LIST = ipc_perf_test
ipc_perf_test_INCLUDE = $(ipc_perf_test_PATH)
ipc_perf_test_BOARDLIST = $(drvipc_BOARDLIST)
export ipc_perf_test_BOARDLIST
ipc_perf_test_$(SOC)_CORELIST = $(drvipc_$(SOC)_TIRTOS_CORELIST)
export ipc_perf_test_$(SOC)_CORELIST
ipc_EXAMPLE_LIST += ipc_perf_test
export ipc_perf_test_SBL_APPIMAGEGEN = yes

# Multicore IPC performance test
ipc_multicore_perf_test_COMP_LIST = ipc_multicore_perf_test
ipc_multicore_perf_test_RELPATH = ti/drv/ipc/examples/ipc_perf_test
ipc_multicore_perf_test_BINPATH = $(PDK_INSTALL_PATH)/ti/binary/ipc_perf_test/bin
ipc_multicore_perf_test_PATH = $(PDK_IPC_COMP_PATH)/examples/ipc_perf_test
ipc_multicore_perf_test_MAKEFILE = -f$(PDK_IPC_COMP_PATH)/examples/ipc_multicore_perf_test.mk
export ipc_multicore_perf_test_MAKEFILE
# IPC multicore perf test depends on ipc_perf_test for all the cores
ipc_multicore_perf_test_DEPENDS_ON=ipc_perf_test
export ipc_multicore_perf_test_DEPENDS_ON
ipc_multicore_perf_test_BOARD_DEPENDENCY = yes
ipc_multicore_perf_test_CORE_DEPENDENCY = yes
ipc_multicore_perf_test_XDC_CONFIGURO = yes
export ipc_multicore_perf_test_COMP_LIST
export ipc_multicore_perf_test_BOARD_DEPENDENCY
export ipc_multicore_perf_test_CORE_DEPENDENCY
export ipc_multicore_perf_test_XDC_CONFIGURO
ipc_multicore_perf_test_PKG_LIST = ipc_multicore_perf_test
ipc_multicore_perf_test_INCLUDE = $(ipc_multicore_perf_test_PATH)
ipc_multicore_perf_test_BOARDLIST = $(drvipc_BOARDLIST)
export ipc_multicore_perf_test_BOARDLIST
ipc_multicore_perf_test_$(SOC)_CORELIST := $(drvipc_$(SOC)_LASTCORE)
export ipc_multicore_perf_test_$(SOC)_CORELIST
ipc_DUP_EXAMPLE_LIST += ipc_multicore_perf_test
ipc_multicore_perf_test_SBL_APPIMAGEGEN = no
export ipc_multicore_perf_test_SBL_APPIMAGEGEN

# IPC ex04_linux_baremetal_2core_echo_test
ex04_linux_baremetal_2core_echo_test_COMP_LIST = ex04_linux_baremetal_2core_echo_test
ex04_linux_baremetal_2core_echo_test_RELPATH = ti/drv/ipc/examples/ex04_linux_baremetal_2core_echo_test
ex04_linux_baremetal_2core_echo_test_PATH = $(PDK_IPC_COMP_PATH)/examples/ex04_linux_baremetal_2core_echo_test
ex04_linux_baremetal_2core_echo_test_BOARD_DEPENDENCY = no
ex04_linux_baremetal_2core_echo_test_CORE_DEPENDENCY = yes
ex04_linux_baremetal_2core_echo_test_XDC_CONFIGURO = no
export ex04_linux_baremetal_2core_echo_test_COMP_LIST
export ex04_linux_baremetal_2core_echo_test_BOARD_DEPENDENCY
export ex04_linux_baremetal_2core_echo_test_CORE_DEPENDENCY
export ex04_linux_baremetal_2core_echo_test_XDC_CONFIGURO
ex04_linux_baremetal_2core_echo_test_PKG_LIST = ex04_linux_baremetal_2core_echo_test
ex04_linux_baremetal_2core_echo_test_INCLUDE = $(ex04_linux_baremetal_2core_echo_test_PATH)
ex04_linux_baremetal_2core_echo_test_BOARDLIST = j721e_evm am65xx_evm
export ex04_linux_baremetal_2core_echo_test_BOARDLIST
ex04_linux_baremetal_2core_echo_test_$(SOC)_CORELIST = $(drvipc_$(SOC)_BAREMETAL_CORELIST)
export ex04_linux_baremetal_2core_echo_test_$(SOC)_CORELIST
ipc_EXAMPLE_LIST += ex04_linux_baremetal_2core_echo_test

export ipc_LIB_LIST
export ipc_EXAMPLE_LIST
export ipc_DUP_EXAMPLE_LIST
export drvipc_LIB_LIST = $(ipc_LIB_LIST)
export drvipc_EXAMPLE_LIST  = $(ipc_EXAMPLE_LIST)
export drvipc_DUP_EXAMPLE_LIST  = $(ipc_DUP_EXAMPLE_LIST)

IPC_CFLAGS =

# Enable asserts and prints
IPC_CFLAGS += -DIPC_CFG_ASSERT_ENABLE
#IPC_CFLAGS += -DIPC_CFG_USE_STD_ASSERT
IPC_CFLAGS += -DIPC_CFG_PRINT_ENABLE

export IPC_CFLAGS

ipc_component_make_include := 1
endif
