
SRCDIR = . src
INCDIR = . src

# Common source files across all platforms and cores
SRCS_COMMON += pcie.c pcieinit.c 

ifeq ($(SOC),$(filter $(SOC), am65xx))
SRCDIR += src/v2
INCDIR += src/v2
SRCS_COMMON += pciev2.c pciev2_app.c pciev2_plconf.c pciev2_ep.c pciev2_rc.c pciev2_cfg.c
else
ifeq ($(SOC),$(filter $(SOC), j721e))
SRCDIR += src/v3
INCDIR += src/v3
SRCS_COMMON += pciev3.c pciev3_app.c pciev3_plconf.c pciev3_ep.c pciev3_rc.c pciev3_cfg.c
else
SRCDIR+= src/v1 src/v0
INCDIR+= src/v1 src/v0
SRCS_COMMON += pciev0.c pciev0_app.c pciev0_cfg.c pciev1.c pciev1_ticonf.c pciev1_plconf.c pciev1_ep.c pciev1_rc.c pciev1_cfg.c
endif
endif

PACKAGE_SRCS_COMMON = makefile pcie.h pcie_component.mk src/PCIE_osal.h \
                      docs/ReleaseNotes_PCIE_LLD.pdf \
                      src/ src/pcieloc.h \
              src/v0 src/v0/pcieloc.h src/v0/pcie.h \
               src/v1 src/v1/pcieloc.h src/v1/pcie.h \
                      build/makefile.mk build/makefile_profile.mk build/makefile_indp.mk build/makefile_profile_indp.mk src/src_files_common.mk
