/* Linker Settings */
#define L1P_MEM_START               ( 0x00E00000 )
#define L1P_PAGE_SIZE               ( 2K )
#define L1P_NUM_TOTAL_PAGES         ( 16 )

#define L1P_CACHE_SIZE              ( 16 * 1024 )
#define L1P_MEM_SIZE                ( ( L1P_PAGE_SIZE * L1P_NUM_TOTAL_PAGES ) - L1P_CACHE_SIZE )
#define L1P_NUM_AVAILABLE_PAGES     ( L1P_MEM_SIZE / L1P_PAGE_SIZE )


#define L1D_MEM_START               ( 0x00F00000 )
#define L1D_PAGE_SIZE               ( 2K )
#define L1D_NUM_TOTAL_PAGES         ( 16 )

#define L1D_CACHE_SIZE              ( 16 * 1024 )
#define L1D_MEM_SIZE                ( ( L1D_PAGE_SIZE * L1D_NUM_TOTAL_PAGES ) - L1D_CACHE_SIZE )
#define L1D_NUM_AVAILABLE_PAGES     ( L1D_MEM_SIZE / L1D_PAGE_SIZE )


#define L2_MEM_START                ( 0x00800000 )
#define L2_PAGE_SIZE                ( 16K )
#define L2_NUM_TOTAL_PAGES          ( 18 )

#define L2_CACHE_SIZE               ( 0 )
#define L2_MEM_SIZE                 ( ( L2_PAGE_SIZE * L2_NUM_TOTAL_PAGES ) - L2_CACHE_SIZE )
#define L2_NUM_AVAILABLE_PAGES      ( L2_MEM_SIZE / L2_PAGE_SIZE )

#define L2_CODE_START               ( L2_MEM_START )
#define L2_CODE_SIZE                ( L2_RO_DATA_START - L2_CODE_START )
#define L2_RO_DATA_START            ( 0x00820000 )
#define L2_RO_DATA_SIZE             ( L2_RW_DATA_START - L2_RO_DATA_START )
#define L2_RW_DATA_START            ( 0x00828000 )
#define L2_RW_DATA_SIZE             ( L2_MEM_SIZE - L2_CODE_SIZE - L2_RO_DATA_SIZE)


#define L3_MEM_START                ( 0x88000000 )
#define L3_DATA_SIZE                ( 0x00390000 )

#define HWA_MEM_START               ( 0x82000000 )
#define HWA_DATA_SIZE               ( 0x00020000 )

#define DEFAULT_SECTION_ALIGNMENT   ( 1K )


MEMORY
{
PAGE 0:

#if (L1P_CACHE_SIZE < 0x8000)
    L1PSRAM (IX)                :   o = L1P_MEM_START,      l = L1P_MEM_SIZE
#endif
#if (L1D_CACHE_SIZE < 0x8000)
    L1DSRAM (RW)                :   o = L1D_MEM_START,      l = L1D_MEM_SIZE
#endif
    L2_CODE_SRAM (RIX)          :   o = L2_CODE_START,      l = L2_CODE_SIZE
    L2_READ_ONLY_SRAM (R)       :   o = L2_RO_DATA_START,   l = L2_RO_DATA_SIZE
    L2_READ_WRITE_SRAM (RW)     :   o = L2_RW_DATA_START,   l = L2_RW_DATA_SIZE
    L3SRAM                      :   o = L3_MEM_START,       l = L3_DATA_SIZE
    HWA_RAM                     :   o = HWA_MEM_START,      l = HWA_DATA_SIZE
    MSMC3 (RWIX)            : o = 0x70040000  l = 0x7B0000      /* 8MB - 320KB */
    /* PAGEs 1 and onwards are for overlay purposes for memory optimization.
       Some examples:
       1. Overlay one-time only text with uninitialized data.
       2. Overlay L1PSRAM data path processing fast code and use copy tables
          to page in (before entering data path) and out of L1PSRAM (when entering
          sleep/low power).
    */
PAGE 1:
    L3SRAM:   o = L3_MEM_START, l = L3_DATA_SIZE
}

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x1000                              /* HEAP AREA SIZE                */
-u InterruptVectorTable

/* Set L1D, L1P and L2 Cache Sizes */
ti_sysbios_family_c66_Cache_l1dSize = L1D_CACHE_SIZE;
ti_sysbios_family_c66_Cache_l1pSize = L1P_CACHE_SIZE;
ti_sysbios_family_c66_Cache_l2Size  = L2_CACHE_SIZE;

SECTIONS
{
	/* For SafeRTOS based applications the following symbols need to be exported:
	 *
	 * lnkFlashStartAddr - the start of the Flash memory section used during runtime.
	 *
	 * IMPORTANT NOTE: lnkFlashStartAddr can be relocated (e.g. when a
	 * bootloader is used), but it must be aligned according to its size
	 * (rounded up to the next power of 2), otherwise the MPU will be
	 * configured incorrectly.
	 *
	 * lnkFlashEndAddr - the end of the Flash memory section used during runtime.
	 *
	 * lnkKernelFuncStartAddr - the start address of the Flash block containing
	 * the interrupt vector table and the kernel functions.
	 * NOTE: This should be aligned according to the section size, i.e. 0x8000.
	 *
	 * lnkKernelFuncEndAddr - the end address of the Flash block containing
	 * the interrupt vector table and the kernel functions.
	 *
	 * lnkKernelDataStartAddr - the start address of the RAM block containing
	 * the system stacks and the kernel data.
	 *
	 * lnkKernelDataEndAddr - the end address of the RAM block containing
	 * the system stacks and the kernel data.
	 */

    /* hard addresses forces vecs to be allocated there */

    .priv_code          palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkKernelFuncStartAddr ),
                        LOAD_END( lnkKernelFuncEndAddr ) :
    {
        *(.interrupt_vectors)
        *(.KERNEL_FUNCTION)
    } > L2_CODE_START

    .unpriv_code        palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkStartFlashAddress ) :
    {
        *(.text)
    } > L2_CODE_SRAM

    .unpriv_rodata      palign( DEFAULT_SECTION_ALIGNMENT ) :
    {
        *(.const)
        *(.switch)
    } > L2_READ_ONLY_SRAM

    .rodata             palign( DEFAULT_SECTION_ALIGNMENT )     : {} > L2_READ_WRITE_SRAM
    .cinit              LOAD_END( lnkEndFlashAddress )          : {} > L2_READ_WRITE_SRAM

    .KERNEL_DATA        palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkKernelDataStartAddr ),
                        LOAD_END( lnkKernelDataEndAddr )        : {} > L2_RW_DATA_START

    GROUP               palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkUnprivilegedDataStartAddr ),
                        LOAD_END( lnkUnprivilegedDataEndAddr )
    {
    .bss:
    .neardata:
    .cio:
    .data:
    .sysmem:
    .fardata:
    .far:
    } > L2_READ_WRITE_SRAM

    .stack              palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkStacksStartAddr )        : {} > L2_READ_WRITE_SRAM


/* Additional sections settings     */

    /* These configuration settings are for the SafeRTOS demonstration
     * project. They are included as a demonstration of how task data can be
     * grouped together into known locations, therefore enabling MPU regions to
     * be defined. */


    __idle_hook_data__  palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkIdleHookDataStartAddr )           : {} > MSMC3

    __led_task_data__   palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkLEDFlashTestDataStartAddr )       : {} > MSMC3

    __notified_task_data__ palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkTaskNotifyDataStartAddr )         : {} > MSMC3

    __timer_demo_task_data__ palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkStartTimerTestData )              : {} > MSMC3

    __com_test_data__   palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkComTestDataStartAddr )            : {} > MSMC3

    __block_tim_data__  palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkBlockTimeTestDataStartAddr )      : {} > MSMC3

    __block_q_data__    palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkBlockQueueTestDataStartAddr )     : {} > MSMC3

    __dynamic_task_data__ palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkDynamicTestDataStartAddr )        : {} > MSMC3

    __poll_q_data__     palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkPollQTestDataStartAddr )          : {} > MSMC3

    __binary_semaphore_task_data__ palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkSemaphoreTestDataStartAddr )      : {} > MSMC3

    __maths_test_data__ palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkMathsTestDataStartAddr )          : {} > MSMC3

    __create_delete_data__ palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkCreateDeleteDemoDataStartAddr )   : {} > MSMC3

    __counting_semaphore_task_data__ palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkCountSemTestDataStartAddr )       : {} > MSMC3

    __rec_mutex_data__  palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkRecMutexDataStartAddr )           : {} > MSMC3

    __streambuffer_data__ palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkStreamBufferDataStartAddr )       : {} > MSMC3

	__streambuffer_data_common__ palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkStreamBufferDataCommonStartAddr ) : {} > MSMC3

	__evt_mplx_data__   palign( DEFAULT_SECTION_ALIGNMENT ),
                        START( lnkMplxDataStartAddr )               : {} > MSMC3
}

