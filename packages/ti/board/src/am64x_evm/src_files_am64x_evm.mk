
SRCDIR += src/am64x_evm src/am64x_evm/include
INCDIR += src/am64x_evm src/am64x_evm/include

# Common source files across all platforms and cores
SRCS_COMMON += board_init.c board_lld_init.c board_clock.c board_mmr.c board_pll.c
SRCS_COMMON += board_ddr.c board_info.c board_ethernet_config.c board_pinmux.c board_serdes_cfg.c

PACKAGE_SRCS_COMMON = src/am64x_evm/src_files_am64x_evm.mk
