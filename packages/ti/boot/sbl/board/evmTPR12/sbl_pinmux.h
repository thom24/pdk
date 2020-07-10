/*
 * sbl_pinmux.h
 *
 * PINMUX functionality for SBL accessed peripherals Header file
 *
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */
#ifndef SBL_PINMUX_H
#define SBL_PINMUX_H

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>

/* R5 RBL Include Files */
#include <ti/csl/csl.h>

#ifdef __cplusplus
extern "C" {
#endif


extern void SBL_UARTAPinmuxConfig(void);


extern void SBL_QSPIPinmuxConfig(void);


#ifdef __cplusplus
}
#endif

#endif // DRV_PINMUX_H
