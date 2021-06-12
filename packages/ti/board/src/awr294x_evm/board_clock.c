/******************************************************************************
 * Copyright (c) 2021 Texas Instruments Incorporated - http://www.ti.com
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
 *
 *****************************************************************************/

 /** \file board_clock.c
  *
  *  \brief AWR294X_EVM Board library clock config file.
  *
  *  PRCM manages clocks, resets, and power domain control of subsystems and
  *  modules inside the device.The Master subsystem TOPRCM module controls all
  *  the Subsystem Resets and Clocks.SubSystem RCM modules control their
  *  respective subsystem modules.
  *
  */

#include "board_clock.h"
#include "board_internal.h"
#include "board_utils.h"

static void Board_moduleClockProgramMode1 (void)
{
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_CR5_DIV_VAL, 0x000);
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_CR5_CLK_SRC_SEL, 0x222);

    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_SYS_CLK_DIV_VAL, 0x111);

    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_HSI_CLK_SRC_SEL_OFFSET, 0x333);
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_HSI_DIV_VAL_OFFSET, 0x000);
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_HSI_CLK_GATE_OFFSET , 0x0);

    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_TRCCLKOUT_DIV_VAL_OFFSET, 0x333);
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_TRCCLKOUT_CLK_SRC_SEL_OFFSET, 0x111);
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_CSIRX_CLK_SRC_SEL_OFFSET, 0x222);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_MCANA_CLK_DIV_VAL_OFFSET, 0x444);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_MCANA_CLK_SRC_SEL_OFFSET, 0x444);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_MCANB_CLK_DIV_VAL_OFFSET, 0x444);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_MCANB_CLK_SRC_SEL_OFFSET, 0x444);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RTIA_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RTIB_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RTIC_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_WDT_CLK_SRC_SEL_OFFSET, 0x222);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_QSPI_CLK_DIV_VAL_OFFSET, 0x444);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_QSPI_CLK_SRC_SEL_OFFSET, 0x444);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_SPIA_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_SPIB_CLK_SRC_SEL_OFFSET, 0x222);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_I2C_CLK_SRC_SEL_OFFSET, 0x222);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_SCIA_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_SCIB_CLK_SRC_SEL_OFFSET, 0x222);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_CPTS_CLK_DIV_VAL_OFFSET, 0x111);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_CPTS_CLK_SRC_SEL_OFFSET, 0x333);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_MSS_MII100_CLK_DIV_VAL_OFFSET, 0x999);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_MSS_MII10_CLK_DIV_VAL_OFFSET, 0x636363);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_MSS_RGMII_CLK_DIV_VAL_OFFSET, 0x111);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_CPSW_CLK_DIV_VAL_OFFSET, 0x111);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_CPSW_CLK_SRC_SEL_OFFSET, 0x444);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_HSM_RTIA_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_HSM_WDT_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_HSM_RTC_CLK_SRC_SEL_OFFSET, 0x777);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_HSM_DMTA_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_HSM_DMTB_CLK_SRC_SEL_OFFSET, 0x222);

    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_DSP_CLK_DIV_VAL, 0x0);
    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_DSP_CLK_SRC_SEL, 0x222);

    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_RCM_DSS_HWA_CLK_SRC_SEL_OFFSET, 0x000);
    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_RTIA_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_RTIB_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_WDT_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_SCIA_CLK_SRC_SEL_OFFSET, 0x222);

    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_RSS_CLK_SRC_SEL_OFFSET, 0x666);
}

static void Board_moduleClockProgramMode2 (void)
{
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_CR5_DIV_VAL, 0x000);
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_CR5_CLK_SRC_SEL, 0x222);

    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_SYS_CLK_DIV_VAL, 0x111);

    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_HSI_CLK_SRC_SEL_OFFSET, 0x333);
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_HSI_DIV_VAL_OFFSET, 0x000);
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_HSI_CLK_GATE_OFFSET, 0x0);

    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_TRCCLKOUT_DIV_VAL_OFFSET, 0x333);
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_TRCCLKOUT_CLK_SRC_SEL_OFFSET, 0x111);
    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_CSIRX_CLK_SRC_SEL_OFFSET, 0x222);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_MCANA_CLK_DIV_VAL_OFFSET, 0x444);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_MCANA_CLK_SRC_SEL_OFFSET, 0x444);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_MCANB_CLK_DIV_VAL_OFFSET, 0x444);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_MCANB_CLK_SRC_SEL_OFFSET, 0x444);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RTIA_CLK_SRC_SEL_OFFSET, 0x333);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RTIB_CLK_SRC_SEL_OFFSET, 0x333);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RTIC_CLK_SRC_SEL_OFFSET, 0x333);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_WDT_CLK_SRC_SEL_OFFSET, 0x333);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_QSPI_CLK_DIV_VAL_OFFSET, 0x444);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_QSPI_CLK_SRC_SEL_OFFSET, 0x444);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_SPIA_CLK_SRC_SEL_OFFSET, 0x111);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_SPIB_CLK_SRC_SEL_OFFSET, 0x111);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_I2C_CLK_SRC_SEL_OFFSET, 0x333);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_SCIA_CLK_SRC_SEL_OFFSET, 0x333);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_SCIB_CLK_SRC_SEL_OFFSET, 0x333);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_CPTS_CLK_DIV_VAL_OFFSET, 0x111);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_CPTS_CLK_SRC_SEL_OFFSET, 0x333);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_MSS_MII100_CLK_DIV_VAL_OFFSET, 0x999);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_MSS_MII10_CLK_DIV_VAL_OFFSET, 0x636363);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_MSS_RGMII_CLK_DIV_VAL_OFFSET, 0x111);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_CPSW_CLK_DIV_VAL_OFFSET, 0x111);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_CPSW_CLK_SRC_SEL_OFFSET, 0x444);

    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_HSM_RTIA_CLK_SRC_SEL_OFFSET, 0x333);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_HSM_WDT_CLK_SRC_SEL_OFFSET, 0x333);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_HSM_RTC_CLK_SRC_SEL_OFFSET, 0x777);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_HSM_DMTA_CLK_SRC_SEL_OFFSET, 0x333);
    HW_WR_REG32(BOARD_MSS_RCM_U_BASE+BOARD_MSS_RCM_HSM_DMTB_CLK_SRC_SEL_OFFSET, 0x333);

    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_DSP_CLK_DIV_VAL, 0x0);
    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_DSP_CLK_SRC_SEL, 0x222);

    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_RCM_DSS_HWA_CLK_SRC_SEL_OFFSET, 0x000);
    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_RTIA_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_RTIB_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_WDT_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(BOARD_DSS_RCM_U_BASE+BOARD_DSS_SCIA_CLK_SRC_SEL_OFFSET, 0x222);

    HW_WR_REG32(BOARD_MSS_TOPRCM_U_BASE+BOARD_MSS_TOPRCM_RSS_CLK_SRC_SEL_OFFSET , 0x666);
}

/**
 * \brief clock Initialization function
 *
 * Enables the clock for all the modules and subsystems by registering the
 * clock source and clokck divider values into their respective clock control
 * registers.
 *
 * \return  BOARD_SOK              - Clock initialization sucessful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
Board_STATUS Board_moduleClockInit(void)
{
    Board_STATUS status = BOARD_SOK;
    Board_initParams_t boardInitParams;

    Board_getInitParams(&boardInitParams);

    Board_unlockMMR();

    if(boardInitParams.pllMode == BOARD_PLL_MODE2)
    {
        Board_moduleClockProgramMode2();
    }
    else
    {
        Board_moduleClockProgramMode1();
    }

    return status;
}

Board_STATUS Board_aPLLClockSwitch(void)
{
    Board_STATUS status = BOARD_SOK;
    volatile uint32_t bssStatus;
    bssStatus = CSL_FEXT((*(volatile uint32_t *)(CSL_RSS_PROC_CTRL_U_BASE+CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG0)),
                RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG0_RSS_CR4_BOOT_INFO_REG0_CONFIG);
    
    /* Make sure APLL calibration was successful before switching BSS to use APLL */
    if ((bssStatus & 0x20000U) != 0U)
    {
        /*TOPRCM : PLLP_CLK1_SRC_SEL_CLKSRCSEL */
        HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLLP_CLK1_SRC_SEL, 0x111);

        /* TOPRCM: RSS_CLK_SRC_SEL_CLKSRCSEL */
        HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_RSS_CLK_SRC_SEL, 0x333);

        /* TOPRCM: RSS_DIV_VAL */
        HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_RSS_DIV_VAL, 0x0);
        
        /*HSI*/
        {
            /*Enable HSI Clock from Analog */
            /*Configure CBUFF/Aurora*/
            /*Enable the clock to the HSI Clock mux :*/
            /*TOPRCM::PLL_CORE_HSDIVIDER_CLKOUT0 = m;*/
            //MSS_TOPRCM_Ptr->PLL_CORE_HSDIVIDER_CLKOUT0_UN.PLL_CORE_HSDIVIDER_CLKOUT0_UL = 0x101;
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_CORE_HSDIVIDER_CLKOUT0, 0x101);

            /*while (MSS_TOPRCM_Ptr->PLL_CORE_HSDIVIDER_CLKOUT0_UN.PLL_CORE_HSDIVIDER_CLKOUT0_ST.PLL_CORE_HSDIVIDER_CLKOUT0_STATUS != 0x1); */
            while (((*(volatile uint32_t *)(0x0214042C)) & (1U << 9U)) != (1U << 9U));
            /*TOPRCM::HSI_CLK_SRC_SEL = n;*/
            //MSS_TOPRCM_Ptr->HSI_CLK_SRC_SEL_UN.HSI_CLK_SRC_SEL_UL = 0x111;
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_HSI_CLK_SRC_SEL, 0x111);
        }

        /* Switch all the clocks to APLL*/
        /*R5F*/
        if(HW_RD_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLLC_CLK2_SRC_SEL) != 0x111)
        {
            /*Enable 1P2 Clock from Analog*/
            /*TOPRCM::PLL_1P2_HSDIVIDER_CLKOUT0 = n */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER_CLKOUT0, 2);

             /*TOPRCM::PLL_1P2_HSDIVIDER = 4*/
             HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER, 4);

            /*TOPRCM::PLL_1P2_HSDIVIDER = 0*/
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER, 0);

            /*TOPRCM::PLL_1P2_HSDIVIDER_CLKOUT0 = (1 << 8) | n */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER_CLKOUT0, 0x102);
        }

        /*SYS_CLK*/
        {
            /*TOPRCM::PLLC_CLK2_SRC_SEL_CLKSRCSEL = 0x111;*/
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLLC_CLK2_SRC_SEL, 0x111);

            /*TOPRCM::SYS_CLK_DIV_VAL_CLKDIV = 0x111; */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_SYS_CLK_DIV_VAL, 0x111);

            /*TOPRCM::MSS_CR5_CLK_SRC_SEL_CLKSRCSEL     = 0x222*/
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_MSS_CR5_CLK_SRC_SEL, 0x222);
        }

        /*DSP*/
        {
            /*TOPRCM::PLL_1P2_HSDIVIDER_CLKOUT1_UL = n  */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER_CLKOUT1, 2);

            /*TOPRCM::PLL_1P2_HSDIVIDER = 4 */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER, 4);

            /*TOPRCM::PLL_1P2_HSDIVIDER = 0 */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER, 0);

            /*TOPRCM::PLL_1P2_HSDIVIDER_CLKOUT1 = (1 << 8) | n */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER_CLKOUT1, 0x102);
        }

        /*CAN QSPI*/
        {
            /*1. Set the Divider value for PLL_1P2_HSDIV0_CLKOUT2 to generate 1.2ghz/5 = 240mhz
            a.	MSS_TOPRCM::PLL_1P2_HSDIVIDER_CLKOUT2 = 0x4;*/
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER_CLKOUT2, 0x4);

            /*b.	MSS_TOPRCM:: PLL_1P2_HSDIVIDER = 0x4*/
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER, 0x4);

            /*c.	MSS_TOPRCM:: PLL_1P2_HSDIVIDER = 0x1*/
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER, 0x0);

            /*d.	MSS_TOPRCM::PLL_1P2_HSDIVIDER_CLKOUT2 = 0x104;*/
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_1P2_HSDIVIDER_CLKOUT2, 0x104);

            /*5.	Switching  DPLL_DSP_HSDIV0_CLKOUT2 to APLL_1P2_HSDIV0_CLKOUT2 */ 
            /*a.	MSS_TOPRCM::PLLD_CLK2_SRC_SEL = 0x111*/
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLLD_CLK2_SRC_SEL, 0x111);

            /*6.	Setting Dividers for MCANA,MCANB,QSPI to be by 3 to generate 80mhz from 240mhz*/
            /*a.	MSS_RCM::MSS_MCANA_CLK_DIV_VAL = 0x222*/
            HW_WR_REG32(CSL_MSS_RCM_U_BASE + CSL_MSS_RCM_MSS_MCANA_CLK_DIV_VAL, 0x222);

            /*b.	MSS_RCM::MSS_MCANB_CLK_DIV_VAL = 0x222*/
            HW_WR_REG32(CSL_MSS_RCM_U_BASE + CSL_MSS_RCM_MSS_MCANB_CLK_DIV_VAL, 0x222);

            /*c.	MSS_RCM::MSS_QSPI_CLK_DIV_VAL = 0x222*/
            HW_WR_REG32(CSL_MSS_RCM_U_BASE + CSL_MSS_RCM_MSS_QSPI_CLK_DIV_VAL, 0x222);

            /*7.	Switching the MCANA Clock soruce to APLL_1P2_HSDIV0_CLKOUT2  (240 mhz)*/
            /*a.	MSS_RCM::MSS_MCANA_CLK_SRC_SEL = 0x111*/
            HW_WR_REG32(CSL_MSS_RCM_U_BASE + CSL_MSS_RCM_MSS_MCANA_CLK_SRC_SEL, 0x111);

            /*8.	Switching the MCANB Clock source to APLL_1P2_HSDIV0_CLKOUT2 (240mhz)*/
            /*a.	MSS_RCM::MSS_MCANA_CLK_SRC_SEL = 0x111*/
            HW_WR_REG32(CSL_MSS_RCM_U_BASE + CSL_MSS_RCM_MSS_MCANB_CLK_SRC_SEL, 0x111);

            /*9.	Switching the QSPI Clock source to APLL_1P2_HSDIV0_CLKOUT2 (240mhz)*/
            /*a.	MSS_RCM::MSS_QSPI_CLK_SRC_SEL = 0x111*/
            HW_WR_REG32(CSL_MSS_RCM_U_BASE + CSL_MSS_RCM_MSS_QSPI_CLK_SRC_SEL, 0x111);
        }
 
        /*Switch off ADPLL*/
        {
            /*1. Make sure all clocks are switched on in APLL
            2. SWitch off ADPLL (LDOEN = 0, IDLE = 1, CLKOUTEN=0)*/

            /*MSS_TOPRCM_Ptr->PLL_CORE_CLKCTRL_UN.PLL_CORE_CLKCTRL_UL = 0x09831000; */  
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_CORE_CLKCTRL, 0x09831000);

            /*MSS_TOPRCM_Ptr->PLL_CORE_TENABLE_UN.PLL_CORE_TENABLE_UL = 0x1;*/
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_CORE_TENABLE, 0x1);

            /*MSS_TOPRCM_Ptr->PLL_CORE_CLKCTRL_UN.PLL_CORE_CLKCTRL_UL = 0x09831001; */ 
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_CORE_CLKCTRL, 0x09831001);

            /*MSS_TOPRCM_Ptr->PLL_CORE_TENABLE_UN.PLL_CORE_TENABLE_UL = 0x0;      */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_CORE_TENABLE, 0x0);

            /*MSS_TOPRCM_Ptr->PLL_DSP_CLKCTRL_UN.PLL_DSP_CLKCTRL_UL   = 0x09831000;  */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_DSP_CLKCTRL, 0x09831000);

            /*MSS_TOPRCM_Ptr->PLL_DSP_TENABLE_UN.PLL_DSP_TENABLE_UL   = 0x1;  */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_DSP_TENABLE, 0x1);

            /*MSS_TOPRCM_Ptr->PLL_DSP_CLKCTRL_UN.PLL_DSP_CLKCTRL_UL   = 0x09831001;  */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_DSP_CLKCTRL, 0x09831001);

            /*MSS_TOPRCM_Ptr->PLL_DSP_TENABLE_UN.PLL_DSP_TENABLE_UL   = 0x0;      */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_DSP_TENABLE, 0x0);

            /*MSS_TOPRCM_Ptr->PLL_PER_CLKCTRL_UN.PLL_PER_CLKCTRL_UL   = 0x09831000;*/
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_PER_CLKCTRL, 0x09831000);

            /*MSS_TOPRCM_Ptr->PLL_PER_TENABLE_UN.PLL_PER_TENABLE_UL   = 0x1;                  */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_PER_TENABLE, 0x1);

            /*MSS_TOPRCM_Ptr->PLL_PER_CLKCTRL_UN.PLL_PER_CLKCTRL_UL   = 0x09831001;   */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_PER_CLKCTRL, 0x09831001);

            /*MSS_TOPRCM_Ptr->PLL_PER_TENABLE_UN.PLL_PER_TENABLE_UL   = 0x0; */
            HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_PLL_PER_TENABLE, 0x0);
        }
    }

    return status;
}
