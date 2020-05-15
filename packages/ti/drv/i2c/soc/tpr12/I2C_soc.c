/**
 *  \file   tpr12/I2C_soc.c
 *
 *  \brief  TPR12 SoC specific I2C hardware attributes.
 *
 *   This file contains the hardware attributes of I2C peripheral like
 *   base address, interrupt number etc.
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

#include <stdint.h>
#include <ti/csl/soc.h>
#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/csl/csl_i2c.h>

#define I2C_MODULE_FREQ_200M    (200000000U)

#ifdef BUILD_MCU
#define CSL_I2C_CNT    CSL_MSS_I2C_PER_CNT

/**
 * @brief   This is the TPR12 MSS specific I2C configuration. There are
 * 3 I2C instances (MSS-I2C, RCSS-I2C-A/B) available on the MSS. 
 */

/* I2C configuration structure */
I2C_HwAttrs i2cInitCfg[CSL_I2C_CNT] =
{
    {
        CSL_MSS_I2C_U_BASE,
        CSL_MSS_INTR_MSS_I2C_INT, /* I2C int0 number for R5F INTC */
        0, /* Event ID not used for R5F INTC */
        INVALID_INTC_MUX_NUM, /* CIC num not used in R5F GIC */
        0, /* cicEventId not used for ARM */
        0, /* HostIntNum not used for ARM */
        I2C_MODULE_FREQ_200M,  /* default I2C frequency, system clock/6 */
        true, /* interrupt mode enabled */
        I2C_OWN_ADDR, /* default I2C own slave addresse */
    },
    {
        CSL_RCSS_I2CA_U_BASE,
        CSL_MSS_INTR_RCSS_I2CA_INT, /* I2C int0 number for R5F INTC */
        0, /* Event ID not used for R5F INTC */
        INVALID_INTC_MUX_NUM, /* CIC num not used in R5F GIC */
        0, /* cicEventId not used for ARM */
        0, /* HostIntNum not used for ARM */
        I2C_MODULE_FREQ_200M,  /* default I2C frequency, system clock/6 */
        true, /* interrupt mode enabled */
        I2C_OWN_ADDR, /* default I2C own slave addresse */
    },
    {
        CSL_RCSS_I2CB_U_BASE,
        CSL_MSS_INTR_RCSS_I2CB_INT, /* I2C int0 number for R5F INTC */
        0, /* Event ID not used for R5F INTC */
        INVALID_INTC_MUX_NUM, /* CIC num not used in R5F GIC */
        0, /* cicEventId not used for ARM */
        0, /* HostIntNum not used for ARM */
        I2C_MODULE_FREQ_200M,  /* default I2C frequency, system clock/6 */
        true, /* interrupt mode enabled */
        I2C_OWN_ADDR, /* default I2C own slave addresse */
    }
};

/* I2C objects */
I2C_v0_Object I2cObjects[CSL_I2C_CNT];

/* I2C configuration structure */
I2C_config_list I2C_config = {
    {
        &I2C_v0_FxnTable,
        &I2cObjects[0],
        &i2cInitCfg[0]
    },

    {
        &I2C_v0_FxnTable,
        &I2cObjects[1],
        &i2cInitCfg[1]
    },

    {
        &I2C_v0_FxnTable,
        &I2cObjects[2],
        &i2cInitCfg[2]
    },

    /*"pad to full predefined length of array"*/
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL}
};

#elif defined (BUILD_DSP_1)

#define CSL_I2C_CNT    CSL_DSS_I2C_PER_CNT

/**
 * @brief   This is the TPR12 DSS specific I2C configuration. There are
 * 2 I2C instances (RCSS-I2C-A/B) available on the DSS. 
 */

/* I2C configuration structure */
I2C_HwAttrs i2cInitCfg[CSL_I2C_CNT] =
{
    {
        CSL_RCSS_I2CA_U_BASE,
        OSAL_REGINT_INTVEC_EVENT_COMBINER,  /* default DSP Interrupt vector number */
        CSL_DSS_INTR_RCSS_I2CA_INT, /*  DSP INTC I2C Event ID */
        INVALID_INTC_MUX_NUM, /* CIC num not used for TPR12 */
        0, /* cicEventId not used for TPR12 */
        0, /* HostIntNum not used for TPR12 */
        I2C_MODULE_FREQ_200M,  /* default I2C frequency, system clock/6 */
        true, /* interrupt mode enabled */
        I2C_OWN_ADDR, /* default I2C own slave addresse */
    },
    {
        CSL_RCSS_I2CB_U_BASE,
        OSAL_REGINT_INTVEC_EVENT_COMBINER,  /* default DSP Interrupt vector number */
        CSL_DSS_INTR_RCSS_I2CB_INT, /*  DSP INTC I2C Event ID */
        INVALID_INTC_MUX_NUM, /* CIC num not used for TPR12 */
        0, /* cicEventId not used for TPR12 */
        0, /* HostIntNum not used for TPR12 */
        I2C_MODULE_FREQ_200M,  /* default I2C frequency, system clock/6 */
        true, /* interrupt mode enabled */
        I2C_OWN_ADDR, /* default I2C own slave addresse */
    }
};

/* I2C objects */
I2C_v0_Object I2cObjects[CSL_I2C_CNT];

/* I2C configuration structure */
I2C_config_list I2C_config = {
    {
        &I2C_v0_FxnTable,
        &I2cObjects[0],
        &i2cInitCfg[0]
    },

    {
        &I2C_v0_FxnTable,
        &I2cObjects[1],
        &i2cInitCfg[1]
    },

    /*"pad to full predefined length of array"*/
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL}
};

#else
#error "Error: Please check the compiler flags since BUILD_XXX is not defined for the TPR12 device"
#endif


/**
 * \brief  This API gets the SoC level of I2C intial configuration
 *
 * \param  index     I2C instance index.
 * \param  cfg       Pointer to I2C SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t I2C_socGetInitCfg(uint32_t index, I2C_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_I2C_CNT)
    {
        *cfg = i2cInitCfg[index];
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of I2C intial configuration
 *
 * \param  index     I2C instance index.
 * \param  cfg       Pointer to I2C SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t I2C_socSetInitCfg(uint32_t index, const I2C_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_I2C_CNT)
    {
        i2cInitCfg[index] = *cfg;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

