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

/***********************************************************************
 *                       Preprocessor Directives                       *
 ***********************************************************************/
#define F_CPU 8000000

#include <avr/io.h>
#include "std_types.h"
#include "uart.h"
#include "external_eeprom.h"
#include "gpio.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "delay.h"

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

/* Start address of saved data in EEPROM. */
#define SAVED_PASSWORD_FLAG_ADDRESS   					 0x0000
#define PASSWORD_START_ADDRESS							 0x0001


/***********************************************************************
 *                         Functions Prototypes                        *
 ***********************************************************************/
/*
 * Description:
 * Checks the validity of password and re-entered password and save password if valid.
 */
void checkNewPassword( void );
/*
 * Description:
 * This function translates the sent command to the corresponding funcition.
 */
void performCommand( uint8 command );
/*
 * Description:
 * Saves the password in EEPROM and change the status of saved password to LOGIC_HIGH.
 */
void savePassword( uint8* password );
/*
 * Description:
 * Check the length of password and send result to HMI MCU.
 */
void checkPasswordLength( void );
/*
 * Description:
 * Checks the status of saved password flag and send the result to HMI MCU using UART.
 */
void checkSavedPassword( void );
/*
 * Description:
 * Erases the EEPROM password by erasing the flag that keeps status either there is a saved password or not.
 */
void eraseSavedPassword( void );
/*
 * Description:
 * This function is executed when the HMI controller sends CHECK_PASSWORD_WITH_SAVED_PASSWORD command.
 * It receives password using UART and compare it with saved password in EEPROM.
 */
void checkPassword( void );
/*
 * Description:
 * Turns the motor clock wise.
 */
void openDoor( void );
/*
 * Description:
 * Turns the motor anti clock wise.
 */
void closeDoor( void );
/*
 * Description:
 * Stops the motor.
 */
void stopDoor( void );


/***********************************************************************
 *                       Functions Implementation                      *
 ***********************************************************************/
int main ( void )
{
	/* Enable global interrupt. */
	SREG |= (1<<7);
	/* Delay to let UART in the other MCU to be initialized */
	EEPROM_init();
	delay_ms(1);
	UART_ConfigType config = {9600,BITS_8,NO_PARITY,ONE_STOP_BIT};
	UART_init(&config);
	DcMotor_Init();
	BUZZER_Init();

	/* Keep listening for HMI MCU requests. */
	while(1)
	{
		UART_sendByte(CONTROL_MCU_READY);
		uint8 command = UART_recieveByte();
		performCommand( command );
	}

	return 0;
}


/*
 * Description:
 * This function translates the sent command to the corresponding funcition.
 */
void performCommand( uint8 command )
{
	switch(command)
	{
	case CONTROL_COMPARE_TWO_PASSWORDS:
		checkNewPassword();
		break;
	case CONTROL_CHECK_PASSWORD_LENGTH:
		checkPasswordLength();
		break;
	case CONTROL_CHECK_SAVED_PASSWORD_FLAG:
		checkSavedPassword();
		break;
	case CONTROL_ERASE_SAVED_PASSWORD:
		eraseSavedPassword();
		break;
	case CHECK_PASSWORD_WITH_SAVED_PASSWORD:
		checkPassword();
		break;
	case CONTROL_MOTOR_ROTATE_CW:
		openDoor();
		break;
	case CONTROL_MOTOR_STOP:
		stopDoor();
		break;
	case CONTROL_MOTOR_ROTATE_CCW:
		closeDoor();
		break;
	case CONTROL_BUZZER_ON:
		BUZZER_On();
		break;
	case CONTROL_BUZZER_OFF:
		BUZZER_Off();
		break;
	default:
		break;
	}
}


/*
 * Description:
 * Checks the validity of password and re-entered password and save password if valid.
 */
void checkNewPassword( void )
{
	uint8 password[PASSWORD_LENGTH];
	uint8 reEnteredPassword[PASSWORD_LENGTH];

	UART_sendByte(CONTROL_MCU_READY);

	/* Receiving password */
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		password[i] = UART_recieveByte();
	}
	/* Receiving reentered password */
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		reEnteredPassword[i] = UART_recieveByte();
	}

	boolean result = COMPARE_RESULT_TRUE;
	/* Comparing passwords*/
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		if(password[i] != reEnteredPassword[i])
		{
			result = COMPARE_RESULT_FALSE;
			break;
		}
	}
	/* If result and re-enterred passwords are identical, save password to EEPROM. */
	if(result == COMPARE_RESULT_TRUE)
	{
		savePassword( password );
	}
	while(UART_recieveByte() != HMI_MCU_READY);
	UART_sendByte(result);
}


/*
 * Description:
 * Check the length of password and send result to HMI MCU.
 */
void checkPasswordLength( void )
{
	/* Get length of the entered password. */
	UART_sendByte(CONTROL_MCU_READY);
	uint8 length = UART_recieveByte();
	/* Send the result of comparison to HMI MCU to represent an error. */
	while(UART_recieveByte() != HMI_MCU_READY);
	UART_sendByte(length == PASSWORD_LENGTH);
}


/*
 * Description:
 * Saves the password in EEPROM and change the status of saved password to LOGIC_HIGH.
 */
void savePassword( uint8* const password )
{
	for(uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		EEPROM_writeByte(PASSWORD_START_ADDRESS + i, password[i]);
		delay_ms(10);
	}
	EEPROM_writeByte(SAVED_PASSWORD_FLAG_ADDRESS, LOGIC_HIGH);
	delay_ms(10);
}


/*
 * Description:
 * Checks the status of saved password flag and send the result to HMI MCU using UART.
 */
void checkSavedPassword( void )
{
	uint8 flag;
	EEPROM_readByte(SAVED_PASSWORD_FLAG_ADDRESS, &flag);
	while(UART_recieveByte() != HMI_MCU_READY);
	UART_sendByte(flag);
}


/*
 * Description:
 * Erases the EEPROM password by erasing the flag that keeps status either there is a saved password or not.
 */
void eraseSavedPassword( void )
{
	EEPROM_writeByte(SAVED_PASSWORD_FLAG_ADDRESS, LOGIC_LOW);
	delay_ms(10);
}


/*
 * Description:
 * This function is executed when the HMI controller sends CHECK_PASSWORD_WITH_SAVED_PASSWORD command.
 * It receives password using UART and compare it with saved password in EEPROM.
 */
void checkPassword( void )
{
	uint8 savedPassword[PASSWORD_LENGTH];
	uint8 enteredPassword[PASSWORD_LENGTH];
	UART_sendByte(CONTROL_MCU_READY);
	/* Receiving password */
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		enteredPassword[i] = UART_recieveByte();
	}
	/* Getting saved password from EEPROM */
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		EEPROM_readByte(PASSWORD_START_ADDRESS + i, savedPassword + i);
	}
	boolean result = COMPARE_RESULT_TRUE;
	/* Comparing passwords*/
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		if(enteredPassword[i] != savedPassword[i])
		{
			result = COMPARE_RESULT_FALSE;
			break;
		}
	}
	while(UART_recieveByte() != HMI_MCU_READY);
	UART_sendByte(result);
}


/*
 * Description:
 * Turns the motor clock wise.
 */
void openDoor( void )
{
	DcMotor_Rotate(CW);
}


/*
 * Description:
 * Turns the motor anti clock wise.
 */
void closeDoor( void )
{
	DcMotor_Rotate(A_CW);
}


/*
 * Description:
 * Stops the motor.
 */
void stopDoor( void )
{
	DcMotor_Rotate(STOP);
}

