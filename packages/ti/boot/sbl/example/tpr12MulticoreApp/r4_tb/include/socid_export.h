/*
 * SOC Identifier Export file
 *
 * The file is the exported interface which describes the SOC Identifier
 * data which is exported by the R5 Boot ROM over the supported peripherals
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */
#ifndef SOCID_EXPORT_H
#define SOCID_EXPORT_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>

/**************************************************************************
 *************************** Configurations *******************************
 **************************************************************************/

/**
 * @brief   This is the length of the TI/Customer key public hash.
 */
#define CONFIG_SOCID_ROOTKEYHASH_LEN_BYTES      (64U)

/**
 * @brief   This is the length of the Unique identifier.
 */
#define CONFIG_SOCID_UNIQUEID_LEN_BYTES         (64U)

/**
 * @brief   This is the length of the device name
 */
#define CONFIG_SOCID_DEVNAME_LEN_BYTES          (12U)

/**************************************************************************
 ***************************** Structures *********************************
 **************************************************************************/

/**
 * @brief
 *  SOC Identifier Device Type
 *
 * @details
 *  The enumeration describes all the device types which are supported
 */
typedef enum SOCID_DeviceType_e
{
    SOCID_DeviceType_GP     = 0xABCD0001U,  /*!< General Purpose Device */
    SOCID_DeviceType_TEST,                  /*!< Test Device            */
    SOCID_DeviceType_EMU_FS,                /*!< EMU-FS Device          */
    SOCID_DeviceType_HS_FS,                 /*!< HS-FS Device           */
    SOCID_DeviceType_EMU_SE,                /*!< EMU-SE Device          */
    SOCID_DeviceType_HS_SE,                 /*!< HS-SE Device           */
}SOCID_DeviceType;

/**
 * @brief
 *  SOCID Public Information
 *
 * @details
 *  The structure holds the public information specific to the HSM Boot ROM.
 *  Public Information is available for all device types
 */
typedef struct SOCID_HSMPublicInfo_t
{
    /**
     * @brief   Device Name: This is the ASCII null terminated string.
     */
    uint8_t             devName[CONFIG_SOCID_DEVNAME_LEN_BYTES];

    /**
     * @brief   Device Type:
     */
    SOCID_DeviceType    devType;

    /**
     * @brief   Version of the HSM Boot ROM:
     *  - Reserved     : Bit31-Bit24
     *  - Major Version: Bit23-Bit16
     *  - Minor Version: Bit15-Bit8
     *  - Patch Version: Bit7-Bit0
     */
    uint32_t            hsmBootROMVersion;
}SOCID_HSMPublicInfo;

/**
 * @brief
 *  SOCID Secure Information
 *
 * @details
 *  The structure holds the secure information. Secure Information is
 *  available for all HS devices.
 */
typedef struct SOCID_HSMSecureInfo_t
{
    /**
     * @brief   Prime or Non-Prime device:
     * - Prime Device     = 1
     * - Non Prime Device = 0
     * All other values are reserved.
     */
    uint32_t    isPrime;

    /**
     * @brief   Key Count: This is the number of keys which have been provisioned.
     *  - 0 indicates that no keys have been provisioned
     *  - Non Zero indicates that customer keys have been provisioned.
     */
    uint32_t    keyCount;

    /**
     * @brief   Key Revision: The key revision indicates the manufacturer key which will
     * be used i.e. the Secondary or Backup Manufacturer.
     * - Secondary Manufacturer Key = 1
     * - Backup Manufacturer Key    = 2
     * All other values are reserved. This is only applicable for HS-SE devices.
     */
    uint32_t    keyRevision;

    /**
     * @brief   Software Revision for the R5 SBL: The software revision is used to
     * support the software anti-rollback feature.
     */
    uint32_t    swRevSBL;

    /**
     * @brief   Software Revision for the HSM Runtime: The software revision is used to
     * support the software anti-rollback feature.
     */
    uint32_t    swRevHSM;

    /**
     * @brief   TI Public Key Hash:
     */
    uint8_t     tiKeyHash[CONFIG_SOCID_ROOTKEYHASH_LEN_BYTES];

    /**
     * @brief   Active Customer Public Key Hash:
     */
    uint8_t     custKeyHash[CONFIG_SOCID_ROOTKEYHASH_LEN_BYTES];

    /**
     * @brief   Unique Identifier:
     */
    uint8_t     uniqueID[CONFIG_SOCID_UNIQUEID_LEN_BYTES];
}SOCID_HSMSecureInfo;

/**
 * @brief
 *  SOCID HSM Information
 *
 * @details
 *  The structure holds the information pertinent to the SOC. This
 *  information is passed from the HSM to the R5 and this will be
 *  sent out via the peripheral boot modes.
 */
typedef struct SOCIDHSM_Info_t
{
    /**
     * @brief   Public Information:
     */
    SOCID_HSMPublicInfo    publicInfo;

    /**
     * @brief   Secure Information:
     */
    SOCID_HSMSecureInfo    secureInfo;
}SOCIDHSM_Info;

/**
 * @brief
 *  SOCID R5 Information
 *
 * @details
 *  The structure holds the R5 bootrom information.
 */
typedef struct SOCIDR5_Info_t
{
    /**
     * @brief   R5 Boot ROM version information
     *  - Reserved   : Bit31-Bit24
     *  - Major Version: Bit23-Bit16
     *  - Minor Version: Bit15-Bit8
     *  - Patch Version: Bit7-Bit0
     */
    uint32_t            r5BootROMVersion;
}SOCIDR5_Info;

/**
 * @brief
 *  SOCID hardware Information
 *
 * @details
 *  The structure holds the hardware verion information.
 */
typedef struct SOCIDHW_Info_t
{
    /**
     * @brief   EFuse to differentiate between Automotive parts, industrial parts,
     *          Secure parts, Non-secure parts and RnD parts. Only 11-0 are valid
     */
    uint16_t          partID;

    /**
     * @brief   Efuse for part number (TPR12, TPR22). Only 1-0 bits are valid
     */
    uint8_t           partNumber;

    /**
     * @brief   Efuse for PG version. Only 3-0 bits are valid
     */
    uint8_t           PGVersion;

    /**
     * @brief   Efuse for ROM version. Only 2-0 bits are valid
     */
    uint8_t           ROMVersion;

    /**
     * @brief   Efuse for Metal version. Only 2-0 bits are valid
     */
    uint8_t           metalVersion;

    /**
     * @brief   Padding to make 32bits aligned
     */
    uint16_t          padding;
}SOCIDHW_Info;

/**
 * @brief
 *  SOCID Information
 *
 * @details
 *  The structure holds the information pertinent to the SOC. This
 *  information is sent out via peripheral boot interface when
 *  is suitable.
 */
typedef struct SOCID_Info_t
{
    /**
     * @brief   Hardware Information:
     */
    SOCIDHW_Info        hwInfo;

    /**
     * @brief   R5 Information:
     */
    SOCIDR5_Info        r5Info;

    /**
     * @brief   HSM Information:
     */
    SOCIDHSM_Info       hsmInfo;
}SOCID_Info;

#ifdef __cplusplus
}
#endif

#endif /* SOCID_EXPORT_H */

