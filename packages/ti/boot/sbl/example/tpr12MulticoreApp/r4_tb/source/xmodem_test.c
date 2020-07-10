
#include <stdio.h>
#include <stdlib.h>
#include "xmodem_test.h"
#include "xmodem.h"
#include "uart.h"

/*
 * TRANSMIT_MODE_BUFFER decides if the data will be read from a buffer location
 * or from a file.
 * 1U - data is read from Buffer location in TCMB
 * 0U - data is read from a file
 */
#define TRANSMIT_MODE_BUFFER        (1U)

#if (TRANSMIT_MODE_BUFFER == 0U)
#include "socid_export.h"
SOCID_Info socIdInfo;

static void readSocIDInfo (uint8_t *dataBuff, uint32_t length)
{
    uint8_t nibbleBuff[2U];
    uint32_t count;
    
    for (count = 0U; count < length; count++)
    {
        /* read both nibbles */
        DrvUart_ReadBytes (&nibbleBuff[0], 2U);
        dataBuff[count] = ((uint8_t)(nibbleBuff[0] << 4U) | (uint8_t)(nibbleBuff[1] & 0xF));
    }
}
#else
#define SIZE_20K                (20U * 1024U)
#define SIZE_4K                 (4U * 1024U)
#define SBL_IMAGE_MAX_SIZE      ((224 * 1024) - 4)
#define SOC_ID_MAX_SIZE         (SIZE_4K - 4)
typedef struct SBLImageInfo_t {
    volatile uint32_t imageSize;
    uint8_t imageBuff[SBL_IMAGE_MAX_SIZE];
} SBLImageInfo;

typedef struct SOCIdInfo_t {
    uint32_t idSize;
    uint8_t idBuff[SOC_ID_MAX_SIZE];
} SOCIdInfo;

SOCIdInfo gSocIdInfo;
__attribute__ ((section(".priImg")))
SBLImageInfo gPrimaryImageInfo;
//__attribute__ ((section(".secImg")))
//SBLImageInfo gSecondaryImageInfo;
#endif
volatile uint32_t xModemDebug = 0U;

void xmodemTest (void)
{
    int32_t retVal = 0;

    /* Divider value = ((200M/(115200 * 16)) - 1) */
    DrvUart_Open (0x6BU);
#if (TRANSMIT_MODE_BUFFER == 0U)
    FILE *ptr;
    
    /* read the SOC INFO */
    readSocIDInfo ((uint8_t*)&socIdInfo, sizeof(socIdInfo));

    /* open the file */
    ptr = fopen("C:\\git\\mmwave_boot_test\\ti\\sbl\\tpr12\\r5_sbl_tpr12_cert_dkey_se.bin", "rb");
    
    if (ptr != 0)
	{
		retVal = xmodemTransmit(ptr);
	}
    else
    {
        // Error condition
        while (xModemDebug == 0U);
    }
    
#else
    gSocIdInfo.idSize = 0U;
    /* Try sending primary image */
    retVal = xmodemTransmitBuff (&gSocIdInfo.idBuff[0], &gSocIdInfo.idSize, &gPrimaryImageInfo.imageBuff[0], gPrimaryImageInfo.imageSize);
    /* Try sending Secondary image */
    //retVal = xmodemTransmitBuff (&gSocIdInfo.idBuff[0], &gSocIdInfo.idSize, &gSecondaryImageInfo.imageBuff[0], gSecondaryImageInfo.imageSize);

#endif

    if(retVal != 0)
    {
        while(xModemDebug == 0U);
    }
    
    /* wait here */
    while(xModemDebug == 0U);
}
