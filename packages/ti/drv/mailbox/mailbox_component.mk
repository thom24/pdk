# File: mailbox_component.mk
#       This file is component include makefile of Mailbox driver library.
# List of variables set in this file and their purpose:
# <mod>_RELPATH        - This is the relative path of the module, typically from
#                        top-level directory of the package
# <mod>_PATH           - This is the absolute path of the module. It derives from
#                        absolute path of the top-level directory (set in env.mk)
#                        and relative path set above
# <mod>_INCLUDE        - This is the path that has interface header files of the
#                        module. This can be multiple directories (space separated)
# <mod>_PKG_LIST       - Names of the modules (and sub-modules) that are a part
#                        part of this module, including itself.
# <mod>_BOARD_DEPENDENCY - "yes": means the code for this module depends on
#                             platform and the compiled obj/lib has to be kept
#                             under <platform> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no platform dependent code and hence
#                             the obj/libs are not kept under <platform> dir.
# <mod>_CORE_DEPENDENCY     - "yes": means the code for this module depends on
#                             core and the compiled obj/lib has to be kept
#                             under <core> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no core dependent code and hence
#                             the obj/libs are not kept under <core> dir.
# <mod>_APP_STAGE_FILES     - List of source files that belongs to the module
#                             <mod>, but that needs to be compiled at application
#                             build stage (in the context of the app). This is
#                             primarily for link time configurations or if the
#                             source file is dependent on options/defines that are
#                             application dependent. This can be left blank or
#                             not defined at all, in which case, it means there
#                             no source files in the module <mod> that are required
#                             to be compiled in the application build stage.
#
ifeq ($(mailbox_component_make_include), )

drvmailbox_SOCLIST          = tpr12
drvmailbox_tpr12_CORELIST   = mcu1_0 c66xdsp_1
drvmailbox_BOARDLIST        = tpr12_evm

############################
# mailbox package
# List of components included under mailbox lib
# The components included here are built and will be part of mailbox lib
############################
mailbox_LIB_LIST = mailbox

############################
# mailbox examples
# List of examples under mailbox (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
mailbox_EXAMPLE_LIST =

#
# Mailbox Modules
#

# Mailbox library
export mailbox_COMP_LIST = mailbox
mailbox_RELPATH = ti/drv/mailbox
mailbox_PATH = $(PDK_MAILBOX_COMP_PATH)
export mailbox_LIBNAME = mailbox
export mailbox_LIBPATH = $(PDK_MAILBOX_COMP_PATH)/lib
export mailbox_MAKEFILE = -fsrc/makefile
export mailbox_BOARD_DEPENDENCY = no
export mailbox_CORE_DEPENDENCY = yes
mailbox_PKG_LIST = mailbox
mailbox_INCLUDE = $(mailbox_PATH)
export mailbox_SOCLIST = $(drvmailbox_SOCLIST)
export mailbox_$(SOC)_CORELIST = $(drvmailbox_$(SOC)_CORELIST)

#
# Mailbox Examples
#

# mailbox msg test app
export mailbox_msg_testapp_COMP_LIST = mailbox_msg_testapp
mailbox_msg_testapp_RELPATH = ti/drv/mailbox/examples/mailbox_msg_testapp
mailbox_msg_testapp_PATH = $(PDK_MAILBOX_COMP_PATH)/examples/mailbox_msg_testapp
export mailbox_msg_testapp_BOARD_DEPENDENCY = yes
export mailbox_msg_testapp_CORE_DEPENDENCY = yes
export mailbox_msg_testapp_XDC_CONFIGURO = yes
mailbox_msg_testapp_PKG_LIST = mailbox_msg_testapp
mailbox_msg_testapp_INCLUDE = $(mailbox_msg_testapp_PATH)
export mailbox_msg_testapp_BOARDLIST = $(drvmailbox_BOARDLIST)
export mailbox_msg_testapp_$(SOC)_CORELIST = $(drvmailbox_$(SOC)_CORELIST)
mailbox_EXAMPLE_LIST += mailbox_msg_testapp


export mailbox_LIB_LIST
export mailbox_EXAMPLE_LIST

MAILBOX_CFLAGS =
export MAILBOX_CFLAGS

mailbox_component_make_include := 1
endif
