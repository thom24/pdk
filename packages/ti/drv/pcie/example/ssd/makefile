# Makefile for PCIE SSD app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#CHANGES
>>>>>>> 36779907c (updated the makefile and other components to enable freertos)
=======
>>>>>>> 57bdc7c55 (PDK-9352: PCIE SSD support for mpu core on am65xx)
ifeq ($(BUILD_OS_TYPE),tirtos)
    CFLAGS_OS_DEFINES += -DUSE_BIOS -DTIRTOS
    # List all the external components/interfaces, whose interface header files
    #  need to be included for this component
    INCLUDE_EXTERNAL_INTERFACES += bios xdc pdk fatfs
    # List all the components required by the application
    COMP_LIST_COMMON += $(PDK_COMMON_TIRTOS_COMP)
<<<<<<< HEAD
=======
CFLAGS_OS_DEFINES += -DBAREMETAL
COMP_LIST_COMMON  += $(PDK_COMMON_BAREMETAL_COMP)
>>>>>>> 6deb7e263 (PDK-9352: PCIE SSD Support for am65xx)

INCLUDE_EXTERNAL_INTERFACES += pdk fatfs
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES) -DSSD

#Name of the directory created under packages/ti/binary/
APP_NAME = PCIE_ssd_ExampleProject

SRCDIR = . src ../sample/$(SOC)/src
INCDIR = . src ../sample/$(SOC)/src
# Common source files across all platforms and cores
SRCS_COMMON += nvme.c pcie_ssd.c pcie_sample_board.c fs_shell_app_utils.c fatfs_pcie_interface.c
PACKAGE_SRCS_COMMON = .

COMP_LIST_COMMON += fatfs_indp pcie gpio
=======
#Name of the directory created under packages/ti/binary/
APP_NAME = PCIE_ssd_ExampleProject
# Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
LOCAL_APP_NAME =  PCIE_ssd__$(BOARD)_$(CORE)Example_Project
=======
>>>>>>> 36779907c (updated the makefile and other components to enable freertos)

    ifeq ($(SOC),$(filter $(SOC), am65xx))
        ifeq ($(CORE),$(filter $(CORE), mpu1_0))
            # Enable XDC build for application by providing XDC CFG File per core
            XDC_CFG_FILE_$(CORE) += $(SOC)/pcieSSD_a53.cfg
        endif
    endif

    ifeq ($(SOC),$(filter $(SOC), am65xx))
        ifeq ($(CORE),$(filter $(CORE), mpu1_0))
            EXTERNAL_LNKCMD_FILE_LOCAL += $(SOC)/linker_a53.lds
        endif
    endif

    PCIE_OS_TESTPOSTFIX=_tirtos
endif


ifeq ($(BUILD_OS_TYPE), baremetal)
    CFLAGS_OS_DEFINES += -DBAREMETAL
    INCLUDE_EXTERNAL_INTERFACES += pdk fatfs
    LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
    COMP_LIST_COMMON  += $(PDK_COMMON_BAREMETAL_COMP)
    PCIE_OS_TESTPOSTFIX = _baremetal
endif

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES)

#Name of the directory created under packages/ti/binary/
APP_NAME = PCIE_ssd_ExampleProject$(PCIE_OS_TESTPOSTFIX)
#CHNAGES

ifeq ($(SOC),$(filter $(SOC), am65xx))
SRCDIR = . src ../sample/$(SOC)/src
INCDIR = . src ../sample/$(SOC)/src
# Common source files across all platforms and cores
SRCS_COMMON += nvme.c pcie_ssd.c pcie_sample_board.c fs_shell_app_utils.c fatfs_pcie_interface.c
PACKAGE_SRCS_COMMON = .
CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) -DSSD
endif
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)

COMP_LIST_COMMON += fatfs_indp pcie gpio

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
