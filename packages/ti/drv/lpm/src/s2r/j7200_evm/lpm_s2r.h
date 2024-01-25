/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
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
*/

/**
*  \file lpm_s2r.h
*
*  \brief File containing the Binary in a C array.
*
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#ifndef LPM_S2R_H_
#define LPM_S2R_H_


#define LPM_SRAM_S2R_SIZE_IN_BYTES (2060U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#define LPM_SRAM_S2R { \
    0xe24dd010U,     0xe3a00000U,     0xe3a01c02U,     0xe2511001U, \
    0xf57ff05fU,     0xee072f5aU,     0xee072f56U,     0xf57ff04fU, \
    0x1afffff9U,     0xe2800001U,     0xe3500004U,     0x1afffff5U, \
    0xe3090008U,     0xe30b2c5aU,     0xe3440301U,     0xe3033490U, \
    0xe3001800U,     0xe34d2172U,     0xe34638efU,     0xe34310e0U, \
    0xe5906000U,     0xe3160001U,     0x1a000006U,     0xe5916000U, \
    0xe3560000U,     0x1afffffcU,     0xe3a06000U,     0xe5803000U, \
    0xe5802004U,     0xe5816000U,     0xe30c00b8U,     0xe3440301U, \
    0xe5906f50U,     0xe3160001U,     0x1a000008U,     0xe5916000U, \
    0xe3560000U,     0x1afffffcU,     0xe3a06000U,     0xe30d500cU, \
    0xe3445301U,     0xe5803f50U,     0xe5852000U,     0xe5816000U, \
    0xe30d6008U,     0xe3406011U,     0xe5965000U,     0xe3150001U, \
    0x1a000006U,     0xe5915000U,     0xe3550000U,     0x1afffffcU, \
    0xe3a05000U,     0xe5863000U,     0xe5862004U,     0xe5815000U, \
    0xe3001210U,     0xe30c5010U,     0xe3401299U,     0xe3056008U, \
    0xe3405068U,     0xe3406011U,     0xe5853000U,     0xe5852004U, \
    0xe5863000U,     0xe3003100U,     0xe5862004U,     0xe3403101U, \
    0xe59120e8U,     0xe3a06000U,     0xe1822003U,     0xe3a03945U, \
    0xe58120e8U,     0xe5836000U,     0xe58d6004U,     0xe59d2004U, \
    0xe3520063U,     0x8a000005U,     0xe59d2004U,     0xe2822001U, \
    0xe58d2004U,     0xe59d2004U,     0xe3520064U,     0x3afffff9U, \
    0xe5932000U,     0xe3826c01U,     0xe3042004U,     0xe3402011U, \
    0xe5836000U,     0xe592307cU,     0xe3130080U,     0x0afffffcU, \
    0xe5953010U,     0xe3833102U,     0xe5853010U,     0xe5953010U, \
    0xe3730001U,     0xca000000U,     0xe5953010U,     0xe3a03000U, \
    0xe58d3004U,     0xe3043240U,     0xe59d6004U,     0xe340300fU, \
    0xe1560003U,     0x2a000004U,     0xe59d6004U,     0xe2866001U, \
    0xe58d6004U,     0xe59d6004U,     0xeafffff8U,     0xe3a03001U, \
    0xe58230bcU,     0xe5923000U,     0xe3130080U,     0x0afffffcU, \
    0xe3a03000U,     0xe3a06945U,     0xe58230bcU,     0xe30f3fffU, \
    0xe5962000U,     0xe34f30f0U,     0xe3c22c01U,     0xe5862000U, \
    0xe5912024U,     0xe0022003U,     0xe3a03051U,     0xe5812024U, \
    0xe5912000U,     0xe7de2c13U,     0xe5812000U,     0xe5912284U, \
    0xe3120b01U,     0x0afffffcU,     0xe3a03b01U,     0xe3042f4fU, \
    0xe581328cU,     0xe5913024U,     0xe0033002U,     0xe1530002U, \
    0x1afffffbU,     0xe3006000U,     0xe3088000U,     0xe3426005U, \
    0xe3438803U,     0xe30c1020U,     0xe307ecdaU,     0xe3401011U, \
    0xe30acef9U,     0xe348ea6bU,     0xe348c23cU,     0xe5816000U, \
    0xe3a04000U,     0xe581600cU,     0xe5818104U,     0xe3001020U, \
    0xe3441413U,     0xe581e000U,     0xe581c004U,     0xe5912024U, \
    0xe3823002U,     0xe3082300U,     0xe3442301U,     0xe5813024U, \
    0xe51231a0U,     0xe3833c01U,     0xe50231a0U,     0xe5913068U, \
    0xe3833801U,     0xe5813068U,     0xe5913068U,     0xe3833401U, \
    0xe5813068U,     0xe58d4008U,     0xe58d4008U,     0xe59d3008U, \
    0xe3530095U,     0x8a000005U,     0xe59d3008U,     0xe2833001U, \
    0xe58d3008U,     0xe59d3008U,     0xe3530096U,     0x3afffff9U, \
    0xe3053554U,     0xe5915068U,     0xe3453555U,     0xe2865801U, \
    0xe3a06721U,     0xe3837001U,     0xe5823000U,     0xe5827000U, \
    0xe5814000U,     0xe5814004U,     0xe5805000U,     0xe5805018U, \
    0xe58080d8U,     0xe5806048U,     0xe580604cU,     0xe5806030U, \
    0xe5806034U,     0xe5806038U,     0xe580602cU,     0xe5806040U, \
    0xe5806044U,     0xe581e000U,     0xe581c004U,     0xe5910024U, \
    0xe3800001U,     0xe5810024U,     0xe51201a0U,     0xe3800001U, \
    0xe50201a0U,     0xe5910064U,     0xe3800801U,     0xe5810064U, \
    0xe5910064U,     0xe3800040U,     0xe3800401U,     0xe5810064U, \
    0xe58d400cU,     0xe58d400cU,     0xe59d000cU,     0xe3500095U, \
    0x8a000005U,     0xe59d000cU,     0xe2800001U,     0xe58d000cU, \
    0xe59d000cU,     0xe3500096U,     0x3afffff9U,     0xe2830001U, \
    0xe3a05000U,     0xe5823010U,     0xe3004024U,     0xe3444212U, \
    0xe5820010U,     0xe3a00048U,     0xe5815000U,     0xe5815004U, \
    0xeb0000a2U,     0xe3a00086U,     0xe3a01003U,     0xeb0000ebU, \
    0xe3a00048U,     0xeb00009dU,     0xe3a00065U,     0xe3a01002U, \
    0xeb0000e6U,     0xe3a00048U,     0xeb000098U,     0xe3a00034U, \
    0xe3a010c0U,     0xeb0000e1U,     0xe3a00048U,     0xeb000093U, \
    0xe3a00064U,     0xe3a01008U,     0xeb0000dcU,     0xe3a00048U, \
    0xeb00008eU,     0xe3a00050U,     0xe3a010ffU,     0xeb0000d7U, \
    0xe3a00048U,     0xeb000089U,     0xe3a00051U,     0xe3a0103fU, \
    0xeb0000d2U,     0xe3a00048U,     0xeb000084U,     0xe3a0004fU, \
    0xe3a010f7U,     0xeb0000cdU,     0xe3a0004cU,     0xeb00007fU, \
    0xe3a00032U,     0xe3a01001U,     0xeb0000c8U,     0xe3a0004cU, \
    0xeb00007aU,     0xe3a00033U,     0xe3a01001U,     0xeb0000c3U, \
    0xe3a0004cU,     0xeb000075U,     0xe5940000U,     0xe3100a01U, \
    0x0a000002U,     0xe2555001U,     0x1afffffaU,     0xea000001U, \
    0xe3550001U,     0x0a000040U,     0xe3a00001U,     0xe5840074U, \
    0xe5940080U,     0xe3800c06U,     0xe3c00002U,     0xe5840080U, \
    0xe5940080U,     0xe3801001U,     0xe3a00000U,     0xe5841080U, \
    0xe5941000U,     0xe3110010U,     0x1a000002U,     0xe2500001U, \
    0x1afffffaU,     0xea000001U,     0xe3500001U,     0x0a00002eU, \
    0xe3a0103dU,     0xe3a02010U,     0xe3a00000U,     0xe5841078U, \
    0xe5842004U,     0xe5941000U,     0xe3110004U,     0x1a000002U, \
    0xe2500001U,     0x1afffffaU,     0xea000001U,     0xe3500001U, \
    0x0a000021U,     0xe5940000U,     0xe3a01002U,     0xe5840004U, \
    0xe5940080U,     0xe7ca0491U,     0xe3a01001U,     0xe5840080U, \
    0xe5841074U,     0xe5940080U,     0xe3801003U,     0xe3a00000U, \
    0xe5841080U,     0xe5941000U,     0xe3110008U,     0x1a000003U, \
    0xe2500001U,     0x1afffffaU,     0xe3a00000U,     0xea000002U, \
    0xe3500001U,     0x0a00000cU,     0xe2600000U,     0xe5945078U, \
    0xe2600000U,     0xe5941000U,     0xe3110004U,     0x1a000002U, \
    0xe2500001U,     0x1afffffaU,     0xea000001U,     0xe3500001U, \
    0x0a000001U,     0xe5940000U,     0xe5840004U,     0xe3a0004cU, \
    0xe3856002U,     0xeb000029U,     0xe6ef1076U,     0xe3a0003dU, \
    0xeb000072U,     0xe3a0004cU,     0xe3854006U,     0xeb000023U, \
    0xe6ef4074U,     0xe3a0003dU,     0xe1a01004U,     0xeb00006bU, \
    0xe3a0004cU,     0xeb00001dU,     0xe20610fbU,     0xe3a0003dU, \
    0xeb000066U,     0xe3a0004cU,     0xeb000018U,     0xe3a0003dU, \
    0xe1a01004U,     0xeb000061U,     0xe3a0004cU,     0xeb000013U, \
    0xe3a00085U,     0xe3a010c0U,     0xeb00005cU,     0xe3a00048U, \
    0xeb00000eU,     0xe3a00085U,     0xe3a010c0U,     0xeb000057U, \
    0xe3a00048U,     0xeb000009U,     0xe3a000cbU,     0xe3a010baU, \
    0xeb000052U,     0xe3a00048U,     0xeb000004U,     0xe3a00086U, \
    0xe3a01000U,     0xeb00004dU,     0xeafffffeU,     0x00000000U, \
    0xe3031490U,     0xe30c30f8U,     0xe34618efU,     0xe30b2c5aU, \
    0xe3443301U,     0xe34d2172U,     0xe5831f10U,     0xe30d100cU, \
    0xe3441301U,     0xe5812000U,     0xe3002010U,     0xe5931000U, \
    0xe3442212U,     0xe3811701U,     0xe5831000U,     0xe5931004U, \
    0xe3811701U,     0xe5831004U,     0xe5921000U,     0xe3811002U, \
    0xe5821000U,     0xe5921000U,     0xe3811008U,     0xe5821000U, \
    0xe5921094U,     0xe3811902U,     0xe5821094U,     0xe5921080U, \
    0xe3110001U,     0x0afffffcU,     0xe59210a0U,     0xe3a03007U, \
    0xe7c71013U,     0xe3a03009U,     0xe58210a0U,     0xe59210a4U, \
    0xe7c71013U,     0xe58210a4U,     0xe59210a8U,     0xe7c71013U, \
    0xe30330fcU,     0xe34f3fffU,     0xe58210a8U,     0xe5921094U, \
    0xe0011003U,     0xe3073f7fU,     0xe34f3fffU,     0xe5821094U, \
    0xe5921088U,     0xe7cf101fU,     0xe5821088U,     0xe5921084U, \
    0xe0011003U,     0xe3a030b4U,     0xe5821084U,     0xe5921098U, \
    0xe7c91013U,     0xe5821098U,     0xe5921094U,     0xe3811902U, \
    0xe5821094U,     0xe5921080U,     0xe3110001U,     0x0afffffcU, \
    0xe592109cU,     0xe304c040U,     0xe7c9101fU,     0xe1813000U, \
    0xe3a000a1U,     0xe3a0109bU,     0xe582309cU,     0xe582c084U, \
    0xea000002U,     0x00000000U,     0x00000000U,     0x00000000U, \
    0xe92d4010U,     0xe3002024U,     0xe3442212U,     0xe5923000U, \
    0xe3130a01U,     0x1afffffcU,     0xe3a03002U,     0xe3a04000U, \
    0xe3a0c010U,     0xe5823074U,     0xe5923080U,     0xe3833c06U, \
    0xe3c33002U,     0xe5823080U,     0xe5923080U,     0xe383e003U, \
    0xe582e080U,     0xe5923000U,     0xe3130010U,     0x0afffffcU, \
    0xe3540000U,     0x0a000002U,     0xe5821078U,     0xe582c004U, \
    0xea000000U,     0xe5820078U,     0xe2844001U,     0xe3540002U, \
    0x1afffff3U,     0xe5920000U,     0xe3100004U,     0x0afffffcU, \
    0xe5920000U,     0xe5820004U,     0xe8bd8010U\
} /* 2060 bytes */

#endif
