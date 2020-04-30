/*
 *  Copyright (C) 2020 Texas Instruments Incorporated
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
 *  \file sciclient_fw_main.c
 *
 *  \brief Implementation of Sciclient Firewall Unit Test
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#if defined (__aarch64__)
#include <ti/sysbios/family/arm/v8a/Mmu.h>
#endif
#include <ti/sysbios/family/arm/v7r/Cache.h>
#include <ti/osal/TimerP.h>

#include <stdint.h>
#include <string.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/drv/sciclient/examples/common/sciclient_appCommon.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define SEC_SUPV_WRITE_MASK (0x00000001U)
#define SEC_SUPV_READ_MASK (0x00000002U)
#define SEC_SUPV_CACHEABLE_MASK (0x00000004U)
#define SEC_SUPV_DEBUG_MASK (0x00000008U)
#define SEC_USER_WRITE_MASK (0x00000010U)
#define SEC_USER_READ_MASK (0x00000020U)
#define SEC_USER_CACHEABLE_MASK (0x00000040U)
#define SEC_USER_DEBUG_MASK (0x00000080U)
#define NONSEC_SUPV_WRITE_MASK (0x00000100U)
#define NONSEC_SUPV_READ_MASK (0x00000200U)
#define NONSEC_SUPV_CACHEABLE_MASK (0x00000400U)
#define NONSEC_SUPV_DEBUG_MASK (0x00000800U)
#define NONSEC_USER_WRITE_MASK (0x00001000U)
#define NONSEC_USER_READ_MASK (0x00002000U)
#define NONSEC_USER_CACHEABLE_MASK (0x00004000U)
#define NONSEC_USER_DEBUG_MASK (0x00008000U)

#if defined (SOC_J721E)
#define MCU_1_0_PRIVID (96)
#define MCU_SRAM_FWL_ID (1050)
#define MSMC_SRAM_FWL_ID (4760)
#define DRAM_FWL_ID (1280)
#endif

#if defined (SOC_AM65XX)
#define MCU_1_0_PRIVID (96)
#define MCU_SRAM_FWL_ID (1050)
#define MSMC_SRAM_FWL_ID (4449)
#define DRAM_FWL_ID (1280)
#endif

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
void mainTask(UArg arg0, UArg arg1);

int32_t Sciclient_fw_test(
        uint16_t fwl_id,
        uint32_t pass_start_address,
        uint32_t pass_end_address,
        uint32_t fail_start_address,
        uint32_t fail_end_address,
        uint32_t hostId,
        uint32_t privId, uint32_t passTest, uint32_t failTest);

int32_t Sciclient_firewallBackground();

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static volatile int32_t gTestStatus;
static uint8_t  gAppTskStackMain[16*1024] __attribute__((aligned(8192)));;
/* IMPORTANT NOTE: For C7x,
 * - stack size and stack ptr MUST be 8KB aligned
 * - AND min stack size MUST be 16KB
 * - AND stack assigned for task context is "size - 8KB"
 *       - 8KB chunk for the stack area is used for interrupt handling in this task context
 */

uint32_t gAbortRecieved = 0U;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    Task_Handle task;
    Task_Params taskParams;
    Error_Block eb;

#if defined (SOC_J721E)
    extern const UInt32 ti_sysbios_family_arm_v7r_keystone3_Hwi_vectors[];
    memcpy((void*)CSL_MCU_ARMSS_ATCM_BASE, 
           (void*) &ti_sysbios_family_arm_v7r_keystone3_Hwi_vectors,
           0x60);
    Cache_wbInvAll();
#endif

    uint32_t retVal = CSL_PASS;

    Task_Params_init(&taskParams);
    taskParams.priority = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stackSize    = sizeof (gAppTskStackMain);

    Error_init(&eb);
    
    task = Task_create(mainTask, &taskParams, &eb);
    if(NULL==task)
    {
        BIOS_exit(0);
    }

    BIOS_start();

    return retVal;
}

void mainTask(UArg arg0, UArg arg1)
{
    /*To suppress unused variable warning*/
    (void)arg0;
    (void)arg1;

    int32_t r = CSL_PASS;
    volatile uint32_t loopForever = 1U;

    App_sciclientConsoleInit();
    App_sciclientPrintf( "\n=====================\n");
    App_sciclientPrintf( "\nSciclient FW Test\n");
    App_sciclientPrintf( "\n=====================\n");
  
    r = Sciclient_firewallBackground(); 
    /* Firwalling MCU SRAM */
    if (r == CSL_PASS)
    {
        #define MCU_SRAM_ADDRESS_PASS_START (0x41C3E000)
        #define MCU_SRAM_ADDRESS_PASS_END (0x41C3E000 + 4 * 1024 - 1)
        #define MCU_SRAM_ADDRESS_FAIL_START (0x41C3E000 + 8 * 1024 - 1)
        #define MCU_SRAM_ADDRESS_FAIL_END (0x41C3E000 + 12 * 1024 - 1)
        r = Sciclient_fw_test(
                MCU_SRAM_FWL_ID,
                MCU_SRAM_ADDRESS_PASS_START,
                MCU_SRAM_ADDRESS_PASS_END,
                MCU_SRAM_ADDRESS_FAIL_START,
                MCU_SRAM_ADDRESS_FAIL_END,
                TISCI_HOST_ID_R5_0,
                MCU_1_0_PRIVID, TRUE, TRUE);
        if (CSL_PASS == r)
        {
            App_sciclientPrintf(
                              "\nMCU SRAM Tests Passed.\n");
        }
        else
        {
            App_sciclientPrintf(
                              "\nMCU SRAM Tests have FAILED.\n");
        }
    }
    
    /* Firewalling MSMC RAM */
    if (r == CSL_PASS)
    {
        #define MSMC_RAM_ADDRESS_PASS_START (0x70100000)
        #define MSMC_RAM_ADDRESS_PASS_END (0x70100000 + 4 * 1024 - 1)
        #define MSMC_RAM_ADDRESS_FAIL_START (0x70100000 + 8 * 1024)
        #define MSMC_RAM_ADDRESS_FAIL_END (0x70100000 + 12 * 1024 - 1)
        r = Sciclient_fw_test(
                MSMC_SRAM_FWL_ID,
                MSMC_RAM_ADDRESS_PASS_START,
                MSMC_RAM_ADDRESS_PASS_END,
                MSMC_RAM_ADDRESS_FAIL_START,
                MSMC_RAM_ADDRESS_FAIL_END,
                TISCI_HOST_ID_R5_0,
                MCU_1_0_PRIVID,
                FALSE, TRUE);
        if (CSL_PASS == r)
        {
            App_sciclientPrintf(
                              "\nMSMC SRAM Tests Passed.\n");
        }
        else
        {
            App_sciclientPrintf(
                              "\nMSMC SRAM Tests have FAILED.\n");
        }
    }

    /* Firewalling DDR */
    if (CSL_PASS == r) {
        /*
         * From MCU domain or UDMA coming from MCU domain – 
         * There are two options. 
         * a.Interdomain firewall from MCU to DDR can be 
         * configured to filter accesses. – J721E ID DRAM_FWL_ID |  AM65xx DRAM_FWL_ID
         * b.Northbridge firewall between NAVSS and Compute cluster can 
         * be configured. – J721E ID 4762, 4763 | AM65xx 4450
         * The below example shows option a.
         */
        #define DRAM_ADDRESS_PASS_START (0x81000000)
        #define DRAM_ADDRESS_PASS_END (0x81000000 + 4 * 1024 - 1)
        #define DRAM_ADDRESS_FAIL_START (0x81000000 + 4 * 1024)
        #define DRAM_ADDRESS_FAIL_END (0x81000000 + 8 * 1024 - 1)
        /* Tests are not run to avoid overwriting DDR data sections */
        r = Sciclient_fw_test(
                DRAM_FWL_ID,
                DRAM_ADDRESS_PASS_START,
                DRAM_ADDRESS_PASS_END,
                DRAM_ADDRESS_FAIL_START,
                DRAM_ADDRESS_FAIL_END,
                TISCI_HOST_ID_R5_0,
                MCU_1_0_PRIVID,
                TRUE, TRUE);
        if (CSL_PASS == r)
        {
            App_sciclientPrintf("\nDRAM Tests Passed.\n");
        }
        else
        {
            App_sciclientPrintf(
                              "\nDRAM Tests have FAILED.\n");
        }
    }
    
    
    if (CSL_PASS == r)
    {
        App_sciclientPrintf(
                          "\nAll tests have PASSED.\n");
    }
    else
    {
        App_sciclientPrintf(
                          "\nSome of the Test-cases have FAILED.\n");
    }

    while(loopForever);
}

/* ========================================================================== */
/*                 Internal Function Definitions                              */
/* ========================================================================== */

int32_t Sciclient_firewallBackground()
{
    int32_t ret = CSL_PASS;
    uint32_t fwl_id;
    const uint32_t perm_for_access =
            SEC_SUPV_WRITE_MASK | SEC_SUPV_READ_MASK | 
            SEC_SUPV_CACHEABLE_MASK | SEC_SUPV_DEBUG_MASK |
            SEC_USER_WRITE_MASK | SEC_USER_READ_MASK | 
            SEC_USER_CACHEABLE_MASK | SEC_USER_DEBUG_MASK |
            NONSEC_SUPV_WRITE_MASK | NONSEC_SUPV_READ_MASK | 
            NONSEC_SUPV_CACHEABLE_MASK | NONSEC_SUPV_DEBUG_MASK |
            NONSEC_USER_WRITE_MASK | NONSEC_USER_READ_MASK | 
            NONSEC_USER_CACHEABLE_MASK | NONSEC_USER_DEBUG_MASK;
    uint32_t timeout = 0xFFFFFFFFU;
    struct tisci_msg_fwl_change_owner_info_req req = {
        .fwl_id = (uint16_t)0,
        .region = (uint16_t) 0,
        .owner_index = (uint8_t) TISCI_HOST_ID_R5_0
        };
    struct tisci_msg_fwl_change_owner_info_resp resp = {0};
    struct tisci_msg_fwl_set_firewall_region_resp resp_fw_set_pass = {0};
    struct tisci_msg_fwl_set_firewall_region_req req_fw_set_pass = { 
        .fwl_id = (uint16_t)0,
        .region = (uint16_t) 0,
        .n_permission_regs = (uint32_t) 3,
        .control = (uint32_t) 0xA,
        .permissions[0] = (uint32_t) 0,
        .permissions[1] = (uint32_t) 0,
        .permissions[2] = (uint32_t) 0,
        .start_address = 0,
        .end_address = 0
        };
    fwl_id = MCU_SRAM_FWL_ID;
    /* Set this background region for access from the CPU core
     */
    req_fw_set_pass.fwl_id = fwl_id;
    req_fw_set_pass.start_address = 0x41C00000;
    req_fw_set_pass.end_address = 0x41c00000 + 512 *1024 - 1;
    req_fw_set_pass.region = 0;
    req_fw_set_pass.n_permission_regs = 3;
    req_fw_set_pass.permissions[0] = 195 << 16 | perm_for_access;
    req_fw_set_pass.permissions[1] = 0;
    req_fw_set_pass.permissions[2] = 0;
    req_fw_set_pass.control = 0x10A;
    req.fwl_id = fwl_id;
    req.region = 0;
    ret = Sciclient_firewallChangeOwnerInfo(&req, &resp, timeout);
    if (ret == CSL_PASS)
    {
        /* Set background region */
        ret = Sciclient_firewallSetRegion(&req_fw_set_pass, &resp_fw_set_pass, timeout);
    }
    if (ret == CSL_PASS)
    {
        fwl_id = MSMC_SRAM_FWL_ID;
        /* DMSC sets the default region accessible. Only change ownership for MSMC
         */
        req.fwl_id = fwl_id;
        req.region = 0;
        ret = Sciclient_firewallChangeOwnerInfo(&req, &resp, timeout);
    }
    if (ret == CSL_PASS)
    {
        fwl_id = DRAM_FWL_ID;
        /* DMSC sets the default region accessible. Only change ownership for MSMC
         */
        req.fwl_id = fwl_id;
        req.region = 0;
        req_fw_set_pass.fwl_id = fwl_id;
        req_fw_set_pass.start_address = 0x70000000;
        req_fw_set_pass.end_address = 0xFFFFFFFF;
        req_fw_set_pass.region = 0;
        req_fw_set_pass.n_permission_regs = 3;
        req_fw_set_pass.permissions[0] = 195 << 16 | perm_for_access;
        req_fw_set_pass.permissions[1] = 0;
        req_fw_set_pass.permissions[2] = 0;
        req_fw_set_pass.control = 0x10A;
 
        ret = Sciclient_firewallChangeOwnerInfo(&req, &resp, timeout);
        if (ret == CSL_PASS)
        {
            /* Set background region */
            ret = Sciclient_firewallSetRegion(&req_fw_set_pass, &resp_fw_set_pass, timeout);
        }
    }
    return ret;
}

volatile uint32_t *p;
/**
 * \brief This function will first set up the firewalls to access a region of 
 *        memory and not have any access to another region of memory. Once set
 *        the CPU would try to write a known pattern to the region of memory
 *        and check it is able to write and read back to the region where it
 *        has access and cannot write and readback from the region it does not
 *        have access.
 *
 * \param fwl_id             ID of the firewall being tested.
 * \param pass_start_address Start Address of the pass region
 * \param pass_end_address   End Address of the pass region
 * \param fail_start_address Start Address of the fail region
 * \param fail_end_address   End Address of the fail region
 * \param hostId             Host ID being tested.
 * \param privId             Priv ID being tested.
 * \param passTest           Bool Flag to run the test of writing and
 *                           reading patterns.
 * \param failTest           Bool Flag to run the test of writing and
 *                           reading patterns.
 * 
 * \return CSL_PASS/CSL_EFAIL based on the status of the test run.
 */
int32_t Sciclient_fw_test(
        uint16_t fwl_id,
        uint32_t pass_start_address,
        uint32_t pass_end_address,
        uint32_t fail_start_address,
        uint32_t fail_end_address,
        uint32_t hostId,
        uint32_t privId, uint32_t passTest, uint32_t failTest)
{   
    int32_t r = CSL_PASS;
    const uint32_t perm_for_access =
            SEC_SUPV_WRITE_MASK | SEC_SUPV_READ_MASK | 
            SEC_SUPV_CACHEABLE_MASK | SEC_SUPV_DEBUG_MASK |
            SEC_USER_WRITE_MASK | SEC_USER_READ_MASK | 
            SEC_USER_CACHEABLE_MASK | SEC_USER_DEBUG_MASK |
            NONSEC_SUPV_WRITE_MASK | NONSEC_SUPV_READ_MASK | 
            NONSEC_SUPV_CACHEABLE_MASK | NONSEC_SUPV_DEBUG_MASK |
            NONSEC_USER_WRITE_MASK | NONSEC_USER_READ_MASK | 
            NONSEC_USER_CACHEABLE_MASK | NONSEC_USER_DEBUG_MASK;
    const uint32_t perm_for_no_access = 0;
    uint32_t timeout = 0xFFFFFFFFU;
    struct tisci_msg_fwl_change_owner_info_req req = {
        .fwl_id = (uint16_t)fwl_id,
        .region = (uint16_t) 1,
        .owner_index = (uint8_t) hostId
        };
    struct tisci_msg_fwl_change_owner_info_resp resp = {0};
    struct tisci_msg_fwl_set_firewall_region_req req_fw_set_pass = { 
        .fwl_id = (uint16_t)fwl_id,
        .region = (uint16_t) 1,
        .n_permission_regs = (uint32_t) 1,
        .control = (uint32_t) 0xA,
        .permissions = (uint32_t) (privId << 16) | perm_for_access,
        .start_address = pass_start_address,
        .end_address = pass_end_address
        };
    struct tisci_msg_fwl_set_firewall_region_req req_fw_set_fail = { 
        .fwl_id = (uint16_t)fwl_id,
        .region = (uint16_t) 2,
        .n_permission_regs = (uint32_t) 1,
        .control = (uint32_t) 0xA,
        .permissions = (uint32_t) (privId << 16) | perm_for_no_access,
        .start_address = fail_start_address,
        .end_address = fail_end_address
        };
    struct tisci_msg_fwl_set_firewall_region_resp resp_fw_set = {0};
    
    gAbortRecieved = 0U;
    App_sciclientPrintf( "\nTesting Firewalls:");
    App_sciclientPrintf( "\n1. Changing the Firewall Owner for region 1.");
    r = Sciclient_firewallChangeOwnerInfo(&req, &resp, timeout);
    if (r == CSL_PASS)
    {
        req.region = (uint16_t) 2;
        App_sciclientPrintf( "\n2. Changing the Firewall Owner for region 2.");
        r = Sciclient_firewallChangeOwnerInfo(&req, &resp, timeout);
        if (r != CSL_PASS)
        {
            App_sciclientPrintf( "\nThis Step Failed!!");
        }
    }
    if (r == CSL_PASS)
    {
        App_sciclientPrintf( "\n3. Changing the Firewall Permissions for region 1.");
        r = Sciclient_firewallSetRegion(&req_fw_set_pass, &resp_fw_set, timeout);
        if (r != CSL_PASS)
        {
            App_sciclientPrintf( "\nThis Step Failed!!");
        }
    }
    if (r == CSL_PASS)
    {
        App_sciclientPrintf( "\n4. Changing the Firewall Permissions for region 2.");
        r = Sciclient_firewallSetRegion(&req_fw_set_fail, &resp_fw_set, timeout);
        if (r != CSL_PASS)
        {
            App_sciclientPrintf( "\nThis Step Failed!!");
        }
    }
    if ((r == CSL_PASS) && (passTest == TRUE))
    {
        App_sciclientPrintf( "\n5. Reading content from Region 1 to make sure the address is accesible.");
        /* Access memory region to make sure able to read and write */
        p = (uint32_t*)pass_start_address;
        while (p < (uint32_t*)pass_end_address)
        {
            *p = 0xABCDEF01;
            Cache_wbInvAll();
            volatile uint32_t value = *p;
            if (value != 0xABCDEF01)
            {
                r = CSL_EFAIL;
                break;
            }
            p++;
        }
        if (r != CSL_PASS)
        {
            App_sciclientPrintf( "\nThis Step Failed!!");
        }
    }
    if ((r == CSL_PASS) && (failTest == TRUE))
    {
        App_sciclientPrintf( "\n5. Reading content from Region 2 to make sure the address is not accesible.");
        /* Access memory region to make sure unable to read and write */
        p = (uint32_t*)fail_start_address;
        while (p < (uint32_t*) fail_end_address)
        {
            *p = (uint32_t)p; // This should fail
            Cache_wbInvAll();
            volatile uint32_t value = *p;
            if (value == (uint32_t)p)
            {
                r = CSL_EFAIL;
                break;
            }
            p++;
        }
        if (gAbortRecieved == (fail_end_address + 1 - fail_start_address)/4U)
        {
                r = CSL_PASS;
        }
        else
        {
                r = CSL_EFAIL;
        }
        if (r != CSL_PASS)
        {
            App_sciclientPrintf( "\nThis Step Failed!!");
        }
    }
    return r;
}

void Sciclient_fw_abort_C_handler()
{
    gAbortRecieved++;
}

