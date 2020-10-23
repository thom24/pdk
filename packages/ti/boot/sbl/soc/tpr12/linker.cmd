/*----------------------------------------------------------------------------*/
/* File: linker.cmd                                                           */
/* Description:                                                               */
/*    Link command file for TPR12 SBL                                         */
/*                                                                            */
/*    Platform: R5 Cores on TPR12                                             */
/* (c) Texas Instruments 2018, All rights reserved.                           */
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
--entry_point=_sblResetVectors		/* SBL entry in SBL_init.asm	*/

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x800;
__UND_STACK_SIZE = 0x800;
__SVC_STACK_SIZE = 0x2000;

/***********************************************************************************
 * This is reserved for the initialization code which is a part of the TCMA. The
 * code should setup the MPU to allow L2 execution permissions
 ***********************************************************************************/
#define SBL_INIT_CODE_SIZE          640
#define SBL_TEST_CCS_LOAD           0

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY
{
    /* IVT Table for the R5 SBL is always placed at the beginning of the TCM Memory */
    /* Initialization code which executes in the TCM memory and sets up the MPU. */
    INIT_CODE   : origin = 0x00000000, length = SBL_INIT_CODE_SIZE
    TCMA_RAM (RX) : origin=SBL_INIT_CODE_SIZE length=(0x00008000 - SBL_INIT_CODE_SIZE)
    TCMB_RAM (RW) : origin=0x00080000 length=0x00008000
    /* L2 MEMORY reserved for IVT+Initialization code: */
    L2_RESVD    : ORIGIN = 0x10200000, LENGTH = SBL_INIT_CODE_SIZE, fill = 0xBDBDBDBD
    L2_RAM_SBL (RW) : origin = (0x10200000 + SBL_INIT_CODE_SIZE) length = (0x00020000 - SBL_INIT_CODE_SIZE)
    SBL_SCRATCH   : origin=0x10220000 length=0x00040000
    L3_RAM (RW)   : origin=0x88000000 length=0x00390000
    HWA_RAM (RW)  : origin=0x82000000 length=0x00020000
}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
    /* SBL Initialization Code: This needs to be relocated from the L2 to the TCMA
     * The code is executed initially and this will setup the MPU permissions. */
    .sbl_init_code: palign(8), fill=0xabcdabcd
    {
       *(.rstvectors) /* IVT is put at the beginning of the section */
       . = align(8);
       *(.bootCode)
       . = align(8);
#if  (SBL_TEST_CCS_LOAD == 1)
    } > INIT_CODE
#else
    } load=L2_RESVD, run=INIT_CODE
#endif


    .bootCode             : {} palign(8)                            > L2_RAM_SBL
    .startupCode          : {} palign(8)                            > L2_RAM_SBL
    .startupData          : {} palign(8)                            > L2_RAM_SBL
    .sblScratch           : {} palign(8)                            > SBL_SCRATCH, type = NOINIT
    .sbl_profile_info     : {} palign(8)                            > L2_RAM_SBL
    .text    	          : {} palign(8)                            > L2_RAM_SBL
    .const   	          : {} palign(8)                            > L2_RAM_SBL
    .cinit                : {} palign(8)                            > L2_RAM_SBL
    .pinit                : {} palign(8)                            > L2_RAM_SBL
    .boardcfg_data	  : {} palign(128)                          > L2_RAM_SBL

    .data                 : {} palign(128)                          > L2_RAM_SBL
    .bss     	          : {} align(4)                             > L2_RAM_SBL
    .sysmem  	          : {}                                      > L2_RAM_SBL

	.stack  	          : {} align(4)                             > L2_RAM_SBL  (HIGH)
	.irqStack  	          : {. = . + __IRQ_STACK_SIZE;} align(4)    > L2_RAM_SBL  (HIGH)
                            RUN_START(__IRQ_STACK_START)
                            RUN_END(__IRQ_STACK_END)
    .fiqStack  	          : {. = . + __FIQ_STACK_SIZE;} align(4)    > L2_RAM_SBL  (HIGH)
                            RUN_START(__FIQ_STACK_START)
                            RUN_END(__FIQ_STACK_END)
    .abortStack  	      : {. = . + __ABORT_STACK_SIZE;} align(4)  > L2_RAM_SBL  (HIGH)
                            RUN_START(__ABORT_STACK_START)
                            RUN_END(__ABORT_STACK_END)
    .undStack  	          : {. = . + __UND_STACK_SIZE;} align(4)    > L2_RAM_SBL  (HIGH)
                            RUN_START(__UND_STACK_START)
                            RUN_END(__UND_STACK_END)
    .svcStac              : {. = . + __SVC_STACK_SIZE;} align(4)    > L2_RAM_SBL  (HIGH)
                            RUN_START(__SVC_STACK_START)
                            RUN_END(__SVC_STACK_END)
}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
