/*----------------------------------------------------------------------------*/
/* File: linker.cmd                                                           */
/* Description:                                                               */
/*    Link command file for TPR12 Uniflash                                    */
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
    RESET_VECTORS (X) : origin=0x00000000 length=0x100
    /* RESET_VECTORS (X) : origin=0x00020000 length=0x100 */
    TCMA_RAM (RX) : origin=0x00000100 length=0x00007F00
    TCMB_RAM (RW) : origin=0x00080000 length=0x00008000
    L2_RAM (RW)   : origin=0x10200000 length=0x00060000
    L3_RAM (RW)   : origin=0x88000000 length=0x00390000
    HWA_RAM (RW)  : origin=0x82000000 length=0x00020000
}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
    .rstvectors           : {} palign(8)                            > RESET_VECTORS
    .bootCode    	      : {} palign(8)                            > L2_RAM
    .startupCode 	      : {} palign(8)                            > L2_RAM
    .startupData 	      : {} palign(8)                            > L2_RAM, type = NOINIT
    .text    	          : {} palign(8)                            > L2_RAM
    .const   	          : {} palign(8)                            > L2_RAM
    .cinit                : {} palign(8)                            > L2_RAM
    .pinit                : {} palign(8)                            > L2_RAM
    .boardcfg_data	  : {} palign(128)                          > L2_RAM

    .data                 : {} palign(128)                          > L2_RAM
    .bss     	          : {} align(4)                             > L2_RAM
    .sysmem  	          : {}                                      > L2_RAM

	.stack  	          : {} align(4)                             > L2_RAM  (HIGH)
	.irqStack  	          : {. = . + __IRQ_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
                            RUN_START(__IRQ_STACK_START)
                            RUN_END(__IRQ_STACK_END)
    .fiqStack  	          : {. = . + __FIQ_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
                            RUN_START(__FIQ_STACK_START)
                            RUN_END(__FIQ_STACK_END)
    .abortStack  	      : {. = . + __ABORT_STACK_SIZE;} align(4)  > L2_RAM  (HIGH)
                            RUN_START(__ABORT_STACK_START)
                            RUN_END(__ABORT_STACK_END)
    .undStack  	          : {. = . + __UND_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
                            RUN_START(__UND_STACK_START)
                            RUN_END(__UND_STACK_END)
    .svcStac              : {. = . + __SVC_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
                            RUN_START(__SVC_STACK_START)
                            RUN_END(__SVC_STACK_END)
}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
