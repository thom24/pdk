/******************************************************************************
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
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

#ifdef VLAB_SIM
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

    status |= GP_Core_CTL_MCU_PowerUp();
    status |= PD_M4F_PowerUp();

    return status;
}
#endif /* #ifdef BUILD_MPU/MCU */
#endif /* #ifdef VLAB_SIM */

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

Board_STATUS Board_moduleClockInit(void)
{
    Board_STATUS status = BOARD_SOK;
#if defined(__TI_ARM_V7R4__)
    int32_t      ret;
    uint64_t     mcuClkFreq;
#if 0
    ret = Sciclient_pmGetModuleClkFreq(TISCI_DEV_MCU_ARMSS0_CPU0,
                                       TISCI_DEV_MCU_ARMSS0_CPU0_BUS_CPU_CLK,
                                       &mcuClkFreq,
                                       SCICLIENT_SERVICE_WAIT_FOREVER);
#else
    /* 
     * Temporary hard-code R5 core clock to 800 MHz (default on Zebu)
     * until Sciclent PM is supported
     */
    mcuClkFreq = 800000000U;
    ret = 0U;
#endif
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
            hwAttrs.cpuFreqKHz = (int32_t)(mcuClkFreq/1000U);
            ctrlBitmap         = OSAL_HWATTR_SET_CPU_FREQ;
            ret = Osal_setHwAttrs(ctrlBitmap, &hwAttrs);
        }
    }
    if(ret != 0)
    {
        status = BOARD_INIT_CLOCK_FAIL;
    }
#endif    
#ifdef VLAB_SIM
#ifdef BUILD_MPU
    status |= Board_unlockMMR();
    status |= MAIN_PSC_Peripheral_PowerUp();
#endif
#ifdef BUILD_MCU
    status |= Board_unlockMMR();
    status |= MCU_PSC_Peripheral_PowerUp();
#endif
#endif
    return (status);
}
