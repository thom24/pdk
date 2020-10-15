
/* This is standard linker options used by examples applications and tests    */
/* Please refer user guide that came with this release for more details       */
/* on which sections could be used at load time and runtime                   */

--fill_value=0
/* Retain entry point */
-e __VECS_ENTRY_POINT

/*----------------------------------------------------------------------------*/
/* Memory Sections                                                            */
MEMORY
{
    /* Refer the user guide for details on persistence of these sections */
    /* Also, when these memories can be used by apps */
    MCU0_ATCM_NOT_USED          (R)     : origin=0x0        length=0x7FFF
    MCU0_BTCM_NOT_USED          (R)     : origin=0x41010000 length=0x7FFF

    MCU_MSRAM_RSVD_UNUSED       (R)     : origin=0x41C00000 length=0x200
    MCU_MSRAM_RSVD_SBL          (RWIX)  : origin=0x41C00200 length=0x3DE00

    MCU_MSRAM_APPS_VECS         (RWIX)  : origin=0x41C3E000 length=0x100
    MCU_MSRAM_APPS              (RWIX)  : origin=0x41C3E100 length=0x41F00

    /* COMPUTE_CLUSTER0_MSMC_SRAM */
    CMPT_CLUST_MSMC_SRAM_APPS   (RWIX) : origin=0x70000000 length=0x1EFC00

    DDR_APPS                    (RWIX)  : origin=0x80000000 length=0x7FFFFFE4

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
    /* Place sysbios entry point - starts */
    .vecs : { *(.vecs) } palign(8) > MCU_MSRAM_APPS_VECS
    .vecs : { __VECS_ENTRY_POINT = .; } > MCU_MSRAM_APPS_VECS

    xdc.meta (COPY): { *(xdc.meta) } > MCU_MSRAM_APPS
    .init_text  : {
                     boot.*(.text)
                     *(.text:ti_sysbios_family_arm_MPU_*)
                     *(.text:ti_sysbios_family_arm_v7r_Cache_*)
                  }  palign(8) > MCU_MSRAM_APPS

    .text:xdc_runtime_Startup_reset__I: {} palign(8) > MCU_MSRAM_APPS

    .bootCode       : {} palign(8) > MCU_MSRAM_APPS
    .startupCode    : {} palign(8) > MCU_MSRAM_APPS
    .startupData    : {} palign(8) > MCU_MSRAM_APPS, type = NOINIT

    /* Place sysbios entry point - ends */

    .text    : {} palign(8) > CMPT_CLUST_MSMC_SRAM_APPS
    .const   : {} palign(8) > CMPT_CLUST_MSMC_SRAM_APPS
    .cinit   : {} palign(8) > CMPT_CLUST_MSMC_SRAM_APPS
    .pinit   : {} palign(8) > CMPT_CLUST_MSMC_SRAM_APPS

    /* For NDK packet memory, we need to map this sections before .bss*/
    .bss:NDK_MMBUFFER  (NOLOAD) {} ALIGN (128) > DDR_APPS
    .bss:NDK_PACKETMEM (NOLOAD) {} ALIGN (128) > DDR_APPS

    .bss    : {} align(8)   > CMPT_CLUST_MSMC_SRAM_APPS
    .far    : {} align(8)   > DDR_APPS
    .data   : {} palign(128)> CMPT_CLUST_MSMC_SRAM_APPS
    .boardcfg_data : {} palign(128) > CMPT_CLUST_MSMC_SRAM_APPS
    .sysmem : {} align(8)   > CMPT_CLUST_MSMC_SRAM_APPS
    .stack  : {} align(8)   > CMPT_CLUST_MSMC_SRAM_APPS  (HIGH)

    /* SA sections */
    .scBufs         : {} align(8)   > DDR_APPS
    .saSrcBuffers   : {} align(8)   > DDR_APPS
    .saDstBuffers   : {} align(8)   > DDR_APPS

    /* LLD buffer for benchmarking */
    .benchmark_buffer (NOLOAD) {} ALIGN (8) > DDR_APPS

    /* Additional sections settings */

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
