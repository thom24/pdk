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

/*! \file
 *  \brief Functions header file for SoC-related functions
 */
#ifndef SOC_FUNCTIONS_H_
#define SOC_FUNCTIONS_H_

#include <stdint.h>
#include <stdbool.h>
#include <ti/drv/lpm/include/io_retention/cgt_pragmas.h>

/* CNUM vectors for all the cores of the same core type of the test you're running. */
/* Useful for Z tests. */

#define SOC_ALL_M3CORE_CNUM_VECTOR      (0x0001u)
#define SOC_ALL_R5FCORE_CNUM_VECTOR     (0x0036u)
#define SOC_ALL_A72CORE_CNUM_VECTOR     (0x0180u)
#define SOC_ALL_C71XCORE_CNUM_VECTOR    (0x0000u)
#define SOC_ALL_C66XCORE_CNUM_VECTOR    (0x0000u)

#if (defined A72CORE)
#define SOC_ALL_CORETYPE_CNUM_VECTOR  SOC_ALL_A72CORE_CNUM_VECTOR
#elif (defined R5FCORE)
#define SOC_ALL_CORETYPE_CNUM_VECTOR  SOC_ALL_R5FCORE_CNUM_VECTOR
#elif (defined M3CORE)
#define SOC_ALL_CORETYPE_CNUM_VECTOR  SOC_ALL_M3CORE_CNUM_VECTOR
#elif (defined C71XCORE)
#define SOC_ALL_CORETYPE_CNUM_VECTOR  SOC_ALL_C71XCORE_CNUM_VECTOR
#elif (defined C66XCORE)
#define SOC_ALL_CORETYPE_CNUM_VECTOR  SOC_ALL_C66XCORE_CNUM_VECTOR
#endif

#define SOC_ALL_CORES_CNUM_VECTOR (SOC_ALL_A72CORE_CNUM_VECTOR | SOC_ALL_R5FCORE_CNUM_VECTOR | SOC_ALL_M3CORE_CNUM_VECTOR | SOC_ALL_C71XCORE_CNUM_VECTOR | SOC_ALL_C66XCORE_CNUM_VECTOR)

/* Reserved locks for use as arguments to SOC_lock() and SOC_unlock() */

#define SOC_LOCK_MMR_UNLOCK     (0u)
#define SOC_LOCK_SOC_BARRIER    (0x5u)
#define SOC_LOCK_INTR_PRG (88) /*used by the introuter programming functions */

typedef enum
{
    SOC_BARRIER_IDLE     = 0,
    SOC_BARRIER_CPU_SYNC = 1,
    SOC_BARRIER_CPU_ACK  = 2,
} SOC_BarrierState_t;


/* NOTE:  Normally, function definitions must be defined in a .c to prevent */
/*        "multiple symbol definition" errors.  Static inline functions are  */
/*        an exception to this rule and are allowed to be defined in headers. */

/*! \brief Read 8-bits from target address
    \param addr Target address

    \warning No address translation provided!
*/
static INLINE ALWAYS_INLINE OPTIMIZE(2) uint8_t SOC_read8( uintptr_t addr )
{
    return *(volatile uint8_t *)(addr);
}

/*! \brief Read 16-bits from target address
    \param addr Target address

    \warning No address translation provided!
*/
static INLINE ALWAYS_INLINE OPTIMIZE(2) uint16_t SOC_read16( uintptr_t addr )
{
    return *(volatile uint16_t *)(addr);
}

/*! \brief Read 32-bits from target address
    \param addr Target address

    \warning No address translation provided!
*/
static INLINE ALWAYS_INLINE OPTIMIZE(2) uint32_t SOC_read32( uintptr_t addr )
{
    return *(volatile uint32_t *)(addr);
}

/*! \brief Read 64-bits from target address
    \param addr Target address

    \warning No address translation provided!
*/
static INLINE ALWAYS_INLINE OPTIMIZE(2) uint64_t SOC_read64( uintptr_t addr )
{
    return *(volatile uint64_t *)(addr);
}

/*! \brief Write 8-bits to target address
    \param addr Target address
    \param data Data to write

    \warning No address translation provided!
*/
static INLINE ALWAYS_INLINE OPTIMIZE(2) void SOC_write8( uintptr_t addr, uint8_t data )
{
    *(volatile uint8_t *)addr = data;
}

/*! \brief Write 16-bits to target address
    \param addr Target address
    \param data Data to write

    \warning No address translation provided!
*/
static INLINE ALWAYS_INLINE OPTIMIZE(2) void SOC_write16( uintptr_t addr, uint16_t data )
{
    *(volatile uint16_t *)addr = data;
}

/*! \brief Write 32-bits to target address
    \param addr Target address
    \param data Data to write

    \warning No address translation provided!
*/
static INLINE ALWAYS_INLINE OPTIMIZE(2) void SOC_write32( uintptr_t addr, uint32_t data )
{
    *(volatile uint32_t *)addr = data;
}

/*! \brief Write 64-bits to target address
    \param addr Target address
    \param data Data to write

    \warning No address translation provided!
*/
static INLINE ALWAYS_INLINE OPTIMIZE(2) void SOC_write64( uintptr_t addr, uint64_t data )
{
    *(volatile uint64_t *)addr = data;
}


/*! \brief Implementation of memcpy which preserves the volatility of arguments
 *  \returns pointer to destination memory buffer
 *
 *  \b Notes:
 *    - Does not check for null terminator, will copy every byte as specified by size.
 *    - Checks for alignment of src and dst to improve cpu performance.
 *    - Does not perform RAT address translation.
 *    - Preserves volatile attribute on pointers
 *
 *  \b Example:
 *  \code
 *  volatile uint32_t * msgmgr_queue_ptr = (volatile uint32_t *)msgmgr_queue_base;
 *  uint32_t my_data [] = {...};
 *  SOC_memcpy(msgmgr_queue_ptr, my_data, sizeof(my_data));
 *  \endcode
 */
volatile void *SOC_memcpy(
    volatile void       *dst, /*< [out] pointer to destination buffer */
    const volatile void *src, /*< [in]  pointer to source buffer */
    uint32_t            size  /*< [in]  number of bytes to copy */
    );

/*!
 *  \brief SOC implementation of memcmp
 *
 *  - Same interface as C std memcmp.
 *  - Logs the first error with SOC_assert.
 *  - Exits after the first error.
 */
int32_t SOC_memcmp(
    const void  *gold,  /*< [in] Pointer to the golden data. */
    const void  *check, /*< [in] Pointer to the checked data. */
    uint32_t    num     /*< [in] number of bytes to compare. */
    );

/*!
 *  \brief Enhanced version of SOC_memcmp performing comparisons on
 *  limited number of elements spread-out across the region
 *
 *  - Same interface as C std memcmp.
 *  - Logs the first error with SOC_assert.
 *  - Exits after the first error.
 */
int32_t SOC_sparsememcmp (
    const void *gold,   /*< [in] Pointer to the golden data. */
    const void *check,  /*< [in] Pointer to the checked data. */
    uint32_t   num,     /*< [in] number of bytes in array. */
    uint32_t   elements /*< [in] number of elements to compare. */
    );

/*! \brief SoC implementation of rand
 *
 * Implements a LCG algorithm as light-weight pseudo-random number generator
 * X(n+1) = (aXn + c) mod m
 * Implementation similar to C99, C11 implementation
*/
uint32_t SOC_rand32( void );

/*! \brief SoC implementation of locking mechanism across cores
 *  \param Lock number
 *  \return 0 for success, -1 for error (invalid lock number)
 *
 * Implements a multi-core, hardware-based locking mechanism.
 * Uses the SpinLock module to manage shared hardware resource 
 * or critical section of code.
*/
int32_t SOC_lock( uint32_t lockNum );

/*! \brief Unlock a prevously held lock
 *  \param Lock number
 *  \return 0 for success, -1 for error (invalid lock number)
*/
int32_t SOC_unlock( uint32_t lockNum );

/*! \brief Synchronize all CPUs via barrier
 *  \param[in] cnumVector Vector representing CPUs to synchronize, indexed by CNUM
 *
 *  Example cnumVectors:
 *      SOC_ALL_A72CORE_CNUM_VECTOR  -- All A72 cores
 *      SOC_ALL_R5FCORE_CNUM_VECTOR  -- All non-lockstep R5F cores
 *      SOC_ALL_M3CORE_CNUM_VECTOR   -- All M3 cores
 *      SOC_ALL_CORES_CNUM_VECTOR    -- All A53, non-lockstep R5F, and M3 cores
*/
void SOC_barrier( uint32_t cnumVector );

/*! \brief Implements a architecture-specific memory barrier
 *
 * Ensure completion of previous memory access instructions before proceeding in code execution
*/
void SOC_memory_barrier( void );

/*! \brief SOC implementation of memset which doesn't assume the address is cachable
 *
 */

volatile void *SOC_memset( 
    volatile void *dst, /*< [out] pointer to destination buffer */
    uint32_t ch,        /*< [in]  constant value to set */
    uint32_t size       /*< [in]  number of bytes to copy */
    );

void SOC_sync_req();
void SOC_sync_ack();
void SOC_chan_send(uint32_t data);
uint32_t SOC_chan_read();

void SOC_chan_send64(uint64_t data);
uint64_t SOC_chan_read64();

void SOC_chan_send_ptr(volatile void* ptr);
volatile void* SOC_chan_read_ptr();

#endif  /* SOC_FUNCTIONS_H_ */
