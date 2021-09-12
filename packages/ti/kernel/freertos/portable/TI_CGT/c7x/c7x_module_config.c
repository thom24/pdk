#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <c7x.h>
#include "Hwi.h"
#include "Cache.h"
#include "Exception.h"
#include "Mmu.h"
#include "TimestampProvider.h"



/* useInternalBuffer */
#pragma DATA_SECTION(Exception_useInternalBuffer, ".const:Exception_useInternalBuffer");
const bool Exception_useInternalBuffer = 0;

/* enablePrint */
#pragma DATA_SECTION(Exception_enablePrint, ".const:Exception_enablePrint");
const bool Exception_enablePrint = 1;

/* exceptionHook */
#pragma DATA_SECTION(Exception_exceptionHook, ".const:Exception_exceptionHook");
const Exception_FuncPtr Exception_exceptionHook = ((Exception_FuncPtr)NULL);

/* internalHook */
#pragma DATA_SECTION(Exception_internalHook, ".const:Exception_internalHook");
const Exception_FuncPtr Exception_internalHook = ((Exception_FuncPtr)NULL);

/* returnHook */
#pragma DATA_SECTION(Exception_returnHook, ".const:Exception_returnHook");
const Exception_FuncPtr Exception_returnHook = ((Exception_FuncPtr)NULL);


/* enableCache__C */
#pragma DATA_SECTION(Cache_enableCache, ".const:Cache_enableCache");
const bool Cache_enableCache = 1;

/* initSize */
#pragma DATA_SECTION(Cache_initSize, ".const:Cache_initSize");
const Cache_Size Cache_initSize = {
    Cache_L1Size_32K,  /* l1pSize */
    Cache_L1Size_32K,  /* l1dSize */
    Cache_L2Size_0K,  /* l2Size */
};


/* --> InitMmu */
extern void InitMmu(void);

/* initFunc */
#pragma DATA_SECTION(Mmu_initFunc, ".const:Mmu_initFunc");
const Mmu_InitFuncPtr Mmu_initFunc = ((Mmu_InitFuncPtr)(InitMmu));


/* dispatcherAutoNestingSupport */
#pragma DATA_SECTION(Hwi_dispatcherAutoNestingSupport, ".const:Hwi_dispatcherAutoNestingSupport");
const bool Hwi_dispatcherAutoNestingSupport = 0;

/* DEFAULT_INT_PRIORITY */
#pragma DATA_SECTION(Hwi_DEFAULT_INT_PRIORITY, ".const:Hwi_DEFAULT_INT_PRIORITY");
const unsigned int Hwi_DEFAULT_INT_PRIORITY = (unsigned int)0x6U;

