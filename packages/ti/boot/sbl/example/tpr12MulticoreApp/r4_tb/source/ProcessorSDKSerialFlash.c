/**
 * \file  ProcessorSDKSerialFlash.cpp
 *
 * \brief Command Line Application for File Transfer over UART
 */
/*
* Copyright (C) 2017-2019 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
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
#include "../include/ProcessorSDKSerialFlash.h"
#include "uart.h"
#include "stdlib.h"
#include "ctype.h"

#define DEBUG 1
#define DEVICE_SUPPORTED 7
#define IMAGETYPE_SUPPORTED 5
#define BUFF_SIZE 20
#define FIND_STRING 2

unsigned int baudRate = BAUDRATE_0;

int devtype = 0, imgtype = 0, addroff = 0, elen = 0, RBL = 0, FW = 1, fdesc = 0, flashErase = 0;
unsigned int offset = 0, def_elen = 0;
char def_itype = '5', def_dtype = '2', def_off = '0';
char device_list[DEVICE_SUPPORTED][BUFF_SIZE]={"NAND", "SPI", "QSPI", "OSPI", "eMMC", "HyperFlash", "UFS"};

unsigned int standardBaudRate[NUM_OF_SUP_BAUDRATE] = { BAUDRATE_0,
                                                       BAUDRATE_1,
                                                       BAUDRATE_2,
                                                       BAUDRATE_3,
                                                       BAUDRATE_4,
                                                       BAUDRATE_5,
                                                       BAUDRATE_6,
                                                       BAUDRATE_7,
                                                       BAUDRATE_8,
                                                       BAUDRATE_9,
                                                       BAUDRATE_10,
                                                       BAUDRATE_11,
                                                       BAUDRATE_12,
                                                       BAUDRATE_13
#ifdef WINDOWS
                                                       ,BAUDRATE_14
#endif                                                       
};

#ifdef WINDOWS	/* Windows APIs */

/**
 * Prints COM port Configuration Parameters.
 *
 * \param dcb specifies the control setting for a serial communications device.
 *
 * This function prints the COM port parameters like BaudRate, ByteSize, Parity
 * and StopBits
 *
 * \return None.
 */

void PrintCommState(DCB dcb)
{
    /*  Print some of the DCB structure values */
    printf("\nBaudRate = %d, ByteSize = %d, Parity = %d, StopBits = %d\n\n", dcb.BaudRate,
              dcb.ByteSize, 
              dcb.Parity,
              dcb.StopBits);
}

SerialPort::SerialPort() {
	serialPortHandle = INVALID_HANDLE_VALUE;
}
 
SerialPort::~SerialPort() {
	if (serialPortHandle!=INVALID_HANDLE_VALUE)
	CloseHandle(serialPortHandle);
	serialPortHandle = INVALID_HANDLE_VALUE;
}

/**
 * Establishes Serial connection with the COM Port
 *
 * \param device specifies the COM Port to be connected.
 *
 * This function establishes serial connection with the COM Port with the defined 
 * dcb parameters.
 *
 * \return Returns 0 on success.
 */

int SerialPort::connect( wchar_t* device, unsigned int baudrate ) {
	int error=0, buf_siz = 1024;
	wchar_t append_str[20] = L"\\\\.\\";
	wchar_t* port_name;
	DCB dcb = {0};
	COMMTIMEOUTS timeouts = {0};
    port_name = wcscat(append_str, device);

	if(*device == NULL)
	{
		cout << "Invalid Device!" << endl;
		return ERR_INVALID;
	}
	serialPortHandle = CreateFileW(port_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (serialPortHandle == INVALID_HANDLE_VALUE)
	{
		cout << "Couldn't Open Serial Port!\n";
		return ERR_INVALID;
	}
	dcb.DCBlength = sizeof(DCB);

	if(!(GetCommState(serialPortHandle, &dcb)))
	{
		cout << "GetComm Failed!" << endl;
		return ERR_COMSTAT;
	}

	dcb.BaudRate = baudrate;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;
	dcb.fParity = 0;
	dcb.XoffChar = 19;
	dcb.XonChar = 17;
	dcb.fOutxCtsFlow = 0;
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = 0;
	dcb.fDsrSensitivity = 0;
	dcb.fTXContinueOnXoff = 0;
	dcb.fRtsControl = 0;
	dcb.XonLim = 0;
	dcb.XoffLim = 0;

	#ifdef DEBUG
	cout << endl << "Configuring COM Port!" << endl;
	#endif
	if(!SetCommState(serialPortHandle,&dcb))
	{
		error=ERR_COMSTAT;
		cout << endl << "Configuration Failed!" << endl;
	}

	GetCommState(serialPortHandle, &dcb);
	#ifdef DEBUG
		PrintCommState(dcb);
	#endif
	if(SetupComm(serialPortHandle, buf_siz, buf_siz))
	{
		#ifdef DEBUG
		cout << "Configuring COM Port success!" << endl;
		#endif
	}
	else
	{
		cout << "Configuring COM Port failed!"  << endl;
	}
	#ifdef DEBUG
		PrintCommState(dcb);
	#endif
	if (error!=0) {
	disconnect();

	}
	else {
		clear();
	}
	return error;
}

/**
 * Closes the established connection with the COM Port
 *
 * \param None
 *
 * This function Closes the established connection with the COM Port
 *
 * \return None
 */

void SerialPort::disconnect(void) {
	CloseHandle(serialPortHandle);
	serialPortHandle = INVALID_HANDLE_VALUE;
}

int SerialPort::sendArray(unsigned char *buffer, int len) {
	unsigned long result =0U;
	if(len <= 0)
	{
		cout << "Read Length cannot be 0!" << endl;
		return ERR_INVALID;
	}

	if (serialPortHandle!=INVALID_HANDLE_VALUE)
		WriteFile(serialPortHandle, buffer, len, (LPDWORD)&result, NULL);
	else
		cout << "Invalid Handle!!" << endl;
	return result;
}

/**
 * Reads the COM port
 *
 * \param buffer points to the buffer to store the input data
 * \param len specifies the length of the input data
 *
 * This function receives input over the COM port.
 *
 * \return Returns the length of the data received.
 */

int SerialPort::getArray (unsigned char *buffer, int len) {
	unsigned long read_nbr;
	DWORD dwRead;
	OVERLAPPED osReader = {0};
	BOOL fWaitingOnRead = FALSE; 
	read_nbr = 0;

	if( len <= 0 )
	{
		cout << "SerialPortGet: Invalid input parameters!" << len << endl;
		return ERR_INVALID;
	}

	if (!fWaitingOnRead) {
	   /* Issue read operation */
	   if (!ReadFile(serialPortHandle, buffer, len, &dwRead, &osReader)) {
		  if (GetLastError() != ERROR_IO_PENDING)
			 /* Error in communications */
			 cout <<"Error in reading!!" << endl;
		  else
			 fWaitingOnRead = TRUE;
	   }
	}
	return((int) dwRead);
}

/**
 * Terminates pending read or write operations
 *
 * \param None
 *
 * This function Discards all characters from the output or input 
 * buffer on the Serial Port
 *
 * \return None.
 */

void SerialPort::clear() {
	PurgeComm (serialPortHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

/**
 * Opens the source file
 *
 * \param fname points to the absolute path to the file
 * \param fsize points to the size of the file
 *
 * This function opens the source file to be transferred over 
 * COM Port
 *
 * \return Returns the handle to the file.
 */

ifstream file_open( char *fname, int *fsize)
{
    ifstream ifile;

#ifdef DEBUG
    cout << endl << "Opening file " << fname << endl;
#endif
    ifile.open(fname, ios::binary);	/* Open the file to be transfered */
    if(!ifile)
    {
        cout<<"Error in opening file..!!";
        exit(0);
    }

    ifile.seekg (0, ifile.end);
    *fsize = ifile.tellg();
    ifile.seekg (0, ifile.beg);
    return ifile;
}
#elif (defined TB_UNIFLASH)

#else/* Linux APIs */

FILE *file;

/**
 * Serial Port Setup
 *
 * \param fd points to the file descriptor of the Serial Port
 * \param speed specifies the Bps of the Serial Port
 * \param parity specifies the Parity of the Serial Port
 *
 * This function configures the Serial Port with given Parameters
 *
 * \return Returns 0 on success and error value on failure.
 */

int set_interface_attribs (int fd, int speed, int parity)
{
    int error = 0;
    struct termios tty;
    memset (&tty, 0, sizeof(tty));
    if (tcgetattr (fd, &tty) != 0)
    {
        cout << "Error " << strerror(errno) << "from tcgetattr\n";
        error = errno;
        return error;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     /* 8-bit chars */
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); /* shut off xon/xoff ctrl */
    tty.c_cflag &= ~(PARENB | PARODD);      /* shut off parity */
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_oflag = 0;
    tty.c_lflag = 0;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        cout << "error " << errno << "from tcsetattr";
        error = errno;
        return error;
    }
    return error;
}

/**
 * Serial Port Read 
 *
 * \param buff points to the buffer to get the input byte
 *
 * This function reads a byte from Serial port
 *
 * \return Returns the number of bytes read.
 *
 */

int readport(char *buff, int fd)
{
    int n;
    while (1) { 
        n = read(fd, buff, 1);
        if (n == -1) 
        switch(errno) {
            case EAGAIN: /* sleep() */ 
                continue;
            default: break;
        }
        if (n ==0) break;
        return n;
    }
}

#endif
 
/**
 * CRC calculation
 *
 * \param buf points to the buffer that contains the data
 * \param len specifies the length of the data
 *
 * This function calculates CRC for each and every packet to 
 * be transmitted over the Serial interface.
 *
 * \return Returns the calculated CRC value.
 */
unsigned short crc16_ccitt(unsigned char *buf, int len)
{
    int crc;
    char i;
    crc = 0;

    while (--len >= 0)
    {
        crc = crc ^ (int) *buf++ << 8;
        i = 8;
        do
        {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
            } while(--i);
    }
    return (crc);
}

/**
 * Xmodem File Transmit
 *
 * \param destname points to the COM Port to be connected
 * \param sname points to the file to be transferred over the COM port
 *
 * This function Transfers the file over UART using XMODEM protocol
 *
 * \return Returns 0 on success and error value on failure.
 *
 */
int xmodemFTransfer(char *destname, char *sname)
{
    unsigned char rxBuff[1];
    unsigned char txBuff[1030], src[1030], packetno = 1, ack = ACK, EoT = EOT;
    int i = 0, c =0, len = 0, retry = 0, bufsz = 1, crc = -1, error = 0, ktbytes = 1024;
    int srcsz = 0, maxretry = 25, inbyte = 1, txretry =16, eotretry = 20;

    if((*destname || *sname ) == NULL)
    {
        printf("xmodemFTransfer: Invalid input parameters!\n");
        //DrvUart_Close();
        return ERR_INVALID;
    }

    srcsz = *(int *)TB_FLASHIMAGE_LENGTH_ADDR;

    /* Wait to start transfer:
      -"C" - ping character to start sending
      - NAK - start sending ?
      - CAN - wait for another CAN command then ack and return
     */
    for( retry = 0; retry < txretry; ++retry) {

        /* Read 1 byte from serial interface - blocking call */
        DrvUart_ReadBytes(&rxBuff[0U], 1U);
        switch (rxBuff[0])
        {
            case 'C':
                if( FW == 1 )
                    printf("\nFlashing Image of size %d bytes\n " , srcsz );
                else
                    printf("\nTransferring File of size%d bytes\n " , srcsz );

                crc = 1;

                goto start_trans;
            case NAK:
#ifdef DEBUG
                printf("In case NAK\n");
#endif
                crc = 0;
                goto start_trans;
            case CAN:
#ifdef DEBUG
                printf("In case CAN\n");
#endif
                DrvUart_ReadBytes(&rxBuff[0U], 1U);
                {
                    if (rxBuff[0] == CAN) {
                        /* Send ack */
                        DrvUart_WriteBytes(&ack, 1U);
                        printf("Failure: Canceled by remote !!\n");
                        error = ERR_RCAN; /* canceled by remote */
                        return error;
                    }
                }
                break;
            default:
#ifdef DEBUG
                printf("In Default, rxbuff=%d\n", rxBuff[0]);
#endif
                /* Not supported command, get another char */
                break;
        }
    }

    /* Faillure after number of tries , exit with error */
    printf("\nFailure:Unknown response from target! ");

    DrvUart_WriteBytes(&ack, 1U);
    printf("Please check the connection!\n");
    error = ERR_NOSYNC; /* no sync */

    //DrvUart_Close();
    return error;

    for(;;)
    {
    
start_trans:
        /* Running in a loop until all data are transferred */
        txBuff[0] = STX; bufsz = ktbytes;
        txBuff[1] = packetno;
        txBuff[2] = ~packetno;
        c = srcsz - len;
        if (c > bufsz) c = bufsz;

#ifdef WINDOWS
            ifile.read((char*)src,c);
#elif (defined TB_UNIFLASH)
        /* Test Bench: copy data to be sent over UART */
        if(c >0)
        {
            memcpy((void *)src, (void *)(TB_FLASHIMAGE_ADDR +len), c);
        }
#else
        fread(src, inbyte, c, file);
#endif
        if (c > 0) {
            /* There are data to sent to target */
            memset (&txBuff[3], 0, bufsz);
            if (c == 0) 
            {
                txBuff[3] = CTRLZ;
            }
            else 
            {
                printf("%d complete\r", (((len+ c)*100)/srcsz) );

                /* Copy available data */
                memcpy (&txBuff[3], &src[0], c);

                /* if last packet , mark EOT */
                if (c < bufsz)
                {
                    txBuff[3+c] = CTRLZ;
                }
            }
            if (crc)
            {
                unsigned short ccrc = crc16_ccitt(&txBuff[3], bufsz);
                if( crc < 0 )
                {
                    //DrvUart_Close();
                    return crc;
                }
                txBuff[bufsz+3] = (ccrc>>8) & 0xFF;
                txBuff[bufsz+4] = ccrc & 0xFF;
            }
            else {
                unsigned char ccks = 0;
                for (i = 3; i < bufsz+3; ++i)
                {
                    ccks += txBuff[i];
                }
                txBuff[bufsz+3] = ccks;
            }

            /* Buffer is filled, ready to sent to UART */
            for (retry = 0; retry < maxretry; ++retry) {
                unsigned int tx_len;
                tx_len = bufsz+4+(crc?1:0);

                /* Send tx buffer to target */
                DrvUart_WriteBytes(&txBuff[0], tx_len);

                /* Read response from target - skip 'C' */
                do {
                    DrvUart_ReadBytes((uint8_t*)rxBuff, inbyte);
                } while(rxBuff[0] == 'C');

                switch (rxBuff[0])
                {
                    case ACK:
                        ++packetno;
                        len += bufsz;
                        goto start_trans;

                    case CAN:
                        DrvUart_ReadBytes((uint8_t*)rxBuff, inbyte);
                        if (rxBuff[0] == CAN)
                        {

                            DrvUart_WriteBytes(&ack, 1U);

                            printf("\nFailure: Canceled by remote!!\n");
                            //DrvUart_Close();
                            error = ERR_RCAN; 
                            return error; /* canceled by remote */
                        }
                        break;

                    case FLASH_SUCCESS:
                        printf("\nFlash Programming successful!!\n");

                        DrvUart_WriteBytes(&ack, 1U);
                        break;
                    case FLASH_FAILURE:
                        printf("\nFlash Programming Failed!!\n");

                        DrvUart_WriteBytes(&ack, 1U);
                        break;

                    case NAK:
                    default:
                        break;
                }
            }

            //DrvUart_Close();
            error = ERR_XMIT; 
            return error; /* xmit error */
        }
        else 
        {

            /* No more data to send -send EOT */
            error = 0;

            for (retry = 0; retry < eotretry; ++retry) 
            {
#ifdef DEBUG
                printf("Sending EOT\n");
#endif
                DrvUart_WriteBytes(&EoT, 1U);

                DrvUart_ReadBytes((uint8_t*)rxBuff, inbyte);

                if ( rxBuff[0] == ack)
                {
                    #ifdef DEBUG
                        printf("Rcvd ACK for EOT\n");
                    #endif
                    if( FW == 0 )
                        printf("File Transfer complete!\n");

                    break;
                }
            }

            /* Wait for flash programming result */
            if( FW == 1 )
            {
                for(;;)
                {
                    DrvUart_ReadBytes((uint8_t*)rxBuff, inbyte);

                    if ( rxBuff[0] == FLASH_SUCCESS)
                    {
                        printf("\rFlash Programming Success!\n");
                        rxBuff[0] = ack;
                        DrvUart_WriteBytes(&ack, 1U);
                        break;

                    }
                    else if ( rxBuff[0] == FLASH_FAILURE)
                    {
                        printf("\nFlash Programming Failed!!\n");
                        DrvUart_WriteBytes(&ack, 1U);
                        break;
                    }
                }
            }
            goto conn_close;
        }
    }
conn_close:
    //DrvUart_Close();

    error = (rxBuff[0] == ack)?0:ERR_NOACK;
    return error;
}

/**
 * Xmodem Header Transmit
 *
 * This function request a baudrate change over UART using XMODEM protocol.
 *
 * \return Returns 0 on success and error value on failure.
 *
 */
int getMaxBaudRate( unsigned char *maxBaudrate)
{
    char rxBuff[1];
    unsigned char ack = ACK, EoT = EOT, nSupported = NOT_SUPPORTED_BAUDRATE;
    int error = 0, inbyte = 1, hsize = 12;
    unsigned short ccrc;
    unsigned char command[14];
    const char start = 'S', getCmd = GET_MAX_BAUDRATE_CMD;
    unsigned short i;

    DrvUart_Open(0x6B);

    memset (command, 0, sizeof(command));
    memcpy (command, &start, 1);
    memcpy (&command[1], &getCmd, 1);

    for (;;){

        DrvUart_ReadBytes((uint8_t *)rxBuff, 1U);
        if (rxBuff[0] == 'C')
        {
            ccrc = crc16_ccitt(command, hsize);
            command[hsize] = (ccrc>>8) & 0xFF;
            command[hsize+1] = ccrc & 0xFF;

            DrvUart_WriteBytes(command, (hsize+2));

            for(;;)
            {
                DrvUart_ReadBytes((uint8_t *)rxBuff, 1U);
                if (rxBuff[0] == ACK)
                {
                    for (i=0; i<2; i++) 
                    { //Maximum supported baudrate can be get maximum of 2 times
                        DrvUart_ReadBytes((uint8_t *)rxBuff, 1U);
                        /* Currently, BAUDRATE_6000000 is the maximum supported baudrate supported */
                        if (rxBuff[0] <= BAUDRATE_6000000)
                        {
                            if (rxBuff[0] <= MAX_SUPPORTED_BAUDRATE)
                                break;
                            else
                            {
                                DrvUart_WriteBytes(&nSupported, inbyte);
                            }
                        }
                        else
                        {
                            DrvUart_ReadBytes((uint8_t *)rxBuff, inbyte);
                            if (rxBuff[0] == CAN) {
                                DrvUart_WriteBytes(&ack, inbyte);

                                printf("\nFailure: Canceled by remote!!\n");
                                error = ERR_RCAN;
                                //DrvUart_Close();
                                return error; /* canceled by remote */
                            }
                        }
                    }

                    if (i < 2)
                    { //checks whether a valid baudrate received within 2times
                        *maxBaudrate = rxBuff[0];
                        DrvUart_WriteBytes(&ack, inbyte);
                        DrvUart_WriteBytes(&EoT, inbyte);

                        DrvUart_ReadBytes((uint8_t *)rxBuff, inbyte);

                        if (rxBuff[0] != ack)
                        {
                            error = ERR_RCAN;
                            //DrvUart_Close();
                            return error; /* canceled by remote */
                        }
                        else
                        {
                            return error;
                        }
                    }
                    else //else if CAN
                    {
                        DrvUart_ReadBytes((uint8_t *)rxBuff, 1U);
                        if (rxBuff[0] == CAN)
                        {
                            DrvUart_WriteBytes((uint8_t *)&ack, inbyte);

                            printf("\nFailure: Canceled by remote!!\n");
                            error = ERR_RCAN;
                            //DrvUart_Close();
                            return error; /* canceled by remote */
                        }
                    }
                }
                else
                {
                    DrvUart_ReadBytes((uint8_t *)rxBuff, 1U);
                    if (rxBuff[0] == CAN)
                    {
                        DrvUart_WriteBytes((uint8_t *)&ack, inbyte);
                        printf("\nFailure: Canceled by remote!!\n");
                        error = ERR_RCAN;
                        //DrvUart_Close();
                        return error; /* canceled by remote */
                    }
                }
            }
        }
    }
}


/**
 * Xmodem Header Transmit
 *
 * \param src points to the Header packet to be transferred over the COM port
 *
 * This function Transfers the Header over UART using XMODEM protocol.
 * The Header contains information about the file to be tranfered to the Flash Writer 
 * program
 *
 * \return Returns 0 on success and error value on failure.
 *
 */
int xmodemHTransfer( unsigned char *src)
{
    char rxBuff[1], ack = ACK, EoT = EOT;
    int error = 0, retry = 0, txretry = 16, inbyte = 1, hsize = 12;
    unsigned short ccrc;
    int idx;

    if(( *src ) == NULL)
    {
        printf("xmodemHTransfer: Invalid input parameters!\n");
        return ERR_INVALID;
    }

    for(idx=0;idx<100;idx++) {
        DrvUart_ReadBytes((uint8_t *)rxBuff, 1U);
        switch (rxBuff[0])
        {
            case 'C':
                ccrc = crc16_ccitt(src, hsize);
                src[hsize] = (ccrc>>8) & 0xFF;
                src[hsize+1] = ccrc & 0xFF;

                DrvUart_WriteBytes(src, (hsize+2));
                DrvUart_ReadBytes((uint8_t *)rxBuff, 1U);

                switch (rxBuff[0])
                {
                    case ACK:
                        for (retry = 0; retry < txretry; ++retry) 
                        {
                            DrvUart_WriteBytes((uint8_t *)&EoT, inbyte);

                            DrvUart_ReadBytes((uint8_t *)rxBuff, 1U);
                            if ( rxBuff[0] == ack)
                            {
#ifdef DEBUG
                                printf("Rcvd ACK for EOT\n");
#endif
                                break;
                            }
                        }
                        break;
                    case CAN:
                        DrvUart_ReadBytes((uint8_t *)rxBuff, 1U);
                        if (rxBuff[0] == CAN)
                        {
                            DrvUart_WriteBytes((uint8_t *)&ack, inbyte);
                            printf("\nFailure: Canceled by remote!!\n");
                            error = ERR_RCAN;
                            //DrvUart_Close();
                            return error; /* canceled by remote */
                        }
                        break;
                    case NAK:
                    default:
                        printf("NAK: Transmit of header Failed\n");
                        error = ERR_XMIT;
                        break;
                }
                break;

            default:
                break;;
        }
        break;
    }

    if(flashErase && (!error))
    {
        //printf("Header Transfer Complete!!\n");
        for(;;) /* Wait for Flash Erase to complete */
        {
            DrvUart_ReadBytes((uint8_t *)rxBuff, 1U);
            if ( rxBuff[0] == FLASH_SUCCESS)
            {
                printf("Flash Erase Success!\n");
                DrvUart_WriteBytes((uint8_t *)&ack, inbyte);

            }
            else if ( rxBuff[0] == FLASH_FAILURE)
            {
                printf("Flash Erase Failed!!\n");
                DrvUart_WriteBytes((uint8_t *)&ack, inbyte);
            }
            break;
        }
    }
    return error;
}

/**
 * Input Validation
 *
 * \param argval contains the input argument to be validated
 *
 * This function checks for the valid Command Line Arguments
 *
 * \return Returns 0 on success and error value on failure.
 *
 */
int validate( const char *argval )
{
    int error = 0, dtype = 0, itype = 0;
    /* Validate the Command Line Arguments recived */
    if( devtype == 1 )
    {
        dtype = atoi( argval);
        if( (dtype<0) || (dtype>=DEVICE_SUPPORTED) || !isdigit((int) (*argval)) )
        {
            displayVersion();
            printf("Invalid Device Type!!\n");
            devtype++;
            displayHelp();
        }
        devtype++;
        memcpy(&def_dtype, argval, 1);
    }
    if( imgtype == 1 )
    {
        itype = atoi( argval );
        if( (itype<0) || (itype>IMAGETYPE_SUPPORTED) || !isdigit((int) (*argval)) )
        {
            displayVersion();
            printf("Invalid Image Type!!\n");
            imgtype++;
            displayHelp();
        }
        imgtype++;
        if( itype == 0 )
            RBL = 1;
        memcpy(&def_itype, argval, 1);
    }
    if( addroff == 1 )
    {
        offset = strtol( argval, NULL, 16 );
        if( !isxdigit((int)*argval) )
        {
            displayVersion();
            printf("Invalid Offset Value!!\n");
            addroff++;
            displayHelp();
        }
        addroff++;
    }
    if( elen == 1 )
    {
        def_elen = strtol( argval, NULL, 16 );
        if(!isxdigit((int) (*argval)) )
        {
            displayVersion();
            printf("Invalid Erase Length!!\n");
            elen++;
            displayHelp();
        }
        elen++;
    }
    return error;
}

/**
 * Input Parser
 *
 * \param cliarg contains the parser command
 * \param cliargval contains the input argument to be validated
 *
 * This function parses the Command Line Argument and validates it
 *
 * \return Returns 0 on success and error value on failure.
 *
 */
 int chparam( const char *cliarg, const char *cliargval)
{
    int err = 0;
    if( !(strcmp(cliarg,"-d")) )
    {
        if( devtype > 0 )
        {
            displayVersion();
            printf("\nDuplication of Device Type Detected!!");
            err = -1;
            return err;
        }
        else
            devtype++;
        validate(cliargval);
    }
    else if( !(strcmp(cliarg,"-i")) )
    {
        if( imgtype > 0 )
        {
            displayVersion();
            printf("\nDuplication of Image Type Detected!!");
            err = -1;
            return err;
        }
        else
            imgtype++;
        validate(cliargval);
    }
    else if( !(strcmp(cliarg,"-o")) )
    {
        if( addroff > 0 )
        {
            displayVersion();
            printf("\nDuplication of Offset Detected!!");
            err = -1;
            return err;
        }
        else
            addroff++;
        validate(cliargval);
    }
    else if( !(strcmp(cliarg,"-e")) )
    {
        if( elen > 0 )
        {
            displayVersion();
            printf("\nDuplication of Erase Length Detected!!");
            err = -1;
            return err;
        }
        else
            elen++;
        validate(cliargval);
    }
    else if( !(strcmp(cliarg,"-f")) )
    {
        err = 0;
    }
    else
    {
        err = -1;
    }
    return err;
}

/**
 * FlashErase
 *
 * \param optionNames points to the options available
 * \param optionValues points to the values of the options
 * \param optionLen points to the number of the options 
 *
 * This function erases the flash memory starting from the 
 * offset value till the erase length
 *
 * \return Returns 0 on success and error value on failure.
 *
 */
int FlashErase( const char** optionNames, const char** optionValues, const int optionLen)
{
    const char **optInName, **optInVal;
    int err = 0, i = 0;
    unsigned char header[16];
    const char start = 'S', erase = ERASE_CMD;

    flashErase++;

    for( i = 0; i < (optionLen-2) ; i+=2 )
    {
        optInName = (&(*optionNames))+i;
        optInVal = (&(*optionValues))+i;
        if(!(strcmp( *optInName,"-c" )))
        {
        }
        else
        {
            if( chparam(*optInName, *optInVal) != 0)
                displayHelp();
        }
    }

    /* Open default port */
    DrvUart_Open(0x6B);

    /* Create Header Packet */
    memset (header, 0, sizeof(header));
    memcpy(header, &start, 1);
    memcpy(&header[1], &erase, 1);
    memcpy(&header[2], &def_dtype, 1);
    memcpy(&header[3], &def_itype, 1);
    memcpy(&header[4], &offset, 4);
    memcpy(&header[8], &def_elen, 4);
    //displayVersion();
    printf("\nErasing Flash....\n");

#ifdef DEBUG
    printf("Header %d %d %d %d ", header[0], header[1], header[2], header[3]);
#endif

    printf("\nTransferring Header information..\n");

    err = xmodemHTransfer(header);

    if(!err)
    {
        printf("Flash erase Complete!\n");
    }
    else
    {
        printf("Flash erase Failed!\n");
    }


    //DrvUart_Close();
    flashErase = 0;
    return err;
}

/**
 * LoadImage
 *
 * \param imagePath points the path to the image file to be opened
 * \param optionNames points to the options available
 * \param optionValues points to the values of the options
 * \param optionLen points to the number of the options 
 *
 * This function loads the image from the imagePath to the flash device
 *
 * \return Returns 0 on success and error value on failure.
 *
 */

int LoadImage( const char * imagePath, const char** optionNames, const char** optionValues, const int optionLen )
{
    const char **optInName, **optInVal;
    int err = 0, i = 0;
    unsigned char header[16];
    unsigned char supBaudRate = 0;
    const char start = 'S', program = PROGRAM_CMD, setBaudrate = SET_BAUDRATE_CMD;

#ifdef DEBUG
    if( optionValues == NULL )
        printf("No option values!\n");
#endif
    for( i = 0; i < (optionLen-2) ; i+=2 )
    {
        optInName = (&(*optionNames))+(i);
        optInVal = (&(*optionValues))+(i);
        {
            if( chparam(*optInName, *optInVal) != 0)
                displayHelp();
        }
    }
    if((RBL == 1) | (optionLen < 6))
    {
        displayVersion();
        printf("\nDownloading Flash Programmer..\n");
        RBL = 1;
        FW = 0;
    }
    else
    {
        displayVersion();
        FW = 1;
        printf("\nTransferring the Image to Flash Programmer..\n");
        if( addroff == 0 )
            offset = strtol( &def_off, NULL, 16 );
    }
#ifdef DEBUG
    printf("Device Type-%c \n", def_dtype);
    printf("ImageType-%c\n", def_itype);
    printf("Offset-%d\n", offset);
#endif

    /* Open default port */
    DrvUart_Open(0x6B);

    if( RBL )/* Transfer Flash Writer to RBL */
    {
FileTx:
        err =xmodemFTransfer( "com1", (char *)imagePath);

#if 0//Not needed for TPR12
        /* System Firmware Transfer */
        {
            /* Create Header Packet */
            def_itype = FIRMWARE_DEVTYPE; /* Image Type is Firmware */
            memset (header, 0, sizeof(header));
            memcpy(header, &start, 1);
            memcpy(&header[1], &program, 1);
            memcpy(&header[3], &def_itype, 1);
#ifdef DEBUG
            printf("\nSystem Firmware Header - %d\n" ,header[0]);
#endif

            err =xmodemHTransfer( header );
            if(!err)
            {
                printf("Header Transfer complete\n");
                printf("Transferring System Firmware..");

                err =xmodemFTransfer( "com1", "default");
            }
        }
#endif
        return err;
    }
    else	/* Transfer Files to Flash Writer */
    {
        err = getMaxBaudRate( &supBaudRate );
        if(err)
        {
#ifndef WINDOWS
            //DrvUart_Close();
#endif
            printf( "File Transfer Failed!\n");
            return err;
        }

        printf("get max baud rate = %d!\n" , supBaudRate);

        /* Create Header Packet */
        memset (header, 0, sizeof(header));
        memcpy(header, &start, 1);
        memcpy(&header[1], &setBaudrate, 1);
        memcpy(&header[2], &supBaudRate, 1);

        err =xmodemHTransfer( header );

        if(err)
        {
            //DrvUart_Close();
            printf("File Transfer Failed!\n");
            return err;
        }

        /* Create Header Packet */
        memset (header, 0, sizeof(header));
        memcpy(header, &start, 1);
        memcpy(&header[1], &program, 1);
        memcpy(&header[2], &def_dtype, 1);
        memcpy(&header[3], &def_itype, 1);
        memcpy(&header[4], &offset, 4);

        printf("\nTransferring Header Information..\n");
        err =xmodemHTransfer( header );
        if(!err)
        {
            printf("Header Transfer Complete!\n");
            baudRate = standardBaudRate[supBaudRate];

            /* Todo: re-open with new baudrate */
            //DrvUart_Open(0x6B);

            goto FileTx;
        }
        else
        {
            if(!err)
                printf("File Transfer Complete!\n");
            else
                printf("File Transfer Failed!\n");

            return err;
        }
    }
}

int Init(void)
{
    return 0;
}

int ExecCommand( const char * commandName, const char** optionNames, const char** optionValues, const int optionLen )
{
    int retVal = 0;
    if(!strcmp(commandName,"FlashErase"))
        retVal = FlashErase( optionNames, optionValues, optionLen );
    else
        printf("Command not supported!\n");
    return retVal;
}

int Shutdown()
{
    return 0;
}

