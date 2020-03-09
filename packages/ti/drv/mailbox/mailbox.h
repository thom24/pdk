/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 */

 /** \defgroup DRV_MAILBOX_MODULE      MAILBOX Driver
 *
 *  The Mailbox driver simplifies reading and writing to the Mailbox
 *  peripherals on the board with multiple modes of operation and performance.
 *  These include blocking, non-blocking, callback.
 *  Throughout this document the term <B>local endpoint</B> refers to the subsystem that instantiates the mailbox driver.
 *  The term <B>remote endpoint</B> refers to the subsystem that communicates to the local endpoint through the mailbox instance.\n
 *  The application initializes the Mailbox driver by calling Mailbox_init() and it is then ready to
 *  open a Mailbox instance to a remote endpoint by calling Mailbox_open(). Once the mailbox instance is open,
 *  read and write operations can be performed.\n
 *
 *  The Mailbox header file should be included in an application as follows:
 *  \code
    #include <ti/drv/mailbox/mailbox.h>
    \endcode
 *
 *  The mailbox/include/reg_mailbox.h has the register layer definitions for the
 *  Mailbox Module.
 *
 *  ## Initializing the driver #
 *  The Mailbox Driver needs to be initialized once per master. This is
 *  done using the #Mailbox_init. None of the Mailbox APIs can be used before invoking
 *  this API.
 *
 * ## Opening the driver #
 * Once the Mailbox Driver has been initialized, the Mailbox Driver instance can be opened
 * using the #Mailbox_open. \n
 *
 * ## Writing a message #
 * Once the Mailbox Driver has been opened, the application can write a message to the remote
 * endpoint using #Mailbox_write.
 *
 * ## Reading a message #
 * Once the Mailbox Driver has been opened, the application can read a message from the remote
 * endpoint using #Mailbox_read followed by #Mailbox_readFlush after the message is fully read.
 *
 */

/**
 *  \file       drv/mailbox/mailbox.h
 *
 *  \brief      Mailbox driver interface
 *
 */

#ifndef MAILBOX_DRIVER_H
#define MAILBOX_DRIVER_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include "ti/osal/osal.h"
#include "ti/drv/mailbox/soc/mailbox_soc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** @addtogroup MAILBOX_DRIVER_ERROR_CODE
\ingroup DRV_MAILBOX_MODULE
 *
 *
 @{ */

/** \brief Mailbox driver error base */
#define MAILBOX_ERRNO_BASE              (-2100)

/**
 * \brief   Error Code: Invalid argument
 */
#define MAILBOX_SOK                    (0)

/**
 * \brief   Error Code: Invalid argument
 */
#define MAILBOX_EINVAL                 (MAILBOX_ERRNO_BASE-1)

/**
 * \brief   Error Code: Operation cannot be implemented because a previous
 * operation is still not complete.
 */
#define MAILBOX_EINUSE                 (MAILBOX_ERRNO_BASE-2)

/**
 * \brief   Error Code: Out of memory
 */
#define MAILBOX_ENOMEM                 (MAILBOX_ERRNO_BASE-3)

/**
 * \brief   Error Code: Invalid configuration
 */
#define MAILBOX_EINVALCFG              (MAILBOX_ERRNO_BASE-4)

/**
 * \brief   Error Code: TX mailbox full. Application tried to send a message before ACK of
 *          the previously TX message was received.
 *
 *          This ACK is part of the mailbox peripheral protocol layer and is
 *          transparent to the application.
 */
#define MAILBOX_ETXFULL                (MAILBOX_ERRNO_BASE-5)

/**
 * \brief   Error Code: Write Acknowledge timed out. Driver was pending on semaphore
 *          waiting for an acknowledge and the semaphore timed out.
 */
#define MAILBOX_ETXACKTIMEDOUT         (MAILBOX_ERRNO_BASE-6)

/**
 * \brief   Error Code: Blocking read timed out.
 */
#define MAILBOX_EREADTIMEDOUT          (MAILBOX_ERRNO_BASE-7)

/**
 * \brief   Error Code: Mailbox driver already initialized.
 */
#define MAILBOX_EINITIALIZED           (MAILBOX_ERRNO_BASE-8)

/**
 * \brief   Error Code: Osal call failed.
 */
#define MAILBOX_EOSAL                  (MAILBOX_ERRNO_BASE-9)

/**
 * \brief   Error Code: Invalid channel type.
 *          Indicates that the channel type is invalid, OR
 *          Indicates that a mailbox instance is tried to be
 *          opened with a channel type different than other instances (for the same endpoint).
 */
#define MAILBOX_EBADCHTYPE             (MAILBOX_ERRNO_BASE-10)

/**
 * \brief   Error Code: Invalid channel type.
 *          Indicates that the channel ID is invalid, OR
 *          Indicates that a channel ID is already in use.
 */
#define MAILBOX_EBADCHID               (MAILBOX_ERRNO_BASE-11)

/**
 * \brief   Error Code: Mailbox channel in use by another instance.
 */
#define MAILBOX_ECHINUSE               (MAILBOX_ERRNO_BASE-12)


/** @}*/

/**
 * \brief   Number of possible endpoint types
 */
#define MAILBOX_NUM_ENDPOINT_TYPES      ((uint8_t)3)

/**
 * \brief   Mailbox buffer size (in bytes) for data transfer (per direction).
 */
#define MAILBOX_DATA_BUFFER_SIZE        ((uint32_t)2044U)

/*!
 *  \brief    Wait forever define
 */
#define MAILBOX_WAIT_FOREVER            (SemaphoreP_WAIT_FOREVER)

/** @addtogroup MAILBOX_DRIVER_EXTERNAL_DATA_STRUCTURE
\ingroup DRV_MAILBOX_MODULE
 @{ */

/*!
 *  \brief      A handle that is returned from a Mailbox_open() call.
 */
typedef void*  Mbox_Handle;

/*!
 *  \brief      Mailbox mode settings
 *
 *  This enum defines the read and write modes for the configured Mailbox.
 *  Note that not all types are supported by read/write APIs.
 */
typedef enum
{
    /*!
      *  Blocking mode.
      *  Driver uses a semaphore to block while data is being sent or received. Context of the call
      *  must be a Task. Please refer to Mailbox_write() and Mailbox_read() APIs for specific information.
      */
    MAILBOX_MODE_BLOCKING,
    /*!
      *  Polling mode.
      *  Please refer to Mailbox_write() and Mailbox_read() APIs for specific information.
      */
    MAILBOX_MODE_POLLING,
    /*!
      *  Call back mode.
      *  Valid only for Mailbox_read API.
      *  Application call back functions are called when interrupt is received
      *  for a new message to be read or when interrupted is received from an acknowledge of a
      *  previously transmitted message. Application still needs to call the Mailbox_read() API
      *  to handle the received message.
      */
    MAILBOX_MODE_CALLBACK
} Mailbox_Mode;

/*!
 *  \brief      Mailbox operation mode settings
 *
 *  This enum defines the operation modes for the configured Mailbox.
 */
typedef enum
{
    /*!
      *  Current only one available operation mode. In this mode the driver allows for a partial
      *  read of a received message. #Mailbox_readFlush() needs to be used once application finishes
      *  reading the message.
      */
    MAILBOX_OPERATION_MODE_PARTIAL_READ_ALLOWED
} Mailbox_OpMode;

/*!
 *  \brief      Mailbox data transfer mode
 *
 *  This enum defines the data transfer mode for the mailbox instance.\n
 *  It configures how the data is transferred from the application buffer
 *  to the mailbox peripheral and from the mailbox peripheral to the
 *  application buffer in the #Mailbox_write and #Mailbox_read APIs.
 *  The configuration applies to both directions.\n
 */
typedef enum
{
    /*!
      *  Data transfer between maibox memory and application memory done through memcpy()
      */
    MAILBOX_DATA_TRANSFER_MEMCPY,
    /*!
      *  Data transfer between maibox memory and application memory done through DMA
      *  @warning    This mode is currently not supported.
      */
    MAILBOX_DATA_TRANSFER_DMA
} Mailbox_DataTransferMode;

/*!
 *  \brief      Mailbox channel type
 *
 *  This enum defines the mailbox channel type.
 *  This field allows to use multiple (virtual) mailbox channels to communicate.\n
 *  As there is only one mailbox physical channel, even when multiple channels are open, only one
 *  channel can be used at a time by the application.\n
 *  When multiple channels are used, each channel is identified by a #Mailbox_ChID.\n
 *  The maximum number of channels (or mailbox instances) is given by #MAILBOX_CH_ID_MAX.\n
 *  Each channel is created by opening a new instance of the mailbox driver using
 *  a different Mailbox_ChID.\n
 *  If a channel is opened as #MAILBOX_CHTYPE_SINGLE then only one channel is allowed and Mailbox_ChID
 *  does not need to be populated.\n
 *  Note that the channel types can not be mixed. That is, a channel of type #MAILBOX_CHTYPE_SINGLE in
 *  MSS (or DSS) can not communicate with a channel of type #MAILBOX_CHTYPE_MULTI on DSS (or MSS).\n
 *  Also note that if a channel is opened of type #MAILBOX_CHTYPE_MULTI, then all channels <B>must</B> be of #MAILBOX_CHTYPE_MULTI.
 */
typedef enum
{
    /*!
      *  Only one mailbox channel (in each direction) allowed between local and remote endpoint.\n
      *  In this case, the field Mailbox_ChID does NOT need to be specified.
      */
    MAILBOX_CHTYPE_SINGLE = 0,
    /*!
      *  Multiple mailbox channels (in each direction) allowed between local and remote endpoint.\n
      *  In this case, the field Mailbox_ChID needs to be specified.
      */
    MAILBOX_CHTYPE_MULTI = 1,
    /*!
      *  Maximum channel type value.
      */
    MAILBOX_CHTYPE_MAX = MAILBOX_CHTYPE_MULTI
} Mailbox_ChType;

/*!
 *  \brief      Mailbox channel ID
 *
 *  This enum defines the possible mailbox channel IDs.\n
 *  IDs are used only when #Mailbox_ChType is of type #MAILBOX_CHTYPE_MULTI.
 *
 */
typedef enum
{
    /*!
      *  ID = 0
      */
    MAILBOX_CH_ID_0 = 0,
    /*!
      *  ID = 1
      */
    MAILBOX_CH_ID_1 = 1,
    /*!
      *  ID = 2
      */
    MAILBOX_CH_ID_2 = 2,
    /*!
      *  ID = 3
      */
    MAILBOX_CH_ID_3 = 3,
    /*!
      *  ID = 4
      */
    MAILBOX_CH_ID_4 = 4,
    /*!
      *  ID = 5
      */
    MAILBOX_CH_ID_5 = 5,
    /*!
      *  ID = 6
      */
    MAILBOX_CH_ID_6 = 6,
    /*!
      *  ID = 7
      */
    MAILBOX_CH_ID_7 = 7,
    /*!
      *  Maximum ID value, which is #MAILBOX_CH_ID_7
      */
    MAILBOX_CH_ID_MAX = MAILBOX_CH_ID_7
} Mailbox_ChID;

/*!
 *  \brief      The definition of a callback function used by the MAILBOX driver
 *              when used in MAILBOX_MODE_CALLBACK.
 *              The callback can occur in task or HWI context.
 *
 *  @warning    Making Mailbox_read() calls within its own callback
 *              routines are STRONGLY discouraged as it will impact Task and
 *              System stack size requirements!
 *
 *  @param      handle                  Mbox_Handle
 *  @param      remoteEndpoint          Remote endpoint
 *
 */
typedef void        (*Mailbox_Callback)    (Mbox_Handle handle, Mailbox_Instance remoteEndpoint);

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/*!
 *  \brief    Mailbox Configuration Parameters
 *
 *  Mailbox configuration parameters are used with the Mailbox_open() call. Default values for
 *  these parameters are set as part of Mailbox_openParams_init().
 *
 */
typedef struct Mailbox_Config_t
{
    /**
     * \brief  Mailbox read mode
     */
    Mailbox_Mode             readMode;
    /**
     * \brief  Mailbox write mode
     */
    Mailbox_Mode             writeMode;
    /**
     * \brief   Timeout for read semaphore
     */
    uint32_t                 readTimeout;
    /**
     * \brief   Timeout for write semaphore
     */
    uint32_t                 writeTimeout;
    /**
     * \brief   Pointer to read callback
     */
    Mailbox_Callback         readCallback;
    /**
     * \brief  Driver mode of operation
     */
    Mailbox_OpMode           opMode;
    /**
     * \brief  Data transfer mode
     */
    Mailbox_DataTransferMode dataTransferMode;
    /**
     * \brief  Mailbox channel type.
     *
     */
    Mailbox_ChType           chType;
    /**
     * \brief  Channel ID for this instance of the driver.
     *         Used only when chType is set to MAILBOX_CHTYPE_MULTI.
     *
     */
    Mailbox_ChID             chId;
} Mailbox_Config;

/**
 * \brief
 *  Mailbox Statistics
 *
 * @details
 *  The structure describes the Mailbox statistics information
 *
 */
typedef struct Mailbox_Stats_t
{
    /**
     * \brief   Status of TX box (full or empty)
     */
    uint32_t              txBoxStatus;
    /**
     * \brief   Number of TX messages successfully sent (acknowledgement has been received)
     */
    uint16_t              txCount;
    /**
     * \brief   Number of RX messages received
     */
    uint16_t              rxCount;
    /**
     * \brief   Flag used for read polling mode. Indicate if a new message has arrived.
     */
    uint32_t              newMessageFlag;
    /**
     * \brief   Number of bytes already read by past calls to the read API in current message
     */
    uint32_t              numBytesRead;
    /**
     * \brief   Number of full box ISR received
     */
    uint16_t              boxFullIsrCount;
    /**
     * \brief   Number of empty box ISR received
     */
    uint16_t              boxEmptyIsrCount;
    /**
     * \brief   Number of times the readFlush API was called.
     */
    uint16_t              readFlushCount;
    /**
     * \brief   Number of full box ISR received with invalid arg
     */
    uint8_t               boxFullIsrErrCount;
    /**
     * \brief   Number of empty box ISR received with invalid arg
     */
    uint8_t               boxEmptyIsrErrCount;
    /**
     * \brief   Mailbox register
     */
    uint32_t              regLclToRemRaw;
    /**
     * \brief   Mailbox register
     */
    uint32_t              regRemToLocRaw;
    /**
     * \brief   Mailbox register
     */
    uint32_t              regLclToRemMask;
    /**
     * \brief   Mailbox register
     */
    uint32_t              regRemToLocMask;
}Mailbox_Stats;

/**
 * \brief
 *  Mailbox initialization parameters
 */
typedef struct Mailbox_initParams_t
{
    /* \brief Local Mailbox endpoint. */
    Mailbox_Instance localEndpoint;
}Mailbox_initParams;

/**
 * \brief
 *  Mailbox Open parameters
 */
typedef struct Mailbox_openParams_t
{
    /* \brief Local Mailbox endpoint. */
    Mailbox_Instance remoteEndpoint;
    /* \brief configuration parameters. */
    Mailbox_Config  cfg;
}Mailbox_openParams;

/**
@}
*/

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
\defgroup MAILBOX_DRIVER_EXTERNAL_FUNCTION       Mailbox Driver External Functions
\ingroup DRV_MAILBOX_MODULE
\brief
*   The section has a list of all the exported APIs which the application can
*   invoke in order to use the driver.
 @{ */

/*!
 *  \brief  init structure initialization function.
 *
 *  @param[in]   initParam      Pointer to init param structure.
 *
 *  @return Returns error code in case of failure.
 *
 */
static inline int32_t Mailbox_initParams_init (Mailbox_initParams *initParam);

/*!
 *  \brief  open structure initialization function.
 *
 *  @param[in]   openParam      Pointer to open param structure.
 *
 *  @return Returns error code in case of failure.
 *
 */

static inline int32_t Mailbox_openParams_init (Mailbox_openParams *openParam);

/*!
 *  \brief  Function to initialize the Mailbox module.\n
 *          It must be called only once per local endpoint.
 *
 *  @pre    This function must be called before
 *          any other Mailbox driver API is used.
 *
 *  @param[in]   initParam   Pointer to init parameters.
 *
 *  @return Returns error code in case of failure.
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern int32_t Mailbox_init(Mailbox_initParams *initParam);

/*!
 *  \brief  Function to initialize an instance of the mailbox driver.
 *
 *  @pre    Mailbox_init() has been called
 *
 *  @param[in]   openParam          Pointer to open parameters.
 *  @param[out]  errCode            Pointer to error code value/status to be returned by the function
 *
 *
 *  @return A Mbox_Handle upon success. NULL if an error occurs.
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern Mbox_Handle Mailbox_open(Mailbox_openParams *openParam,  int32_t* errCode);

/*!
 *  \brief  Function that writes data to a Mailbox.
 *
 * Mailbox can only send one message at a time to a remote endpoint.
 * After data is copied to mailbox buffer, driver triggers interrupt to remote endpoint.
 * This means that  a call to the Mailbox_write() is always a complete mailbox transaction.
 * A new message can only be sent after the previous message has been acknowledged by the remote endpoint.
 * The acknowledgement process is handled internally by the driver.
 * All interrupts related to the write operations are managed by the driver and are not exposed to the application.
 * If called with data that will surpass the maximum size of the mailbox buffer, the write operation will fail with an error
 * code and nothing will be written to the mailbox buffer. Returns number of bytes written or error.\n
 * The maximum size of the mailbox buffer is given by #MAILBOX_DATA_BUFFER_SIZE. Application code is responsible for
 * fragmentation of the message if the size is bigger than the mailbox buffer.
 *
 *  Write modes:
 *
 * In Mailbox_MODE_BLOCKING: \n
 *  Mailbox_write() will block task execution until the message has been copied from application to the mailbox buffer
 *  and an acknowledgement is received from the remote endpoint.
 *  If a new Mailbox_write() is issued before the acknowledgement is received, the write will fail with an error
 *  code returned to application.
 *
 * In Mailbox_MODE_POLLING: \n
 *  Mailbox_write() will block task execution until the message has been copied from application to the mailbox buffer
 *  but it does not wait for an acknowledgement from the remote endpoint.
 *  If a new Mailbox_write() is issued before the acknowledgement is received, the write will fail with an error code
 *  returned to application.
 *  In this mode, application does not know when acknowledgement is received and it may try to write the next message
 *  multiple times until it succeeds as if it is polling for the status of the acknowledgement.
 *
 *  @pre    Mailbox_open() has been called
 *
 *
 *  @param[in]  handle      A Mbox_Handle
 *
 *  @param[in]  buffer      A pointer to buffer containing data to
 *                          be written to the Mailbox.
 *
 *  @param[in]  size        The number of bytes in the buffer that should be written
 *                          to the Mailbox.
 *
 *  @return Returns the number of bytes that have been written to the Mailbox.
 *          If an error occurs, one of the Mailbox Error codes is returned.
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
 /* Code review: Input variable "size" can be 16bit but 32bit matches the RL interface*/
extern int32_t Mailbox_write(Mbox_Handle handle, const uint8_t *buffer, uint32_t size);

/*!
 *  \brief  Function that reads data from a Mailbox.
 *
 *
 * Mailbox can only read one message at a time from a remote endpoint.
 * Multiple Mailbox_read() calls can be done for the same message in the mailbox.
 *
 * E.g. Application can read part of the message to figure out full message length and issue a subsequent Mailbox_read().
 *
 * Mailbox driver keeps track internally of number of bytes read for a message.
 * The first Mailbox_read() for a message always starts at byte zero.
 * In case of multiple  Mailbox_read() calls for the same message, the subsequent reads start in the
 * next byte from where the previous read stopped.
 * Once application finishes reading the message it <B>must</B> issue a  Mailbox_readFlush()
 * to release the mailbox buffer and notify the remote endpoint.\n
 * The maximum size of the mailbox buffer is given by #MAILBOX_DATA_BUFFER_SIZE. Application code is responsible for
 * fragmentation of the message if the size is bigger than the mailbox buffer.
 *
 * All interrupts related to the read operation are managed by the driver and are not exposed to the application.
 * Returns number of bytes read or error.
 *
 *  Read modes:
 *
 * In Mailbox_MODE_BLOCKING: \n
 * If this is the first read on a new message, Mailbox_read() will block task execution until a new message has arrived
 * in the local Mailbox buffer
 * and it is copied to the application buffer.
 * If this is not a new message, Mailbox_read() will block task execution until "size" bytes are copied to the application buffer.
 *
 * In Mailbox_MODE_POLLING: \n
 * If this is the first read on a new message and the new message has not arrived, Mailbox_read() will return size
 * zero to indicate no bytes were read.
 * If the new message has arrived or if this is not a new message, Mailbox_read() will copy the data to application buffer.
 * Mailbox_read() will return after "size" bytes are copied to the application buffer.
 *
 * In Mailbox_MODE_CALLBACK: \n
 * The driver will invoke the application callback function once a new message is received in the mailbox.
 * Application is responsible for calling Mailbox_read() to read the new message.
 * Mailbox_read() copies the data into the application buffer and then exit.
 *
 * In any of the modes described above, Mailbox_readFlush() needs to be issued after the message is fully read by the application.
 *
 *  @pre    Mailbox_open() has been called
 *
 *  @param[in]  handle        A Mbox_Handle
 *
 *  @param[in]  buffer        A pointer to an empty buffer in which
 *                            received data should be written to.
 *
 *  @param[in]  size          The number of bytes to be written into buffer
 *
 *  @return Returns the number of bytes that have been read from the Mailbox.
 *          If an error occurs, one of the Mailbox Error codes is returned.
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern int32_t Mailbox_read(Mbox_Handle handle, uint8_t *buffer, uint32_t size);

/*!
 *  \brief  Function that should be called after application is done reading the message.
 *
 * Notifies the remote endpoint that the local mailbox is ready to receive a new message after the previous message was read.\n
 * Remote endpoint can not send a new message to the local endpoint until Mailbox_readFlush() is issued by the local endpoint.
 * Once Mailbox_readFlush() is issued, the local endpoint must assume that the previously received message
 * is no longer in the mailbox buffer and subsequent Mailbox_read() will return no data until a new message arrives in the mailbox.
 *
 *  @pre    Mailbox_open() has been called
 *
 *  @param[in]  handle        A Mbox_Handle
 *
 *  @return Returns error code in case of failure.
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern int32_t Mailbox_readFlush(Mbox_Handle handle);

/*!
 *  \brief  Function that collects mailbox driver statistics
 *
 * This function is used to collect mailbox driver statistics.
 * It's main purpose is debugging of mailbox driver.
 *
 *  @pre    Mailbox_open() has been called
 *
 *  @param[in]   handle        A Mbox_Handle
 *  @param[out]  stats         Pointer to mailbox stats structure to be filled
 *
 *
 *  @return Returns error code in case of failure.
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern int32_t Mailbox_getStats(Mbox_Handle handle, Mailbox_Stats *stats);

/*!
 *  \brief  Function to close a Mailbox peripheral specified by the Mailbox handle
 *
 *  @pre    Mailbox_open() has been called.
 *
 *  @param[in]  handle      A Mbox_Handle returned from Mailbox_open()
 *
 *  @sa     Mailbox_open()
 *
 *  @return Returns error code in case of failure.
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern int32_t Mailbox_close(Mbox_Handle handle);

/** @} */ /* end defgroup MAILBOX_DRIVER_EXTERNAL_FUNCTION */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

static inline int32_t Mailbox_initParams_init (Mailbox_initParams *initParam)
{
    int32_t retVal = MAILBOX_EINVAL;
    if(initParam != NULL)
    {
        initParam->localEndpoint = MAILBOX_INST_INVALID;
        retVal = MAILBOX_SOK;
    }
    return retVal;
}

static inline int32_t Mailbox_openParams_init (Mailbox_openParams *openParam)
{
    int32_t retVal = MAILBOX_EINVAL;
    if(openParam != NULL)
    {
        openParam->remoteEndpoint = MAILBOX_INST_INVALID;
        openParam->cfg.readMode         = MAILBOX_MODE_BLOCKING;
        openParam->cfg.writeMode        = MAILBOX_MODE_BLOCKING;
        openParam->cfg.readTimeout      = MAILBOX_WAIT_FOREVER;
        openParam->cfg.writeTimeout     = MAILBOX_WAIT_FOREVER;
        openParam->cfg.readCallback     = NULL;
        openParam->cfg.opMode           = MAILBOX_OPERATION_MODE_PARTIAL_READ_ALLOWED;
        openParam->cfg.dataTransferMode = MAILBOX_DATA_TRANSFER_MEMCPY;
        openParam->cfg.chType           = MAILBOX_CHTYPE_SINGLE;
        openParam->cfg.chId             = MAILBOX_CH_ID_0;
        retVal = MAILBOX_SOK;
    }
    return retVal;
}

#ifdef __cplusplus
}
#endif

#endif /* MAILBOX_DRIVER_H */
