
SRCDIR += src/awr294x_evm src/awr294x_evm/include
INCDIR += src/awr294x_evm src/awr294x_evm/include

# Common source files across all platforms and cores
SRCS_COMMON += board_init.c board_mmr.c board_pinmux.c board_pll.c board_clock.c board_utils.c board_info.c board_lld_init.c board_ethernet_config.c

#Default Variant is AWR294X_LOP
BOARD_VARIANT ?= AWR294X_LOP

ifeq ($(BOARD_VARIANT),)
SRCS_COMMON += AWR294X_pinmux_data_lop.c
endif

ifeq ($(BOARD_VARIANT), AWR294X_ETS)
SRCS_COMMON += AWR294X_pinmux_data.c
endif

ifeq ($(BOARD_VARIANT),AWR294X_LOP)
SRCS_COMMON += AWR294X_pinmux_data_lop.c
endif

ifeq ($(BOARD_VARIANT),AWR294X_LOP_CZ)
SRCS_COMMON += AWR294X_pinmux_data_lopCZ.c
endif