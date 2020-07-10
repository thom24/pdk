
-heap 3072
--retain="*(.intvecs)"
--entry_point=_c_int00


/* SPECIFY THE SYSTEM MEMORY MAP */
MEMORY
{
    P_VEC               : org = 0x00000000   len = 0x00000020
    P_STARTUP           : org = 0x00000020   len = 0x000003E0
    P_MEM               : org = 0x00000400   len = 0x00007C00
    PRIMARY_IMAGE       : org = 0x00008000   len = 0x00038000  /* 228K - primary image     */
    D_MEM               : org = 0x08000000   len = 0x00010000  /* DATA MEMORY (RAM)    TCMB0-1*/
}

/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY */
SECTIONS
{
    .intvecs : {*(.intvecs)} > P_VEC                /* INTERRUPT VECTORS                 */
    .startup : {} > P_STARTUP ALIGN(8)
    .init_array : {} > P_STARTUP ALIGN(8)
    .vtable : {}  > P_STARTUP ALIGN(8)

    .priImg : {} > PRIMARY_IMAGE ALIGN(8), type = NOINIT
	.code : { *(.text) } > P_MEM ALIGN(8)
	.const {} > P_MEM            ALIGN(8)
	.cinit {} > P_MEM            ALIGN(8)
	.pinit {} > P_MEM            ALIGN(8)

    .bss     : {} > D_MEM              /* GLOBAL & STATIC VARS              */
    .data    : {} > D_MEM              /* GLOBAL & STATIC VARS with C-init  */
    .sysmem  : {} > D_MEM              /* DYNAMIC MEMORY ALLOCATION AREA    */
    .stack   :                         /* SOFTWARE SYSTEM STACK             */
     {                                                
      . += 0x2000;    _SP_USERMODE   = .;
      . += 0x400;    _SP_SVCMODE    = .;
      . += 0x1000;    _SP_IRQMODE    = .;
      . += 0x400;    _SP_FIQMODE    = .;
      . += 0x400;    _SP_UDEFMODE   = .;
      . += 0x400;    _SP_ABTMODE    = .;
     } > D_MEM (HIGH)
     

     .tcmb_data {} > D_MEM
}

