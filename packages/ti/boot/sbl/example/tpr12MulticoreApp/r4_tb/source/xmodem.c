/*
 * xmodem
 *
 * Xmodem transmitter implementation
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include "stdint.h"
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "xmodem.h"


#define SOH                 0x01U
#define STX                 0x02U
#define EOT                 0x04U
#define ACK                 0x06U
#define NAK                 0x15U
#define CAN                 0x18U
#define CTRLZ               0x1AU
#define MAXRETRANS          25U
#define CRC_POLY_CCITT		0x1021U

/**********************************************************************
*********************** Compile Time MACROS to change behavior ********
**********************************************************************/
/* below MACROs simulate alternte scenario */
/* Set FAST_CRC to 1 to calculate CRC for all packets at the start */
#define FAST_CRC                    (1U)

/* Set PRINT_PROGRESS to 1 to pring progress on output console */
#define PRINT_PROGRESS              (1U)

/* Set GENERATE_CRC_INIT_TABLE to 1 to initialize the table instead
   of using already initialized table */
#define GENERATE_CRC_INIT_TABLE     (0U)

#define UTILS_ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#define XMODEM_ASSERT(x) do {\
                             volatile int waitInLoop = 1; \
							 if (!(x)) while(waitInLoop);    \
                         } while(0)							 


/*
 * XMODEM_1K decides if the data will be sent in chunks of 128 or 1024 bytes
 * 1U - data chunks are 1024 bytes
 * 0U - data chunks are 128bytes
 */
#define XMODEM_1K                   (1U)

/* below MACROs are to simulate various negative scenarios */
#define CANCEL_ONGOING_TRANSFER     (0U)
#define PHYSICAL_MEDIUM_DISCONNECT  (0U)
/**********************************************************************
*********************** Local Functions Prototype **************************
**********************************************************************/
static void init_crcccitt_tab(void);
static uint16_t crc16_ccitt(const uint8_t *input_str, uint32_t num_bytes);

/**************************************************************************
 ****************************** Global Variables **************************
 **************************************************************************/

uint8_t crc_tabccitt_init = 0U;
uint16_t crcValues[1024U];

#if (GENERATE_CRC_INIT_TABLE == 0U)
/* 
 * CRC-16 lookup table
 */
const uint16_t crc_tabccitt[] =
{
	0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50a5U, 0x60c6U, 0x70e7U,
	0x8108U, 0x9129U, 0xa14aU, 0xb16bU, 0xc18cU, 0xd1adU, 0xe1ceU, 0xf1efU,
	0x1231U, 0x0210U, 0x3273U, 0x2252U, 0x52b5U, 0x4294U, 0x72f7U, 0x62d6U,
	0x9339U, 0x8318U, 0xb37bU, 0xa35aU, 0xd3bdU, 0xc39cU, 0xf3ffU, 0xe3deU,
	0x2462U, 0x3443U, 0x0420U, 0x1401U, 0x64e6U, 0x74c7U, 0x44a4U, 0x5485U,
	0xa56aU, 0xb54bU, 0x8528U, 0x9509U, 0xe5eeU, 0xf5cfU, 0xc5acU, 0xd58dU,
	0x3653U, 0x2672U, 0x1611U, 0x0630U, 0x76d7U, 0x66f6U, 0x5695U, 0x46b4U,
	0xb75bU, 0xa77aU, 0x9719U, 0x8738U, 0xf7dfU, 0xe7feU, 0xd79dU, 0xc7bcU,
	0x48c4U, 0x58e5U, 0x6886U, 0x78a7U, 0x0840U, 0x1861U, 0x2802U, 0x3823U,
	0xc9ccU, 0xd9edU, 0xe98eU, 0xf9afU, 0x8948U, 0x9969U, 0xa90aU, 0xb92bU,
	0x5af5U, 0x4ad4U, 0x7ab7U, 0x6a96U, 0x1a71U, 0x0a50U, 0x3a33U, 0x2a12U,
	0xdbfdU, 0xcbdcU, 0xfbbfU, 0xeb9eU, 0x9b79U, 0x8b58U, 0xbb3bU, 0xab1aU,
	0x6ca6U, 0x7c87U, 0x4ce4U, 0x5cc5U, 0x2c22U, 0x3c03U, 0x0c60U, 0x1c41U,
	0xedaeU, 0xfd8fU, 0xcdecU, 0xddcdU, 0xad2aU, 0xbd0bU, 0x8d68U, 0x9d49U,
	0x7e97U, 0x6eb6U, 0x5ed5U, 0x4ef4U, 0x3e13U, 0x2e32U, 0x1e51U, 0x0e70U,
	0xff9fU, 0xefbeU, 0xdfddU, 0xcffcU, 0xbf1bU, 0xaf3aU, 0x9f59U, 0x8f78U,
	0x9188U, 0x81a9U, 0xb1caU, 0xa1ebU, 0xd10cU, 0xc12dU, 0xf14eU, 0xe16fU,
	0x1080U, 0x00a1U, 0x30c2U, 0x20e3U, 0x5004U, 0x4025U, 0x7046U, 0x6067U,
	0x83b9U, 0x9398U, 0xa3fbU, 0xb3daU, 0xc33dU, 0xd31cU, 0xe37fU, 0xf35eU,
	0x02b1U, 0x1290U, 0x22f3U, 0x32d2U, 0x4235U, 0x5214U, 0x6277U, 0x7256U,
	0xb5eaU, 0xa5cbU, 0x95a8U, 0x8589U, 0xf56eU, 0xe54fU, 0xd52cU, 0xc50dU,
	0x34e2U, 0x24c3U, 0x14a0U, 0x0481U, 0x7466U, 0x6447U, 0x5424U, 0x4405U,
	0xa7dbU, 0xb7faU, 0x8799U, 0x97b8U, 0xe75fU, 0xf77eU, 0xc71dU, 0xd73cU,
	0x26d3U, 0x36f2U, 0x0691U, 0x16b0U, 0x6657U, 0x7676U, 0x4615U, 0x5634U,
	0xd94cU, 0xc96dU, 0xf90eU, 0xe92fU, 0x99c8U, 0x89e9U, 0xb98aU, 0xa9abU,
	0x5844U, 0x4865U, 0x7806U, 0x6827U, 0x18c0U, 0x08e1U, 0x3882U, 0x28a3U,
	0xcb7dU, 0xdb5cU, 0xeb3fU, 0xfb1eU, 0x8bf9U, 0x9bd8U, 0xabbbU, 0xbb9aU,
	0x4a75U, 0x5a54U, 0x6a37U, 0x7a16U, 0x0af1U, 0x1ad0U, 0x2ab3U, 0x3a92U,
	0xfd2eU, 0xed0fU, 0xdd6cU, 0xcd4dU, 0xbdaaU, 0xad8bU, 0x9de8U, 0x8dc9U,
	0x7c26U, 0x6c07U, 0x5c64U, 0x4c45U, 0x3ca2U, 0x2c83U, 0x1ce0U, 0x0cc1U,
	0xef1fU, 0xff3eU, 0xcf5dU, 0xdf7cU, 0xaf9bU, 0xbfbaU, 0x8fd9U, 0x9ff8U,
	0x6e17U, 0x7e36U, 0x4e55U, 0x5e74U, 0x2e93U, 0x3eb2U, 0x0ed1U, 0x1ef0U
};

static void init_crcccitt_tab(void)
{
	crc_tabccitt_init = 1U;
}

#else

uint16_t crc_tabccitt[256U];

/*
 * The following function is only there to create the CRC init array. This 
 * should not be used as it takes a lot of time to create the array
 */
static void init_crcccitt_tab(void)
{

	uint16_t i;
	uint16_t j;
	uint16_t crc;
	uint16_t c;
	FILE *fp = fopen("Output.txt", "w");

	for (i = 0; i < 256; i++) {

		crc = 0;
		c = i << 8;

		for (j = 0; j < 8; j++) {

			if ((crc ^ c) & 0x8000) crc = (crc << 1) ^ CRC_POLY_CCITT;
			else                      crc = crc << 1;

			c = c << 1;
		}

		crc_tabccitt[i] = crc;
		//if ((i % 8) == 7)
		//{
		//	fprintf(fp, "0x%04x\n", crc_tabccitt[i]);
		//}
		//else
		//{
		//	fprintf(fp, "0x%04x,", crc_tabccitt[i]);
		//}
	}

	crc_tabccitt_init = 1U;

}  /* init_crcccitt_tab */
#endif

/**
 *  @b Description
 *  @n
 *     Function to calculate the CRC-16 for the byte stream
 *
 *  @param[in]  input_str
 *              pointer to the data for which the CRC-16 needs to be calculated
 *  @param[in]  num_bytes
 *              number of bytes in the input string for which the CRC-16 needs to
 *              be calculated   
 *
 *  @retval     CRC-16 value corresponding to the input string
 */
static uint16_t crc16_ccitt(const uint8_t *input_str, uint32_t num_bytes)
{

	uint16_t crc;
	uint16_t start_value = 0x0;
	uint16_t tmp;
	uint16_t short_c;
	const uint8_t *ptr;
	uint32_t a;

	if (!crc_tabccitt_init)
	{
		init_crcccitt_tab();
	}

	crc = start_value;
	ptr = input_str;

	if (ptr != 0U) for (a = 0; a < num_bytes; a++) {

		short_c = 0x00ff & (uint16_t)*ptr;
		tmp = (crc >> 8) ^ short_c;
		crc = (crc << 8) ^ crc_tabccitt[tmp];

		ptr++;
	}

	return crc;

}

#if (PRINT_PROGRESS == 1U)
static void printProgress (uint32_t transmitDataLen, uint32_t imageSize)
{
    uint32_t percentageData;
    
    percentageData = (transmitDataLen * 100U)/ imageSize;
    if(percentageData > 100)
    {
        percentageData = 100;
    }
    
    printf ("%d%%\n", percentageData);
}
#endif

/**************************************************************************
 ************************ Function definitions ****************************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *     Function to transmit the File content via xmodem protocol over UART port
 *
 *  @param[in]  ptr
 *              File pointer from where the data will be read for transmitting
 *
 *  @retval     retVal
 *               0  -   SUCCESS
 *              -1  -   CANCEL command is received instead of data request
 *              -2  -   Request is not received (only if timeout is implemented)
 *              -3  -   CANCEL command is received between the data packets
 *              -4  -   No Acknowledgement is received for EOT
 *          
 */
int32_t xmodemTransmit(FILE *ptr)
{
    uint8_t xbuff[1030U]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
	uint32_t bufsz, crc = 0U;
	uint8_t packetno = 1U;
	uint32_t packetSize = 0U;
	uint8_t recvBuff[1U] = { 0U };
	uint8_t requestReceived = 0U;
	uint8_t ackReceived = 0U;
	uint8_t ackMsg = ACK;
	uint8_t cancMsg = CAN;
	uint8_t eotMsg = EOT;
	int32_t dataRead;
	int32_t retVal = 0U;
	uint32_t i, len = 0U;
	uint32_t retry;
    
    /* check if we are sending 1024 bytes or 128 bytes chunk */
#if (XMODEM_1K == 1U)
    xbuff[0U] = STX;
    bufsz = 1024U;
#else
    xbuff[0U] = SOH;
    bufsz = 128U;
#endif
	
    /* Wait for receiving the Transfer request */
	for (retry = 0U; retry < MAXRETRANS; retry++)
	{
		/* read the data */
		DrvUart_ReadBytes(&recvBuff[0U], 1U);

		/* parse the message */
		switch (recvBuff[0U])
		{
			case 'C':
				crc = 1U;
				requestReceived = 1U;
                printf ("\nRequest received for CRC based packets\n");
                printf ("Download Starting\n");
				break;
			case NAK:
				crc = 0U;
				requestReceived = 1U;
				break;
			case CAN:
				DrvUart_ReadBytes(&recvBuff[0U], 1U);
				if (recvBuff[0U] == CAN)
				{
					/* send confirmation */
					DrvUart_WriteBytes(&ackMsg, 1U);
					retVal = -1;
				}
				break;
			default:
				break;
		}
        
        /* break the loop if the request or cancel command is received */
		if ((retVal != 0) || (requestReceived == 1U))
		{
			break;
		}
	}

	/* check if it went through all retries */
	if ((requestReceived == 0U) && (retVal == 0))
	{
		/* send the cancel command */
		DrvUart_WriteBytes(&cancMsg, 1U);
		DrvUart_WriteBytes(&cancMsg, 1U);
		retVal = -2;
	}
    
    /* loop to send the data until the end of file or some error condition */
	while (1U)
	{
		if (retVal != 0U)
		{
			break;
		}

		dataRead = 0U;
		ackReceived = 0U;
        
        /* add the packet number to the buffer */
		xbuff[1U] = packetno;
		xbuff[2U] = ~packetno;
        
        /* read the data from file into the buffer */
		dataRead = fread(&xbuff[3U], 1U, bufsz, ptr);

        /* if data is read, send the data. Otherwise try to send EOT */
		if (dataRead > 0U)
		{
            /* if the data is less than the chunk size, pad the rest with 0x1A */
			if (dataRead < bufsz)
			{
				for (i = 0U; i < (bufsz - dataRead) ; i++)
				{
					xbuff[3U + dataRead + i] = CTRLZ;
				}
			}
            
            /* check if the CRC is to be used or CRC */
			if (crc) {
                /* calculate CRC and add it to buffer */
            #if (FAST_CRC == 1U)
                uint16_t ccrc = crcValues[packetno - 1];
            #else
				uint16_t ccrc = crc16_ccitt(&xbuff[3U], bufsz);
            #endif
				xbuff[bufsz + 3U] = (ccrc >> 8U) & 0xFF;
				xbuff[bufsz + 4U] = ccrc & 0xFF;
				packetSize = bufsz + 5U;
			}
			else {
                /* calculate checksum and add it to buffer */
				uint8_t ccks = 0U;
				for (i = 3U; i < (bufsz + 3U); i++) {
					ccks += xbuff[i];
				}
				xbuff[bufsz + 3U] = ccks;
				packetSize = bufsz + 4U;
			}
            
            /* try to send the buffer created */
			for (retry = 0U; retry < MAXRETRANS; retry++)
			{
                #if (PHYSICAL_MEDIUM_DISCONNECT == 1U)
                packetSize = 10U;
                #endif

				/* send the message packet */
				DrvUart_WriteBytes(&xbuff[0U], packetSize);

				/* read the data */
				DrvUart_ReadBytes(&recvBuff[0U], 1U);

				switch (recvBuff[0U])
				{
					case ACK:
                        /* ACK is received as expected, set ackReceived */ 
						packetno = packetno + 1U;
						len = len + bufsz;
						ackReceived = 1U;
                        
                        #if (CANCEL_ONGOING_TRANSFER == 1U)
                        /* send cancel message */
                        DrvUart_WriteBytes(&cancMsg, 1U);
                        /* read the data */
                        DrvUart_ReadBytes(&recvBuff[0U], 1U);
                        retVal = -5;
                        #endif
                        
						break;
					case CAN:
                        /* Cancel command is received, set the error code */ 
						DrvUart_ReadBytes(&recvBuff[0U], 1U);
						if (recvBuff[0U] == CAN)
						{
							/* send confirmation */
							DrvUart_WriteBytes(&ackMsg, 1U);
							retVal = -3;
						}
						break;
					case NAK:
                        /* NAK is received, retry the transmission */
					default:
						break;
				}
                
                /* break out of loop if either the data chunk is acked or if
                 * cancel command is received
                 */
				if ((retVal != 0U) || (ackReceived == 1U))
				{
					break;
				}
			}

			/* check if it went through all retries */
			if ((retVal == 0U) && (ackReceived == 0U))
			{
				/* send the cancel command */
				DrvUart_WriteBytes(&cancMsg, 1U);
				DrvUart_WriteBytes(&cancMsg, 1U);
				break;
			}
		}
		else
		{
            /* try to send the EOT */
			for (retry = 0U; retry < MAXRETRANS; retry++)
			{                
				/* send the EOT Msg */
				DrvUart_WriteBytes(&eotMsg, 1U);
                printf ("EOT sent\n");
				DrvUart_ReadBytes(&recvBuff[0U], 1U);
                
                /* break out of loop if EOT is Acked */
				if (recvBuff[0U] == ACK)
				{
                    printf ("Transmission successfull\n");
					break;
				}
			}
            
            /* check if all the retries are exhausted. If yes, set the error code */
			if (retry == 10U)
			{
				retVal = -4;
			}
			else
			{
                /* transmission successful, break the loop */
				break;
			}
		}
	}
	return retVal;
}

uint8_t xbuff[1030U]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */

int32_t xmodemTransmitBuff (uint8_t *ptrSocIdBuff, uint32_t *ptrSocIdSize, uint8_t* ptrImageBuff, uint32_t imageSize)
{
	uint32_t bufsz, crc = 0U;
	uint8_t packetno = 1U;
	uint32_t packetSize = 0U;
	uint8_t recvBuff[1U] = { 0U };
	uint8_t requestReceived = 0U;
	uint8_t ackReceived = 0U;
	uint8_t ackMsg = ACK;
	uint8_t cancMsg = CAN;
	uint8_t eotMsg = EOT;
	int32_t retVal = 0U;
	uint32_t i, len = 0U;
	uint32_t retry;
    uint32_t dataPending = imageSize;

    /* check if we are sending 1024 bytes or 128 bytes chunk */
#if (XMODEM_1K == 1U)
    xbuff[0U] = STX;
    bufsz = 1024U;
#else
    xbuff[0U] = SOH;
    bufsz = 128U;
#endif

#if (FAST_CRC == 1U)
    /* calculate all the CRCs beforehand to avoid delay between packets */
    uint32_t totalData = imageSize;
    uint32_t numPackets = 0U;
    
    while(totalData > 0U)
    {
        /* if the pending data is less than the chunk size, pad the rest with 0x1A */
        if (totalData < bufsz)
        {
            /* read the data from Image buffer into packet buffer */
            memcpy(&xbuff[3U], &ptrImageBuff[len], totalData);
            for (i = 0U; i < (bufsz - totalData) ; i++)
            {
                xbuff[3U + totalData + i] = CTRLZ;
            }
            totalData = 0U;
            XMODEM_ASSERT(numPackets < UTILS_ARRAYSIZE(crcValues));
			crcValues[numPackets] = crc16_ccitt(&xbuff[3U], bufsz);
        }
        else
        {
            XMODEM_ASSERT(numPackets < UTILS_ARRAYSIZE(crcValues));
            crcValues[numPackets] = crc16_ccitt(&ptrImageBuff[len], bufsz);
            totalData = totalData - bufsz;
        }
        len = len + bufsz;
        numPackets = numPackets + 1U;     
    }
    len = 0U;
#endif

    /* Wait for receiving the Transfer request */
	for (retry = 0U; retry < 4096; retry++)
	{
		/* read the data */
		DrvUart_ReadBytes(&recvBuff[0U], 1U);

		/* parse the message */
		switch (recvBuff[0U])
		{
			case 'C':
				crc = 1U;
				requestReceived = 1U;
                *ptrSocIdSize = retry + 1U;
                printf ("\nRequest received for CRC based packets\n");
                printf ("Download Starting\n");
				break;
			case NAK:
				crc = 0U;
				requestReceived = 1U;
				break;
			case CAN:
				DrvUart_ReadBytes(&recvBuff[0U], 1U);
				if (recvBuff[0U] == CAN)
				{
					/* send confirmation */
					DrvUart_WriteBytes(&ackMsg, 1U);
					retVal = -1;
				}
				break;
			default:
                ptrSocIdBuff[retry] = recvBuff[0U];
				break;
		}
        
        /* break the loop if the request or cancel command is received */
		if ((retVal != 0) || (requestReceived == 1U))
		{
			break;
		}
	}

	/* check if it went through all retries */
	if ((requestReceived == 0U) && (retVal == 0))
	{
		/* send the cancel command */
		DrvUart_WriteBytes(&cancMsg, 1U);
		DrvUart_WriteBytes(&cancMsg, 1U);
		retVal = -2;
	}
    
    /* loop to send the data until the end of file or some error condition */
	while (1U)
	{
		if (retVal != 0U)
		{
			break;
		}

		ackReceived = 0U;
        
        /* add the packet number to the buffer */
		xbuff[1U] = packetno;
		xbuff[2U] = ~packetno;

        /* if data is pending, send the data. Otherwise try to send EOT */
		if (dataPending > 0U)
		{
            /* if the pending data is less than the chunk size, pad the rest with 0x1A */
			if (dataPending < bufsz)
			{
                /* read the data from Image buffer into packet buffer */
                memcpy(&xbuff[3U], &ptrImageBuff[len], dataPending);
                for (i = 0U; i < (bufsz - dataPending) ; i++)
				{
					xbuff[3U + dataPending + i] = CTRLZ;
				}
                dataPending = 0U;
            }
            else
            {
                /* read the data from Image buffer into packet buffer */
                memcpy(&xbuff[3U], &ptrImageBuff[len], bufsz);
                dataPending = dataPending - bufsz;
            }
            
            /* check if the CRC is to be used or CRC */
			if (crc)
            {
                /* calculate CRC and add it to buffer */
            #if (FAST_CRC == 1U)
                uint16_t ccrc = crcValues[packetno - 1];
            #else
				uint16_t ccrc = crc16_ccitt(&xbuff[3U], bufsz);
            #endif
				xbuff[bufsz + 3U] = (ccrc >> 8U) & 0xFF;
				xbuff[bufsz + 4U] = ccrc & 0xFF;
				packetSize = bufsz + 5U;
			}
			else
            {
                /* calculate checksum and add it to buffer */
				uint8_t ccks = 0U;
				for (i = 3U; i < (bufsz + 3U); i++) {
					ccks += xbuff[i];
				}
				xbuff[bufsz + 3U] = ccks;
				packetSize = bufsz + 4U;
			}
            
            /* try to send the buffer created */
			for (retry = 0U; retry < MAXRETRANS; retry++)
			{
            #if (PHYSICAL_MEDIUM_DISCONNECT == 1U)
                if ((packetno == 2U) && (retry == 0U))
                {
                    packetSize = 10U;
                }
                else
                {
                    if (crc)
                    {
                        packetSize = bufsz + 5U;
                    }
                    else
                    {
                        packetSize = bufsz + 4U;
                    }
                }
            #endif
				/* send the message packet */
				DrvUart_WriteBytes(&xbuff[0U], packetSize);

				/* read the data */
				DrvUart_ReadBytes(&recvBuff[0U], 1U);

				switch (recvBuff[0U])
				{
					case ACK:
                        /* ACK is received as expected, set ackReceived */ 
						packetno = packetno + 1U;
						len = len + bufsz;
						ackReceived = 1U;
                    #if (PRINT_PROGRESS == 1U)
                        printProgress (len, imageSize);
                    #endif
                        
                    #if (CANCEL_ONGOING_TRANSFER == 1U)
                        /* send cancel message */
                        DrvUart_WriteBytes(&cancMsg, 1U);
                        /* read the data */
                        DrvUart_ReadBytes(&recvBuff[0U], 1U);
                        retVal = -5;
                    #endif
                        
						break;
					case CAN:
                        /* Cancel command is received, set the error code */ 
						DrvUart_ReadBytes(&recvBuff[0U], 1U);
						if (recvBuff[0U] == CAN)
						{
							/* send confirmation */
							DrvUart_WriteBytes(&ackMsg, 1U);
							retVal = -3;
						}
						break;
					case NAK:
                        printf ("NAK received\n");
                        /* NAK is received, retry the transmission */
                        break;
					default:
                        printf ("Unexpected response 0x%x\n", recvBuff[0U]);
						break;
				}
                
                /* break out of loop if either the data chunk is acked or if
                 * cancel command is received
                 */
				if ((retVal != 0U) || (ackReceived == 1U))
				{
					break;
				}
			}

			/* check if it went through all retries */
			if ((retVal == 0U) && (ackReceived == 0U))
			{
				/* send the cancel command */
				DrvUart_WriteBytes(&cancMsg, 1U);
				DrvUart_WriteBytes(&cancMsg, 1U);
				break;
			}
		}
		else
		{
            /* try to send the EOT */
			for (retry = 0U; retry < MAXRETRANS; retry++)
			{               
				/* send the EOT Msg */
				DrvUart_WriteBytes(&eotMsg, 1U);
                printf ("EOT sent\n");
				DrvUart_ReadBytes(&recvBuff[0U], 1U);
                
                /* break out of loop if EOT is Acked */
				if (recvBuff[0U] == ACK)
				{
                    printf ("Transmission successfull\n");
					break;
				}
			}
            
            /* check if all the retries are exhausted. If yes, set the error code */
			if (retry == 10U)
			{
				retVal = -4;
			}
			else
			{
                /* transmission successful, break the loop */
				break;
			}
		}
	}
	return retVal;
}

