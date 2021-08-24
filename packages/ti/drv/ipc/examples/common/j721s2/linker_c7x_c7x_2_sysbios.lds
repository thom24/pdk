/* Linker Settings */
#define DDR0_ALLOCATED_START  0xA0000000
#define C7X_2_ALLOCATED_START DDR0_ALLOCATED_START + 0x07000000

#define C7X_2_EXT_DATA_BASE   (C7X_2_ALLOCATED_START + 0x00100000)
#define C7X_2_MEM_TEXT_BASE   (C7X_2_ALLOCATED_START + 0x00200000)
#define C7X_2_MEM_DATA_BASE   (C7X_2_ALLOCATED_START + 0x00300000)
#define C7X_2_DDR_SPACE_BASE  (C7X_2_ALLOCATED_START + 0x00400000)

MEMORY
{
    L2SRAM (RWX):  org = 0x64800000,          len = 0x00070000
    DDR0_RESERVED: org = 0x80000000,          len = 0x20000000   /* 512MB Reserved for A72 OS */
    C7X_2_IPC_D:     org = C7X_2_ALLOCATED_START, len = 0x00100000   /*  1MB DDR */
    C7X_2_EXT_D:     org = C7X_2_EXT_DATA_BASE,   len = 0x00100000   /*  1MB DDR */
    C7X_2_TEXT:      org = C7X_2_MEM_TEXT_BASE,   len = 0x00100000   /*  1MB DDR */
    C7X_2_DATA:      org = C7X_2_MEM_DATA_BASE,   len = 0x00100000   /*  1MB DDR */
    C7X_2_DDR_SPACE: org = C7X_2_DDR_SPACE_BASE,  len = 0x00C00000   /* 12MB DDR */
}

-c
-heap  0x20000
-stack 0x20000
--args 0x1000
--diag_suppress=10068 /* "no matching section" */
--cinit_compression=off
--retain="*(xdc.meta)"
-e _c_int00_secure

ti_sysbios_family_c7x_Cache_l1dSize = 32768;
ti_sysbios_family_c7x_Cache_l2Size = 32768;


SECTIONS
{
    xdc.meta (COPY):    { } > C7X_2_DDR_SPACE
    boot:
    {
      boot.*<boot.oe71>(.text)
    } load > C7X_2_TEXT ALIGN(0x200000)
    .vecs       >       C7X_2_DDR_SPACE ALIGN(0x400000)
    .secure_vecs >      C7X_2_DDR_SPACE ALIGN(0x200000)
    .text:_c_int00_secure > C7X_2_DDR_SPACE ALIGN(0x200000)
    .text       >       C7X_2_DDR_SPACE ALIGN(0x200000)

    .bss        >       C7X_2_DDR_SPACE  /* Zero-initialized data */
    .data       >       C7X_2_DDR_SPACE  /* Initialized data */

    .cinit      >       C7X_2_DDR_SPACE  /* could be part of const */
    .init_array >       C7X_2_DDR_SPACE  /* C++ initializations */
    .stack      >       C7X_2_DDR_SPACE  ALIGN(0x20000) /* Needed for Nested ISR handling */
    .args       >       C7X_2_DDR_SPACE
    .cio        >       C7X_2_DDR_SPACE
    .const      >       C7X_2_DDR_SPACE
    .switch     >       C7X_2_DDR_SPACE /* For exception handling. */
    .sysmem     >       C7X_2_DDR_SPACE /* heap */

    GROUP:              >  C7X_2_DDR_SPACE
    {
        .data.ti_sysbios_family_c7x_Mmu_tableArray          : type=NOINIT
        .data.ti_sysbios_family_c7x_Mmu_tableArraySlot      : type=NOINIT
        .data.ti_sysbios_family_c7x_Mmu_level1Table         : type=NOINIT
        .data.ti_sysbios_family_c7x_Mmu_tableArray_NS       : type=NOINIT
        .data.ti_sysbios_family_c7x_Mmu_tableArraySlot_NS   : type=NOINIT
        .data.ti_sysbios_family_c7x_Mmu_level1Table_NS      : type=NOINIT
    }

    ipc_data_buffer:       > C7X_2_DDR_SPACE
    .resource_table: { __RESOURCE_TABLE = .;} > C7X_2_EXT_D
}

