/*
    Copyright (C)2006 onward WITTENSTEIN aerospace & simulation limited. All rights reserved.

    This file is part of the SafeRTOS product, see projdefs.h for version number
    information.

    SafeRTOS is distributed exclusively by WITTENSTEIN high integrity systems,
    and is subject to the terms of the License granted to your organization,
    including its warranties and limitations on use and distribution. It cannot be
    copied or reproduced in any way except as permitted by the License.

    Licenses authorize use by processor, compiler, business unit, and product.

    WITTENSTEIN high integrity systems is a trading name of WITTENSTEIN
    aerospace & simulation ltd, Registered Office: Brown's Court, Long Ashton
    Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
    Tel: +44 (0) 1275 395 600, fax: +44 (0) 1275 393 630.
    E-mail: info@HighIntegritySystems.com

    www.HighIntegritySystems.com
*/

/* Generic headers */
#include <stdlib.h>

/* SafeRTOS includes */
#include "SafeRTOS_API.h"

/* Program Includes */
#include "aborts.h"     /* Required for Hook Function Prototypes */

#include <string.h>
#include <ti/osal/TimerP.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/src/nonos/Nonos_config.h>

/*-----------------------------------------------------------------------------
 * Local Constants
 *---------------------------------------------------------------------------*/

/* Scheduler Initialisation Definitions */

/* SafeRTOS inspects the vector table to ensure
 * the necessary handlers have been installed. */
#define configSTACK_CHECK_MARGIN            ( 0U )

/* The user configuration for the idle task. */
#define configIDLE_TASK_STACK_SIZE          ( configMINIMAL_STACK_SIZE_WITH_FPU )

/* The user configuration for the timer module. */
#define configTIMER_TASK_STACK_SIZE         ( 4096U )
//#define configTIMER_TASK_PRIORITY           ( configMAX_PRIORITIES - 1U )
#define configTIMER_CMD_QUEUE_LEN           ( 10 )
#define configTIMER_CMD_QUEUE_BUFFER_SIZE   ( ( configTIMER_CMD_QUEUE_LEN * sizeof( timerQueueMessageType ) ) + portQUEUE_OVERHEAD_BYTES )

/* The size of the Idle task data section. */
#define configIDLE_TASK_DATA_ADDR           ( ( void * ) &lnkIdleTaskDataStartAddr )
#define configIDLE_TASK_DATA_SIZE           ( ( portUInt32Type ) 0x20U )

/* The size of the RAM. */
#define configRAM_ADDR           ( ( portUInt32Type ) &lnkRamStartAddr )
#define configRAM_SIZE           ( ( portUInt32Type ) &lnkRamEndAddr - ( portUInt32Type ) &lnkRamStartAddr )


/*-----------------------------------------------------------------------------
 * Local Prototypes
 *---------------------------------------------------------------------------*/
/* Set DLFO bit is defined in utils_asm.asm. */
void vPortSetDLFOBit( void );

/* The call to xTaskInitializeScheduler is included within a wrapper
 * initialisation function. */
portBaseType xInitializeScheduler( void );


/*-----------------------------------------------------------------------------
 * Local Variables
 *---------------------------------------------------------------------------*/

static portInt8Type acIdleTaskStack[ configIDLE_TASK_STACK_SIZE ] __attribute__( ( aligned ( configIDLE_TASK_STACK_SIZE ) ) ) = { 0 };

/* Declare the stack for the timer task, it cannot be done in the timer
 * module as the syntax for alignment is port specific. Also the callback
 * functions are executed in the timer task and their complexity/stack
 * requirements are application specific. */
static portInt8Type acTimerTaskStack[ configTIMER_TASK_STACK_SIZE ] __attribute__( ( aligned ( configTIMER_TASK_STACK_SIZE ) ) ) = { 0 };

/* The buffer for the timer command queue. */
static portInt8Type acTimerCommandQueueBuffer[ configTIMER_CMD_QUEUE_BUFFER_SIZE ] __attribute__( ( aligned ( portWORD_ALIGNMENT ) ) ) = { 0 };

/* A linker defined symbol that gives the start address of the Idle task
 * data section. */
extern portUInt32Type lnkIdleTaskDataStartAddr;

/* Linker defined symbols that give the start and end address of the RAM. */
extern portUInt32Type lnkRamStartAddr;
extern portUInt32Type lnkRamEndAddr;

/** \brief Full access to privileged and user modes */
#define CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR                  ((uint32_t) 0x3U)
/** \brief  Cache Policy: Non-cacheable */
#define CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE               ((uint32_t) 0x0U)
/** \brief  Cache Policy: Write-back, write-allocate */
#define CSL_ARM_R5_CACHE_POLICY_WB_WA                       ((uint32_t) 0x1U)
/** \brief  This should be passed to configuration. */
#define CSL_ARM_R5_MEM_ATTR_MAX                             ((uint32_t) 0x7U)
#define CSL_ARM_R5_MPU_REGION_AC_XN_SHIFT                   (0x0000000CU)
#define CSL_ARM_R5_MPU_REGION_AC_AP_SHIFT                   (0x00000008U)
#define CSL_ARM_R5_MPU_REGION_AC_S_SHIFT                    (0x00000002U)
#define CSL_ARM_R5_MPU_REGION_AC_TEX_SHIFT                  (0x00000003U)
#define CSL_ARM_R5_MPU_REGION_AC_CB_SHIFT                   (0x00000000U)
#define CSL_ARM_R5_MPU_REGION_AC_B_SHIFT                    (0x00000000U)
#define CSL_ARM_R5_MPU_REGION_AC_C_SHIFT                    (0x00000001U)


/* The structure passed to xMPUConfigureGlobalRegion() to configure the kernel
 * with MPU global region id. */
typedef struct MPU_CONFIG_ACCESS
{
    portUInt32Type          ulexeNeverControl;
    portUInt32Type          ulaccessPermission;
    portUInt32Type          ulshareable;
    portUInt32Type          ulcacheable;
    portUInt32Type          ulcachePolicy;
    portUInt32Type          ulmemAttr;
}xMPU_CONFIG_ACCESS;

/* The structure passed to xMPUConfigureGlobalRegion() to configure the kernel
 * with MPU global region id. */
typedef struct MPU_CONFIG_PARAMETERS
{
    /**< Region number to configure.
     *   Range: 1 to (portmpuGLOBAL_CONFIGURABLE_REGION_LAST) */
    portUInt32Type          ulRegionNumber;
    /**< Region base address: 32 bytes aligned. */
    portUInt32Type          ulRegionBeginAddress;
    xMPU_CONFIG_ACCESS      xRegionAccess;
    portUInt32Type          ulRegionSize;
    portUInt32Type          ulSubRegionDisable;
}xMPU_CONFIG_PARAMETERS;

/**
 * \brief  TEX[2:0], C and B values.
 *         CSL_ArmR5MemAttr is used as intex here.
 *         gMemAttr[x][0]: TEX[2:0] values
 *         gMemAttr[x][1]: C bit value
 *         gMemAttr[x][2]: B bit value
 */
static const uint32_t gMemAttr[CSL_ARM_R5_MEM_ATTR_MAX][3U] =
{
/*    TEX[2:0], C,     B bits */
    {   0x0U,   0x0U,  0x0U,}, /* Strongly-ordered.*/
    {   0x0U,   0x0U,  0x1U,}, /* Shareable Device.*/
    {   0x0U,   0x1U,  0x0U,}, /* Outer and Inner write-through, no write-allocate. */
    {   0x0U,   0x1U,  0x1U,}, /* Outer and Inner write-back, no write-allocate. */
    {   0x1U,   0x0U,  0x0U,}, /* Outer and Inner Non-cacheable. */
    {   0x1U,   0x1U,  0x1U,}, /* Outer and Inner write-back, write-allocate.*/
    {   0x2U,   0x0U,  0x0U,}, /* Non-shareable Device.*/
};
#define CSL_ARM_R5_MPU_REGION_AC_XN_SHIFT                                          (0x0000000CU)
#define CSL_ARM_R5_MPU_REGION_AC_AP_SHIFT                                          (0x00000008U)
#define CSL_ARM_R5_MPU_REGION_AC_S_SHIFT                                           (0x00000002U)
#define CSL_ARM_R5_MPU_REGION_AC_TEX_SHIFT                                         (0x00000003U)
#define CSL_ARM_R5_MPU_REGION_AC_CB_SHIFT                                          (0x00000000U)
#define CSL_ARM_R5_MPU_REGION_AC_B_SHIFT                                           (0x00000000U)
#define CSL_ARM_R5_MPU_REGION_AC_C_SHIFT                                           (0x00000001U)

xMPU_CONFIG_PARAMETERS gMPUConfigParms[] =
{
    {
        /* Region 0 configuration: complete 32 bit address space = 4Gbits add one more 2gb */
        /* ulRegionNumber */
        .ulRegionNumber         = 1U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x0U,
        /* Access permission */
        {
            .ulexeNeverControl  = 1U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 0U,
            .ulcachePolicy      = 0U,
            .ulmemAttr          = 0U,
        },
        /* TODO region size is 4GB, but 2GB is largest supported */
        .ulRegionSize           = portmpuLARGEST_REGION_SIZE_ACTUAL,
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = portmpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 0 configuration: second half of 2 GB */
        /* ulRegionNumber */
        .ulRegionNumber         = 2U,
        /* Starting address */
        .ulRegionBeginAddress   = portmpuLARGEST_REGION_SIZE_ACTUAL,
        /* Access permission */
        {
            .ulexeNeverControl  = 1U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 0U,
            .ulcachePolicy      = 0U,
            .ulmemAttr          = 0U,
        },
        /* TODO region size is 4GB, but 2GB is largest supported */
        .ulRegionSize           = portmpuLARGEST_REGION_SIZE_ACTUAL,
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = portmpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 1 configuration: 128 bytes memory for exception vector execution */
        /* ulRegionNumber */
        .ulRegionNumber         = 3U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x0U,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
            .ulmemAttr          = 0U,
        },
        /* ulRegionSize */
        .ulRegionSize           = 128U,
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = portmpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 2 configuration: 1MB KB MCU MSRAM */
        /* ulRegionNumber */
        .ulRegionNumber         = 4U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x41C00000,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
            .ulmemAttr          = 0U,
        },
        /* Size is 512KB */
        .ulRegionSize           = (512U * 1024U),
#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4)
        /* Size is 1MB */
        .ulRegionSize           = (1024U * 1024U),
#endif
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = portmpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 3 configuration: 2 MB MCMS3 RAM */
        /* ulRegionNumber */
        .ulRegionNumber         = 5U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x70000000,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
            .ulmemAttr          = 0U,
        },
        /* Size is 2MB */
        .ulRegionSize           = (2U * 1024U * 1024U),
#if defined (SOC_J721E)
        /* Size is 8MB */
        .ulRegionSize           = (8U * 1024U * 1024U),
#endif

#if defined (SOC_J7200)
        /* Size is 1MB */
        .ulRegionSize           = (1U * 1024U * 1024U),
#endif

#if defined (SOC_J721S2) || defined (SOC_J784S4)
        /* Size is 4MB */
        .ulRegionSize           = (4U * 1024U * 1024U),
#endif
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = portmpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 4 configuration: 2 GB DDR RAM */
        /* ulRegionNumber */
        .ulRegionNumber         = 6U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x80000000,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
            .ulmemAttr          = 0U,
        },
        /* size is 2GB */
        .ulRegionSize           = portmpuLARGEST_REGION_SIZE_ACTUAL,
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = portmpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 5 configuration: 32 KB BTCM */
        /* Address of ATCM/BTCM are configured via MCU_SEC_MMR registers
           It can either be '0x0' or '0x41010000'. Application/Boot-loader shall
           take care this configurations and linker command file shall be
           in sync with this. For either of the above configurations,
           MPU configurations will not changes as both regions will have same
           set of permissions in almost all scenarios.
           Application can chose to overwrite this MPU configuration if needed.
           The same is true for the region corresponding to ATCM. */
        /* ulRegionNumber */
        .ulRegionNumber         = 7U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x41010000,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE,
            .ulmemAttr          = 0U,
        },
        /* size is 32KB */
        .ulRegionSize           = (32U * 1024U),
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = portmpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 6 configuration: 32 KB ATCM */
        /* ulRegionNumber */
        .ulRegionNumber         = 8U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x0,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE,
            .ulmemAttr          = 0U,
        },
        /* size is 32KB */
        .ulRegionSize           = (32U * 1024U),
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = portmpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
};

/*-----------------------------------------------------------------------------
 * Routines
 *---------------------------------------------------------------------------*/
portUInt32Type xConfigureMPUAccessCtrl(xMPU_CONFIG_ACCESS *xMPUconfigAccess)
{
    portUInt32Type accessCtrlRegVal = 0, tex;

    accessCtrlRegVal |= ( xMPUconfigAccess->ulexeNeverControl <<
                           CSL_ARM_R5_MPU_REGION_AC_XN_SHIFT);
    accessCtrlRegVal |= ( xMPUconfigAccess->ulaccessPermission <<
                           CSL_ARM_R5_MPU_REGION_AC_AP_SHIFT);
    accessCtrlRegVal |= ( xMPUconfigAccess->ulshareable <<
                           CSL_ARM_R5_MPU_REGION_AC_S_SHIFT);
    if (xMPUconfigAccess->ulcacheable == 1U)
    {
        tex = (1U << 2U);
        tex |= (xMPUconfigAccess->ulcachePolicy);
        accessCtrlRegVal |=
                        ( tex << CSL_ARM_R5_MPU_REGION_AC_TEX_SHIFT);
        accessCtrlRegVal |= ( xMPUconfigAccess->ulcachePolicy <<
                           CSL_ARM_R5_MPU_REGION_AC_CB_SHIFT);
    }
    else
    {
        tex = gMemAttr[xMPUconfigAccess->ulmemAttr][0U];
        accessCtrlRegVal |=
                        ( tex << CSL_ARM_R5_MPU_REGION_AC_TEX_SHIFT);
        accessCtrlRegVal |=
                        ( gMemAttr[xMPUconfigAccess->ulmemAttr][1U] <<
                        CSL_ARM_R5_MPU_REGION_AC_B_SHIFT);
        accessCtrlRegVal |=
                        ( gMemAttr[xMPUconfigAccess->ulmemAttr][2U] <<
                        CSL_ARM_R5_MPU_REGION_AC_C_SHIFT);
    }
    return accessCtrlRegVal;
}

portBaseType xConfigureMPU()
{
    portBaseType xInitSchedResult;
    uint32_t i;
    xMPU_CONFIG_PARAMETERS *xMPUconfig;
    xMPU_CONFIG_ACCESS     *xMPUconfigAccess;
    portUInt32Type         ulRegionAccess;

    for(i = 0; i < (sizeof(gMPUConfigParms) / sizeof(gMPUConfigParms[0])); i++)
    {
        ulRegionAccess = 0;
        xMPUconfig = &gMPUConfigParms[i];

        /* Update access control */
        xMPUconfigAccess = &xMPUconfig->xRegionAccess;

        ulRegionAccess = xConfigureMPUAccessCtrl(xMPUconfigAccess);

        xInitSchedResult = xMPUConfigureGlobalRegion(
                xMPUconfig->ulRegionNumber,
                xMPUconfig->ulRegionBeginAddress,
                ulRegionAccess,
                xMPUconfig->ulRegionSize,
                xMPUconfig->ulSubRegionDisable
                );
        if(pdPASS != xInitSchedResult)
        {
            break;
        }
    }

    return xInitSchedResult;
}

void vConfigTimeStampInitialisation( void )
{
}
/*---------------------------------------------------------------------------*/

/* return current counter value of high speed counter in units of 10's of usecs */
portUInt32Type ulPortGetRunTimeCounterValue( void )
{
    portUInt64Type ullTimeInUsecs = TimerP_getTimeInUsecs();

    /* note, there is no overflow protection for this 32b value in SafeRTOS
     *
     * Dividing by 10 to reduce the resolution and avoid overflow for longer duration
     */
    return ( portUInt32Type )( ullTimeInUsecs/10 );
}
/*---------------------------------------------------------------------------*/

void vDemoApplicationErrorHook( portTaskHandleType xHandleOfTaskWithError,
                                const portCharType *pcErrorString,
                                portBaseType xErrorCode )
{
    /* The parameters are not used, these lines prevent compiler warnings. */
    ( void ) xHandleOfTaskWithError;
    ( void ) pcErrorString;
    ( void ) xErrorCode;

    /* Will only get here if an internal kernel error occurs. */
    vApplicationAbort();
}

#if 0
extern void vPortMPUREADRegion(portUInt32Type ulRegionNumber);
#endif

portBaseType xInitializeScheduler( void )
{
    portBaseType xInitSchedResult;

    memset( acIdleTaskStack, 0xFF, sizeof( acIdleTaskStack ) );
    memset( acTimerTaskStack, 0xFF, sizeof( acTimerTaskStack ) );
    memset( acTimerCommandQueueBuffer, 0xFF, sizeof( acTimerCommandQueueBuffer ) );

    /* The structure passed to xTaskInitializeScheduler() to configure the kernel
     * with the application defined constants and call back functions. */
    xPORT_INIT_PARAMETERS xPortInit =
    {
        configCPU_CLOCK_HZ,                 /* ulCPUClockHz */
        configTICK_RATE_HZ,                 /* ulTickRateHz */

        /* Hook Functions */
        NULL,                               /* pxSetupTickInterruptHookFunction */
        NULL,                               /* pxTaskDeleteHookFunction */
        &vDemoApplicationErrorHook,         /* pxErrorHookFunction */
        NULL,                               /* pxIdleHookFunction */
        NULL,                               /* pxTickHookFunction */
        NULL,                               /* pxSvcHookFunction */

        /* System Stack parameters */
        configSTACK_CHECK_MARGIN,           /* uxAdditionalStackCheckMarginBytes */

        /* Idle Task parameters */
        acIdleTaskStack,                    /* pcIdleTaskStackBuffer */
        configIDLE_TASK_STACK_SIZE,         /* uxIdleTaskStackSizeBytes */
#if defined (BUILD_MCU)
        pdFALSE,                            /* xIdleTaskUsingFPU */

        /* xIdleTaskMPUParameters */
        {
            mpuUNPRIVILEGED_TASK,           /* The idle hook will be executed in unprivileged mode. */
            {
                {
                    configIDLE_TASK_DATA_ADDR,
                    configIDLE_TASK_DATA_SIZE,
                    ( portmpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE |
                      portmpuREGION_EXECUTE_NEVER |
                      portmpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),
                    0U
                },
                { NULL, 0U, 0U, 0U }
            }
        },
#endif
        NULL,                               /* pvIdleTaskTLSObject */

        /* Timer feature initialisation parameters */
        configTIMER_TASK_PRIORITY,          /* uxTimerTaskPriority */
        configTIMER_TASK_STACK_SIZE,        /* uxTimerTaskStackSize */
        acTimerTaskStack,                   /* pcTimerTaskStackBuffer */
        configTIMER_CMD_QUEUE_LEN,          /* uxTimerCommandQueueLength */
        configTIMER_CMD_QUEUE_BUFFER_SIZE,  /* uxTimerCommandQueueBufferSize */
        acTimerCommandQueueBuffer,          /* pcTimerCommandQueueBuffer */
#if defined (BUILD_MCU)
        pdTRUE                              /* xEnableCache */
#endif
    };

    /* Initialise the kernel by passing in a pointer to the xPortInit structure
     * and return the resulting error code. */
    xInitSchedResult = xTaskInitializeScheduler( &xPortInit );

    xInitSchedResult = xConfigureMPU();

    return xInitSchedResult;
}
/*--------------------------------------------------------------------------*/
void __mpu_init( void )
{
    /* Override the startup code mpu initialisation. */
}

/*-------------------------------------------------------------------------*/

/*****************************************************************************/
/* _SYSTEM_POST_CINIT() - _system_post_cinit() is a hook function called in  */
/* the C/C++ auto-initialization function after cinit() and before pinit().  */
/*                                                                           */
/* The version of _system_post_cinit() below is skeletal and is provided to  */
/* illustrate the interface and provide default behavior.  To replace this   */
/* version rewrite the routine and include it as part of the current project.*/
/* The linker will include the updated version if it is linked in prior to   */
/* linking with the C/C++ runtime library.                                   */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* __TI_default_system_post_cinit indicates that the default                 */

void _system_post_cinit(void)
{
    osalArch_Config_t cfg;

    cfg.disableIrqOnInit = true;
    osalArch_Init(&cfg);

    uint32_t            loopCnt = 0U, regAddr;
    CSL_ArmR5CPUInfo    info;
    uint32_t            maxIntrs;
    CSL_vimRegs         *pRegs;

    CSL_armR5GetCpuID(&info);

#if defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR0) && defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR1)
    if (info.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        /* MCU SS Pulsar R5 SS */
        regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                                 CSL_MCU_DOMAIN_VIM_BASE_ADDR0:
                                 CSL_MCU_DOMAIN_VIM_BASE_ADDR1;

    }
    else
    {
        /* MAIN SS Pulsar R5 SS */
        regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                                 CSL_MAIN_DOMAIN_VIM_BASE_ADDR0:
                                 CSL_MAIN_DOMAIN_VIM_BASE_ADDR1;

    }
#else
    /* MCU SS Pulsar R5 SS */
    regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                             CSL_MCU_DOMAIN_VIM_BASE_ADDR0:
                             CSL_MCU_DOMAIN_VIM_BASE_ADDR1;
#endif
    pRegs       = (CSL_vimRegs *)(uintptr_t) regAddr;
    maxIntrs    = pRegs->INFO;

#if defined (SOC_AM65XX) || defined (SOC_J721E)
    /* Limit the outstanding transactions to 2
     * only for AM65xx and J721e platforms
     * Later SoCs do not have this issue
     */
    vPortSetDLFOBit();
#else
    /* Fix for PRSDK-8161
     * For AM64x and J7200, there is no issue of limitting outstanding
     * transactions. R5F core can support full 7 outstanding transactions
     */
#endif

    /* Disable/Clear pending Interrupts in VIM before enabling CPU Interrupts */
    /* This is done to prevent serving any bogus interrupt */
    for (loopCnt = 0U ; loopCnt < maxIntrs; loopCnt++)
    {
        /* Disable interrupt in vim */
        CSL_vimSetIntrEnable((CSL_vimRegs *)(uintptr_t)regAddr,
                                    loopCnt,
                                    false);
        /* Clear interrupt status */
        CSL_vimClrIntrPending((CSL_vimRegs *)(uintptr_t)regAddr,
                                     loopCnt);
    }
}

