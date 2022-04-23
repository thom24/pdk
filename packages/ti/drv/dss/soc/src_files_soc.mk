SRCS_COMMON += dss_soc.c dss_soc_graph.c

ifeq ($(SOC),$(filter $(SOC), j721e))
SRCS_COMMON += dss_soc_fw.c
endif

ifeq ($(SOC),$(filter $(SOC), j721s2))
SRCS_COMMON += dss_soc_fw.c
endif