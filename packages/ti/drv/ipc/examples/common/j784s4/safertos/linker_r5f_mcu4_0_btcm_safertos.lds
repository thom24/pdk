#include "memory_map_defines.inc"

#define __VECS                  MCU_R5F_TCMB_VECS
#define __BOOT                  MCU_R5F_TCMB
#define __CORE_IPC_DATA         MCU4_0_IPC_DATA
#define __CORE_EXT_DATA_BASE    MCU4_0_EXT_DATA_BASE
#define __CORE_EXT_DATA         MCU4_0_EXT_DATA
#define __CORE_R5F_MEM_TEXT     MCU4_0_MEM_TEXT
#define __CORE_R5F_MEM_DATA     MCU4_0_MEM_DATA
#define __CORE_DDR_SPACE        MCU4_0_DDR_SPACE
#define __CORE_MSMC_SPACE		MCU4_0_MSMC_SPACE

#include "linker_r5f_safertos_common.inc"