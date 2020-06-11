/*
 *  Copyright (c) Texas Instruments Incorporated 2019-2020
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 *  Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the
 *  distribution.
 *
 *  Neither the name of Texas Instruments Incorporated nor the names of
 *  its contributors may be used to endorse or promote products derived
 *  from this software without specific prior written permission.
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

/*
 * main.c
 */
#include <stdint.h>

#ifdef USE_BIOS
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#endif /* #ifdef USE_BIOS */

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>

void Test_DDRTempMonitorCallback(Board_DDRTempEventType DDRTempEventType)
{
    UART_printf("\n Temperature events %d detected:", DDRTempEventType);
    switch(DDRTempEventType)
    {
        case BOARD_DDR_TEMP_EVENT_TEMP_ALERT:
            UART_printf(" Temperature alert\n");
            break;
        case BOARD_DDR_TEMP_EVENT_LOW_TEMP_ALARM:
            UART_printf(" Low temperature alarm\n");
            break;
        case BOARD_DDR_TEMP_EVENT_TEMP_CHANGE_4X_REFRESH:
            UART_printf(" High temperature change, needs 4x refresh\n");
            break;
        case BOARD_DDR_TEMP_EVENT_TEMP_CHANGE_2X_REFRESH:
            UART_printf(" High temperature change, needs 2x refresh\n");
            break;
        case BOARD_DDR_TEMP_EVENT_TEMP_CHANGE_1X_REFRESH:
            UART_printf(" High temperature change, needs 1x refresh\n");
            break;
        case BOARD_DDR_TEMP_EVENT_TEMP_CHANGE_1_BY_2_REFRESH:
            UART_printf(" Low temperature change, needs 0.5x refresh\n");
            break;
        case BOARD_DDR_TEMP_EVENT_TEMP_CHANGE_1_BY_4_REFRESH:
            UART_printf(" Low temperature change, needs 0.25x refresh\n");
            break;
        case BOARD_DDR_TEMP_EVENT_TEMP_CHANGE_1_BY_4_REFRESH_WITH_DERATING:
            UART_printf(" Low temperature change, needs 0.25x refresh with derating\n");
            break;
        case BOARD_DDR_TEMP_EVENT_HIGH_TEMP_ALARM:
            UART_printf(" High temperature alarm\n");
            break;
    }
}
#ifdef USE_BIOS
/*
 *  ======== taskFxn ========
 */
Void taskFxn(UArg a0, UArg a1)
#else
int main(void)
#endif /* #ifdef USE_BIOS */
{
    int32_t       retVal = 0;
    char inputChar;
#ifndef USE_BIOS
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;

    boardCfg = BOARD_INIT_PINMUX_CONFIG
               | BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        retVal = -1;
        UART_printf("[Error] Board init failed!!\n");
    }
#endif /* USE_BIOS */
    if (retVal == 0)
    {
        Board_DDRTempMonitoringInit(&Test_DDRTempMonitorCallback);
    }

    do
    {
        UART_printf("\n Press any key to continue... Press q to quit");
        inputChar = UART_getc();

        if (inputChar == 'q')
        {
            UART_printf("\n Ending test \n");
            break;
        }
    } while(inputChar != 'q');

#ifdef USE_BIOS
    return;
#else
    return retVal;
#endif /* USE_BIOS */
}
#ifdef USE_BIOS
/*
 *  ======== main ========
 */
Int main()
{
    Task_Handle task;
    Error_Block eb;
    Task_Params taskParams;

    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;

    boardCfg = BOARD_INIT_PINMUX_CONFIG
               | BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        UART_printf("[Error] Board init failed!!\n");
        return(-1);
    }

    Error_init(&eb);

    /* Initialize the task params */
    Task_Params_init(&taskParams);

    /* Set the task priority higher than the default priority (1) */
    taskParams.priority = 2;
    taskParams.stackSize = 0x6000;

    task = Task_create(taskFxn, &taskParams, &eb);
    if (task == NULL) {

        BIOS_exit(0);
    }
        BIOS_start();    /* does not return */
    return(0);
}
#endif /* USE_BIOS */
