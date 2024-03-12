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


#define LPM_SRAM_S2R_SIZE_IN_BYTES (1812U)

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
    0xe3a00000U,     0xe3a01c02U,     0xe2511001U,     0xf57ff05fU, \
    0xee072f5aU,     0xee072f56U,     0xf57ff04fU,     0x1afffff9U, \
    0xe2800001U,     0xe3500004U,     0x1afffff5U,     0xe24dd008U, \
    0xe30b0c5aU,     0xe3031490U,     0xe34d0172U,     0xe305b008U, \
    0xe3044240U,     0xe301e69cU,     0xe34618efU,     0xe340b011U, \
    0xe340400fU,     0xe344e101U,     0xe30c3010U,     0xe3a08000U, \
    0xe3a02601U,     0xe30ca020U,     0xe3a07000U,     0xe1a09000U, \
    0xe3403068U,     0xe5831000U,     0xe5830004U,     0xe3031490U, \
    0xe0875087U,     0xe34618efU,     0xe1a0300eU,     0xe7b30185U, \
    0xe58b1000U,     0xe3001100U,     0xe3401101U,     0xe5936004U, \
    0xe58b9004U,     0xe58012f8U,     0xe7868002U,     0xe58d8000U, \
    0xe59d0000U,     0xe3500063U,     0x8a000005U,     0xe59d0000U, \
    0xe2800001U,     0xe58d0000U,     0xe59d0000U,     0xe3500064U, \
    0x3afffff9U,     0xe1a00006U,     0xe08e5185U,     0xe7b03002U, \
    0xe3831c01U,     0xe595300cU,     0xe5801000U,     0xe7930002U, \
    0xe3100080U,     0x0afffffcU,     0xe5953014U,     0xe1a00003U, \
    0xe7b0100aU,     0xe3811102U,     0xe5801000U,     0xe5900000U, \
    0xe3700001U,     0xca000000U,     0xe793000aU,     0xe58d8000U, \
    0xe59d0000U,     0xe1500004U,     0x2a000003U,     0xe59d0000U, \
    0xe2800001U,     0xe58d0000U,     0xeafffff8U,     0xe595c008U, \
    0xe3a00001U,     0xe5953010U,     0xe7830002U,     0xe79c0002U, \
    0xe3100080U,     0x0afffffcU,     0xe7838002U,     0xe2877001U, \
    0xe7b60002U,     0xe3570004U,     0xe3c00c01U,     0xe5860000U, \
    0x1affffc1U,     0xe30f6fffU,     0xe3a01000U,     0xe34f60f0U, \
    0xe3a02051U,     0xe3a03b01U,     0xe3047f4fU,     0xe0810081U, \
    0xe79e5180U,     0xe5950234U,     0xe0000006U,     0xe5850234U, \
    0xe5950210U,     0xe7de0c12U,     0xe5850210U,     0xe5950494U, \
    0xe3100b01U,     0x0afffffcU,     0xe585349cU,     0xe5950234U, \
    0xe0000007U,     0xe1500007U,     0x1afffffbU,     0xe2811001U, \
    0xe3510004U,     0x1affffecU,     0xe3a0004cU,     0xeb000067U, \
    0xe28d1006U,     0xe3a00001U,     0xe3a02f7dU,     0xeb0000afU, \
    0xe3a04000U,     0xe31000ffU,     0x1a000007U,     0xe5dd0006U, \
    0xe2401086U,     0xe2400007U,     0xe16f1f11U,     0xe16f0f10U, \
    0xe1a012a1U,     0xe1a002a0U,     0xe1814000U,     0xe3a00048U, \
    0xeb000056U,     0xe3a00086U,     0xe3a01003U,     0xeb0000f7U, \
    0xe3a00048U,     0xeb000051U,     0xe3a01002U,     0xe3540000U, \
    0x1a000005U,     0xe3a00066U,     0xe3a01001U,     0xeb0000efU, \
    0xe3a00048U,     0xeb000049U,     0xe3a01026U,     0xe3a00065U, \
    0xeb0000eaU,     0xe3a00048U,     0xeb000044U,     0xe3a00085U, \
    0xe3a01080U,     0xeb0000e5U,     0xe3a00048U,     0xeb00003fU, \
    0xe3a00034U,     0xe3a010c0U,     0xeb0000e0U,     0xe3a00048U, \
    0xeb00003aU,     0xe3a00064U,     0xe3a01008U,     0xeb0000dbU, \
    0xe3a00048U,     0xeb000035U,     0xe3a00050U,     0xe3a010f7U, \
    0xeb0000d6U,     0xe3a00048U,     0xeb000030U,     0xe3a00051U, \
    0xe3a0103fU,     0xeb0000d1U,     0xe3a00048U,     0xeb00002bU, \
    0xe3a0004fU,     0xe3a010ffU,     0xeb0000ccU,     0xe3a00048U, \
    0xeb000026U,     0xe3a00036U,     0xe3a01001U,     0xeb0000c7U, \
    0xe3a00048U,     0xeb000021U,     0xe28d1007U,     0xe3a0003dU, \
    0xe3e02000U,     0xeb000069U,     0xe5dd5007U,     0xe3a00048U, \
    0xeb00001aU,     0xe3851020U,     0xe3a0003dU,     0xeb0000bbU, \
    0xe3540000U,     0x0a000009U,     0xe3a0004cU,     0xeb000013U, \
    0xe3a00085U,     0xe3a01080U,     0xeb0000b4U,     0xe3a00048U, \
    0xeb00000eU,     0xe3a00085U,     0xe3a01080U,     0xeb0000afU, \
    0xe3a00048U,     0xeb000009U,     0xe3a000cbU,     0xe3a010baU, \
    0xeb0000aaU,     0xe3a00048U,     0xeb000004U,     0xe3a00086U, \
    0xe3a01000U,     0xeb0000a5U,     0xe28dd008U,     0xeafffffeU, \
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
    0xea00005aU,     0x00000000U,     0x00000000U,     0x00000000U, \
    0xe92d4070U,     0xe300e024U,     0xe1a0c000U,     0xe344e212U, \
    0xe2825001U,     0xe3a03000U,     0xe59e0000U,     0xe3100a01U, \
    0x0a000003U,     0xe2833001U,     0xe1550003U,     0x1afffff9U, \
    0xe1a03005U,     0xe3e00000U,     0xe1530002U,     0x2a000043U, \
    0xe3a03001U,     0xe2824001U,     0xe58e3074U,     0xe59e3080U, \
    0xe3833c06U,     0xe3c33002U,     0xe58e3080U,     0xe59e3080U, \
    0xe3836001U,     0xe3a03000U,     0xe58e6080U,     0xe59e6000U, \
    0xe3160010U,     0x1a000003U,     0xe2833001U,     0xe1540003U, \
    0x1afffff9U,     0xe1a03005U,     0xe1530002U,     0x2a00002fU, \
    0xe3a06010U,     0xe3a03000U,     0xe58ec078U,     0xe58e6004U, \
    0xe59e6000U,     0xe3160004U,     0x1a000003U,     0xe2833001U, \
    0xe1540003U,     0x1afffff9U,     0xe1a03005U,     0xe1530002U, \
    0x2a000022U,     0xe59e3000U,     0xe3a06002U,     0xe58e3004U, \
    0xe59e3080U,     0xe7ca3496U,     0xe3a06001U,     0xe58e3080U, \
    0xe58e6074U,     0xe59e3080U,     0xe3836003U,     0xe3a03000U, \
    0xe58e6080U,     0xe59e6000U,     0xe3160008U,     0x1a000003U, \
    0xe2833001U,     0xe1540003U,     0x1afffff9U,     0xe1a03005U, \
    0xe1530002U,     0x2a00000dU,     0xe59e6078U,     0xe5c16000U, \
    0xe59e1000U,     0xe3110004U,     0x1a000003U,     0xe2833001U, \
    0xe1540003U,     0x1afffff9U,     0xe1a03005U,     0xe1530002U, \
    0x28bd8070U,     0xe59e1000U,     0xe3a00000U,     0xe58e1004U, \
    0xe8bd8070U,     0x00000000U,     0x00000000U,     0x00000000U, \
    0xe92d4010U,     0xe3002024U,     0xe3442212U,     0xe5923000U, \
    0xe3130a01U,     0x1afffffcU,     0xe3a03002U,     0xe3a04000U, \
    0xe3a0c010U,     0xe5823074U,     0xe5923080U,     0xe3833c06U, \
    0xe3c33002U,     0xe5823080U,     0xe5923080U,     0xe383e003U, \
    0xe582e080U,     0xe5923000U,     0xe3130010U,     0x0afffffcU, \
    0xe3540000U,     0x0a000002U,     0xe5821078U,     0xe582c004U, \
    0xea000000U,     0xe5820078U,     0xe2844001U,     0xe3540002U, \
    0x1afffff3U,     0xe5920000U,     0xe3100004U,     0x0afffffcU, \
    0xe5920000U,     0xe5820004U,     0xe8bd8010U,     0x02990000U, \
    0x00014000U,     0x00014004U,     0x00014080U,     0x000140c0U, \
    0x0068c000U,     0x029b0000U,     0x00014010U,     0x00014014U, \
    0x00014090U,     0x000140d0U,     0x0069a000U,     0x029d0000U, \
    0x00014020U,     0x00014024U,     0x000140a0U,     0x000140e0U, \
    0x0069b000U,     0x029f0000U,     0x00014030U,     0x00014034U, \
    0x000140b0U,     0x000140f0U,     0x0069c000U,     0x00000000U, \
    0x00000000U,     0x00000000U,     0x00000000U,     0x00000000U, \
    0x00000000U\
} /* 1812 bytes */

#endif
