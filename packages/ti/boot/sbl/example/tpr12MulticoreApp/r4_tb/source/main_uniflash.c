/**
 * \file  main.cpp
 *
 * \brief Main Function
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

/**
 * Display Help
 *
 * \param None
 *
 * This function displays help menu and exits the program.
 *
 * \return None
 *
 */

void displayHelp( void )
{
    printf("\nUsage:");
#if 0
    
    printf("dslite.bat --mode processors -c <COM_Port> -f <Path_to_the_file_to_be_transfered> -d <Device_Type> -i <Image_Type> -e <erase_length -o <Offset>\
        \nDevice_Type:\n0 - NAND\n1 - SPI\n2 - QSPI\n3 - OSPI\n4 - eMMC\n5 - HyperFlash\n6 - UFS\
        \nImage_Type:\n0 - Flash\n1 - MLO\n2 - Uboot\n3 - UImage\n4 - Firmware\n5 - Custom Image\
        \nerase_length:Length in Bytes\
        \nNote: File Path should not be specified in case of Flash Erase");
#endif

}

/**
 * Display Version
 *
 * \param None
 *
 * This function displays the version number of the Uniflash Tool.
 *
 * \return None
 *
 */
void displayVersion( void )
{
#if 0
    printf("\n----------------------------------------------------------------------------");
    printf("\nProcessorSDKSerialFlash CLI Tool");
    printf("\nCopyright (C) 2017-2019 Texas Instruments Incorporated - http://www.ti.com/");
    printf("\nVersion %s" , VERSION);
    printf("\n----------------------------------------------------------------------------");
#endif
}


int flashEraseProcess(unsigned int flashOffset, int eraseLen)
{
    int argc = 0;
    int retVal = 0;
    const char *eraseCommand = "FlashErase";
    char    cmdOpt[8][4];
    char    cmdValue[8][16];
    char    *ptrCmdArgv[16];

    /* -c com1 */
    sprintf(&cmdOpt[0][0], "%s", "-c");
    sprintf(&cmdValue[0][0], "%s", "com1");
    ptrCmdArgv[1] = &cmdOpt[0][0];
    ptrCmdArgv[2] = &cmdValue[0][0];

    /* -e len */
    sprintf(&cmdOpt[1][0], "%s", "-e");
    sprintf(&cmdValue[1][0], "%x", eraseLen);
    ptrCmdArgv[3] = &cmdOpt[1][0];
    ptrCmdArgv[4] = &cmdValue[1][0];

    /* -o  offset */
    sprintf(&cmdOpt[2][0], "%s", "-o");
    sprintf(&cmdValue[2][0], "%x", flashOffset);
    ptrCmdArgv[5] = &cmdOpt[2][0];
    ptrCmdArgv[6] = &cmdValue[2][0];

    argc = 7;

    printf("erase : %s %s, %s %s\n", cmdOpt[1], cmdValue[1], cmdOpt[2], cmdValue[2]);

    retVal = ExecCommand( eraseCommand, (const char **)&ptrCmdArgv[1], (const char **)&ptrCmdArgv[2], argc );

    return retVal;
}

int flashImageProcess(unsigned int  flashOffset)
{
    int argc = 0;
    int retVal = 0;
    char    cmdOpt[8][4];
    char    cmdValue[8][16];
    char    *ptrCmdArgv[16];

    /* -c com1 */
    sprintf(&cmdOpt[0][0], "%s", "-c");
    sprintf(&cmdValue[0][0], "%s", "com1");
    ptrCmdArgv[1] = &cmdOpt[0][0];
    ptrCmdArgv[2] = &cmdValue[0][0];

    /* -f fname */
    sprintf(&cmdOpt[1][0], "%s", "-f");
    sprintf(&cmdValue[1][0], "%s", "sbl.bin");
    ptrCmdArgv[3] = &cmdOpt[1][0];
    ptrCmdArgv[4] = &cmdValue[1][0];

    /* -d  device -qspi=2 */
    sprintf(&cmdOpt[2][0], "%s", "-d");
    sprintf(&cmdValue[2][0], "%d", 2);
    ptrCmdArgv[5] = &cmdOpt[2][0];
    ptrCmdArgv[6] = &cmdValue[2][0];

    /* -i  image uboot=2 */
    sprintf(&cmdOpt[3][0], "%s", "-i");
    sprintf(&cmdValue[3][0], "%d", 2);
    ptrCmdArgv[7] = &cmdOpt[3][0];
    ptrCmdArgv[8] = &cmdValue[3][0];

    /* -o  offset */
    sprintf(&cmdOpt[4][0], "%s", "-o");
    sprintf(&cmdValue[4][0], "%x", flashOffset);
    ptrCmdArgv[9] = &cmdOpt[4][0];
    ptrCmdArgv[10] = &cmdValue[4][0];

    argc = 11;

    printf("load : %s %s, %s %s, %s %s, %s %s\n", cmdOpt[1], cmdValue[1], cmdOpt[2], cmdValue[2],
                                cmdOpt[3], cmdValue[3], cmdOpt[4], cmdValue[4]);

    retVal = LoadImage( ptrCmdArgv[4], (const char **)&ptrCmdArgv[1], (const char **)&ptrCmdArgv[2], argc );

    return retVal;
}

/**
 * MAIN
 *
 * \param argc points the number of command line arguments
 * \param argv points to the command line arguments
 *
 * This is the main function
 *
 * \return Returns 0 on success and error value on failure.
 *
 */
int main(void)
{
    int     testSelection;
    int     retVal = 0;
    int     imageLen;
    int     imageOffset;

    /* Get length and offset from predefined location */
    imageOffset = *(int *)TB_FLASHIMAGE_OFFSET_ADDR;
    imageLen = *(int *)TB_FLASHIMAGE_LENGTH_ADDR;

    while(1)
    {
        /* Command Init */
        printf("Please select the type of test to execute:\n");
        printf("1. Flash erase \n");
        printf("2. Load image \n");
        printf(">Enter your selection:\n");

        scanf("%d", &testSelection);
        switch(testSelection)
        {
            case 1:
            /* Prepare for commands */
                retVal = flashEraseProcess(imageOffset, imageLen);
                if(retVal != 0)
                {
                    printf("Flash erase failed!\n");
                }

                break;

            case 2:
                /* -c <COM_Port> -f <Path_to_the_file_to_be_transfered> -d <Device_Type> -i <Image_Type> -e <erase_length -o <Offset>\ */

                retVal = flashImageProcess(imageOffset);
                if(retVal != 0)
                {
                    printf("Flash Image failed!\n");
                }

                break;

            case 0:
            default:
                displayVersion();
                printf("\nDisplaying Help..\n");
                displayHelp();
                break;
        }
    }

}

