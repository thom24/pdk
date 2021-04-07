/*
 *  Copyright (c) Texas Instruments Incorporated 2021
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *   \file  main_gpadc_test.c
 *
 *   \brief
 *      Unit Test code for the GPADC Driver which executes on the MSS
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ti/sysbios/BIOS.h"
#include "ti/sysbios/knl/Task.h"
#include <ti/board/board.h>
#include <ti/osal/DebugP.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/gpadc/gpadc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define GPADC_CHANNEL_READ     1U
#define GPADC_GROUP_READ       2U

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t PlatformInit(void);
static int32_t GPADC_Group_Channel_Read(void);
static int32_t GPADC_Single_Channel_Read(uint8_t channelIndex);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile uint32_t       testSelection = 0;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  @b Description
 *  @n
 *      Platform specific intializations.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t PlatformInit(void)
{
    Board_STATUS boardStatus;
    Board_initCfg boardCfg;

    boardCfg = (BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK |
                BOARD_INIT_UART_STDIO);
    boardStatus = Board_init(boardCfg);

    DebugP_assert(boardStatus == BOARD_SOK);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      GPADC Read voltage Test.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t GPADC_Single_Channel_Read(uint8_t channelIndex)
{
	int32_t retVal = E_OK;
	GPADC_ConfigType CfgPtr;
	uint16_t gpadc_result = 0;
	GPADC_ConvResultType convRes = GPADC_CONV_ERROR;

    memset(&CfgPtr,0,sizeof(CfgPtr));

	CfgPtr.triggSrc = GPADC_TRIGG_SRC_SW;
	CfgPtr.convMode = GPADC_ONESHOT_CONV_MODE;

	CfgPtr.channelConfig[0].channelID = (GPADC_MeasExtSrcType)channelIndex;
	CfgPtr.channelConfig[0].isConfigured = TRUE;
	CfgPtr.channelConfig[0].isBufferedMode = FALSE;
	CfgPtr.channelConfig[0].useLuTable = TRUE;

	GPADC_Init(&CfgPtr);
	convRes = GPADC_StartSingleChannelConversion((GPADC_MeasExtSrcType)channelIndex, &gpadc_result);

	if(GPADC_CONV_DONE == convRes)
	{
		UART_printf("Channel adc conversion successful\n");
		retVal = E_OK;
	}
	else if(GPADC_CONV_CHANNEL_CONFIG_MISSING == convRes)
	{
		UART_printf("channel adc configuration missing\n");
		retVal = E_NOT_OK;
	}
	else
	{
		UART_printf("Channel adc conversion error\n");
		retVal = E_NOT_OK;
	}

	UART_printf("GPADC EXT%d  AvgValue %d\n",(channelIndex+1), gpadc_result);

	return retVal;
}

/**
 *  @b Description
 *  @n
 *      GPADC Read voltage Test.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t GPADC_Group_Channel_Read(void)
{
	int32_t retVal = E_OK;
	uint16_t gpadc_result[9];
	GPADC_ConfigType CfgPtr;
	GPADC_channelsGroupSelectType channels;
	uint8_t index =0;
	GPADC_ConvResultType convRes;
	uint32_t channelIndex;


    memset(&CfgPtr,0,sizeof(CfgPtr));
    memset(&gpadc_result[0],0,sizeof(gpadc_result));


	CfgPtr.triggSrc = GPADC_TRIGG_SRC_SW;
	CfgPtr.convMode = GPADC_ONESHOT_CONV_MODE;

	UART_printf("Select Channel Bitmap for Group Read <0x1FF to 0x001>\t");
	UART_scanFmt ("%x",&channelIndex);

	for (index = 0; index < MAX_GPADC_MEAS_SOURCES; index++)
	{
		if(channelIndex | (0x01 << index))
		{
			CfgPtr.channelConfig[index].channelID = (GPADC_MeasExtSrcType)index;
			CfgPtr.channelConfig[index].isConfigured = TRUE;
			CfgPtr.channelConfig[index].isBufferedMode = FALSE;
			CfgPtr.channelConfig[index].useLuTable = TRUE;
		}
	}

	channels.bits.b9_ChannelSelectionBitMap = (channelIndex & 0x1FF);

	GPADC_Init(&CfgPtr);
	GPADC_SetupResultBuffer(&gpadc_result[0]);
	convRes = GPADC_StartGroupConversion(channels);
	if(GPADC_CONV_DONE == convRes)
	{
		UART_printf("Channel adc conversion successful\n");
		retVal = E_OK;
	}
	else if(GPADC_CONV_CHANNEL_CONFIG_MISSING == convRes)
	{
		UART_printf("channel adc configuration missing\n");
		retVal = E_NOT_OK;
	}
	else
	{
		UART_printf("Channel adc conversion error\n");
		retVal = E_NOT_OK;
	}

	for(index=0; index < MAX_GPADC_MEAS_SOURCES; index++)
	{
		UART_printf("GPADC EXT%d  AvgValue %d\n",index+1, gpadc_result[index]);
	}

	return retVal;
}

int32_t main (void)
{
    int32_t retVal = 0;
	uint8_t channelIndex = 0;

    /* Initialize the platform */
    retVal = PlatformInit();
    DebugP_assert(retVal == 0);

    do
    {
    	UART_printf ("*******************************************************\n");
    	UART_printf ("GPADC Unit Test Menu                        \n");
    	UART_printf ("Please select the type of test to execute:  \n");
    	UART_printf ("1. GPADC Single Channel Read                       \n");
    	UART_printf ("2. GPADC Group  Channel Read                       \n");
    	UART_printf ("*******************************************************\n");
    	UART_printf ("> Enter your selection:\t");

    	UART_scanFmt ("%d",&testSelection);

        /* Validate the selection: */
    }while((testSelection < GPADC_CHANNEL_READ) || (testSelection > GPADC_GROUP_READ ));

    if (testSelection == GPADC_CHANNEL_READ)
    {
    	do
    	{
    		UART_printf("Select Channel to be read <1-9>\t");
    		UART_scanFmt ("%d",&channelIndex);
    	}while((channelIndex <= 0) || (channelIndex > 9));

    	channelIndex = channelIndex - 1U;

        retVal = GPADC_Single_Channel_Read(channelIndex);
        if (retVal == E_NOT_OK)
        {
        	UART_printf("GPADC Channel Read testing : Fail\n");
        }
        else
        {
        	UART_printf("GPADC Channel Read testing : Pass\n");
        }
    }
    else if	(testSelection == GPADC_GROUP_READ)
    {
        retVal = GPADC_Group_Channel_Read();
        if (retVal == E_NOT_OK)
        {
        	UART_printf("GPADC Group Read testing : Fail\n");
        }
        else
        {
        	UART_printf("GPADC Group Read testing : Pass\n");
        }
    }
}

/*
 * END OF main_gpadc_test.c FILE
 */