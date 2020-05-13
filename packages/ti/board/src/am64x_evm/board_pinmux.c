/******************************************************************************
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
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
 *****************************************************************************/

/** 
 *  \file board_pinmux.c
 *
 *  \brief This file Enables pinmux for the board 
 *
 */

#include <ti/csl/soc.h>				
#include <ti/csl/soc/am64x/src/cslr_main_ctrl_mmr.h>
#include <ti/csl/soc/am64x/src/cslr_mcu_ctrl_mmr.h>
#include <ti/csl/soc/am64x/src/cslr_main_pll_mmr.h>
#include <ti/csl/soc/am64x/src/cslr_mcu_pll_mmr.h>
#include "board_internal.h"
#include "board_pinmux.h"

#define AVV_PASS   (1U)
#define AVV_FAIL   (0U)

static const uint32_t main_ctrl_mmr_kick_offsets[]= {  CSL_MAIN_CTRL_MMR_CFG0_LOCK0_KICK0,
                                                CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK0,
                                                CSL_MAIN_CTRL_MMR_CFG0_LOCK2_KICK0,
                                                CSL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK0,
                                                CSL_MAIN_CTRL_MMR_CFG0_LOCK4_KICK0,
                                                CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK0,
                                                //CSL_MAIN_CTRL_MMR_CFG0_LOCK6_KICK0,
                                                //CSL_MAIN_CTRL_MMR_CFG0_LOCK7_KICK0,
                                             };
static const uint32_t main_ctrl_mmr_kick_num = sizeof(main_ctrl_mmr_kick_offsets)/sizeof(uint32_t);

static const uint32_t mcu_ctrl_mmr_kick_offsets[]= {   CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK0,
                                                CSL_MCU_CTRL_MMR_CFG0_LOCK1_KICK0,
                                                CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK0,
                                                CSL_MCU_CTRL_MMR_CFG0_LOCK3_KICK0,
                                                CSL_MCU_CTRL_MMR_CFG0_LOCK4_KICK0,
                                                //CSL_MCU_CTRL_MMR_CFG0_LOCK5_KICK0,
                                                CSL_MCU_CTRL_MMR_CFG0_LOCK6_KICK0,
                                                //CSL_MCU_CTRL_MMR_CFG0_LOCK7_KICK0,
                                             };
static const uint32_t mcu_ctrl_mmr_kick_num = sizeof(mcu_ctrl_mmr_kick_offsets)/sizeof(uint32_t);



static const uint32_t main_pll_mmr_kick_offsets[]= {  CSL_MAIN_PLL_MMR_CFG_PLL0_LOCKKEY0,
                                               CSL_MAIN_PLL_MMR_CFG_PLL1_LOCKKEY0,
                                               CSL_MAIN_PLL_MMR_CFG_PLL2_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL3_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL4_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL5_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL6_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL7_LOCKKEY0,
                                               CSL_MAIN_PLL_MMR_CFG_PLL8_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL9_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL10_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL11_LOCKKEY0,
                                               CSL_MAIN_PLL_MMR_CFG_PLL12_LOCKKEY0,
                                             };
static const uint32_t main_pll_mmr_kick_num = sizeof(main_pll_mmr_kick_offsets)/sizeof(uint32_t);

static const uint32_t mcu_pll_mmr_kick_offsets[]= {  CSL_MCU_PLL_MMR_CFG_PLL0_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL1_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL2_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL3_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL4_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL5_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL6_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL7_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL8_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL9_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL10_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL11_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL12_LOCKKEY0,
                                            };
static const uint32_t mcu_pll_mmr_kick_num = sizeof(mcu_pll_mmr_kick_offsets)/sizeof(uint32_t);


#define TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG_LOCK0_KICK0_OFFSET (0x1008)
#define TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG_LOCK1_KICK0_OFFSET (0x5008)

#define TEMP_REPLACE_WITH_CSL_MCU_PADCONFIG_LOCK0_KICK0_OFFSET (0x1008)
#define TEMP_REPLACE_WITH_CSL_MCU_PADCONFIG_LOCK1_KICK0_OFFSET (0x5008)

static const uint32_t main_padcfg_mmr_kick_offsets[]= {  TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG_LOCK0_KICK0_OFFSET,
                                                  TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG_LOCK1_KICK0_OFFSET,
                                               };
static const uint32_t main_padcfg_mmr_kick_num = sizeof(main_padcfg_mmr_kick_offsets)/sizeof(uint32_t);

static const uint32_t mcu_padcfg_mmr_kick_offsets[]= {   TEMP_REPLACE_WITH_CSL_MCU_PADCONFIG_LOCK0_KICK0_OFFSET,
                                                  TEMP_REPLACE_WITH_CSL_MCU_PADCONFIG_LOCK1_KICK0_OFFSET,
                                              };
static const uint32_t mcu_padcfg_mmr_kick_num = sizeof(mcu_padcfg_mmr_kick_offsets)/sizeof(uint32_t);



typedef enum {
    MMR_UNLOCK= 1,
    MMR_LOCK=0
} mmr_lock_actions_t;

/* define the unlock and lock values */
#define KICK_LOCK_VAL    0x00000000

#define MAIN_MMR_BASE_ADDRESS   CSL_CTRL_MMR0_CFG0_BASE
#define MCU_MMR_BASE_ADDRESS    CSL_MCU_CTRL_MMR0_CFG0_BASE

#define MAIN_PLL_MMR_BASE_ADDRESS   CSL_PLL0_CFG_BASE
#define MCU_PLL_MMR_BASE_ADDRESS    CSL_MCU_PLL0_CFG_BASE

#define MAIN_PADCONFIG_MMR_BASE_ADDRESS CSL_PADCFG_CTRL0_CFG0_BASE
#define MCU_PADCONFIG_MMR_BASE_ADDRESS CSL_MCU_PADCFG_CTRL0_CFG0_BASE

uint32_t MMR_change_lock(mmr_lock_actions_t target_state, uint32_t * kick0);
uint32_t generic_mmr_change_all_locks(mmr_lock_actions_t target_state, uint32_t base_addr, const  uint32_t * offset_array, uint32_t array_size);

uint32_t MAIN_PADCONFIG_MMR_unlock_all();
uint32_t MAIN_PADCONFIG_MMR_lock_all();
uint32_t MAIN_PADCONFIG_MMR_change_all_locks(mmr_lock_actions_t target_state);

uint32_t MCU_PADCONFIG_MMR_unlock_all();
uint32_t MCU_PADCONFIG_MMR_lock_all();
uint32_t MCU_PADCONFIG_MMR_change_all_locks(mmr_lock_actions_t target_state);

uint32_t MAIN_CTRL_MMR_unlock_all();
uint32_t MAIN_CTRL_MMR_lock_all();
uint32_t MAIN_CTRL_MMR_change_all_locks(mmr_lock_actions_t target_state);

uint32_t MCU_CTRL_MMR_unlock_all();
uint32_t MCU_CTRL_MMR_lock_all();
uint32_t MCU_CTRL_MMR_change_all_locks(mmr_lock_actions_t target_state);

uint32_t MAIN_PLL_MMR_unlock_all();
uint32_t MAIN_PLL_MMR_lock_all();
uint32_t MAIN_PLL_MMR_change_all_locks(mmr_lock_actions_t target_state);

uint32_t MCU_PLL_MMR_unlock_all();
uint32_t MCU_PLL_MMR_lock_all();
uint32_t MCU_PLL_MMR_change_all_locks(mmr_lock_actions_t target_state);


    uint32_t MMR_change_lock(mmr_lock_actions_t target_state, uint32_t * kick0) {
        uint32_t * kick1 = kick0 + 1;
        uint32_t lock_state = (*kick0 & 0x1); //status is 1 if unlocked, 0 if locked

        //If lock state is not what we want, change it
        if (lock_state != (uint32_t) target_state ) {
            switch(target_state) {
                case MMR_LOCK:
                    // lock the partition by writing the lock values to the kick lock registers
                    *kick0 = KICK_LOCK_VAL;
                    *kick1 = KICK_LOCK_VAL;
                    break;
                case MMR_UNLOCK:
                    // unlock the partition by writing the unlock values to the kick lock registers
                    *kick0 = KICK0_UNLOCK_VAL;
                    *kick1 = KICK1_UNLOCK_VAL;
                    break;
            }

            lock_state = (*kick0 & 0x1);
            //Error out if the change did not take effect
            if(lock_state!= (uint32_t) target_state ){
                //Could insert debug statement here
                //printf("SAVV_DEBUG: Error in changing MMR lock state at address %llx", kick0 );
                return AVV_FAIL;
            }
        }
        //Return pass if lock is already what we want or if changing lock succeeds
        return AVV_PASS;
    }
    uint32_t generic_mmr_change_all_locks(mmr_lock_actions_t target_state, uint32_t base_addr, const uint32_t * offset_array, uint32_t array_size) {
        uint32_t errors=0;
        uint32_t i=0;
        uint32_t * kick0_ptr;
        for(i=0;i<array_size;i++) {
            kick0_ptr = (uint32_t *) (base_addr + offset_array[i]);
            if(MMR_change_lock(target_state, kick0_ptr) == AVV_FAIL){
                errors++;
            }
        }
        return errors;
    }
    //MAIN_CTRL_MMR
    uint32_t MAIN_CTRL_MMR_unlock_all() {
        return MAIN_CTRL_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MAIN_CTRL_MMR_lock_all() {
        return MAIN_CTRL_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MAIN_CTRL_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MAIN_MMR_BASE_ADDRESS, main_ctrl_mmr_kick_offsets, main_ctrl_mmr_kick_num);
        if(errors==0) { return AVV_PASS; }
        else          { return AVV_FAIL; }
    }
    //MCU_CTRL_MMR
    uint32_t MCU_CTRL_MMR_unlock_all() {
        return MCU_CTRL_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MCU_CTRL_MMR_lock_all() {
        return MCU_CTRL_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MCU_CTRL_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MCU_MMR_BASE_ADDRESS, mcu_ctrl_mmr_kick_offsets, mcu_ctrl_mmr_kick_num);
        if(errors==0) { return AVV_PASS; }
        else          { return AVV_FAIL; }
    }
    //MAIN_PLL_MMR
    uint32_t MAIN_PLL_MMR_unlock_all() {
        return MAIN_PLL_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MAIN_PLL_MMR_lock_all() {
        return MAIN_PLL_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MAIN_PLL_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MAIN_PLL_MMR_BASE_ADDRESS, main_pll_mmr_kick_offsets, main_pll_mmr_kick_num);
        if(errors==0) { return AVV_PASS; }
        else          { return AVV_FAIL; }
    }
    //MCU_PLL_MMR
    uint32_t MCU_PLL_MMR_unlock_all() {
        return MCU_PLL_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MCU_PLL_MMR_lock_all() {
        return MCU_PLL_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MCU_PLL_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MCU_PLL_MMR_BASE_ADDRESS, mcu_pll_mmr_kick_offsets, mcu_pll_mmr_kick_num);
        if(errors==0) { return AVV_PASS; }
        else          { return AVV_FAIL; }
    }
    //MAIN_PADCONFIG_MMR
    uint32_t MAIN_PADCONFIG_MMR_unlock_all() {
        return MAIN_PADCONFIG_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MAIN_PADCONFIG_MMR_lock_all() {
        return MAIN_PADCONFIG_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MAIN_PADCONFIG_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MAIN_PADCONFIG_MMR_BASE_ADDRESS, main_padcfg_mmr_kick_offsets, main_padcfg_mmr_kick_num);
        if(errors==0) { return AVV_PASS; }
        else          { return AVV_FAIL; }
    }
    //MCU_PADCONFIG_MMR
    uint32_t MCU_PADCONFIG_MMR_unlock_all() {
        return MCU_PADCONFIG_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MCU_PADCONFIG_MMR_lock_all() {
        return MCU_PADCONFIG_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MCU_PADCONFIG_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MCU_PADCONFIG_MMR_BASE_ADDRESS, mcu_padcfg_mmr_kick_offsets, mcu_padcfg_mmr_kick_num);
        if(errors==0) { return AVV_PASS; }
        else          { return AVV_FAIL; }
    }



//Region of pinmux padconfigs
//Can't find the CSL defines for the padconfig0 offset, need to replace
//The offsets were found from the AM64X MAIN and MCU padcfg_ctrl pdf documents
#define TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG0_OFFSET    (0x4000)
#define TEMP_REPLACE_WITH_CSL_MCU_PADCONFIG0_OFFSET     (0x4000)
typedef enum {
    PINMUX_REGION_MAIN = CSL_PADCFG_CTRL0_CFG0_BASE + TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG0_OFFSET,
    PINMUX_REGION_MCU = CSL_MCU_PADCFG_CTRL0_CFG0_BASE + TEMP_REPLACE_WITH_CSL_MCU_PADCONFIG0_OFFSET,
} pinmux_regions_t;

typedef enum {
    PINMUX_TX_ENABLE=0,
    PINMUX_TX_DISABLE=1,
    PINMUX_TX_IGNORE,
} tx_options_t;

typedef enum {
    PINMUX_DRIVE_STRENGTH_00 =0,
    PINMUX_DRIVE_STRENGTH_01,
    PINMUX_DRIVE_STRENGTH_10,
    PINMUX_DRIVE_STRENGTH_11,
    PINMUX_DRIVE_STRENGTH_IGNORE,
} drive_strength_val_t;

typedef enum {
    PINMUX_RX_DISABLE=0,
    PINMUX_RX_ENABLE=1,
    PINMUX_RX_IGNORE,
} rx_options_t;

typedef enum {
    PINMUX_PULLTYPE_PULLUP,
    PINMUX_PULLTYPE_PULLDOWN,
    PINMUX_PULLTYPE_DISABLE,
    PINMUX_PULLTYPE_IGNORE,
} pull_up_down_options_t;

typedef enum {
    PINMUX_SCHMITT_TRIG_DISABLE =0,
    PINMUX_SCHMITT_TRIG_ENABLE =1,
    PINMUX_SCHMITT_TRIG_IGNORE,
} schmitt_trig_options_t;

typedef enum {
    PINMUX_MUX_MODE_0 = 0,
    PINMUX_MUX_MODE_1,
    PINMUX_MUX_MODE_2,
    PINMUX_MUX_MODE_3,
    PINMUX_MUX_MODE_4,
    PINMUX_MUX_MODE_5,
    PINMUX_MUX_MODE_6,
    PINMUX_MUX_MODE_7,
    PINMUX_MUX_MODE_8,
    PINMUX_MUX_MODE_9,
    PINMUX_MUX_MODE_10,
    PINMUX_MUX_MODE_11,
    PINMUX_MUX_MODE_12,
    PINMUX_MUX_MODE_13,
    PINMUX_MUX_MODE_14,
    PINMUX_MUX_MODE_15,
    PINMUX_MUX_MODE_IGNORE,
} muxmode_t;
typedef enum {
    PINMUX_DEBOUNCE_PERIOD_000 = 0,
    PINMUX_DEBOUNCE_PERIOD_001,
    PINMUX_DEBOUNCE_PERIOD_010,
    PINMUX_DEBOUNCE_PERIOD_011,
    PINMUX_DEBOUNCE_PERIOD_100,
    PINMUX_DEBOUNCE_PERIOD_101,
    PINMUX_DEBOUNCE_PERIOD_110,
    PINMUX_DEBOUNCE_PERIOD_111,
    PINMUX_DEBOUNCE_PERIOD_IGNORE,
} debounce_period_t;

typedef struct{
    pinmux_regions_t        mmrBaseAddress;
    uint32_t                padOffset;
    muxmode_t               muxMode;
    tx_options_t            txEnable;
    rx_options_t            rxEnable;
    pull_up_down_options_t  pullType;
    drive_strength_val_t    driveStrength;
    schmitt_trig_options_t  schmittTrig;
    debounce_period_t       debouncePeriod;
} pinmux_t;

void set_pinmux(pinmux_t *Array, uint8_t arraysize);
void writeToPadConfigReg(pinmux_t * arrayPtr, uint32_t mmr_reg_value);
uint32_t readFromPadConfigReg(pinmux_t * arrayPtr);

#ifdef BOARD_QT_MANUAL_PINMUX_SETUP
static const pinmux_t OSPI_COMPLETE_PINMUX_array [] = {
    //Pinmux Region,     Offset, Muxmode,           TX,                RX,                Pull Up/Down/None,  Drive Strength,           Schmitt Trigger,        Debounce Period
    {PINMUX_REGION_MAIN, 0x0000, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_DISABLE, PINMUX_PULLTYPE_PULLDOWN, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI0 CLK
    {PINMUX_REGION_MAIN, 0x0004, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_ENABLE,  PINMUX_PULLTYPE_PULLDOWN, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI0 LBCLK
    {PINMUX_REGION_MAIN, 0x0008, PINMUX_MUX_MODE_0, PINMUX_TX_DISABLE, PINMUX_RX_ENABLE,  PINMUX_PULLTYPE_PULLDOWN, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI0 DQS
    {PINMUX_REGION_MAIN, 0x000C, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_ENABLE,  PINMUX_PULLTYPE_IGNORE, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI D0
    {PINMUX_REGION_MAIN, 0x0010, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_ENABLE,  PINMUX_PULLTYPE_IGNORE, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI D1
    {PINMUX_REGION_MAIN, 0x0014, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_ENABLE,  PINMUX_PULLTYPE_IGNORE, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI D2
    {PINMUX_REGION_MAIN, 0x0018, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_ENABLE,  PINMUX_PULLTYPE_IGNORE, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI D3
    {PINMUX_REGION_MAIN, 0x001C, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_ENABLE,  PINMUX_PULLTYPE_IGNORE, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI D4
    {PINMUX_REGION_MAIN, 0x0020, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_ENABLE,  PINMUX_PULLTYPE_IGNORE, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI D5
    {PINMUX_REGION_MAIN, 0x0024, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_ENABLE,  PINMUX_PULLTYPE_IGNORE, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI D6
    {PINMUX_REGION_MAIN, 0x0028, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_ENABLE,  PINMUX_PULLTYPE_IGNORE, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI D7
    {PINMUX_REGION_MAIN, 0x002C, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_DISABLE, PINMUX_PULLTYPE_IGNORE, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI CS0
    {PINMUX_REGION_MAIN, 0x0030, PINMUX_MUX_MODE_0, PINMUX_TX_ENABLE,  PINMUX_RX_DISABLE, PINMUX_PULLTYPE_IGNORE, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI CS1
    {PINMUX_REGION_MAIN, 0x0034, PINMUX_MUX_MODE_2, PINMUX_TX_ENABLE,  PINMUX_RX_DISABLE, PINMUX_PULLTYPE_PULLUP, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI RESET_OUT1
    {PINMUX_REGION_MAIN, 0x0038, PINMUX_MUX_MODE_1, PINMUX_TX_ENABLE,  PINMUX_RX_DISABLE, PINMUX_PULLTYPE_PULLUP, PINMUX_DRIVE_STRENGTH_IGNORE, PINMUX_SCHMITT_TRIG_IGNORE, PINMUX_DEBOUNCE_PERIOD_IGNORE}, //OSPI RESET_OUT0
};
#endif

void set_pinmux(pinmux_t *Array, uint8_t arraysize){
    uint32_t c = 0;
    uint32_t i = 0;
    uint32_t mmr_reg_value = 0;
    uint32_t errors = 0;
    uint16_t field_val = 0;
    //Pointer for loop
    pinmux_t * arrayPtr = NULL;
    arrayPtr = Array;

    //Loop on each pinmux_t struct
    for (i = 0; i < arraysize; i++){

        //Read the PADCONFIG register
        mmr_reg_value = readFromPadConfigReg(arrayPtr);

        //Check if addressed padconfig is locked or not
        if( (mmr_reg_value & (uint32_t)(1<<31)) == (uint32_t) (1<<31)) {
            mmr_reg_value &= ~(1<<31);
            writeToPadConfigReg(arrayPtr, mmr_reg_value);
            for(c = 0; c < 20; c++);
            if(readFromPadConfigReg(arrayPtr)!= mmr_reg_value) {
                //Can print here details
                //Increase error count and go to next mmr
                errors++;
                arrayPtr++;
                continue;
            }

        }

        //Mux mode selection
        if(arrayPtr->muxMode != PINMUX_MUX_MODE_IGNORE){
            mmr_reg_value  &= ~( 0b1111 <<0 );  //Clear out the 4 bits for mux mode
            field_val = ((uint16_t) arrayPtr->muxMode) & 0b1111; //Mask the field
            mmr_reg_value  |= ( field_val << 0 ); //Place the value in the correct position
        }
        //Transmit driver enable
        if(arrayPtr->txEnable != PINMUX_TX_IGNORE){
            mmr_reg_value  &= ~( 0b1 << 21 );  //Clear out the bit for tx
            field_val = ((uint16_t) arrayPtr->txEnable) & 0b1; //Mask the field
            mmr_reg_value  |= ( field_val << 21 ); //Place the value in the correct position
        }
        //Receive enable
        if(arrayPtr->rxEnable != PINMUX_RX_IGNORE){
            mmr_reg_value  &= ~( 0b1 << 18 );  //Clear out the bit for rx
            field_val = ((uint16_t) arrayPtr->rxEnable) & 0b1; //Mask the field
            mmr_reg_value  |= ( field_val << 18 ); //Place the value in the correct position
        }
        //PU-PD Selection
        if(arrayPtr->pullType != PINMUX_PULLTYPE_IGNORE){
            if(arrayPtr->pullType == PINMUX_PULLTYPE_DISABLE){
                mmr_reg_value  |= ( 0b1 << 16 );  //Set the bit to disable pulls
            } else {
                mmr_reg_value  &= ~( 0b1 << 16 ); //Clear the bit to enable pulls
                //With Pulls enabled, place in pull down or up
                mmr_reg_value  &= ~( 0b1 << 17 );
                field_val = ((uint16_t) arrayPtr->pullType) & 0b1; //Mask the field
                mmr_reg_value  |= ( field_val << 17 ); //Place the value in the correct position
            }
        }

        //Drive strength selection
        if(arrayPtr->driveStrength != PINMUX_DRIVE_STRENGTH_IGNORE){
            mmr_reg_value  &= ~( 0b11 << 19 );  //Clear out the 2 bits for mux mode
            field_val = ((uint16_t) arrayPtr->driveStrength) & 0b11; //Mask the field
            mmr_reg_value  |= ( field_val << 19 ); //Place the value in the correct position
        }

        //Schmitt Trigger
        if(arrayPtr->schmittTrig != PINMUX_SCHMITT_TRIG_IGNORE){
            mmr_reg_value  &= ~( 0b1 << 14 );  //Clear out the bit for rx
            field_val = ((uint16_t) arrayPtr->schmittTrig) & 0b1; //Mask the field
            mmr_reg_value  |= ( field_val << 14 ); //Place the value in the correct position
        }
        //Debounce period
        if(arrayPtr->debouncePeriod != PINMUX_DEBOUNCE_PERIOD_IGNORE){
            mmr_reg_value  &= ~( 0b111 << 11 ); //Clear out the bits for debounce period
            field_val = ((uint16_t) arrayPtr->debouncePeriod) & 0b111; //Mask the field
            mmr_reg_value |= (field_val << 11);
        }
        //Writing to Padconfig register
        writeToPadConfigReg(arrayPtr, mmr_reg_value);
        for(c = 0; c < 20; c++);
        //Verify write
        if(readFromPadConfigReg(arrayPtr)!= mmr_reg_value) {
            errors++;
        }

        arrayPtr++;
    }
}

void writeToPadConfigReg(pinmux_t * arrayPtr, uint32_t mmr_reg_value) {
    volatile uint32_t * placeToWrite= (volatile uint32_t *)(uintptr_t)(arrayPtr->mmrBaseAddress + arrayPtr->padOffset);
    *placeToWrite = mmr_reg_value;
    //*(volatile uint32_t *)(uintptr_t)(arrayPtr->mmrBaseAddress + arrayPtr->padOffset) = mmr_reg_value;
}

uint32_t readFromPadConfigReg(pinmux_t * arrayPtr) {
    return *(volatile uint32_t *)(uintptr_t)(arrayPtr->mmrBaseAddress + arrayPtr->padOffset);
}

Board_STATUS Board_pinmuxConfig (void)
{
#ifdef BOARD_QT_MANUAL_PINMUX_SETUP
    MAIN_PADCONFIG_MMR_unlock_all();
    set_pinmux((pinmux_t *)&OSPI_COMPLETE_PINMUX_array, (sizeof(OSPI_COMPLETE_PINMUX_array)/sizeof(pinmux_t)) );
#else
    pinmuxModuleCfg_t* pModuleData = NULL;
    pinmuxPerCfg_t* pInstanceData = NULL;
    int32_t i, j, k;

    MAIN_PADCONFIG_MMR_unlock_all();

    for(i = 0; PINMUX_END != gAM64xxMainPinmuxData[i].moduleId; i++)
    {
        pModuleData = gAM64xxMainPinmuxData[i].modulePinCfg;
        for(j = 0; (PINMUX_END != pModuleData[j].modInstNum); j++)
        {
            if(pModuleData[j].doPinConfig == TRUE)
            {
                pInstanceData = pModuleData[j].instPins;
                for(k = 0; (PINMUX_END != pInstanceData[k].pinOffset); k++)
                {
                    HW_WR_REG32((MAIN_PADCONFIG_MMR_BASE_ADDRESS + pInstanceData[k].pinOffset),
                                (pInstanceData[k].pinSettings));
                }
            }
        }
    }

	for(i = 0; PINMUX_END != gAM64xxWkupPinmuxData[i].moduleId; i++)
    {
        pModuleData = gAM64xxWkupPinmuxData[i].modulePinCfg;
        for(j = 0; (PINMUX_END != pModuleData[j].modInstNum); j++)
        {
            if(pModuleData[j].doPinConfig == TRUE)
            {
                pInstanceData = pModuleData[j].instPins;
                for(k = 0; (PINMUX_END != pInstanceData[k].pinOffset); k++)
                {
                    HW_WR_REG32((MCU_PADCONFIG_MMR_BASE_ADDRESS + pInstanceData[k].pinOffset),
                                 (pInstanceData[k].pinSettings));
                }
            }
        }
    }

#endif

    return BOARD_SOK;
}
