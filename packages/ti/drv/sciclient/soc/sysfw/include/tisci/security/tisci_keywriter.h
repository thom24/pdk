/*
 *  Copyright (C) 2017-2021 Texas Instruments Incorporated
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
 * \ingroup TISCI
 * \defgroup tisci_keywriter tisci_keywriter
 *
 * DMSC controls the power management, security and resource management
 * of the device.
 *
 *
 * @{
 */
/**
 *
 *  \brief  This file contains:
 *
 *          WARNING!!: Autogenerated file from SYSFW. DO NOT MODIFY!!
 * Copyright (C) 2019-2021 Texas Instruments Incorporated - http://www.ti.com/
 */
#ifndef KEYWRITER_H
#define KEYWRTIER_H

/**
 *
 *  \brief Declarations for all functions dealing with keywriter
 */


/**
 * \brief Request message for keywriter
 *
 * \param hdr                           standard TISCI header
 * \param image_addr_lo        Lower 32 bit address (in LE format) of the certificate
 * \param image_addr_hi        Higher 32 bit address (in LE format) of the certificate
 * \param key_prog_mask        Reserved for future use
 */
struct tisci_msg_keywriter_req {
    struct tisci_header    hdr;
    uint32_t            image_addr_lo;
    uint32_t            image_addr_hi;
    uint32_t            key_prog_mask[2];
} __attribute__((__packed__));

/**
 * \brief Response message for keywriter
 *
 * \param hdr        standard TISCI header
 * \param debug_response  debug code for success/failure
 */
struct tisci_msg_keywriter_resp {
    struct tisci_header    hdr;
    uint32_t            debug_response;
} __attribute__((__packed__));

enum keywriter_error_codes {
    /** Error in Decrypting AES256 key randomly generated by customer */
    KEYWR_ERR_DECRYPT_AES256_KEY = 0,
    /** Error in Decrypting BMEK extension field */
    KEYWR_ERR_DECRYPT_BMEK = 1,
    /** Error in Decrypting BMPKH extension field */
    KEYWR_ERR_DECRYPT_BMPKH = 2,
    /** Error in Decrypting SMEK extension field */
    KEYWR_ERR_DECRYPT_SMEK = 3,
    /** Error in Decrypting SMPKH extension field */
    KEYWR_ERR_DECRYPT_SMPKH = 4,
    /** Internal Operation Error */
    KEYWR_ERR_INTERAL_OP = 5,
    /** Invalid extension count in x509 certificate. Either SMPKH, SMEK pair or BMPKH, BMEK, SMPKH, SMEK should be used. Any other combination will trigger error */
    KEYWR_ERR_INVALID_EXT_COUNT = 6,
    /** Error in parsing certificate */
    KEYWR_ERR_PARSE_CERT = 7,
    /** Error in parsing TI FEK (appended to TIFS binary, before encryption) */
    KEYWR_ERR_PARSE_FEK = 8,
    /** Error in parsing SMPK signed certificate (certificate that contains customer key data) */
    KEYWR_ERR_PARSE_SMPK_CERT = 9,
    /** Error in programming BMEK into SoC eFuses */
    KEYWR_ERR_PROGR_BMEK = 10,
    /** Error in programming BMPKH part 1 into SoC eFuses */
    KEYWR_ERR_PROGR_BMPKH_PART_1 = 11,
    /** Error in programming BMPKH part 2 into SoC eFuses */
    KEYWR_ERR_PROGR_BMPKH_PART_2 = 12,
    /** Error in programming KEY COUNT into SoC eFuses */
    KEYWR_ERR_PROGR_KEYCOUNT = 13,
    /** Error in programming KEY REV into SoC eFuses */
    KEYWR_ERR_PROGR_KEYREV = 14,
    /** Error in programming SMEK into SoC eFuses */
    KEYWR_ERR_PROGR_SMEK = 15,
    /** Error in programming SMPKH part 1 into SoC eFuses */
    KEYWR_ERR_PROGR_SMPKH_PART_1 = 16,
    /** Error in programming SMPKH part 2 into SoC eFuses */
    KEYWR_ERR_PROGR_SMPKH_PART_2 = 17,
    /** Error validating certificate */
    KEYWR_ERR_VALIDATION_CERT = 18,
    /** Error validating SMPK signed certificate */
    KEYWR_ERR_VALIDATION_SMPK_CERT = 19,
    /** Error validating BMPK key */
    KEYWR_ERR_VALIDATION_BMPK_KEY = 20,
    /** Error validating SMPK key */
    KEYWR_ERR_VALIDATION_SMPK_KEY = 21,
    /** Error write protecting key count row */
    KEYWR_ERR_WRITE_PROT_KEYCOUNT = 22,
    /** Error write protecting key revision row */
    KEYWR_ERR_WRITE_PROT_KEYREV = 23,
};


#endif

/* @} */
