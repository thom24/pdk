/*
 * UART Header File
 *
 * This is the header file which exposes the API to configure the UART
 * specific functionality
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */
#ifndef UART_H
#define UART_H

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
/* Standard Include Files. */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief
 *  SCI Register Layer
 *
 * @details
 *  The structure defines the layout of the registers for the SCI module.
 *
 */
typedef volatile struct SCIRegs_t
{
    uint32_t    SCIGCR0                            ;        /**< @brief   Offset: 0x000 */
    uint32_t    SCIGCR1                            ;        /**< @brief   Offset: 0x004 */
    uint32_t    RESTRICTED1                        ;        /**< @brief   Offset: 0x008 */
    uint32_t    SCISETINT                          ;        /**< @brief   Offset: 0x00C */
    uint32_t    SCICLEARINT                        ;        /**< @brief   Offset: 0x010 */
    uint32_t    SCISETINTLVL                       ;        /**< @brief   Offset: 0x014 */
    uint32_t    SCICLEARINTLVL                     ;        /**< @brief   Offset: 0x018 */
    uint32_t    SCIFLR                             ;        /**< @brief   Offset: 0x01C */
    uint32_t    SCIINTVECT0                        ;        /**< @brief   Offset: 0x020 */
    uint32_t    SCIINTVECT1                        ;        /**< @brief   Offset: 0x024 */
    uint32_t    SCICHAR                            ;        /**< @brief   Offset: 0x028 */
    uint32_t    SCIBAUD                            ;        /**< @brief   Offset: 0x02C */
    uint32_t    SCIED                              ;        /**< @brief   Offset: 0x030 */
    uint32_t    SCIRD                              ;        /**< @brief   Offset: 0x034 */
    uint32_t    SCITD                              ;        /**< @brief   Offset: 0x038 */
    uint32_t    SCIPIO0                            ;        /**< @brief   Offset: 0x03C */
    uint32_t    SCIPIO1                            ;        /**< @brief   Offset: 0x040 */
    uint32_t    SCIPIO2                            ;        /**< @brief   Offset: 0x044 */
    uint32_t    SCIPIO3                            ;        /**< @brief   Offset: 0x048 */
    uint32_t    SCIPIO4                            ;        /**< @brief   Offset: 0x04C */
    uint32_t    SCIPIO5                            ;        /**< @brief   Offset: 0x050 */
    uint32_t    SCIPIO6                            ;        /**< @brief   Offset: 0x054 */
    uint32_t    SCIPIO7                            ;        /**< @brief   Offset: 0x058 */
    uint32_t    SCIPIO8                            ;        /**< @brief   Offset: 0x05C */
    uint32_t    RESTRICTED2[8]                     ;        /**< @brief   Offset: 0x060-0x7C */
    uint32_t    SCIPIO9                            ;        /**< @brief   Offset: 0x080 */
    uint32_t    RESTRICTED3[3]                     ;        /**< @brief   Offset: 0x084-0x8c */
    uint32_t    SCIIODCTRL                         ;        /**< @brief   Offset: 0x090 */
}SCIRegs;

/**
 * @brief   This is the base address of the Serial Communication Interface
 * (SCI) module on the TPR12 R4 (TB core). This is applicable for the SCI Instance A
 */
#define R4_SCIA_BASE_ADDRESS    (0xFFF7EC00U)

#define ptrUARTRegs             ((SCIRegs*)R4_SCIA_BASE_ADDRESS)

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/
extern void DrvUart_Open (uint32_t baudRateDiv);
extern void DrvUart_WriteBytes (uint8_t* dataBuff, uint32_t length);
extern void DrvUart_ReadBytes (uint8_t* dataBuff, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* UART_H */

