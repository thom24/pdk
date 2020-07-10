/*
 * uart.c
 *
 * UART driver code for R4
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */

/**************************************************************************
 ***************************** Include Files ******************************
 **************************************************************************/
#include "stdint.h"
#include "uart.h"
#include "csl.h"

/**************************************************************************
 ***************************** Function Definitions ***********************
 **************************************************************************/
 
/**
 *  @b Description
 *  @n
 *     Function to initialize and enable uart port
 *
 *  @param[in] baudRateDiv
 *             Baud rate divider value that needs to be programmed to get the 
 *             desired baud rate                    
 *
 *  @retval None
 */
void DrvUart_Open (uint32_t baudRateDiv)
{
    /* reset the module */
    ptrUARTRegs->SCIGCR0 = CSL_insert8 (ptrUARTRegs->SCIGCR0, 0U, 0U, 0x1U);
    
    /* Soft reset */
    ptrUARTRegs->SCIGCR1 = CSL_insert8 (ptrUARTRegs->SCIGCR1, 7U, 7U, 0x0U);
    
    /* transmit enable */
    ptrUARTRegs->SCIGCR1 = CSL_insert8 (ptrUARTRegs->SCIGCR1, 25U, 25U, 0x1U);

    /* Enable Receive */
    ptrUARTRegs->SCIGCR1 = CSL_insert8 (ptrUARTRegs->SCIGCR1, 24U, 24U, 0x1U);

    /* Enable Internal clock */
    ptrUARTRegs->SCIGCR1 = CSL_insert8 (ptrUARTRegs->SCIGCR1, 5U, 5U, 0x1U);

    /* Set Asynchronouse mode */
    ptrUARTRegs->SCIGCR1 = CSL_insert8 (ptrUARTRegs->SCIGCR1, 0U, 0U, 0x0U);

    /* Set Idle-line mode */
    ptrUARTRegs->SCIGCR1 = CSL_insert8 (ptrUARTRegs->SCIGCR1, 1U, 1U, 0x1U);
    
    /* number of stop bits = 1 */
    ptrUARTRegs->SCIGCR1 = CSL_insert8 (ptrUARTRegs->SCIGCR1, 4U, 4U, 0x0U);
    
    /* No parity */
    ptrUARTRegs->SCIGCR1 = CSL_insert8 (ptrUARTRegs->SCIGCR1, 2U, 2U, 0x0U);
    ptrUARTRegs->SCIGCR1 = CSL_insert8 (ptrUARTRegs->SCIGCR1, 3U, 3U, 0x0U);
    
    /* Set Number of Data bits = 8 */
    ptrUARTRegs->SCICHAR = CSL_insert8 (ptrUARTRegs->SCICHAR, 2U, 0U, 0x7U);

    /* Set the Baud rate divider value */
    ptrUARTRegs->SCIBAUD = CSL_insert32(ptrUARTRegs->SCIBAUD, 23U, 0U, (baudRateDiv - 1U));

    /* Set Functional Mode */
    ptrUARTRegs->SCIPIO0 = CSL_insert32(ptrUARTRegs->SCIPIO0, 2U, 0U, 0x6U);
    
    /* Bring UART out of reset */
    ptrUARTRegs->SCIGCR1 = CSL_insert8 (ptrUARTRegs->SCIGCR1, 7U, 7U, 0x1U);
}

/**
 *  @b Description
 *  @n
 *     Function to Write Bytes to the UART port
 *
 *  @param[in] dataBuff
 *             pointer to the data buffer from where the data is to be sent over
 *             the UART port
 *  @param[in] length
 *             number of bytes to be sent 
 *
 *  @retval None
 */
void DrvUart_WriteBytes (uint8_t* dataBuff, uint32_t length)
{
    uint32_t count;
    
    for (count = 0U; count < length ; count++)
    {   
        /* wait for Tx Ready */
        while(CSL_extract8 (ptrUARTRegs->SCIFLR, 8U, 8U) != 1U);
        
        /* write the data */
        ptrUARTRegs->SCITD = CSL_insert8 (ptrUARTRegs->SCITD, 7U, 0U, dataBuff[count]);
    }
}

/**
 *  @b Description
 *  @n
 *     Function to Write Bytes to the UART port
 *
 *  @param[in] dataBuff
 *             pointer to the data buffer where the input data over received
 *             over UART port is to be stored
 *             the UART port
 *  @param[in] length
 *             number of bytes to be read before returning 
 *
 *  @retval None
 */
void DrvUart_ReadBytes (uint8_t* dataBuff, uint32_t length)
{
    uint32_t count;
    for (count = 0U; count < length ; count++)
    {   
        /* wait for Rx Ready */
        while(CSL_extract8 (ptrUARTRegs->SCIFLR, 9U, 9U) != 1U);
        
        /* read the data */
        dataBuff[count] = CSL_extract8 (ptrUARTRegs->SCIRD, 7U, 0U);
    }
}
