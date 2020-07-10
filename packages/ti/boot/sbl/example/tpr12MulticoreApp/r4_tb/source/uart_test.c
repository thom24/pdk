#include "uart_test.h"
#include "uart.h"

uint8_t recvBuffer[100U] = {0U};
volatile int iDebug = 0U;

void uart_config ()
{
    DrvUart_Open (0x6B);
}

void send_hundred_bytes ()
{
    uint8_t txBuff[100U];
    uint32_t count;
    
    for (count = 0U; count < 100U; count++)
    {
        txBuff[count] = 0x61 + (count % 10U);
    }
    DrvUart_WriteBytes (txBuff, 100U);
}

void send_single_byte ()
{
    uint8_t txBuff[1U] = {0x41U};
    DrvUart_WriteBytes (txBuff, 1U);
}

void send_two_bytes ()
{
    send_single_byte ();
    send_single_byte ();
}

void receive_ten_bytes ()
{
    DrvUart_ReadBytes (recvBuffer, 10U);
}

void send_multiple_bytes_and_start_receiving ()
{
    uint32_t count;
    for(count=0; count < 16; count++)
    {   
        send_single_byte();
    }
    
    receive_ten_bytes ();
}

void uart_test ()
{
    while( iDebug == 0U);
    iDebug = 0U;
    uart_config ();
    
    send_single_byte ();
    
    while( iDebug == 0U);
    iDebug = 0U;
    
    send_hundred_bytes ();
    
    while( iDebug == 0U);
    iDebug = 0U;
    
    send_two_bytes ();
    
    while( iDebug == 0U);
    iDebug = 0U;
    
    send_multiple_bytes_and_start_receiving ();
    
    while( iDebug == 0U);
    iDebug = 0U; 
}
