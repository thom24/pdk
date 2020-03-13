#
# This file is the makefile for building sciclient .
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = sciclient

SRCDIR = src
INCDIR = soc/sysfw/include
PACKAGE_SRCS_COMMON =

# SoC Specific source files
ifeq ($(SOC),$(filter $(SOC), am65xx))
  SCICLIENT_SOCVER = V0
endif
ifeq ($(SOC),$(filter $(SOC), j721e j7200))
  SCICLIENT_SOCVER = V1
endif

SRCDIR += soc/$(SCICLIENT_SOCVER)
INCDIR += soc/$(SCICLIENT_SOCVER)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk osal

# Common source files and CFLAGS across all platforms and cores
ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200))
# Keep the sciclient.c the first file in this list. This is used to trigger
# the generation of the header files from the firware binary file.
SRCS_COMMON  = sciclient.c
SRCS_COMMON += sciclient_fmwSecureProxyMap.c
SRCS_COMMON += sciclient_pm.c
SRCS_COMMON += sciclient_rm.c 
SRCS_COMMON += sciclient_boardcfg.c
SRCS_COMMON += sciclient_procboot.c
SRCS_COMMON += sciclient_genericMsgs.c
endif

#Use CSL implementation to bypass DMSC
# ifeq ($(SOC),$(filter $(SOC), j721e j7200))
# SRCS_COMMON = sciclient_rm_csl.c sciclient_dummy.c
# endif

#Bypass DMSC for host emulation and Loki build
ifeq ($(BOARD),$(filter $(BOARD), j721e_hostemu j721e_loki))
 SRCS_COMMON = sciclient_rm_csl.c sciclient_dummy.c
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
PACKAGE_SRCS_COMMON += sciclient.h sciclient_component.mk makefile .gitignore
PACKAGE_SRCS_COMMON += include docs tools $(SRCDIR) soc/sciclient_soc_priv.h
PACKAGE_SRCS_COMMON += config_mk.bld package.bld package.xdc package.xs
PACKAGE_SRCS_COMMON += Settings.xdc.xdt sciclientver.h sciclientver.h.xdt

# Selectively package sysfw components
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/system-firmware-public-documentation
PACKAGE_SRCS_COMMON += soc/sysfw/include

ifeq ($(SOC),$(filter $(SOC), am65xx))
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/ti-sci-firmware-am65x-gp.bin
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/ti-sci-firmware-am65x_sr2-gp.bin
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/am65x_sr2

ifeq ($(BUILD_HS),yes)
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/ti-sci-firmware-am65x-hs-cert.bin
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/ti-sci-firmware-am65x-hs-enc.bin
CFLAGS_LOCAL_COMMON += -DBUILD_HS
endif

endif

ifeq ($(SOC),$(filter $(SOC), j721e))
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/ti-sci-firmware-j721e-gp.bin
endif


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

# Special case for Windows Build
ifeq ($(OS),Windows_NT)
  EXE_EXT=.exe
endif

# ObjCopy Tool
ifneq ("$(wildcard $(TOOLCHAIN_PATH_GCC)/bin/arm-none-eabi-objcopy$(EXE_EXT))","")
  SCICLIENT_OBJCOPY?=$(TOOLCHAIN_PATH_GCC)/bin/arm-none-eabi-objcopy$(EXE_EXT)
endif
ifneq ("$(wildcard $(TOOLCHAIN_PATH_GCC_ARCH64)/bin/aarch64-elf-objcopy$(EXE_EXT))","")
  SCICLIENT_OBJCOPY?=$(TOOLCHAIN_PATH_GCC_ARCH64)/bin/aarch64-elf-objcopy$(EXE_EXT)
endif

# Other SBL Tools
ifneq ("$(wildcard $(PDK_SBL_AUTO_COMP_PATH)/tools)","")
  SBL_TOOLS_PATH=$(PDK_SBL_AUTO_COMP_PATH)/tools
else
  SBL_TOOLS_PATH=$(PDK_SBL_COMP_PATH)/tools
endif
export SCICLIENT_OBJCOPY

# Certificate Generation script
ifeq ($(OS),Windows_NT)
  SCICLIENT_CERT_GEN=powershell -executionpolicy unrestricted -command $(ROOTDIR)/ti/build/makerules/x509CertificateGen.ps1
  BIN2C_EXE=bin2c.exe
else
  SCICLIENT_CERT_GEN=$(ROOTDIR)/ti/build/makerules/x509CertificateGen.sh
  BIN2C_EXE=bin2c.out
endif

# Bin to C header file tools
BIN2C_GEN=$(ROOTDIR)/ti/drv/sciclient/tools/bin2c/$(BIN2C_EXE)

# Certificate Keys
ifeq ($(BUILD_HS),yes)
SCICLIENT_CERT_KEY=$(ROOTDIR)/ti/build/makerules/k3_dev_mpk.pem
else
SCICLIENT_CERT_KEY=$(ROOTDIR)/ti/build/makerules/rom_degenerateKey.pem
endif

# For matching cfg files (purposely truncated to match main cfg file)
BRD_CFG := sciclient_defaultBoard
CFG_DIR := $(ROOTDIR)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)

# These will be used to distinguish between an HS configuration and
# .. a GP configuration in the rules below.
ifeq ($(BUILD_HS),yes)
SGN_EXT := .hs.signed
HEX_EXT := _hexhs.h
else
SGN_EXT := .signed
HEX_EXT := _hex.h
endif

# We want to create an include-able header file with a hex representation of
# .. each board configuration (signed according to SoC). Because each Board
# .. Configuration will require one of these header files, we can simply parse
# .. through the appropriate SOC directory and generate one of these hex header
# .. files for each Board Configuration file.
HEXFILES := $(foreach file, \
		$(wildcard $(CFG_DIR)/$(BRD_CFG)*.c), \
		$(subst .c,$(HEX_EXT),$(file)))

# Append the proper hex-file targets to "all" to guarantee they are
# .. generated if missing/a board configuration file has been changed
all: $(HEXFILES)

# Build the Board Configuration object files. A modified Configuration file
# .. will trigger this chain for that particular configuration
$(OBJDIR)/$(BRD_CFG)%.$(OBJEXT) : soc/$(SCICLIENT_SOCVER)/$(BRD_CFG)%.c
ifeq ($(CORE), mcu1_0)
	@echo "Build Board Configuration Obj Files"
	$(MKDIR) -p $(OBJDIR)
	$(CC) $(_CFLAGS) $(INCLUDES) $(CFLAGS_DIROPTS) -fc $<
endif

# Copy the raw binary into an intermediate file (will get deleted by make)
$(OBJDIR)/$(BRD_CFG)%.bin.unsigned : $(OBJDIR)/$(BRD_CFG)%.$(OBJEXT)
ifeq ($(CORE), mcu1_0)
	@echo "Create unsigned Board Configuration binary"
	$(SCICLIENT_OBJCOPY) -S -O binary $< $@
endif

# If an HS signature is required, sign it with the HS key (will get deleted by make)
$(OBJDIR)/$(BRD_CFG)%.bin.hs.signed : $(OBJDIR)/$(BRD_CFG)%.bin.unsigned
ifeq ($(CORE), mcu1_0)
	@echo "Sign Board Configuration with HS key"
ifneq ($(OS),Windows_NT)
	$(CHMOD) a+x $(SCICLIENT_CERT_GEN)
endif
	$(SCICLIENT_CERT_GEN) -b $< -o $@ -k $(SCICLIENT_CERT_KEY) -f 1
endif

# If a GP signature is required, sign it with the GP key (will get deleted by make)
$(OBJDIR)/$(BRD_CFG)%.bin.signed : $(OBJDIR)/$(BRD_CFG)%.bin.unsigned
ifeq ($(CORE), mcu1_0)
	@echo "Sign Board Configuration with GP key"
	@echo "  (note: GP board configuration not ACTUALLY signed with anything)"
	$(CP) $< $@
endif

# Generate the actual header file from the signed Board Configuration binary.
# This rule determines the type of signature required in the previous rules.
$(CFG_DIR)/%$(HEX_EXT) : $(OBJDIR)/%.bin$(SGN_EXT)
ifeq ($(CORE), mcu1_0)
	@echo "Generate include-able header file from signed Board Configuration"
ifneq ($(OS),Windows_NT)
	$(CHMOD) a+x $(BIN2C_GEN)
endif
	$(BIN2C_GEN) $< $*$(HEX_EXT) $(subst default,,$*) > $@
endif


# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
