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

#define CONTROL_COMPARE_TWO_PASSWORDS					 0x01
#define CONTROL_CHECK_PASSWORD_LENGTH  					 0x02
#define CONTROL_CHECK_SAVED_PASSWORD_FLAG 				 0x03
#define CONTROL_ERASE_SAVED_PASSWORD					 0x04
#define CHECK_PASSWORD_WITH_SAVED_PASSWORD				 0x05
#define CONTROL_MOTOR_ROTATE_CW							 0x06
#define CONTROL_MOTOR_STOP 								 0x07
#define CONTROL_MOTOR_ROTATE_CCW						 0x08

#define PASSWORD_LENGTH								   	5

#include <util/delay.h>
#include "lcd.h"
#include "gpio.h"
#include "keypad.h"
#include "uart.h"

typedef enum {
	PASSWORD_LENGTH_ERROR, PASSWORD_REENTERING_ERROR, PASSWORD_INCORRECT
} Error;


void requestNewPassword( void );
void displayError (Error error);
uint8 checkNewPassword( void );
void displayMainOptions( void );
void requestPassword( void );
uint8 checkPasswordLength(uint8 length);
uint8 checkTryingPassword( void );
void getPassword( uint8* pass, uint8* counter );

uint8 password[PASSWORD_LENGTH];
uint8 reEnteredPassword[PASSWORD_LENGTH];
uint8 tryingPassword[PASSWORD_LENGTH];

int main ( void )
{
	UART_ConfigType config = {9600,BITS_8,NO_PARITY,ONE_STOP_BIT};
	UART_init(&config);

	LCD_init();
//			while(UART_recieveByte() != CONTROL_MCU_READY);
//			UART_sendByte(CONTROL_ERASE_SAVED_PASSWORD);
	while(UART_recieveByte() != CONTROL_MCU_READY);
	UART_sendByte(CONTROL_CHECK_SAVED_PASSWORD_FLAG);
	UART_sendByte(HMI_MCU_READY);
	uint8 flag = UART_recieveByte();
	/* If there is no saved password, get one.*/
	if(flag == LOGIC_LOW)
	{
		do
		{
			requestNewPassword();
		}while(	checkNewPassword() == LOGIC_LOW);
		_delay_ms(1000);
	}


	/////////////////////////////////////////////////////////////////////
	//	for(uint8 i = 0; i < 5; i++)
	//	{
	//		LCD_intgerToString(password[i]);
	//	}
	//	for(uint8 i = 0; i < 5; i++)
	//	{
	//		LCD_intgerToString(reEnteredPassword[i]);
	//	}
	/////////////////////////////////////////////////////////////////////
	//	while(UART_recieveByte() != CONTROL_MCU_READY);
	//	UART_sendByte(CONTROL_ERASE_SAVED_PASSWORD);


	while(1)
	{
		displayMainOptions();
	}

	return 0;
}

void getPassword( uint8* pass, uint8* counter )
{
	uint8 key;

	LCD_moveCursor(1, 0);
	*counter = 0;

	while(1)
	{
		key = KEYPAD_getPressedKey();
		pass[*counter] = key;
		/*If user press enter -> end of edit.*/
		if(key == 13)
		{
			_delay_ms(400);
			break;
		}
		LCD_displayCharacter('*');
		_delay_ms(400);
		(*counter)++;
	}
}

void requestNewPassword( void )
{
	uint8 passLength;
	boolean errorFlag = LOGIC_HIGH;
	LCD_clearScreen();
	/* Enter password till pressing enter */
	while(errorFlag == LOGIC_HIGH)
	{
		errorFlag = LOGIC_LOW;
		/* Request new password from user and then press Enter key*/
		LCD_displayString("Enter new pass.:");
		getPassword(password, &passLength);
		uint8 result = checkPasswordLength( passLength );

		if(result == LOGIC_LOW)
		{
			errorFlag = LOGIC_HIGH;
			Error error = PASSWORD_LENGTH_ERROR;
			displayError(error);
		}
	}

	LCD_clearScreen();

	errorFlag = LOGIC_HIGH;
	/* Enter password till pressing enter */
	while(errorFlag == LOGIC_HIGH)
	{
		errorFlag = LOGIC_LOW;
		LCD_displayString("Re-enter pass.:");
		getPassword(reEnteredPassword, &passLength);

		uint8 result = checkPasswordLength( passLength );

		if(result == LOGIC_LOW)
		{
			errorFlag = LOGIC_HIGH;
			Error error = PASSWORD_LENGTH_ERROR;
			displayError(error);
		}
	}

}

void displayError( Error error )
{
	switch(error)
	{
	case PASSWORD_LENGTH_ERROR:
		LCD_clearScreen();
		LCD_moveCursor(0, 0);
		LCD_displayString("Password must be");
		LCD_moveCursor(1, 0);
		LCD_displayString("5 characters");
		_delay_ms(1000);
		LCD_clearScreen();
		break;
	case PASSWORD_REENTERING_ERROR:
		LCD_clearScreen();
		LCD_moveCursor(0, 6);
		LCD_displayString("Error");
		LCD_moveCursor(1, 3);
		LCD_displayString("Try Again");
		_delay_ms(1000);
		LCD_clearScreen();
		break;
	case PASSWORD_INCORRECT:
		LCD_clearScreen();
		LCD_moveCursor(0, 6);
		LCD_displayString("Error");
		LCD_moveCursor(1, 0);
		LCD_displayString("Incorrect Pass.");
		_delay_ms(1000);
		LCD_clearScreen();
		break;
	}
}

uint8 checkNewPassword( void )
{
	while(UART_recieveByte() != CONTROL_MCU_READY);
	UART_sendByte(CONTROL_COMPARE_TWO_PASSWORDS);

	while(UART_recieveByte() != CONTROL_MCU_READY);
	/* Sending password */
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		UART_sendByte(password[i]);
	}
	/* Sending reentered password */
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		UART_sendByte(reEnteredPassword[i]);
	}

	UART_sendByte(HMI_MCU_READY);
	uint8 compareResult = UART_recieveByte();

	if(compareResult)
	{
		LCD_clearScreen();
		LCD_moveCursor(0, 6);
		LCD_displayString("Match");
	}
	else
	{
		LCD_clearScreen();
		LCD_moveCursor(0, 4);
		LCD_displayString("Mis Match");
	}
	_delay_ms(1000);
	return compareResult;
}

void displayMainOptions( void )
{
	LCD_clearScreen();
	LCD_displayString("+ : Open Door.");
	LCD_moveCursor(1, 0);
	LCD_displayString("- : Change Pass.");
	uint8 key = KEYPAD_getPressedKey();
	_delay_ms(400);
	switch(key)
	{
	case '+':
		requestPassword();
		while(!checkTryingPassword())
		{
			Error error = PASSWORD_INCORRECT;
			displayError(error);
			requestPassword();
		}
		LCD_clearScreen();
		LCD_displayString("Openning");
		while(UART_recieveByte() != CONTROL_MCU_READY);
		UART_sendByte(CONTROL_MOTOR_ROTATE_CW);
		_delay_ms(1000);
		LCD_clearScreen();
		while(UART_recieveByte() != CONTROL_MCU_READY);
		UART_sendByte(CONTROL_MOTOR_STOP);
		_delay_ms(500);
		LCD_displayString("Closing");
		UART_sendByte(CONTROL_MOTOR_ROTATE_CCW);
		while(UART_recieveByte() != CONTROL_MCU_READY);
		_delay_ms(1000);
		LCD_clearScreen();
		while(UART_recieveByte() != CONTROL_MCU_READY);
		UART_sendByte(CONTROL_MOTOR_STOP);
		break;
	case '-':
		requestPassword();
		while(!checkTryingPassword())
		{
			Error error = PASSWORD_INCORRECT;
			displayError(error);
			requestPassword();
		}
		requestNewPassword();
		while(!checkNewPassword())
		{
			Error error = PASSWORD_REENTERING_ERROR;
			displayError(error);
			requestNewPassword();
		}
		break;
	default:
		break;
	}
}

void requestPassword( void )
{
	uint8 passLength;
	boolean errorFlag = LOGIC_HIGH;
	LCD_clearScreen();
	/* Enter password till pressing enter */
	while(errorFlag == LOGIC_HIGH)
	{
		errorFlag = LOGIC_LOW;
		/* Request password from user and then press Enter key*/
		LCD_displayString("Enter pass.:");
		getPassword(tryingPassword, &passLength);
		uint8 result = checkPasswordLength( passLength );

		if(result == LOGIC_LOW)
		{
			errorFlag = LOGIC_HIGH;
			Error error = PASSWORD_LENGTH_ERROR;
			displayError(error);
		}
	}
}

uint8 checkPasswordLength(uint8 length)
{
	/* Display error if password is not 5 characters. */
	while(UART_recieveByte() != CONTROL_MCU_READY);
	UART_sendByte(CONTROL_CHECK_PASSWORD_LENGTH);

	while(UART_recieveByte() != CONTROL_MCU_READY);
	UART_sendByte(length);

	UART_sendByte(HMI_MCU_READY);
	return UART_recieveByte();
}

uint8 checkTryingPassword( void )
{
	while(UART_recieveByte() != CONTROL_MCU_READY);
	UART_sendByte(CHECK_PASSWORD_WITH_SAVED_PASSWORD);

	while(UART_recieveByte() != CONTROL_MCU_READY);
	/* Sending password */
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		UART_sendByte(tryingPassword[i]);
	}

	UART_sendByte(HMI_MCU_READY);
	return UART_recieveByte();
}
