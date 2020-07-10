###################################################################################
# R4 Testbench Build file
###################################################################################

###################################################################################
# Tool Definitions:
###################################################################################
R4_CC        = $(R4_CODEGEN_INSTALL_PATH)/bin/armcl
R4_LD        = $(R4_CODEGEN_INSTALL_PATH)/bin/armcl
R4_AR        = $(R4_CODEGEN_INSTALL_PATH)/bin/armar
R4_HEX       = $(R4_CODEGEN_INSTALL_PATH)/bin/armhex
R4_DIS       = $(R4_CODEGEN_INSTALL_PATH)/bin/armdis

OBJDIR       = obj

###################################################################################
# OS specific definitions:
###################################################################################
ifeq ($(OS),Windows_NT)
DEL      = $(XDC_INSTALL_PATH)/bin/rm -Rf
COPY_CMD = $(XDC_INSTALL_PATH)/bin/cp -f
MKDIR    = $(XDC_INSTALL_PATH)/bin/mkdir
MAKE     = gmake
GREP     = grep
SED      = sed
else
DEL      = rm -Rf
COPY_CMD = cp -f
MKDIR    = mkdir
MAKE     = make
GREP     = grep
SED      = sed
endif
###################################################################################
# R4 Build Options:
###################################################################################

# File extension to use for R4 library file
R4_LIB_EXT = aer4

# File extension to use for R4 Objects
R4_OBJ_EXT = oer4

# File extension to use for R4 Executable
R4_EXE_EXT = xer4

# File extension to use for R4 Dependency
R4_DEP_EXT = dr4

# Include paths for the R4 Builds:
R4_INCLUDE = -i$(R4_TB_PATH) -i$(R4_TB_PATH)/include -i$(R4_CODEGEN_INSTALL_PATH)/include

# Preprocessor definitions
R4_DEFINE = --define=_LITTLE_ENDIAN					\

# Compiler flags used for the R4 Builds:
R4_CFLAGS  = -mv7R4 --code_state=32 --abi=eabi -me --float_support=VFPv3D16          	\
             $(R4_DEFINE) $(R4_INCLUDE)                         						\
              -g -display_error_number --diag_warning=225 --diag_wrap=off 			\
              --little_endian --preproc_with_compile --gen_func_subsections 			\
			  --emit_warnings_as_errors

# RTS Library used from the compiler: This is needed for buidling the unit tests
R4_LD_RTS_FLAGS = -l"rtsv7R4_T_le_v3D16_eabi.lib"

# Linker flags used for the R4 Builds:
R4_LDFLAGS = -mv7R4 --code_state=32 --abi=eabi -me -g  --float_support=VFPv3D16 		\
              --display_error_number --diag_warning=225 --diag_wrap=off -z              \
              --reread_libs --warn_sections --rom_model --unused_section_elimination	\
	      --emit_warnings_as_errors	-i$(R4_CODEGEN_INSTALL_PATH)/lib --cinit_compression=off

# Archiver options:
R4_AR_OPTS = r

#HFLAGS
R4_HFLAGS  = -romwidth 32 -memwidth 8 -i

#DATFLAGS
R4_DATFLAGS = -b --image

###################################################################################
# Build the object directory
###################################################################################
buildDirectories:
	@$(MKDIR) -p $(OBJDIR)

###################################################################################
# Build Suffix:
###################################################################################
$(OBJDIR)/%.$(R4_OBJ_EXT): %.c
	@echo '[R4] Building file: $<'
	$(R4_CC) -c $(R4_CFLAGS) -ppd=$(OBJDIR)/"$(basename $(<F)).$(R4_DEP_EXT)"  $< --output_file $@

$(OBJDIR)/%.$(R4_OBJ_EXT): %.asm
	@echo '[R4] Building file: $<'
	@$(R4_CC) -c $(R4_CFLAGS)  --asm_dependency=$(OBJDIR)/"$(basename $(<F)).$(R4_DEP_EXT)" $< --output_file $@
	@$(R4_CC) -c $(R4_CFLAGS) $< --output_file $@

