/*
 * CSL Functionality
 *
 * This is the core CSL file which implements the field get/set functions
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>

/* mmWave RBL Include Files: */
#include <csl.h>

/**************************************************************************
 ***************************** CSL Functions ******************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is the core CSL make function which is used to set the
 *      specified value in the bits between the MSB & LSB.
 *
 *      Use this variant of the function if the value is 8bits
 *
 *  @param[in]  msb
 *      Most significant Bit
 *  @param[in]  lsb
 *      Least significant Bit
 *  @param[in]  val
 *      Value to be set
 *
 *  \ingroup CSL_FUNCTION
 *
 *  @retval
 *      Updated value
 */
uint32_t CSL_make8 (uint8_t msb, uint8_t lsb, uint8_t val)
{
    uint32_t    mask;
    uint8_t     bits;
    uint32_t    newVal;

    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    newVal = (uint32_t)val & mask;

    return (newVal << lsb);
}

/**
 *  @b Description
 *  @n
 *      This is the core CSL make function which is used to set the
 *      specified value in the bits between the MSB & LSB.
 *
 *      Use this variant of the function if the value is 16bits
 *
 *  @param[in]  msb
 *      Most significant Bit
 *  @param[in]  lsb
 *      Least significant Bit
 *  @param[in]  val
 *      Value to be set
 *
 *  \ingroup CSL_FUNCTION
 *
 *  @retval
 *      Updated value
 */
uint32_t CSL_make16 (uint8_t msb, uint8_t lsb, uint16_t val)
{
    uint32_t    mask;
    uint8_t     bits;
    uint32_t    newVal;

    /* Compute the mask: */
    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    newVal = (uint32_t)val & mask;

    return (newVal << lsb);
}

/**
 *  @b Description
 *  @n
 *      This is the core CSL make function which is used to set the
 *      specified value in the bits between the MSB & LSB.
 *
 *      Use this variant of the function if the value is 32bits
 *
 *  @param[in]  msb
 *      Most significant Bit
 *  @param[in]  lsb
 *      Least significant Bit
 *  @param[in]  val
 *      Value to be set
 *
 *  \ingroup CSL_FUNCTION
 *
 *  @retval
 *      Updated value
 */
uint32_t CSL_make32 (uint8_t msb, uint8_t lsb, uint32_t val)
{
    uint32_t    mask;
    uint8_t     bits;
    uint32_t    newVal;

    /* Compute the mask: */
    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    newVal = val & mask;

    return (newVal << lsb);
}

/**
 *  @b Description
 *  @n
 *      This is the core CSL extract function which is used to get
 *      the value from the specified register in the bits
 *      between the MSB & LSB.
 *
 *      Use this variant of the function if the value is 8bits
 *
 *  @param[in]  reg
 *      Register from which the value is to be extracted
 *  @param[in]  msb
 *      Most significant Bit
 *  @param[in]  lsb
 *      Least significant Bit
 *
 *  \ingroup CSL_FUNCTION
 *
 *  @retval
 *      Extracted value
 */
uint8_t CSL_extract8 (volatile uint32_t reg, uint8_t msb, uint8_t lsb)
{
    uint32_t    mask;
    uint8_t     bits;
    uint8_t     value;

    /* Compute the mask: */
    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    value = (uint8_t)((reg >> lsb) & mask);
    return value;
}

/**
 *  @b Description
 *  @n
 *      This is the core CSL extract function which is used to get
 *      the value from the specified register in the bits
 *      between the MSB & LSB.
 *
 *      Use this variant of the function if the value is 16bits
 *
 *  @param[in]  reg
 *      Register from which the value is to be extracted
 *  @param[in]  msb
 *      Most significant Bit
 *  @param[in]  lsb
 *      Least significant Bit
 *
 *  \ingroup CSL_FUNCTION
 *
 *  @retval
 *      Extracted value
 */
uint16_t CSL_extract16 (volatile uint32_t reg, uint8_t msb, uint8_t lsb)
{
    uint32_t    mask;
    uint8_t     bits;
    uint16_t    value;

    /* Compute the mask: */
    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    value = (uint16_t)((reg >> lsb) & mask);
    return value;
}

/**
 *  @b Description
 *  @n
 *      This is the core CSL extract function which is used to get
 *      the value from the specified register in the bits
 *      between the MSB & LSB.
 *
 *      Use this variant of the function if the value is 32bits
 *
 *  @param[in]  reg
 *      Register from which the value is to be extracted
 *  @param[in]  msb
 *      Most significant Bit
 *  @param[in]  lsb
 *      Least significant Bit
 *
 *  \ingroup CSL_FUNCTION
 *
 *  @retval
 *      Extracted value
 */
uint32_t CSL_extract32 (volatile uint32_t reg, uint8_t msb, uint8_t lsb)
{
    uint32_t    mask;
    uint8_t     bits;
    uint32_t    value;

    /* Compute the mask: */
    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    value = (reg >> lsb) & mask;
    return value;
}

/**
 *  @b Description
 *  @n
 *      This is the CSL core insert function which is used to insert the
 *      value into a specific register in the bits between the MSB & LSB.
 *
 *      Use this variant of the function if the value is 8bits
 *
 *  @param[in]  reg
 *      Register to which the value is to be inserted
 *  @param[in]  msb
 *      Most significant Bit
 *  @param[in]  lsb
 *      Least significant Bit
 *  @param[in]  val
 *      Value to be set
 *
 *  \ingroup CSL_FUNCTION
 *
 *  @retval
 *      Updated value
 */
uint32_t CSL_insert8 (volatile uint32_t reg, uint8_t msb, uint8_t lsb, uint8_t val)
{
    uint32_t    mask;
    uint8_t     bits;
    uint32_t    value;
    uint32_t    tmp;

    /* Compute the mask: */
    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    value = (mask << lsb);
    tmp   = (reg & ~value);
    return (tmp | CSL_make8(msb, lsb, val));
}

/**
 *  @b Description
 *  @n
 *      This is the CSL core insert function which is used to insert the
 *      value into a specific register in the bits between the MSB & LSB.
 *
 *      Use this variant of the function if the value is 16bits
 *
 *  @param[in]  reg
 *      Register to which the value is to be inserted
 *  @param[in]  msb
 *      Most significant Bit
 *  @param[in]  lsb
 *      Least significant Bit
 *  @param[in]  val
 *      Value to be set
 *
 *  \ingroup CSL_FUNCTION
 *
 *  @retval
 *      Updated value
 */
uint32_t CSL_insert16 (volatile uint32_t reg, uint8_t msb, uint8_t lsb, uint16_t val)
{
    uint32_t    mask;
    uint8_t     bits;
    uint32_t    value;
    uint32_t    tmp;

    /* Compute the mask: */
    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    value = (mask << lsb);
    tmp   = (reg & ~value);
    return (tmp | CSL_make16(msb, lsb, val));
}

/**
 *  @b Description
 *  @n
 *      This is the CSL core insert function which is used to insert the
 *      value into a specific register in the bits between the MSB & LSB.
 *
 *      Use this variant of the function if the value is 32bits
 *
 *  @param[in]  reg
 *      Register to which the value is to be inserted
 *  @param[in]  msb
 *      Most significant Bit
 *  @param[in]  lsb
 *      Least significant Bit
 *  @param[in]  val
 *      Value to be set
 *
 *  \ingroup CSL_FUNCTION
 *
 *  @retval
 *      Updated value
 */
uint32_t CSL_insert32 (volatile uint32_t reg, uint8_t msb, uint8_t lsb, uint32_t val)
{
    uint32_t    mask;
    uint8_t     bits;
    uint32_t    value;
    uint32_t    tmp;

    /* Compute the mask: */
    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    value = (mask << lsb);
    tmp   = (reg & ~value);
    return (tmp | CSL_make32(msb, lsb, val));
}

