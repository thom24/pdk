/*----------------------------------------------------------------------------*/
/* File: k3m4_r5f_linker.cmd                                                  */
/* Description:                                                               */
/*    Link command file for Maxwell M4 MCU 0 view                             */
/*      TI ARM Compiler version 15.12.3 LTS or later                          */
/*                                                                            */
/*    Platform: QT                                                            */
/* (c) Texas Instruments 2017, All rights reserved.                           */
/*----------------------------------------------------------------------------*/
/*  History:                                                                  *'
/*    Aug 26th, 2016 Original version .......................... Loc Truong   */
/*    Aug 01th, 2017 new TCM mem map  .......................... Loc Truong   */
/*    Nov 07th, 2017 Changes for R5F Init Code.................. Vivek Dhande */
/*    Feb 07th, 2017 Copied and changed for sciclient to build.. Piyali G.    */
/*    Mar 26th, 2019 Changes for R5F startup Code............... Vivek Dhande */
/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
/* Standard linker options                                                    */
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

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x100;
__FIQ_STACK_SIZE = 0x100;
__ABORT_STACK_SIZE = 0x100;
__UND_STACK_SIZE = 0x100;
__SVC_STACK_SIZE = 0x100;


/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY
{
    /* MCU0_R5F_0 local view                                                  */
    MCU0_R5F_TCMA (X)     : origin=0x0           length=0x8000
    MCU0_R5F_TCMB0 (RWIX) : origin=0x41010000    length=0x8000

    /*  Reset Vectors base address(RESET_VECTORS) should be 64 bytes aligned  */
    RESET_VECTORS (X)           : origin=0x41C00100 length=0x100

    /* MCU0 memory used for sciclientTest. Available to app for dynamic use ~160KB */
    /* RBL uses 0x41C58000 and beyond. sciclientTest, at load cannot cross this */
    OCMRAM_sciclientTest    (RWIX)   : origin=0x41C00200 length=0x50000-0x200

    /* Used by sciclientTest at runtime to load SYSFW. Available to app for dynamic use */
    OCMRAM_sciclientTest_SYSFW (RWIX)   : origin=0x41C50000 length=0x30000
    
    /* Populate the SCISERVER Board configuration paramters */
    OCMC_RAM_BOARD_CFG_TABLE (RWIX) : ORIGIN = 0x41c80000 , LENGTH = 0x40
    /* Location of all board configurations */
    OCMC_RAM_BOARD_CFG (RWIX)       : ORIGIN = 0x41c80040 , LENGTH = 0x2000 - 0x40
    /* Space where Sciserver App will run */
    OCMC_RAM_SCISERVER (RWIX)       : ORIGIN = 0x41C82000 , LENGTH = 0x60000
    /* X509 Header to be left behind by CCS Init for Sciserver to read */
    OCMC_RAM_X509_HEADER1 (RWIX)     : ORIGIN = 0x41cffb00 , LENGTH = 0x500 /* covers header for J7200/J721E */
    OCMC_RAM_X509_HEADER2 (RWIX)     : ORIGIN = 0x41cfdb00 , LENGTH = 0x500 /* covers header for J721S2 */

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
/* 'intvecs' and 'intc_text' sections shall be placed within                  */
/* a range of +\- 16 MB                                                       */
    .rstvectors                   : {} palign(8)     > RESET_VECTORS
    .boardcfg_data                : {} palign(8)     > OCMRAM_sciclientTest
    .bootCode                     : {} palign(8)     > OCMRAM_sciclientTest
    .startupCode                  : {} palign(8)     > OCMRAM_sciclientTest
    .startupData                  : {} palign(8)     > OCMRAM_sciclientTest, type = NOINIT
    .text                         : {} palign(8)     > OCMRAM_sciclientTest
    .const                        : {} palign(8)     > OCMRAM_sciclientTest
    .rodata                       : {} palign(8)     > OCMRAM_sciclientTest
    .const.devgroup.MCU_WAKEUP    : {} align(4)      > OCMRAM_sciclientTest
    .const.devgroup.MAIN          : {} align(4)      > OCMRAM_sciclientTest
    .const.devgroup.DMSC_INTERNAL : {} align(4)      > OCMRAM_sciclientTest
    .cinit                        : {} palign(8)     > OCMRAM_sciclientTest
    .pinit                        : {} palign(8)     > OCMRAM_sciclientTest
    .data                         : {} palign(128)   > OCMRAM_sciclientTest
    .bss                          : {} align(4)      > OCMRAM_sciclientTest
    .bss.devgroup.MAIN            : {} align(4)      > OCMRAM_sciclientTest
    .bss.devgroup.MCU_WAKEUP      : {} align(4)      > OCMRAM_sciclientTest
    .bss.devgroup.DMSC_INTERNAL   : {} align(4)      > OCMRAM_sciclientTest
    .sysmem                       : {}               > OCMRAM_sciclientTest
    .stack                        : {} align(4)      > OCMRAM_sciclientTest  (HIGH)
    .irqStack                     : {. = . + __IRQ_STACK_SIZE;} align(4)    > OCMRAM_sciclientTest  (HIGH)
                            RUN_START(__IRQ_STACK_START)
                            RUN_END(__IRQ_STACK_END)
    .fiqStack             : {. = . + __FIQ_STACK_SIZE;} align(4)    > OCMRAM_sciclientTest  (HIGH)
                            RUN_START(__FIQ_STACK_START)
                            RUN_END(__FIQ_STACK_END)
    .abortStack           : {. = . + __ABORT_STACK_SIZE;} align(4)  > OCMRAM_sciclientTest  (HIGH)
                            RUN_START(__ABORT_STACK_START)
                            RUN_END(__ABORT_STACK_END)
    .undStack             : {. = . + __UND_STACK_SIZE;} align(4)    > OCMRAM_sciclientTest  (HIGH)
                            RUN_START(__UND_STACK_START)
                            RUN_END(__UND_STACK_END)
    .svcStac              : {. = . + __SVC_STACK_SIZE;} align(4)    > OCMRAM_sciclientTest  (HIGH)
                            RUN_START(__SVC_STACK_START)
                            RUN_END(__SVC_STACK_END)
    .firmware             : {} palign(8)                            > OCMRAM_sciclientTest_SYSFW

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
