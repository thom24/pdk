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


/*! \file   psc.h
 *  \brief  MAIN PSC Register Offsets and Reset values
 *  \author Nanthakumar S nantha@ti.com
 */

#ifndef _PSC_H_
#define _PSC_H_

#include <ti/drv/lpm/include/io_retention/dev_info.h> 
#include <stdint.h>

/* scate_tb translate_on */

#define PSC_PID_OFFSET        0x000

#define PSC_GBLCTL_OFFSET     0x010
#define PSC_GBLSTAT_OFFSET    0x014
#define PSC_INTEVAL_OFFSET    0x018
#define PSC_IPWKCNT_OFFSET    0x01C

#define PSC_MERRPR0_OFFSET    0x040
#define PSC_MERRPR1_OFFSET    0x044
#define PSC_UNUSD_REG0_OFFSET 0x048
#define PSC_UNUSD_REG1_OFFSET 0x04C
#define PSC_MERRCR0_OFFSET    0x050
#define PSC_MERRCR1_OFFSET    0x054
#define PSC_UNUSD_REG2_OFFSET 0x058
#define PSC_UNUSD_REG3_OFFSET 0x05C
#define PSC_PERRPR_OFFSET     0x060
#define PSC_PERRPR_H_OFFSET   0x064
#define PSC_PERRCR_OFFSET     0x068
#define PSC_PERRCR_H_OFFSET   0x06C
#define PSC_EPCPR_OFFSET      0x070
#define PSC_EPCPR_H_OFFSET    0x074
#define PSC_EPCCR_OFFSET      0x078
#define PSC_EPCCR_H_OFFSET    0x07C
#define PSC_RAILSTAT_OFFSET   0x100
#define PSC_RAILCTL_OFFSET    0x104
#define PSC_RAILSEL_OFFSET    0x108
/*#define PSC_RAILSEL_H_OFFSET  0x10C */

#define PSC_PTCMD_OFFSET      0x120
#define PSC_PTCMD_H_OFFSET    0x124
#define PSC_PTSTAT_OFFSET     0x128
#define PSC_PTSTAT_H_OFFSET   0x12C

#define PSC_PDSTAT00_OFFSET   0x200
#define PSC_PDSTAT01_OFFSET   0x204
#define PSC_PDSTAT02_OFFSET   0x208
#define PSC_PDSTAT03_OFFSET   0x20C
#define PSC_PDSTAT04_OFFSET   0x210
#define PSC_PDSTAT05_OFFSET   0x214
#define PSC_PDSTAT06_OFFSET   0x218
#define PSC_PDSTAT07_OFFSET   0x21C
#define PSC_PDSTAT08_OFFSET   0x220
#define PSC_PDSTAT09_OFFSET   0x224
#define PSC_PDSTAT10_OFFSET   0x228
#define PSC_PDSTAT11_OFFSET   0x22C
#define PSC_PDSTAT12_OFFSET   0x230
#define PSC_PDSTAT13_OFFSET   0x234
#define PSC_PDSTAT14_OFFSET   0x238
#define PSC_PDSTAT15_OFFSET   0x23C
#define PSC_PDSTAT16_OFFSET   0x240
#define PSC_PDSTAT17_OFFSET   0x244
#define PSC_PDSTAT18_OFFSET   0x248
#define PSC_PDSTAT19_OFFSET   0x24C
#define PSC_PDSTAT20_OFFSET   0x250
#define PSC_PDSTAT21_OFFSET   0x254
#define PSC_PDSTAT22_OFFSET   0x258
#define PSC_PDSTAT23_OFFSET   0x25C
#define PSC_PDSTAT24_OFFSET   0x260
#define PSC_PDSTAT25_OFFSET   0x264
#define PSC_PDSTAT26_OFFSET   0x268
#define PSC_PDSTAT27_OFFSET   0x26C
#define PSC_PDSTAT28_OFFSET   0x270
#define PSC_PDSTAT29_OFFSET   0x274
#define PSC_PDSTAT30_OFFSET   0x278
#define PSC_PDSTAT31_OFFSET   0x27C

#define PSC_PDCTL00_OFFSET    0x300
#define PSC_PDCTL01_OFFSET    0x304
#define PSC_PDCTL02_OFFSET    0x308
#define PSC_PDCTL03_OFFSET    0x30C
#define PSC_PDCTL04_OFFSET    0x310
#define PSC_PDCTL05_OFFSET    0x314
#define PSC_PDCTL06_OFFSET    0x318
#define PSC_PDCTL07_OFFSET    0x31C
#define PSC_PDCTL08_OFFSET    0x320
#define PSC_PDCTL09_OFFSET    0x324
#define PSC_PDCTL10_OFFSET    0x328
#define PSC_PDCTL11_OFFSET    0x32C
#define PSC_PDCTL12_OFFSET    0x330
#define PSC_PDCTL13_OFFSET    0x334
#define PSC_PDCTL14_OFFSET    0x338
#define PSC_PDCTL15_OFFSET    0x33C
#define PSC_PDCTL16_OFFSET    0x340
#define PSC_PDCTL17_OFFSET    0x344
#define PSC_PDCTL18_OFFSET    0x348
#define PSC_PDCTL19_OFFSET    0x34C
#define PSC_PDCTL20_OFFSET    0x350
#define PSC_PDCTL21_OFFSET    0x354
#define PSC_PDCTL22_OFFSET    0x358
#define PSC_PDCTL23_OFFSET    0x35C
#define PSC_PDCTL24_OFFSET    0x360
#define PSC_PDCTL25_OFFSET    0x364
#define PSC_PDCTL26_OFFSET    0x368
#define PSC_PDCTL27_OFFSET    0x36C
#define PSC_PDCTL28_OFFSET    0x370
#define PSC_PDCTL29_OFFSET    0x374
#define PSC_PDCTL30_OFFSET    0x378
#define PSC_PDCTL31_OFFSET    0x37C

#define PSC_PDCFG00_OFFSET    0x400
#define PSC_PDCFG01_OFFSET    0x404
#define PSC_PDCFG02_OFFSET    0x408
#define PSC_PDCFG03_OFFSET    0x40C
#define PSC_PDCFG04_OFFSET    0x410
#define PSC_PDCFG05_OFFSET    0x414
#define PSC_PDCFG06_OFFSET    0x418
#define PSC_PDCFG07_OFFSET    0x41C
#define PSC_PDCFG08_OFFSET    0x420
#define PSC_PDCFG09_OFFSET    0x424
#define PSC_PDCFG10_OFFSET    0x428
#define PSC_PDCFG11_OFFSET    0x42C
#define PSC_PDCFG12_OFFSET    0x430
#define PSC_PDCFG13_OFFSET    0x434
#define PSC_PDCFG14_OFFSET    0x438
#define PSC_PDCFG15_OFFSET    0x43C
#define PSC_PDCFG16_OFFSET    0x440
#define PSC_PDCFG17_OFFSET    0x444
#define PSC_PDCFG18_OFFSET    0x448
#define PSC_PDCFG19_OFFSET    0x44C
#define PSC_PDCFG20_OFFSET    0x450
#define PSC_PDCFG21_OFFSET    0x454
#define PSC_PDCFG22_OFFSET    0x458
#define PSC_PDCFG23_OFFSET    0x45C
#define PSC_PDCFG24_OFFSET    0x460
#define PSC_PDCFG25_OFFSET    0x464
#define PSC_PDCFG26_OFFSET    0x468
#define PSC_PDCFG27_OFFSET    0x46C
#define PSC_PDCFG28_OFFSET    0x470
#define PSC_PDCFG29_OFFSET    0x474
#define PSC_PDCFG30_OFFSET    0x478
#define PSC_PDCFG31_OFFSET    0x47C

#define PSC_MDCFG00_OFFSET    0x600
#define PSC_MDCFG01_OFFSET    0x604
#define PSC_MDCFG02_OFFSET    0x608
#define PSC_MDCFG03_OFFSET    0x60C
#define PSC_MDCFG04_OFFSET    0x610
#define PSC_MDCFG05_OFFSET    0x614
#define PSC_MDCFG06_OFFSET    0x618
#define PSC_MDCFG07_OFFSET    0x61C
#define PSC_MDCFG08_OFFSET    0x620
#define PSC_MDCFG09_OFFSET    0x624
#define PSC_MDCFG10_OFFSET    0x628
#define PSC_MDCFG11_OFFSET    0x62C
#define PSC_MDCFG12_OFFSET    0x630
#define PSC_MDCFG13_OFFSET    0x634
#define PSC_MDCFG14_OFFSET    0x638
#define PSC_MDCFG15_OFFSET    0x63C
#define PSC_MDCFG16_OFFSET    0x640
#define PSC_MDCFG17_OFFSET    0x644
#define PSC_MDCFG18_OFFSET    0x648
#define PSC_MDCFG19_OFFSET    0x64C
#define PSC_MDCFG20_OFFSET    0x650
#define PSC_MDCFG21_OFFSET    0x654
#define PSC_MDCFG22_OFFSET    0x658
#define PSC_MDCFG23_OFFSET    0x65C
#define PSC_MDCFG24_OFFSET    0x660
#define PSC_MDCFG25_OFFSET    0x664
#define PSC_MDCFG26_OFFSET    0x668
#define PSC_MDCFG27_OFFSET    0x66C
#define PSC_MDCFG28_OFFSET    0x670
#define PSC_MDCFG29_OFFSET    0x674
#define PSC_MDCFG30_OFFSET    0x678
#define PSC_MDCFG31_OFFSET    0x67C
#define PSC_MDCFG32_OFFSET    0x680
#define PSC_MDCFG33_OFFSET    0x684
#define PSC_MDCFG34_OFFSET    0x688
#define PSC_MDCFG35_OFFSET    0x68C
#define PSC_MDCFG36_OFFSET    0x690
#define PSC_MDCFG37_OFFSET    0x694
#define PSC_MDCFG38_OFFSET    0x698
#define PSC_MDCFG39_OFFSET    0x69C
#define PSC_MDCFG40_OFFSET    0x6A0
#define PSC_MDCFG41_OFFSET    0x6A4
#define PSC_MDCFG42_OFFSET    0x6A8
#define PSC_MDCFG43_OFFSET    0x6AC
#define PSC_MDCFG44_OFFSET    0x6B0
#define PSC_MDCFG45_OFFSET    0x6B4
#define PSC_MDCFG46_OFFSET    0x6B8
#define PSC_MDCFG47_OFFSET    0x6BC
#define PSC_MDCFG48_OFFSET    0x6C0
#define PSC_MDCFG49_OFFSET    0x6C4
#define PSC_MDCFG50_OFFSET    0x6C8
#define PSC_MDCFG51_OFFSET    0x6CC
#define PSC_MDCFG52_OFFSET    0x6D0
#define PSC_MDCFG53_OFFSET    0x6D4
#define PSC_MDCFG54_OFFSET    0x6D8
#define PSC_MDCFG55_OFFSET    0x6DC
#define PSC_MDCFG56_OFFSET    0x6E0
#define PSC_MDCFG57_OFFSET    0x6E4
#define PSC_MDCFG58_OFFSET    0x6E8
#define PSC_MDCFG59_OFFSET    0x6EC
#define PSC_MDCFG60_OFFSET    0x6F0
#define PSC_MDCFG61_OFFSET    0x6F4
#define PSC_MDCFG62_OFFSET    0x6F8
#define PSC_MDCFG63_OFFSET    0x6FC
#define PSC_MDCFG64_OFFSET    0x700
#define PSC_MDCFG65_OFFSET    0x704
#define PSC_MDCFG66_OFFSET    0x708
#define PSC_MDCFG67_OFFSET    0x70C
#define PSC_MDCFG68_OFFSET    0x710
#define PSC_MDCFG69_OFFSET    0x714
#define PSC_MDCFG70_OFFSET    0x718
#define PSC_MDCFG71_OFFSET    0x71C
#define PSC_MDCFG72_OFFSET    0x720
#define PSC_MDCFG73_OFFSET    0x724
#define PSC_MDCFG74_OFFSET    0x728
#define PSC_MDCFG75_OFFSET    0x72C
#define PSC_MDCFG76_OFFSET    0x730
#define PSC_MDCFG77_OFFSET    0x734
#define PSC_MDCFG78_OFFSET    0x738
#define PSC_MDCFG79_OFFSET    0x73C
#define PSC_MDCFG80_OFFSET    0x740
#define PSC_MDCFG81_OFFSET    0x744
#define PSC_MDCFG82_OFFSET    0x748
#define PSC_MDCFG83_OFFSET    0x74C
#define PSC_MDCFG84_OFFSET    0x750
#define PSC_MDCFG85_OFFSET    0x754
#define PSC_MDCFG86_OFFSET    0x758
#define PSC_MDCFG87_OFFSET    0x75C
#define PSC_MDCFG88_OFFSET    0x760
#define PSC_MDCFG89_OFFSET    0x764
#define PSC_MDCFG90_OFFSET    0x768
#define PSC_MDCFG91_OFFSET    0x76C
#define PSC_MDCFG92_OFFSET    0x770
#define PSC_MDCFG93_OFFSET    0x774
#define PSC_MDCFG94_OFFSET    0x778
#define PSC_MDCFG95_OFFSET    0x77C
#define PSC_MDCFG96_OFFSET    0x780
#define PSC_MDCFG97_OFFSET    0x784
#define PSC_MDCFG98_OFFSET    0x788
#define PSC_MDCFG99_OFFSET    0x78C
#define PSC_MDCFG100_OFFSET   0x790
#define PSC_MDCFG101_OFFSET   0x794
#define PSC_MDCFG102_OFFSET   0x798
#define PSC_MDCFG103_OFFSET   0x79C
#define PSC_MDCFG104_OFFSET   0x7A0
#define PSC_MDCFG105_OFFSET   0x7A4
#define PSC_MDCFG106_OFFSET   0x7A8
#define PSC_MDCFG107_OFFSET   0x7AC
#define PSC_MDCFG108_OFFSET   0x7B0
#define PSC_MDCFG109_OFFSET   0x7B4

#define PSC_MDSTAT00_OFFSET   0x800
#define PSC_MDSTAT01_OFFSET   0x804
#define PSC_MDSTAT02_OFFSET   0x808
#define PSC_MDSTAT03_OFFSET   0x80C
#define PSC_MDSTAT04_OFFSET   0x810
#define PSC_MDSTAT05_OFFSET   0x814
#define PSC_MDSTAT06_OFFSET   0x818
#define PSC_MDSTAT07_OFFSET   0x81C
#define PSC_MDSTAT08_OFFSET   0x820
#define PSC_MDSTAT09_OFFSET   0x824
#define PSC_MDSTAT10_OFFSET   0x828
#define PSC_MDSTAT11_OFFSET   0x82C
#define PSC_MDSTAT12_OFFSET   0x830
#define PSC_MDSTAT13_OFFSET   0x834
#define PSC_MDSTAT14_OFFSET   0x838
#define PSC_MDSTAT15_OFFSET   0x83C
#define PSC_MDSTAT16_OFFSET   0x840
#define PSC_MDSTAT17_OFFSET   0x844
#define PSC_MDSTAT18_OFFSET   0x848
#define PSC_MDSTAT19_OFFSET   0x84C
#define PSC_MDSTAT20_OFFSET   0x850
#define PSC_MDSTAT21_OFFSET   0x854
#define PSC_MDSTAT22_OFFSET   0x858
#define PSC_MDSTAT23_OFFSET   0x85C
#define PSC_MDSTAT24_OFFSET   0x860
#define PSC_MDSTAT25_OFFSET   0x864
#define PSC_MDSTAT26_OFFSET   0x868
#define PSC_MDSTAT27_OFFSET   0x86C
#define PSC_MDSTAT28_OFFSET   0x870
#define PSC_MDSTAT29_OFFSET   0x874
#define PSC_MDSTAT30_OFFSET   0x878
#define PSC_MDSTAT31_OFFSET   0x87C
#define PSC_MDSTAT32_OFFSET   0x880
#define PSC_MDSTAT33_OFFSET   0x884
#define PSC_MDSTAT34_OFFSET   0x888
#define PSC_MDSTAT35_OFFSET   0x88C
#define PSC_MDSTAT36_OFFSET   0x890
#define PSC_MDSTAT37_OFFSET   0x894
#define PSC_MDSTAT38_OFFSET   0x898
#define PSC_MDSTAT39_OFFSET   0x89C
#define PSC_MDSTAT40_OFFSET   0x8A0
#define PSC_MDSTAT41_OFFSET   0x8A4
#define PSC_MDSTAT42_OFFSET   0x8A8
#define PSC_MDSTAT43_OFFSET   0x8AC
#define PSC_MDSTAT44_OFFSET   0x8B0
#define PSC_MDSTAT45_OFFSET   0x8B4
#define PSC_MDSTAT46_OFFSET   0x8B8
#define PSC_MDSTAT47_OFFSET   0x8BC
#define PSC_MDSTAT48_OFFSET   0x8C0
#define PSC_MDSTAT49_OFFSET   0x8C4
#define PSC_MDSTAT50_OFFSET   0x8C8
#define PSC_MDSTAT51_OFFSET   0x8CC
#define PSC_MDSTAT52_OFFSET   0x8D0
#define PSC_MDSTAT53_OFFSET   0x8D4
#define PSC_MDSTAT54_OFFSET   0x8D8
#define PSC_MDSTAT55_OFFSET   0x8DC
#define PSC_MDSTAT56_OFFSET   0x8E0
#define PSC_MDSTAT57_OFFSET   0x8E4
#define PSC_MDSTAT58_OFFSET   0x8E8
#define PSC_MDSTAT59_OFFSET   0x8EC
#define PSC_MDSTAT60_OFFSET   0x8F0
#define PSC_MDSTAT61_OFFSET   0x8F4
#define PSC_MDSTAT62_OFFSET   0x8F8
#define PSC_MDSTAT63_OFFSET   0x8FC
#define PSC_MDSTAT64_OFFSET   0x900
#define PSC_MDSTAT65_OFFSET   0x904
#define PSC_MDSTAT66_OFFSET   0x908
#define PSC_MDSTAT67_OFFSET   0x90C
#define PSC_MDSTAT68_OFFSET   0x910
#define PSC_MDSTAT69_OFFSET   0x914
#define PSC_MDSTAT70_OFFSET   0x918
#define PSC_MDSTAT71_OFFSET   0x91C
#define PSC_MDSTAT72_OFFSET   0x920
#define PSC_MDSTAT73_OFFSET   0x924
#define PSC_MDSTAT74_OFFSET   0x928
#define PSC_MDSTAT75_OFFSET   0x92C
#define PSC_MDSTAT76_OFFSET   0x930
#define PSC_MDSTAT77_OFFSET   0x934
#define PSC_MDSTAT78_OFFSET   0x938
#define PSC_MDSTAT79_OFFSET   0x93C
#define PSC_MDSTAT80_OFFSET   0x940
#define PSC_MDSTAT81_OFFSET   0x944
#define PSC_MDSTAT82_OFFSET   0x948
#define PSC_MDSTAT83_OFFSET   0x94C
#define PSC_MDSTAT84_OFFSET   0x950
#define PSC_MDSTAT85_OFFSET   0x954
#define PSC_MDSTAT86_OFFSET   0x958
#define PSC_MDSTAT87_OFFSET   0x95C
#define PSC_MDSTAT88_OFFSET   0x960
#define PSC_MDSTAT89_OFFSET   0x964
#define PSC_MDSTAT90_OFFSET   0x968
#define PSC_MDSTAT91_OFFSET   0x96C
#define PSC_MDSTAT92_OFFSET   0x970
#define PSC_MDSTAT93_OFFSET   0x974
#define PSC_MDSTAT94_OFFSET   0x978
#define PSC_MDSTAT95_OFFSET   0x97C
#define PSC_MDSTAT96_OFFSET   0x980
#define PSC_MDSTAT97_OFFSET   0x984
#define PSC_MDSTAT98_OFFSET   0x988
#define PSC_MDSTAT99_OFFSET   0x98C
#define PSC_MDSTAT100_OFFSET  0x990
#define PSC_MDSTAT101_OFFSET  0x994
#define PSC_MDSTAT102_OFFSET  0x998
#define PSC_MDSTAT103_OFFSET  0x99C
#define PSC_MDSTAT104_OFFSET  0x9A0
#define PSC_MDSTAT105_OFFSET  0x9A4
#define PSC_MDSTAT106_OFFSET  0x9A8
#define PSC_MDSTAT107_OFFSET  0x9AC
#define PSC_MDSTAT108_OFFSET  0x9B0
#define PSC_MDSTAT109_OFFSET  0x9B4

#define PSC_MDCTL00_OFFSET    0xA00
#define PSC_MDCTL01_OFFSET    0xA04
#define PSC_MDCTL02_OFFSET    0xA08
#define PSC_MDCTL03_OFFSET    0xA0C
#define PSC_MDCTL04_OFFSET    0xA10
#define PSC_MDCTL05_OFFSET    0xA14
#define PSC_MDCTL06_OFFSET    0xA18
#define PSC_MDCTL07_OFFSET    0xA1C
#define PSC_MDCTL08_OFFSET    0xA20
#define PSC_MDCTL09_OFFSET    0xA24
#define PSC_MDCTL10_OFFSET    0xA28
#define PSC_MDCTL11_OFFSET    0xA2C
#define PSC_MDCTL12_OFFSET    0xA30
#define PSC_MDCTL13_OFFSET    0xA34
#define PSC_MDCTL14_OFFSET    0xA38
#define PSC_MDCTL15_OFFSET    0xA3C
#define PSC_MDCTL16_OFFSET    0xA40
#define PSC_MDCTL17_OFFSET    0xA44
#define PSC_MDCTL18_OFFSET    0xA48
#define PSC_MDCTL19_OFFSET    0xA4C
#define PSC_MDCTL20_OFFSET    0xA50
#define PSC_MDCTL21_OFFSET    0xA54
#define PSC_MDCTL22_OFFSET    0xA58
#define PSC_MDCTL23_OFFSET    0xA5C
#define PSC_MDCTL24_OFFSET    0xA60
#define PSC_MDCTL25_OFFSET    0xA64
#define PSC_MDCTL26_OFFSET    0xA68
#define PSC_MDCTL27_OFFSET    0xA6C
#define PSC_MDCTL28_OFFSET    0xA70
#define PSC_MDCTL29_OFFSET    0xA74
#define PSC_MDCTL30_OFFSET    0xA78
#define PSC_MDCTL31_OFFSET    0xA7C
#define PSC_MDCTL32_OFFSET    0xA80
#define PSC_MDCTL33_OFFSET    0xA84
#define PSC_MDCTL34_OFFSET    0xA88
#define PSC_MDCTL35_OFFSET    0xA8C
#define PSC_MDCTL36_OFFSET    0xA90
#define PSC_MDCTL37_OFFSET    0xA94
#define PSC_MDCTL38_OFFSET    0xA98
#define PSC_MDCTL39_OFFSET    0xA9C
#define PSC_MDCTL40_OFFSET    0xAA0
#define PSC_MDCTL41_OFFSET    0xAA4
#define PSC_MDCTL42_OFFSET    0xAA8
#define PSC_MDCTL43_OFFSET    0xAAC
#define PSC_MDCTL44_OFFSET    0xAB0
#define PSC_MDCTL45_OFFSET    0xAB4
#define PSC_MDCTL46_OFFSET    0xAB8
#define PSC_MDCTL47_OFFSET    0xABC
#define PSC_MDCTL48_OFFSET    0xAC0
#define PSC_MDCTL49_OFFSET    0xAC4
#define PSC_MDCTL50_OFFSET    0xAC8
#define PSC_MDCTL51_OFFSET    0xACC
#define PSC_MDCTL52_OFFSET    0xAD0
#define PSC_MDCTL53_OFFSET    0xAD4
#define PSC_MDCTL54_OFFSET    0xAD8
#define PSC_MDCTL55_OFFSET    0xADC
#define PSC_MDCTL56_OFFSET    0xAE0
#define PSC_MDCTL57_OFFSET    0xAE4
#define PSC_MDCTL58_OFFSET    0xAE8
#define PSC_MDCTL59_OFFSET    0xAEC
#define PSC_MDCTL60_OFFSET    0xAF0
#define PSC_MDCTL61_OFFSET    0xAF4
#define PSC_MDCTL62_OFFSET    0xAF8
#define PSC_MDCTL63_OFFSET    0xAFC
#define PSC_MDCTL64_OFFSET    0xB00
#define PSC_MDCTL65_OFFSET    0xB04
#define PSC_MDCTL66_OFFSET    0xB08
#define PSC_MDCTL67_OFFSET    0xB0C
#define PSC_MDCTL68_OFFSET    0xB10
#define PSC_MDCTL69_OFFSET    0xB14
#define PSC_MDCTL70_OFFSET    0xB18
#define PSC_MDCTL71_OFFSET    0xB1C
#define PSC_MDCTL72_OFFSET    0xB20
#define PSC_MDCTL73_OFFSET    0xB24
#define PSC_MDCTL74_OFFSET    0xB28
#define PSC_MDCTL75_OFFSET    0xB2C
#define PSC_MDCTL76_OFFSET    0xB30
#define PSC_MDCTL77_OFFSET    0xB34
#define PSC_MDCTL78_OFFSET    0xB38
#define PSC_MDCTL79_OFFSET    0xB3C
#define PSC_MDCTL80_OFFSET    0xB40
#define PSC_MDCTL81_OFFSET    0xB44
#define PSC_MDCTL82_OFFSET    0xB48
#define PSC_MDCTL83_OFFSET    0xB4C
#define PSC_MDCTL84_OFFSET    0xB50
#define PSC_MDCTL85_OFFSET    0xB54
#define PSC_MDCTL86_OFFSET    0xB58
#define PSC_MDCTL87_OFFSET    0xB5C
#define PSC_MDCTL88_OFFSET    0xB60
#define PSC_MDCTL89_OFFSET    0xB64
#define PSC_MDCTL90_OFFSET    0xB68
#define PSC_MDCTL91_OFFSET    0xB6C
#define PSC_MDCTL92_OFFSET    0xB70
#define PSC_MDCTL93_OFFSET    0xB74
#define PSC_MDCTL94_OFFSET    0xB78
#define PSC_MDCTL95_OFFSET    0xB7C
#define PSC_MDCTL96_OFFSET    0xB80
#define PSC_MDCTL97_OFFSET    0xB84
#define PSC_MDCTL98_OFFSET    0xB88
#define PSC_MDCTL99_OFFSET    0xB8C
#define PSC_MDCTL100_OFFSET   0xB90
#define PSC_MDCTL101_OFFSET   0xB94
#define PSC_MDCTL102_OFFSET   0xB98
#define PSC_MDCTL103_OFFSET   0xB9C
#define PSC_MDCTL104_OFFSET   0xBA0
#define PSC_MDCTL105_OFFSET   0xBA4
#define PSC_MDCTL106_OFFSET   0xBA8
#define PSC_MDCTL107_OFFSET   0xBAC
#define PSC_MDCTL108_OFFSET   0xBB0
#define PSC_MDCTL109_OFFSET   0xBB4

/* Reserved MMRs in WKUP PSC */
#define PSC_WKUP_RSV00_START  0x004
#define PSC_WKUP_RSV00_END    0x00F
#define PSC_WKUP_RSV01_START  0x020
#define PSC_WKUP_RSV01_END    0x03F
#define PSC_WKUP_RSV02_START  0x080
#define PSC_WKUP_RSV02_END    0x0FF
#define PSC_WKUP_RSV03_START  0x110
#define PSC_WKUP_RSV03_END    0x11F
#define PSC_WKUP_RSV04_START  0x130
#define PSC_WKUP_RSV04_END    0x1FF
#define PSC_WKUP_RSV05_START  0x208
#define PSC_WKUP_RSV05_END    0x2FF
#define PSC_WKUP_RSV06_START  0x308
#define PSC_WKUP_RSV06_END    0x3FF
#define PSC_WKUP_RSV07_START  0x408
#define PSC_WKUP_RSV07_END    0x5FF
#define PSC_WKUP_RSV08_START  0x658
#define PSC_WKUP_RSV08_END    0x7FF
#define PSC_WKUP_RSV09_START  0x858
#define PSC_WKUP_RSV09_END    0x9FF
#define PSC_WKUP_RSV10_START  0xA58
#define PSC_WKUP_RSV10_END    0xFFF

/* Reserved MMRs in MAIN PSC */
#define PSC_MAIN_RSV00_START  0x004
#define PSC_MAIN_RSV00_END    0x00F
#define PSC_MAIN_RSV01_START  0x020
#define PSC_MAIN_RSV01_END    0x03F
#define PSC_MAIN_RSV02_START  0x080
#define PSC_MAIN_RSV02_END    0x0FF
#define PSC_MAIN_RSV03_START  0x110
#define PSC_MAIN_RSV03_END    0x11F
#define PSC_MAIN_RSV04_START  0x130
#define PSC_MAIN_RSV04_END    0x1FF
#define PSC_MAIN_RSV05_START  0x278
#define PSC_MAIN_RSV05_END    0x2FF
#define PSC_MAIN_RSV06_START  0x378
#define PSC_MAIN_RSV06_END    0x3FF
#define PSC_MAIN_RSV07_START  0x478
#define PSC_MAIN_RSV07_END    0x5FF
#define PSC_MAIN_RSV08_START  0x7B0
#define PSC_MAIN_RSV08_END    0x7FF
#define PSC_MAIN_RSV09_START  0x9B0
#define PSC_MAIN_RSV09_END    0x9FF
#define PSC_MAIN_RSV10_START  0xBB0
#define PSC_MAIN_RSV10_END    0xFFF

/* scate_tb translate_off */

/* PID section */
#define PSC_WKUP_PID_VAL  0x44827A00
#define PSC_MAIN_PID_VAL  0x44827A00
#define PSC_PID_VAL       0x44827A00 /* Common for both */

/* Other Defines */
#define RDONLY_WEM        0x00000000
#define GBLCTL_WEM        0x0000FF01
#define IPWKCT_WEM        0x3FFFFFFF
#define PDCTL_WEM         0xB0FFF301
#define PDCTL_WEM1        0xB0FF0301
#define PDCTL_WEM2        0xB0FF0300
#define PDCTL_WEM3        0xB0FFF300
#define PDCTL_WEMRIO      0xB0FF0300
#define MDCTL_WEM         0x80001F07
#define MDCTL_WEM1        0x80001F00
#define MDCTL_WEM2        0x0000FFFF
#define MDCTL_WEM3        0x00000007
#define MDCTL_WEM4        0x00003FFF
#define MDCTL_WEM5        0x80001F07

#define RDERR             0xFFFF0000
#define WRERR             0xFFFF0001
#define CFG_BUS           0xFFFF1000
#define DMA_BUS           0xFFFF1001

#define PSC_PDCFG_BASE    PSC_PDCFG00_OFFSET
#define PSC_PDSTAT_BASE   PSC_PDSTAT00_OFFSET
#define PSC_PDCTL_BASE    PSC_PDCTL00_OFFSET
#define PSC_MDCFG_BASE    PSC_MDCFG00_OFFSET
#define PSC_MDSTAT_BASE   PSC_MDSTAT00_OFFSET
#define PSC_MDCTL_BASE    PSC_MDCTL00_OFFSET

#define PSC_ALLINT_NO     362

#define PSC_PWRDWN        0x0
#define PSC_PWRUP         0x2
#define PSC_ENABLED       0x1
#define PSC_DISABLED      0x0

#define ENABLED           1
#define DISABLED          0

#define WKUP_PSC          0
#define MAIN_PSC          1
#define BOTH_PSC          2

#define PSC_NUM_POSITION  16

#define PSC_RESET_ISO_SET           (0x1 << 12)
#define PSC_MEMPDMODE_ZEROMASK      (~(0x0F << 12))
#define PSC_MEMPDMODE_DEFAULT       0
#define PSC_MEMPDMODE_RETENTION     (0x1 << 12)

#define LPSC_ALLSTRESS_TEST     0xF
#define LPSC_SWTRSTDIS_TEST     0x0
#define LPSC_SYNCRSTDIS_TEST    0x1
#define LPSC_CLKGATEONLY_TEST   0x2
#define LPSC_POWERDOWN_TEST     0x3
#define LPSC_TEST_PATTERN       LPSC_ALLSTRESS_TEST

struct psc_reg_info {
  uint64_t  offset;       /*/< register offset from base */
  uint32_t  reset_val;    /*/< reset value of the register. */
  uint32_t  wr_mask;      /*/< 1 means bit can be written */
  uint32_t  wr_eff_mask;  /*/< 1 means writes to the bit change value in reg */
  uint32_t  wr_val;       /*/< last written value */
  uint32_t  rd_val;       /*/< last read value. */
};

#endif

