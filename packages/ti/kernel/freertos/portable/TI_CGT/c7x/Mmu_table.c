#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <c7x.h>
#include "Mmu.h"



#pragma DATA_SECTION(Mmu_tableArray, ".data.Mmu_tableArray");
#pragma DATA_ALIGN(Mmu_tableArray, 4096);

uint64_t Mmu_tableArray[16][512];

#pragma DATA_SECTION(Mmu_tableArray_NS, ".data.Mmu_tableArray_NS");
#pragma DATA_ALIGN(Mmu_tableArray_NS, 4096);


uint64_t Mmu_tableArray_NS[16][512];
