/*----------------------------------------------------------------------------*/
/* r4f_linker.cmd                                                                 */
/*                                                                            */
/* (c) Texas Instruments 2016, All rights reserved.                           */
/*                                                                            */

/* USER CODE BEGIN (0) */
/* USER CODE END */


/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY{
PAGE 0:
    VECTORS  (X)  : origin=0x00000000 length=0x00000100
    TCMA_RAM (RX) : origin=0x00000100 length=0x00003F00
    TCMB_RAM (RW) : origin=0x00080000 length=0x00008000
    SBL_RESERVED_L2_RAM_0 (RW)   : origin=0x10200000 length=0x00030000
    SBL_RESERVED_L2_RAM_1 (RW)   : origin=0x10230000 length=0x00030000
    L2_RAM_0 (RW) : origin=0x10260000 length=0x00048000
    L2_RAM_1 (RW) : origin=0x102A8000 length=0x00048000
    L3_RAM_0 (RW) : origin=0x88000000 length=0x001C8000
    L3_RAM_1 (RW) : origin=0x881C8000 length=0x001C8000
    HWA_RAM_0 (RW) : origin=0x82000000 length=0x00010000
    HWA_RAM_1 (RW) : origin=0x82010000 length=0x00010000

PAGE 1:
    L3_RAM_0 (RW) : origin=0x88000000 length=0x001C8000
    L3_RAM_1 (RW) : origin=0x881C8000 length=0x001C8000
}

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS{
    .intvecs : {} > VECTORS

    /* The linker notation "X >> Y | Z" indicates section X is first allocated in Y
       and allowed to overflow into Z and can be split from Y to Z.
       The linker notation "X > Y | Z" indicates section X is first allocated in Y
       and allowed to overflow into Z and cannot be split from Y to Z. Some sections
       like bss are not allowed to be split so > notation is used for them
    */
    .text    : {} >> TCMA_RAM | L2_RAM_1

    .const   : {} > L2_RAM_1
    .switch  : {} > L2_RAM_1
    .cio:    : {} > SBL_RESERVED_L2_RAM_1 | L2_RAM_1
    .data:   : {} > L2_RAM_1

    .cinit   : {} > L2_RAM_1
    .pinit   : {} > L2_RAM_1
    .bss     : {} > SBL_RESERVED_L2_RAM_1 | L2_RAM_1
    .stack   : {} > TCMB_RAM | SBL_RESERVED_L2_RAM_1 | L2_RAM_1

    .boot:{
       *.*(*ti_sysbios_family_arm_MPU*)
       boot.aer5f*(*.text)
       *.*(*startup*)
       *.*(*Startup*)
       *.*(*Cache*)
     } > TCMA_RAM | TCMB_RAM
    .l3ram   : {} > L3_RAM_1
    .l2ram   : {} > SBL_RESERVED_L2_RAM_1 | L2_RAM_1
}
/*----------------------------------------------------------------------------*/
