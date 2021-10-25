/*
 *
 * Module: Door locking hmi application.
 *
 * File Name: door_locking_hmi.c
 *
 * Description: Source file of hmiMCU application.
 *
 * Author: Mohamed Khaled.
 *
 */
#define F_CPU 8000000
#define CONTROL_MCU_READY 0x10
#define HMI_MCU_READY 0xff
#define COMPARE_RESULT_TRUE 0x01
#define COMPARE_RESULT_FALSE 0x00

#include <util/delay.h>
#include "lcd.h"
#include "gpio.h"
#include "keypad.h"
#include "uart.h"

typedef enum {
	PASS_LENGTH_ERROR, PASS_REENTERING_ERROR
} Error;


void requestNewPassword( uint8* password, uint8* reEnteredPassword );
void displayError (Error error);


int main ( void )
{
	UART_init(9600);

	LCD_init();
	uint8 password[5];
	uint8 reEnteredPassword[5];

	requestNewPassword(password, reEnteredPassword);
	/////////////////////////////////////////////////////////////////////
	for(uint8 i = 0; i < 5; i++)
	{
		LCD_intgerToString(password[i]);
	}
	for(uint8 i = 0; i < 5; i++)
	{
		LCD_intgerToString(reEnteredPassword[i]);
	}
	/////////////////////////////////////////////////////////////////////


	_delay_ms(1000);
	while(UART_recieveByte() != CONTROL_MCU_READY);
	/* Sending password */
	for (uint8 i = 0; i < 5; i++)
	{
		UART_sendByte(password[i]);
	}
	/* Sending reentered password */
	for (uint8 i = 0; i < 5; i++)
	{
		UART_sendByte(reEnteredPassword[i]);
	}

	UART_sendByte(HMI_MCU_READY);
	uint8 compareResult = UART_recieveByte();

	if(compareResult)
	{
		LCD_clearScreen();
		LCD_moveCursor(0, 0);
		LCD_displayString("Match");
	}
	else
	{
		LCD_clearScreen();
		LCD_moveCursor(0, 0);
		LCD_displayString("Mis Match");
	}

	while(1)
	{

	}

	return 0;
}



void requestNewPassword( uint8* password, uint8* reEnteredPassword )
{
	uint8 i = 0;
	uint8 key;
	boolean errorFlag = LOGIC_HIGH;
	/* Enter password till pressing '=' */
	while(errorFlag == LOGIC_HIGH)
	{
		i = 0;
		/* Request new password from user and then press '=' key*/
		LCD_displayString("Enter new pass.:");
		LCD_moveCursor(1, 0);
		errorFlag = LOGIC_LOW;
		while(1)
		{
			key = KEYPAD_getPressedKey();
			password[i] = key;
			/*If user press '=' -> end of edit.*/
			if(key == '=')
			{
				_delay_ms(300);
				break;
			}
			LCD_displayCharacter('*');
			_delay_ms(300);
			i++;
		}
		/* Display error if password is not 5 characters. */
		if(i != 5)
		{
			errorFlag = LOGIC_HIGH;
			Error error = PASS_LENGTH_ERROR;
			displayError(error);
		}
	}

	LCD_clearScreen();

	i = 0;
	errorFlag = LOGIC_HIGH;
	/* Enter password till pressing '=' */
	while(errorFlag == LOGIC_HIGH)
	{
		i = 0;
		LCD_displayString("Re-enter pass.:");
		LCD_moveCursor(1, 0);
		errorFlag = LOGIC_LOW;
		while(1)
		{
			key = KEYPAD_getPressedKey();
			reEnteredPassword[i] = key;
			/*If user press '=' -> end of edit.*/
			if(key == '=')
			{
				_delay_ms(300);
				break;
			}
			LCD_displayCharacter('*');
			_delay_ms(300);
			i++;
		}
		/* Display error if password is not 5 characters. */
		if(i != 5)
		{
			errorFlag = LOGIC_HIGH;
			Error error = PASS_LENGTH_ERROR;
			displayError(error);
		}
	}
}

void displayError( Error error )
{
	switch(error)
	{
	case PASS_LENGTH_ERROR:
		LCD_clearScreen();
		LCD_moveCursor(0, 0);
		LCD_displayString("Password must be");
		LCD_moveCursor(1, 0);
		LCD_displayString("5 characters");
		_delay_ms(1000);
		LCD_clearScreen();
		break;
	case PASS_REENTERING_ERROR:
		LCD_clearScreen();
		LCD_moveCursor(0, 6);
		LCD_displayString("Error");
		LCD_moveCursor(1, 3);
		LCD_displayString("Try Again");
		_delay_ms(1000);
		LCD_clearScreen();
		break;
	}
}