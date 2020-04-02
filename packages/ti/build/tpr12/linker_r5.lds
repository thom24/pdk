/*----------------------------------------------------------------------------*/
/* r5f_linker.cmd                                                             */
/*                                                                            */
/* (c) Texas Instruments 2020, All rights reserved.                           */
/*                                                                            */

/* USER CODE BEGIN (0) */
/* USER CODE END */
--retain="*(.intvecs)"
--retain="*(.intc_text)"
--retain="*(.rstvectors)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */


/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x1000;
__UND_STACK_SIZE = 0x1000;
__SVC_STACK_SIZE = 0x1000;

/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY{
PAGE 0:
    VECTORS  (X)  : origin=0x102EF000 length=0x00001000
    /*  Reset Vectors base address(RESET_VECTORS) should be 64 bytes aligned  */
    RESET_VECTORS (X) : origin=0x00000000 length=0x100
    /* RESET_VECTORS (X) : origin=0x00020000 length=0x100 */
    TCMA_RAM (RX) : origin=0x00000100 length=0x00007F00
    TCMB_RAM (RW) : origin=0x00080000 length=0x00008000
    L2_RAM_BANK0 (RW) : origin=0x10200000 length=0x00080000
    L2_RAM_BANK1 (RW) : origin=0x10280000 length=0x0006F000
    L3_RAM (RW)   : origin=0x88000000 length=0x00300000
    HWA_RAM (RW)  : origin=0x28000000 length=0x00020000

PAGE 1:
    L3_RAM (RW)   : origin=0x88000000 length=0x00300000
}

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS{
    .intvecs       : {} palign(8)      > VECTORS
    .intc_text     : {} palign(8)      > VECTORS
    .rstvectors    : {} palign(8)      > RESET_VECTORS
    .bootCode      : {} palign(8)      > TCMA_RAM
    .startupCode   : {} palign(8)      > TCMA_RAM
    .startupData   : {} palign(8)      > TCMB_RAM, type = NOINIT

    /* Allocate data preferentially in one bank and code (.text) in another,
       this can improve performance due to simultaneous misses from L1P
       and L1D caches to L2 SRAM.
       The linker notation "X >> Y | Z" indicates section X is first allocated in Y
       and allowed to overflow into Z and can be split from Y to Z.
       The linker notation "X > Y | Z" indicates section X is first allocated in Y
       and allowed to overflow into Z and cannot be split from Y to Z. Some sections
       like bss are not allowed to be split so > notation is used for them
    */
    .text    : {} >> TCMA_RAM | L2_RAM_BANK0 | L2_RAM_BANK1

    .const   : {} >> L2_RAM_BANK1 | L2_RAM_BANK0
    .switch  : {} >> L2_RAM_BANK1 | L2_RAM_BANK0
    .cio:    : {} >> L2_RAM_BANK1 | L2_RAM_BANK0
    .data:   : {} >> L2_RAM_BANK1 | L2_RAM_BANK0

    .cinit   : {} > L2_RAM_BANK1 | L2_RAM_BANK0
    .pinit   : {} > L2_RAM_BANK1 | L2_RAM_BANK0
    .bss     : {} > L2_RAM_BANK1 | L2_RAM_BANK0
    .stack   : {} > TCMB_RAM | L2_RAM_BANK1 | L2_RAM_BANK0
    .sysmem  : {} > L2_RAM_BANK1 | L2_RAM_BANK0
    .irqStack  : {. = . + __IRQ_STACK_SIZE;} align(4)    > L2_RAM_BANK1  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
    .fiqStack  : {. = . + __FIQ_STACK_SIZE;} align(4)    > L2_RAM_BANK1  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
    .abortStack  : {. = . + __ABORT_STACK_SIZE;} align(4)> L2_RAM_BANK1  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
    .undStack  : {. = . + __UND_STACK_SIZE;} align(4)    > L2_RAM_BANK1  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)
    .svcStack  : {. = . + __SVC_STACK_SIZE;} align(4)    > L2_RAM_BANK1  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}
/*----------------------------------------------------------------------------*/

