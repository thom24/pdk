/*
 * SDL TEST
 *
 * SafeTI Diagnostics Library Test
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <sdl_common.h>
#include <sdl_ecc.h>
#include "sdtf_errortype.h"
#include "sdtf_common.h"
#include "sdtf_injecterr.h"

void *SDTF_ramTestaddr=(void *)0x41c50000;

/*******************************************************************************
 * @fn      SDTF_injectError
 *
 * @brief   Routine to inject error
 *
 * @param   errorType  Type of error to introduce
 *
 * @return    0 : Success; < 0 for failures
 */
int32_t SDTF_injectError(SDTF_errorType_t errorType)
{
    SDL_Result result;

    switch(errorType) {
        case SDTF_ERROR_TYPE_ECC_RAM_SINGLE_BIT:
            {
                SDL_ECC_InjectErrorConfig_t injectErrorConfig;

                injectErrorConfig.pErrMem =  SDTF_ramTestaddr;
                injectErrorConfig.flipBitMask =  SDTF_RAM_TEST_1BIT;
                /* Call SDL API to inject ecc single bit error */
                result = SDL_ECC_injectError(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                            SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                            SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                            &injectErrorConfig);
                if(result != SDL_PASS) {
                    SDTF_printf("\n SDTF_ERROR_TYPE_ECC_RAM_SINGLE_BIT test failed");
                    return -1;
                }
            }
            break;

        case SDTF_ERROR_TYPE_ECC_RAM_DOUBLE_BIT:
	    /* Call SDL API to inject ecc double bit error */
            {
                SDL_ECC_InjectErrorConfig_t injectErrorConfig;

                injectErrorConfig.pErrMem =  SDTF_ramTestaddr;
                injectErrorConfig.flipBitMask =  SDTF_RAM_TEST_2BIT;
                /* Call SDL API to inject ecc double bit error */
                result = SDL_ECC_injectError(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                            SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                            SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                            &injectErrorConfig);
                if(result != SDL_PASS) {
                    SDTF_printf("\n SDTF_ERROR_TYPE_ECC_RAM_DOUBLE_BIT test failed");
                    return -1;
                }

            }
            break;

        case SDTF_ERROR_TYPE_NONE:
        case SDTF_ERROR_TYPE_MAX_ERROR:
        default:
            SDTF_printf(" ERROR: invalid Error type %d \n", errorType);
            break;
    }

    return 0;
}
