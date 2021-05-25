/*============================*/
/*      DDR MEMORY MAP        */
/*============================*/

#define DDR0_RESERVED_START     0x80000000
#define DDR0_RESERVED_SIZE      0x20000000 /* 512MB */

#define DDR0_ALLOCATED_START    0xA0000000 /* DDR0_RESERVED_START + DDR0_RESERVED_SIZE */

#define SHARED_DDR_SPACE_START  0xA2000000
#define SHARED_DDR_SPACE_SIZE   0x00200000  /*  2MB */

/*------------------------------------------------*/
/* Size of various Memory Locations for each core */ 
/*------------------------------------------------*/
#define IPC_DATA_SIZE   0x00100000 /*  1MB */
#define EXT_DATA_SIZE   0x00100000 /*  1MB */
#define MEM_TEXT_SIZE   0x00100000 /*  1MB */
#define MEM_DATA_SIZE   0x00100000 /*  1MB */
#define DDR_SPACE_SIZE  0x00C00000 /* 12MB */

#define CORE_TOTAL_SIZE 0x01000000 /* 16MB (IPC_DATA_SIZE + R5F_MEM_TEXT_SIZE + R5F_MEM_DATA_SIZE + DDR_SPACE_SIZE) */

/*-----------------------------*/
/* Start address for each core */ 
/*-----------------------------*/

#define MCU1_0_ALLOCATED_START   (DDR0_ALLOCATED_START)
#define MCU1_1_ALLOCATED_START   (MCU1_0_ALLOCATED_START + CORE_TOTAL_SIZE)

/*--------------------------- MCU R5FSS0 CORE0 --------------------------*/
#define MCU1_0_IPC_DATA_BASE     (MCU1_0_ALLOCATED_START)
#define MCU1_0_EXT_DATA_BASE     (MCU1_0_IPC_DATA_BASE     + IPC_DATA_SIZE)
#define MCU1_0_MEM_TEXT_BASE     (MCU1_0_EXT_DATA_BASE     + EXT_DATA_SIZE)
#define MCU1_0_MEM_DATA_BASE     (MCU1_0_MEM_TEXT_BASE     + MEM_TEXT_SIZE)
#define MCU1_0_DDR_SPACE_BASE    (MCU1_0_MEM_DATA_BASE     + MEM_DATA_SIZE)
/*--------------------------- MCU R5FSS0 CORE1 --------------------------*/
#define MCU1_1_IPC_DATA_BASE     (MCU1_1_ALLOCATED_START)
#define MCU1_1_EXT_DATA_BASE     (MCU1_1_IPC_DATA_BASE     + IPC_DATA_SIZE)
#define MCU1_1_MEM_TEXT_BASE     (MCU1_1_EXT_DATA_BASE     + EXT_DATA_SIZE)
#define MCU1_1_MEM_DATA_BASE     (MCU1_1_MEM_TEXT_BASE     + MEM_TEXT_SIZE)
#define MCU1_1_DDR_SPACE_BASE    (MCU1_1_MEM_DATA_BASE     + MEM_DATA_SIZE)


MEMORY
{
    DDR0_RESERVED       (RWIX)  : ORIGIN = DDR0_RESERVED_START      LENGTH = DDR0_RESERVED_SIZE 
    /*---------------------------------- MCU R5FSS0 CORE0 ------------------------------*/
    MCU1_0_IPC_DATA     (RWIX)	: ORIGIN = MCU1_0_IPC_DATA_BASE     LENGTH = IPC_DATA_SIZE
    MCU1_0_EXT_DATA     (RWIX)	: ORIGIN = MCU1_0_EXT_DATA_BASE     LENGTH = EXT_DATA_SIZE
    MCU1_0_R5F_MEM_TEXT (RWIX)	: ORIGIN = MCU1_0_MEM_TEXT_BASE     LENGTH = MEM_TEXT_SIZE
    MCU1_0_R5F_MEM_DATA (RWIX)	: ORIGIN = MCU1_0_MEM_DATA_BASE     LENGTH = MEM_DATA_SIZE
    MCU1_0_DDR_SPACE    (RWIX)	: ORIGIN = MCU1_0_DDR_SPACE_BASE    LENGTH = DDR_SPACE_SIZE
    /*---------------------------------- MCU R5FSS0 CORE1 ------------------------------*/
    MCU1_1_IPC_DATA     (RWIX)	: ORIGIN = MCU1_1_IPC_DATA_BASE     LENGTH = IPC_DATA_SIZE
    MCU1_1_EXT_DATA     (RWIX)	: ORIGIN = MCU1_1_EXT_DATA_BASE     LENGTH = EXT_DATA_SIZE
    MCU1_1_R5F_MEM_TEXT (RWIX)	: ORIGIN = MCU1_1_MEM_TEXT_BASE     LENGTH = MEM_TEXT_SIZE
    MCU1_1_R5F_MEM_DATA (RWIX)	: ORIGIN = MCU1_1_MEM_DATA_BASE     LENGTH = MEM_DATA_SIZE
    MCU1_1_DDR_SPACE    (RWIX)	: ORIGIN = MCU1_1_DDR_SPACE_BASE    LENGTH = DDR_SPACE_SIZE
    /*---------------------------------- Shared Region ----------------------------------------*/
    SHARED_DDR_SPACE    (RWIX)	: ORIGIN = SHARED_DDR_SPACE_START  LENGTH = SHARED_DDR_SPACE_SIZE  
} 
