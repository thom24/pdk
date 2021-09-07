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


/*!
 *  \file
 *  \brief Functions header file for RAT related functions.
 */
#ifndef RAT_FUNCTIONS_H_
#define RAT_FUNCTIONS_H_

#include <stdint.h>
#include <stdbool.h>
#include <ti/drv/lpm/include/io_retention/cgt_pragmas.h>

/*!
 *  \brief Configure a RAT translation region.
 *  \returns PASS if everything worked properly. FAIL if it did not.
 *
 *  \sa CSL_ratConfigRegionTranslation
 *
 *  \code
 *  map 0x9000_0000 to 0x9000_1000 => 0x57_0000_0000 to 0x57_0000_1000
 *  set_rat_region(rat_base, 8, 0x00000057u, 0x90000000u, 0x00000000u, 12);
 *  \endcode
 *
 *  \b Notes:
 *  - On M3, the RAT window starts @ 0x6000_0000 through 0xdfff_ffff.
 *    - This means the 'addr' parameter must be within this window.
 *  - On R5, the entire 32 bit address space can be remapped. Try to use the upper 2GB address space for that mapping so that you don't mask off access to peripherals in the lower 2GB address space.
 *    - E.g. Remap in the 0x8000_0000 through 0xffff_ffff address space.
 */
uint32_t set_rat_region(
    uintptr_t rat_base,     /*< [in] RAT base address. Note: A base2core() conversion is done on this value before use. */
    uint32_t  id,           /*< [in] RAT region ID to use. */
    uint32_t  upper,        /*< [in] Upper 16 bits of the RAT output translated address. */
    uint32_t  addr,         /*< [in] Local 32 bit base address of the region. */
    uint32_t  xlate,        /*< [in] Lower 32 bits of the RAT output translated address. */
    uint32_t  region_width  /*< [in] Region width in bits of the translation window. */
    );

#endif  /* RAT_FUNCTIONS_H_ */

