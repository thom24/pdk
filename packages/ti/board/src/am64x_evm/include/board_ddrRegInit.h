/* Copyright (c) 2019, Texas Instruments Incorporated
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
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#define DDRSS_PLL_FHS_CNT 10
#define DDRSS_PLL_FREQUENCY_1 933000000
#define DDRSS_PLL_FREQUENCY_2 933000000

#define DDRSS_CTL_REG_INIT_COUNT          (422U)
#define DDRSS_PHY_INDEP_REG_INIT_COUNT    (345U)
#define DDRSS_PHY_REG_INIT_COUNT          (507U)


uint32_t DDRSS_ctlReg[] = {
 0x00000a00U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000008U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000008U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000008U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x01010100U,
 0x01000000U,
 0x01000010U,
 0x02010002U,
 0x000000a0U,
 0x00000190U,
 0x02550255U,
 0x00000255U,
 0x00000000U,
 0x00000000U,
 0x00020200U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000010U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x04000918U,
 0x1c1c1c1cU,
 0x04000918U,
 0x1c1c1c1cU,
 0x04000918U,
 0x1c1c1c1cU,
 0x05050404U,
 0x00002806U,
 0x0602001dU,
 0x05001d0cU,
 0x00280605U,
 0x0602001dU,
 0x05001d0cU,
 0x00280605U,
 0x0602001dU,
 0x07001d0cU,
 0x00180807U,
 0x0400db60U,
 0x07070009U,
 0x00001808U,
 0x0400db60U,
 0x07070009U,
 0x00001808U,
 0x0400db60U,
 0x03000009U,
 0x0d0d0002U,
 0x0d0d0d0dU,
 0x01010000U,
 0x031a1a1aU,
 0x0c0c0c0cU,
 0x00000c0cU,
 0x00000101U,
 0x00000000U,
 0x01000000U,
 0x01b80803U,
 0x00001860U,
 0x000001b8U,
 0x00001860U,
 0x000001b8U,
 0x00001860U,
 0x00000005U,
 0x000001b8U,
 0x0000017eU,
 0x000001b8U,
 0x0000017eU,
 0x000001b8U,
 0x0000017eU,
 0x03004000U,
 0x00001201U,
 0x00090009U,
 0x00000009U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00010001U,
 0x00025501U,
 0x02550168U,
 0x02550168U,
 0x01680168U,
 0x01680168U,
 0x00000000U,
 0x00040000U,
 0x00000000U,
 0x00000400U,
 0x00000000U,
 0x01010104U,
 0x03010000U,
 0x00010000U,
 0x00000000U,
 0x01000000U,
 0x80104002U,
 0x00040003U,
 0x00040005U,
 0x00030000U,
 0x00050004U,
 0x00000004U,
 0x00040003U,
 0x00040005U,
 0x00000000U,
 0x00061800U,
 0x00061800U,
 0x00061800U,
 0x00061800U,
 0x00061800U,
 0x00000000U,
 0x0000aaa0U,
 0x00061800U,
 0x00061800U,
 0x00061800U,
 0x00061800U,
 0x00061800U,
 0x00000000U,
 0x0000aaa0U,
 0x00061800U,
 0x00061800U,
 0x00061800U,
 0x00061800U,
 0x00061800U,
 0x00000000U,
 0x0000aaa0U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x080c0000U,
 0x080c080cU,
 0x00000000U,
 0x07010a09U,
 0x000e0a09U,
 0x010a0900U,
 0x0e0a0907U,
 0x0a090000U,
 0x0a090701U,
 0x00002e0eU,
 0x00040003U,
 0x00000007U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x01000001U,
 0x00000000U,
 0x00001500U,
 0x0000100eU,
 0x00000000U,
 0x00000000U,
 0x00000001U,
 0x00000002U,
 0x00000c00U,
 0x00001000U,
 0x00000c00U,
 0x00001000U,
 0x00000c00U,
 0x00001000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x005000a0U,
 0x000000c8U,
 0x000000c8U,
 0x005000a0U,
 0x000000c8U,
 0x000000c8U,
 0x005000a0U,
 0x000000c8U,
 0x000000c8U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00041400U,
 0x00000201U,
 0x00000000U,
 0x00000414U,
 0x00000201U,
 0x00000000U,
 0x00000414U,
 0x00000201U,
 0x00000000U,
 0x00000414U,
 0x00000201U,
 0x00000000U,
 0x00000414U,
 0x00000201U,
 0x00000000U,
 0x00000414U,
 0x00000201U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000481U,
 0x00000481U,
 0x00000481U,
 0x00000481U,
 0x00000481U,
 0x00000481U,
 0x00000400U,
 0x00000400U,
 0x00000400U,
 0x00000400U,
 0x00000400U,
 0x00000400U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000020U,
 0x00010000U,
 0x00000100U,
 0x00000000U,
 0x00000000U,
 0x00000101U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x0c181513U,
 0x00000304U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00040000U,
 0x00800200U,
 0x00000320U,
 0x02000400U,
 0x03200080U,
 0x00040000U,
 0x00800200U,
 0x00000320U,
 0x00000000U,
 0x00000000U,
 0x00000100U,
 0x00000000U,
 0x00000202U,
 0x1fff0000U,
 0x0007ff00U,
 0x3fff2000U,
 0x0007FF00U,
 0x0a000000U,
 0x0001ffffU,
 0x01010101U,
 0x01010101U,
 0x00000114U,
 0x00000c03U,
 0x00000000U,
 0x00000000U,
 0x01010000U,
 0x01000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x0c000000U,
 0x060c0606U,
 0x06060c06U,
 0x00010101U,
 0x02000000U,
 0x03020101U,
 0x00000303U,
 0x02020200U,
 0x02020202U,
 0x02020202U,
 0x02020202U,
 0x00000000U,
 0x00000000U,
 0x04000100U,
 0x1f000004U,
 0x000030c0U,
 0x00000200U,
 0x00000200U,
 0x00000200U,
 0x00000200U,
 0x0000db60U,
 0x0001e780U,
 0x0a0b0302U,
 0x001f090aU,
 0x000030c0U,
 0x00000200U,
 0x00000200U,
 0x00000200U,
 0x00000200U,
 0x0000db60U,
 0x0001e780U,
 0x0a0b0302U,
 0x001f090aU,
 0x000030c0U,
 0x00000200U,
 0x00000200U,
 0x00000200U,
 0x00000200U,
 0x0000db60U,
 0x0001e780U,
 0x0a0b0302U,
 0x0000090aU,
 0x00000000U,
 0x0302000aU,
 0x01000500U,
 0x01010001U,
 0x00010001U,
 0x01010001U,
 0x02010000U,
 0x00000200U,
 0x02000201U,
 0x00000000U,
 0x00202020U,
};

uint32_t DDRSS_phyIndepReg[] = {
 0x00000a00U,
 0x2e8d79e2U,
 0x00b8d308U,
 0x01001387U,
 0x00000001U,
 0x00010064U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00030001U,
 0x00000000U,
 0x00010001U,
 0x00000005U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x280d0001U,
 0x00000000U,
 0x00030000U,
 0x00003200U,
 0x00000000U,
 0x00000000U,
 0x01060602U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000001U,
 0x00000055U,
 0x000000aaU,
 0x000000adU,
 0x00000052U,
 0x0000006aU,
 0x00000095U,
 0x00000095U,
 0x000000adU,
 0x00000000U,
 0x00000000U,
 0x00030300U,
 0x00000014U,
 0x000007d0U,
 0x00000300U,
 0x00000000U,
 0x00000000U,
 0x01000000U,
 0x00010101U,
 0x01000000U,
 0x03000000U,
 0x00010000U,
 0x00001703U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x0a0a140aU,
 0x10020401U,
 0x01000210U,
 0x05000404U,
 0x00000001U,
 0x0001010eU,
 0x02040300U,
 0x00010000U,
 0x00000034U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000005U,
 0x01000000U,
 0x04020000U,
 0x00020000U,
 0x00010002U,
 0x00000001U,
 0x00020001U,
 0x00020002U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000300U,
 0x0a090b0cU,
 0x04060708U,
 0x01000005U,
 0x00000800U,
 0x00000000U,
 0x00010008U,
 0x00000000U,
 0x0000aa00U,
 0x00000000U,
 0x00010000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00010100U,
 0x00000000U,
 0x00000000U,
 0x000000a0U,
 0x00000190U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x01000000U,
 0x00010003U,
 0x02000101U,
 0x01030001U,
 0x00010400U,
 0x06000105U,
 0x01070001U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00010000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00010000U,
 0x00000004U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00007800U,
 0x00780078U,
 0x00141414U,
 0x00000039U,
 0x00000039U,
 0x00040039U,
 0x04000400U,
 0x68040009U,
 0x04000918U,
 0x00091868U,
 0x00186804U,
 0x000001b8U,
 0x00001860U,
 0x000001b8U,
 0x00001860U,
 0x000001b8U,
 0x04001860U,
 0x00000404U,
 0x00001500U,
 0x00150015U,
 0x010c010cU,
 0x0000010cU,
 0x00000000U,
 0x03000000U,
 0x01010303U,
 0x01010101U,
 0x00181818U,
 0x01000000U,
 0x00000000U,
 0x0b000000U,
 0x0a0a0b0bU,
 0x0303030aU,
 0x00090031U,
 0x00090031U,
 0x00090031U,
 0x00000000U,
 0x0000000aU,
 0x0000000aU,
 0x0100000aU,
 0x00000000U,
 0x00000100U,
 0x01000000U,
 0x00000000U,
 0x1e1a1e1aU,
 0x01011e1aU,
 0x0d090701U,
 0x0d09070dU,
 0x0d09070dU,
 0x000c000dU,
 0x00001000U,
 0x00000c00U,
 0x00001000U,
 0x00000c00U,
 0x16001000U,
 0x001600c8U,
 0x001600c8U,
 0x010100c8U,
 0x00001b01U,
 0x1e1a0051U,
 0x03000001U,
 0x001b0a0bU,
 0x1e1a0051U,
 0x03000001U,
 0x001b0a0bU,
 0x1e1a0051U,
 0x03000001U,
 0x00000a0bU,
 0x0d0c0700U,
 0x000d0605U,
 0x0000c570U,
 0x2000001dU,
 0x180a0800U,
 0x0c071c1cU,
 0x0d06050dU,
 0x0000c570U,
 0x2000001dU,
 0x180a0800U,
 0x0c071c1cU,
 0x0d06050dU,
 0x0000c570U,
 0x2000001dU,
 0x180a0800U,
 0x00001c1cU,
 0x000030c0U,
 0x0001e780U,
 0x000030c0U,
 0x0001e780U,
 0x000030c0U,
 0x0001e780U,
 0x03000300U,
 0x03030300U,
 0x00025503U,
 0x02550255U,
 0x0c080c08U,
 0x00000c08U,
 0x00000008U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000168U,
 0x00000008U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000168U,
 0x00000008U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x02000168U,
 0x03200080U,
 0x00020000U,
 0x03200080U,
 0x00020000U,
 0x03200080U,
 0x00000000U,
 0x00000000U,
 0x00040404U,
 0x00000000U,
 0x02010102U,
 0x67676767U,
 0x00000202U,
 0x55000000U,
 0x00000000U,
 0x3c00005aU,
 0x00005500U,
 0x00005a00U,
 0x0d100f3cU,
 0x0003020eU,
 0x00000001U,
 0x01000000U,
 0x00020201U,
 0x00000000U,
 0x00000414U,
 0x00000201U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000481U,
 0x00000400U,
 0x00004d4dU,
 0x00000414U,
 0x00000201U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000481U,
 0x00000400U,
 0x00004d4dU,
 0x00000414U,
 0x00000201U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000481U,
 0x00000400U,
 0x00004d4dU,
 0x00000414U,
 0x00000201U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000481U,
 0x00000400U,
 0x00004d4dU,
 0x00000414U,
 0x00000201U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000481U,
 0x00000400U,
 0x00004d4dU,
 0x00000414U,
 0x00000201U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000481U,
 0x00000400U,
 0x00004d4dU,
};

uint32_t DDRSS_phyReg[] = 
{
 0x04c00002U,
 0x00000000U,
 0x00000200U,
 0x00000000U,
 0x00000000U,
 0x05000000U,
 0x00010504U,
 0x00000005U,
 0x00000001U,
 0x00000000U,
 0x00000000U,
 0x010101ffU,
 0x00000000U,
 0x00c00004U,
 0x00cc0008U,
 0x00660201U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x0000AAAAU,
 0x00005555U,
 0x0000B5B5U,
 0x00004A4AU,
 0x00005656U,
 0x0000A9A9U,
 0x0000B7B7U,
 0x00004848U,
 0x00000000U,
 0x00000000U,
 0x08000000U,
 0x04000008U,
 0x00000408U,
 0x00e4e400U,
 0x00070820U,
 0x000c0020U,
 0x00062000U,
 0x00000100U,
 0x55555555U,
 0xAAAAAAAAU,
 0x55555555U,
 0xAAAAAAAAU,
 0x00005555U,
 0x01000100U,
 0x00800180U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000004U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x041f07ffU,
 0x00000000U,
 0x01ff0801U,
 0x2000ff08U,
 0x20000125U,
 0x07FF0200U,
 0x0000dd01U,
 0x00000303U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x02020000U,
 0x51515042U,
 0x31804000U,
 0x041f00a0U,
 0x00008080U,
 0x04050000U,
 0x00000504U,
 0x42100010U,
 0x010c053eU,
 0x000f0c14U,
 0x01000140U,
 0x007a0120U,
 0x00000c00U,
 0x000001ccU,
 0x20100200U,
 0x00000005U,
 0x76543210U,
 0x00000008U,
 0x02800280U,
 0x02800280U,
 0x02800280U,
 0x02800280U,
 0x00000280U,
 0x00008000U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00990080U,
 0x01A00000U,
 0x00000000U,
 0x00000000U,
 0x00080200U,
 0x00000000U,
 0x00000000U,
 0x04c00002U,
 0x00000000U,
 0x00000200U,
 0x00000000U,
 0x00000000U,
 0x05000000U,
 0x00010504U,
 0x00000005U,
 0x00000001U,
 0x00000000U,
 0x00000000U,
 0x010101ffU,
 0x00000000U,
 0x00c00004U,
 0x00cc0008U,
 0x00660201U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x0000AAAAU,
 0x00005555U,
 0x0000B5B5U,
 0x00004A4AU,
 0x00005656U,
 0x0000A9A9U,
 0x0000B7B7U,
 0x00004848U,
 0x00000000U,
 0x00000000U,
 0x08000000U,
 0x04000008U,
 0x00000408U,
 0x00e4e400U,
 0x00070820U,
 0x000c0020U,
 0x00062000U,
 0x00000100U,
 0x55555555U,
 0xAAAAAAAAU,
 0x55555555U,
 0xAAAAAAAAU,
 0x00005555U,
 0x01000100U,
 0x00800180U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000004U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x041f07ffU,
 0x00000000U,
 0x01ff0801U,
 0x2000ff08U,
 0x20000125U,
 0x07FF0200U,
 0x0000dd01U,
 0x00000303U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x02020000U,
 0x51515042U,
 0x31804000U,
 0x041f00a0U,
 0x00008080U,
 0x04050000U,
 0x00000504U,
 0x42100010U,
 0x010c053eU,
 0x000f0c14U,
 0x01000140U,
 0x007a0120U,
 0x00000c00U,
 0x000001ccU,
 0x20100200U,
 0x00000005U,
 0x76543210U,
 0x00000008U,
 0x02800280U,
 0x02800280U,
 0x02800280U,
 0x02800280U,
 0x00000280U,
 0x00008000U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00800080U,
 0x00990080U,
 0x01A00000U,
 0x00000000U,
 0x00000000U,
 0x00080200U,
 0x00000000U,
 0x00000000U,
 0x00000100U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000100U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00400000U,
 0x00000080U,
 0x00dcba98U,
 0x03000000U,
 0x00200000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x0000002aU,
 0x00000015U,
 0x00000015U,
 0x0000002aU,
 0x00000033U,
 0x0000000cU,
 0x0000000cU,
 0x00000033U,
 0x0a418820U,
 0x103f0000U,
 0x000f013fU,
 0x0000000fU,
 0x020000ffU,
 0x00030000U,
 0x00000300U,
 0x00000300U,
 0x00000300U,
 0x00000300U,
 0x00000300U,
 0x42080010U,
 0x0000803eU,
 0x00000003U,
 0x00000002U,
 0x00000100U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000100U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00400000U,
 0x00000080U,
 0x00dcba98U,
 0x03000000U,
 0x00200000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x0000002aU,
 0x00000015U,
 0x00000015U,
 0x0000002aU,
 0x00000033U,
 0x0000000cU,
 0x0000000cU,
 0x00000033U,
 0x16a4a0e6U,
 0x103f0000U,
 0x000f003fU,
 0x0000000fU,
 0x020000ffU,
 0x00030000U,
 0x00000300U,
 0x00000300U,
 0x00000300U,
 0x00000300U,
 0x00000300U,
 0x42080010U,
 0x0000803eU,
 0x00000003U,
 0x00000002U,
 0x00000100U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000100U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00400000U,
 0x00000080U,
 0x00dcba98U,
 0x03000000U,
 0x00200000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x0000002aU,
 0x00000015U,
 0x00000015U,
 0x0000002aU,
 0x00000033U,
 0x0000000cU,
 0x0000000cU,
 0x00000033U,
 0x2307b9acU,
 0x10030000U,
 0x000f0003U,
 0x0000000fU,
 0x020000ffU,
 0x00030000U,
 0x00000300U,
 0x00000300U,
 0x00000300U,
 0x00000300U,
 0x00000300U,
 0x42080010U,
 0x0000803eU,
 0x00000003U,
 0x00000002U,
 0x00000000U,
 0x00000100U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00050000U,
 0x04000100U,
 0x00000055U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x01002000U,
 0x00004003U,
 0x00020028U,
 0x00010100U,
 0x00000001U,
 0x00000000U,
 0x0f0f0e06U,
 0x00010101U,
 0x010f0004U,
 0x00000000U,
 0x00000000U,
 0x00000064U,
 0x00000000U,
 0x00000000U,
 0x01020103U,
 0x03020102U,
 0x03030303U,
 0x03030303U,
 0x00041142U,
 0x00005001U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x07070101U,
 0x00005401U,
 0x000040a2U,
 0x00004410U,
 0x00004410U,
 0x00004410U,
 0x00004410U,
 0x00004410U,
 0x00004410U,
 0x00004410U,
 0x00004410U,
 0x00004410U,
 0x00004410U,
 0x00000000U,
 0x00000044U,
 0x00000000U,
 0x00000208U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x03000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x04102035U,
 0x00041020U,
 0x01c98c98U,
 0x3f400000U,
 0x3f3f1f3fU,
 0x1f3f3f1fU,
 0x001f3f3fU,
 0x00000000U,
 0x00000000U,
 0x00000001U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x76543210U,
 0x00000198U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00040700U,
 0x00000000U,
 0x00000000U,
 0x00000000U,
 0x00000002U,
 0x00000100U,
 0x00000000U,
 0x00000000U,
 0x00020000U,
 0x00000000U,
 0x00001142U,
 0x03020400U,
 0x00000080U,
 0x03000300U,
 0x03000300U,
 0x03000300U,
 0x03000300U,
 0x03000300U,
 0x03000300U,
 0x00000300U,
 0x00000300U,
 0x00000300U,
 0x00000300U,
 0x000200ffU,
 0x00020008U,
 0x48000900U,
 0x00000100U,
 0x00010011U,
 0x01ffff00U,
 0x000100ffU,
 0x00008011U,
 0x00010011U,
 0x01ffff00U,
 0x00010011U,
 0x01ffff00U,
 0x00010011U,
 0x01ffff00U,
 0x00010011U,
 0x01ffff00U,
 0x00010011U,
 0x01ffff00U,
 0x20040002U,
};

uint16_t DDRSS_ctlRegNum[] =
{
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88, // Removed for LPDDR4	89,
	90,
	91,
	92,
	93,
	94,
	95,
	96,
	97,
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	123,
	124,
	125,
	126,
	127,
	128,
	129,
	130,
	131,
	132,
	133,
	134,
	135,
	136,
	137,
	138,
	139,
	140,
	141,
	142,
	143,
	144,
	145,
	146,
	147,
	148,
	149,
	150,
	151,
	152,
	153,
	154,
	155,
	156,
	157,
	158,
	159,
	160,
	161,
	162,
	163,
	164,
	165,
	166,
	167,
	168,
	169,
	170,
	171,
	172,
	173,
	174,
	175,
	176,
	177,
	178,
	179,
	180,
	181,
	182,
	183,
	184,
	185,
	186,
	187,
	188,
	189,
	190,
	191,
	192,
	193,
	194,
	195,
	196,
	197,
	198,
	199,
	200,
	201,
	202,
	203,
	204,
	205,
	206,
	207,
	208,
	209,
	210,
	211,
	212,
	213,
	214,
	215,
	216,
	217,
	218,
	219,
	220,
	221,
	222,
	223,
	224,
	225,
	226,
	227,
	228,
	229,
	230,
	231,
	232,
	233,
	234,
	235,
	236,
	237,
	238,
	239,
	240,
	241,
	242,
	243,
	244,
	245,
	246,
	247,
	248,
	249,
	250,
	251,
	252,
	253,
	254,
	255,
	256,
	257,
	258,
	259,
	260,
	261,
	262,
	263,
	264,
	265,
	266,
	267,
	268,
	269,
	270,
	271,
	272,
	273,
	274,
	275,
	276,
	277,
	278,
	279,
	280,
	281,
	282,
	283,
	284,
	285,
	286,
	287,
	288,
	289,
	290,
	291,
	292,
	293,
	294,
	295,
	296,
	297,
	298,
	299,
	300,
	301,
	302,
	303,
	304,
	305,
	306,
	307,
	308,
	309,
	310,
	311,
	312,
	313,
	314,
	315,
	316,
	317,
	318,
	319,
	320,
	321,
	322,
	323,
	324,
	325,
	326,
	327,
	328,
	329,
	330,
	331,
	332,
	333,
	334,
	335,
	336,
	337,
	338,
	339,
	340,
	341,
	342,
	343,
	344,
	345,
	346,
	347,
	348,
	349,
	350,
	351,
	352,
	353,
	354,
	355,
	356,
	357,
	358,
	359,
	360,
	361,
	362,
	363,
	364,
	365,
	366,
	367,
	368,
	369,
	370,
	371,
	372,
	373,
	374,
	375,
	376,
	377,
	378,
	379,
	380,
	381,
	382,
	383,
	384,
	385,
	386,
	387,
	388,
	389,
	390,
	391,
	392,
	393,
	394,
	395,
	396,
	397,
	398,
	399,
	400,
	401,
	402,
	403,
	404,
	405,
	406,
	407,
	408,
	409,
	410,
	411,
	412,
	413,
	414,
	415,
	416,
	417,
	418,
	419,
	420,
	421,
	422,
};

uint16_t DDRSS_phyIndepRegNum[] =
{
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	91,
	92,
	93,
	94,
	95,
	96,
	97,
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	123,
	124,
	125,
	126,
	127,
	128,
	129,
	130,
	131,
	132,
	133,
	134,
	135,
	136,
	137,
	138,
	139,
	140,
	141,
	142,
	143,
	144,
	145,
	146,
	147,
	148,
	149,
	150,
	151,
	152,
	153,
	154,
	155,
	156,
	157,
	158,
	159,
	160,
	161,
	162,
	163,
	164,
	165,
	166,
	167,
	168,
	169,
	170,
	171,
	172,
	173,
	174,
	175,
	176,
	177,
	178,
	179,
	180,
	181,
	182,
	183,
	184,
	185,
	186,
	187,
	188,
	189,
	190,
	191,
	192,
	193,
	194,
	195,
	196,
	197,
	198,
	199,
	200,
	201,
	202,
	203,
	204,
	205,
	206,
	207,
	208,
	209,
	210,
	211,
	212,
	213,
	214,
	215,
	216,
	217,
	218,
	219,
	220,
	221,
	222,
	223,
	224,
	225,
	226,
	227,
	228,
	229,
	230,
	231,
	232,
	233,
	234,
	235,
	236,
	237,
	238,
	239,
	240,
	241,
	242,
	243,
	244,
	245,
	246,
	247,
	248,
	249,
	250,
	251,
	252,
	253,
	254,
	255,
	256,
	257,
	258,
	259,
	260,
	261,
	262,
	263,
	264,
	265,
	266,
	267,
	268,
	269,
	270,
	271,
	272,
	273,
	274,
	275,
	276,
	277,
	278,
	279,
	280,
	281,
	282,
	283,
	284,
	285,
	286,
	287,
	288,
	289,
	290,
	291,
	292,
	293,
	294,
	295,
	296,
	297,
	298,
	299,
300,
301,
302,
303,
304,
305,
306,
307,
308,
309,
310,
311,
312,
313,
314,
315,
316,
317,
318,
319,
320,
321,
322,
323,
324,
325,
326,
327,
328,
329,
330,
331,
332,
333,
334,
335,
336,
337,
338,
339,
340,
341,
342,
343,
344,
};

uint16_t DDRSS_phyRegNum[] =
{
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	91,
	92,
	93,
	94,
	95,
	96,
	97,
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	123,
	124,
	125,
	256,
	257,
	258,
	259,
	260,
	261,
	262,
	263,
	264,
	265,
	266,
	267,
	268,
	269,
	270,
	271,
	272,
	273,
	274,
	275,
	276,
	277,
	278,
	279,
	280,
	281,
	282,
	283,
	284,
	285,
	286,
	287,
	288,
	289,
	290,
	291,
	292,
	293,
	294,
	295,
	296,
	297,
	298,
	299,
	300,
	301,
	302,
	303,
	304,
	305,
	306,
	307,
	308,
	309,
	310,
	311,
	312,
	313,
	314,
	315,
	316,
	317,
	318,
	319,
	320,
	321,
	322,
	323,
	324,
	325,
	326,
	327,
	328,
	329,
	330,
	331,
	332,
	333,
	334,
	335,
	336,
	337,
	338,
	339,
	340,
	341,
	342,
	343,
	344,
	345,
	346,
	347,
	348,
	349,
	350,
	351,
	352,
	353,
	354,
	355,
	356,
	357,
	358,
	359,
	360,
	361,
	362,
	363,
	364,
	365,
	366,
	367,
	368,
	369,
	370,
	371,
	372,
	373,
	374,
	375,
	376,
	377,
	378,
	379,
	380,
	381,
	512,
	513,
	514,
	515,
	516,
	517,
	518,
	519,
	520,
	521,
	522,
	523,
	524,
	525,
	526,
	527,
	528,
	529,
	530,
	531,
	532,
	533,
	534,
	535,
	536,
	537,
	538,
	539,
	540,
	541,
	542,
	543,
	544,
	545,
	546,
	547,
	548,
	549,
	550,
	551,
	552,
	553,
	554,
	768,
	769,
	770,
	771,
	772,
	773,
	774,
	775,
	776,
	777,
	778,
	779,
	780,
	781,
	782,
	783,
	784,
	785,
	786,
	787,
	788,
	789,
	790,
	791,
	792,
	793,
	794,
	795,
	796,
	797,
	798,
	799,
	800,
	801,
	802,
	803,
	804,
	805,
	806,
	807,
	808,
	809,
	810,
	1024,
	1025,
	1026,
	1027,
	1028,
	1029,
	1030,
	1031,
	1032,
	1033,
	1034,
	1035,
	1036,
	1037,
	1038,
	1039,
	1040,
	1041,
	1042,
	1043,
	1044,
	1045,
	1046,
	1047,
	1048,
	1049,
	1050,
	1051,
	1052,
	1053,
	1054,
	1055,
	1056,
	1057,
	1058,
	1059,
	1060,
	1061,
	1062,
	1063,
	1064,
	1065,
	1066,
	1280,
	1281,
	1282,
	1283,
	1284,
	1285,
	1286,
	1287,
	1288,
	1289,
	1290,
	1291,
	1292,
	1293,
	1294,
	1295,
	1296,
	1297,
	1298,
	1299,
	1300,
	1301,
	1302,
	1303,
	1304,
	1305,
	1306,
	1307,
	1308,
	1309,
	1310,
 	1311,
	1312,
	1313,
	1314,
	1315,
	1316,
	1317,
	1318,
	1319,
	1320,
	1321,
	1322,
	1323,
	1324,
	1325,
	1326,
	1327,
	1328,
	1329,
	1330,
	1331,
	1332,
	1333,
	1334,
	1335,
	1336,
	1337,
	1338,
	1339,
	1340,
	1341,
	1342,
	1343,
	1344,
	1345,
	1346,
	1347,
	1348,
	1349,
	1350,
	1351,
	1352,
	1353,
	1354,
	1355,
	1356,
	1357,
	1358,
	1359,
	1360,
	1361,
	1362,
	1363,
	1364,
	1365,
	1366,
	1367,
	1368,
	1369,
	1370,
	1371,
	1372,
	1373,
	1374,
	1375,
	1376,
	1377,
	1378,
	1379,
	1380,
	1381,
	1382,
	1383,
	1384,
	1385,
	1386,
	1387,
	1388,
	1389,
	1390,
	1391,
	1392,
	1393,
	1394,
	1395,
	1396,
	1397,
	1398,
	1399,
	1400,
	1401,
	1402,
	1403,
	1404,
	1405,
};

