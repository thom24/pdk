#################################################################################
# Path Configuration
#################################################################################
# Get directory of makefile (RELATIVE)
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

# Setup build variables relative to makefile folder
ifeq ($(SDK_INSTALL_PATH),)
PDK_INSTALL_PATH ?= $(abspath $(mkfile_dir)../../../../..)
SDK_INSTALL_PATH ?= $(abspath $(PDK_INSTALL_PATH)../../..)
else
PDK_INSTALL_PATH ?= $(mkfile_dir)../../../../..
endif

#################################################################################
# General Configuration
#################################################################################
# Output Configuration
#
#     OUT_DIR defines the output directory for the combined appimage.
#     OUT_IMG defines the name of the combined appimage
#
OUT_DIR ?= $(mkfile_dir)bin
OUT_IMG ?= combined.appimage

# Raw Binary Extensions
#
#     Each of the extensions in this list will be treated as a binary blob, thus
#     indicating that these formats must be converted into ELF prior to RPRC
#     generation. ALL OTHER EXTENSIONS WILL BE CONSIDERED AS EXECUTABLE.
#
RAW_EXTENSIONS ?= .bin .dtb

# Consider extensionless files as raw binary or executable?
#
#     Some files do not have extensions, we can either consider them as raw
#     binary or as executables, but not as both.
#
#     Choices:
#         - raw: raw binary
#         - exe: executable
#
EXTENSIONLESS_TYPE ?= raw

# SOC/Board Configuration
#
#     This is ONLY important for expanding macros in <ti/build/makerules/platform.mk>
#
#     NOTE: all K3 devices have the same definitions here, so the default should
#           be safe.
#
SOC ?= am65xx
BOARD ?= am65xx_evm

#################################################################################
# HLOS Configuration
#################################################################################

# HLOS Boot Profile
#
#     This setting configures the method by which the HLOS is "booted". In all
#     cases, SBL will technically bring up the board, and then kick off an HLOS
#     "boot" as configured here.
#
#     Choices are as follows:
#         - "optimized" : ATF/OPTEE start the HLOS kernel directly
#         - "development"   : ATF/OPTEE start SPL, which uses U-Boot to boot HLOS
#         - <NULL>        : No HLOS boot
#
HLOS_BOOT ?= optimized

# Path which contains all HLOS binaries
HLOS_BIN_PATH ?= ~/ti-processor-sdk-linux/board-support/prebuilt-images

# HLOS Images
#
#     Each HLOS component needs to be converted to an ELF prior to RPRC/appimage
#     generation. So a Load Address will be needed, and if the image is intended to
#     be started by SBL (i.e. ATF), then an Entry Point must also be provided.
#
#     Each image configuration should be formatted as follows:
#
#         <Device ID>,<Binary File>,<Load Address>,[Entry Point]
#
#     NOTE: Device ID's are defined in <ti/build/makerules/platform.mk>
#
ATF_IMG    ?= mpu1_0,$(HLOS_BIN_PATH)/bl31.bin,0x70000000,0x70000000
OPTEE_IMG  ?= load_only,$(HLOS_BIN_PATH)/bl32.bin,0x9e800000
KERNEL_IMG ?= load_only,$(HLOS_BIN_PATH)/Image,0x80080000
DTB_IMG    ?= load_only,$(HLOS_BIN_PATH)/base-board.dtb,0x82000000
SPL_IMG    ?= 

#################################################################################
# RTOS/Baremetal Configuration
#################################################################################

# Path which contains all RTOS binaries
RTOS_BIN_PATH ?= ~/ti/pdk/packages/ti/boot/sbl/example/k3MulticoreApp/binary/am65xx

# RTOS/Baremetal Images
#
#     Each image here is expected to be in an executable format (e.g. xer5f (ELF),
#     ELF, COFF, etc). These images will be covnerted to RPRC before combined into
#     an appimage.
#
#     A finite number of image variables has been provided below, but users may
#     add as many as needed. The load-order of these images is determined in
#     $IMG_LIST below.
#
#     Each image configuration here should be formatted as follows:
# 
#         <Device ID>,<RTOS/Baremetal Binary>
#
#     NOTE: Device ID's are defined in <ti/build/makerules/platform.mk>
#
#     NOTE: images configured to run on the same device as SBL will be started
#           last. For example, on AM65xx SBL runs on MCU1_0, so any images
#           configured to run on MCU1 will be started when SBL is finished.
#
# Example:
# IMG1 ?= mcu1_0,$(RTOS_BIN_PATH)/sbl_baremetal_boot_test_am65xx_idk_mcu1_0TestApp_release.xer5f
#
IMG1 ?= 
IMG2 ?= 
IMG3 ?= 
IMG4 ?= 
IMG5 ?= 
IMG6 ?= 
IMG7 ?= 
IMG8 ?= 

#################################################################################
# Final Image Configuration
#################################################################################

# ATF is last on purpose, as it is only image that will be executed, thus it is
# important that the other HLOS images get loaded first.
ifeq ($(HLOS_BOOT),optimized)
HLOS_IMGS = $(OPTEE_IMG) $(KERNEL_IMG) $(DTB_IMG) $(ATF_IMG)
else ifeq ($(HLOS_BOOT),development)
HLOS_IMGS = $(OPTEE_IMG) $(SPL_IMG) $(ATF_IMG)
else
HLOS_IMGS =
endif

# The final list of images, listed in inteded load/boot order
# (please see note above in "RTOS/Baremetal Configuration" regarding boot order)
IMG_LIST ?= $(IMG1) $(IMG2) $(IMG3) $(IMG4) $(IMG5) $(IMG6) $(IMG7) $(IMG8) $(HLOS_IMGS)