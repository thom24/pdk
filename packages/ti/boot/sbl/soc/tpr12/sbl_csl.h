/*
 * CSL Header File
 *
 * This is the CSL Header exported file
 *
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */

#ifndef SBL_CSL_H
#define SBL_CSL_H

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/
extern uint32_t CSL_insert32 (volatile uint32_t reg, uint8_t msb, uint8_t lsb, uint32_t val);
extern uint32_t CSL_insert16 (volatile uint32_t reg, uint8_t msb, uint8_t lsb, uint16_t val);
extern uint32_t CSL_insert8 (volatile uint32_t reg, uint8_t msb, uint8_t lsb, uint8_t val);

extern uint32_t CSL_extract32 (volatile uint32_t reg, uint8_t msb, uint8_t lsb);
extern uint16_t CSL_extract16 (volatile uint32_t reg, uint8_t msb, uint8_t lsb);
extern uint8_t CSL_extract8 (volatile uint32_t reg, uint8_t msb, uint8_t lsb);

#ifdef __cplusplus
}
#endif

#endif /* SBL_CSL_H */

