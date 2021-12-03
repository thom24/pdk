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
 *  \brief MMR related functions
 */

#ifndef _LPM_MMR_FUNCTIONS_H_
#define _LPM_MMR_FUNCTIONS_H_

#include <ti/drv/lpm/include/io_retention/soc_functions.h>
#include <ti/drv/lpm/include/io_retention/mmr_common.h>
#include <ti/drv/lpm/include/io_retention/cgt_pragmas.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief Function to unlock a partition within an MMR block.
 *
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *    uint32_t partition = 7;
 *    Lpm_mmr_unlock(mmr0_cfg_base, partition);
 *  }
 *  \endcode
 */
void Lpm_mmr_unlock (
    uintptr_t base,     /*!< base address of the mmr to unlock */
    uint32_t partition  /*!< which partition to unlock */
    );

/*!
 *  \brief Function to lock a partition within an MMR block.
 *
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *    uint32_t partition = 5;
 *    Lpm_mmr_lock(mmr1_cfg_base, partition);
 *  }
 *  \endcode
 */
void Lpm_mmr_lock (
    uintptr_t base,     /*!< base address of the mmr to target */
    uint32_t partition  /*!< which partition to lock */
    );

/*!
 *  \brief Function to return the partition lock status
 *
 *  \return True for locked, false for unlocked
 *
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *    uint32_t partition = 5;
 *    bool isLocked;
 *    isLocked = Lpm_mmr_isLocked(mmr1_cfg_base, partition);
 *  }
 *  \endcode
 */
bool Lpm_mmr_isLocked (
    uintptr_t base,     /*!< physical base address of the mmr to target */
    uint32_t partition  /*!< which partition to test for lock */
    );

/**
 * @brief return the partition which the offset belongs to.
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *  	int partitionNum = Lpm_getPartitionNum(MAIN_CTRL_MMR, MAIN_CTRL_INTR_EN_CLR);
 *		unlock(partitionNum, base);   
 *  }
 *  \endcode
 * @param base the base address of the register.
 * @param offset the offset of the register.
 * @return the partition which the register belongs to 
 */
uint32_t Lpm_getPartitionNum(unsigned int base, unsigned int offset);

/** @brief Unlock partition unlock_num at MMR with base address base
 *
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *  	int partitionNum = 0;
 *		unlock(partitionNum, base);   
 *    
 *  }
 *  \endcode
 *
 *  @param unlock_num The partition that is required to be unlocked.
 *  @param base the base address of the MMR.
 *  @return Void.
 */
void Lpm_unlock(unsigned int unlock_num, unsigned int base );

/** @brief lock partition lock_num at MMR with base address base
 *
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *  	int partitionNum = 0;
 *		lock(partitionNum, base);   
 *    
 *  }
 *  \endcode
 *  @param lock_num The partition that is required to be locked.
 *  @param base the base address of the MMR.
 *  @return Void.
 */
void Lpm_lock( unsigned int lock_num, unsigned int base);

/*!
 *  \ lock all the partitions at a MMR
 *
 *  \return void 
 *
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *  	lockAllPartitions(MAIN_CTRL_MMR);     
 *    
 *  }
 *  \endcode
 */
void Lpm_lockAllPartitions(
	unsigned int base /*!< base address of the MMR to lock */
	);

/*!
 *  \ unlock all the partitions at a MMR
 *
 *  \return void 
 *
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *  	unlockAllPartitions(MAIN_CTRL_MMR);     
 *    
 *  }
 *  \endcode
 */
void Lpm_unlockAllPartitions(
	unsigned int base /*!< base address of the MMR to unlock */
);



#endif
