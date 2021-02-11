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
 *  \file adcbuf_soc.c
 *
 *       ADCBUF Low Level Driver SOC specific file.
 *
 */

#ifndef ADCBUF_SOC_H_
#define ADCBUF_SOC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*************************************************************
 * MMWAVE System level defines
 *************************************************************/
#define SYS_COMMON_NUM_RX_CHANNEL                   (4U)

/* Definition for field DMMADCBUFWREN in Register DMMSWINT1 */
#define DMMSWINT1_DMMADCBUFWREN_BIT_START                   (17U)
#define DMMSWINT1_DMMADCBUFWREN_BIT_END                     (17U)

/* Definition for field ADCBUFNUMCHRPPING in Register ADCBUFCFG4 */
#define ADCBUFCFG4_ADCBUFNUMCHRPPING_BIT_START              (16U)
#define ADCBUFCFG4_ADCBUFNUMCHRPPING_BIT_END                (20U)

/* Definition for field ADCBUFNUMCHRPPONG in Register ADCBUFCFG4 */
#define ADCBUFCFG4_ADCBUFNUMCHRPPONG_BIT_START              (21U)
#define ADCBUFCFG4_ADCBUFNUMCHRPPONG_BIT_END                (25U)

/* Definition for field ADCBUFCONTMODEEN in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFCONTMODEEN_BIT_START               (13U)
#define ADCBUFCFG1_ADCBUFCONTMODEEN_BIT_END                 (13U)

/* Definition for field ADCBUFCONTSTRTPL in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFCONTSTRTPL_BIT_START               (14U)
#define ADCBUFCFG1_ADCBUFCONTSTRTPL_BIT_END                 (14U)

/* Definition for field ADCBUFSAMPCNT in Register ADCBUFCFG4 */
#define ADCBUFCFG4_ADCBUFSAMPCNT_BIT_START                  (0U)
#define ADCBUFCFG4_ADCBUFSAMPCNT_BIT_END                    (15U)

/* Definition for field ADCBUFCONTSTOPPL in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFCONTSTOPPL_BIT_START               (15U)
#define ADCBUFCFG1_ADCBUFCONTSTOPPL_BIT_END                 (15U)

/* Definition for field ADCBUFREALONLYMODE in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFREALONLYMODE_BIT_START             (2U)
#define ADCBUFCFG1_ADCBUFREALONLYMODE_BIT_END               (2U)

/* Definition for field ADCBUFIQSWAP in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFIQSWAP_BIT_START                   (5U)
#define ADCBUFCFG1_ADCBUFIQSWAP_BIT_END                     (5U)

/* Definition for field ADCBUFREALONLYMODE in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFREALONLYMODE_BIT_START             (2U)
#define ADCBUFCFG1_ADCBUFREALONLYMODE_BIT_END               (2U)

/* Definition for field ADCBUFWRITEMODE in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFWRITEMODE_BIT_START                (12U)
#define ADCBUFCFG1_ADCBUFWRITEMODE_BIT_END                  (12U)

/* Definition for field RX0EN in Register ADCBUFCFG1 */
#define ADCBUFCFG1_RX0EN_BIT_START                          (6U)
#define ADCBUFCFG1_RX0EN_BIT_END                            (6U)

/* Definition for field RX1EN in Register ADCBUFCFG1 */
#define ADCBUFCFG1_RX1EN_BIT_START                          (7U)
#define ADCBUFCFG1_RX1EN_BIT_END                            (7U)

/* Definition for field RX2EN in Register ADCBUFCFG1 */
#define ADCBUFCFG1_RX2EN_BIT_START                          (8U)
#define ADCBUFCFG1_RX2EN_BIT_END                            (8U)

/* Definition for field RX3EN in Register ADCBUFCFG1 */
#define ADCBUFCFG1_RX3EN_BIT_START                          (9U)
#define ADCBUFCFG1_RX3EN_BIT_END                            (9U)

/* Definition for field ADCBUFADDRX0 in Register ADCBUFCFG2 */
#define ADCBUFCFG2_ADCBUFADDRX0_BIT_START                   (0U)
#define ADCBUFCFG2_ADCBUFADDRX0_BIT_END                     (10U)

/* Definition for field ADCBUFADDRX1 in Register ADCBUFCFG2 */
#define ADCBUFCFG2_ADCBUFADDRX1_BIT_START                   (16U)
#define ADCBUFCFG2_ADCBUFADDRX1_BIT_END                     (26U)

/* Definition for field ADCBUFADDRX2 in Register ADCBUFCFG3 */
#define ADCBUFCFG3_ADCBUFADDRX2_BIT_START                   (0U)
#define ADCBUFCFG3_ADCBUFADDRX2_BIT_END                     (10U)

/* Definition for field ADCBUFADDRX3 in Register ADCBUFCFG3 */
#define ADCBUFCFG3_ADCBUFADDRX3_BIT_START                   (16U)
#define ADCBUFCFG3_ADCBUFADDRX3_BIT_END                     (26U)

/* Definition for field TSTPATRX1IINCR in Register TESTPATTERNRX1ICFG */
#define TESTPATTERNRX1ICFG_TSTPATRX1IINCR_BIT_START         (16U)
#define TESTPATTERNRX1ICFG_TSTPATRX1IINCR_BIT_END           (31U)

/* Definition for field TSTPATRX1IOFFSET in Register TESTPATTERNRX1ICFG */
#define TESTPATTERNRX1ICFG_TSTPATRX1IOFFSET_BIT_START       (0U)
#define TESTPATTERNRX1ICFG_TSTPATRX1IOFFSET_BIT_END         (15U)

/* Definition for field TSTPATRX1QINCR in Register TESTPATTERNRX1QCFG */
#define TESTPATTERNRX1QCFG_TSTPATRX1QINCR_BIT_START         (16U)
#define TESTPATTERNRX1QCFG_TSTPATRX1QINCR_BIT_END           (31U)

/* Definition for field TSTPATRX1QOFFSET in Register TESTPATTERNRX1QCFG */
#define TESTPATTERNRX1QCFG_TSTPATRX1QOFFSET_BIT_START       (0U)
#define TESTPATTERNRX1QCFG_TSTPATRX1QOFFSET_BIT_END         (15U)

/* Definition for field TSTPATRX2IINCR in Register TESTPATTERNRX2ICFG */
#define TESTPATTERNRX2ICFG_TSTPATRX2IINCR_BIT_START         (16U)
#define TESTPATTERNRX2ICFG_TSTPATRX2IINCR_BIT_END           (31U)

/* Definition for field TSTPATRX2IOFFSET in Register TESTPATTERNRX2ICFG */
#define TESTPATTERNRX2ICFG_TSTPATRX2IOFFSET_BIT_START       (0U)
#define TESTPATTERNRX2ICFG_TSTPATRX2IOFFSET_BIT_END         (15U)

/* Definition for field TSTPATRX2QINCR in Register TESTPATTERNRX2QCFG */
#define TESTPATTERNRX2QCFG_TSTPATRX2QINCR_BIT_START         (16U)
#define TESTPATTERNRX2QCFG_TSTPATRX2QINCR_BIT_END           (31U)

/* Definition for field TSTPATRX2QOFFSET in Register TESTPATTERNRX2QCFG */
#define TESTPATTERNRX2QCFG_TSTPATRX2QOFFSET_BIT_START       (0U)
#define TESTPATTERNRX2QCFG_TSTPATRX2QOFFSET_BIT_END         (15U)

/* Definition for field TSTPATRX3IINCR in Register TESTPATTERNRX3ICFG */
#define TESTPATTERNRX3ICFG_TSTPATRX3IINCR_BIT_START         (16U)
#define TESTPATTERNRX3ICFG_TSTPATRX3IINCR_BIT_END           (31U)

/* Definition for field TSTPATRX3IOFFSET in Register TESTPATTERNRX3ICFG */
#define TESTPATTERNRX3ICFG_TSTPATRX3IOFFSET_BIT_START       (0U)
#define TESTPATTERNRX3ICFG_TSTPATRX3IOFFSET_BIT_END         (15U)

/* Definition for field TSTPATRX3QINCR in Register TESTPATTERNRX3QCFG */
#define TESTPATTERNRX3QCFG_TSTPATRX3QINCR_BIT_START         (16U)
#define TESTPATTERNRX3QCFG_TSTPATRX3QINCR_BIT_END           (31U)

/* Definition for field TSTPATRX3QOFFSET in Register TESTPATTERNRX3QCFG */
#define TESTPATTERNRX3QCFG_TSTPATRX3QOFFSET_BIT_START       (0U)
#define TESTPATTERNRX3QCFG_TSTPATRX3QOFFSET_BIT_END         (15U)

/* Definition for field TSTPATRX4IINCR in Register TESTPATTERNRX4ICFG */
#define TESTPATTERNRX4ICFG_TSTPATRX4IINCR_BIT_START         (16U)
#define TESTPATTERNRX4ICFG_TSTPATRX4IINCR_BIT_END           (31U)

/* Definition for field TSTPATRX4IOFFSET in Register TESTPATTERNRX4ICFG */
#define TESTPATTERNRX4ICFG_TSTPATRX4IOFFSET_BIT_START       (0U)
#define TESTPATTERNRX4ICFG_TSTPATRX4IOFFSET_BIT_END         (15U)

/* Definition for field TSTPATRX4QINCR in Register TESTPATTERNRX4QCFG */
#define TESTPATTERNRX4QCFG_TSTPATRX4QINCR_BIT_START         (16U)
#define TESTPATTERNRX4QCFG_TSTPATRX4QINCR_BIT_END           (31U)

/* Definition for field TSTPATRX4QOFFSET in Register TESTPATTERNRX4QCFG */
#define TESTPATTERNRX4QCFG_TSTPATRX4QOFFSET_BIT_START       (0U)
#define TESTPATTERNRX4QCFG_TSTPATRX4QOFFSET_BIT_END         (15U)

/* Definition for field TSTPATVLDCNT in Register TESTPATTERNVLDCFG */
#define TESTPATTERNVLDCFG_TSTPATVLDCNT_BIT_START            (0U)
#define TESTPATTERNVLDCFG_TSTPATVLDCNT_BIT_END              (7U)

/* Definition for field TSTPATVLDCNT in Register TESTPATTERNVLDCFG */
#define TESTPATTERNVLDCFG_TSTPATVLDCNT_BIT_START            (0U)
#define TESTPATTERNVLDCFG_TSTPATVLDCNT_BIT_END              (7U)

/* Definition for field TSTPATGENEN in Register TESTPATTERNVLDCFG */
#define TESTPATTERNVLDCFG_TSTPATGENEN_BIT_START             (8U)
#define TESTPATTERNVLDCFG_TSTPATGENEN_BIT_END               (10U)

/* Definition for field CQDATAWIDTH in Register CQCFG1 */
#define CQCFG1_CQDATAWIDTH_BIT_START                        (0U)
#define CQCFG1_CQDATAWIDTH_BIT_END                          (1U)

/* Definition for field CQ96BITPACKEN in Register CQCFG1 */
#define CQCFG1_CQ96BITPACKEN_BIT_START                      (3U)
#define CQCFG1_CQ96BITPACKEN_BIT_END                        (3U)

/* Definition for field CQ0BASEADDR in Register CQCFG1 */
#define CQCFG1_CQ0BASEADDR_BIT_START                        (4U)
#define CQCFG1_CQ0BASEADDR_BIT_END                          (12U)

/* Definition for field CQ1BASEADDR in Register CQCFG1 */
#define CQCFG1_CQ1BASEADDR_BIT_START                        (13U)
#define CQCFG1_CQ1BASEADDR_BIT_END                          (21U)

/* Definition for field CQ2BASEADDR in Register CQCFG1 */
#define CQCFG1_CQ2BASEADDR_BIT_START                        (22U)
#define CQCFG1_CQ2BASEADDR_BIT_END                          (30U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ADCBUF_SOC_H_ */
