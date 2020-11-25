/*----------------------------------------------------------------------------*/
/* File: linker.cmd                                                           */
/* Description:                                                               */
/*    Link command file for OTP Keywriter                                     */
/*                                                                            */
/*    Platform: R5 Cores on J721e                                             */
/* (c) Texas Instruments 2020, All rights reserved.                           */
/*----------------------------------------------------------------------------*/
--retain="*(.bootCode)"
--retain="*(.startupCode)"
--retain="*(.startupData)"
--retain="*(.intvecs)"
--retain="*(.intc_text)"
--retain="*(.rstvectors)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
--fill_value=0
--stack_size=0x2000
--heap_size=0x2000
--entry_point=_keywriterResetVectors                /* Keywriter entry in Keywriter_init.asm        */

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x800;
__UND_STACK_SIZE = 0x800;
__SVC_STACK_SIZE = 0x2000;

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY
{
    /*  Reset Vectors base address(RESET_VECTORS) should be 64 bytes aligned  */
        RESET_VECTORS (X)                          : origin=0x41C00100 length=0x100

    /* MCU0 memory used for Keywriter. Available to app for dynamic use ~160KB */
    /* RBL uses 0x41CC0000 and beyond. Keywriter, at load cannot cross this */
    OCMRAM_Keywriter    (RWIX)   : origin=0x41C00200 length=0x80000-0x200

    /* Used by Keywriter at runtime to load SYSFW. Available to app for dynamic use
     * Populate the SCISERVER Board configuration paramters at 0x41C80000
     * after SYSFW is done.
     * Location of all board configurations: 0x41c80040 after SYSFW is done.
     */
    OCMRAM_Keywriter_SYSFW (RWIX)   : origin=0x41C80000 length=0x40000

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
/* 'intvecs' and 'intc_text' sections shall be placed within                  */
/* a range of +\- 16 MB                                                       */
    .rstvectors                   : {} palign(8)     > RESET_VECTORS
    .bootCode                     : {} palign(8)     > OCMRAM_Keywriter
    .startupCode                  : {} palign(8)     > OCMRAM_Keywriter
    .startupData                  : {} palign(8)     > OCMRAM_Keywriter, type = NOINIT
    .keywriter_profile_info             : {} palign(8)     > RESET_VECTORS  (HIGH)
    .text                         : {} palign(8)     > OCMRAM_Keywriter
    .const                        : {} palign(8)     > OCMRAM_Keywriter
    .const.devgroup.MCU_WAKEUP    : {} align(4)      > OCMRAM_Keywriter
    .const.devgroup.MAIN          : {} align(4)      > OCMRAM_Keywriter
    .const.devgroup.DMSC_INTERNAL : {} align(4)      > OCMRAM_Keywriter
    .cinit                        : {} palign(8)     > OCMRAM_Keywriter
    .pinit                        : {} palign(8)     > OCMRAM_Keywriter
    .boardcfg_data                : {} palign(128)   > OCMRAM_Keywriter

    .data                         : {} palign(128)   > OCMRAM_Keywriter
    .bss                          : {} align(4)      > OCMRAM_Keywriter
    .bss.devgroup.MAIN            : {} align(4)      > OCMRAM_Keywriter
    .bss.devgroup.MCU_WAKEUP      : {} align(4)      > OCMRAM_Keywriter
    .bss.devgroup.DMSC_INTERNAL   : {} align(4)      > OCMRAM_Keywriter
    .sysmem                       : {}               > OCMRAM_Keywriter

	.keywr_bin_end				  : {}				 > OCMRAM_Keywriter  (HIGH)
        .stack                    : {} align(4)                             > OCMRAM_Keywriter  (HIGH)
        .irqStack                    : {. = . + __IRQ_STACK_SIZE;} align(4)    > OCMRAM_Keywriter  (HIGH)
                            RUN_START(__IRQ_STACK_START)
                            RUN_END(__IRQ_STACK_END)
    .fiqStack                    : {. = . + __FIQ_STACK_SIZE;} align(4)    > OCMRAM_Keywriter  (HIGH)
                            RUN_START(__FIQ_STACK_START)
                            RUN_END(__FIQ_STACK_END)
    .abortStack                : {. = . + __ABORT_STACK_SIZE;} align(4)  > OCMRAM_Keywriter  (HIGH)
                            RUN_START(__ABORT_STACK_START)
                            RUN_END(__ABORT_STACK_END)
    .undStack                    : {. = . + __UND_STACK_SIZE;} align(4)    > OCMRAM_Keywriter  (HIGH)
                            RUN_START(__UND_STACK_START)
                            RUN_END(__UND_STACK_END)
    .svcStac              : {. = . + __SVC_STACK_SIZE;} align(4)    > OCMRAM_Keywriter  (HIGH)
                            RUN_START(__SVC_STACK_START)
                            RUN_END(__SVC_STACK_END)
    .firmware             : {} palign(8)                            > OCMRAM_Keywriter_SYSFW

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
