/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
  *  \brief This file contains initialization of wakeup and main PSC
  *  configuration structures and function definitions to get the number
  *  of wakeup and main PSC config exists.
  */

/**
 * \brief wkup PSC configuration parameters
 *
 *  This structure provides the device-level view with module association to
 *  the clock, power, and voltage domains.
 *
 *  The PSC provides the user with an interface to control several important
 *  power and clock operations. The device has two PSC - WKUP_PSC0 and PSC0
 *  in WKUPSS and MAIN SoC, respectively.
 *
 *  PSC: The Power Sleep Controller is the device has several power domains
 *  that can be turned ON for operation or OFF to minimize power dissipation,
 *  which includes a Global Power Sleep Controller(GPSC) and Local Power
 *  Sleep Controller(LPSC).
 *
 *  GPSC: Global Power Sleep Controller, is used to control the power gating
 *  of various power domains.
 *
 *  LPSC: Local Power Sleep Controller, manages the clock gating for to each
 *  logic block. For modules with a dedicated clock or multiple clocks, the
 *  LPSC communicates with the PLL controller to enable and disable that
 *  module's clock(s) at the source. For modules that share a clock with
 *  other modules, the LPSC controls the clock gating logic for each module.
 */

#include "board_clock.h"

#define MSS_CR5_DIV_VAL_CLKDIV          CSL_MSS_TOPRCM_MSS_CR5_DIV_VAL
#define MSS_CR5_CLK_SRC_SEL_CLKSRCSEL   CSL_MSS_TOPRCM_MSS_CR5_CLK_SRC_SEL
#define SYS_CLK_DIV_VAL_CLKDIV          CSL_MSS_TOPRCM_SYS_CLK_DIV_VAL
#define CSIRX_CLK_SRC_SEL               CSL_MSS_TOPRCM_CSIRX_CLK_SRC_SEL
#define CSIRX_DIV_VAL                   CSL_MSS_TOPRCM_CSIRX_DIV_VAL
#define MSS_MCANA_CLK_SRC_SEL           CSL_MSS_RCM_MSS_MCANA_CLK_SRC_SEL
#define MSS_MCANB_CLK_SRC_SEL           CSL_MSS_RCM_MSS_MCANB_CLK_SRC_SEL
#define MSS_RTIA_CLK_SRC_SEL            CSL_MSS_RCM_MSS_RTIA_CLK_SRC_SEL
#define MSS_RTIB_CLK_SRC_SEL            CSL_MSS_RCM_MSS_RTIB_CLK_SRC_SEL
#define MSS_RTIC_CLK_SRC_SEL            CSL_MSS_RCM_MSS_RTIC_CLK_SRC_SEL
#define MSS_WDT_CLK_SRC_SEL             CSL_MSS_RCM_MSS_WDT_CLK_SRC_SEL
#define MSS_QSPI_CLK_SRC_SEL            CSL_MSS_RCM_MSS_QSPI_CLK_SRC_SEL
#define MSS_SPIA_CLK_SRC_SEL            CSL_MSS_RCM_MSS_SPIA_CLK_SRC_SEL
#define MSS_SPIB_CLK_SRC_SEL            CSL_MSS_RCM_MSS_SPIB_CLK_SRC_SEL
#define MSS_I2C_CLK_SRC_SEL             CSL_MSS_RCM_MSS_I2C_CLK_SRC_SEL
#define MSS_SCIA_CLK_SRC_SEL            CSL_MSS_RCM_MSS_SCIA_CLK_SRC_SEL
#define MSS_SCIB_CLK_SRC_SEL            CSL_MSS_RCM_MSS_SCIB_CLK_SRC_SEL
#define MSS_CPSW_CLK_SRC_SEL            CSL_MSS_RCM_MSS_CPSW_CLK_SRC_SEL
#define MSS_CPTS_CLK_SRC_SEL            CSL_MSS_RCM_MSS_CPTS_CLK_SRC_SEL

#define MSS_MCANA_CLK_DIV_VAL           CSL_MSS_RCM_MSS_MCANA_CLK_DIV_VAL
#define MSS_MCANB_CLK_DIV_VAL           CSL_MSS_RCM_MSS_MCANB_CLK_DIV_VAL
#define MSS_RTIA_CLK_DIV_VAL            CSL_MSS_RCM_MSS_RTIA_CLK_DIV_VAL
#define MSS_RTIB_CLK_DIV_VAL            CSL_MSS_RCM_MSS_RTIB_CLK_DIV_VAL
#define MSS_RTIC_CLK_DIV_VAL            CSL_MSS_RCM_MSS_RTIC_CLK_DIV_VAL
#define MSS_WDT_CLK_DIV_VAL             CSL_MSS_RCM_MSS_WDT_CLK_DIV_VAL
#define MSS_QSPI_CLK_DIV_VAL            CSL_MSS_RCM_MSS_QSPI_CLK_DIV_VAL
#define MSS_SPIA_CLK_DIV_VAL            CSL_MSS_RCM_MSS_SPIA_CLK_DIV_VAL
#define MSS_SPIB_CLK_DIV_VAL            CSL_MSS_RCM_MSS_SPIB_CLK_DIV_VAL
#define MSS_I2C_CLK_DIV_VAL             CSL_MSS_RCM_MSS_I2C_CLK_DIV_VAL
#define MSS_SCIA_CLK_DIV_VAL            CSL_MSS_RCM_MSS_SCIA_CLK_DIV_VAL
#define MSS_SCIB_CLK_DIV_VAL            CSL_MSS_RCM_MSS_SCIB_CLK_DIV_VAL
#define MSS_CPSW_CLK_DIV_VAL            CSL_MSS_RCM_MSS_CPSW_CLK_DIV_VAL
#define MSS_CPTS_CLK_DIV_VAL            CSL_MSS_RCM_MSS_CPTS_CLK_DIV_VAL

#define DSS_DSP_CLK_SRC_SEL             CSL_DSS_RCM_DSS_DSP_CLK_SRC_SEL
#define DSS_RTIA_CLK_SRC_SEL            CSL_DSS_RCM_DSS_RTIA_CLK_SRC_SEL
#define DSS_RTIB_CLK_SRC_SEL            CSL_DSS_RCM_DSS_RTIB_CLK_SRC_SEL
#define DSS_WDT_CLK_SRC_SEL             CSL_DSS_RCM_DSS_WDT_CLK_SRC_SEL
#define DSS_SCIA_CLK_SRC_SEL            CSL_DSS_RCM_DSS_SCIA_CLK_SRC_SEL

#define DSS_DSP_CLK_DIV_VAL             CSL_DSS_RCM_DSS_DSP_CLK_DIV_VAL
#define DSS_RTIA_CLK_DIV_VAL            CSL_DSS_RCM_DSS_RTIA_CLK_DIV_VAL
#define DSS_RTIB_CLK_DIV_VAL            CSL_DSS_RCM_DSS_RTIB_CLK_DIV_VAL
#define DSS_WDT_CLK_DIV_VAL             CSL_DSS_RCM_DSS_WDT_CLK_DIV_VAL
#define DSS_SCIA_CLK_DIV_VAL            CSL_DSS_RCM_DSS_SCIA_CLK_DIV_VAL

#define RCSS_SCIA_CLK_SRC_SEL           CSL_RCSS_RCM_RCSS_SCIA_CLK_SRC_SEL
#define RCSS_SPIA_CLK_SRC_SEL           CSL_RCSS_RCM_RCSS_SPIA_CLK_SRC_SEL
#define RCSS_SPIB_CLK_SRC_SEL           CSL_RCSS_RCM_RCSS_SPIB_CLK_SRC_SEL
#define RCSS_I2CA_CLK_SRC_SEL           CSL_RCSS_RCM_RCSS_I2CA_CLK_SRC_SEL
#define RCSS_I2CB_CLK_SRC_SEL           CSL_RCSS_RCM_RCSS_I2CB_CLK_SRC_SEL
#define RCSS_ATL_CLK_SRC_SEL            CSL_RCSS_RCM_RCSS_ATL_CLK_SRC_SEL
#define RCSS_MCASPA_REF0_CLK_SRC_SEL    CSL_RCSS_RCM_RCSS_MCASPA_REF0_CLK_SRC_SEL
#define RCSS_MCASPA_REF1_CLK_SRC_SEL    CSL_RCSS_RCM_RCSS_MCASPA_REF1_CLK_SRC_SEL
#define RCSS_MCASPA_AUX_CLK_SRC_SEL     CSL_RCSS_RCM_RCSS_MCASPA_AUX_CLK_SRC_SEL
#define RCSS_MCASPB_REF0_CLK_SRC_SEL    CSL_RCSS_RCM_RCSS_MCASPB_REF0_CLK_SRC_SEL
#define RCSS_MCASPB_REF1_CLK_SRC_SEL    CSL_RCSS_RCM_RCSS_MCASPB_REF1_CLK_SRC_SEL
#define RCSS_MCASPB_AUX_CLK_SRC_SEL     CSL_RCSS_RCM_RCSS_MCASPB_AUX_CLK_SRC_SEL
#define RCSS_MCASPC_REF0_CLK_SRC_SEL    CSL_RCSS_RCM_RCSS_MCASPC_REF0_CLK_SRC_SEL
#define RCSS_MCASPC_REF1_CLK_SRC_SEL    CSL_RCSS_RCM_RCSS_MCASPC_REF1_CLK_SRC_SEL
#define RCSS_MCASPC_AUX_CLK_SRC_SEL     CSL_RCSS_RCM_RCSS_MCASPC_AUX_CLK_SRC_SEL

#define RCSS_SCIA_CLK_DIV_VAL           CSL_RCSS_RCM_RCSS_SCIA_CLK_DIV_VAL
#define RCSS_SPIA_CLK_DIV_VAL           CSL_RCSS_RCM_RCSS_SPIA_CLK_DIV_VAL
#define RCSS_SPIB_CLK_DIV_VAL           CSL_RCSS_RCM_RCSS_SPIB_CLK_DIV_VAL
#define RCSS_I2CA_CLK_DIV_VAL           CSL_RCSS_RCM_RCSS_I2CA_CLK_DIV_VAL
#define RCSS_I2CB_CLK_DIV_VAL           CSL_RCSS_RCM_RCSS_I2CB_CLK_DIV_VAL
#define RCSS_ATL_CLK_DIV_VAL            CSL_RCSS_RCM_RCSS_ATL_CLK_DIV_VAL
#define RCSS_MCASPA_REF0_CLK_DIV_VAL    CSL_RCSS_RCM_RCSS_MCASPA_REF0_CLK_DIV_VAL
#define RCSS_MCASPA_REF1_CLK_DIV_VAL    CSL_RCSS_RCM_RCSS_MCASPA_REF1_CLK_DIV_VAL
#define RCSS_MCASPA_AUX_CLK_DIV_VAL     CSL_RCSS_RCM_RCSS_MCASPA_AUX_CLK_DIV_VAL
#define RCSS_MCASPB_REF0_CLK_DIV_VAL    CSL_RCSS_RCM_RCSS_MCASPB_REF0_CLK_DIV_VAL
#define RCSS_MCASPB_REF1_CLK_DIV_VAL    CSL_RCSS_RCM_RCSS_MCASPB_REF1_CLK_DIV_VAL
#define RCSS_MCASPB_AUX_CLK_DIV_VAL     CSL_RCSS_RCM_RCSS_MCASPB_AUX_CLK_DIV_VAL
#define RCSS_MCASPC_REF0_CLK_DIV_VAL    CSL_RCSS_RCM_RCSS_MCASPC_REF0_CLK_DIV_VAL
#define RCSS_MCASPC_REF1_CLK_DIV_VAL    CSL_RCSS_RCM_RCSS_MCASPC_REF1_CLK_DIV_VAL
#define RCSS_MCASPC_AUX_CLK_DIV_VAL     CSL_RCSS_RCM_RCSS_MCASPC_AUX_CLK_DIV_VAL

/**
 * \brief clock Initialization function
 *
 * Enables different power domains and peripheral clocks of the SoC.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization sucessful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
Board_STATUS Board_moduleClockInit(void)
{
    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+MSS_CR5_DIV_VAL_CLKDIV, 0x000);  //MSS_CR5_DIV_VAL_CLKDIV = 0x111;
    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+MSS_CR5_CLK_SRC_SEL_CLKSRCSEL, 0x222);  //MSS_CR5_CLK_SRC_SEL_CLKSRCSEL = 0x222

    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+SYS_CLK_DIV_VAL_CLKDIV, 0x111);  //Div by 2

    /* Generate 96 MHz CSIRX Control Clock */
    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+CSIRX_CLK_SRC_SEL, 0x222);
    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+CSIRX_DIV_VAL, 0x000);

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANA_CLK_DIV_VAL, 0x444);  //MSS_MCANA_CLK_DIV_VAL_CLKDIVR   = 0x444; 400/5
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANA_CLK_SRC_SEL, 0x444);  //MSS_MCANA_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANB_CLK_DIV_VAL, 0x444);  //MSS_MCANB_CLK_DIV_VAL_CLKDIVR   = 0x444; 
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANB_CLK_SRC_SEL, 0x444);  //MSS_MCANB_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIA_CLK_DIV_VAL, 0x000);  //MSS_RTIA_CLK_DIV_VAL_CLKDIVR   = 0x444; 200/1
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIA_CLK_SRC_SEL, 0x222);  //MSS_RTIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIB_CLK_DIV_VAL, 0x000);  //MSS_RTIB_CLK_DIV_VAL_CLKDIVR   = 0x444;
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIB_CLK_SRC_SEL, 0x222);  //MSS_RTIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIC_CLK_DIV_VAL, 0x000);  //MSS_RTIC_CLK_DIV_VAL_CLKDIVR   = 0x444;
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIC_CLK_SRC_SEL, 0x222);  //MSS_RTIC_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_WDT_CLK_DIV_VAL, 0x000);  //MSS_WDT_CLK_DIV_VAL_CLKDIVR   = 0x444;
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_WDT_CLK_SRC_SEL, 0x222);  //MSS_WDT_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_QSPI_CLK_DIV_VAL, 0x444);  //MSS_QSPI_CLK_DIV_VAL_CLKDIVR   = 0x444; 400/5
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_QSPI_CLK_SRC_SEL, 0x444);  //MSS_QSPI_CLK_SRC_SEL_CLKSRCSEL = 0x444;

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIA_CLK_DIV_VAL, 0x000);  //MSS_SPIA_CLK_DIV_VAL_CLKDIVR = 0x000;   200/1
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIB_CLK_DIV_VAL, 0x000);  //MSS_SPIB_CLK_DIV_VAL_CLKDIVR = 0x000;
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIA_CLK_SRC_SEL, 0x222);  //MSS_SPIA_CLK_SRC_SEL_CLKSRCSEL = 0x222; 
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIB_CLK_SRC_SEL, 0x222);  //MSS_SPIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_I2C_CLK_DIV_VAL, 0x000);  //MSS_I2C_CLK_DIV_VAL_CLKDIVR   = 0x000; 200/1
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_I2C_CLK_SRC_SEL, 0x222);  //MSS_I2C_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIA_CLK_DIV_VAL, 0x000);  //MSS_SCIA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIB_CLK_DIV_VAL, 0x000);  //MSS_SCIB_CLK_DIV_VAL_CLKDIVR = 0x000;
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIA_CLK_SRC_SEL, 0x222);  //MSS_SCIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIB_CLK_SRC_SEL, 0x222);  //MSS_SCIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPSW_CLK_DIV_VAL, 0x111);  //MSS_CPSW_CLK_DIV_VAL_CLKDIVR   = 0x000; 400/2
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPSW_CLK_SRC_SEL, 0x444);  //MSS_CPSW_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPTS_CLK_DIV_VAL, 0x111);  //MSS_CPTS_CLK_DIV_VAL_CLKDIVR   = 0x000; 400/2
    HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPTS_CLK_SRC_SEL, 0x444);  //MSS_CPTS_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    /* add more modules, add DSP and RCSS */
    HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_DSP_CLK_DIV_VAL, 0x000);  //DSS_WDT_CLK_DIV_VAL_CLKDIVR   = 0x000;  500/1
    HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_DSP_CLK_SRC_SEL, 0x222);  //DSS_WDT_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIA_CLK_DIV_VAL, 0x000);  //DSS_RTIA_CLK_DIV_VAL_CLKDIVR   = 0x444; 200/1
    HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIA_CLK_SRC_SEL, 0x222);  //DSS_RTIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIB_CLK_DIV_VAL, 0x000);  //DSS_RTIB_CLK_DIV_VAL_CLKDIVR   = 0x444;
    HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIB_CLK_SRC_SEL, 0x222);  //DSS_RTIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_WDT_CLK_DIV_VAL, 0x000);  //DSS_WDT_CLK_DIV_VAL_CLKDIVR   = 0x444;
    HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_WDT_CLK_SRC_SEL, 0x222);  //DSS_WDT_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_SCIA_CLK_DIV_VAL, 0x000);  //DSS_SCIA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_SCIA_CLK_SRC_SEL, 0x222);  //DSS_SCIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SCIA_CLK_DIV_VAL, 0x000);  //RCSS_SCIA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SCIA_CLK_SRC_SEL, 0x222);  //RCSS_SCIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SPIA_CLK_DIV_VAL, 0x000);  //RCSS_SPIA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SPIA_CLK_SRC_SEL, 0x222);  //RCSS_SPIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SPIB_CLK_DIV_VAL, 0x000);  //RCSS_SPIB_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SPIB_CLK_SRC_SEL, 0x222);  //RCSS_SPIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_I2CA_CLK_DIV_VAL, 0x000);  //RCSS_I2CA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_I2CA_CLK_SRC_SEL, 0x222);  //RCSS_I2CA_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_I2CB_CLK_DIV_VAL, 0x000);  //RCSS_I2CB_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_I2CB_CLK_SRC_SEL, 0x222);  //RCSS_I2CB_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_ATL_CLK_DIV_VAL, 0x111);   //RCSS_ATL_CLK_DIV_VAL_CLKDIVR = 0x000; 240/2
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_ATL_CLK_SRC_SEL, 0x666);   //RCSS_ATL_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_AUX_CLK_DIV_VAL, 0x111);  //RCSS_MCASPA_AUX_CLK_DIV_VAL_CLKDIVR = 0x111; 192/2
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_AUX_CLK_SRC_SEL, 0x222);  //RCSS_MCASPA_AUX_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_AUX_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_AUX_CLK_DIV_VAL_CLKDIVR = 0x111; 192/2
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_AUX_CLK_SRC_SEL, 0x222);  //RCSS_MCASPB_AUX_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_AUX_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_AUX_CLK_DIV_VAL_CLKDIVR = 0x111; 192/2
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_AUX_CLK_SRC_SEL, 0x222);  //RCSS_MCASPB_AUX_CLK_SRC_SEL_CLKSRCSEL = 0x222;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_REF0_CLK_DIV_VAL, 0x111);  //RCSS_MCASPA_REF0_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_REF0_CLK_SRC_SEL, 0x111);  //RCSS_MCASPA_REF0_CLK_SRC_SEL_CLKSRCSEL = 0x111;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_REF0_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_REF0_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_REF0_CLK_SRC_SEL, 0x111);  //RCSS_MCASPB_REF0_CLK_SRC_SEL_CLKSRCSEL = 0x111;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_REF0_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_REF0_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_REF0_CLK_SRC_SEL, 0x111);  //RCSS_MCASPB_REF0_CLK_SRC_SEL_CLKSRCSEL = 0x111;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_REF1_CLK_DIV_VAL, 0x111);  //RCSS_MCASPA_REF1_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_REF1_CLK_SRC_SEL, 0x111);  //RCSS_MCASPA_REF1_CLK_SRC_SEL_CLKSRCSEL = 0x111;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_REF1_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_REF1_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_REF1_CLK_SRC_SEL, 0x111);  //RCSS_MCASPB_REF1_CLK_SRC_SEL_CLKSRCSEL = 0x111;

    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_REF1_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_REF1_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_REF1_CLK_SRC_SEL, 0x111);  //RCSS_MCASPB_REF1_CLK_SRC_SEL_CLKSRCSEL = 0x111;

    return BOARD_SOK;
}

