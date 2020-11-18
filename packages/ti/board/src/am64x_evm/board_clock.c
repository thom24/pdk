/******************************************************************************
 * Copyright (c) 2019-2020 Texas Instruments Incorporated - http://www.ti.com
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

#include "board_clock.h"
#include <ti/drv/sciclient/sciclient.h>

#ifdef ENABLE_PSC_REG_ACCESS
#if defined(BUILD_MPU) || defined(BUILD_MCU)
/* power domain indices */
#define GP_CORE_CTL      0
#define PD_A53_CLUSTER_0 1
#define PD_A53_0         2
#define PD_A53_1         3
#define PD_PULSAR_0      4
#define PD_PULSAR_1      5
#define PD_ICSSG_0       6
#define PD_ICSSG_1       7
#define PD_CPSW          9

/* lpsc indices */
#define LPSC_MAIN_ALWAYSON        0
#define LPSC_MAIN_TEST            1
#define LPSC_MAIN_PBIST           2
#define LPSC_DMSC                 3
#define LPSC_EMMC_4B              4
#define LPSC_EMMC_8B              5
#define LPSC_USB                  6
#define LPSC_ADC                  7
#define LPSC_DEBUGSS              8
#define LPSC_GPMC                 9
#define LPSC_EMIF_CFG            10
#define LPSC_EMIF_DATA           11
#define LPSC_MCAN_0              12
#define LPSC_MCAN_1              13
#define LPSC_SA2UL               14
#define LPSC_SERDES_0            15
#define LPSC_PCIE_0              16
#define LPSC_A53_CLUSTER_0       20
#define LPSC_A53_CLUSTER_0_PBIST 21
#define LPSC_A53_0               22
#define LPSC_A53_1               23
#define LPSC_PULSAR_0_R5_0       24
#define LPSC_PULSAR_0_R5_1       25
#define LPSC_PULSAR_PBIST_0      26
#define LPSC_PULSAR_1_R5_0       27
#define LPSC_PULSAR_1_R5_1       28
#define LPSC_PULSAR_PBIST_1      29
#define LPSC_ICSSG_0             30
#define LPSC_ICSSG_1             31
#define LPSC_CPSW3G              33

/*
 * macro definitions for the MCU_PSC
*/

/* power domain indices */
#define GP_CORE_CTL_MCU          0
#define PD_M4F                   1

/* lpsc indices */
#define LPSC_MCU_ALWAYSON 0
#define LPSC_TEST 2
#define LPSC_MAIN2MCU 3
#define LPSC_MCU2MAIN 4
#define LPSC_M4F 7

// PSC Register Definitions
//#define PSC_PID                     (0x44827A00)
//#define M3_SOC_OFFSET               (0x60000000)
//#define M3_RAT_CTRL                 (0x44200020)

#define MCU_PSC_BASE                (0x04000000)
#define MAIN_PSC_BASE               (0x00400000)



#define MAIN_PSC_INDEX              0
#define MCU_PSC_INDEX               1

#define PSC_MDCTL00                 (0xA00)
#define PSC_MDSTAT00                (0x800)
#define PSC_PDCTL00                 (0x300)
#define PSC_PDSTAT00                (0x200)
#define PSC_PTCMD                   (0x120)
#define PSC_PTSTAT                  (0x128)

#define PSC_PD_OFF                  (0x0)
#define PSC_PD_ON                   (0x1)

#define PSC_SYNCRESETDISABLE        (0x0)
#define PSC_SYNCRESET               (0x1)
#define PSC_DISABLE                 (0x2)
#define PSC_ENABLE                  (0x3)

#define PSC_TIMEOUT                 (100)

#define MCU_PSC_MDCTL_BASE          MCU_PSC_BASE + PSC_MDCTL00
#define MCU_PSC_MDSTAT_BASE         MCU_PSC_BASE + PSC_MDSTAT00
#define MCU_PSC_PDCTL_BASE          MCU_PSC_BASE + PSC_PDCTL00
#define MCU_PSC_PDSTAT_BASE         MCU_PSC_BASE + PSC_PDSTAT00
#define MCU_PSC_PTCMD_BASE          MCU_PSC_BASE + PSC_PTCMD
#define MCU_PSC_PTSTAT_BASE         MCU_PSC_BASE + PSC_PTSTAT
#define MCU_PSC_PTCMD               MCU_PSC_PTCMD_BASE
#define MCU_PSC_PTSTAT              MCU_PSC_PTSTAT_BASE

#define MAIN_PSC_MDCTL_BASE         MAIN_PSC_BASE + PSC_MDCTL00
#define MAIN_PSC_MDSTAT_BASE        MAIN_PSC_BASE + PSC_MDSTAT00
#define MAIN_PSC_PDCTL_BASE         MAIN_PSC_BASE + PSC_PDCTL00
#define MAIN_PSC_PDSTAT_BASE        MAIN_PSC_BASE + PSC_PDSTAT00
#define MAIN_PSC_PTCMD_BASE         MAIN_PSC_BASE + PSC_PTCMD
#define MAIN_PSC_PTSTAT_BASE        MAIN_PSC_BASE + PSC_PTSTAT
#define MAIN_PSC_PTCMD              MAIN_PSC_PTCMD_BASE
#define MAIN_PSC_PTSTAT             MAIN_PSC_PTSTAT_BASE

Board_STATUS Set_MAIN_PSC_State(unsigned int pd_id, unsigned int md_id, unsigned int pd_state, unsigned int md_state)
{
    unsigned int* mdctl;
    unsigned int* mdstat;
    unsigned int* pdctl;
    unsigned int* pdstat;

    unsigned int loop_cnt = 0;
    Board_STATUS ret = BOARD_SOK;

    unsigned int address_offset = 0;

    address_offset = 0; //offset 0x80000000 on M3 core

    mdctl = (unsigned int*)(MAIN_PSC_MDCTL_BASE + ( 4 * md_id ) + address_offset);
    mdstat = (unsigned int*)(MAIN_PSC_MDSTAT_BASE + ( 4 * md_id )+ address_offset);
    pdctl = (unsigned int*)(MAIN_PSC_PDCTL_BASE + ( 4 * pd_id ) + address_offset);
    pdstat = (unsigned int*)(MAIN_PSC_PDSTAT_BASE + ( 4 * pd_id )+ address_offset);

    // If state is already set, do nothing
    if ( (( *pdstat & 0x1 ) == pd_state) && (( *mdstat & 0x1f ) == md_state) )
    {
        return ret;
    }

    // Wait for GOSTAT to clear
    while( (loop_cnt < PSC_TIMEOUT) && (*(unsigned int*)(MAIN_PSC_PTSTAT+address_offset) & (0x1 << pd_id)) != 0 )
    {
        loop_cnt++;
    }

    // Check if we got timeout error while waiting
    if (loop_cnt >= PSC_TIMEOUT)
    {
        ret = BOARD_FAIL;
        return ret;
    }

    // Set PDCTL NEXT to new state
    *pdctl = ((*pdctl) & ~(0x1)) | pd_state;

    // Set MDCTL NEXT to new state
    *mdctl = ((*mdctl) & ~(0x1f)) | md_state;

    // Start power transition by setting PTCMD GO to 1
    *((unsigned int*)(MAIN_PSC_PTCMD+address_offset)) = (*(unsigned int*)(MAIN_PSC_PTCMD+address_offset)) | (0x1<<pd_id);

    loop_cnt = 0;

    // Wait for PTSTAT GOSTAT to clear
    while( (loop_cnt < PSC_TIMEOUT) && (*(unsigned int*)(MAIN_PSC_PTSTAT+address_offset) & (0x1 << pd_id)) != 0 )
    {
        loop_cnt++;
    }

    // Check if we got timeout error while waiting
    if (loop_cnt >= PSC_TIMEOUT)
    {
        ret = BOARD_FAIL;
        return ret;
    }

    // Verify power domain and module domain state got changed
    if (( *pdstat & 0x1 ) != pd_state)
    {
        ret = BOARD_FAIL;
        return ret;
    }
    if (( *mdstat & 0x1f ) != md_state)
    {
        ret = BOARD_FAIL;
    }

    return ret;
}

Board_STATUS GP_CORE_CTL_PowerUp()
{
    Board_STATUS status = BOARD_SOK;

    status |= Set_MAIN_PSC_State( GP_CORE_CTL, LPSC_EMIF_CFG, PSC_PD_ON, PSC_ENABLE);
    status |= Set_MAIN_PSC_State( GP_CORE_CTL, LPSC_EMIF_DATA, PSC_PD_ON, PSC_ENABLE);
    return status;
}

Board_STATUS PD_A53_CLUSTER_0_PowerUp()
{
    Board_STATUS status = BOARD_SOK;

    status |= Set_MAIN_PSC_State( PD_A53_CLUSTER_0, LPSC_A53_CLUSTER_0, PSC_PD_ON, PSC_ENABLE);

    return status;
}

Board_STATUS PD_A53_0_PowerUp()
{
    Board_STATUS status = BOARD_SOK;

    status |= Set_MAIN_PSC_State( PD_A53_0, LPSC_A53_0, PSC_PD_ON, PSC_ENABLE);

    return status;
}

Board_STATUS PD_PULSAR_0_PowerUp()
{
    Board_STATUS status = BOARD_SOK;

    //status |= Set_MAIN_PSC_State( PD_PULSAR_0,LPSC_PULSAR_0_R5_0, PSC_PD_ON, PSC_ENABLE);
    //status |= Set_MAIN_PSC_State( PD_PULSAR_0,LPSC_PULSAR_0_R5_1, PSC_PD_ON, PSC_ENABLE);

    return status;
}

Board_STATUS MAIN_PSC_Peripheral_PowerUp()
{
    Board_STATUS status = BOARD_SOK;

    status |= GP_CORE_CTL_PowerUp();
    status |= PD_A53_CLUSTER_0_PowerUp();
    status |= PD_A53_0_PowerUp();
    status |= PD_PULSAR_0_PowerUp();

    return status;
}

Board_STATUS Set_MCU_PSC_State(unsigned int pd_id, unsigned int md_id, unsigned int pd_state, unsigned int md_state)
{
   unsigned int* mdctl;
   unsigned int* mdstat;
   unsigned int* pdctl;
   unsigned int* pdstat;

   unsigned int loop_cnt = 0;
   Board_STATUS ret = BOARD_SOK;

   unsigned int address_offset = 0;

   address_offset = 0; // offset is 0x80000000 on M3 core

   mdctl  = (unsigned int*)(MCU_PSC_MDCTL_BASE + ( 4 * md_id ) + address_offset);
   mdstat = (unsigned int*)(MCU_PSC_MDSTAT_BASE + ( 4 * md_id ) + address_offset);
   pdctl  = (unsigned int*)(MCU_PSC_PDCTL_BASE + ( 4 * pd_id ) + address_offset);
   pdstat = (unsigned int*)(MCU_PSC_PDSTAT_BASE + ( 4 * pd_id ) + address_offset);

   // If state is already set, do nothing
   if ( (( *pdstat & 0x1 ) == pd_state) && (( *mdstat & 0x1f ) == md_state) )
   {
       return ret;
   }

   // Wait for GOSTAT to clear
   while( (loop_cnt < PSC_TIMEOUT) && (*((unsigned int*)(MCU_PSC_PTSTAT+address_offset)) & (0x1 << pd_id)) != 0 )
   {
       loop_cnt++;
   }

   // Check if we got timeout error while waiting
   if (loop_cnt >= PSC_TIMEOUT)
   {
       ret = BOARD_FAIL;
       return ret;
   }

   // Set PDCTL NEXT to new state
   *pdctl = ((*pdctl) & ~(0x1)) | pd_state;

   // Set MDCTL NEXT to new state
   *mdctl = ((*mdctl) & ~(0x1f)) | md_state;

   // Start power transition by setting PTCMD GO to 1
   (*(unsigned int*)(MCU_PSC_PTCMD + address_offset)) = (*(unsigned int*)(MCU_PSC_PTCMD + address_offset)) | (0x1<<pd_id);

   loop_cnt = 0;

   // Wait for PTSTAT GOSTAT to clear
   while( (loop_cnt < PSC_TIMEOUT) && (*((unsigned int*)(MCU_PSC_PTSTAT+address_offset)) & (0x1 << pd_id)) != 0 )
   {
       loop_cnt++;
   }

   // Check if we got timeout error while waiting
   if (loop_cnt >= PSC_TIMEOUT)
   {
       ret = BOARD_FAIL;
       return ret;
   }

   // Verify power domain and module domain state got changed
   if (( *pdstat & 0x1 ) != pd_state)
   {
       ret = BOARD_FAIL;
       return ret;
   }
   if (( *mdstat & 0x1f ) != md_state)
   {
       ret = BOARD_FAIL;
       return ret;
   }

   return ret;
}

Board_STATUS GP_Core_CTL_MCU_PowerUp()
{
    Board_STATUS status = BOARD_SOK;

    status |= Set_MCU_PSC_State( GP_CORE_CTL_MCU, LPSC_MAIN2MCU, PSC_PD_ON, PSC_ENABLE);
    status |= Set_MCU_PSC_State( GP_CORE_CTL_MCU, LPSC_MCU2MAIN, PSC_PD_ON, PSC_ENABLE);

    return status;
}

Board_STATUS PD_M4F_PowerUp()
{
    Board_STATUS status = BOARD_SOK;

    status |= Set_MCU_PSC_State( PD_M4F, LPSC_M4F, PSC_PD_ON, PSC_ENABLE);

    return status;
}

Board_STATUS MCU_PSC_Peripheral_PowerUp()
{
    Board_STATUS status = BOARD_SOK;

    return status;
}
#endif /* #ifdef BUILD_MPU/MCU */

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
const pscConfig wkupPscConfigs[] =
{

};

/**
 * \brief main PSC configuration parameters
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
const pscConfig mainPscConfigs[] =
{

};

/**
 *  \brief    This function is used to get the number of \
 *            wkup PSC configs exists.
 *
 * \return
 * \n         uint32_t - Number of wkup PSC configs.
 */
uint32_t Board_getNumWkupPscCconfigs(void)
{
    return (sizeof(wkupPscConfigs) / sizeof(pscConfig));
}

/**
 *  \brief    This function is used to get the number of \
 *            main PSC configs exists.
 *
 * \return
 * \n         uint32_t - Number of main PSC configs.
 */
uint32_t Board_getNumMainPscCconfigs(void)
{
    return (sizeof(mainPscConfigs) / sizeof(pscConfig));
}

#else /*  #ifdef ENABLE_PSC_REG_ACCESS */

#include <ti/drv/sciclient/sciclient.h>

uint32_t gBoardClkModuleID[] = {
    TISCI_DEV_ADC0,
    TISCI_DEV_CPSW0,
    TISCI_DEV_CPT2_AGGR0,
    TISCI_DEV_DMASS0,
    TISCI_DEV_DMASS0_BCDMA_0,
    TISCI_DEV_DMASS0_CBASS_0,
    TISCI_DEV_DMASS0_INTAGGR_0,
    TISCI_DEV_DMASS0_IPCSS_0,
    TISCI_DEV_DMASS0_PKTDMA_0,
    TISCI_DEV_DMASS0_PSILCFG_0,
    TISCI_DEV_DMASS0_PSILSS_0,
    TISCI_DEV_DMASS0_RINGACC_0,
    TISCI_DEV_MCU_TIMER0,
    TISCI_DEV_TIMER0,
    TISCI_DEV_TIMER1,
    TISCI_DEV_TIMER2,
    TISCI_DEV_TIMER3,
    TISCI_DEV_TIMER4,
    TISCI_DEV_TIMER5,
    TISCI_DEV_TIMER6,
    TISCI_DEV_TIMER7,
    TISCI_DEV_TIMER8,
    TISCI_DEV_TIMER9,
    TISCI_DEV_TIMER10,
    TISCI_DEV_TIMER11,
    TISCI_DEV_MCU_TIMER1,
    TISCI_DEV_MCU_TIMER2,
    TISCI_DEV_MCU_TIMER3,
    TISCI_DEV_ECAP0,
    TISCI_DEV_ECAP1,
    TISCI_DEV_ECAP2,
    TISCI_DEV_ELM0,
    TISCI_DEV_EMIF_DATA_0_VD,
    TISCI_DEV_MMCSD0,
    TISCI_DEV_MMCSD1,
    TISCI_DEV_EQEP0,
    TISCI_DEV_EQEP1,
    TISCI_DEV_GTC0,
    TISCI_DEV_EQEP2,
    TISCI_DEV_ESM0,
    TISCI_DEV_MCU_ESM0,
    TISCI_DEV_FSIRX0,
    TISCI_DEV_FSIRX1,
    TISCI_DEV_FSIRX2,
    TISCI_DEV_FSIRX3,
    TISCI_DEV_FSIRX4,
    TISCI_DEV_FSIRX5,
    TISCI_DEV_FSITX0,
    TISCI_DEV_FSITX1,
    TISCI_DEV_FSS0,
    TISCI_DEV_FSS0_FSAS_0,
    TISCI_DEV_FSS0_OSPI_0,
    TISCI_DEV_GICSS0,
    TISCI_DEV_GPIO0,
    TISCI_DEV_GPIO1,
    TISCI_DEV_MCU_GPIO0,
    TISCI_DEV_GPMC0,
    TISCI_DEV_PRU_ICSSG0,
    TISCI_DEV_PRU_ICSSG1,
    TISCI_DEV_LED0,
    TISCI_DEV_CPTS0,
    TISCI_DEV_DDPA0,
    TISCI_DEV_EPWM0,
    TISCI_DEV_EPWM1,
    TISCI_DEV_EPWM2,
    TISCI_DEV_EPWM3,
    TISCI_DEV_EPWM4,
    TISCI_DEV_EPWM5,
    TISCI_DEV_EPWM6,
    TISCI_DEV_EPWM7,
    TISCI_DEV_EPWM8,
    TISCI_DEV_VTM0,
    TISCI_DEV_MAILBOX0,
    TISCI_DEV_MCAN0,
    TISCI_DEV_MCAN1,
    TISCI_DEV_I2C0,
    TISCI_DEV_I2C1,
    TISCI_DEV_I2C2,
    TISCI_DEV_I2C3,
    TISCI_DEV_MCU_I2C0,
    TISCI_DEV_MCU_I2C1,
    TISCI_DEV_PCIE0,
    TISCI_DEV_SA2_UL0,
    TISCI_DEV_DDR16SS0,
    TISCI_DEV_MCSPI0,
    TISCI_DEV_MCSPI1,
    TISCI_DEV_MCSPI2,
    TISCI_DEV_MCSPI3,
    TISCI_DEV_MCSPI4,
    TISCI_DEV_UART0,
    TISCI_DEV_MCU_MCSPI0,
    TISCI_DEV_MCU_MCSPI1,
    TISCI_DEV_MCU_UART0,
    TISCI_DEV_TIMERMGR0,
    TISCI_DEV_UART1,
    TISCI_DEV_UART2,
    TISCI_DEV_UART3,
    TISCI_DEV_UART4,
    TISCI_DEV_UART5,
    TISCI_DEV_BOARD0,
    TISCI_DEV_UART6,
    TISCI_DEV_MCU_UART1,
    TISCI_DEV_USB0,
    TISCI_DEV_SERDES_10G0
};

/**
 * \brief Enables module clock
 *
 * \return  BOARD_SOK   - Clock enable sucessful. 
 *          BOARD_FAIL  - Clock enable failed.
 *
 */
Board_STATUS Board_moduleClockEnable(uint32_t moduleId)
{
    Board_STATUS retVal = BOARD_SOK;
	int32_t      status = CSL_EFAIL;
    uint32_t     moduleState = 0U;
    uint32_t     resetState = 0U;
    uint32_t     contextLossState = 0U;

    /* Get the module state. 
       No need to change the module state if it
       is already ON 
     */
    status = Sciclient_pmGetModuleState(moduleId,
                                        &moduleState,
                                        &resetState,
                                        &contextLossState,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(moduleState == TISCI_MSG_VALUE_DEVICE_HW_STATE_OFF)
    {
        status = Sciclient_pmSetModuleState(moduleId,
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                            (TISCI_MSG_FLAG_AOP |
                                             TISCI_MSG_FLAG_DEVICE_RESET_ISO),
                                             SCICLIENT_SERVICE_WAIT_FOREVER);
        if (status == CSL_PASS)
        {
            status = Sciclient_pmSetModuleRst (moduleId,
                                               0x0U,
                                               SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                retVal = BOARD_FAIL;
            }
        }
        else
        {
            retVal = BOARD_FAIL;
        }
    }

    return retVal;
}
#endif /* #ifdef ENABLE_PSC_REG_ACCESS */

Board_STATUS Board_moduleClockInit(void)
{
    Board_STATUS  status = BOARD_SOK;
    uint32_t index;
    uint32_t loopCount;
    int32_t  ret;
    uint64_t cpuClkFreq, cpuModuleId, cpuClockId;

#ifdef BUILD_M4F_0
    cpuModuleId = TISCI_DEV_MCU_M4FSS0_CORE0;
    cpuClockId = TISCI_DEV_MCU_M4FSS0_CORE0_DAP_CLK;
#endif
#ifdef BUILD_MCU1_0
    cpuModuleId = TISCI_DEV_R5FSS0_CORE0;
    cpuClockId = TISCI_DEV_R5FSS0_CORE0_CPU_CLK;
#endif
#ifdef BUILD_MCU1_1
    cpuModuleId = TISCI_DEV_R5FSS0_CORE1;
    cpuClockId = TISCI_DEV_R5FSS0_CORE1_CPU_CLK;
#endif
#ifdef BUILD_MCU2_0
    cpuModuleId = TISCI_DEV_R5FSS1_CORE0;
    cpuClockId = TISCI_DEV_R5FSS1_CORE0_CPU_CLK;
#endif
#ifdef BUILD_MCU2_1
    cpuModuleId = TISCI_DEV_R5FSS1_CORE1;
    cpuClockId = TISCI_DEV_R5FSS1_CORE1_CPU_CLK;
#endif
#ifdef BUILD_MPU
    cpuModuleId = TISCI_DEV_A53SS0_CORE_0;
    cpuClockId = TISCI_DEV_A53SS0_CORE_0_A53_CORE0_ARM_CLK_CLK;
#endif
    ret = Sciclient_pmGetModuleClkFreq(cpuModuleId,
                                       cpuClockId,
                                       &cpuClkFreq,
                                       SCICLIENT_SERVICE_WAIT_FOREVER);

    if(ret == 0)
    {
        Osal_HwAttrs  hwAttrs;
        uint32_t      ctrlBitmap;

        ret = Osal_getHwAttrs(&hwAttrs);
        if(ret == 0)
        {
            /*
             * Change the timer input clock frequency configuration
               based on R5 CPU clock configured
             */
            hwAttrs.cpuFreqKHz = (int32_t)(cpuClkFreq/1000U);
            ctrlBitmap         = OSAL_HWATTR_SET_CPU_FREQ;
            ret = Osal_setHwAttrs(ctrlBitmap, &hwAttrs);
        }
    }
    if(ret != 0)
    {
        status = BOARD_INIT_CLOCK_FAIL;
    }

#ifdef ENABLE_PSC_REG_ACCESS   
#ifdef SIM_BUILD
#ifdef BUILD_MPU
    status |= Board_unlockMMR();
    status |= MAIN_PSC_Peripheral_PowerUp();
#endif
#ifdef BUILD_MCU
    status |= Board_unlockMMR();
    status |= MCU_PSC_Peripheral_PowerUp();
#endif
#endif
#else
    if( status == BOARD_SOK)
    {
        loopCount = sizeof(gBoardClkModuleID) / sizeof(uint32_t);

        for(index = 0; index < loopCount; index++)
        {
            status = Board_moduleClockEnable(gBoardClkModuleID[index]);
            if(status != BOARD_SOK)
            {
                return BOARD_INIT_CLOCK_FAIL;
            }
        }
    }
#endif /* #ifdef ENABLE_PSC_REG_ACCESS */
    return status;
}
