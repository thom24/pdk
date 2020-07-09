# File: sciclient_component.mk
#       This file is component include make file.
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
ifeq ($(sciclient_component_make_include), )

############################
# sciclient package
# List of components included under sciclient
# The components included here are built and will be part of sciclient lib
############################
sciclient_LIB_LIST = sciclient
ifeq ($(SOC),$(filter $(SOC), am65xx j721e))
sciclient_LIB_LIST += sciclient_hs
endif

ifeq ($(SOC),$(filter $(SOC), j721e j7200))
sciclient_LIB_LIST += rm_pm_hal 
sciclient_LIB_LIST += sciserver_tirtos
sciclient_LIB_LIST += sciserver_baremetal
sciclient_LIB_LIST += sciclient_direct
endif

drvsciclient_BOARDLIST = am65xx_evm am65xx_idk j721e_sim j721e_evm j7200_evm am64x_evm
drvsciclient_SOCLIST = am65xx j721e j7200 am64x
drvsciclient_am65xx_CORELIST = mcu1_0 mcu1_1 mpu1_0
drvsciclient_j721e_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1 c7x-hostemu
drvsciclient_j7200_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvsciclient_am64x_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 m4f_0
drvsciclient_DISABLE_PARALLEL_MAKE = yes

export sciclient_COMP_LIST = sciclient
sciclient_RELPATH = ti/drv/sciclient
export sciclient_OBJPATH = ti/drv/sciclient
export sciclient_LIBNAME = sciclient
sciclient_PATH = $(PDK_SCICLIENT_COMP_PATH)
export sciclient_LIBPATH = $(PDK_SCICLIENT_COMP_PATH)/lib
export sciclient_MAKEFILE = -fsrc/sciclient_indirect_makefile BUILD_HS=no
# Simulator versus Silicon has a different Firmware Image.
sciclient_BOARD_DEPENDENCY = no
ifeq ($(BOARD),$(filter $(BOARD), j721e_ccqt j721e_loki j721e_hostemu))
sciclient_BOARD_DEPENDENCY = yes
endif
export sciclient_BOARD_DEPENDENCY
export sciclient_CORE_DEPENDENCY = yes
sciclient_PKG_LIST = sciclient
sciclient_INCLUDE = $(sciclient_PATH)
export sciclient_SOCLIST = $(drvsciclient_SOCLIST)
export sciclient_BOARDLIST = $(drvsciclient_BOARDLIST)
export sciclient_$(SOC)_CORELIST = $(drvsciclient_$(SOC)_CORELIST)

export sciclient_hs_COMP_LIST = sciclient_hs
sciclient_hs_RELPATH = ti/drv/sciclient
export sciclient_hs_OBJPATH = ti/drv/sciclient_hs
sciclient_hs_PATH = $(PDK_SCICLIENT_COMP_PATH)
export sciclient_hs_LIBNAME = sciclient_hs
export sciclient_hs_LIBPATH = $(PDK_SCICLIENT_COMP_PATH)/lib
export sciclient_hs_MAKEFILE = -fsrc/sciclient_indirect_makefile BUILD_HS=yes
# Simulator versus Silicon has a different Firmware Image.
sciclient_hs_BOARD_DEPENDENCY = no
ifeq ($(BOARD),$(filter $(BOARD), j721e_ccqt j721e_loki j721e_hostemu))
sciclient_hs_BOARD_DEPENDENCY = yes
endif
export sciclient_hs_BOARD_DEPENDENCY
export sciclient_hs_CORE_DEPENDENCY = yes
sciclient_hs_PKG_LIST = sciclient_hs
sciclient_hs_INCLUDE = $(sciclient_hs_PATH)
export sciclient_hs_SOCLIST = $(drvsciclient_SOCLIST)
export sciclient_hs_BOARDLIST = $(drvsciclient_BOARDLIST)
export sciclient_hs_$(SOC)_CORELIST = $(drvsciclient_$(SOC)_CORELIST)

export sciclient_direct_COMP_LIST = sciclient_direct
export sciclient_direct_RELPATH = ti/drv/sciclient_direct
export sciclient_direct_OBJPATH = ti/drv/sciclient_direct
export sciclient_direct_LIBNAME = sciclient_direct
export sciclient_direct_PATH = $(PDK_SCICLIENT_COMP_PATH)
export sciclient_direct_LIBPATH = $(PDK_SCICLIENT_COMP_PATH)/lib
export sciclient_direct_MAKEFILE = -fsrc/sciclient_direct_makefile BUILD_HS=no
export sciclient_direct_BOARD_DEPENDENCY = no
export sciclient_direct_CORE_DEPENDENCY = yes
export sciclient_direct_PKG_LIST = sciclient_direct
export sciclient_direct_INCLUDE = $(sciclient_direct_PATH)
export sciclient_direct_SOCLIST = j721e j7200
export sciclient_direct_BOARDLIST = j721e_evm j7200_evm
export sciclient_direct_$(SOC)_CORELIST = mcu1_0

export sciclient_direct_hs_COMP_LIST = sciclient_direct_hs
export sciclient_direct_hs_RELPATH = ti/drv/sciclient_direct
export sciclient_direct_hs_OBJPATH = ti/drv/sciclient_direct
export sciclient_direct_hs_LIBNAME = sciclient_hs_direct
export sciclient_direct_hs_PATH = $(PDK_SCICLIENT_COMP_PATH)
export sciclient_direct_hs_LIBPATH = $(PDK_SCICLIENT_COMP_PATH)/lib
export sciclient_direct_hs_MAKEFILE = -fsrc/sciclient_direct_makefile BUILD_HS=yes
export sciclient_direct_hs_BOARD_DEPENDENCY = no
export sciclient_direct_hs_CORE_DEPENDENCY = yes
export sciclient_direct_hs_PKG_LIST = sciclient_direct
export sciclient_direct_hs_INCLUDE = $(sciclient_hs_direct_PATH)
export sciclient_direct_hs_SOCLIST = j721e j7200
export sciclient_direct_hs_BOARDLIST = j721e_evm j7200_evm
export sciclient_direct_hs_$(SOC)_CORELIST = mcu1_0

export sciserver_tirtos_COMP_LIST = sciserver_tirtos
export sciserver_tirtos_RELPATH = ti/drv/sciserver_tirtos
export sciserver_tirtos_OBJPATH = ti/drv/sciserver_tirtos
export sciserver_tirtos_LIBNAME = sciserver_tirtos
export sciserver_tirtos_PATH = $(PDK_SCICLIENT_COMP_PATH)
export sciserver_tirtos_LIBPATH = $(PDK_SCICLIENT_COMP_PATH)/lib
export sciserver_tirtos_MAKEFILE = -fsrc/sciserver_makefile BUILD_OS_TYPE=tirtos
export sciserver_tirtos_BOARD_DEPENDENCY = no
export sciserver_tirtos_CORE_DEPENDENCY = yes
export sciserver_tirtos_PKG_LIST = sciserver_tirtos
export sciserver_tirtos_INCLUDE = $(sciserver_PATH)
export sciserver_tirtos_SOCLIST = j721e j7200
export sciserver_tirtos_BOARDLIST = j721e_evm j7200_evm
export sciserver_tirtos_$(SOC)_CORELIST = mcu1_0

export sciserver_baremetal_COMP_LIST = sciserver_baremetal
export sciserver_baremetal_RELPATH = ti/drv/sciserver_baremetal
export sciserver_baremetal_OBJPATH = ti/drv/sciserver_baremetal
export sciserver_baremetal_LIBNAME = sciserver_baremetal
export sciserver_baremetal_PATH = $(PDK_SCICLIENT_COMP_PATH)
export sciserver_baremetal_LIBPATH = $(PDK_SCICLIENT_COMP_PATH)/lib
export sciserver_baremetal_MAKEFILE = -fsrc/sciserver_makefile BUILD_OS_TYPE=baremetal
export sciserver_baremetal_BOARD_DEPENDENCY = no
export sciserver_baremetal_CORE_DEPENDENCY = yes
export sciserver_baremetal_PKG_LIST = sciserver_baremetal
export sciserver_baremetal_INCLUDE = $(sciserver_PATH)
export sciserver_baremetal_SOCLIST = j721e j7200
export sciserver_baremetal_BOARDLIST = j721e_evm j7200_evm
export sciserver_baremetal_$(SOC)_CORELIST = mcu1_0


export sciclient_UTILS_LIST = sciclient_boardcfg
export sciclient_boardcfg_COMP_LIST = sciclient_boardcfg
export sciclient_boardcfg_RELPATH = ti/drv/sciclient
export sciclient_boardcfg_PATH = $(PDK_SCICLIENT_COMP_PATH)
export sciclient_boardcfg_MAKEFILE = -fsrc/boardcfg_makefile
export sciclient_boardcfg_BOARD_DEPENDENCY = yes
export sciclient_boardcfg_CORE_DEPENDENCY = yes
export sciclient_boardcfg_BOARDLIST = $(sciclient_BOARDLIST)
export sciclient_boardcfg_$(SOC)_CORELIST = mcu1_0
export sciclient_boardcfg_LIBNAME = sciclient_boardcfg

export rm_pm_hal_COMP_LIST = rm_pm_hal
rm_pm_hal_RELPATH = ti/drv/sciclient/src/rm_pm_hal
export rm_pm_hal_OBJPATH = ti/drv/sciclient/src/rm_pm_hal
rm_pm_hal_PATH = $(PDK_SCICLIENT_COMP_PATH)
export rm_pm_hal_LIBNAME = rm_pm_hal
export rm_pm_hal_LIBPATH = $(PDK_SCICLIENT_COMP_PATH)/lib
export rm_pm_hal_MAKEFILE = -fsrc/rm_pm_hal_makefile BUILD_HS=no
# Simulator versus Silicon has a different Firmware Image.
rm_pm_hal_BOARD_DEPENDENCY = no
export rm_pm_hal_BOARD_DEPENDENCY
export rm_pm_hal_CORE_DEPENDENCY = yes
rm_pm_hal_PKG_LIST = rm_pm_hal
rm_pm_hal_INCLUDE = $(rm_pm_hal_PATH)
export rm_pm_hal_SOCLIST = j721e j7200
export rm_pm_hal_BOARDLIST = j721e_evm j7200_evm
export rm_pm_hal_$(SOC)_CORELIST = mcu1_0

############################
# sciclient examples
# List of examples under sciclient (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
sciclient_EXAMPLE_LIST =

# SCICLIENT Firmware Boot Application
sciclient_firmware_boot_TestApp_COMP_LIST = sciclient_firmware_boot_TestApp
sciclient_firmware_boot_TestApp_RELPATH = ti/drv/sciclient/examples/sciclient_firmware_boot_TestApp
sciclient_firmware_boot_TestApp_PATH = $(PDK_SCICLIENT_COMP_PATH)/examples/sciclient_firmware_boot_TestApp
export sciclient_firmware_boot_TestApp_SBL_CERT_KEY=$(SBL_CERT_KEY)
sciclient_firmware_boot_TestApp_BOARD_DEPENDENCY = no
sciclient_firmware_boot_TestApp_CORE_DEPENDENCY = yes
export sciclient_firmware_boot_TestApp_COMP_LIST
export sciclient_firmware_boot_TestApp_BOARD_DEPENDENCY
export sciclient_firmware_boot_TestApp_CORE_DEPENDENCY
sciclient_firmware_boot_TestApp_PKG_LIST = sciclient_firmware_boot_TestApp
sciclient_firmware_boot_TestApp_INCLUDE = $(sciclient_firmware_boot_TestApp_PATH)
sciclient_firmware_boot_TestApp_BOARDLIST = am65xx_evm am64x_evm j7200_evm
export sciclient_firmware_boot_TestApp_BOARDLIST
sciclient_firmware_boot_TestApp_$(SOC)_CORELIST = mcu1_0
export sciclient_firmware_boot_TestApp_$(SOC)_CORELIST
sciclient_firmware_boot_TestApp_SBL_APPIMAGEGEN = no
export sciclient_firmware_boot_TestApp_SBL_APPIMAGEGEN
ifeq ($(CORE),mcu1_0)
sciclient_firmware_boot_TestApp_SBL_IMAGEGEN = yes
else
sciclient_firmware_boot_TestApp_SBL_IMAGEGEN = no
endif
export sciclient_firmware_boot_TestApp_SBL_IMAGEGEN
sciclient_EXAMPLE_LIST += sciclient_firmware_boot_TestApp

# SCICLIENT CCS Init Application
sciclient_ccs_init_COMP_LIST = sciclient_ccs_init
sciclient_ccs_init_RELPATH = ti/drv/sciclient/examples/sciclient_ccs_init
sciclient_ccs_init_PATH = $(PDK_SCICLIENT_COMP_PATH)/examples/sciclient_ccs_init
export sciclient_ccs_init_SBL_CERT_KEY=$(SBL_CERT_KEY)
sciclient_ccs_init_BOARD_DEPENDENCY = no
sciclient_ccs_init_CORE_DEPENDENCY = yes
export sciclient_ccs_init_COMP_LIST
export sciclient_ccs_init_BOARD_DEPENDENCY
export sciclient_ccs_init_CORE_DEPENDENCY
sciclient_ccs_init_PKG_LIST = sciclient_ccs_init
sciclient_ccs_init_INCLUDE = $(sciclient_ccs_init_PATH)
sciclient_ccs_init_BOARDLIST = am65xx_evm j721e_sim j721e_evm am64x_evm j7200_evm
export sciclient_ccs_init_BOARDLIST
# This application is only for mcu1_0
sciclient_ccs_init_$(SOC)_CORELIST = mcu1_0
export sciclient_ccs_init_$(SOC)_CORELIST
sciclient_ccs_init_SBL_APPIMAGEGEN = no
export sciclient_ccs_init_SBL_APPIMAGEGEN
sciclient_ccs_init_SBL_IMAGEGEN = yes
export sciclient_ccs_init_SBL_IMAGEGEN
sciclient_EXAMPLE_LIST += sciclient_ccs_init

# SCICLIENT RTOS Application
sciclient_rtos_app_COMP_LIST = sciclient_rtos_app
sciclient_rtos_app_RELPATH = ti/drv/sciclient/examples/sciclient_rtos_app
sciclient_rtos_app_PATH = $(PDK_SCICLIENT_COMP_PATH)/examples/sciclient_rtos_app
sciclient_rtos_app_BOARD_DEPENDENCY = no
sciclient_rtos_app_CORE_DEPENDENCY = yes
sciclient_rtos_app_XDC_CONFIGURO = yes
export sciclient_rtos_app_COMP_LIST
export sciclient_rtos_app_BOARD_DEPENDENCY
export sciclient_rtos_app_CORE_DEPENDENCY
export sciclient_rtos_app_XDC_CONFIGURO
sciclient_rtos_app_PKG_LIST = sciclient_rtos_app
sciclient_rtos_app_INCLUDE = $(sciclient_rtos_app_PATH)
sciclient_rtos_app_BOARDLIST = am65xx_evm j721e_sim j721e_evm am64x_evm j7200_evm
export sciclient_rtos_app_BOARDLIST
ifeq ($(SOC), am64x)
# No M4F for the RTOS App
sciclient_rtos_app_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
else
sciclient_rtos_app_$(SOC)_CORELIST = $(drvsciclient_$(SOC)_CORELIST)
endif
export sciclient_rtos_app_$(SOC)_CORELIST
sciclient_rtos_app_SBL_APPIMAGEGEN = yes
export sciclient_rtos_app_SBL_APPIMAGEGEN
sciclient_rtos_app_SBL_IMAGEGEN = no
export sciclient_rtos_app_SBL_IMAGEGEN
ifeq ($(BUILD_OS_TYPE),tirtos)
sciclient_EXAMPLE_LIST += sciclient_rtos_app
endif

# SCICLIENT UT
sciclient_unit_testapp_COMP_LIST = sciclient_unit_testapp
sciclient_unit_testapp_RELPATH = ti/drv/sciclient/examples/sciclient_unit_testapp
sciclient_unit_testapp_PATH = $(PDK_SCICLIENT_COMP_PATH)/examples/sciclient_unit_testapp
sciclient_unit_testapp_BOARD_DEPENDENCY = no
sciclient_unit_testapp_CORE_DEPENDENCY = yes
sciclient_unit_testapp_XDC_CONFIGURO = yes
export sciclient_unit_testapp_COMP_LIST
export sciclient_unit_testapp_BOARD_DEPENDENCY
export sciclient_unit_testapp_CORE_DEPENDENCY
export sciclient_unit_testapp_XDC_CONFIGURO
sciclient_unit_testapp_PKG_LIST = sciclient_unit_testapp
sciclient_unit_testapp_INCLUDE = $(sciclient_unit_testapp_PATH)
sciclient_unit_testapp_BOARDLIST = am65xx_evm j721e_sim j721e_evm am64x_evm j7200_evm
export sciclient_unit_testapp_BOARDLIST
ifeq ($(SOC),am64x)
# No M4F for the RTOS App
sciclient_unit_testapp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
else
sciclient_unit_testapp_$(SOC)_CORELIST = $(drvsciclient_$(SOC)_CORELIST)
endif
ifeq ($(BOARD),$(filter $(BOARD), am64x_evm))
sciclient_unit_testapp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
endif
export sciclient_unit_testapp_$(SOC)_CORELIST

sciclient_unit_testapp_SBL_APPIMAGEGEN = no
ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200))
  sciclient_unit_testapp_SBL_APPIMAGEGEN = yes
endif
export sciclient_unit_testapp_SBL_APPIMAGEGEN

sciclient_unit_testapp_SBL_IMAGEGEN = no
export sciclient_unit_testapp_SBL_IMAGEGEN
ifeq ($(BUILD_OS_TYPE),tirtos)
sciclient_EXAMPLE_LIST += sciclient_unit_testapp
endif

# SCISERVER
export sciserver_testapp_COMP_LIST = sciserver_testapp
export sciserver_testapp_RELPATH = ti/drv/sciclient/examples/sciserver_testapp
export sciserver_testapp_PATH = $(PDK_SCICLIENT_COMP_PATH)/examples/sciserver_testapp
export sciserver_testapp_BOARD_DEPENDENCY = no
export sciserver_testapp_CORE_DEPENDENCY = yes
export sciserver_testapp_XDC_CONFIGURO = yes
export sciserver_testapp_PKG_LIST = sciserver_testapp
sciserver_testapp_INCLUDE = $(sciserver_testapp_PATH)
export sciserver_testapp_BOARDLIST = j721e_evm j7200_evm
export sciserver_testapp_$(SOC)_CORELIST = mcu1_0
export sciserver_testapp_SBL_APPIMAGEGEN = yes
export sciserver_testapp_SBL_IMAGEGEN = no
ifeq ($(BUILD_OS_TYPE),tirtos)
sciclient_EXAMPLE_LIST += sciserver_testapp
endif


# SCICLIENT Firewall Unit test
export sciclient_fw_testapp_COMP_LIST = sciclient_fw_testapp
sciclient_fw_testapp_RELPATH = ti/drv/sciclient/examples/sciclient_fw_testapp
sciclient_fw_testapp_PATH = $(PDK_SCICLIENT_COMP_PATH)/examples/sciclient_fw_testapp
export sciclient_fw_testapp_BOARD_DEPENDENCY = no
export sciclient_fw_testapp_CORE_DEPENDENCY = yes
export sciclient_fw_testapp_XDC_CONFIGURO = yes
sciclient_fw_testapp_PKG_LIST = sciclient_fw_testapp
export sciclient_fw_testapp_BOARDLIST = am65xx_evm j721e_evm j7200_evm
export sciclient_fw_testapp_$(SOC)_CORELIST = mcu1_0
export sciclient_fw_testapp_SBL_APPIMAGEGEN = yes
export sciclient_fw_testapp_SBL_IMAGEGEN = no
ifeq ($(BUILD_OS_TYPE),tirtos)
sciclient_EXAMPLE_LIST += sciclient_fw_testapp
endif

export sciclient_LIB_LIST
export sciclient_EXAMPLE_LIST
export drvsciclient_LIB_LIST = $(sciclient_LIB_LIST)
export drvsciclient_EXAMPLE_LIST  = $(sciclient_EXAMPLE_LIST)

sciclient_component_make_include := 1
endif
