/*
 *
 * Module: Door locking control application.
 *
 * File Name: door_locking_control.c
 *
 * Description: Source file of controlMCU application.
 *
 * Author: Mohamed Khaled.
 *
 */
#define F_CPU 8000000

#include "uart.h"
#include "gpio.h"
#include <util/delay.h>

#define CONTROL_MCU_READY 0x10
#define HMI_MCU_READY 0xff
#define COMPARE_RESULT_TRUE 0x01
#define COMPARE_RESULT_FALSE 0x00



int main ( void )
{
	/* Delay to let UART in the other MCU to be initialized */
	_delay_ms(1);
	UART_init(9600);

	uint8 password[5];
	uint8 reEnteredPassword[5];
	GPIO_setupPinDirection(PORTC_ID, PIN0_ID, PIN_OUTPUT);

	UART_sendByte(CONTROL_MCU_READY);
	/* Receiving password */
	for (uint8 i = 0; i < 5; i++)
	{
		password[i] = UART_recieveByte();
	}
	/* Receiving reentered password */
	for (uint8 i = 0; i < 5; i++)
	{
		reEnteredPassword[i] = UART_recieveByte();
	}

	boolean result = COMPARE_RESULT_TRUE;
	/* Comparing passwords*/
	for (uint8 i = 0; i < 5; i++)
	{
		if(password[i] != reEnteredPassword[i])
		{
			result = COMPARE_RESULT_FALSE;
			break;
		}
	}
	while(UART_recieveByte() != HMI_MCU_READY);
	UART_sendByte(result);

	while(1)
	{

	}


	return 0;
}
