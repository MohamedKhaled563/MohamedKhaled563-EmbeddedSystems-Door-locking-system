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

/***********************************************************************
 *                       Preprocessor Directives                       *
 ***********************************************************************/
#define F_CPU 8000000

#include <avr/io.h>
#include "delay.h"
#include "lcd.h"
#include "gpio.h"
#include "keypad.h"
#include "uart.h"

/* This messages are used for MCUs hand shaking. */
#define CONTROL_MCU_READY 								 0x10
#define HMI_MCU_READY									 0xff

/* Commands that can be handled by control MCU. */
#define CONTROL_COMPARE_TWO_PASSWORDS					 0x01
#define CONTROL_CHECK_PASSWORD_LENGTH  					 0x02
#define CONTROL_CHECK_SAVED_PASSWORD_FLAG 				 0x03
#define CONTROL_ERASE_SAVED_PASSWORD					 0x04
#define CHECK_PASSWORD_WITH_SAVED_PASSWORD				 0x05
#define CONTROL_MOTOR_ROTATE_CW							 0x06
#define CONTROL_MOTOR_STOP 								 0x07
#define CONTROL_MOTOR_ROTATE_CCW						 0x08
#define CONTROL_BUZZER_ON 								 0X09
#define CONTROL_BUZZER_OFF								 0X0A

#define COMPARE_RESULT_TRUE 0x01
#define COMPARE_RESULT_FALSE 0x00
#define PASSWORD_LENGTH								   	5

/***********************************************************************
 *                          User Defined Types                         *
 ***********************************************************************/
/* All types of errors that may occur in the application.*/
typedef enum {
	PASSWORD_LENGTH_ERROR, PASSWORD_REENTERING_ERROR, PASSWORD_INCORRECT, PASSWORD_INCORRECT_THREE_TIMES
} Error;

/***********************************************************************
 *                         Functions Prototypes                        *
 ***********************************************************************/
/*
 * Description:
 * Asks the user to enter new password.
 */
void requestNewPassword( void );
/*
 * Description:
 * Displays different types of errors in the system.
 */
void displayError (Error error);
/*
 * Description:
 * Sends commands to control MCU to compare the entered two passwords.
 */
uint8 checkNewPassword( void );
/*
 * Description:
 * Displays main screen on LCD.
 */
void displayMainOptions( void );
/*
 * Description:
 * Displays requesting password screen on LCD.
 */
void requestPassword( void );
/*
 * Description:
 * Checks length of password by sending "CONTROL_CHECK_PASSWORD_LENGTH" to controlling MCU and then
 * sending the password.
 */
uint8 checkPasswordLength(uint8 length);
/*
 * Description:
 * Checks entered password with saved password on EEPROM by sending "CHECK_PASSWORD_WITH_SAVED_PASSWORD" to control MCU and then
 * sending the password.
 */
uint8 checkTryingPassword( void );
/*
 * Description:
 * Requests password from user and displays '*' in LCD instead of real characters.
 */
void getPassword( uint8* pass, uint8* counter );

uint8 password[PASSWORD_LENGTH];
uint8 reEnteredPassword[PASSWORD_LENGTH];
uint8 tryingPassword[PASSWORD_LENGTH];


/***********************************************************************
 *                       Functions Implementation                      *
 ***********************************************************************/
int main ( void )
{
	/* Enable global interrupt. */
	SREG |= (1<<7);
	UART_ConfigType config = {9600,BITS_8,NO_PARITY,ONE_STOP_BIT};
	UART_init(&config);
	LCD_init();
	/*************************** UNCOMMENT the next two lines to make hard reset and set new password ***************************/
	/*
	while(UART_recieveByte() != CONTROL_MCU_READY);
	UART_sendByte(CONTROL_ERASE_SAVED_PASSWORD);
	*/
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
		delay_ms(1000);
	}

	while(1)
	{
		displayMainOptions();
	}

	return 0;
}


/*
 * Description:
 * Requests password from user and displays '*' in LCD instead of real characters.
 */
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
			delay_ms(400);
			break;
		}
		LCD_displayCharacter('*');
		delay_ms(400);
		(*counter)++;
	}
}


/*
 * Description:
 * Asks the user to enter new password.
 */
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


/*
 * Description:
 * Displays different types of errors in the system.
 */
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
		delay_ms(1000);
		LCD_clearScreen();
		break;
	case PASSWORD_REENTERING_ERROR:
		LCD_clearScreen();
		LCD_moveCursor(0, 6);
		LCD_displayString("Error");
		LCD_moveCursor(1, 3);
		LCD_displayString("Try Again");
		delay_ms(1000);
		LCD_clearScreen();
		break;
	case PASSWORD_INCORRECT:
		LCD_clearScreen();
		LCD_moveCursor(0, 6);
		LCD_displayString("Error");
		LCD_moveCursor(1, 0);
		LCD_displayString("Incorrect Pass.");
		delay_ms(1000);
		LCD_clearScreen();
		break;
	case PASSWORD_INCORRECT_THREE_TIMES:
		LCD_clearScreen();
		LCD_moveCursor(0, 4);
		LCD_displayString("Warning");
		LCD_moveCursor(1, 5);
		LCD_displayString("Thief");
		delay_ms(1000);
		LCD_clearScreen();
		break;
	}
}


/*
 * Description:
 * Sends commands to control MCU to compare the entered two passwords.
 */
uint8 checkNewPassword( void )
{
	/* Sending checking password command to control MCU after getting acknoledgement.*/
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
	delay_ms(1000);
	return compareResult;
}

/*
 * Description:
 * Displays main screen on LCD.
 */
void displayMainOptions( void )
{
	LCD_clearScreen();
	LCD_displayString("+ : Open Door.");
	LCD_moveCursor(1, 0);
	LCD_displayString("- : Change Pass.");
	uint8 key = KEYPAD_getPressedKey();
	delay_ms(400);
	uint8 trials = 0;
	switch(key)
	{
	case '+':
		trials = 0;
		requestPassword();
		Error error;
		while(!checkTryingPassword())
		{
			error = PASSWORD_INCORRECT;
			displayError(error);
			requestPassword();
			trials++;
			if(trials >= 3)
			{
				/* This is a thief. */
				while(UART_recieveByte() != CONTROL_MCU_READY);
				UART_sendByte(CONTROL_BUZZER_ON);
				error = PASSWORD_INCORRECT_THREE_TIMES;
				displayError(error);
				while(UART_recieveByte() != CONTROL_MCU_READY);
				UART_sendByte(CONTROL_BUZZER_OFF);
				trials = 0;
			}
		}
		LCD_clearScreen();
		LCD_displayString("Openning");
		while(UART_recieveByte() != CONTROL_MCU_READY);
		UART_sendByte(CONTROL_MOTOR_ROTATE_CW);
		delay_ms(1000);
		LCD_clearScreen();
		while(UART_recieveByte() != CONTROL_MCU_READY);
		UART_sendByte(CONTROL_MOTOR_STOP);
		delay_ms(500);
		LCD_displayString("Closing");
		UART_sendByte(CONTROL_MOTOR_ROTATE_CCW);
		while(UART_recieveByte() != CONTROL_MCU_READY);
		delay_ms(1000);
		LCD_clearScreen();
		while(UART_recieveByte() != CONTROL_MCU_READY);
		UART_sendByte(CONTROL_MOTOR_STOP);
		break;
	case '-':
		trials = 0;
		requestPassword();
		while(!checkTryingPassword())
		{
			error = PASSWORD_INCORRECT;
			displayError(error);
			requestPassword();
			trials++;
			if(trials >= 3)
			{
				/* This is a thief. */
				while(UART_recieveByte() != CONTROL_MCU_READY);
				UART_sendByte(CONTROL_BUZZER_ON);
				error = PASSWORD_INCORRECT_THREE_TIMES;
				displayError(error);
				while(UART_recieveByte() != CONTROL_MCU_READY);
				UART_sendByte(CONTROL_BUZZER_OFF);
				trials = 0;
			}
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


/*
 * Description:
 * Displays requesting password screen on LCD.
 */
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


/*
 * Description:
 * Checks length of password by sending "CONTROL_CHECK_PASSWORD_LENGTH" to controlling MCU and then
 * sending the password.
 */
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


/*
 * Description:
 * Checks entered password with saved password on EEPROM by sending "CHECK_PASSWORD_WITH_SAVED_PASSWORD" to control MCU and then
 * sending the password.
 */
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
