/* Linker Settings */
--retain="*(.rstvectors)"
--retain="*(.bootCode)"
--retain="*(.startupCode)"
--retain="*(.startupData)"
--retain="*(.intvecs)"
--retain="*(.intc_text)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
--fill_value=0
--entry_point=_safeRTOSresetvectors     /* C RTS boot.asm with added SVC handler	*/
/* --entry_point=_resetvectors          Default C RTS boot.asm	*/

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x1000;
__UND_STACK_SIZE = 0x1000;
__SVC_STACK_SIZE = 0x1000;


#define MCU0_R5F_TCMA_START         ( 0x0 )
#define MCU0_R5F_TCMA_SIZE          ( 0x8000 )
#define MCU0_R5F_TCMA_SBL_RSVD_SIZE ( 0x100 )

#define MCU0_R5F_TCMB0_START        ( 0x41010000 )
#define MCU0_R5F_TCMB0_SIZE         ( 0x8000 )

#define MCU0_R5F1_ATCM_START        ( 0x41400000 )
#define MCU0_R5F1_ATCM_SIZE         ( 0x8000 )

#define MCU0_R5F1_BTCM_START        ( 0x41410000 )
#define MCU0_R5F1_BTCM_SIZE         ( 0x8000 )

#define MCU0_R5F1_BTCM_START        ( 0x41410000 )
#define MCU0_R5F1_BTCM_SIZE         ( 0x8000 )

#define OCMC_RAM_BOARD_CFG_START    ( 0x41C80000 )
#define OCMC_RAM_BOARD_CFG_SIZE     ( 0x2000 )

#define OCMC_RAM_SCISERVER_START    ( 0x41C82000 )
#define OCMC_RAM_SCISERVER_SIZE     ( 0x60000 )

#define VECTORS_START               ( 0x41CE2000 )
#define VECTORS_SIZE                ( 0x1000 )

#define RESET_VECTORS_START         ( 0x41CE3000 )
#define RESET_VECTORS_SIZE          ( 0x100 )

#define OCMC_RAM_START              ( 0x41CE3100 )
#define OCMC_RAM_SIZE               ( 0x1CA00 )

#define OCMC_RAM_X509_HEADER_START  ( 0x41CFFB00 )
#define OCMC_RAM_X509_HEADER_SIZE   ( 0x500 )

#define MSMC3_ARM_FW_START          ( 0x70000000 )
#define MSMC3_ARM_FW_SIZE           ( 0x40000 )         /* 256KB */

#define MSMC3_START                 ( 0x70040000 )
#define MSMC3_SIZE                  ( 0x7B0000 )        /* 8MB - 320KB */
#define MSMC3_RO_SIZE               ( 0x40000 )

#define MSMC3_DMSC_FW_START         ( 0x707F0000 )         /* 64KB */
#define MSMC3_DMSC_FW_SIZE          ( 0x10000 )

#define DDR0_START                  ( 0x80000000 )
#define DDR0_SIZE                   ( 0x80000000 )      /* 2GB */

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY
{
    /* MCU0_R5F_0 local view */
    MCU0_R5F_TCMA_SBL_RSVD  (X) : origin=MCU0_R5F_TCMA_START                                length=MCU0_R5F_TCMA_SBL_RSVD_SIZE
    MCU0_R5F_TCMA           (X) : origin=MCU0_R5F_TCMA_START+MCU0_R5F_TCMA_SBL_RSVD_SIZE    length=MCU0_R5F_TCMA_SIZE-MCU0_R5F_TCMA_SBL_RSVD_SIZE
    MCU0_R5F_TCMB0       (RWIX) : origin=MCU0_R5F_TCMB0_START                               length=MCU0_R5F_TCMB0_SIZE

    /* MCU0_R5F_1 SoC view */
    MCU0_R5F1_ATCM       (RWIX) : origin=MCU0_R5F1_ATCM_START                               length=MCU0_R5F1_ATCM_SIZE
    MCU0_R5F1_BTCM       (RWIX) : origin=MCU0_R5F1_BTCM_START                               length=MCU0_R5F1_BTCM_SIZE

    /* Refer the user guide for details on persistence of these sections */
    OCMC_RAM_BOARD_CFG   (RWIX) : origin=OCMC_RAM_BOARD_CFG_START                           length=OCMC_RAM_BOARD_CFG_SIZE
    OCMC_RAM_SCISERVER   (RWIX) : origin=OCMC_RAM_SCISERVER_START                           length=OCMC_RAM_SCISERVER_SIZE
    VECTORS                 (X) : origin=VECTORS_START                                      length=VECTORS_SIZE
    RESET_VECTORS           (X) : origin=RESET_VECTORS_START                                length=RESET_VECTORS_SIZE
    OCMC_RAM             (RWIX) : origin=OCMC_RAM_START                                     length=OCMC_RAM_SIZE
    OCMC_RAM_X509_HEADER (RWIX) : origin=OCMC_RAM_X509_HEADER_START                         length=OCMC_RAM_X509_HEADER_SIZE

    /* j721e MCMS3 locations */
    /* j721e Reserved Memory for ARM Trusted Firmware */
    MSMC3_ARM_FW         (RWIX) : origin=MSMC3_ARM_FW_START                                 length=MSMC3_ARM_FW_SIZE         /* 256KB */
    MSMC3_FLASH           (RIX) : origin=MSMC3_START                                        length=MSMC3_RO_SIZE
    MSMC3_RAM             (RWI) : origin=MSMC3_START+MSMC3_RO_SIZE                          length=MSMC3_SIZE-MSMC3_RO_SIZE

    /* j721e Reserved Memory for DMSC Firmware */
    MSMC3_DMSC_FW        (RWIX) : origin=MSMC3_DMSC_FW_START                                length=MSMC3_DMSC_FW_SIZE         /* 64KB */
    DDR0                 (RWIX) : origin=DDR0_START                                         length=DDR0_SIZE       /* 2GB */

/* Additional memory settings	*/

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
    GROUP
    {
        .safeRTOSrstvectors                                 : {} palign(8)
        .rstvectors                                         : {} palign(8)
    } > MCU0_R5F_TCMA_SBL_RSVD

/* 'intvecs' and 'intc_text' sections shall be placed within                  */
/* a range of +\- 16 MB                                                       */
    .intvecs        palign(32),
                    fill =0xffffffff {}                                     > VECTORS
    .intc_text                                          : {} palign(8)      > VECTORS
	.cinit  align(32) : {} > MSMC3_RAM

    GROUP LOAD_START( lnkStartFlashAddress ), LOAD_END( lnkEndFlashAddress )
    {
        .bootCode                                           : {} palign(8)
        .startupCode                                        : {} palign(8)

        .pinit  align(32) : {}
        
        .unpriv_flash palign(32) :
        {
            *(.text)
            *(.rodata)
        }

       .kernel_function palign(0x10000), LOAD_START( lnkKernelFuncStartAddr ),
                                         LOAD_END( lnkKernelFuncEndAddr ) :
        {
            *(KERNEL_FUNCTION)
        }
    } > MSMC3_FLASH

    GROUP LOAD_START( lnkRamStartAddr )
    {
        .startupData                                        : {} palign(8), type = NOINIT

        .bss                                                : {} align(4)
        .far                                                : {} align(4)
        .data                                               : {} palign(128)
        .boardcfg_data                                      : {} palign(128)
        .sysmem                                             : {}
        .bss.devgroup*                                      : {} align(4)
        .const.devgroup*                                    : {} align(4)
        KERNEL_DATA palign(0x800), LOAD_START( lnkKernelDataStartAddr ),
                                    LOAD_END( lnkKernelDataEndAddr )    : {}
    } > MSMC3_RAM

    GROUP palign(0x10000), LOAD_END( lnkRamEndAddr )
    {
        .stack  RUN_START( lnkStacksStartAddr ) : {}                            align(4)
        .irqStack                               : {. = . + __IRQ_STACK_SIZE;}   align(4)
        RUN_START(__IRQ_STACK_START)
        RUN_END(__IRQ_STACK_END)
        .fiqStack                               : {. = . + __FIQ_STACK_SIZE;}   align(4)
        RUN_START(__FIQ_STACK_START)
        RUN_END(__FIQ_STACK_END)
        .abortStack                             : {. = . + __ABORT_STACK_SIZE;} align(4)
        RUN_START(__ABORT_STACK_START)
        RUN_END(__ABORT_STACK_END)
        .undStack                               : {. = . + __UND_STACK_SIZE;}   align(4)
        RUN_START(__UND_STACK_START)
        RUN_END(__UND_STACK_END)
        .svcStack    END( lnkStacksEndAddr )    : {. = . + __SVC_STACK_SIZE;}   align(4)
        RUN_START(__SVC_STACK_START)
        RUN_END(__SVC_STACK_END)
    } > MSMC3_RAM
/* Additional sections settings 	*/

	/* These configuration settings are for the SafeRTOS FPU demonstration
	 * project. They are included as a demonstration of how task data can be
	 * grouped together into known locations, therefore enabling MPU regions to
	 * be defined. */

    GROUP
    {
    __idle_hook_data__
                    : START( lnkIdleTaskDataStartAddr )
                        {} palign( 0x20 )
    __block_tim_data__
                    : START( lnkBlockTimeTestDataStartAddr )
                        {} palign( 0x20 )
    __block_q_data__
                    : START( lnkBlockQueueTestDataStartAddr )
                        {} palign( 0x80 )
    __dynamic_task_data__
                    : START( lnkDynamicTestDataStartAddr )
                        {} palign( 0x40 )
    __led_task_data__
                    : START( lnkLEDFlashTestDataStartAddr )
                        {} palign( 0x20 )
    __poll_q_data__
                    : START( lnkPollQTestDataStartAddr )
                        {} palign( 0x20 )
    __binary_semaphore_task_data__
                    : START( lnkSemaphoreTestDataStartAddr )
                        {} palign( 0x40 )
    __create_delete_data__
                    : START( lnkDeathTestDataStartAddr )
                        {} palign( 0x20 )
    __com_test_data__
                    : START( lnkComTestDataStartAddr )
                        {} palign( 0x20 )
    __maths_test_data__
                    : START( lnkMathsTestDataStartAddr )
                        {} palign( 0x40 )
    __counting_semaphore_task_data__
                    : START( lnkCountSemTestDataStartAddr )
                        {} palign( 0x40 )
    __notified_task_data__
                    : START( lnkTaskNotifyDataStartAddr )
                        {} palign( 0x20 )
    __timer_demo_task_data__
                    : START( lnkStartTimerTestData )
                        {} palign( 0x800 )
    __rec_mutex_data__
                    : START( lnkRecMutexDataStartAddr )
                        {} palign( 0x400 )
    __event_poll_demo_data__
                    : START( lnkEventPollDemoDataStartAddr )
                        {} palign( 0x40 )
    } > MSMC3_RAM
}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
