/*
 * UART TEST Header File
 *
 * This is the header file which exposes the API to test UART
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */
#ifndef UART_TEST_H
#define UART_TEST_H

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
/* Standard Include Files. */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void uart_config ();
extern void send_hundred_bytes ();
extern void send_single_byte ();
extern void send_two_bytes ();
extern void receive_ten_bytes ();
extern void send_multiple_bytes_and_start_receiving ();
extern void uart_test ();

#ifdef __cplusplus
}
#endif

#endif /* UART_TEST_H */
