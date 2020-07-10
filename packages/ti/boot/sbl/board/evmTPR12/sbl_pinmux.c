/**
 *  \file   sbl_pinmux.c
 *
 *  \brief  This file contain pinmux functionality for peripherals used by SBL.
 *
 */

/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <stddef.h>
#include <ti/csl/soc.h>
#include "sbl_csl.h"

 /* TI RTOS header files */

/**
 * @brief List of all the pulls configuration
 *
 * @details
    Data enumeration holds the possible pull configuration values
 */
typedef enum DrvPinmux_PullValues_e
{
    DrvPinmux_PullDisable,          //!< No pulls at the pad level
    DrvPinmux_PullDown,             //!< Pull-Down at the Pad level
    DrvPinmux_PullUp,               //!< Pull-Up at the Pad level
} DrvPinmux_PullValues;

/**
 * @brief List of all the output over-ride configuration values
 *
 * @details
    Data enumeration holds the possible output over-ride configuration values
 */
typedef enum DrvPinmux_OutOverrideConfig_e
{
    DrvPinmux_OutOverrideDisable,      //!< No Pad level over-ride for output
    DrvPinmux_OutEnable,               //!< Output enable from Pad level override
    DrvPinmux_OutDisable,              //!< Output enable from Pad level override
} DrvPinmux_OutOverrideConfig;

/**
 * @brief List of all the input over-ride configuration values
 *
 * @details
    Data enumeration holds the possible input over-ride configuration values
 */
typedef enum DrvPinmux_InOverrideConfig_e
{
    DrvPinmux_InOverrideDisable,      //!< No Pad level over-ride for input
    DrvPinmux_InEnable,               //!< Input enable from Pad level override
    DrvPinmux_InDisable,              //!< INput enable from Pad level override
} DrvPinmux_InOverrideConfig;

/**                          
 * @brief PAD indexes for all the pads
 */
#define DRV_PINMUX_PAD_AA       (0U)
#define DRV_PINMUX_PAD_AB       (1U)
#define DRV_PINMUX_PAD_AC       (2U)
#define DRV_PINMUX_PAD_AD       (3U)
#define DRV_PINMUX_PAD_AE       (4U)
#define DRV_PINMUX_PAD_AF       (5U)
#define DRV_PINMUX_PAD_AG       (6U)
#define DRV_PINMUX_PAD_AH       (7U)
#define DRV_PINMUX_PAD_AI       (8U)
#define DRV_PINMUX_PAD_AJ       (9U)
#define DRV_PINMUX_PAD_AK       (10U)
#define DRV_PINMUX_PAD_AL       (11U)
#define DRV_PINMUX_PAD_AM       (12U)
#define DRV_PINMUX_PAD_AN       (13U)
#define DRV_PINMUX_PAD_AO       (14U)
#define DRV_PINMUX_PAD_AP       (15U)
#define DRV_PINMUX_PAD_AQ       (16U)
#define DRV_PINMUX_PAD_AR       (17U)
#define DRV_PINMUX_PAD_AS       (18U)
#define DRV_PINMUX_PAD_AT       (19U)
#define DRV_PINMUX_PAD_AU       (20U)
#define DRV_PINMUX_PAD_AV       (21U)
#define DRV_PINMUX_PAD_AW       (22U)
#define DRV_PINMUX_PAD_AX       (23U)
#define DRV_PINMUX_PAD_AY       (24U)
#define DRV_PINMUX_PAD_AZ       (25U)
#define DRV_PINMUX_PAD_BA       (26U)
#define DRV_PINMUX_PAD_BB       (27U)
#define DRV_PINMUX_PAD_BC       (28U)
#define DRV_PINMUX_PAD_BD       (29U)
#define DRV_PINMUX_PAD_BE       (30U)
#define DRV_PINMUX_PAD_BF       (31U)
#define DRV_PINMUX_PAD_BG       (32U)
#define DRV_PINMUX_PAD_BH       (33U)
#define DRV_PINMUX_PAD_BI       (34U)
#define DRV_PINMUX_PAD_BJ       (35U)
#define DRV_PINMUX_PAD_BK       (36U)
#define DRV_PINMUX_PAD_BL       (37U)
#define DRV_PINMUX_PAD_BM       (38U)
#define DRV_PINMUX_PAD_BN       (39U)
#define DRV_PINMUX_PAD_BO       (40U)
#define DRV_PINMUX_PAD_BP       (41U)
#define DRV_PINMUX_PAD_BQ       (42U)
#define DRV_PINMUX_PAD_BR       (43U)
#define DRV_PINMUX_PAD_BS       (44U)
#define DRV_PINMUX_PAD_BT       (45U)
#define DRV_PINMUX_PAD_BU       (46U)
#define DRV_PINMUX_PAD_BV       (47U)
#define DRV_PINMUX_PAD_BW       (48U)
#define DRV_PINMUX_PAD_BX       (49U)
#define DRV_PINMUX_PAD_BY       (50U)
#define DRV_PINMUX_PAD_BZ       (51U)
#define DRV_PINMUX_PAD_CA       (52U)
#define DRV_PINMUX_PAD_CB       (53U)
#define DRV_PINMUX_PAD_CC       (54U)
#define DRV_PINMUX_PAD_CD       (55U)
#define DRV_PINMUX_PAD_CE       (56U)
#define DRV_PINMUX_PAD_CF       (57U)
#define DRV_PINMUX_PAD_CG       (58U)
#define DRV_PINMUX_PAD_CH       (59U)
#define DRV_PINMUX_PAD_CI       (60U)
#define DRV_PINMUX_PAD_CJ       (61U)
#define DRV_PINMUX_PAD_CK       (62U)
#define DRV_PINMUX_PAD_CL       (63U)
#define DRV_PINMUX_PAD_CM       (64U)
#define DRV_PINMUX_PAD_CN       (65U)
#define DRV_PINMUX_PAD_CO       (66U)
#define DRV_PINMUX_PAD_CP       (67U)
#define DRV_PINMUX_PAD_CQ       (68U)
#define DRV_PINMUX_PAD_CR       (69U)
#define DRV_PINMUX_PAD_CS       (70U)
#define DRV_PINMUX_PAD_CT       (71U)
#define DRV_PINMUX_PAD_CU       (72U)
#define DRV_PINMUX_PAD_CV       (73U)
#define DRV_PINMUX_PAD_CW       (74U)
#define DRV_PINMUX_PAD_CX       (75U)
#define DRV_PINMUX_PAD_CY       (76U)
#define DRV_PINMUX_PAD_CZ       (77U)
#define DRV_PINMUX_PAD_DA       (78U)
#define DRV_PINMUX_PAD_DB       (79U)
#define DRV_PINMUX_PAD_DC       (80U)
#define DRV_PINMUX_PAD_DD       (81U)
#define DRV_PINMUX_PAD_DE       (82U)
#define DRV_PINMUX_PAD_DF       (83U)
#define DRV_PINMUX_PAD_DG       (84U)
#define DRV_PINMUX_PAD_DH       (85U)
#define DRV_PINMUX_PAD_DI       (86U)
#define DRV_PINMUX_PAD_DJ       (87U)
#define DRV_PINMUX_PAD_DK       (88U)
#define DRV_PINMUX_PAD_DL       (89U)
#define DRV_PINMUX_PAD_DM       (90U)
#define DRV_PINMUX_PAD_DN       (91U)
#define DRV_PINMUX_PAD_DO       (92U)
#define DRV_PINMUX_PAD_DP       (93U)
#define DRV_PINMUX_PAD_DQ       (94U)
#define DRV_PINMUX_PAD_DR       (95U)
#define DRV_PINMUX_PAD_DS       (96U)
#define DRV_PINMUX_PAD_DT       (97U)
#define DRV_PINMUX_PAD_DU       (98U)
#define DRV_PINMUX_PAD_DV       (99U)
#define DRV_PINMUX_PAD_DW       (100U)
#define DRV_PINMUX_PAD_DX       (101U)
#define DRV_PINMUX_PAD_DY       (102U)
#define DRV_PINMUX_PAD_DZ       (103U)
#define DRV_PINMUX_PAD_EA       (104U)
#define DRV_PINMUX_PAD_EB       (105U)
#define DRV_PINMUX_PAD_EC       (106U)
#define DRV_PINMUX_PAD_ED       (107U)
#define DRV_PINMUX_PAD_EE       (108U)
#define DRV_PINMUX_PAD_EF       (109U)
#define DRV_PINMUX_PAD_EG       (110U)
#define DRV_PINMUX_PAD_EH       (111U)
#define DRV_PINMUX_PAD_EI       (112U)
#define DRV_PINMUX_PAD_EJ       (113U)
#define DRV_PINMUX_PAD_EK       (114U)
#define DRV_PINMUX_PAD_EL       (115U)
#define DRV_PINMUX_PAD_EM       (116U)
#define DRV_PINMUX_PAD_EN       (117U)
#define DRV_PINMUX_PAD_EO       (118U)
#define DRV_PINMUX_PAD_EP       (119U)


/**
 *  @b Description
 *  @n
 *      The function is used to get the base address of the IO-Mux configuration
 *      registers.
 *
 *  \ingroup CSL_PINMUX_FUNCTION
 *
 *  @retval
 *      Pointer to the IO-Mux configuration registers base address.
 */
static CSL_mss_iomuxRegs* CSL_IOMUX_getBaseAddress(void)
{
    return ((CSL_mss_iomuxRegs*)CSL_MSS_IOMUX_U_BASE);
}

/**
 *  @b Description
 *  @n
 *      The function is used to enable the access to the IO-Mux register space
 *
 *  @param[in] ptrIOMuxRegs
 *      Pointer to the IO-Mux Registers base address
 *
 *  @retval
 *      Not Applicable.
 */
static void CSL_IOMUX_enableAccess(CSL_mss_iomuxRegs* ptrIOMuxRegs)
{
    ptrIOMuxRegs->IOCFGKICK0 = 0x83E70B13U;
    ptrIOMuxRegs->IOCFGKICK1 = 0x95A4F1E0U;
}

/**
 *  @b Description
 *  @n
 *      The function is used to disable the access to the IO-Mux register space
 *
 *  @param[in] ptrIOMuxRegs
 *      Pointer to the IO-Mux Registers base address
 *
 *  @retval
 *      Not Applicable.
 */
static void CSL_IOMUX_disableAccess(CSL_mss_iomuxRegs* ptrIOMuxRegs)
{
    ptrIOMuxRegs->IOCFGKICK0 = 0x0U;
    ptrIOMuxRegs->IOCFGKICK1 = 0x0U;
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the pull settings for the PAD
 *
 *  @param[in] ptrIOMuxRegs
 *      Pointer to the IO-Mux Registers base address
 *  @param[in] pinId
 *      PAD index which is to be configured
 *  @param[in] pullVal
 *      value to be written into the pull configuration bits
 *
 *  @retval
 *      Not Applicable.
 */
static void CSL_IOMUX_setPull(CSL_mss_iomuxRegs* ptrIOMuxRegs, uint32_t pinId, uint8_t pullVal)
{
    uint32_t *padCfg = (uint32_t *)(((uintptr_t)ptrIOMuxRegs) + offsetof(CSL_mss_iomuxRegs, PADAA_CFG_REG) + (pinId * sizeof(ptrIOMuxRegs->PADAA_CFG_REG)));

    *padCfg = CSL_insert8 (*padCfg, 9U, 8U, pullVal);
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the Output override settings
 *
 *  @param[in] ptrIOMuxRegs
 *      Pointer to the IO-Mux Registers base address
 *  @param[in] pinId
 *      PAD index which is to be configured
 *  @param[in] overrideVal
 *      value to be written into the output override configuration bits
 *
 *  @retval
 *      Not Applicable.
 */
static void CSL_IOMUX_setOutput(CSL_mss_iomuxRegs* ptrIOMuxRegs, uint32_t pinId, uint8_t overrideVal)
{
    uint32_t *padCfg = (uint32_t *)(((uintptr_t)ptrIOMuxRegs) + offsetof(CSL_mss_iomuxRegs, PADAA_CFG_REG) + (pinId * sizeof(ptrIOMuxRegs->PADAA_CFG_REG)));

    *padCfg = CSL_insert8 (*padCfg, 7U, 6U, overrideVal);
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the functional mode of the PAD
 *
 *  @param[in] ptrIOMuxRegs
 *      Pointer to the IO-Mux Registers base address
 *  @param[in] pinId
 *      PAD index which is to be configured
 *  @param[in] funcMode
 *      value to be written into the functional configuration bits
 *
 *  @retval
 *      Not Applicable.
 */
static void CSL_IOMUX_setFuncMode(CSL_mss_iomuxRegs* ptrIOMuxRegs, uint32_t pinId, uint8_t funcMode)
{
    uint32_t *padCfg = (uint32_t *)(((uintptr_t)ptrIOMuxRegs) + offsetof(CSL_mss_iomuxRegs, PADAA_CFG_REG) + (pinId * sizeof(ptrIOMuxRegs->PADAA_CFG_REG)));

    *padCfg = CSL_insert8 (*padCfg, 3U, 0U, funcMode);
}

/**
 *  @b Description
 *  @n
 *     Function to enable access to the PinMux Pad Registers
 *
 *  \ingroup DRIVER_PINMUX_FUNCTIONS
 *
 *  @retval
 *      None
 */
static void DrvPinmux_UnlockIOs(void)
{
    CSL_mss_iomuxRegs* ptrIOMuxRegs;
    ptrIOMuxRegs = CSL_IOMUX_getBaseAddress ();
    CSL_IOMUX_enableAccess (ptrIOMuxRegs);
}

/**
 *  @b Description
 *  @n
 *     Function to disable access to the PinMux Pad Registers
 *
 *  \ingroup DRIVER_PINMUX_FUNCTIONS
 *
 *  @retval
 *      None
 */
static void DrvPinmux_LockIOs(void)
{
    CSL_mss_iomuxRegs* ptrIOMuxRegs;
    ptrIOMuxRegs = CSL_IOMUX_getBaseAddress ();
    CSL_IOMUX_disableAccess (ptrIOMuxRegs);
}

/**
 *  @b Description
 *  @n
 *     Function to select the functional mode for a Pin
 *
 *  @param[in] pin          Pin index for the pinmux pad
 *
 *  @param[in] funcMode     Functional mode to be selected for the Pad
 *
 *  \ingroup DRIVER_PINMUX_FUNCTIONS
 *
 *  @retval
 *      DrvPinmux_ReturnSuccess     - Successfull selection of functional mode
 *      DrvPinmux_ReturnInvalidCfg  - Invalid configuration resulting in failure
 */
static void DrvPinmux_SelectFunc(uint32_t pin, uint8_t funcMode)
{
    CSL_mss_iomuxRegs* ptrIOMuxRegs;
    ptrIOMuxRegs = CSL_IOMUX_getBaseAddress ();

    CSL_IOMUX_setFuncMode (ptrIOMuxRegs, pin, funcMode);

}

/**
 *  @b Description
 *  @n
 *     Function to configure the pull setting for the Pin
 *
 *  @param[in] pin         Pin index for the pinmux pad
 *
 *  @param[in] pullVal     Pull configuration for the pad
 *                         DrvPinmux_PullDisable
 *                         DrvPinmux_PullDown
 *                         DrvPinmux_PullUp
 *
 *
 *  \ingroup DRIVER_PINMUX_FUNCTIONS
 *
 *  @retval
 *      DrvPinmux_ReturnSuccess     - Successfullconfiguration of pulls
 *      DrvPinmux_ReturnInvalidCfg  - Invalid configuration resulting in failure
 */
static void DrvPinmux_ConfigPulls(uint32_t pin, DrvPinmux_PullValues pullVal)
{
    CSL_mss_iomuxRegs* ptrIOMuxRegs;
    ptrIOMuxRegs = CSL_IOMUX_getBaseAddress ();

    if(pullVal == DrvPinmux_PullDown)
    {
        /* Enable pull and pulldown the pad */
        CSL_IOMUX_setPull (ptrIOMuxRegs, pin, 0x0U);
    }
    else if (pullVal == DrvPinmux_PullUp)
    {
        /* Enable pull and pull up the pad */
        CSL_IOMUX_setPull (ptrIOMuxRegs, pin, 0x2U);
    }
    else
    {
        /* Disable pulls */
        CSL_IOMUX_setPull (ptrIOMuxRegs, pin, 0x1U);
    }
}

/**
 *  @b Description
 *  @n
 *     Function to configure the pull setting for the Pin
 *
 *  @param[in] pin         Pin index for the pinmux pad
 *
 *  @param[in] outputCtrl  Output over-ride config
 *
 *  @param[in] inputCtrl   Input over-ride config
 *
 *
 *  \ingroup DRIVER_PINMUX_FUNCTIONS
 *
 *  @retval
 *      DrvPinmux_ReturnSuccess     - Successfullconfiguration of pulls
 *      DrvPinmux_ReturnInvalidCfg  - Invalid configuration resulting in failure
 */
static void DrvPinmux_OverrideCtrl(uint32_t pin, DrvPinmux_OutOverrideConfig outputCtrl, DrvPinmux_InOverrideConfig inputCtrl)
{
    CSL_mss_iomuxRegs* ptrIOMuxRegs;
    ptrIOMuxRegs = CSL_IOMUX_getBaseAddress ();

    if(outputCtrl == DrvPinmux_OutEnable)
    {
        /* Enable output over-ride and enable output for pad */
        CSL_IOMUX_setOutput (ptrIOMuxRegs, pin, 0x3U);
    }
    else if (outputCtrl == DrvPinmux_OutDisable)
    {
        /* Enable output over-ride and disable output for pad */
        CSL_IOMUX_setOutput (ptrIOMuxRegs, pin, 0x1U);
    }
    else
    {
        /* Disable output over-ride */
        CSL_IOMUX_setOutput (ptrIOMuxRegs, pin, 0x0U);
    }

    if(inputCtrl == DrvPinmux_InEnable)
    {
        /* Enable input over-ride and enable input for pad */
        CSL_IOMUX_setOutput (ptrIOMuxRegs, pin, 0x3U);
    }
    else if (inputCtrl == DrvPinmux_InDisable)
    {
        /* Enable input over-ride and disable input for pad */
        CSL_IOMUX_setOutput (ptrIOMuxRegs, pin, 0x1U);
    }
    else
    {
        /* Disable input over-ride */
        CSL_IOMUX_setOutput (ptrIOMuxRegs, pin, 0x0U);
    }

}

void SBL_UARTAPinmuxConfig(void)
{
    /* unlock the IOs */
    DrvPinmux_UnlockIOs ();

    /* Select Uart Rx as functional mode for PAD DA */
    DrvPinmux_SelectFunc (DRV_PINMUX_PAD_DA, 0x5U);

    /* configure the pulls */
    DrvPinmux_ConfigPulls (DRV_PINMUX_PAD_DA, DrvPinmux_PullUp);

    /* Disable the pad level over-ride */
    DrvPinmux_OverrideCtrl (DRV_PINMUX_PAD_DA, DrvPinmux_OutOverrideDisable, DrvPinmux_InOverrideDisable);

    /* Select Uart Tx as functional mode for PAD DB */
    DrvPinmux_SelectFunc (DRV_PINMUX_PAD_DB, 0x5U);

    /* configure the pulls */
    DrvPinmux_ConfigPulls (DRV_PINMUX_PAD_DB, DrvPinmux_PullUp);

    /* Disable the pad level over-ride */
    DrvPinmux_OverrideCtrl (DRV_PINMUX_PAD_DB, DrvPinmux_OutOverrideDisable, DrvPinmux_InOverrideDisable);

    /* configure the PAD EA (default UARTA Rx) to some other value to avoid conflict */
    /* Select GPIO22 as functional mode for PAD EA */
    DrvPinmux_SelectFunc (DRV_PINMUX_PAD_EA, 0x0U);

    /* configure the PAD EB (default UARTA Tx) to some other value to avoid conflict */
    /* Select GPIO23 as functional mode for PAD EB */
    DrvPinmux_SelectFunc (DRV_PINMUX_PAD_EB, 0x0U);

    /* Lock the IOs */
    DrvPinmux_LockIOs ();
}

/**
 *  @b Description
 *  @n
 *     Function to pinmux the PADS for QSPI
 *
 *  \ingroup DRIVER_PINMUX_FUNCTIONS
 *
 *  @retval
 *      None
 */
void SBL_QSPIPinmuxConfig(void)
{
    /* unlock the IOs */
    DrvPinmux_UnlockIOs ();

    /* Select DOUT as functional mode for PAD AL */
    DrvPinmux_SelectFunc (DRV_PINMUX_PAD_AL, 0x1U);

    /* Disable the pad level over-ride */
    DrvPinmux_OverrideCtrl (DRV_PINMUX_PAD_AL, DrvPinmux_OutOverrideDisable, DrvPinmux_InOverrideDisable);

    /* Select DIN as functional mode for PAD AM */
    DrvPinmux_SelectFunc (DRV_PINMUX_PAD_AM, 0x1U);

    /* Disable the pad level over-ride */
    DrvPinmux_OverrideCtrl (DRV_PINMUX_PAD_AM, DrvPinmux_OutOverrideDisable, DrvPinmux_InOverrideDisable);

    /* Select DIN-0 as functional mode for PAD AN */
    DrvPinmux_SelectFunc (DRV_PINMUX_PAD_AN, 0x1U);

    /* Disable the pad level over-ride */
    DrvPinmux_OverrideCtrl (DRV_PINMUX_PAD_AN, DrvPinmux_OutOverrideDisable, DrvPinmux_InOverrideDisable);

    /* Select DIN-1 as functional mode for PAD AO */
    DrvPinmux_SelectFunc (DRV_PINMUX_PAD_AO, 0x1U);

    /* Disable the pad level over-ride */
    DrvPinmux_OverrideCtrl (DRV_PINMUX_PAD_AO, DrvPinmux_OutOverrideDisable, DrvPinmux_InOverrideDisable);

    /* Select CLK as functional mode for PAD AP */
    DrvPinmux_SelectFunc (DRV_PINMUX_PAD_AP, 0x1U);

    /* Disable the pad level over-ride */
    DrvPinmux_OverrideCtrl (DRV_PINMUX_PAD_AP, DrvPinmux_OutOverrideDisable, DrvPinmux_InOverrideDisable);

    /* Select CS_N as functional mode for PAD AQ */
    DrvPinmux_SelectFunc (DRV_PINMUX_PAD_AQ, 0x1U);

    /* Disable the pad level over-ride */
    DrvPinmux_OverrideCtrl (DRV_PINMUX_PAD_AQ, DrvPinmux_OutOverrideDisable, DrvPinmux_InOverrideDisable);

    /* Lock the IOs */
    DrvPinmux_LockIOs ();
}

