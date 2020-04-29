/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 * All rights reserved.
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
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  \ingroup DRV_SPI_MODULE
 *  \defgroup DRV_SPI_MIBSPI_MODULE SPI Driver MIBSPI API
 *            Multi-Buffer RAM SPI driver interface
 *  @{
 */

/** ============================================================================
 *  @file       MIBSPI.h
 *
 *  @brief      Multi-Buffer RAM SPI driver interface
 *
 *  The MIBSPI header file should be included in an application as follows:
 *  @code
 *  #include <ti/drv/mibspi/MIBSPI.h>
 *  @endcode
 *
 *  # Operation #
 *  The SPI driver in TI-RTOS is designed to serve a means to move data
 *  between SPI peripherals. This driver does not interpret any of the data sent
 *  to or received from this peripheral.
 *
 *  The APIs in this driver serve as an interface to a typical TI-RTOS
 *  application. The specific peripheral implementations are responsible to
 *  create all the SYS/BIOS specific primitives to allow for thread-safe
 *  operation.
 *
 *  The SPI driver operates on some key definitions and assumptions:
 *  - The driver operates transparent from the chip select. Some SPI controllers
 *    feature a hardware chip select to assert SPI slave peripherals. See the
 *    specific peripheral implementations on chip select requirements.
 *
 *  - The SPI protocol does not account for a built-in handshaking mechanism and
 *    neither does this SPI driver. Therefore, when operating in ::MIBSPI_SLAVE
 *    mode, the application must provide such a mechanism to ensure that the
 *    SPI slave is ready for the SPI master. The SPI slave must call
 *    SPI_transfer() *before* the SPI master starts transmitting. Some example
 *    application mechanisms could include:
 *    - Timed delays on the SPI master to guarantee the SPI slave is be ready
 *      for a SPI transaction.
 *    - A form of GPIO flow control from the slave to the SPI master to notify
 *      the master when ready.
 *  - When SPI operates in ::SPI_MASTER, the partition of TX/RX RAM should be 
 *    provided by application by providing proper values in ::MIBSPI_Params, such as 
 *    number of slaves and slave profiles. 
 *    The maximum number of supported slaves is platform specfic, please refer to 
 *    TRM for the information. The maximum number of slaves supported in the 
 *    driver is ::MIBSPI_SLAVE_MAX.
 *
 *  The platform specific SPI file present in the ti/drivers/mibspi/platform
 *  directory. This file is built as a part of the SPI Library for the specific
 *  platform.
 *
 *  ## Opening the driver #
 *  
 *  Code examples for opening SPI driver in master and slave mode
 *
 *  Open SPI driver as ::SPI_MASTER
 *
 *  @code
 *  MIBSPI_Handle      handle;
 *  MIBSPI_Params      params;
 *  MIBSPI_Transaction spiTransaction;
 *
 *  SPI_Params_init(&params);
 *  params.mode  = SPI_MASTER;
 *  params.u.masterParams.bitRate = bitRate;
 *  params.u.masterParams.numSlaves = 1;
 *  params.u.masterParams.slaveProf[0].chipSelect = 0;
 *  params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM/2;
 *  params.u.masterParams.slaveProf[0].dmaCfg.txDmaChanNum =1U; 
 *  params.u.masterParams.slaveProf[0].dmaCfg.rxDmaChanNum =0U;
 *  params.u.masterParams.slaveProf[1].chipSelect = 1;
 *  params.u.masterParams.slaveProf[1].ramBufLen = MIBSPI_RAM_MAX_ELEM/2;
 *  params.u.masterParams.slaveProf[1].dmaCfg.txDmaChanNum =3U; 
 *  params.u.masterParams.slaveProf[1].dmaCfg.rxDmaChanNum =2U;
 *
 *  handle = SPI_open(someSPI_configIndexValue, &params);
 *  if (!handle) {
 *      System_printf("SPI did not open");
 *  }
 *  @endcode
 *
 *
 *  Open SPI driver as ::MIBSPI_SLAVE
 *
 *  @code
 *  MIBSPI_Handle      handle;
 *  MIBSPI_Params      params;
 *  MIBSPI_Transaction spiTransaction;
 *
 *  SPI_Params_init(&params);
 *  params.mode = MIBSPI_SLAVE;
 *  params.frameFormat = SPI_POL0_PHA0;
 *  params.pinMode = SPI_PINMODE_4PIN_CS;
 *  params.shiftFormat = SPI_MSB_FIRST;
 *  params.dmaEnable = 1;
 *  params.dmaHandle = gDmaHandle;
 *  params.u.slaveParams.dmaCfg.txDmaChanNum =1U;
 *  params.u.slaveParams.dmaCfg.rxDmaChanNum =0U;
 *  handle = SPI_open(someSPI_configIndexValue, &params);
 *  if (!handle) {
 *      System_printf("SPI did not open");
 *  }
 *  @endcode
 *
 *  ## Transferring data #
 *  Data transmitted and received by the SPI peripheral is performed using
 *  SPI_transfer(). SPI_transfer() accepts a pointer to a MIBSPI_Transaction
 *  structure that dictates what quantity of data is sent and received.
 *
 *  @code
 *  MIBSPI_Transaction spiTransaction;
 *
 *  spiTransaction.count = someIntegerValue;
 *  spiTransaction.txBuf = transmitBufferPointer;
 *  spiTransaction.rxBuf = receiveBufferPointer;
 *  spiTransaction.slaveIndex = index1;  // For master mode only
 *
 *  ret = SPI_transfer(handle, &spiTransaction);
 *  if (!ret) {
 *      System_printf("Unsuccessful SPI transfer");
 *  }
 *  @endcode
 *
 *  When SPI driver is configured in ::SPI_MASTER model, slave index need to be
 *  provided to indicate which slave the transfer is for.
 *
 *  ## Canceling a transaction #
 *  SPI_transferCancel() is used to cancel a SPI transaction - not supported.
 *
 *  @code
 *  SPI_transferCancel(handle);
 *  @endcode
 *
 *  # Implementation #
 *
 *  This module serves as the main interface for TI-RTOS applications. Its
 *  purpose is to redirect the module's APIs to specific peripheral
 *  implementations which are specified using a pointer to a SPI_FxnTable.
 *
 *  The SPI driver interface module is joined (at link time) to a
 *  NULL-terminated array of MIBSPI_Config data structures named *SPI_config*.
 *  *SPI_config* is implemented in the application with each entry being an
 *  instance of a SPI peripheral. Each entry in *SPI_config* contains a:
 *  - (SPI_FxnTable *) to a set of functions that implement a SPI peripheral
 *  - (void *) data object that is associated with the SPI_FxnTable
 *  - (void *) hardware attributes that are associated to the SPI_FxnTable
 *
 *  # SPI transfer with multiple RAM buffer (icount)
 *  SPI hardware has an internal RAM buffer that stores transmit/receive data 
 *  element in 8bits or 16bits. 
 *  The SPI driver has a compile time option to transfer data bigger than RAM 
 *  buffer size. This is intended for high throughput transfers. But it has some 
 *  limitations and not supported in all SPI modes.
 *  
 *  SPI mode |  Pin Mode  | Supported?  | Limitations
 *  -------- | -------- |----------- |----------
 *  Master   |  4-pin  | Yes       |  Note1
 *  Master   |  3-pin  | Yes       |  Note1
 *  Slave    |  4-pin  | Yes       |  Note2
 *  Slave    |  3-pin  | No        |  None
 * 
 * 
 *  Note1: There will be gaps between every transfer of the RAM buffer size for 
 *        internal DMA copy of the data. During this time, Clock and CS will be 
 *        inactive.
 *  
 *  Note2: Slave needs time to do DMA copy of received data for every RAM buffer.
 *        Hence require the SPI master to deactivate  CS/ClK signal during this time. 
 *        When using another XWR1xxx device as master, it can be achieved by
 *        setting C2Tdelay/T2Cdelay/wdelay. 
 *  
 *  The maximum elements is defined as MIBSPI_RAM_MAX_ELEM. 
 *  For multiple slaves scenarios, this number is divided 
 *  among all slaves. This information is provided from application in 
 *  SPI_MasterModeParams. The maximum icount value is 32. Please make sure 
 *  the transfer length is not exceed 32 * ramLen. For High clock rate, it is recommended
 *  to use small icount values.
 *
 *  # Limitation #
 *  
 *  # Instrumentation #
 *
 *  The SPI driver interface produces log statements if instrumentation is
 *  enabled.
 *
 *  Diagnostics Mask | Log details |
 *  ---------------- | ----------- |
 *  Diags_USER1      | basic operations performed |
 *  Diags_USER2      | detailed operations performed |
 *
 *  ============================================================================
 */

#ifndef MIBSPI_H
#define MIBSPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/drv/mibspi/soc/MIBSPI_soc.h>

/*!
 * Common SPI_control command code reservation offset.
 * SPI driver implementations should offset command codes with SPI_CMD_RESERVED
 * growing positively
 *
 * Example implementation specific command codes:
 * @code
 * #define SPIXYZ_COMMAND0         SPI_CMD_RESERVED + 0
 * #define SPIXYZ_COMMAND1         SPI_CMD_RESERVED + 1
 * @endcode
 */
#define MIBSPI_CMD_RESERVED            (32U)

/*!
 * Common SPI_control status code reservation offset.
 * SPI driver implementations should offset status codes with
 * SPI_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define SPIXYZ_STATUS_ERROR0    SPI_STATUS_RESERVED - 0
 * #define SPIXYZ_STATUS_ERROR1    SPI_STATUS_RESERVED - 1
 * #define SPIXYZ_STATUS_ERROR2    SPI_STATUS_RESERVED - 2
 * @endcode
 */
#define MIBSPI_STATUS_RESERVED        (-((int32_t)32))

/*!
 * \brief   Successful status code returned by SPI_control().
 *
 * SPI_control() returns SPI_STATUS_SUCCESS if the control code was executed
 * successfully.
 */
#define MIBSPI_STATUS_SUCCESS         (int32_t)(0)

/*!
 * \brief   Generic error status code returned by SPI_control().
 *
 * SPI_control() returns MIBSPI_STATUS_ERROR if the control code was not executed
 * successfully.
 */
#define MIBSPI_STATUS_ERROR          (-((int32_t)1))

/*!
 * \brief   An error status code returned by SPI_control() for undefined
 * command codes.
 *
 * SPI_control() returns SPI_STATUS_UNDEFINEDCMD if the control code is not
 * recognized by the driver implementation.
 */
#define MIBSPI_STATUS_UNDEFINEDCMD   (-((int32_t)2))

/**
 *  @defgroup SPI_CMD Command Codes
 *  SPI_CMD_* macros are general command codes for SPI_control(). Not all SPI
 *  driver implementations support these command codes.
 *  @{
 *  @ingroup SPI_CONTROL
 */

/*!
 * @brief Command used by SPI_control to enable loopback
 *
 */
#define MIBSPI_CMD_LOOPBACK_ENABLE                         (uint32_t)(MIBSPI_CMD_RESERVED + 0)

/*!
 * @brief Command used by SPI_control to set Clock phase and polarity
 *
 */
#define MIBSPI_CMD_SET_CLOCK_PHASEPOLARITY                 (uint32_t)(MIBSPI_CMD_RESERVED + 1)

/*!
 * @brief Command used by SPI_control to get SPI driver stats
 *
 */
#define MIBSPI_CMD_GET_STATS                               (uint32_t)(MIBSPI_CMD_RESERVED + 2)

/** @}*/

/*!
 *  @brief    Wait forever define
 */
#define MIBSPI_WAIT_FOREVER                                             (~(0U))

/*!
 *  @brief
 *  Definitions for SPI Pins Operation Mode.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum MIBSPI_PinMode_e
{
    MIBSPI_PINMODE_3PIN   = 0,        /*!< 3 Pin operation Mode */
    MIBSPI_PINMODE_4PIN_CS  = 1       /*!< 4 Pin operation Mode with CS signal */
} MIBSPI_PinMode;

/*!
 *  @brief
 *  Definitions for SPI Data shift format.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum MIBSPI_DataShiftFmt_e
{
    MIBSPI_MSB_FIRST = 0,    /*!< MSB shift out first */
    MIBSPI_LSB_FIRST  = 1    /*!< LSB shift out first */
} MIBSPI_DataShiftFmt;

/*!
 *  @brief
 *  Definitions for  SPI loopback modes.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum MibSpi_LoopBackType_e
{
    MIBSPI_LOOPBK_DIGITAL= 0,    /*!< MIBSPI digital loopback  mode */
    MIBSPI_LOOPBK_ANALOG = 1,    /*!< MIBSPI analog loopback  mode */
    MIBSPI_LOOPBK_NONE = 2,    /*!< MIBSPI loopback  disable */
} MibSpi_LoopBackType;

/*!
 *  @brief      Status codes that are set by the SPI driver.
 */
typedef enum MIBSPI_Status_s {
    MIBSPI_TRANSFER_COMPLETED = 0,
    MIBSPI_TRANSFER_STARTED,
    MIBSPI_TRANSFER_CANCELED,
    MIBSPI_TRANSFER_FAILED,
    MIBSPI_TRANSFER_CSN_DEASSERT,
    MIBSPI_TRANSFER_TIMEOUT
} MIBSPI_Status;

/*!
 *  @brief
 *  Definitions for various SPI modes of operation.
 */
typedef enum MIBSPI_Mode_s {
    MIBSPI_MASTER      = 0,    /*!< SPI in master mode */
    MIBSPI_SLAVE       = 1     /*!< SPI in slave mode */
} MIBSPI_Mode;

/*!
 *  @brief
 *  Definitions for various SPI data frame formats.
 */
typedef enum MIBSPI_FrameFormat_s {
    MIBSPI_POL0_PHA0   = 0,    /*!< SPI mode Polarity 0 Phase 0 */
    MIBSPI_POL0_PHA1   = 1,    /*!< SPI mode Polarity 0 Phase 1 */
    MIBSPI_POL1_PHA0   = 2,    /*!< SPI mode Polarity 1 Phase 0 */
    MIBSPI_POL1_PHA1   = 3,    /*!< SPI mode Polarity 1 Phase 1 */
} MIBSPI_FrameFormat;

/*!
 *  @brief
 *
 *  SPI transfer mode determines the whether the SPI controller operates
 *  synchronously or asynchronously. In ::SPI_MODE_BLOCKING mode SPI_transfer()
 *  blocks code execution until the SPI transaction has completed. In
 *  ::SPI_MODE_CALLBACK SPI_transfer() does not block code execution and instead
 *  calls a ::SPI_CallbackFxn callback function when the transaction has
 *  completed.
 */
typedef enum MIBSPI_TransferMode_s {
    /*!
     * MIBSPI_transfer() blocks execution. This mode can only be used when called
     * within a Task context
     */
    MIBSPI_MODE_BLOCKING,
    /*!
     * SPI_transfer() does not block code execution and will call a
     * ::SPI_CallbackFxn. This mode can be used in a Task, Swi, or Hwi context.
     */
    MIBSPI_MODE_CALLBACK
} MIBSPI_TransferMode;

/*!
 * \brief MIBSPI info/debug print function prototype
 *
 * This function is used by the driver to print info/debug messages.
 *
 * \param str      [OUT] Info string to print
 */
typedef int (*Mibspi_PrintFxnCb)(const char *str, ...);

/*!
 * \brief MIBSPI log trace function prototype
 *
 * This function is used by the driver to trace log messages.
 *
 * \param str      [OUT] Info string to print
 */
typedef int (*Mibspi_TraceFxnCb)(const char *str, ...);



/*!
 * \brief MIBSPI utils parameters
 */
typedef struct
{
    /*! If not NULL, this function will be called to print debug/info message
     *  with appropriate string */
    Mibspi_PrintFxnCb printFxn;

    /*! If not NULL, this function will be called to log trace message
     *  with appropriate string */
    Mibspi_TraceFxnCb traceFxn;

} MIBSPI_UtilsPrms;




/*!
 *  @brief
 *  A ::MIBSPI_Transaction data structure is used with SPI_transfer(). It indicates
 *  how many ::MIBSPI_FrameFormat frames are sent and received from the buffers
 *  pointed to txBuf and rxBuf.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MIBSPI_Transaction_t
{
    /* User input (write-only) fields */
    size_t     count;      /*!< Number of frames for this transaction */
    void      *txBuf;      /*!< void * to a buffer with data to be transmitted */
    void      *rxBuf;      /*!< void * to a buffer to receive data */
    void      *arg;        /*!< Argument to be passed to the callback function */

    void      *custom;     /*!< Implementation specific argument passed to
                                SPI_Transfer(). Refer the SPI_vX implementation
                                documentation to see the specific use of this
                                parameter. */
    /* User output (read-only) fields */
    MIBSPI_Status status;     /*!< Status code set by SPI_transfer */
    uint8_t   slaveIndex; /*!< Index of the slave enabled for this transfer */

} MIBSPI_Transaction;


/*!
 *  @brief
 *  A ::MIBSPI_Stats data structure is used with SPI_getStats() to get driver statistics. 
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MIBSPI_Stats_t
{
    uint32_t   dlenErr;     /*!<Number of data length error */
    uint32_t   timeout;     /*!<Number of timeout */
    uint32_t   parErr;      /*!<Number of Parity error */
    uint32_t   desync;      /*!<Number of De-synchronization of slave device - master only */
    uint32_t   bitErr;      /*!<Number of data mismatch transmit data error - master only */
    uint32_t   rxOvrnErr;   /*!<Number of RX Overrun Error */
} MIBSPI_Stats;


/*!
 *  @brief SPI slave profile Parameters
 *
 *  When SPI is configured as master mode, it may connect to multiple slaves. This data structure captures 
 *  the the configurations for remote slaves.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MIBSPI_SlaveProfile_t
{
    uint8_t             chipSelect;                /*!< CS0-CS7 signal number from 0 -7 */

    uint8_t             ramBufLen;                 /*!< RAM Length in bytes that used by the slave */

    uint32_t            dmaReqLine;                /*!< DMA request line to be used for slave */
} MIBSPI_SlaveProfile;

/*!
 *  @brief SPI master Parameters
 *
 *  SPI master Parameters are used  with the SPI_open() call when mode is set to SPI_MASTER. 
 *  Default values for these parameters are set using SPI_Params_init().
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa         SPI_Params_init()
 */
typedef struct MIBSPI_MasterModeParams_t
{
    uint32_t            bitRate;                    /*!< SPI bit rate in Hz */

    uint8_t             numSlaves;                  /*!< Number of slaves connected to SPI master */

    uint8_t             t2cDelay;                   /*!< Transmit end to chip select inactive delay */
    
    uint8_t             c2tDelay;                   /*!< Chip select active to transmit start delay */
    
    uint8_t             wDelay;                     /*!< Delay in between transmissions */
    
    MIBSPI_SlaveProfile    slaveProf[MIBSPI_SLAVE_MAX]; /*!< Slave profile for each slave */
} MIBSPI_MasterModeParams;

/*!
 *  @brief SPI slave mode Parameters
 *
 *  SPI slave Parameters are used with the SPI_open() call when mode is set to MIBSPI_SLAVE. 
 *  Default values for these parameters are set using SPI_Params_init().
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa         SPI_Params_init()
 */
typedef struct MIBSPI_SlaveModeParams_t
{
    uint32_t            dmaReqLine;                /*!< DMA request line to be used for Slave */

    uint8_t             chipSelect;                /*!< CS0-CS7 signal number from 0 -7 */
    
} MIBSPI_SlaveModeParams;


/*!
 *  @brief      The definition of a callback function used by the SPI driver
 *              when used in ::SPI_MODE_CALLBACK
 *
 *  @param      MIBSPI_Handle          MIBSPI_Handle
 *  @param      MIBSPI_Transaction*    MIBSPI_Transaction*
 */
typedef void        (*MIBSPI_CallbackFxn) (MIBSPI_Handle handle,
                                           MIBSPI_Transaction * transaction);


/*!
 *  @brief MIBSPI Parameters
 *
 *  MIBSPI Parameters are used with the MIBSPI_open() call. Default values for
 *  these parameters are set using MIBSPI_Params_init().
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa         SPI_Params_init()
 */
typedef struct MIBSPI_Params_t
{
    MIBSPI_TransferMode    transferMode;       /*!< Blocking or Callback mode */
    uint32_t               transferTimeout;    /*!< Transfer timeout in system
                                                 ticks (Not supported with all
                                                 implementations */
    MIBSPI_CallbackFxn     transferCallbackFxn;/*!< Callback function pointer */
    MIBSPI_Mode            mode;               /*!< Master or Slave mode */
    uint32_t               bitRate;            /*!< SPI bit rate in Hz */
    uint32_t               dataSize;           /*!< SPI data frame size in bits */
    MIBSPI_FrameFormat     frameFormat;        /*!< SPI frame format */
    union 
    {
        MIBSPI_MasterModeParams  masterParams;        /*!< Configuration dedicated to master mode, refer to ::SPI_MasterModeParams_t */
    
        MIBSPI_SlaveModeParams   slaveParams;         /*!< Configuration dedicated to slave mode, refer to ::SPI_SlaveModeParams_t */
    }u;
    
    MIBSPI_PinMode         pinMode;                   /*!< Pin operation Mode, refer to ::SPI_PinMode_e */

    MIBSPI_DataShiftFmt    shiftFormat;                  /*!< SPI Data shift format, refer to ::SPI_DataShiftFmt_e */ 
    
    uint8_t             dmaEnable;                 /*!< DMA mode enable. When Dma mode is disable, SPI driver
                                                        is operated in polling mode, there is while(1) loop waiting for
                                                        hardware to finish */
    uint8_t             eccEnable;                 /*!< ECC mode enable */
    
    uint8_t             csHold;                    /*!< CS Hold enable */

    uint16_t            txDummyValue;              /*!< 16 bit value transmitted when no TX data is provided for SPI_tranfer() */

    void               *dmaHandle;                 /*!< DMA handle */

    /**
     * @brief  EDMA link channel id
     * The SPI requries an additional param to terminate Tx transfer.
     * This should be from the pool of free param sets 
     * As EDMA driver currently does not support dynamic allocation of
     * params forced to statically configure this from application.
     * TODO: Remove this once edma driver supports allocation of paramId
     *       similar to edma3 drv
     */
    uint16_t           edmaLinkParamId;            /*!< Link param id */
} MIBSPI_Params;

/*!
 *  @brief  Function to close a channel of a SPI peripheral specified by the MIBSPI handle
 *
 *  @pre    MIBSPI_open() has to be called first.
 *
 *  @param  handle A MCSPI handle returned from MCSPI_open()
 *
 *  @sa     MIBSPI_open()
 */
extern void MIBSPI_close(MIBSPI_Handle handle);

/*!
 *  @brief  Function performs implementation specific features 
 *          of a SPI peripheral specified by the SPI handle.
 *
 *  @pre    MIBSPI_open() has to be called first.
 *
 *  @param  handle      A SPI handle returned from MIBSPI_open()
 *
 *  @param  cmd         A command value defined by the driver specific
 *                      implementation
 *
 *  @param  arg         An optional R/W (read/write) argument that is
 *                      accompanied with cmd
 *
 *  @return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 *
 *  @sa     MIBSPI_open()
 */
extern int32_t MIBSPI_control(MIBSPI_Handle handle, uint32_t cmd, void *arg);

/*!
 *  @brief  This function initializes the MCSPI module.
 *
 *  @pre    The SPI_config structure must exist and be persistent before this
 *          function can be called. This function must also be called before
 *          any other MIBSPI driver APIs. This function call does not modify any
 *          peripheral registers.
 */
extern void MIBSPI_init(MIBSPI_UtilsPrms *pUtilsPrms);

/*!
 *  @brief  This function opens a given channel of a given MCSPI peripheral.
 *
 *  @pre    MIBSPI controller has been initialized using MIBSPI_init()
 *
 *  @param  mibspiInstanceId  Id of the SPI instance.The SPI instances
 *                           available in an SOC are listed in the SOC specific SPI_soc.h
 *                           file
 *
 *  @param  params        Pointer to an parameter block, if NULL it will use
 *                        default values. All the fields in this structure are
 *                        RO (read-only).
 *
 *  @return A MCSPI_Handle on success or a NULL on an error or if it has been
 *          opened already.
 *
 *  @sa     MIBSPI_init()
 *  @sa     MIBSPI_close()
 */
extern MIBSPI_Handle MIBSPI_open(enum MibSpi_InstanceId mibspiInstanceId, MIBSPI_Params *params);

/*!
 *  @brief  Function to initialize the MIBSPI_Params struct to its defaults
 *
 *  @param  params      An pointer to MIBSPI_Params structure for
 *                      initialization
 *
 *  Defaults values are:
 *      transferMode        = SPI_MODE_BLOCKING
 *      transferTimeout     = SPI_WAIT_FOREVER
 *      transferCallbackFxn = NULL
 *      mode                = SPI_MASTER
 *      bitRate             = 
 *      dataSize            = 
 *      frameFormat         = 
 */
extern void MIBSPI_Params_init(MIBSPI_Params *params);

/*!
 *  @brief  Function to perform SPI transactions on a channel of
 *          a SPI peripheral specified by the MIBSPI handle.
 *
 *  If the MIBSPI is in ::SPI_MASTER mode, it will immediately start the
 *  transaction. If the MIBSPI is in ::MIBSPI_SLAVE mode, it prepares itself for a
 *  transaction with a SPI master.
 *
 *  In ::SPI_MODE_BLOCKING, MIBSPI_transfer will block task execution until the
 *  transaction has completed.
 *
 *  In ::SPI_MODE_CALLBACK, MIBSPI_transfer() does not block task execution and
 *  calls a ::MIBSPI_CallbackFxn. This makes the MIBSPI_tranfer() safe to be used
 *  within a Task, Swi, or Hwi context. The ::MIBSPI_Transaction structure must
 *  stay persistent until the MCSPI_transfer function has completed!
 *
 *  @param  handle      A MIBSPI_Handle
 *
 *  @param  spiTrans    A pointer to a MIBSPI_Transaction. All of the fields within
 *                      transaction except MIBSPI_Transaction.count and
 *                      MIBSPI_Transaction.status are WO (write-only) unless
 *                      otherwise noted in the driver implementations. If a
 *                      transaction timeout has occured, MIBSPI_Transaction.count
 *                      will contain the number of frames that were transferred.
 *
 *  @return true if started successfully; else false
 *
 *  @sa     MIBSPI_open
 *  @sa     MIBSPI_transferCancel
 */
extern bool MIBSPI_transfer(MIBSPI_Handle handle, MIBSPI_Transaction *spiTrans);

/*!
 *  @brief  Function to cancel SPI transactions on channel of a
 *          SPI peripheral specified by the SPI handle.
 *
 *  In ::SPI_MODE_BLOCKING, SPI_transferCancel has no effect.
 *
 *  In ::SPI_MODE_CALLBACK, SPI_transferCancel() will stop an SPI transfer if
 *  if one is in progress.
 *  If a transaction was in progress, its callback function will be called
 *  in context from which this API is called from. The ::SPI_CallbackFxn
 *  function can determine if the transaction was successful or not by reading
 *  the ::MIBSPI_Status status value in the ::MIBSPI_Transaction structure.
 *
 *  @param  handle      A MIBSPI_Handle
 *
 *  @sa     MIBSPI_open
 *  @sa     MIBSPI_transfer
 */
extern void MIBSPI_transferCancel(MIBSPI_Handle handle);

/*!
 *  @brief  Function to get driver statistics
 *
 *
 *  @param  handle      A MIBSPI_Handle
 *
 *  @sa     MIBSPI_open
 *  @sa     MIBSPI_transfer
 */
extern int32_t MIBSPI_getStats(MIBSPI_Handle handle, MIBSPI_Stats *ptrStats);



#ifdef __cplusplus
}
#endif

#endif /* _MCSPI_H */

/* @} */
