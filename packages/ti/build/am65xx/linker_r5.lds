
/* This is standard linker options used by examples applications and tests    */
/* Please refer user guide that came with this release for more details       */
/* on which sections could be used at load time and runtime                   */


/* Linker Settings */
/* Standard linker options */
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
--heap_size=0x1000
--entry_point=_resetvectors /* Default C RTS boot.asm */

-stack  0x2000              /* SOFTWARE STACK SIZE */
-heap   0x2000              /* HEAP AREA SIZE */

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x1000;
__UND_STACK_SIZE = 0x1000;
__SVC_STACK_SIZE = 0x1000;



/*----------------------------------------------------------------------------*/
/* Memory Sections                                                            */
MEMORY
{
    /* Refer the user guide for details on persistence of these sections */
    /* Also, when these memories can be used by apps */
    MCU0_ATCM_NOT_USED          (R)     : origin=0x0        length=0x7FFF
    MCU0_BTCM_NOT_USED          (R)     : origin=0x41010000 length=0x7FFF
    /* Used by SBL, can be used after APPs is started */
    MCU_MSRAM_RSVD_UNUSED       (R)     : origin=0x41C00000 length=0x200
    MCU_MSRAM_RSVD_SBL          (RWIX)  : origin=0x41C00200 length=0x3DE00

    VECTORS                     (RWIX)  : origin=0x41C3E000 length=0x100
    RESET_VECTORS               (RWIX)  : origin=0x41C3E100 length=0x1000
    OCMRAM                      (RWIX)  : origin=0x41C3F100 length=0x40F00

    /* COMPUTE_CLUSTER0_MSMC_SRAM */
    MSMC3                       (RWIX)  : origin=0x70000000 length=0x1EFC00
    /* The origin and length is determined by board cfg, */
    /* refer user guide for details */
    MSMC3_RSVD_DMSC             (RWIX)  : origin=0x701F0000 length=0x1000

    DDR0                        (RWIX)  : origin=0x80000000 length=0x7FFFFFE4

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
    .intvecs        : {} palign(8) > VECTORS
    .intc_text      : {} palign(8) > VECTORS
    .rstvectors     : {} palign(8) > RESET_VECTORS
    .bootCode       : {} palign(8) > MSMC3
    .startupCode    : {} palign(8) > MSMC3
    .startupData    : {} palign(8) > MSMC3, type = NOINIT


    .text    : {} palign(8) > MSMC3
    .const   : {} palign(8) > MSMC3
    .rodata  : {} palign(8) > MSMC3
    .cinit   : {} palign(8) > MSMC3
    .pinit   : {} palign(8) > MSMC3

    /* For NDK packet memory, we need to map this sections before .bss*/
    .bss:NDK_MMBUFFER  (NOLOAD) {} ALIGN (128) > DDR0
    .bss:NDK_PACKETMEM (NOLOAD) {} ALIGN (128) > DDR0

    .bss    : {} align(8)   > MSMC3
    .far    : {} align(8)   > DDR0
    .data   : {} palign(128)> MSMC3
    .boardcfg_data : {} palign(128) > MSMC3
    .sysmem : {} align(8)   > MSMC3

    /* USB ram disk dev-msc example */
    .bss:extMemCache:ramdisk : {} align (32)    > DDR0

    /* SA sections */
    .scBufs         : {} align(8)   > DDR0
    .saSrcBuffers   : {} align(8)   > DDR0
    .saDstBuffers   : {} align(8)   > DDR0

    /* LLD buffer for benchmarking */
    .benchmark_buffer (NOLOAD) {} ALIGN (8) > DDR0

    .stack      : {} align(8)                           > MSMC3  (HIGH)
    .irqStack   : {. = . + __IRQ_STACK_SIZE;} align(8)  > MSMC3  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)

    .fiqStack  	: {. = . + __FIQ_STACK_SIZE;} align(8)  > MSMC3  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)

    .abortStack  	: {. = . + __ABORT_STACK_SIZE;} align(8) > MSMC3  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)

    .undStack  	: {. = . + __UND_STACK_SIZE;} align(8)      > MSMC3  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)

    .svcStack  	: {. = . + __SVC_STACK_SIZE;} align(8)      > MSMC3  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)


    /* Additional sections settings */

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
