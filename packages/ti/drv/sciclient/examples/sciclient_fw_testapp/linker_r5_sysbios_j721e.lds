/* linker options */
--fill_value=0
--stack_size=0x2000
--heap_size=0x1000

-e __VECS_ENTRY_POINT

MEMORY
{
    R5F_TCMA_SBL_RSVD(X): ORIGIN = 0x00000000 , LENGTH = 0x100
    VECTORS (X)                  : origin=0x41C7F000 length=0x1000
    /*  Reset Vectors base address(RESET_VECTORS) should be 64 bytes aligned  */
    RESET_VECTORS (X)                 : origin=0x41C00000 length=0x100
    R5F_TCMB0(RWIX)     : ORIGIN = 0x41010000 , LENGTH = 0x00008000
    MSMC3 (RWIX)        : ORIGIN = 0x70000000 , LENGTH = 0x180000
    MSRAM (RWIX)        : ORIGIN = 0x03600000 , LENGTH = 0x80000       /* 512K OCMC, used for allocation of ICSSG port queues */
}

SECTIONS
{
    .vecs       : {
        __VECS_ENTRY_POINT = .;
    } palign(8) > RESET_VECTORS
    .intvecs         : {} palign(8)                 > VECTORS
    .intc_text       : {} palign(8)                 > VECTORS
    .rstvectors      : {} palign(8)                 > RESET_VECTORS
    .text_boot {
        *boot.aer5f*<*boot.o*>(.text)
     }  palign(8)   > R5F_TCMB0
    .text:xdc_runtime_Startup_reset__I     : {} palign(8) > R5F_TCMB0
    .text:ti_sysbios_family_arm_v7r_Cache* : {} palign(8) > R5F_TCMB0
    .text:ti_sysbios_family_arm_MPU*       : {} palign(8) > R5F_TCMB0

    .text       : {} palign(8)   > MSMC3
    .cinit      : {} palign(8)   > MSMC3
    .bss        : {} align(8)    > MSMC3
    .far        : {} align(8)    > MSMC3
    .const      : {} palign(8)   > MSMC3
    .data       : {} palign(128) > MSMC3
    .sysmem     : {} align(8)    > MSMC3
    .stack      : {} align(4)    > MSMC3
    .data_buffer: {} palign(128) > MSMC3

    /* USB or any other LLD buffer for benchmarking */
    .benchmark_buffer: (NOLOAD) {} align (8) > MSMC3
    
}
