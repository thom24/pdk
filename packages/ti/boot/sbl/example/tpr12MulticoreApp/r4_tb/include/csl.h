/*
 * CSL Header File
 *
 * This is the CSL Header exported file
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */

#ifndef CSL_H
#define CSL_H

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>

#ifdef SOC_TPR12
#include <ti/csl/soc/csl_tpr12.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/
extern uint32_t CSL_make8 (uint8_t msb, uint8_t lsb, uint8_t val);
extern uint32_t CSL_make16 (uint8_t msb, uint8_t lsb, uint16_t val);
extern uint32_t CSL_make32 (uint8_t msb, uint8_t lsb, uint32_t val);
extern uint8_t  CSL_extract8 (volatile uint32_t reg, uint8_t msb, uint8_t lsb);
extern uint16_t CSL_extract16 (volatile uint32_t reg, uint8_t msb, uint8_t lsb);
extern uint32_t CSL_extract32 (volatile uint32_t reg, uint8_t msb, uint8_t lsb);
extern uint32_t CSL_insert8 (volatile uint32_t reg, uint8_t msb, uint8_t lsb, uint8_t val);
extern uint32_t CSL_insert16 (volatile uint32_t reg, uint8_t msb, uint8_t lsb, uint16_t val);
extern uint32_t CSL_insert32 (volatile uint32_t reg, uint8_t msb, uint8_t lsb, uint32_t val);

#ifdef __cplusplus
}
#endif

#endif /* CSL_H */

