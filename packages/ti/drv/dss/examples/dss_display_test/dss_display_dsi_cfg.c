/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2022
 *  All rights reserved.
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
 *  \file dss_display_test.c
 *
 *  \brief DSS sample application that displays two ARGB32 buffers.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/board/src/devices/board_devices.h>
#include <ti/board/board.h>
#include <ti/board/board_cfg.h>
#if defined (SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_i2c_io_exp.h>
#endif
#if defined (SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_control.h>
#endif
#if defined (SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_control.h>
#endif
#include <dss_display_test.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DISP_APP_I2C_TIMEOUT 1000U

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

extern void App_wait(uint32_t wait_in_ms);
int32_t DispApp_SetBoardMux();
int32_t DispApp_InitI2c(DispApp_Obj *appObj);


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

I2C_Handle gI2cHandle = NULL;

/* Bridge configuration array for 800x600 resolution.
 * Tupples represent {Address, Value}.
 */
uint8_t gI2cDsiBridgeCfg[][2] = {
    {0xFF, 0x7},
    {0x16, 0x1},
    {0xFF, 0x0},
    {0x0A, 0x2},
    {0x10, 0x36},
    {0x12, 0x2B},
    {0x13, 0x2B},
    {0x94, 0x20},
    {0x0D, 0x1},
    {0x5A, 0x4},
    {0x93, 0x10},
    {0x96, 0x0A},
    {0x20, 0x20},
    {0x21, 0x03},
    {0x22, 0x0},
    {0x23, 0x0},
    {0x24, 0x58},
    {0x25, 0x02},
    {0x2C, 0x48},
    {0x2D, 0x00},
    {0x30, 0x02},
    {0x31, 0x00},
    {0x34, 0x80},
    {0x36, 0x16},
    {0x38, 0x18},
    {0x3A, 0x01},
    {0x5B, 0x0},
    {0x3C, 0x02},
    {0x5A, 0x0C},
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


uint8_t serdesConfig[][4] = {
{0x16, 0x01, 0x0A, 0x5},
{0x16, 0x03, 0x9A, 0x5},
{0x16, 0x17, 0x9E, 0x5},
{0x16, 0x07, 0x58, 0x5},
{0x16, 0x08, 0x22, 0x5},
{0x16, 0x70, 0x80, 0x5},
{0x16, 0x77, 0x24, 0x5},


{0x16, 0x01, 0x08, 0x5},
{0x16, 0x1E, 0x01, 0x5},
{0x16, 0x03, 0x9A, 0x5},
{0x16, 0x03, 0x9A, 0x5},
{0x16, 0x03, 0x9A, 0x5},
{0x16, 0x40, 0x04, 0x5},
{0x16, 0x40, 0x05, 0x5},
{0x16, 0x41, 0x21, 0x5},
{0x16, 0x42, 0x60, 0x5},
{0x16, 0x40, 0x09, 0x5},
{0x16, 0x40, 0x09, 0x5},
{0x16, 0x41, 0x21, 0x5},
{0x16, 0x42, 0x60, 0x5},
{0x16, 0x5b, 0x85, 0x5},
{0x16, 0x4f, 0x8c, 0x5},
{0x16, 0x4f, 0x84, 0x5},
{0x16, 0x40, 0x05, 0x5},
{0x16, 0x40, 0x04, 0x5},
{0x16, 0x41, 0x05, 0x5},
{0x16, 0x42, 0x16, 0x5},
{0x16, 0x40, 0x08, 0x5},
{0x16, 0x40, 0x08, 0x5},
{0x16, 0x41, 0x05, 0x5},
{0x16, 0x42, 0x0c, 0x5},
{0x16, 0x01, 0x00, 0x5},
{0x16, 0x66, 0x03, 0x5},
{0x16, 0x67, 0x03, 0x5},
{0x16, 0x65, 0x01, 0x5},
{0x16, 0x64, 0x00, 0x5},
{0x16, 0x64, 0x04, 0x5},


{0x27, 0x00, 0xFE, 0x5},

{0x11, 0x1D, 0x28, 0x5},
{0x11, 0x1D, 0x29, 0x5},

{0x11, 0x01, 0x06, 0x5},
{0x11, 0x01, 0x04, 0x5},
{0x11, 0x03, 0xf0, 0x5},
{0x11, 0x03, 0xf0, 0x5},
{0x11, 0x03, 0xf8, 0x5},
{0x11, 0x29, 0x00, 0x5},
{0x11, 0x29, 0x00, 0x5},
{0x11, 0x65, 0x00, 0x5},
{0x11, 0x65, 0x00, 0x5},

{0x12, 0x0C, 0x20, 0x5},
{0x12, 0x00, 0x01, 0x5},
{0x12, 0x04, 0xE6, 0x5},

};

int32_t DispApp_SetBoardMux()
{
#if defined (SOC_J721E)
    Board_I2cInitCfg_t i2cCfg;

    /*setting power mux for dsi lcd*/
    i2cCfg.i2cInst   = BOARD_I2C_IOEXP_DEVICE4_INSTANCE;
    i2cCfg.socDomain = BOARD_SOC_DOMAIN_MAIN;
    i2cCfg.enableIntr = false;
    Board_setI2cInitConfig(&i2cCfg);

    /* Enable DSI in IO Expander */
    Board_i2cIoExpInit();

    /* Enable Reset for external Power */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                                  ONE_PORT_IOEXP,
                                  PORTNUM_0,
                                  PIN_NUM_7,
                                  PIN_DIRECTION_OUTPUT);
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                              ONE_PORT_IOEXP,
                              PORTNUM_0,
                              PIN_NUM_7,
                              GPIO_SIGNAL_LEVEL_HIGH);

    /* Enable Power Switch control */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                                  ONE_PORT_IOEXP,
                                  PORTNUM_0,
                                  PIN_NUM_2,
                                  PIN_DIRECTION_OUTPUT);

    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                              ONE_PORT_IOEXP,
                              PORTNUM_0,
                              PIN_NUM_2,
                              GPIO_SIGNAL_LEVEL_HIGH);
    Board_i2cIoExpDeInit();
#endif
    return (FVID2_SOK);
}

void DispApp_ErrorRegRead(void)
{
    int i=0;
    uint8_t readVal;
    I2C_Params i2cParams;
    I2C_Handle i2cHandle = NULL;
    I2C_init();
    I2C_Params_init(&i2cParams);
    i2cHandle = I2C_open(4, &i2cParams);

    /* Bridge status registers range from address 0xF0 to 0xFF. */
    printf("\n-----------STARTING REGISTER DUMP-----------\n");
    for(i = 0xF0; i < 0xFF; i++)
    {
        Board_i2c8BitRegRd(i2cHandle,
        0x2c,
        i & 0xFF,
        &readVal,
        1,
        DISP_APP_I2C_TIMEOUT);
        printf("Data at 0x%x is 0x%x...\n", i, readVal);
    }
}

int32_t DispApp_InitI2c(DispApp_Obj *appObj)
{
    int32_t status = FVID2_SOK;

#if defined (SOC_J721E)
    uint32_t loopCnt;
    uint8_t domain, i2cInst, slaveAddr;
    I2C_Params i2cParams;
    I2C_HwAttrs i2cConfig;

    /* Initialize I2C Driver */
    for(loopCnt = 0; loopCnt < I2C_HWIP_MAX_CNT; loopCnt++)
    {
        I2C_socGetInitCfg(loopCnt, &i2cConfig);
        i2cConfig.enableIntr = false;
        I2C_socSetInitCfg(loopCnt, &i2cConfig);
    }

    /* Initializes the I2C */
    I2C_init();

    /* Initializes the I2C Parameters */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz; /* 400KHz */

    Board_fpdUb941GetI2CAddr(&domain, &i2cInst, &slaveAddr);

    /* Configures the I2C instance with the passed parameters*/
    gI2cHandle = I2C_open(i2cInst, &i2cParams);
    if(gI2cHandle == NULL)
    {
        App_print("\nI2C Open failed!\n");
        status = FVID2_EFAIL;
    }
#elif defined (SOC_J721S2) || defined(SOC_J784S4)
    uint32_t i=0;
    I2C_Params i2cParams;
    I2C_Handle i2cHandle = NULL;

    /* Enable the DSI to eDP bridge. */
    Board_control(BOARD_CTRL_CMD_ENABLE_DSI2DP_BRIDGE, NULL);
    Osal_delay(1000);
    App_print("DSI to EDP bridge is enabled...\n");

    /* Do the i2c driver init */
    I2C_init();

    /* Initializes the I2C Parameters */
    I2C_Params_init(&i2cParams);
    i2cHandle = I2C_open(4, &i2cParams);
    if(i2cHandle == NULL)
    {
        printf("\nI2C Open failed!\n");
        return -1;
    }

    uint8_t readVal;
    for(i=0; i<sizeof(gI2cDsiBridgeCfg)/sizeof(gI2cDsiBridgeCfg[0]); i++)
    {
        /* Read the value at bridge address. */
        Board_i2c8BitRegRd(i2cHandle,
                            0x2c,
                            gI2cDsiBridgeCfg[i][0] & 0xFF,
                            &readVal,
                            1,
                            DISP_APP_I2C_TIMEOUT);
        Osal_delay(100);
        printf("Before Write operation, data at 0x%x is 0x%x...\n",gI2cDsiBridgeCfg[i][0], readVal);
        printf("Writing at 0x%x the value of 0x%x...\n", gI2cDsiBridgeCfg[i][0], gI2cDsiBridgeCfg[i][1]);

        /* Write the required value at bridge address to configure the bridge.. */
        Board_i2c8BitRegWr(i2cHandle,
                            0x2c,
                            gI2cDsiBridgeCfg[i][0] & 0xFF,
                            &(gI2cDsiBridgeCfg[i][1]),
                            1,
                            DISP_APP_I2C_TIMEOUT);
        Osal_delay(100);

        /* Read back the value after writing at bridge address. */
        Board_i2c8BitRegRd(i2cHandle,
                            0x2c,
                            gI2cDsiBridgeCfg[i][0] & 0xFF,
                            &readVal,
                            1,
                            DISP_APP_I2C_TIMEOUT);
        Osal_delay(100);
        printf("After Write operation, data at 0x%x is 0x%x...\n\n",gI2cDsiBridgeCfg[i][0], readVal);

        /* Wait untill the DP_PLL has been locked. */
        if(gI2cDsiBridgeCfg[i][0] == 0x0d)
        {
            Board_i2c8BitRegRd(i2cHandle,
                            0x2c,
                            0x0a,
                            &readVal,
                            1,
                            DISP_APP_I2C_TIMEOUT);
            while( (readVal & 0x80) == 0x00){
                Board_i2c8BitRegRd(i2cHandle,
                            0x2c,
                            0x0a,
                            &readVal,
                            1,
                            DISP_APP_I2C_TIMEOUT);
            }
        }

        /* Main link should not be off. So keep waiting as long as the main links are off. */
        if(gI2cDsiBridgeCfg[i][0] == 0x96)
        {
            Board_i2c8BitRegRd(i2cHandle,
                            0x2c,
                            0x96,
                            &readVal,
                            1,
                            DISP_APP_I2C_TIMEOUT);
            while( (readVal) == 0x00){
                Board_i2c8BitRegRd(i2cHandle,
                            0x2c,
                            0x96,
                            &readVal,
                            1,
                            DISP_APP_I2C_TIMEOUT);
            }
        }
    }
    
    I2C_close(i2cHandle);
    status = FVID2_SOK;
#endif
    return (status);
}

int32_t DispApp_SetDsiSerdesCfg(DispApp_Obj *appObj)
{
    int32_t status = FVID2_SOK;
    uint32_t cnt, clientAddr;

    status = DispApp_SetBoardMux();

    if (FVID2_SOK == status)
    {
        status = DispApp_InitI2c(appObj);
    }

#if defined (SOC_J721E)
    if (FVID2_SOK == status)
    {
        for (cnt = 0; cnt < sizeof(serdesConfig)/4; cnt ++)
        {
            clientAddr = serdesConfig[cnt][0];
            status = Board_i2c8BitRegWr(
                gI2cHandle, clientAddr, serdesConfig[cnt][1],
                &serdesConfig[cnt][2], 1U, BOARD_I2C_TRANSACTION_TIMEOUT);

            App_wait((uint32_t)serdesConfig[cnt][3]);

            if (0 != status)
            {
                App_print("\n Write Failed for 0x%x 0x%x 0x%x !\n",
                    clientAddr, serdesConfig[cnt][1], serdesConfig[cnt][2]);
                break;
            }
        }
    }

    I2C_close(gI2cHandle);
#endif
    return (status);
}
