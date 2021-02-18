#
# Driver Configuration
#

#
# RM & PM HAL Configuration
#
CONFIG_ADDR_REMAP_OFFSET=0x00000000
# end of RM & PM HAL Configuration

#
# Power Management Feature support
#
CONFIG_PM=y
CONFIG_PSC=y
CONFIG_CLOCK=y
# CONFIG_CLK_RPLL028 is not set
CONFIG_CLK_ADPLLM=y
CONFIG_CLK_PLL_16FFT=y
CONFIG_CLK_PLL_DESKEW=y
# end of Power Management Feature support

#
# Resource Manager Feature Support
#
CONFIG_RM=y
# CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS is not set
CONFIG_RM_IRQ=y
CONFIG_RM_RA=y
# CONFIG_RM_RA_DMSS_RING is not set
CONFIG_RM_RA_NAV_RING=y
CONFIG_RM_UDMAP=y
CONFIG_UDMAP_CHANNEL_BURST_SIZE=y
CONFIG_UDMAP_TX_CHANNEL_TEARDOWN_TYPE=y
CONFIG_UDMAP_UDMA=y
# CONFIG_UDMAP_BCDMA is not set
# CONFIG_UDMAP_PKTDMA is not set
CONFIG_RM_PROXY=y
# end of Resource Manager Feature Support

#
# Trace configuration for RM/PM
# This should be enabled only when debugging.
# If you enable this there is an additional size and boot time increase
#
# CONFIG_TRACE=y
# CONFIG_TRACE_BUFFER=y
# CONFIG_TRACE_UART=y

ifeq ($(SOC),$(filter $(SOC), j721e))
CONFIG_SOC_FOLDER_STRING="j721e"
endif

ifeq ($(SOC),$(filter $(SOC), j7200))
CONFIG_SOC_FOLDER_STRING="j7200"
CONFIG_CLK_PLL_16FFT_FRACF_CALIBRATION=y
endif

# SoC Specific source files
ifeq ($(SOC),$(filter $(SOC), j721e))
  SCICLIENT_SOCVER = V1
endif
ifeq ($(SOC),$(filter $(SOC), j7200))
  SCICLIENT_SOCVER = V2
endif

TARGET_SOC = $(shell echo $(CONFIG_SOC_FOLDER_STRING))

#
# Power Management Feature support
#

ifeq ($(CONFIG_PM),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_PM
endif
ifeq ($(CONFIG_PSC),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_PSC
endif
ifeq ($(CONFIG_CLOCK),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_CLOCK
endif
ifeq ($(CONFIG_CLK_RPLL028),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_CLK_RPLL028
endif
ifeq ($(CONFIG_CLK_ADPLLM),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_CLK_ADPLLM
endif
ifeq ($(CONFIG_CLK_PLL_16FFT),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_CLK_PLL_16FFT
endif
ifeq ($(CONFIG_CLK_PLL_DESKEW),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_CLK_PLL_DESKEW
endif
ifeq ($(CONFIG_CLK_PLL_16FFT_FRACF_CALIBRATION),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_CLK_PLL_16FFT_FRACF_CALIBRATION
endif

#
# Resource Manager Feature Support
#
ifeq ($(CONFIG_RM),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_RM
endif
ifeq ($(CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
endif
ifeq ($(CONFIG_RM_IRQ),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_RM_IRQ
endif
ifeq ($(CONFIG_RM_RA),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_RM_RA
endif
ifeq ($(CONFIG_RM_RA_DMSS_RING),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_RM_RA_DMSS_RING
endif
ifeq ($(CONFIG_RM_RA_NAV_RING),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_RM_RA_NAV_RING
endif
ifeq ($(CONFIG_RM_UDMAP),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_RM_UDMAP
endif
ifeq ($(CONFIG_UDMAP_CHANNEL_BURST_SIZE),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_UDMAP_CHANNEL_BURST_SIZE
endif
ifeq ($(CONFIG_UDMAP_TX_CHANNEL_TEARDOWN_TYPE),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_UDMAP_TX_CHANNEL_TEARDOWN_TYPE
endif
ifeq ($(CONFIG_UDMAP_UDMA),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_UDMAP_UDMA
endif
ifeq ($(CONFIG_UDMAP_BCDMA),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_UDMAP_BCDMA
endif
ifeq ($(CONFIG_UDMAP_PKTDMA),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_UDMAP_PKTDMA
endif
ifeq ($(CONFIG_RM_PROXY),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_RM_PROXY
endif

#
# Trace configuration for RM/PM
#
ifeq ($(CONFIG_TRACE),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_TRACE
endif
ifeq ($(CONFIG_TRACE_BUFFER),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_TRACE_BUFFER
endif
ifeq ($(CONFIG_TRACE_UART),y)
CFLAGS_LOCAL_COMMON += -DCONFIG_TRACE_UART
endif

CFLAGS_LOCAL_COMMON += -DCONFIG_DEVICE_TYPE_GP

include $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/build.inc

objtree := $(PDK_INSTALL_PATH)/ti/drv/sciclient/src
srctree := $(objtree)
srcroot := $(srctree)

$(call _recurse_inc,)

CFLAGS_LOCAL_COMMON += $(cppflags-y)
CFLAGS_LOCAL_COMMON += -I$(srctree)/rm_pm_hal/pm/soc/$(TARGET_SOC)/include
CFLAGS_LOCAL_COMMON += -I$(srctree)/rm_pm_hal/include
CFLAGS_LOCAL_COMMON += -I$(PDK_INSTALL_PATH)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)
CFLAGS_LOCAL_COMMON += -I$(PDK_INSTALL_PATH)/ti/drv/sciclient/src/priv
CFLAGS_LOCAL_COMMON += -I$(PDK_INSTALL_PATH)/ti/osal
