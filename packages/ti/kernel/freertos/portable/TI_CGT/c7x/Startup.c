#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <c7x.h>
#include "Hwi.h"
#include "Cache.h"
#include "Exception.h"





/*
 *  ======== Hwi_initStackMin ========
 *  Initialize only the first byte/word of the Common interrupt stack
 */
static void Hwi_initStackMin(void)
{
    Hwi_StackInfo stkInfo;
    uintptr_t curStack;

    Hwi_getStackInfo(&stkInfo, false);

    curStack = (uintptr_t)(stkInfo.hwiStackBase);
    *((volatile uint8_t *)curStack) = 0xbe;

}


void c7x_startup_init(void)
{
    Hwi_initStackMin();
    Cache_Module_startup();
    Hwi_Module_startup();
    Exception_Module_startup();
}

