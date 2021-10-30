 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: 	 Khaled.
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                               User Defined Types                            *
 *******************************************************************************/
/*
 * Description:
 * Enum for selecting number of stop bits in UART frame.
 */
typedef enum
{
	ONE_STOP_BIT, TWO_STOP_BITS
} UART_StopBits;
/*
 * Description:
 * Enum for selecting type of parity in UART frame.
 */
typedef enum
{
	NO_PARITY, EVEN_PARITY = 2, ODD_PARITY
} UART_ParityType;
/*
 * Description:
 * Specify the number of bits required to be transmitted in one frame.
 */
typedef enum
{
	BITS_5, BITS_6, BITS_7, BITS_8, BITS_9
} UART_BitLength;
/*
 * Description:
 * Configuration structure of UART module to select:
 *  - Number of stop bits
 *  - Type of parity check
 *  - Bit rate
 *  - Length of data in UART frame.
 */
typedef struct
{
	uint16 bitRate;
	UART_BitLength dataLength; /*5, 6, 7, 8 or 9 bits*/
	UART_ParityType parity;
	UART_StopBits stopBits;
} UART_ConfigType;

/*******************************************************************************
 *                             Functions Prototypes                            *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init( UART_ConfigType* config );

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
