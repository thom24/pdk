/*
 * Copyright (c) 2022 Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file sciserver_secproxyConfigData.h
 *
 * \brief SoC defines for secure proxy configs for J784S4 device
 *
 */

#ifndef SOC_J784S4_SPROXY_CONFIG_DATA_H
#define SOC_J784S4_SPROXY_CONFIG_DATA_H

/** Number of Secure Proxy Transmit (Tx) thread configurations */
#define SOC_MAX_SPT_RX_CONFIG_INSTANCES                                                   (0x54U)

#define J784S4_NAVSS0_SEC_PROXY_0_RX_TIFS_SEC_HIGH_PRIORITY_RX_THR369_CONF000             (0x00U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_TIFS_SEC_LOW_PRIORITY_RX_THR368_CONF001              (0x01U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_TIFS_SEC_NOTIFY_RESP_RX_THR367_CONF002               (0x02U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_DM_NONSEC_HIGH_PRIORITY_RX_THR342_CONF003            (0x03U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_DM_NONSEC_LOW_PRIORITY_RX_THR341_CONF004             (0x04U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_DM_NONSEC_NOTIFY_RESP_RX_THR340_CONF005              (0x05U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_0_NOTIFY_THR000_CONF006                          (0x06U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_0_RESPONSE_THR001_CONF007                        (0x07U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_1_NOTIFY_THR005_CONF008                          (0x08U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_1_RESPONSE_THR006_CONF009                        (0x09U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_2_NOTIFY_THR010_CONF010                          (0x0AU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_2_RESPONSE_THR011_CONF011                        (0x0BU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_3_NOTIFY_THR015_CONF012                          (0x0CU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_3_RESPONSE_THR016_CONF013                        (0x0DU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_4_NOTIFY_THR020_CONF014                          (0x0EU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_4_RESPONSE_THR021_CONF015                        (0x0FU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_5_NOTIFY_THR025_CONF016                          (0x10U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_5_RESPONSE_THR026_CONF017                        (0x11U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_6_NOTIFY_THR030_CONF018                          (0x12U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_6_RESPONSE_THR031_CONF019                        (0x13U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_7_NOTIFY_THR035_CONF020                          (0x14U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_A72_7_RESPONSE_THR036_CONF021                        (0x15U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_0_0_NOTIFY_THR040_CONF022                        (0x16U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_0_0_RESPONSE_THR041_CONF023                      (0x17U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_0_1_NOTIFY_THR045_CONF024                        (0x18U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_0_1_RESPONSE_THR046_CONF025                      (0x19U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_1_0_NOTIFY_THR050_CONF026                        (0x1AU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_1_0_RESPONSE_THR051_CONF027                      (0x1BU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_1_1_NOTIFY_THR055_CONF028                        (0x1CU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_1_1_RESPONSE_THR056_CONF029                      (0x1DU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_2_0_NOTIFY_THR060_CONF030                        (0x1EU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_2_0_RESPONSE_THR061_CONF031                      (0x1FU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_2_1_NOTIFY_THR065_CONF032                        (0x20U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_2_1_RESPONSE_THR066_CONF033                      (0x21U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_3_0_NOTIFY_THR070_CONF034                        (0x22U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_3_0_RESPONSE_THR071_CONF035                      (0x23U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_3_1_NOTIFY_THR075_CONF036                        (0x24U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_C7X_3_1_RESPONSE_THR076_CONF037                      (0x25U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_GPU_0_NOTIFY_THR080_CONF038                          (0x26U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_GPU_0_RESPONSE_THR081_CONF039                        (0x27U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_0_R5_0_NOTIFY_THR085_CONF040                    (0x28U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_0_R5_0_RESPONSE_THR086_CONF041                  (0x29U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_0_R5_1_NOTIFY_THR090_CONF042                    (0x2AU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_0_R5_1_RESPONSE_THR091_CONF043                  (0x2BU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_0_R5_2_NOTIFY_THR095_CONF044                    (0x2CU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_0_R5_2_RESPONSE_THR096_CONF045                  (0x2DU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_0_R5_3_NOTIFY_THR100_CONF046                    (0x2EU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_0_R5_3_RESPONSE_THR101_CONF047                  (0x2FU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_1_R5_0_NOTIFY_THR105_CONF048                    (0x30U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_1_R5_0_RESPONSE_THR106_CONF049                  (0x31U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_1_R5_1_NOTIFY_THR110_CONF050                    (0x32U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_1_R5_1_RESPONSE_THR111_CONF051                  (0x33U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_1_R5_2_NOTIFY_THR115_CONF052                    (0x34U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_1_R5_2_RESPONSE_THR116_CONF053                  (0x35U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_1_R5_3_NOTIFY_THR120_CONF054                    (0x36U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_1_R5_3_RESPONSE_THR121_CONF055                  (0x37U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_2_R5_0_NOTIFY_THR125_CONF056                    (0x38U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_2_R5_0_RESPONSE_THR126_CONF057                  (0x39U)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_2_R5_1_NOTIFY_THR130_CONF058                    (0x3AU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_2_R5_1_RESPONSE_THR131_CONF059                  (0x3BU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_2_R5_2_NOTIFY_THR135_CONF060                    (0x3CU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_2_R5_2_RESPONSE_THR136_CONF061                  (0x3DU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_2_R5_3_NOTIFY_THR140_CONF062                    (0x3EU)
#define J784S4_NAVSS0_SEC_PROXY_0_RX_MAIN_2_R5_3_RESPONSE_THR141_CONF063                  (0x3FU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_TIFS_SEC_HIGH_PRIORITY_RX_THR089_CONF064          (0x40U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_TIFS_SEC_LOW_PRIORITY_RX_THR088_CONF065           (0x41U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_TIFS_SEC_NOTIFY_RESP_RX_THR087_CONF066            (0x42U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_DM_NONSEC_HIGH_PRIORITY_RX_THR080_CONF067         (0x43U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_DM_NONSEC_LOW_PRIORITY_RX_THR079_CONF068          (0x44U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_DM_NONSEC_NOTIFY_RESP_RX_THR078_CONF069           (0x45U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_MCU_0_R5_0_NOTIFY_THR000_CONF070                  (0x46U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_MCU_0_R5_0_RESPONSE_THR001_CONF071                (0x47U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_MCU_0_R5_1_NOTIFY_THR005_CONF072                  (0x48U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_MCU_0_R5_1_RESPONSE_THR006_CONF073                (0x49U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_MCU_0_R5_2_NOTIFY_THR010_CONF074                  (0x4AU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_MCU_0_R5_2_RESPONSE_THR011_CONF075                (0x4BU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_MCU_0_R5_3_NOTIFY_THR015_CONF076                  (0x4CU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_MCU_0_R5_3_RESPONSE_THR016_CONF077                (0x4DU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_DM2TIFS_NOTIFY_THR020_CONF078                     (0x4EU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_DM2TIFS_RESPONSE_THR021_CONF079                   (0x4FU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_TIFS2DM_NOTIFY_THR025_CONF080                     (0x50U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_RX_TIFS2DM_RESPONSE_THR026_CONF081                   (0x51U)
#define J784S4_MCU_SA3_SS0_SEC_PROXY_0_RX_TIFS_HSM_SEC_LOW_PRIORITY_RX_THR015_CONF082     (0x52U)
#define J784S4_MCU_SA3_SS0_SEC_PROXY_0_RX_HSM_RESPONSE_THR000_CONF083                     (0x53U)

/** Number of Secure Proxy Transmit (Rx) thread configurations */
#define SOC_MAX_SPT_TX_CONFIG_INSTANCES                                                   (0xB1U)

#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_A72_0_NOTIFY_TX_THR366_CONF000              (0x00U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_A72_0_RESPONSE_TX_THR365_CONF001            (0x01U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_A72_1_NOTIFY_TX_THR364_CONF002              (0x02U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_A72_1_RESPONSE_TX_THR363_CONF003            (0x03U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_C7X_0_0_NOTIFY_TX_THR362_CONF004            (0x04U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_C7X_0_0_RESPONSE_TX_THR361_CONF005          (0x05U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_C7X_1_0_NOTIFY_TX_THR360_CONF006            (0x06U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_C7X_1_0_RESPONSE_TX_THR359_CONF007          (0x07U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_C7X_2_0_NOTIFY_TX_THR358_CONF008            (0x08U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_C7X_2_0_RESPONSE_TX_THR357_CONF009          (0x09U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_C7X_3_0_NOTIFY_TX_THR356_CONF010            (0x0AU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_C7X_3_0_RESPONSE_TX_THR355_CONF011          (0x0BU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_0_R5_1_NOTIFY_TX_THR354_CONF012        (0x0CU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_0_R5_1_RESPONSE_TX_THR353_CONF013      (0x0DU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_0_R5_3_NOTIFY_TX_THR352_CONF014        (0x0EU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_0_R5_3_RESPONSE_TX_THR351_CONF015      (0x0FU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_1_R5_1_NOTIFY_TX_THR350_CONF016        (0x10U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_1_R5_1_RESPONSE_TX_THR349_CONF017      (0x11U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_1_R5_3_NOTIFY_TX_THR348_CONF018        (0x12U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_1_R5_3_RESPONSE_TX_THR347_CONF019      (0x13U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_2_R5_1_NOTIFY_TX_THR346_CONF020        (0x14U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_2_R5_1_RESPONSE_TX_THR345_CONF021      (0x15U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_2_R5_3_NOTIFY_TX_THR344_CONF022        (0x16U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_TIFS_SEC_MAIN_2_R5_3_RESPONSE_TX_THR343_CONF023      (0x17U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_2_NOTIFY_TX_THR339_CONF024             (0x18U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_2_RESPONSE_TX_THR338_CONF025           (0x19U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_3_NOTIFY_TX_THR337_CONF026             (0x1AU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_3_RESPONSE_TX_THR336_CONF027           (0x1BU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_4_NOTIFY_TX_THR335_CONF028             (0x1CU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_4_RESPONSE_TX_THR334_CONF029           (0x1DU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_5_NOTIFY_TX_THR333_CONF030             (0x1EU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_5_RESPONSE_TX_THR332_CONF031           (0x1FU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_6_NOTIFY_TX_THR331_CONF032             (0x20U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_6_RESPONSE_TX_THR330_CONF033           (0x21U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_7_NOTIFY_TX_THR329_CONF034             (0x22U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_7_RESPONSE_TX_THR328_CONF035           (0x23U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_0_1_NOTIFY_TX_THR327_CONF036           (0x24U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_0_1_RESPONSE_TX_THR326_CONF037         (0x25U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_1_1_NOTIFY_TX_THR325_CONF038           (0x26U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_1_1_RESPONSE_TX_THR324_CONF039         (0x27U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_2_1_NOTIFY_TX_THR323_CONF040           (0x28U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_2_1_RESPONSE_TX_THR322_CONF041         (0x29U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_3_1_NOTIFY_TX_THR321_CONF042           (0x2AU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_3_1_RESPONSE_TX_THR320_CONF043         (0x2BU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_GPU_0_NOTIFY_TX_THR319_CONF044             (0x2CU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_GPU_0_RESPONSE_TX_THR318_CONF045           (0x2DU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_0_NOTIFY_TX_THR317_CONF046       (0x2EU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_0_RESPONSE_TX_THR316_CONF047     (0x2FU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_2_NOTIFY_TX_THR315_CONF048       (0x30U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_2_RESPONSE_TX_THR314_CONF049     (0x31U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_1_R5_0_NOTIFY_TX_THR313_CONF050       (0x32U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_1_R5_0_RESPONSE_TX_THR312_CONF051     (0x33U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_1_R5_2_NOTIFY_TX_THR311_CONF052       (0x34U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_1_R5_2_RESPONSE_TX_THR310_CONF053     (0x35U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_2_R5_0_NOTIFY_TX_THR309_CONF054       (0x36U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_2_R5_0_RESPONSE_TX_THR308_CONF055     (0x37U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_2_R5_2_NOTIFY_TX_THR307_CONF056       (0x38U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_2_R5_2_RESPONSE_TX_THR306_CONF057     (0x39U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_0_HIGH_PRIORITY_THR002_CONF058                   (0x3AU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_0_LOW_PRIORITY_THR003_CONF059                    (0x3BU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_0_NOTIFY_RESP_THR004_CONF060                     (0x3CU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_1_HIGH_PRIORITY_THR007_CONF061                   (0x3DU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_1_LOW_PRIORITY_THR008_CONF062                    (0x3EU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_1_NOTIFY_RESP_THR009_CONF063                     (0x3FU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_2_HIGH_PRIORITY_THR012_CONF064                   (0x40U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_2_LOW_PRIORITY_THR013_CONF065                    (0x41U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_2_NOTIFY_RESP_THR014_CONF066                     (0x42U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_3_HIGH_PRIORITY_THR017_CONF067                   (0x43U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_3_LOW_PRIORITY_THR018_CONF068                    (0x44U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_3_NOTIFY_RESP_THR019_CONF069                     (0x45U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_4_HIGH_PRIORITY_THR022_CONF070                   (0x46U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_4_LOW_PRIORITY_THR023_CONF071                    (0x47U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_4_NOTIFY_RESP_THR024_CONF072                     (0x48U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_5_HIGH_PRIORITY_THR027_CONF073                   (0x49U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_5_LOW_PRIORITY_THR028_CONF074                    (0x4AU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_5_NOTIFY_RESP_THR029_CONF075                     (0x4BU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_6_HIGH_PRIORITY_THR032_CONF076                   (0x4CU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_6_LOW_PRIORITY_THR033_CONF077                    (0x4DU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_6_NOTIFY_RESP_THR034_CONF078                     (0x4EU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_7_HIGH_PRIORITY_THR037_CONF079                   (0x4FU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_7_LOW_PRIORITY_THR038_CONF080                    (0x50U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_A72_7_NOTIFY_RESP_THR039_CONF081                     (0x51U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_0_0_HIGH_PRIORITY_THR042_CONF082                 (0x52U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_0_0_LOW_PRIORITY_THR043_CONF083                  (0x53U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_0_0_NOTIFY_RESP_THR044_CONF084                   (0x54U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_0_1_HIGH_PRIORITY_THR047_CONF085                 (0x55U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_0_1_LOW_PRIORITY_THR048_CONF086                  (0x56U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_0_1_NOTIFY_RESP_THR049_CONF087                   (0x57U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_1_0_HIGH_PRIORITY_THR052_CONF088                 (0x58U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_1_0_LOW_PRIORITY_THR053_CONF089                  (0x59U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_1_0_NOTIFY_RESP_THR054_CONF090                   (0x5AU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_1_1_HIGH_PRIORITY_THR057_CONF091                 (0x5BU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_1_1_LOW_PRIORITY_THR058_CONF092                  (0x5CU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_1_1_NOTIFY_RESP_THR059_CONF093                   (0x5DU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_2_0_HIGH_PRIORITY_THR062_CONF094                 (0x5EU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_2_0_LOW_PRIORITY_THR063_CONF095                  (0x5FU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_2_0_NOTIFY_RESP_THR064_CONF096                   (0x60U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_2_1_HIGH_PRIORITY_THR067_CONF097                 (0x61U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_2_1_LOW_PRIORITY_THR068_CONF098                  (0x62U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_2_1_NOTIFY_RESP_THR069_CONF099                   (0x63U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_3_0_HIGH_PRIORITY_THR072_CONF100                 (0x64U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_3_0_LOW_PRIORITY_THR073_CONF101                  (0x65U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_3_0_NOTIFY_RESP_THR074_CONF102                   (0x66U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_3_1_HIGH_PRIORITY_THR077_CONF103                 (0x67U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_3_1_LOW_PRIORITY_THR078_CONF104                  (0x68U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_C7X_3_1_NOTIFY_RESP_THR079_CONF105                   (0x69U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_GPU_0_HIGH_PRIORITY_THR082_CONF106                   (0x6AU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_GPU_0_LOW_PRIORITY_THR083_CONF107                    (0x6BU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_GPU_0_NOTIFY_RESP_THR084_CONF108                     (0x6CU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_0_HIGH_PRIORITY_THR087_CONF109             (0x6DU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_0_LOW_PRIORITY_THR088_CONF110              (0x6EU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_0_NOTIFY_RESP_THR089_CONF111               (0x6FU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_1_HIGH_PRIORITY_THR092_CONF112             (0x70U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_1_LOW_PRIORITY_THR093_CONF113              (0x71U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_1_NOTIFY_RESP_THR094_CONF114               (0x72U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_2_HIGH_PRIORITY_THR097_CONF115             (0x73U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_2_LOW_PRIORITY_THR098_CONF116              (0x74U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_2_NOTIFY_RESP_THR099_CONF117               (0x75U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_3_HIGH_PRIORITY_THR102_CONF118             (0x76U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_3_LOW_PRIORITY_THR103_CONF119              (0x77U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_0_R5_3_NOTIFY_RESP_THR104_CONF120               (0x78U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_0_HIGH_PRIORITY_THR107_CONF121             (0x79U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_0_LOW_PRIORITY_THR108_CONF122              (0x7AU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_0_NOTIFY_RESP_THR109_CONF123               (0x7BU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_1_HIGH_PRIORITY_THR112_CONF124             (0x7CU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_1_LOW_PRIORITY_THR113_CONF125              (0x7DU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_1_NOTIFY_RESP_THR114_CONF126               (0x7EU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_2_HIGH_PRIORITY_THR117_CONF127             (0x7FU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_2_LOW_PRIORITY_THR118_CONF128              (0x80U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_2_NOTIFY_RESP_THR119_CONF129               (0x81U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_3_HIGH_PRIORITY_THR122_CONF130             (0x82U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_3_LOW_PRIORITY_THR123_CONF131              (0x83U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_1_R5_3_NOTIFY_RESP_THR124_CONF132               (0x84U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_0_HIGH_PRIORITY_THR127_CONF133             (0x85U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_0_LOW_PRIORITY_THR128_CONF134              (0x86U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_0_NOTIFY_RESP_THR129_CONF135               (0x87U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_1_HIGH_PRIORITY_THR132_CONF136             (0x88U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_1_LOW_PRIORITY_THR133_CONF137              (0x89U)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_1_NOTIFY_RESP_THR134_CONF138               (0x8AU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_2_HIGH_PRIORITY_THR137_CONF139             (0x8BU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_2_LOW_PRIORITY_THR138_CONF140              (0x8CU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_2_NOTIFY_RESP_THR139_CONF141               (0x8DU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_3_HIGH_PRIORITY_THR142_CONF142             (0x8EU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_3_LOW_PRIORITY_THR143_CONF143              (0x8FU)
#define J784S4_NAVSS0_SEC_PROXY_0_TX_MAIN_2_R5_3_NOTIFY_RESP_THR144_CONF144               (0x90U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_TIFS_SEC_MCU_0_R5_1_NOTIFY_TX_THR086_CONF145      (0x91U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_TIFS_SEC_MCU_0_R5_1_RESPONSE_TX_THR085_CONF146    (0x92U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_TIFS_SEC_MCU_0_R5_3_NOTIFY_TX_THR084_CONF147      (0x93U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_TIFS_SEC_MCU_0_R5_3_RESPONSE_TX_THR083_CONF148    (0x94U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_TIFS_SEC_DM2TIFS_NOTIFY_TX_THR082_CONF149         (0x95U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_TIFS_SEC_DM2TIFS_RESPONSE_TX_THR081_CONF150       (0x96U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_0_NOTIFY_TX_THR077_CONF151     (0x97U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_0_RESPONSE_TX_THR076_CONF152   (0x98U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_2_NOTIFY_TX_THR075_CONF153     (0x99U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_2_RESPONSE_TX_THR074_CONF154   (0x9AU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_TIFS2DM_NOTIFY_TX_THR073_CONF155        (0x9BU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_TIFS2DM_RESPONSE_TX_THR072_CONF156      (0x9CU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_0_HIGH_PRIORITY_THR002_CONF157           (0x9DU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_0_LOW_PRIORITY_THR003_CONF158            (0x9EU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_0_NOTIFY_RESP_THR004_CONF159             (0x9FU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_1_HIGH_PRIORITY_THR007_CONF160           (0xA0U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_1_LOW_PRIORITY_THR008_CONF161            (0xA1U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_1_NOTIFY_RESP_THR009_CONF162             (0xA2U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_2_HIGH_PRIORITY_THR012_CONF163           (0xA3U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_2_LOW_PRIORITY_THR013_CONF164            (0xA4U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_2_NOTIFY_RESP_THR014_CONF165             (0xA5U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_3_HIGH_PRIORITY_THR017_CONF166           (0xA6U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_3_LOW_PRIORITY_THR018_CONF167            (0xA7U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_MCU_0_R5_3_NOTIFY_RESP_THR019_CONF168             (0xA8U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM2TIFS_HIGH_PRIORITY_THR022_CONF169              (0xA9U)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM2TIFS_LOW_PRIORITY_THR023_CONF170               (0xAAU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM2TIFS_NOTIFY_RESP_THR024_CONF171                (0xABU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_TIFS2DM_HIGH_PRIORITY_THR027_CONF172              (0xACU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_TIFS2DM_LOW_PRIORITY_THR028_CONF173               (0xADU)
#define J784S4_MCU_NAVSS0_SEC_PROXY0_TX_TIFS2DM_NOTIFY_RESP_THR029_CONF174                (0xAEU)
#define J784S4_MCU_SA3_SS0_SEC_PROXY_0_TX_TIFS_HSM_SEC_HSM_RESPONSE_TX_THR014_CONF175     (0xAFU)
#define J784S4_MCU_SA3_SS0_SEC_PROXY_0_TX_HSM_LOW_PRIORITY_THR001_CONF176                 (0xB0U)

#endif /* SOC_J784S4_SPROXY_CONFIG_DATA_H */
