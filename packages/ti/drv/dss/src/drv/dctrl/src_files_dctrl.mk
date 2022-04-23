SRCS_COMMON += dss_dctrlGraph.c dss_dctrlApi.c

ifeq ($(SOC),$(filter $(SOC), j721e))
SRCS_COMMON += dss_dctrlExtended.c  dss_dctrlDsi.c
endif

ifeq ($(SOC),$(filter $(SOC), j721s2))
SRCS_COMMON += dss_dctrlExtended.c  dss_dctrlDsi.c
endif