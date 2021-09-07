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

#include <ti/drv/lpm/include/lpm_mmr_functions.h>
#include <ti/drv/lpm/include/io_retention/mmr_common.h>
#include <ti/drv/lpm/include/io_retention/cgt_pragmas.h>
#include <ti/drv/lpm/include/io_retention/dev_info.h>
#include <ti/drv/lpm/include/io_retention/j7vcl_base_address.h>

#include <stdint.h>

#define SOC_SPINLOCK_BASEADDR   ( nav0_spinlock0_cfg_base )
#define SOC_NUM_SPINLOCKS       ( 256u )

 int32_t SOC_lock( uint32_t lockNum )
 {
     volatile uint32_t *spinLockReg;
 
     if ( lockNum >= SOC_NUM_SPINLOCKS )
         return -1;
 
     spinLockReg = (volatile uint32_t *)(base2core(SOC_SPINLOCK_BASEADDR + 0x800));
 
     while ( spinLockReg[ lockNum ] != 0x0 );
 
     // Ensure all memory loads/stores executed by this proc are seen
     // by other processors before any load/stores follow the DMB.
     // Also prevents speculative execution before lock is held
 //    SOC_memory_barrier();
 
     return 0;
 }


int32_t SOC_unlock( uint32_t lockNum )
{
    volatile uint32_t *spinLockReg;

    if ( lockNum >= SOC_NUM_SPINLOCKS )
        return -1;

    spinLockReg = (volatile uint32_t *)(base2core(SOC_SPINLOCK_BASEADDR + 0x800));

    // Ensure all other cores can see modifications made to critical section data
//    SOC_memory_barrier();

    spinLockReg[ lockNum ] = 0x0;

    return 0;
}

bool mmr_isLocked(
    uintptr_t base,
    uint32_t  partition
    )
{
    volatile uint32_t * lock = mkptr(base, partition * 0x4000 + MMR_LOCK0_KICK0);

    return (*lock & 0x1u) ? false : true;
}

/*!
 *  \brief Function to unlock a partition within an MMR block.
 *
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *    uint32_t partition = 7;
 *    mmr_unlock(mmr0_cfg_base, partition);
 *  }
 *  \endcode
 */
void mmr_unlock (
    uintptr_t base,     /*!< physical base address of the mmr to unlock */
    uint32_t partition  /*!< which partition to unlock */
    )
{
#ifndef TBARM9CORE
  /* translate the base address */
  volatile uint32_t * lock = mkptr(base, partition * 0x4000 + MMR_LOCK0_KICK0);

  if (!mmr_isLocked( base, partition ) ) {
    /* return if already unlocked */
    return;
  } else {
    /* unlock the partition */
    SOC_lock( SOC_LOCK_MMR_UNLOCK );
    lock[0] = MMR_LOCK0_KICK0_UNLOCK_VAL;
    lock[1] = MMR_LOCK0_KICK1_UNLOCK_VAL;
    SOC_unlock( SOC_LOCK_MMR_UNLOCK );
  }
#endif
}

/*!
 *  \brief Function to lock a partition within an MMR block.
 *
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *    uint32_t partition = 5;
 *    mmr_lock(mmr1_cfg_base, partition);
 *  }
 *  \endcode
 */
void mmr_lock (
    uintptr_t base,     /*!< physical base address of the mmr to target */
    uint32_t partition  /*!< which partition to lock */
    )
{
  volatile uint32_t * lock = mkptr(base, partition * 0x4000 + MMR_LOCK0_KICK0);
  *lock = 0;
}

/* Below is the code for MMR modules */

#include <ti/drv/lpm/include/io_retention/base_address.h>
#include <ti/drv/lpm/include/io_retention/pll_mmr_common.h>

#define show_exp_val (*mkptr(mmr2_cfg_base, MMR_TEST_DEBUG_ALLRST1)= exp_val)

void unlock(unsigned int unlock_num, unsigned int base ) {
  unsigned int const_lock_start_addr, partitionOffset, kick1_val, kick0_val;
  if(base == mmr0_cfg_base || base == mmr1_cfg_base || base == mmr2_cfg_base){
	const_lock_start_addr = 0x01008;
	kick0_val = MMR_LOCK0_KICK0_UNLOCK_VAL;
	kick1_val = MMR_LOCK0_KICK1_UNLOCK_VAL; 
	partitionOffset = 0x04000; 
  }else{
	const_lock_start_addr = 0x00010;
	kick0_val = MMR_PLL0_KICK0_UNLOCK_VAL;
	kick1_val = MMR_PLL0_KICK1_UNLOCK_VAL; 	
	partitionOffset = 0x01000;
  }
//  if(*mkptr(base,(const_lock_start_addr+partitionOffset*unlock_num)) == kick0_val && *mkptr(base,(const_lock_start_addr+partitionOffset*unlock_num+0x00004)) == kick1_val){
//	return; 
//  }
  *mkptr(base,(const_lock_start_addr+partitionOffset*unlock_num))         = kick0_val;
  *mkptr(base,(const_lock_start_addr+partitionOffset*unlock_num+0x00004)) = kick1_val;
}

void pllmmr_unlock(unsigned int unlock_num, unsigned int base) {
  unsigned int const_lock_start_addr = 0x00010;
  *mkptr(base,(const_lock_start_addr+0x01000*unlock_num))         = MMR_PLL0_KICK0_UNLOCK_VAL;
  *mkptr(base,(const_lock_start_addr+0x01000*unlock_num+0x00004)) = MMR_PLL0_KICK1_UNLOCK_VAL;
}

void lock( unsigned int lock_num, unsigned int base) {
  unsigned int const_lock_start_addr, partitionOffset;
  if(base == mmr0_cfg_base || base == mmr1_cfg_base || base == mmr2_cfg_base){
	const_lock_start_addr = 0x01008;
	partitionOffset = 0x04000; 
  }else if (base == pllmmr0_cfg_base || base == pllmmr1_cfg_base){
	const_lock_start_addr = 0x00010;
	partitionOffset = 0x01000;
  }
  *mkptr(base,(const_lock_start_addr+partitionOffset*lock_num+0x00004)) = 0;
}

// @ needs to be changed in different projects
void lockAllBut(unsigned int base, uint32_t unlockPart){
	unsigned int numPartitions, i; 
	switch(base){
		case pllmmr0_cfg_base:
			numPartitions = 2 ;
			break;
		case pllmmr1_cfg_base:
			numPartitions = 14;
			break;
		case mmr0_cfg_base:
			numPartitions = 7; 
			break;
		case mmr1_cfg_base:
			numPartitions = 4; 
			break;
		case mmr2_cfg_base:
		  	numPartitions = 7; 	
			break;
		default: 
			numPartitions = 0; 
	}
	unlock(unlockPart, base); 

	for(i = 0; i <= numPartitions; i++){
		if ((base == mmr0_cfg_base && i == 4) || (base == mmr0_cfg_base && i == 5)) 
      continue;
		if ((base == mmr2_cfg_base) && (i == 6))
      continue; 
		if (base == pllmmr1_cfg_base) {
			if ((i == 5) || (i == 6) || (i == 9) || (i == 10) || (i == 11) || (i == 13))
        continue; 
		}
		if ((i == unlockPart) || (base == secmmr0_cfgr0_base) || (base == secmmr0_cfgr2_base) || (base == secmmr1_cfgr0_base) || (base == secmmr1_cfgr2_base))
      continue;  
		lock(i, base); 
	}
}


// @ needs to be changed in different projects
void lockAllPartitions(unsigned int base){
	unsigned int numPartitions, i; 
	switch(base){
		case pllmmr0_cfg_base:
			numPartitions = 2 ;
			break;
		case pllmmr1_cfg_base:
			numPartitions = 14;
			break;
		case mmr0_cfg_base:
			numPartitions = 7; 
			break;
		case mmr1_cfg_base:
			numPartitions = 4; 
			break;
		case mmr2_cfg_base:
		  	numPartitions = 7; 	
			break;
		default: 
			numPartitions = 0; 
	}	
	for(i = 0; i <= numPartitions; i++){
		if ((base == mmr0_cfg_base && i == 4) || (base == mmr0_cfg_base && i == 5)) 
      continue;
		if ((base == mmr2_cfg_base) && (i == 6))
      continue; 
		if (base == pllmmr1_cfg_base) {
			if ((i == 5) || (i == 6) || (i == 9) || (i == 10) || (i == 11) || (i == 13))
        continue; 
		}

		lock(i, base); 
	}
}

uint32_t getPartitionNum(unsigned int base, unsigned int offset){
	uint32_t result = -1;
	
	if(base == mmr0_cfg_base || base == mmr1_cfg_base || base == mmr2_cfg_base){
		result = offset/0x4000;
	}else if (base == pllmmr0_cfg_base || base == pllmmr1_cfg_base ){
		result = offset/0x1000;
	}
	return result; 
}
//
// @ needs to be changed in different projects
void unlockAllBut(unsigned int base, uint32_t lockPart){
	unsigned int numPartitions, i; 
	switch(base){
		case pllmmr0_cfg_base:
			numPartitions = 2 ;
			break;
		case pllmmr1_cfg_base:
			numPartitions = 14;
			break;
		case mmr0_cfg_base:
			numPartitions = 7; 
			break;
		case mmr1_cfg_base:
			numPartitions = 4; 
			break;
		case mmr2_cfg_base:
		  	numPartitions = 7; 	
			break;
		default: 
			numPartitions = 0; 
	}

	lock(lockPart, base);
	for(i = 0; i <= numPartitions; i++){
		if ((base == mmr0_cfg_base && i == 4) || (base == mmr0_cfg_base && i == 5)) 
      continue;
		if ((base == mmr2_cfg_base) && (i == 6))
      continue; 
		if (base == pllmmr1_cfg_base) {
			if ((i == 5) || (i == 6) || (i == 9) || (i == 10) || (i == 11) || (i == 13))
        continue; 
		}

		if(lockPart == i ) continue;  
		unlock(i, base); 
	}
}

// @ needs to be changed in different projects
void unlockAllPartitions(unsigned int base){
	unsigned int numPartitions, i; 
	switch(base){
		case pllmmr0_cfg_base:
			numPartitions = 2;
			break;
		case pllmmr1_cfg_base:
			numPartitions = 14;
			break;
		case mmr0_cfg_base:
			numPartitions = 7; 
			break;
		case mmr1_cfg_base:
			numPartitions = 4; 
			break;
		case mmr2_cfg_base:
		  	numPartitions = 7; 	
			break;
		default: 
			numPartitions = 0; 
	}	
	for(i = 0; i <= numPartitions; i++){
		if ((base == mmr0_cfg_base && i == 4) || (base == mmr0_cfg_base && i == 5)) 
      continue;
		if ((base == mmr2_cfg_base) && (i == 6))
      continue; 
		if (base == pllmmr1_cfg_base) {
			if ((i == 5) || (i == 6) || (i == 9) || (i == 10) || (i == 11) || (i == 13))
        continue; 
		}
		unlock(i, base); 
	}
}

void pllmmr_lock(unsigned int lock_num, unsigned int base) {
  unsigned int const_lock_start_addr = 0x00010;
  *mkptr(base,(const_lock_start_addr+0x01000*lock_num+0x00004)) = 0;
}

void reg_reset_test(unsigned int base, struct reg_info *ri) {
  unsigned int exp_val;
  if (*mkptr(base,ri->offset) != ri->reset_val) {
    exp_val = ri->reset_val;
    show_exp_val;
    err_cnt++;
  }
}

unsigned int reg_reset_test_store(unsigned int base, struct reg_info *ri) {
  unsigned int exp_val, rd_val;
  rd_val = *mkptr(base,ri->offset);
  if (rd_val != ri->reset_val) {
    exp_val = ri->reset_val;
    show_exp_val;
    err_cnt++;
  }
  return rd_val;
}

// unsigned int reg_reset_test_care(unsigned int base, struct reg_info *ri) {
//   unsigned int exp_val, rd_val;
//   rd_val = *mkptr(base,ri->offset);
//   if (((rd_val)& (ri->rd_val)) != (ri->reset_val & ri->rd_val)) {
//     exp_val = ri->reset_val;
//     show_exp_val;
//     err_cnt++;
//   }
//   return rd_val;
// }


// unsigned int reg_reset_test_care_unchange(unsigned int base, struct reg_info *ri) {
//   unsigned int exp_val, rd_val, prev_wr_val;
//   rd_val = *mkptr(base,ri->offset);
//   prev_wr_val = ((~ri->reset_val) & ri->wr_eff_mask)| ((ri->reset_val & ~ ri->wr_eff_mask));
//   if ( (rd_val & ri->rd_val) != (prev_wr_val & ri->rd_val)) {
//     exp_val = prev_wr_val & ri->rd_val;
// 	rd_val = rd_val & ri->rd_val; 
//     show_exp_val;
//     err_cnt++;
//   }
//   return rd_val;
// }

unsigned int reg_rd_store(unsigned int base, struct reg_info *ri) {
  unsigned int rd_val;
  rd_val = *mkptr(base,ri->offset);
  return rd_val;
}

void resume_register(unsigned int base, struct reg_info *ri, unsigned int stored_val) {
  *mkptr(base,ri->offset) = stored_val;
}

void reg_rw_test(unsigned int base, struct reg_info *ri, unsigned int pat) {
    unsigned int write_val, read_val, exp_val, temp;
   	temp = *mkptr(base, ri->offset);  
    write_val = ((pat & ri->wr_mask) | (temp & ~ri->wr_mask));
    exp_val = ((write_val & ri->wr_eff_mask)|
             (temp & ~ri->wr_eff_mask));

    *mkptr(base,ri->offset)= write_val;
    read_val = *mkptr(base,ri->offset);
    if ((read_val & ri->rd_val)!= (exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
}

void reg_rw_test_locked (unsigned int base, struct reg_info *ri, unsigned int pat) {
    unsigned int write_val, read_val, exp_val, old_value;
	old_value = *mkptr(base, ri->offset);
    write_val = ((pat & ri->wr_mask) | (old_value & ~ri->wr_mask));

	if(ri->wr_val){
		exp_val = old_value;
	}else{
		 exp_val = ((write_val & ri->wr_eff_mask) | (old_value & ~ri->wr_eff_mask));
	}
   
    *mkptr(base,ri->offset)= write_val;
    read_val = *mkptr(base,ri->offset);
    if ((read_val & ri->rd_val)!= (exp_val&ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
}

// Same as RW lock test, except that bits under 'mask' are expected NOT to change and others are
// If either of these are violated, it is an error
void reg_rw_test_locked_with_mask (unsigned int base, struct reg_info *ri, unsigned int mask) {
    unsigned int write_val, read_val, exp_val, old_value;
    exp_val = 0;
	  old_value = *mkptr(base, ri->offset);

    // Choose opposite of current value as write pattern
    unsigned int pat = ~old_value;

    write_val = ((pat & ri->wr_mask) | (old_value & ~ri->wr_mask));

    *mkptr(base,ri->offset)= write_val;
    read_val = *mkptr(base,ri->offset);

    // ~OLD_VAL XOR NEW_VAL == MASK
    // i.e. all the bits that went through are not masked and vice versa
    if (((read_val ^ pat) & ri->rd_val) != (mask & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
}

void reg_wr(unsigned int base, struct reg_info *ri, unsigned int pat) {
    ri->wr_val = ((pat & ri->wr_mask) | (ri->reset_val & ~ri->wr_mask));
    *mkptr(base,ri->offset)= ri->wr_val;
}

void reg_rd_test(unsigned int base, struct reg_info *ri, unsigned int pat) {
    unsigned int read_val, exp_val;

    ri->wr_val = ((pat & ri->wr_mask) | (ri->reset_val & ~ri->wr_mask));
    exp_val = ((ri->wr_val & ri->wr_eff_mask)|
              (ri->reset_val & ~ri->wr_eff_mask));

    read_val = *mkptr(base,ri->offset);
    if ((read_val & ri->rd_val)!= exp_val) {
      show_exp_val;
      err_cnt++;
    }
}

// This function tests:
// 1. Claim registers (RVC, WRC, not writable in self address, writable in proxy address)
// 2. Claim bits corresponding to MMR
// 3. Proxy scheme
void reg_rw_test_with_proxy (unsigned int base, struct reg_info *ri, unsigned int pat) {
    unsigned int write_val, read_val, exp_val, claim_bit, claim_reg_num, claim_addr, old_val;
    unsigned int read_val_proxy;
    // Claim registers are spec'ed in CTRL_MMR spec; E.g. Maxwell WKUP_CTRL_MMR spec
    claim_bit     = 1 << ((ri->offset & 0x0007F) >> 2);
    claim_reg_num = (ri->offset & 0x00F80) >> 7;
    claim_addr    = MMR_P0_CLAIM0_RDONLY + (claim_reg_num << 2) + (ri->offset & 0x1C000);

    // Check Claim register reset value or previously cleared value
    exp_val = 0x00000000; 

    read_val       = *mkptr(base, claim_addr);
    read_val_proxy = *mkptr(base, claim_addr+0x2000);
    if ((read_val) != exp_val) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy) != exp_val) {
      show_exp_val;
      err_cnt++;
    }

    // Verify Claim register can't be written through CLAIM_..._READONLY
    write_val = claim_bit;
    exp_val   = 0x00000000;

    *mkptr(base, claim_addr) = write_val;
    read_val       = *mkptr(base, claim_addr);
    read_val_proxy = *mkptr(base, claim_addr+0x2000);
    if ((read_val) != exp_val) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy) != exp_val) {
      show_exp_val;
      err_cnt++;
    }
	old_val = *mkptr(base, ri->offset);
    // Verify MMR is writable through MMR itself
    write_val = ((pat & ri->wr_mask) | (old_val & ~ri->wr_mask));
    exp_val   = ((write_val & ri->wr_eff_mask) |
                 (old_val & ~ri->wr_eff_mask));

    *mkptr(base, ri->offset) = write_val;
    read_val       = *mkptr(base, ri->offset);
    read_val_proxy = *mkptr(base, ri->offset+0x2000);
    if ((read_val & ri->rd_val) != (exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy & ri->rd_val) != (exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
    old_val = *mkptr(base, ri->offset);
    write_val = ~write_val;
    exp_val   = ((write_val & ri->wr_eff_mask) |
                 (old_val & ~ri->wr_eff_mask));

    *mkptr(base, ri->offset) = write_val;
    read_val       = *mkptr(base, ri->offset);
    read_val_proxy = *mkptr(base, ri->offset+0x2000);
    if ((read_val & ri->rd_val) != (exp_val& ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy & ri->rd_val) != (exp_val& ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }

    // Verify MMR is writable through MMR_proxy
	old_val = *mkptr(base, ri->offset); 
    write_val = ((pat & ri->wr_mask) | (old_val & ~ri->wr_mask));
    exp_val   = ((write_val & ri->wr_eff_mask) |
                 (old_val & ~ri->wr_eff_mask));

    *mkptr(base, ri->offset+0x2000) = write_val;
    read_val       = *mkptr(base, ri->offset);
    read_val_proxy = *mkptr(base, ri->offset+0x2000);
    if ((read_val & ri->rd_val) != (exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy & ri->rd_val) != (exp_val& ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
	old_val = *mkptr(base, ri->offset);
    write_val = ~write_val;
    exp_val   = ((write_val & ri->wr_eff_mask) |
                 (old_val & ~ri->wr_eff_mask));

    *mkptr(base, ri->offset+0x2000) = write_val;
    read_val       = *mkptr(base, ri->offset);
    read_val_proxy = *mkptr(base, ri->offset+0x2000);
    if ((read_val & ri->rd_val) != (exp_val&ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy & ri->rd_val) != (exp_val&ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
	
    // Verify Claim bit can't be cleared through CLAIM_..._READONLY
    write_val = 0x1;
    exp_val   = *mkptr(base, claim_addr);

    *mkptr(base, claim_addr) = write_val;
    read_val       = *mkptr(base, claim_addr);
    read_val_proxy = *mkptr(base, claim_addr+0x2000);
    if ((read_val) != (exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy) != (exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }

    // Verify Claim bit can be set through claim register
    write_val = claim_bit;
    exp_val   = claim_bit;

    *mkptr(base, claim_addr+0x2000) = write_val;
    read_val       = *mkptr(base, claim_addr);
    read_val_proxy = *mkptr(base, claim_addr+0x2000);
    if ((read_val) != exp_val) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy) != exp_val) {
      show_exp_val;
      err_cnt++;
    }
    // Verify MMR is not writable through MMR itself
    write_val = ((pat & ri->wr_mask) | (ri->reset_val & ~ri->wr_mask));
    exp_val   = *mkptr(base, ri->offset);

    *mkptr(base, ri->offset) = write_val;
    read_val       = *mkptr(base, ri->offset);
    read_val_proxy = *mkptr(base, ri->offset+0x2000);
    if ((read_val & ri->rd_val) != (exp_val&ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy & ri->rd_val) != (exp_val&ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }

    // Verify MMR is not writable through MMR itself
    write_val = ~write_val;
    exp_val   = *mkptr(base, ri->offset);

    *mkptr(base, ri->offset) = write_val;
    read_val       = *mkptr(base, ri->offset);
    read_val_proxy = *mkptr(base, ri->offset+0x2000);
    if ((read_val & ri->rd_val) != ( exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy & ri->rd_val) != (exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }

    // Verify MMR is writable through MMR_proxy
	old_val = *mkptr(base, ri->offset); 
    write_val = ((pat & ri->wr_mask) | (ri->reset_val & ~ri->wr_mask));
    exp_val   = ((write_val & ri->wr_eff_mask) |
                 (old_val & ~ri->wr_eff_mask));

    *mkptr(base, ri->offset+0x2000) = write_val;
    read_val       = *mkptr(base, ri->offset);
    read_val_proxy = *mkptr(base, ri->offset+0x2000);
    if ((read_val & ri->rd_val) != (exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy & ri->rd_val) != (exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
 	old_val = *mkptr(base, ri->offset);
    write_val = ~write_val;
    exp_val   = ((write_val & ri->wr_eff_mask) |
                 (old_val & ~ri->wr_eff_mask));

    *mkptr(base, ri->offset+0x2000) = write_val;
    read_val       = *mkptr(base, ri->offset);
    read_val_proxy = *mkptr(base, ri->offset+0x2000);
    if ((read_val & ri->rd_val) != (exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }
    if ((read_val_proxy & ri->rd_val) != (exp_val & ri->rd_val)) {
      show_exp_val;
      err_cnt++;
    }

    // Clear Claim register
    *mkptr(base, claim_addr+0x2000) = 0x0;
}

void MMRSetClrInterrupt (unsigned int base, unsigned int offset) {
    *mkptr(base, offset) = 0x00000001;
}



