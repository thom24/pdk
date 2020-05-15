/*
 * SDL TEST
 *
 * SafeTI Diagnostics Library Test
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <stdint.h>
#include <sdl_common.h>
#include <sdl_esm.h>
#include <sdl_ccm.h>
#include <sdl_crc.h>

/* UART driver API header files */
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/soc/UART_soc.h>

/* Osal API header files */
#include <ti/osal/HwiP.h>

/* Local SDTF header files */
#include "sdtf_common.h"
#include "sdtf_platform.h"
#include "sdtf_exception.h"
#include "sdtf_profile.h"
#include "sdtf_wdt.h"
#include "sdtf_ecc.h"

#ifdef SDTF_BOARD
#include <ti/board/board.h>
#include <ti/osal/osal.h>
#endif

/* Defines */
#define MAX_MEM_SECTIONS (8u)

/* Function prototypes */
void SDTF_copyResetVector(void);
void SDTF_uartStdioInit(void);
int32_t SDTF_EsmInitHandlerInit(void);
int32_t SDTF_init (void);
void SDTF_VIMDEDInterruptHandler(void);


SDL_ESM_InitConfig_t SDTF_esmInitConfig =
{
    .esmErrorConfig = {0u, 3u}, /* Self test error config */
    .eventMap = {0xffffffffu, 0xff0fffffu, 0xffffffffu, 0xffffffe7u},
     /**< All events enable: except timer and self test  events */
    /* Temporarily disabling vim compare error as well*/
    .eventPriorityMap = {0xffffffffu, 0xff0fffffu, 0xffffffffu, 0xffffffe6u},
    /**< All events high priority: except timer, self test and selftest error events */
    .errorOutputMap = {0xffffffffu, 0xff0fffffu, 0xffffffffu, 0xffffffe6u},
    /**< All events high priority: except timer, self test and selftest error events */
};

static SDL_ECC_MemSubType SDTF_R5FCoresubMemTypeList[MAX_MEM_SECTIONS] =
{
    SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
    SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK1_VECTOR_ID,
    SDL_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
    SDL_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID,
    SDL_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID,
    SDL_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID,
    SDL_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID,
};
static SDL_ECC_InitConfig_t SDTF_R5FCoreECCInitConfig =
{
    .numRams = MAX_MEM_SECTIONS,
    /**< Number of Rams ECC is enabled  */
    .pMemSubTypeList = &(SDTF_R5FCoresubMemTypeList[0]),
    /**< Sub type list  */
};

HwiP_Handle SDTF_EsmHiHwiPHandle;
HwiP_Handle SDTF_EsmLoHwiPHandle;
HwiP_Handle SDTF_EsmCfgHwiPHandle;

extern void *_resetvectors;
#define VECTOR_SIZE 0x100
#define APP_MAX_PROFILE_SIZE 16

void SDTF_copyResetVector(void)
{
    /* Invalidate cache */
    SDTF_cpuCacheInv(0, VECTOR_SIZE);

    /* copy teh vectors into 0 location */
    /* Vectors are always assumed to be at 0 for R5 */
    memcpy(0, &_resetvectors, VECTOR_SIZE);

    /* Write back cache */
    SDTF_cpuCacheWbInv(0, VECTOR_SIZE);
}

/**
 * \brief   This function initializes the UART instance for use for
 *          console operations.
 *
 * \return  None
 *
 */
void SDTF_uartStdioInit(void)
{
    UART_HwAttrs uart_cfg;

    /* Disable the UART interrupt */
    (void)UART_socGetInitCfg(SDTF_UART_INSTANCE, &uart_cfg);
    uart_cfg.enableInterrupt = FALSE;
    (void)UART_socSetInitCfg(SDTF_UART_INSTANCE, &uart_cfg);

    UART_stdioInit(SDTF_UART_INSTANCE);
    return;
}

int32_t SDTF_EsmInitHandlerInit(void)
{
    int32_t result = 0;
    uint32_t intNumHi, intNumLo, intNumCfg;
    HwiP_Params       hwiParams;


    /* Get the interrupt number corresponding to ESM HI interrupt */
    intNumHi = SDL_ESM_getIntNumber(SDL_ESM_INT_TYPE_HI);
    if ( intNumHi == SDL_ESM_INTNUMBER_INVALID ) {
        result = -1;
    } else {
        /* Get the interrupt number corresponding to ESM LO interrupt */
        intNumLo = SDL_ESM_getIntNumber(SDL_ESM_INT_TYPE_LO);
        if ( intNumLo == SDL_ESM_INTNUMBER_INVALID ) {
            result = -1;
        } else {
            /* Get the interrupt number corresponding to ESM Config interrupt */
            intNumCfg = SDL_ESM_getIntNumber(SDL_ESM_INT_TYPE_CFG);
            if ( intNumCfg == SDL_ESM_INTNUMBER_INVALID ) {
                result = -1;
            } else {
                SDTF_printf("\n SDTF_EsmInitHandlerInit: Obtain interrupt number complete \n");
                /* Clear all interrupts first */
                HwiP_disableInterrupt(intNumHi);
                HwiP_disableInterrupt(intNumLo);
                HwiP_disableInterrupt(intNumCfg);

                SDTF_printf("\n SDTF_EsmInitHandlerInit: Disable interrupts complete \n");

                /* Default parameter initialization */
                HwiP_Params_init(&hwiParams);

                hwiParams.arg = intNumHi;
                SDTF_printf("\n SDTF_EsmInitHandlerInit: HwiP_Params_init complete \n");
                /* Register call back function for ESM Hi Interrupt */
                SDTF_EsmHiHwiPHandle = HwiP_create(intNumHi,
                                                   (HwiP_Fxn)SDL_ESM_hiInterruptHandler,
                                                   (void *)&hwiParams);
                SDTF_printf("\n SDTF_EsmInitHandlerInit: intNumHi registration complete \n");
                if (SDTF_EsmHiHwiPHandle == (HwiP_Handle) NULL) {
                    result = -1;
                } else {
                    hwiParams.arg = intNumLo;
                    /* Register call back function for ESM Lo Interrupt */
                    SDTF_EsmLoHwiPHandle = HwiP_create(intNumLo,
                                                       (HwiP_Fxn)SDL_ESM_loInterruptHandler,
                                                       (void *)&hwiParams);
                    SDTF_printf("\n SDTF_EsmInitHandlerInit: intNumLo registration complete \n");
                    if (SDTF_EsmLoHwiPHandle == (HwiP_Handle) NULL) {
                        result = -1;
                    } else {
                        hwiParams.arg = intNumCfg;
                        /* Register call back function for ESM Config Interrupt */
                        SDTF_EsmCfgHwiPHandle = HwiP_create(intNumCfg,
                                                   (HwiP_Fxn)SDL_ESM_configInterruptHandler,
                                                   (void *)&hwiParams);
                        SDTF_printf("\n SDTF_EsmInitHandlerInit: intNumCfg registration complete \n");
                        if (SDTF_EsmCfgHwiPHandle == (HwiP_Handle) NULL) {
                            result = -1;
                        } else {
                            /* Enable all ESM Interrupts */
                            HwiP_enableInterrupt(intNumHi);
                            HwiP_enableInterrupt(intNumLo);
                            HwiP_enableInterrupt(intNumCfg);
                       }
                    }
                }
            }
        }
    }

    return result;
}
#define SDTF_NEGATIVE_TESTS
#ifdef SDTF_NEGATIVE_TESTS
/*********************************************************************
* @fn      SDTF_negativeInitTests
*
* @brief   Run negative tests on init functions
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t SDTF_negativeInitTests (void)
{
    int32_t retValue=0;
    SDL_Result result;

    /* Run negative test on init functions */
    result = SDL_ESM_init(NULL);
    if (result == SDL_PASS) {
        /* Negative test failed */
        SDTF_printf("SDTF_init: Error ESM Negative tests: result = %d\n", result);
        retValue = -1;
    }

    if (retValue == 0) {
        result = SDL_ECC_init(SDL_ECC_MEMTYPE_MCU_R5F0_CORE, NULL);
        if (result == SDL_PASS) {
            /* Negative test failed */
            SDTF_printf("SDTF_init: Error ECC Negative tests: result = %d\n", result);
            retValue = -2;
        }
    }
    return retValue;
}
#endif


/*********************************************************************
* @fn      SDTF_init
*
* @brief   Initializes Safety Diagostics Test Framework
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t SDTF_init (void)
{
    int32_t retValue=0;
    SDL_Result result;
#ifdef SDTF_BOARD
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);
#else
    /* Initialize UART instance */
    SDTF_uartStdioInit();
#endif

    SDTF_printf("\nSDTF_init: UART ready to print, proceeding with SDTF init \n");

    /* Initialise Profile module */
    SDTF_profileInit(APP_MAX_PROFILE_SIZE);

    SDTF_printf("\nSDTF_init: Profile init complete \n");

    /* Call Init for SDTF software structures */

    /* Copy reset vector to 0 location */
    SDTF_copyResetVector();

    SDTF_printf("\nSDTF_init: Copy Reset vector complete \n");

    /* Initialise exception handler */
    SDTF_exceptionInit();

    SDTF_printf("\nSDTF_init: Exception init complete \n");

    /* Register VIM DED interrupt handler */
    SDL_ECC_registerVIMDEDHandler(&SDTF_VIMDEDInterruptHandler);

    SDTF_printf("\nSDTF_init: register VIM DED Handler complete \n");

#ifdef SDTF_NEGATIVE_TESTS
    retValue = SDTF_negativeInitTests();
#endif

    if (retValue == 0) {
        /* Initialize ESM module */
        result = SDL_ESM_init(&SDTF_esmInitConfig);
        if (result != SDL_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing ESM: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: Init ESM complete \n");
        }
    }

    if (retValue == 0) {
        /* Initialize ECC */
        result = SDL_ECC_init(SDL_ECC_MEMTYPE_MCU_R5F0_CORE, &SDTF_R5FCoreECCInitConfig);
        if (result != SDL_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing R5F core ECC: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: R5F Core Init ECC complete \n");
        }
    }

    if (retValue == 0) {
        /* Initialize VIM ECC memory ; This is specifically for VIM in lockstep*/
        /* All other memories are auto initialized by hardware */
        result = SDL_ECC_initMemory(SDL_ECC_MEMTYPE_MCU_R5F0_CORE, SDL_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID);
        if (result != SDL_PASS) {
             /* print error and quit */
              SDTF_printf("SDTF_init: Error initializing ECC memory: retValue = %d\n", retValue);
             retValue = -1;;
         } else {
             SDTF_printf("\nSDTF_init: ECC Memory init complete \n");
         }
    }

    if (retValue == 0) {
        /* Wait for any self test running to settle */
        Osal_delay(2000);

        /* Initialize CCM module */
        result = SDL_CCM_init();
        if (result != SDL_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing CCM: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: Init CCM complete \n");
        }
    }

    if (retValue == 0) {
        /* Register Esm handlers */
        retValue = SDTF_EsmInitHandlerInit();
        if (retValue != 0) {
            SDTF_printf("\nSDTF_init: Init ESM handlers failed \n");
        } else {
            SDTF_printf("\nSDTF_init: Init ESM handlers complete \n");
        }
    }

    if (retValue == 0)
    {
        /* Initialize CRC SDL */
        SDL_CRC_init();
        SDTF_printf("\nSDTF_init: Init CRC complete \n");
    }

    if (retValue == 0) {
        /* Initialize Watchdog timer */
        retValue = SDTF_WDT_init();
        if (retValue != 0) {
            SDTF_printf("\nSDTF_init: Init WDT failed \n");
        } else {
            SDTF_printf("\nSDTF_init: Init WDT complete \n");
        }
    }

    return retValue;
}

void SDL_assertExternalFunction(SDL_assertErrorNumber errorNumber)
{

    SDTF_printf("\n  Assert called with error Number %d \n",
                errorNumber);
    /* Asserts are fatal: Currently just wait for ever */
    while(1);
}


uint32_t SDL_getTime(void)
{
    return SDTF_profileTimerRead();
}

void SDL_ESM_applicationCallbackFunction(uintptr_t arg, uint32_t grpChannel, uint32_t index,
                                         uint32_t intSrc){

    SDTF_printf("\n  ESM Call back function called : arg 0x%x, grpChannel 0x%x, index 0x%x, intSrc 0x%x \n",
                arg, grpChannel, index, intSrc);
    SDTF_printf("  Take action \n");

    /* Any additional customer specific actions can be added here */

}

void SDL_ECC_applicationCallbackFunction(uint32_t errorSrc, uint32_t address){

    SDTF_printf("\n  ECC Error Call back function called : errorSrc 0x%x, address 0x%x. \n",
                errorSrc, address);
    SDTF_printf("  Take action \n");

    /* Any additional customer specific actions can be added here */

}

void SDL_CCM_applicationCallbackFunction(SDL_CCM_MonitorType monitorType,
                                         SDL_CCM_ErrorStatus_t *pErrorStatus)
{
    SDTF_printf("\n  CCM Call back function called : monitorType 0x%x, cmp error flag 0x%x,"
            "self test error flag  0x%x, self test error type 0x%x.\n",
            monitorType, pErrorStatus->compareErrorFlag, pErrorStatus->selfTestErrorFlag,
            pErrorStatus->selfTestErrorTypeFlag);
    SDTF_printf("  Take action \n");

   /* Any additional customer specific actions can be added here */

}

/* Note the following DED vector handler need to be declared with the attribute of interrupt
 * so that the appropriate return is implemented correctly
 */
__attribute__((interrupt))     void SDTF_VIMDEDInterruptHandler(void);
/*********************************************************************
* @fn      SDTF_VIMDEDInterruptHandler
*
* @brief   Sample DED Interrupt handler
*
* @param   None
*
* @return   None
*/
void SDTF_VIMDEDInterruptHandler(void)
{
    /* Action to be taken by customer */
    SDTF_printf("\n  VIM DED error happened \n");
    SDTF_printf("  Take action \n");

    /* clear appropriate interrupt events */

    SDTF_ECC_indicateDEDTrigger();
}
