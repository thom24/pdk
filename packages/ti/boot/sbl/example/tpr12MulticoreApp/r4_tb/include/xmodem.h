/*
 * XMODEM File
 *
 * This is the header file for XMODEM transmitter
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */
#ifndef XMODEM_H
#define XMODEM_H

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
/* Standard Include Files. */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/

extern int32_t xmodemTransmit(FILE *ptr);
extern int32_t xmodemTransmitBuff (uint8_t *ptrSocIdBuff, uint32_t *ptrSocIdSize, uint8_t* ptrImageBuff, uint32_t imageSize);
#ifdef __cplusplus
}
#endif

#endif /* XMODEM_H */

