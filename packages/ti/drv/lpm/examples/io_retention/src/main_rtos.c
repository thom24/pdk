/*
*
* Copyright (c) 2021 Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <j7vcl_base_address.h>
#include <dmsc.h>
#include <dmsc_cm.h>
#include <io_pm_functions.h>
#include <lpm_mmr_functions.h>
#include <ti/drv/lpm/include/lpm_pmic.h>
#include <ti/drv/lpm/lpm.h>


#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/drv/uart/UART_stdio.h>

#include "board.h"
#include <ti/board/src/j7200_evm/include/board_cfg.h>
#if (defined(SOC_J721E) || defined(SOC_J7200))
#include "board_utils.h"
#endif

#if (defined(SOC_J721E) || defined(SOC_J7200))
#include "diag_common_cfg.h"
#include <ti/csl/soc.h>
#endif

#define PMIC_DEV_HERA_LP8764X  (1U)
#define PMIC_INTF_SINGLE_I2C   (0U)
#define BOARD_DIAG_LP8764X_PMICB_VOLTAGE_VAL     (1700U) /* 1.7V */


#if (defined(SOC_J721E) || defined(SOC_J7200))
Board_I2cInitCfg_t boardI2cInitCfg = {0, BOARD_SOC_DOMAIN_WKUP, false};
#endif

/*********************************  *************************************
 ************************** Internal functions ************************
 **********************************************************************/
int main_io_pm_seq (void);
void ACT_30_10_a ();
 

/* Board specific definitions */
#define I2C_INSTANCE                       (0U)
#define BOARD_NAME_LENGTH                  (16)

/* TPS65941 Register value */
#define TPS65941_PMICA_I2C_SLAVE_ADDR     (0x48U)
#define TPS65941_PMICID_REG               (0x01U)
#define TPS65941_PMIC_REG                 (0x14U)
#define TPS65941_PMIC_VOLTAGE_VAL         (0xAAU)

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/

int BoardDiag_pmic_test()
{
    int ret = 0;
    Board_IDInfo_v2 info = {0};
    int32_t stat = BOARD_SOK;

    Board_setI2cInitConfig(&boardI2cInitCfg);
    stat = Board_getIDInfo_v2(&info, BOARD_I2C_EEPROM_ADDR);
    if(stat == BOARD_INVALID_PARAM)
    {
        stat = BOARD_SOK;
    }

    if(stat == BOARD_SOK)
    {

        ACT_30_10_a ();
        Lpm_pmicInit();

        Lpm_activeToIoRetSwitch();

    }
    return ret;
}

/*----------------------------------------------------------------------------------------------------------*
 * MAIN IO PM daisy chain deepsleep + isolation entry sequence                                              *
 *----------------------------------------------------------------------------------------------------------*/
int main_io_pm_seq (void)
{
    UART_printf("MAIN_CTRL_MMR_BASE+0x1c020 - PADCONF0 0x%x\n", *mkptr(MAIN_CTRL_MMR_BASE, 0x1c020));
    UART_printf("MAIN_CTRL_MMR_BASE+0x1c024 - PADCONF0 0x%x\n", *mkptr(MAIN_CTRL_MMR_BASE, 0x1c024));
    UART_printf("MAIN_CTRL_MMR_BASE+0x1c02c - PADCONF1 0x%x\n",  *mkptr(MAIN_CTRL_MMR_BASE, 0x1c02c));
    UART_printf("MAIN_CTRL_MMR_BASE+0x1c124 - PMIC_WAKE 0x%x\n", *mkptr(MAIN_CTRL_MMR_BASE, 0x1c124));

    UART_printf("dmsc_ssr0_base+(DMSC_PWR_CTRL_OFFSET + DMSC_CM_LOCK0_KICK0) 0x%x set to 0x%x\n", dmsc_ssr0_base+(DMSC_PWR_CTRL_OFFSET + DMSC_CM_LOCK0_KICK0), *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_LOCK0_KICK0)));
    UART_printf("dmsc_ssr0_base+(DMSC_PWR_CTRL_OFFSET + DMSC_CM_LOCK0_KICK1) 0x%x set to 0x%x\n", dmsc_ssr0_base+(DMSC_PWR_CTRL_OFFSET + DMSC_CM_LOCK0_KICK1), *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_LOCK0_KICK1)));

    unsigned int read_data = 0;
    volatile unsigned int i = 0; /* Make sure below for loop is not optimized */

    /* Configure MCAN0 PADCONF */
    *mkptr(MAIN_CTRL_MMR_BASE, 0x1c020) = 0x20050000;
    *mkptr(MAIN_CTRL_MMR_BASE, 0x1c024) = 0x20050000;

    /* Configure MCAN1 PADCONF */
    *mkptr(MAIN_CTRL_MMR_BASE, 0x1c02c) = 0x20050000;

    /* Configure PMIC_WAKE */
    *mkptr(MAIN_CTRL_MMR_BASE, 0x1c124) = 0x38038000;

    sleep(10000);

    /* Unlock DMSC reg */
    *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_LOCK0_KICK0)) = 0x8a6b7cda;
    *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_LOCK0_KICK1)) = 0x823caef9;

    /* Enable fw_ctrl_out[0] (J7VCL Power Mngmt Spec: ACT-25-03) */
    read_data = *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_FW_CTRL_OUT0_SET));
    UART_printf("DMSC_CM_FW_CTRL_OUT0_SET 0x%x\n", read_data);
    *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_FW_CTRL_OUT0_SET)) = 0x2;
    read_data = *mkptr(WKUP_CTRL_MMR_BASE, WKUP_CTRL_DEEPSLEEP_CTRL);
    UART_printf("WKUP_CTRL_DEEPSLEEP_CTRL 0x%x\n", read_data);
    *mkptr(WKUP_CTRL_MMR_BASE, WKUP_CTRL_DEEPSLEEP_CTRL) |= 0x100;

    read_data =  *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_PMCTRL_IO_1));
    UART_printf("DMSC_CM_PMCTRL_IO_1 0x%x\n", read_data);
    *mkptr(mmr0_cfg_base, WKUP_CTRL_CANUART_WAKE_CTRL) = 0x55555554;
    *mkptr(mmr0_cfg_base, WKUP_CTRL_CANUART_WAKE_CTRL) = 0x55555555;
    *mkptr(mmr0_cfg_base, WKUP_CTRL_CANUART_WAKE_CTRL) = 0x55555554;

    /* disable isolation bit */
    *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_PMCTRL_IO_1)) = read_data & ~(0b1 << 24);
    for(i=0; i<io_timeout; i++);

    UART_printf("Polling for IO Status bit\n");
    int cnt = 0;
    while(read_data & 0x2000000)
    {
        read_data =  *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_PMCTRL_IO_1));
        *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_PMCTRL_IO_1)) = read_data & ~(0b1 << 24);
        for(i=0; i<io_timeout; i++);
        if (cnt == 1000)
        {
            UART_printf(".");
        }
    }
    UART_printf("Polling complete - out of isolation mode\n");
    UART_printf("DMSC_CM_PMCTRL_IO_1 after clearing isolation bit: 0x%x\n", read_data);

#ifndef IGNORE_MAIN_IO_PADCONFIG
    /* Enable Wakeup_enable (J7VCL Power Mngmt Spec: ACT-25-04) */
    read_data =  *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_PMCTRL_IO_1));
    UART_printf("DMSC_CM_PMCTRL_IO_1 0x%x\n", read_data);

    *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_PMCTRL_IO_1 )) = read_data | 0x10000;
#endif

    /* Enable ISOIN (J7VCL Power Mngmt Spec: ACT-25-05) */
    read_data =  *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_PMCTRL_IO_1));
    UART_printf("DMSC_CM_PMCTRL_IO_1 0x%x\n", read_data);
    *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_PMCTRL_IO_1)) = read_data | 0x1000000;

    for(i=0; i<io_timeout; i++);    /* Wait for 10us */

    /* Check ISOIN status */
    read_data = *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_PMCTRL_IO_1));

    if (!(read_data & 0x03000000)) {
       return 0;    /* DMSC IO PM seq. failed. */
    }

    if (err_cnt == 0) {
        return 1;   /* DMSC IO PM seq. passed. */
    } else {
        return 0;
    }

}


void ACT_30_10_a ()
{
    uint32_t daisy_chain = main_io_pm_seq();
    if (daisy_chain == 0){
      err_cnt++;
    }
    /* Load the Magic word */
    UART_printf("WKUP_CTRL_CANUART_WAKE_CTRL 0x%x\n", *mkptr(mmr0_cfg_base, WKUP_CTRL_CANUART_WAKE_CTRL));

    *mkptr(mmr0_cfg_base, WKUP_CTRL_CANUART_WAKE_CTRL) = 0x55555554;
    *mkptr(mmr0_cfg_base, WKUP_CTRL_CANUART_WAKE_CTRL) = 0x55555555;

    /* Ensure MW is seen in status register */
    UART_printf("WKUP_CTRL_CANUART_WAKE_STAT0 0x%x\n", *mkptr(mmr0_cfg_base, WKUP_CTRL_CANUART_WAKE_STAT0));

    debug1 = *mkptr(mmr0_cfg_base, WKUP_CTRL_CANUART_WAKE_STAT0);
    if (debug1 != 0x55555555)
        err_cnt++;

    /* Ensure MW is seen in status register */
    UART_printf("WKUP_CTRL_CANUART_WAKE_STAT1 0x%x\n", *mkptr(mmr0_cfg_base, WKUP_CTRL_CANUART_WAKE_STAT1));

    debug1 = *mkptr(mmr0_cfg_base, WKUP_CTRL_CANUART_WAKE_STAT1);
    if (debug1 != 0x1)
        err_cnt++;

    debug1 = 0x3010A;
    /* re-lock DMSC reg */
    *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_LOCK0_KICK0)) = 0x0;
    *mkptr(dmsc_ssr0_base, (DMSC_PWR_CTRL_OFFSET + DMSC_CM_LOCK0_KICK1)) = 0x0;
}


/*
 *  ======== main ========
 */
int main(void)
{
    Board_initCfg boardCfg;
#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_PINMUX_CONFIG | 
        BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG;
#endif
    Board_init(boardCfg);

    mmr_unlock(mmr0_cfg_base, 6);
    mmr_unlock(mmr2_cfg_base, 7);

	return BoardDiag_pmic_test();
}

void AppDelay(uint32_t delayVal)
{
	uint32_t cnt = 0;
    while(cnt < delayVal)
    {
		asm("");
        cnt++;
    }
}
