/* linker options */
--fill_value=0
--stack_size=0x2000
--heap_size=0x1000

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

-e __VECS_ENTRY_POINT
--retain="*(.utilsCopyVecsToAtcm)"

MEMORY
{
    /* am64x MSMC3 locations */
    RESET_VECTORS (RWIX)               : origin=0x000070040000 length=0x100
    MCU1_0_MSMC3   (RWIX)              : origin=0x000070040100 length=0x37F00
    MCU1_0_MSMC3_DATA (RWIX)           : origin=0x000070078000 length=0xA000
    MCU1_1_MSMC3   (RWIX)              : origin=0x000070082000 length=0x38000
    MCU1_1_MSMC3_DATA (RWIX)           : origin=0x0000700BA000 length=0xA000
    MCU2_0_MSMC3   (RWIX)              : origin=0x0000700C4000 length=0x38000
    MCU2_0_MSMC3_DATA (RWIX)           : origin=0x0000700FC000 length=0xA000
    MCU2_1_MSMC3   (RWIX)              : origin=0x000070106000 length=0x38000
    MCU2_1_MSMC3_DATA (RWIX)           : origin=0x00007013E000 length=0xA000
    /* Reserved for DMSC */
    /*MSMC3_DMSC (RWIX)		: origin=0x700F0000 length=0x10000*/			/* 64K */
    /*MSMC3_H (RWIX)	 		: origin=0x70100000 length=0xF2000*/			/* 1MB -56K */
    /*MSMC3_NOCACHE (RWIX)	: origin=0x701F2000 length=0xE000*/			/* 56K */
    DDR0    (RWIX)  		: origin=0x90000000 length=0x00010000    	/* 64K */

/* Additional memory settings	*/

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
/* 'intvecs' and 'intc_text' sections shall be placed within                  */
/* a range of +\- 16 MB                                                       */
    .vecs       : {
        __VECS_ENTRY_POINT = .;
    } palign(8) > RESET_VECTORS
    .text_boot {
        *boot.aer5f*<*boot.o*>(.text)
     }  palign(8)   > MCU1_0_MSMC3
    .text:xdc_runtime_Startup_reset__I     : {} palign(8) > MCU1_0_MSMC3
    .text:ti_sysbios_family_arm_v7r_Cache* : {} palign(8) > MCU1_0_MSMC3
    .text:ti_sysbios_family_arm_MPU*       : {} palign(8) > MCU1_0_MSMC3
    .utilsCopyVecsToAtcm                   : {} palign(8) > MCU1_0_MSMC3

    .text       : {} palign(8)   > MCU1_0_MSMC3
    .cinit      : {} palign(8)   > MCU1_0_MSMC3
    .bss        : {} align(8)    > MCU1_0_MSMC3
    .far        : {} align(8)    > MCU1_0_MSMC3
    .const      : {} palign(8)   > MCU1_0_MSMC3
    .data       : {} palign(128) > MCU1_0_MSMC3
    .sysmem     : {} align(8)    > MCU1_0_MSMC3
    .stack      : {} align(4)    > MCU1_0_MSMC3
    .data_buffer: {} palign(128) > MCU1_0_MSMC3

    /* USB or any other LLD buffer for benchmarking */
    .benchmark_buffer (NOLOAD) {} ALIGN (8) > MCU1_0_MSMC3_DATA
/* Additional sections settings 	*/

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
