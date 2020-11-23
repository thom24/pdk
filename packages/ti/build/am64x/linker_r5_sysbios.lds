/* linker options */
--fill_value=0
--stack_size=0x2000
--heap_size=0x1000

-e __VECS_ENTRY_POINT
--retain="*(.utilsCopyVecsToAtcm)"

MEMORY
{
    /* Reserved for SBL code/data */
    SBL_RSVD (X)            : origin=0x70000000 length=0x80000
    
    /*  Reset Vectors base address(RESET_VECTORS) should be 64 bytes aligned  */
    RESET_VECTORS (X)       : origin=0x70080000 length=0x100
    /* am64x MCMS3 locations */
    MSMC3   (RWIX)          : origin=0x70080100 length=0x40000 - 0x1100

    VECTORS (X)             : origin=0x700BF000 length=0x1000
    /* Reserved for SYSFW Secure Proxy */
    MSMC3_H (RWIX)          : origin=0x700C0000 length=0x100000   

    /* Reserved by ROM for SYSFW */
    SYSFW_RSVD_1 (X)      : origin=0x71C00000 length=0x20000 
    SYSFW_RSVD_2 (X)      : origin=0x71E00000 length=0x20000 

    DDR0    (RWIX)          : origin=0x80000000 length=0x80000000  /* 2GB */
}

SECTIONS
{
    .vecs       : {
        __VECS_ENTRY_POINT = .;
    } palign(8) > RESET_VECTORS
    .text_boot {
        *boot.aer5f*<*boot.o*>(.text)
     }  palign(8)   > MSMC3
    .text:xdc_runtime_Startup_reset__I     : {} palign(8) > MSMC3
    .text:ti_sysbios_family_arm_v7r_Cache* : {} palign(8) > MSMC3
    .text:ti_sysbios_family_arm_MPU*       : {} palign(8) > MSMC3
    .utilsCopyVecsToAtcm                   : {} palign(8) > MSMC3

    .text       : {} palign(8)   > DDR0
    .cinit      : {} palign(8)   > DDR0
    .bss        : {} align(8)    > DDR0
    .far        : {} align(8)    > DDR0
    .const      : {} palign(8)   > DDR0
    .data       : {} palign(128) > DDR0
    .sysmem     : {} align(8)    > DDR0
    .stack      : {} align(4)    > DDR0
    .data_buffer: {} palign(128) > DDR0

    .benchmark_buffer: (NOLOAD) {} align (8) > DDR0
    
    
}
